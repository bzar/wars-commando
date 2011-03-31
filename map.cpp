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

#include "map.h"

#include <QGraphicsItemAnimation>

Map::Map(QObject *parent) :
    QObject(parent), scene(new QGraphicsScene(this)), resources(), currentLevel(0), animationFinished(true), texts()
{
    resources.addImage("background", ":/img/map.png");
    resources.addImage("mapBanner", ":/img/mapBanner.png");
    resources.addImage("numbers", ":/img/numbers.png");

    resources.addImage("speechBubble", ":/img/speechBubble.png");
    resources.addImage("text1", ":/img/text1.png");
    resources.addImage("text2", ":/img/text2.png");
    resources.addImage("text3", ":/img/text3.png");
    resources.addImage("text4", ":/img/text4.png");
    resources.addImage("text5", ":/img/text5.png");

    resources.addImage("player", ":/img/player.png");
    resources.addImage("mapTransportCopter", ":/img/mapTransportCopter.png");
    resources.addImage("enemyInfantryShooter", ":/img/enemyInfantryShooter.png");
    resources.addImage("mapLightTank", ":/img/mapLightTank.png");
    resources.addImage("mapMediumTank", ":/img/mapMediumTank.png");
    resources.addImage("mapHeavyTank", ":/img/mapHeavyTank.png");

    music = new Music("music/07.ogg", this);

    scene->setBackgroundBrush(QBrush(resources.getImage("background")));

    speechBubble = scene->addPixmap(resources.getImage("speechBubble"));
    speechBubble->setPos(75,24);
    speechBubble->setZValue(2);

    speechBubbleText = scene->addPixmap(resources.getImage("text1"));
    speechBubbleText->setParentItem(speechBubble);
    speechBubbleText->setZValue(3);
    speechBubbleText->setPos(24,24);

    speechBubble->hide();

    banner = scene->addPixmap(resources.getImage("mapBanner"));
    banner->setZValue(1);
    banner->setPos(0, 300);

    levelNumber = scene->addPixmap(resources.getImage("numbers").copy(0, 2, 50, 140));
    levelNumber->setParentItem(banner);
    levelNumber->setZValue(2);
    levelNumber->setPos(300, 2);

    livesNumber = scene->addPixmap(resources.getImage("numbers").copy(0, 2, 50, 140));
    livesNumber->setParentItem(banner);
    livesNumber->setZValue(2);
    livesNumber->setPos(715, 2);

    banner->hide();

    player = new Animated(":/img/player.txt", resources, scene);
    copter = new Animated(":/img/mapTransportCopter.txt", resources, scene);
    enemy1 = new Animated(":/img/enemyInfantryShooter.txt", resources, scene);
    enemy2 = new Animated(":/img/enemyInfantryShooter.txt", resources, scene);
    enemy3 = new Animated(":/img/mapLightTank.txt", resources, scene);
    enemy4 = new Animated(":/img/enemyInfantryShooter.txt", resources, scene);
    enemy5 = new Animated(":/img/mapLightTank.txt", resources, scene);
    enemy6 = new Animated(":/img/mapMediumTank.txt", resources, scene);
    enemy7 = new Animated(":/img/mapMediumTank.txt", resources, scene);
    enemy8 = new Animated(":/img/mapHeavyTank.txt", resources, scene);

    reset();

    update();

}

QGraphicsScene* Map::getScene()
{
    return scene;
}

void Map::enter()
{
    music->play();
}

void Map::exit()
{
    music->stop();
}

bool Map::ready()
{
    return animationFinished;
}

void Map::reset()
{
    currentLevel = 0;
    banner->hide();

    player->setPos(-48, 0);
    copter->setPos(-48, 0);
    enemy1->setPos(432, 0);
    enemy1->setAnimation(1, Animated::ANIM_LEFT);
    enemy2->setPos(428, 144);
    enemy2->setAnimation(1, Animated::ANIM_LEFT);
    enemy3->setPos(626, 10);
    enemy3->setAnimation(0, Animated::ANIM_LEFT);
    enemy4->setPos(528, 240);
    enemy4->setAnimation(1, Animated::ANIM_LEFT);
    enemy5->setPos(528, 298);
    enemy5->setAnimation(0, Animated::ANIM_LEFT);
    enemy6->setPos(674, 288);
    enemy6->setAnimation(0, Animated::ANIM_LEFT);
    enemy7->setPos(722, 336);
    enemy7->setAnimation(0, Animated::ANIM_LEFT);
    enemy8->setPos(674, 384);
    enemy8->setAnimation(0, Animated::ANIM_LEFT);

    enemy1->show();
    enemy2->show();
    enemy3->show();
    enemy4->show();
    enemy5->show();
    enemy6->show();
    enemy7->show();
    enemy8->show();

    texts.clear();
    texts << resources.getImage("text1") << resources.getImage("text2") << resources.getImage("text3") <<
            resources.getImage("text4") << resources.getImage("text5");
    speechBubbleText->setPixmap(texts.first());
    texts.pop_front();

    speechBubble->show();

    tl = 0;
    animationFinished = false;

}

