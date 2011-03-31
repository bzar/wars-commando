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

#include "enemytank.h"
#include "player.h"
#include "cannonball.h"
#include "bullet.h"
#include "rocket.h"
#include "heartitem.h"
#include "grenadeitem.h"
#include "rocketitem.h"

EnemyTank::EnemyTank(QString definitionFile, Game* game) :
    Vehicle(definitionFile, game), state(STATE_MOVING_FORWARD), life(0), behaviorTimer(BEHAVIOR_CHANGE_INTERVAL_MAX), cannonSound(0)
{
    cannonSound = game->getResourceManager().getSound("cannon");
}

void EnemyTank::preUpdate()
{
    QPointF diff = game->getPlayer()->pos() - pos();
    frozen = abs(diff.x()) > 800;

    if(!frozen)
    {
        velocity.setY(velocity.y() + 1.8);
    }

    if(state != STATE_DYING)
    {
        --behaviorTimer;

        if(behaviorTimer == 0)
        {
            behaviorTimer = BEHAVIOR_CHANGE_INTERVAL_MIN + qrand() % (BEHAVIOR_CHANGE_INTERVAL_MAX - BEHAVIOR_CHANGE_INTERVAL_MIN);

            if(!frozen)
            {
                if(state == STATE_MOVING_FORWARD)
                {
                    if(velocity.x() == 0)
                    {
                        if(facingPlayer())
                        {
                            velocity.setX(getAnimationDirection() ? 3 : -3);
                        }
                        else
                        {
                            switchDirection();
                        }
                    }
                    else if(onGround)
                    {
                        state = STATE_MOVING_BACKWARD;
                        velocity.setX(0);
                    }
                }
                else if(state == STATE_MOVING_BACKWARD)
                {
                    if(velocity.x() == 0)
                    {
                        if(facingPlayer())
                        {
                            velocity.setX(getAnimationDirection() ? -3 : 3);
                        }
                        else
                        {
                            switchDirection();
                        }
                    }
                    else if(onGround)
                    {
                        state = STATE_MOVING_FORWARD;
                        velocity.setX(0);
                    }
                }
            }
        }

        if(facingPlayer() && abs(diff.y()) < 100)
        {
            shootingTimer = (shootingTimer + 1) % SHOOTING_INTERVAL;
            if(shootingTimer == 0 && !frozen)
            {
                shoot();
            }
        }
    }

    selectAnimation();
}

void EnemyTank::postUpdate()
{
    if(!frozen && state != STATE_DYING)
    {
        Player* player = game->getPlayer();
        QPointF playerPos(player->pos().x() + player->pixmap().width()/2, player->pos().y() + player->pixmap().height()/2);
        QPointF myPos(pos().x() + pixmap().width()/2, pos().y() + pixmap().height()/2);
        QPointF diff = playerPos - myPos;
        if(state != STATE_DYING && diff.x() * diff.x() + diff.y() * diff.y() * 2 < pixmap().width()/2 * pixmap().width()/2 + player->pixmap().height()/2 * player->pixmap().height()/2)
        {
            player->hitByEnemy(this);
        }
    }
}

bool EnemyTank::alive()
{
    return getAnimation() != ANIM_DESTROY || getAnimationFrame() / (getAnimationSpeed() * getAnimationLength(ANIM_DESTROY) - 1) != 1;
}


void EnemyTank::shoot()
{
    QPointF shotVelocity(10,0);
    QPointF muzzleDelta(pixmap().width() - 8, 4);
    if(getAnimationDirection() == ANIM_LEFT)
    {
        shotVelocity.setX(-shotVelocity.x());
        muzzleDelta.setX(8);
    }
    CannonBall* cannonBall = new CannonBall(shotVelocity, 50, game);
    cannonBall->setPos(pos() + muzzleDelta);
    cannonSound->play();
}

void EnemyTank::hitByBullet(Bullet *bullet)
{
    if(state != STATE_DYING)
    {
        hitSound->play();
        --life;
        if(life <= 0)
        {
            die();
        }
        else
        {
            flash();
        }

        bullet->destroy();
    }
}

void EnemyTank::hitByRocket(Rocket *rocket)
{
    if(state != STATE_DYING)
    {
        life -= 50;
        if(life <= 0)
        {
            die();
        }
        else
        {
            flash();
        }
        hitSound->play();
        rocket->explode();
    }
}

void EnemyTank::selectAnimation()
{
    if(state == STATE_DYING)
    {
        setAnimation(ANIM_DESTROY, getAnimationDirection());
    }
    else if(velocity.x() == 0)
    {
        setAnimation(ANIM_STOP, getAnimationDirection());
    }
    else if(state == STATE_MOVING_FORWARD)
    {
        setAnimation(ANIM_FORWARD, getAnimationDirection());
    }
    else if(state == STATE_MOVING_BACKWARD)
    {
        setAnimation(ANIM_BACKWARD, getAnimationDirection());
    }
}

void EnemyTank::hitLeftWall()
{
    Vehicle::hitLeftWall();
    if(state == STATE_MOVING_FORWARD)
    {
        state = STATE_MOVING_BACKWARD;
    }
    else if(state == STATE_MOVING_BACKWARD)
    {
        state = STATE_MOVING_FORWARD;
    }
}

void EnemyTank::hitRightWall()
{
    Vehicle::hitRightWall();
    if(state == STATE_MOVING_FORWARD)
    {
        state = STATE_MOVING_BACKWARD;
    }
    else if(state == STATE_MOVING_BACKWARD)
    {
        state = STATE_MOVING_FORWARD;
    }
}

bool EnemyTank::facingPlayer()
{
    return (getAnimationDirection() == ANIM_LEFT && pos().x() + pixmap().width()/2 > game->getPlayer()->pos().x()) ||
           (getAnimationDirection() == ANIM_RIGHT && pos().x() + pixmap().width()/2 < game->getPlayer()->pos().x());
}

void EnemyTank::switchDirection()
{
    setAnimationDirection(getAnimationDirection() == ANIM_LEFT ? ANIM_RIGHT : ANIM_LEFT);
    if(state == STATE_MOVING_FORWARD)
    {
        state = STATE_MOVING_BACKWARD;
    }
    else if(state == STATE_MOVING_BACKWARD)
    {
        state = STATE_MOVING_FORWARD;
    }
}

void EnemyTank::dropItem()
{
    if(!boss)
    {
        HeartItem* item = new HeartItem(game);
        item->setPos(pos().x() + pixmap().width()/2 - item->pixmap().width()/2, pos().y() + pixmap().height() - item->pixmap().height());

        if(qrand() % 2 == 0)
        {
            GrenadeItem* item2 = new GrenadeItem(game);
            item2->setPos(pos().x() + pixmap().width()/2 - item2->pixmap().width()/2 + 10, pos().y() + pixmap().height() - item2->pixmap().height());
        }

        if(qrand() % 4 == 0)
        {
            RocketItem* item3 = new RocketItem(game);
            item3->setPos(pos().x() + pixmap().width()/2 - item3->pixmap().width()/2 - 10, pos().y() + pixmap().height() - item3->pixmap().height());
        }

    }
}

void EnemyTank::die()
{
    state = STATE_DYING;
    velocity.setX(0);
    velocity.setY(0);
    setAnimationFrame(0);
    dropItem();
}
