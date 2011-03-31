/*
 * Wars: Commando
 * Copyright (C) 2011 Teemu Erkkola
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "game.h"
#include "player.h"
#include "grenade.h"
#include "rocket.h"

#include <QFile>
#include <QTextStream>

Game::Game(QString const& levelFilename, QObject *parent) :
    QObject(parent), scene(new QGraphicsScene(this)),
    updatables(), actors(), particles(), hearts(), grenades(), rockets(), player(0), level(0), music(0), winSound(0), failSound(0), resetTimer(0), winTimer(0), resources(), paused(false)
{
    resources.addImage("player", ":/img/player.png");
    resources.addImage("bullet", ":/img/bullet.png");
    resources.addImage("grenade", ":/img/grenade.png");
    resources.addImage("rocket", ":/img/rocket.png");
    resources.addImage("heart", ":/img/heart.png");
    resources.addImage("autoMachineGun", ":/img/machineGun.png");
    resources.addImage("smallCannonBall", ":/img/smallCannonBall.png");
    resources.addImage("mediumCannonBall", ":/img/mediumCannonBall.png");
    resources.addImage("largeCannonBall", ":/img/largeCannonBall.png");
    resources.addImage("enemyInfantryWalker", ":/img/enemyInfantryWalker.png");
    resources.addImage("enemyInfantryShooter", ":/img/enemyInfantryShooter.png");
    resources.addImage("enemyInfantryBazooka", ":/img/enemyInfantryBazooka.png");
    resources.addImage("enemyLightTank", ":/img/lightTank.png");
    resources.addImage("enemyMediumTank", ":/img/mediumTank.png");
    resources.addImage("enemyHeavyTank", ":/img/heavyTank.png");

    resources.addImage("grenadeIcon", ":/img/grenade_icon.png");
    resources.addImage("rocketIcon", ":/img/rocket_icon.png");
    resources.addImage("grenadeItem", ":/img/grenadeItem.png");
    resources.addImage("rocketItem", ":/img/rocketItem.png");
    resources.addImage("heartItem", ":/img/heartItem.png");
    resources.addImage("bossEmblem", ":/img/bossEmblem.png");

    resources.addImage("exitToTitle", ":/img/exitToTitle.png");

    resources.addSound("shot", "sfx/shot.wav");
    resources.addSound("hit", "sfx/hit.wav");
    resources.addSound("die", "sfx/die.wav");
    resources.addSound("jump", "sfx/jump.wav");
    resources.addSound("grenade", "sfx/grenade.wav");
    resources.addSound("bazooka", "sfx/bazooka.wav");
    resources.addSound("pickup", "sfx/pickup.wav");
    resources.addSound("cannon", "sfx/cannon.wav");
    resources.addSound("win", "sfx/win.wav");
    resources.addSound("fail", "sfx/fail.wav");

    winSound = resources.getSound("win");
    failSound = resources.getSound("fail");
    loadLevel(levelFilename);
}

Game::~Game()
{
    delete level;
}

void Game::enter()
{
    if(music)
    {
        music->play();
    }
}

void Game::exit()
{
    if(music)
    {
        music->stop();
    }
}

int Game::levelWidth()
{
    if(level)
    {
        return level->getWidth();
    }
    else
    {
        return 0;
    }
}

int Game::levelHeight()
{
    if(level)
    {
        return level->getHeight();
    }
    else
    {
        return 0;
    }
}

bool Game::isPaused()
{
    return paused;
}

void Game::loadLevel(const QString &levelFilename, bool emptyInventory)
{
    int grenadesLeft = numGrenades();
    int rocketsLeft = numRockets();
    int heartsLeft = numHearts();
    if(heartsLeft < DEFAULT_HEARTS)
    {
        heartsLeft = DEFAULT_HEARTS;
    }
    scene->clear();
    updatables.clear();
    actors.clear();
    particles.clear();
    hearts.clear();
    grenades.clear();
    rockets.clear();

    exitToTitleText = scene->addPixmap(resources.getImage("exitToTitle"));
    exitToTitleText->setZValue(100);
    exitToTitleText->hide();

    if(music)
    {
        delete music;
        music = 0;
    }

    if(level)
    {
        delete level;
        level = 0;
    }

    player = 0;

    QFile levelFile(levelFilename);
    levelFile.open(QIODevice::ReadOnly);
    QTextStream levelStream(&levelFile);

    QString mapFilename;
    QString musicFilename;
    QString blockFilename;
    QString backgroundColor;

    while(!levelStream.atEnd())
    {
        QString propertyName;
        levelStream >> propertyName;

        if(propertyName == "map")
        {
            levelStream >> mapFilename;
        }
        else if(propertyName == "music")
        {
            levelStream >> musicFilename;
        }
        else if(propertyName == "blocks")
        {
            levelStream >> blockFilename;
        }
        else if(propertyName == "background")
        {
            levelStream >> backgroundColor;
        }
    }

    level = new Level(mapFilename, blockFilename, this);
    music = new Music(musicFilename, this);
    scene->setSceneRect(0, -5*48, level->getWidth()*48, (level->getHeight() + 5) * 48);
    scene->setBackgroundBrush(QBrush(QColor(backgroundColor)));

    addHearts(heartsLeft);
    if(!emptyInventory)
    {
        addGrenades(grenadesLeft);
        addRockets(rocketsLeft);
    }
}

void Game::updateGame()
{
    if(paused)
    {
       return;
    }

    if(winTimer)
    {
        ++winTimer;
        if(winTimer >= WIN_DELAY)
        {
            winTimer = 0;
            emit win();
        }
    }

    else
    {
        for(QSet<Updatable*>::iterator i = updatables.begin(); i != updatables.end(); ++i)
        {
            (*i)->update();
        }

        QList<Particle*> particlesToRemove;
        for(QSet<Particle*>::iterator i = particles.begin(); i != particles.end(); ++i)
        {
            if(!(*i)->alive())
            {
                particlesToRemove.append(*i);
            }
        }

        for(QList<Particle*>::iterator i = particlesToRemove.begin(); i < particlesToRemove.end(); ++i)
        {
            removeParticle(*i);
        }

        QList<Actor*> enemiesToRemove;
        for(QSet<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i)
        {
            if(!(*i)->alive())
            {
                enemiesToRemove.append(*i);
            }
        }

        for(QList<Actor*>::iterator i = enemiesToRemove.begin(); i < enemiesToRemove.end(); ++i)
        {
            removeActor(*i);
        }

        if(resetTimer)
        {
            ++resetTimer;
            if(resetTimer >= RESET_DELAY)
            {
                resetTimer = 0;
                removeGrenades(MAX_GRENADES);
                removeRockets(MAX_ROCKETS);
                emit die();
            }
        }
        else if(!alive())
        {
            resetTimer = 1;
            failSound->play();
            music->stop();
        }
    }
}

void Game::pause()
{
    paused = true;
    exitToTitleText->show();
}

void Game::resume()
{
    paused = false;
    exitToTitleText->hide();
}

QGraphicsScene* Game::getScene()
{
    return scene;
}

Player* Game::getPlayer()
{
    return player;
}

Level* Game::getLevel()
{
    return level;
}

Actor* Game::getEnemyAt(QPointF point)
{
    for(QSet<Actor*>::iterator i = actors.begin(); i != actors.end(); ++i)
    {
        if((*i)->isHitting(point))
        {
            return *i;
        }
    }

    return 0;
}

void Game::setPlayer(Player* newPlayer)
{
    player = newPlayer;
    addUpdatable(player);
}

void Game::addActor(Actor* newActor)
{
    actors.insert(newActor);
    addUpdatable(newActor);
}

void Game::removeActor(Actor* actor)
{
    bool boss = actor->isBoss();
    actors.remove(actor);
    updatables.remove(actor);
    scene->removeItem(actor);
    delete actor;

    if(boss)
    {
        winTimer = 1;
        winSound->play();
        music->stop();

    }
}

void Game::addUpdatable(Updatable* updatable)
{
    updatables.insert(updatable);
}

void Game::addParticle(Particle* particle)
{
    particles.insert(particle);
    addUpdatable(particle);
}

void Game::removeParticle(Particle* particle)
{
    updatables.remove(particle);
    particles.remove(particle);
    scene->removeItem(particle);
    delete particle;
}

ResourceManager const& Game::getResourceManager()
{
    return resources;
}

bool Game::alive()
{
    return hearts.size() > 0;
}

int Game::numHearts()
{
    return hearts.size();
}

void Game::addHearts(int amount)
{
    for(int i = 0; i < amount && hearts.size() < MAX_HEARTS; ++i)
    {
        QGraphicsPixmapItem* heart= new QGraphicsPixmapItem(resources.getImage("heart"), 0, scene);
        heart->setZValue(100);
        hearts.append(heart);
    }
}

void Game::removeHearts(int amount)
{
    for(int i = 0; i < amount && hearts.size() > 0; ++i)
    {
        QGraphicsPixmapItem* heart = hearts.at(hearts.size() - 1);
        scene->removeItem(heart);
        delete heart;
        hearts.pop_back();
    }

    if(hearts.size() == 0)
    {
        player->die();
    }
}

int Game::numGrenades()
{
    return grenades.size();
}

void Game::addGrenades(int amount)
{
    for(int i = 0; i < amount && grenades.size() < MAX_GRENADES; ++i)
    {
        QGraphicsPixmapItem* grenade = new QGraphicsPixmapItem(resources.getImage("grenadeIcon"), 0, scene);
        grenade->setZValue(100);
        grenades.append(grenade);
    }
}

void Game::removeGrenades(int amount)
{
    for(int i = 0; i < amount && grenades.size() > 0; ++i)
    {
        QGraphicsPixmapItem* grenade = grenades.at(grenades.size() - 1);
        scene->removeItem(grenade);
        delete grenade;
        grenades.pop_back();
    }
}

int Game::numRockets()
{
    return rockets.size();
}

void Game::addRockets(int amount)
{
    for(int i = 0; i < amount && rockets.size() < MAX_ROCKETS; ++i)
    {
        QGraphicsPixmapItem* rocket = new QGraphicsPixmapItem(resources.getImage("rocketIcon"), 0, scene);
        rocket->setZValue(100);
        rockets.append(rocket);
    }
}

void Game::removeRockets(int amount)
{
    for(int i = 0; i < amount && rockets.size() > 0; ++i)
    {
        QGraphicsPixmapItem* rocket = rockets.at(rockets.size() - 1);
        scene->removeItem(rocket);
        delete rocket;
        rockets.pop_back();
    }
}

void Game::updateHUD(QPointF view)
{
    for(int i = 0; i < hearts.size(); ++i)
    {
        hearts.at(i)->setPos(view.x() + 12 + i * 50, view.y() + 12);
    }

    for(int i = 0; i < grenades.size(); ++i)
    {
        grenades.at(i)->setPos(view.x() + 800 - 12 - (i+1) * 18, view.y() + 12);
    }

    for(int i = 0; i < rockets.size(); ++i)
    {
        rockets.at(i)->setPos(view.x() + 800 - 12 - (i+1) * 30, view.y() + 38);
    }

    if(paused)
    {
        exitToTitleText->setPos(view.x() + 100, view.y() + 200);
    }
}
