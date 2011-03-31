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

#include "actor.h"
#include "bullet.h"

Actor::Actor(QString const& definitionFile, Game* game) :
    Animated(definitionFile, game->getResourceManager(), game->getScene()), game(game), velocity(), onGround(false), frozen(false), boss(false)
{
    hitSound = game->getResourceManager().getSound("hit");
}

void Actor::update()
{

}

bool Actor::isHitting(QPointF point)
{
    return false;
}

bool Actor::alive()
{
    return true;
}

void Actor::setVelocity(QPointF newVelocity)
{
    velocity = newVelocity;
}

void Actor::setBoss(bool value)
{
    boss = value;
}

bool Actor::isBoss()
{
    return boss;
}

void Actor::preUpdate()
{

}

void Actor::postUpdate()
{

}

void Actor::hitLeftWall()
{

}

void Actor::hitRightWall()
{

}

void Actor::hitCeiling()
{

}

void Actor::hitFloor()
{

}

void Actor::hitByBullet(Bullet* bullet)
{

}

void Actor::hitByRocket(Rocket* rocket)
{

}
