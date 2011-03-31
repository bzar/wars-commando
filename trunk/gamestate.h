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

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "applicationstate.h"
#include "gamewindow.h"
#include "game.h"

class GameState : public ApplicationState
{
    Q_OBJECT
public:
    GameState(GameWindow* window);
    void enter();
    void exit();

    void handleKeyPressEvent(QKeyEvent *event);
    void handleKeyReleaseEvent(QKeyEvent *event);
    void update();
    QGraphicsScene* getScene() const;

    Game* getGame();

    void firstLevel();
    void nextLevel();
    void resetLevel();
    bool getCheater();

private:
    GameWindow* window;
    Game* game;

    bool cheater;

    int currentLevel;

    int runTimer;
    static int const RUN_DELAY = 10;

    static QString const LEVEL_FILES[];
    static unsigned int const NUM_LEVELS;

};

#endif // GAMESTATE_H