void Map::update()
{
    player->animate();
    copter->animate();
    enemy1->animate();
    enemy2->animate();
    enemy3->animate();
    enemy4->animate();
    enemy5->animate();
    enemy6->animate();
    enemy7->animate();
    enemy8->animate();
}

void Map::nextLevel()
{
    ++currentLevel;

    banner->hide();
    levelNumber->setPixmap(resources.getImage("numbers").copy(currentLevel*52, 2, 50, 140));

    animationFinished = false;

    if(currentLevel == 1)
    {
        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy1->setAnimation(0, Animated::ANIM_LEFT);

        tl = new QTimeLine(8000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 8000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* copterAnim = new QGraphicsItemAnimation;
        copterAnim->setItem(copter);
        copterAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy1);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(-48, 48));
        copterAnim->setPosAt(0, QPointF(48, -48));
        enemyAnim->setPosAt(0, QPointF(432, 0));
        playerAnim->setScaleAt(0, 1, 1);

        copterAnim->setPosAt(0.1, QPointF(48, 48));

        playerAnim->setPosAt(0.2, QPointF(48, 48));
        playerAnim->setScaleAt(0.2, 1, 1);

        playerAnim->setPosAt(0.4, QPointF(60, 60));
        copterAnim->setPosAt(0.4, QPointF(48, 48));
        playerAnim->setScaleAt(0.4, 0.4, 0.4);

        playerAnim->setPosAt(0.6, QPointF(252, 60));
        copterAnim->setPosAt(0.6, QPointF(240, 48));
        playerAnim->setScaleAt(0.6, 0.4, 0.4);

        playerAnim->setScaleAt(0.8, 1, 1);
        copterAnim->setPosAt(0.8, QPointF(240, 48));
        playerAnim->setPosAt(0.8, QPointF(240, 48));
        enemyAnim->setPosAt(0.8, QPointF(336, 0));

        copterAnim->setPosAt(0.9, QPointF(240, -48));

        playerAnim->setPosAt(1.0, QPointF(288, 48));
        enemyAnim->setPosAt(1.0, QPointF(336, 48));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 2)
    {
        enemy1->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy2->setAnimation(0, Animated::ANIM_LEFT);

        tl = new QTimeLine(3000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy2);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(288, 48));
        enemyAnim->setPosAt(0, QPointF(428, 144));
        playerAnim->setPosAt(1, QPointF(384, 48));
        enemyAnim->setPosAt(1, QPointF(428, 48));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 3)
    {
        enemy2->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy3->setAnimation(1, Animated::ANIM_LEFT);

        tl = new QTimeLine(3000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy3);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(384, 48));
        enemyAnim->setPosAt(0, QPointF(626, 10));
        enemyAnim->setPosAt(0.25, QPointF(528, 10));
        playerAnim->setPosAt(0.5, QPointF(432, 48));
        enemyAnim->setPosAt(0.75, QPointF(528, 106));
        playerAnim->setPosAt(1, QPointF(432, 96));
        enemyAnim->setPosAt(1, QPointF(480, 106));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 4)
    {
        enemy3->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy4->setAnimation(0, Animated::ANIM_LEFT);

        tl = new QTimeLine(3000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy4);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(432, 96));
        enemyAnim->setPosAt(0, QPointF(528, 240));
        playerAnim->setPosAt(0.5, QPointF(480, 96));
        playerAnim->setPosAt(1, QPointF(480, 144));
        enemyAnim->setPosAt(1, QPointF(528, 144));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 5)
    {
        enemy4->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy5->setAnimation(1, Animated::ANIM_LEFT);

        tl = new QTimeLine(3000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy5);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(480, 144));
        enemyAnim->setPosAt(0, QPointF(528, 288));
        playerAnim->setPosAt(1, QPointF(528, 144));
        enemyAnim->setPosAt(1, QPointF(528, 202));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 6)
    {
        enemy5->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy6->setAnimation(1, Animated::ANIM_LEFT);

        tl = new QTimeLine(3000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy6);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(528, 144));
        enemyAnim->setPosAt(0, QPointF(674, 288));
        enemyAnim->setPosAt(0.7, QPointF(674, 148));
        playerAnim->setPosAt(1, QPointF(576, 144));
        enemyAnim->setPosAt(1, QPointF(624, 148));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 7)
    {
        enemy6->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy7->setAnimation(1, Animated::ANIM_LEFT);

        tl = new QTimeLine(2000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy7);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(576, 144));
        enemyAnim->setPosAt(0, QPointF(722, 336));
        enemyAnim->setPosAt(0.2, QPointF(674, 336));
        playerAnim->setPosAt(1, QPointF(624, 144));
        enemyAnim->setPosAt(1, QPointF(674, 148));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else if(currentLevel == 8)
    {
        enemy7->hide();

        player->setAnimation(0, Animated::ANIM_RIGHT);
        enemy8->setAnimation(1, Animated::ANIM_LEFT);

        tl = new QTimeLine(2000);
        tl->setCurveShape(QTimeLine::LinearCurve);
        tl->setFrameRange(0, 3000/30);

        QGraphicsItemAnimation* playerAnim = new QGraphicsItemAnimation;
        playerAnim->setItem(player);
        playerAnim->setTimeLine(tl);

        QGraphicsItemAnimation* enemyAnim = new QGraphicsItemAnimation;
        enemyAnim->setItem(enemy8);
        enemyAnim->setTimeLine(tl);

        playerAnim->setPosAt(0, QPointF(624, 144));
        enemyAnim->setPosAt(0, QPointF(674, 384));
        playerAnim->setPosAt(1, QPointF(672, 144));
        enemyAnim->setPosAt(1, QPointF(674, 192));

        connect(tl, SIGNAL(finished()), this, SLOT(setStandingAnimations()));
        tl->start();
    }
    else
    {
        animationFinished = true;
    }
}

