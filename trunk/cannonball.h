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

#ifndef CANNONBALL_H
#define CANNONBALL_H

#include "particle.h"
#include "sound.h"

class CannonBall : public Particle
{
public:
    enum Size {SMALL, MEDIUM, LARGE};
    CannonBall(QPointF velocity, int life, Game* game, Size size = SMALL);
    void update();
    bool alive();
    void destroy();
    void explode();

private:
    Game* game;
    QPointF velocity;
    int life;
    Size size;

    Sound* explosionSound;

    static int const EXPLOSION_BULLETS[3];
};

#endif // CANNONBALL_H
