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

#ifndef STARTSTATE_H
#define STARTSTATE_H

#include "applicationstate.h"
#include "gamewindow.h"
#include "music.h"
#include "resourcemanager.h"
#include "animated.h"

#include <QList>

class StartState : public ApplicationState
{
    Q_OBJECT
public:
    explicit StartState(GameWindow* window);
    void enter();
    void exit();

    void handleKeyPressEvent(QKeyEvent *event);
    void handleKeyReleaseEvent(QKeyEvent *event);
    void update();
    QGraphicsScene* getScene() const;

signals:

public slots:

private:
    GameWindow* window;
    QGraphicsScene* scene;
    ResourceManager resources;
    Music* music;

    Animated* menuNewGame;
    Animated* menuQuit;

    QList<Animated*> menuItems;

    enum MenuItem { MENU_START, MENU_QUIT, NUM_MENU_ITEMS };
    int currentItem;

};

#endif // STARTSTATE_H