bool Map::hasNextLevel()
{
    return currentLevel < 8;
}

void Map::setLives(int const lives)
{
    livesNumber->setPixmap(resources.getImage("numbers").copy((lives%10)*52, 2, 50, 140));
}

void Map::skipAnimation()
{
    if(!texts.empty())
    {
        speechBubbleText->setPixmap(texts.first());
        texts.pop_front();
    }
    else if(!tl)
    {
        nextLevel();
        speechBubble->hide();
    }
    else
    {
        tl->setCurrentTime(tl->duration());
    }
}

void Map::setStandingAnimations()
{
    player->setAnimation(1, Animated::ANIM_RIGHT);
    enemy1->setAnimation(1, Animated::ANIM_LEFT);
    enemy2->setAnimation(1, Animated::ANIM_LEFT);
    enemy3->setAnimation(0, Animated::ANIM_LEFT);
    enemy4->setAnimation(1, Animated::ANIM_LEFT);
    enemy5->setAnimation(0, Animated::ANIM_LEFT);
    enemy6->setAnimation(0, Animated::ANIM_LEFT);
    enemy7->setAnimation(0, Animated::ANIM_LEFT);
    enemy8->setAnimation(0, Animated::ANIM_LEFT);

    banner->setPos(800, 300);
    banner->show();

    tl = new QTimeLine(500);
    tl->setFrameRange(0, 300);
    tl->setCurveShape(QTimeLine::LinearCurve);
    tl->setFrameRange(0, 3000/30);

    QGraphicsItemAnimation* bannerAnim = new QGraphicsItemAnimation;
    bannerAnim->setItem(banner);

    bannerAnim->setTimeLine(tl);
    bannerAnim->setPosAt(0, QPointF(800, 300));
    bannerAnim->setPosAt(1, QPointF(0, 300));

    connect(tl, SIGNAL(finished()), this, SLOT(animationEnd()));
    tl->start();
}

void Map::animationEnd()
{
    tl = 0;
    animationFinished = true;
}
