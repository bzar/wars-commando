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

#include "gamestate.h"
#include "player.h"
#include "settings.h"

QString const GameState::LEVEL_FILES[] = {":/levels/level1.txt", ":/levels/level2.txt", ":/levels/level3.txt",
                                          ":/levels/level4.txt", ":/levels/level5.txt", ":/levels/level6.txt",
                                          ":/levels/level7.txt", ":/levels/level8.txt"};

unsigned int const GameState::NUM_LEVELS = sizeof(GameState::LEVEL_FILES)/sizeof(char* const);

GameState::GameState(GameWindow* window):
    ApplicationState(window), window(window), game(0), cheater(false), currentLevel(0), runTimer(0)
{
    game = new Game(LEVEL_FILES[currentLevel], window);
    QObject::connect(game, SIGNAL(die()), window, SLOT(loseLife()));
    QObject::connect(game, SIGNAL(win()), window, SLOT(nextLevel()));
}

void GameState::enter()
{
    game->enter();
    window->setSceneRect(game->getScene()->sceneRect());
    window->centerOn(game->getPlayer());
}

void GameState::exit()
{
    game->exit();
}

void GameState::handleKeyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }

    QKeySequence key(event->modifiers() + event->key());

    if(game->isPaused())
    {
        if(key.matches(Settings::YES_KEY))
        {
            window->reset();
        }
        else if(key.matches(Settings::NO_KEY))
        {
            game->resume();
        }

    }
    else
    {
        Player* player = game->getPlayer();

        if(key.matches(Settings::PLAYER_LEFT_KEY))
        {
            if(runTimer)
            {
                player->setRunning(true);
            }
            else
            {
                runTimer = RUN_DELAY;
            }

            player->moveLeft();
        }
        else if(key.matches(Settings::PLAYER_RIGHT_KEY))
        {
            if(runTimer)
            {
                player->setRunning(true);
            }
            else
            {
                runTimer = RUN_DELAY;
            }

            player->moveRight();
        }
        else if(key.matches(Settings::PLAYER_UP_KEY))
        {
            player->aimUp();
        }
        else if(key.matches(Settings::PLAYER_DOWN_KEY))
        {
            player->aimDown();
        }
        else if(key.matches(Settings::PLAYER_JUMP_KEY))
        {
            player->jump();
        }
        else if(key.matches(Settings::PLAYER_SHOOT_KEY))
        {
            player->shoot();
        }
        else if(key.matches(Settings::PLAYER_GRENADE_KEY))
        {
            player->throwGrenade();
        }
        else if(key.matches(Settings::PLAYER_BAZOOKA_KEY))
        {
            player->fireBazooka();
        }
        else if(event->key() == Qt::Key_N && (event->modifiers() & Qt::ShiftModifier))
        {
            window->nextLevel();
            cheater = true;
        }
        else if(event->key() == Qt::Key_G && (event->modifiers() & Qt::ShiftModifier))
        {
            game->addGrenades();
            cheater = true;
        }
        else if(event->key() == Qt::Key_B && (event->modifiers() & Qt::ShiftModifier))
        {
            game->addRockets();
            cheater = true;
        }

        else if(key.matches(Settings::PAUSE_KEY))
        {
            game->pause();
        }
    }
}

void GameState::handleKeyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
        return;
    }

    QKeySequence key(event->modifiers() + event->key());

    if(!game->isPaused())
    {
        Player* player = game->getPlayer();

        if(key.matches(Settings::PLAYER_LEFT_KEY))
        {
            player->stopMovingLeft();
            player->setRunning(false);
        }
        else if(key.matches(Settings::PLAYER_RIGHT_KEY))
        {
            player->stopMovingRight();
            player->setRunning(false);
        }
        else if(key.matches(Settings::PLAYER_UP_KEY))
        {
            player->stopAimingUp();
        }
        else if(key.matches(Settings::PLAYER_DOWN_KEY))
        {
            player->stopAimingDown();
        }
        else if(key.matches(Settings::PLAYER_JUMP_KEY))
        {
            player->stopJumping();
        }
        else if(key.matches(Settings::PLAYER_SHOOT_KEY))
        {
            player->stopShooting();
        }
    }
}

void GameState::update()
{
    if(runTimer > 0)
    {
        --runTimer;
    }
    game->updateGame();
    window->centerOn(game->getPlayer());
    game->updateHUD(window->mapToScene(QPoint(0,0)));

}

QGraphicsScene* GameState::getScene() const
{
    return game->getScene();
}

Game* GameState::getGame()
{
    return game;
}

void GameState::firstLevel()
{
    currentLevel = 0;
    cheater = false;
    game->loadLevel(LEVEL_FILES[currentLevel], true);
    game->resume();
}

void GameState::nextLevel()
{
    game->loadLevel(LEVEL_FILES[currentLevel = (currentLevel + 1) % NUM_LEVELS]);
}

void GameState::resetLevel()
{
    game->loadLevel(LEVEL_FILES[currentLevel]);
}

bool GameState::getCheater()
{
    return cheater;
}
