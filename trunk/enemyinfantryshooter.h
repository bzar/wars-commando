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

#ifndef ENEMYINFANTRYSHOOTER_H
#define ENEMYINFANTRYSHOOTER_H

#include "enemyinfantry.h"
#include "sound.h"

class EnemyInfantryShooter : public EnemyInfantry
{
public:
    EnemyInfantryShooter(Game* game, bool dontFall = false, bool jumping = false);

protected:
    virtual void preUpdate();
    virtual void die();

    virtual void selectAnimation();

private:
    bool shooting;
    bool jumping;
    int shootingTimer;
    static int const SHOOTING_INTERVAL = 20;

    int jumpingTimer;
    static int const JUMPING_INTERVAL = 93;

    Sound* shotSound;
};

#endif // ENEMYINFANTRYSHOOTER_H
