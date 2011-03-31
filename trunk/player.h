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

#ifndef PLAYER_H
#define PLAYER_H

class CannonBall;

#include "infantry.h"
#include <QGraphicsPixmapItem>
#include <QList>
#include <QPointF>
#include "sound.h"

class Player : public Infantry
{
public:
    Player(Game* game);

    void moveLeft();
    void moveRight();
    void aimUp();
    void aimDown();
    void shoot();
    void jump();
    void throwGrenade();
    void fireBazooka();
    void setRunning(bool value);
    void stopMovingLeft();
    void stopMovingRight();
    void stopAimingUp();
    void stopAimingDown();
    void stopShooting();
    void stopJumping();
    virtual void hurt();
    void die();

    virtual void hitByBullet(Bullet* bullet);
    virtual void hitByEnemy(Actor* enemy);
    virtual void hitByRocket(Rocket* rocket);
    virtual void hitByCannonBall(CannonBall* cannonBall);

protected:
    void preUpdate();
    void postUpdate();

    void hitFloor();

private:
    enum Animation {ANIM_WALK, ANIM_STAND, ANIM_JUMP, ANIM_WALK_SHOOT, ANIM_STAND_SHOOT, ANIM_JUMP_SHOOT, ANIM_BAZOOKA, ANIM_DYING, ANIM_DEAD};

    bool movingLeft;
    bool movingRight;
    bool aimingUp;
    bool aimingDown;

    bool shooting;
    bool jumping;
    bool canMove;
    int invisible;
    bool running;
    bool dying;

    Sound* shotSound;
    Sound* bazookaSound;
    Sound* jumpSound;

    static int const INVISIBLE_TIME = 50;
};

#endif // PLAYER_H
