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

#include "mapstate.h"

MapState::MapState(GameWindow* window) :
    ApplicationState(window), window(window), map(new Map(window))
{

}

void MapState::reset()
{
    map->reset();
}

void MapState::enter()
{
    map->enter();
    window->setSceneRect(0, 0, 800, 480);
}

void MapState::exit()
{
    map->exit();
}

void MapState::handleKeyPressEvent(QKeyEvent *event)
{
    if(map->ready())
    {
        window->showGame();
    }
    else
    {
        map->skipAnimation();
    }
}

void MapState::handleKeyReleaseEvent(QKeyEvent *event)
{

}

void MapState::update()
{
    map->update();
}

QGraphicsScene* MapState::getScene() const
{
    return map->getScene();
}

Map* MapState::getMap()
{
    return map;
}
