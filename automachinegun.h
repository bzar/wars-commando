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

#ifndef AUTOMACHINEGUN_H
#define AUTOMACHINEGUN_H

class Game;

#include "animated.h"
#include "updatable.h"
#include "sound.h"

class AutoMachineGun : public Animated, public Updatable
{
public:
    AutoMachineGun(QGraphicsItem* parent, Game* game, qreal angle = 90.0, int numSteps = 5);
    void update();
    void switchDirection();
    void setStep(int newStep);

private:
    enum Animation { ANIM_NORMAL, ANIM_SHOOTING };
    int shootingTimer;
    static int const SHOOTING_INTERVAL = 55;
    static int const SHOOTING_TIME = 30;

    Game* game;
    Sound* shotSound;
    int step;
    int numSteps;
    qreal angle;
};

#endif // AUTOMACHINEGUN_H
