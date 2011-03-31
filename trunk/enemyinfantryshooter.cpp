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

#include "enemyinfantryshooter.h"
#include "game.h"
#include "player.h"
#include "bullet.h"

EnemyInfantryShooter::EnemyInfantryShooter(Game* game, bool dontFall, bool jumping) :
    EnemyInfantry(":/img/enemyInfantryShooter.txt", game, dontFall), shooting(false), jumping(jumping), shootingTimer(0), jumpingTimer(0), shotSound(0)
{
    velocity.setX(0);
    shotSound = game->getResourceManager().getSound("shot");
}

void EnemyInfantryShooter::die()
{
    shooting = false;
    EnemyInfantry::die();
}

void EnemyInfantryShooter::preUpdate()
{
    EnemyInfantry::preUpdate();

    if(!frozen && state == STATE_ALIVE)
    {
        if(jumping)
        {
            jumpingTimer = jumpingTimer + 1 + qrand() % 3;
            if(jumpingTimer >= JUMPING_INTERVAL && onGround)
            {
                velocity.setY(-20);
                jumpingTimer = 0;
            }
        }
        if(abs(game->getPlayer()->pos().y() - pos().y()) < 100 &&
           abs(game->getPlayer()->pos().x() - pos().x()) < 600)
        {
            shootingTimer = (shootingTimer + 1) % SHOOTING_INTERVAL;
            if(shootingTimer == 0)
            {
                shooting = !shooting;
                setAnimationFrame(0);
            }
        }
        else
        {
            shooting = false;
        }

        if(velocity.x() == 0)
        {
            if(game->getPlayer()->pos().x() < pos().x())
            {
                setAnimationDirection(ANIM_LEFT);
            }
            else
            {
                setAnimationDirection(ANIM_RIGHT);
            }
        }

        if(shooting && getAnimationFrame() % getAnimationSpeed() == 0 && (getAnimationFrame() / getAnimationSpeed()) % 2 == 0)
        {
            QPointF bulletVelocity(15,0);
            QPointF muzzleDelta(44, 24);
            if(getAnimationDirection() == ANIM_LEFT)
            {
                bulletVelocity.setX(-bulletVelocity.x());
                muzzleDelta.setX(4);
            }
            Bullet* bullet = new Bullet(bulletVelocity, 50, game);
            bullet->setPos(pos() + muzzleDelta);

            qreal position = (pos().x() - game->getPlayer()->pos().x()) / 1600.0;
            position = position < -1 ? -1 : position > 1 ? 1 : position;
            qreal distance = position < 0 ? -position : position;
            shotSound->play(distance, position);
        }
    }
}

void EnemyInfantryShooter::selectAnimation()
{
    if(shooting)
    {
        Animation animation = ANIM_STAND_SHOOT;
        AnimationDirection direction = getAnimationDirection();

        if(velocity.x() > 0)
        {
            direction = ANIM_RIGHT;
            animation = ANIM_WALK_SHOOT;
        }
        else if(velocity.x() < 0)
        {
            direction = ANIM_LEFT;
            animation = ANIM_WALK_SHOOT;
        }

        if(!onGround)
        {
            animation = ANIM_JUMP_SHOOT;
        }

        setAnimation(animation, direction);
    }
    else
    {
        EnemyInfantry::selectAnimation();
    }
}
