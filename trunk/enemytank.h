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

#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include "vehicle.h"
#include "automachinegun.h"
#include "sound.h"

class EnemyTank : public Vehicle
{
public:
    EnemyTank(QString definitionFile, Game* game);

    void preUpdate();
    void postUpdate();

    bool alive();

    virtual void hitByBullet(Bullet* bullet);
    virtual void hitByRocket(Rocket* rocket);

protected:
    virtual void selectAnimation();
    virtual void shoot();

    virtual void hitLeftWall();
    virtual void hitRightWall();

    bool facingPlayer();
    virtual void switchDirection();
    virtual void dropItem();
    virtual void die();

    enum Animation { ANIM_STOP, ANIM_FORWARD, ANIM_BACKWARD, ANIM_DESTROY };
    enum State { STATE_MOVING_FORWARD, STATE_MOVING_BACKWARD, STATE_DYING };

    State state;

    int life;

    int behaviorTimer;
    int shootingTimer;

    Sound* cannonSound;

private:
    static int const BEHAVIOR_CHANGE_INTERVAL_MIN = 20;
    static int const BEHAVIOR_CHANGE_INTERVAL_MAX = 100;

    static int const SHOOTING_INTERVAL = 100;

};
#endif // ENEMYTANK_H
