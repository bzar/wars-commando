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

#ifndef GRENADE_H
#define GRENADE_H

#include "infantry.h"
#include "sound.h"

class Grenade : public Infantry
{
public:
    Grenade(QPointF velocitya, Game* game, bool playerGrenade = true);

    bool alive();

protected:
    void preUpdate();

    void hitFloor();
    void hitLeftWall();
    void hitRightWall();

private:
    enum Animation {ANIM_NORMAL, ANIM_EXPLODE};
    int timer;
    static int const MAX_TIME = 30;
    bool playerGrenade;
    Sound* explosionSound;

};

#endif // GRENADE_H
