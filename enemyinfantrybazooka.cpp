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

#include "enemyinfantrybazooka.h"

#include "game.h"
#include "player.h"
#include "rocket.h"
#include "rocketitem.h"

EnemyInfantryBazooka::EnemyInfantryBazooka(Game* game) :
    EnemyInfantry(":/img/enemyInfantryBazooka.txt", game, false), shooting(false), shootingTimer(SHOOTING_INTERVAL), shotSound(0)
{
    velocity.setX(0);
    shotSound = game->getResourceManager().getSound("bazooka");
}

void EnemyInfantryBazooka::die()
{
    shooting = false;
    EnemyInfantry::die();
}

void EnemyInfantryBazooka::dropItem()
{
    RocketItem* item = new RocketItem(game);
    item->setPos(pos().x() + pixmap().width()/2 - item->pixmap().width()/2, pos().y() + pixmap().height() - item->pixmap().height());
}

void EnemyInfantryBazooka::preUpdate()
{
    EnemyInfantry::preUpdate();

    if(!frozen && state == STATE_ALIVE)
    {
        if(abs(game->getPlayer()->pos().y() - pos().y()) < 100 &&
           abs(game->getPlayer()->pos().x() - pos().x()) < 600)
        {
            shootingTimer = (shootingTimer + 1) % SHOOTING_INTERVAL;
            if(shootingTimer == 0)
            {
                shooting = true;
                setAnimationFrame(0);
            }
        }

        if(game->getPlayer()->pos().x() < pos().x())
        {
            setAnimationDirection(ANIM_LEFT);
        }
        else
        {
            setAnimationDirection(ANIM_RIGHT);
        }

        if(shooting && getAnimationFrame() == 0)
        {
            QPointF rocketVelocity(15,0);
            QPointF muzzleDelta(10, 16);
            if(getAnimationDirection() == ANIM_LEFT)
            {
                rocketVelocity.setX(-rocketVelocity.x());
                muzzleDelta.setX(10);
            }
            Rocket* rocket = new Rocket(rocketVelocity, 50, game, false);
            rocket->setPos(pos() + muzzleDelta);

            qreal position = (pos().x() - game->getPlayer()->pos().x()) / 1600.0;
            position = position < -1 ? -1 : position > 1 ? 1 : position;
            qreal distance = position < 0 ? -position : position;
            shotSound->play(distance, position);
        }
        else if(shooting && getAnimationFrame() == getAnimationSpeed() * getAnimationLength(ANIM_STAND_SHOOT) - 1)
        {
            shooting = false;
        }
    }
}

void EnemyInfantryBazooka::selectAnimation()
{
    if(shooting)
    {
        setAnimation(ANIM_STAND_SHOOT, getAnimationDirection());
    }
    else
    {
        setAnimation(ANIM_STAND, getAnimationDirection());
    }
}
