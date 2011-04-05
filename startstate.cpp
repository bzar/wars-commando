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

#include "startstate.h"
#include "settings.h"

#include <QGraphicsPixmapItem>

StartState::StartState(GameWindow* window) :
    ApplicationState(window), window(window), scene(new QGraphicsScene()), resources(), music(0), menuItems(), currentItem(MENU_START)
{
    scene->setBackgroundBrush(QBrush(QColor("black")));

    music = new Music("music/01.ogg", this);

    resources.addImage("background", ":/img/titleBackground.png");
    resources.addImage("title", ":/img/title_text.png");
    resources.addImage("credits", ":/img/title_credits.png");
    resources.addImage("menuNewGame", ":/img/menuNewGame.png");
    resources.addImage("menuQuit", ":/img/menuQuit.png");

    resources.addSound("select", "sfx/select.wav");

    scene->setBackgroundBrush(QBrush(resources.getImage("background")));
    scene->addPixmap(resources.getImage("title"))->setPos(120, 100);

    scene->addPixmap(resources.getImage("credits"))->setPos(500, 420);

    menuNewGame = new Animated(":/img/menuNewGame.txt", resources, scene);
    menuQuit = new Animated(":/img/menuQuit.txt", resources, scene);

    menuNewGame->setPos(250, 250);
    menuNewGame->setAnimation(1, Animated::ANIM_RIGHT);
    menuQuit->setPos(250, 325);

    menuItems << menuNewGame << menuQuit;
}

void StartState::enter()
{
    window->setSceneRect(0, 0, 800, 480);
    music->play();
}

void StartState::exit()
{
    music->stop();
}

void StartState::handleKeyPressEvent(QKeyEvent *event)
{
    menuItems.at(currentItem)->setAnimation(0, Animated::ANIM_RIGHT);

//Commented out for Mac compatibility
//    QKeySequence key(event->modifiers() + event->key());
      QKeySequence key(event->key());

    if(key.matches(Settings::PLAYER_DOWN_KEY))
    {
        currentItem = (currentItem + 1) % NUM_MENU_ITEMS;
        resources.getSound("select")->play();
    }
    else if(key.matches(Settings::PLAYER_UP_KEY))
    {
        currentItem = currentItem - 1 >= 0 ? currentItem - 1 : NUM_MENU_ITEMS - 1;
        resources.getSound("select")->play();
    }
    else if(key.matches(Settings::OK_KEY))
    {
        if(currentItem == MENU_START)
        {
            window->newGame();
        }
        else if(currentItem == MENU_QUIT)
        {
            window->quit();
        }
    }

    menuItems.at(currentItem)->setAnimation(1, Animated::ANIM_RIGHT);
}

void StartState::handleKeyReleaseEvent(QKeyEvent *event)
{

}

void StartState::update()
{
    menuNewGame->animate();
    menuQuit->animate();
}

QGraphicsScene* StartState::getScene() const
{
    return scene;
}
