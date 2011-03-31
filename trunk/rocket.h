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

#ifndef ROCKET_H
#define ROCKET_H

#include "particle.h"
#include "game.h"
#include "sound.h"

class Rocket : public Particle
{
public:
    Rocket(QPointF velocity, int life, Game* game, bool playerRocket = false);
    void update();
    bool alive();
    void destroy();
    void explode();
    QPointF getVelocity();

private:
    Game* game;
    QPointF velocity;
    int life;
    bool playerRocket;

    Sound* explosionSound;
};

#endif // ROCKET_H
