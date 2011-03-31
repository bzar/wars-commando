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

#ifndef ACTOR_H
#define ACTOR_H

class Bullet;
class Rocket;

#include "animated.h"
#include "updatable.h"

#include "game.h"
#include "sound.h"

class Actor : public Animated, public Updatable
{
public:
    Actor(QString const& definitionFile, Game* game);

    virtual void update() = 0;
    virtual bool alive();
    void setVelocity(QPointF newVelocity);
    virtual bool isHitting(QPointF point);
    virtual void hitByBullet(Bullet* bullet);
    virtual void hitByRocket(Rocket* rocket);
    virtual void setBoss(bool value);
    bool isBoss();

protected:
    Game* game;
    QPointF velocity;
    bool onGround;
    bool frozen;
    Sound* hitSound;
    bool boss;

    virtual void preUpdate();
    virtual void postUpdate();

    virtual void hitLeftWall();
    virtual void hitRightWall();
    virtual void hitCeiling();
    virtual void hitFloor();
};

#endif // ACTOR_H
