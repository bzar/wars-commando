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

#ifndef ENEMYINFANTRY_H
#define ENEMYINFANTRY_H

#include "infantry.h"
#include "sound.h"

class Game;

class EnemyInfantry : public Infantry
{
public:
    EnemyInfantry(QString const& definitionFile, Game* game, bool dontFall = false);
    virtual void hitByBullet(Bullet *bullet);
    virtual void hitByRocket(Rocket* rocket);
    virtual void hurt();
    virtual bool alive();
    virtual void setBoss(bool value);

protected:
    virtual void preUpdate();
    virtual void postUpdate();

    virtual void hitLeftWall();
    virtual void hitRightWall();
    virtual void hitFloor();
    virtual void die();
    virtual void dropItem();
    virtual void selectAnimation();

    enum State {STATE_ALIVE, STATE_HURT, STATE_DYING, STATE_DEAD};
    enum Animation {ANIM_WALK, ANIM_STAND, ANIM_JUMP, ANIM_DYING, ANIM_WALK_SHOOT, ANIM_STAND_SHOOT, ANIM_JUMP_SHOOT};
    State state;

    Sound* deathSound;

private:
    int life;
    int hurtCounter;
    static int const MAX_LIFE = 3;
    static int const BOSS_MAX_LIFE = 9;
    static int const HURT_FREEZE_TIME = 5;
    bool dontFall;

};

#endif // ENEMYINFANTRY_H
