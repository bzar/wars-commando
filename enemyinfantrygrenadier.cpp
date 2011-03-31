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

#include "enemyinfantrygrenadier.h"
#include "game.h"
#include "player.h"
#include "grenade.h"
#include "grenadeitem.h"

EnemyInfantryGrenadier::EnemyInfantryGrenadier(Game* game, bool dontFall) :
    EnemyInfantry(":/img/enemyInfantryGrenadier.txt", game, dontFall), shooting(false), shootingTimer(SHOOTING_INTERVAL)
{
    velocity.setX(0);
}

void EnemyInfantryGrenadier::die()
{
    shooting = false;
    EnemyInfantry::die();
}

void EnemyInfantryGrenadier::dropItem()
{
    GrenadeItem* item = new GrenadeItem(game);
    item->setPos(pos().x() + pixmap().width()/2 - item->pixmap().width()/2, pos().y() + pixmap().height() - item->pixmap().height());
}

void EnemyInfantryGrenadier::preUpdate()
{
    EnemyInfantry::preUpdate();

    if(!frozen && state == STATE_ALIVE)
    {
        shootingTimer = (shootingTimer + 1) % SHOOTING_INTERVAL;
        if(shootingTimer == 0 && pos().y() - game->getPlayer()->pos().y() < 200 && abs(game->getPlayer()->pos().x() - pos().x()) < 400)
        {
            shooting = true;
        }

        if(shooting && getAnimation() == 4 && getAnimationFrame() == getAnimationSpeed() * getAnimationLength(4) - 1)
        {
            QPointF center = pos();
            center.setX(center.x() + pixmap().width()/2);

            QPointF grenadeVelocity(10, -20);
            if(game->getPlayer()->pos().y() - center.y() > 96)
            {
                grenadeVelocity.setX(6);
                grenadeVelocity.setY(-17);
            }
            else if(game->getPlayer()->pos().y() - center.y() < -96)
            {
                grenadeVelocity.setX(8);
                grenadeVelocity.setY(-25);
            }

            if(getAnimationDirection() == ANIM_LEFT)
            {
                grenadeVelocity.setX(-grenadeVelocity.x());
            }

            Grenade* grenade = new Grenade(grenadeVelocity, game, false);
            grenade->setPos(center + QPointF(-grenade->pixmap().width()/2, 0));
            game->addActor(grenade);
            shooting = false;
        }
    }
}

void EnemyInfantryGrenadier::selectAnimation()
{
    AnimationDirection direction = game->getPlayer()->pos().x() < pos().x() ? ANIM_LEFT : ANIM_RIGHT;
    if(shooting)
    {
        setAnimation(4, direction);
    }
    else
    {
        setAnimation(ANIM_STAND, direction);
    }
}
