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

#include "player.h"
#include "game.h"
#include "bullet.h"
#include "rocket.h"
#include "grenade.h"
#include "cannonball.h"

#include <QPainter>

Player::Player(Game* game) :
    Infantry(":/img/player.txt", game),
    movingLeft(false), movingRight(false), aimingUp(false), aimingDown(false), shooting(false), jumping(false),
    canMove(true), invisible(0), running(false), dying(false),
    shotSound(0), bazookaSound(0), jumpSound(0)
{
    shotSound = game->getResourceManager().getSound("shot");
    bazookaSound = game->getResourceManager().getSound("bazooka");
    jumpSound = game->getResourceManager().getSound("jump");
}

void Player::moveLeft()
{
    movingLeft = true;
}

void Player::moveRight()
{
    movingRight = true;
}

void Player::aimUp()
{
    aimingUp = true;
}

void Player::aimDown()
{
    aimingDown = true;
}

void Player::shoot()
{
    if(!shooting && canMove && !dying)
    {
        shooting = true;
        setAnimationFrame(0);
    }
}
void Player::jump()
{
    if(canMove && onGround && !dying)
    {
        velocity.setY(-20);
        jumping = true;
        jumpSound->play();
    }
}

void Player::throwGrenade()
{
    if(canMove && !dying && game->numGrenades() > 0)
    {
        QPointF center = pos();
        center.setX(center.x() + pixmap().width()/2);

        QPointF grenadeVelocity(running ? 12 : 10, -20);
        if(aimingDown && !aimingUp)
        {
            grenadeVelocity.setX(running ? 15 : 13);
            grenadeVelocity.setY(-17);
        }
        else if(aimingUp && !aimingDown)
        {
            grenadeVelocity.setX(running ? 7 : 5);
            grenadeVelocity.setY(-25);
        }

        if(getAnimationDirection() == ANIM_LEFT)
        {
            grenadeVelocity.setX(-grenadeVelocity.x());
        }

        Grenade* grenade = new Grenade(grenadeVelocity, game);
        grenade->setPos(center + QPointF(-grenade->pixmap().width()/2, 0));
        game->addActor(grenade);
        game->removeGrenades(1);
    }
}

void Player::fireBazooka()
{
    if(onGround && canMove && !dying && game->numRockets() > 0)
    {
        setAnimation(ANIM_BAZOOKA, getAnimationDirection());
        setAnimationFrame(0);
        canMove = false;
        shooting = false;
        game->removeRockets(1);
    }
}

void Player::setRunning(bool value)
{
    running = value && onGround;
}

void Player::stopMovingLeft()
{
    movingLeft = false;
}

void Player::stopMovingRight()
{
    movingRight = false;
}

void Player::stopAimingUp()
{
    aimingUp = false;
}

void Player::stopAimingDown()
{
    aimingDown = false;
}

void Player::stopShooting()
{
    shooting = false;
}

void Player::stopJumping()
{
    if(jumping)
    {
        jumping = false;
        if(velocity.y() < 0)
        {
            velocity.setY(velocity.y()/2);
        }
    }
}

void Player::hurt()
{
    if(!invisible)
    {
        hitSound->play();
        invisible = INVISIBLE_TIME;
        flash();
        game->removeHearts(1);
        if(game->alive())
        {
            setOpacity(0.5);
        }
        running = false;
    }
}

void Player::die()
{
    dying = true;
    velocity.setX(0);
    velocity.setY(0);
    setAnimationSpeed(4);
    setAnimation(ANIM_DYING, getAnimationDirection());
}

void Player::hitByBullet(Bullet* bullet)
{
    if(!invisible)
    {
        bullet->destroy();
    }

    hurt();
}

void Player::hitByEnemy(Actor* enemy)
{
    hurt();
}

void Player::hitByRocket(Rocket *rocket)
{
    if(!invisible)
    {
        rocket->explode();
    }

    hurt();

}

void Player::hitByCannonBall(CannonBall* cannonBall)
{
    if(!invisible)
    {
        cannonBall->explode();
    }

    hurt();
}

void Player::preUpdate()
{
    velocity.setY(velocity.y() + 1.8);

    if(velocity.y() > 25)
    {
        velocity.setY(25);
    }

    if(dying)
    {
        return;
    }
    if(invisible)
    {
        --invisible;
        if(!invisible)
        {
            setOpacity(1.0);
        }
    }

    if(canMove && movingLeft && !movingRight)
    {
        velocity.setX(running ? -9 : -7);
    }
    else if(canMove && movingRight && !movingLeft)
    {
        velocity.setX(running ? 9 : 7);
    } else {
        velocity.setX(0);
    }

    if(shooting && getAnimationFrame() % getAnimationSpeed() == 0 && (getAnimationFrame() / getAnimationSpeed()) % 2 == 0)
    {
        QPointF bulletVelocity(running ? 17 : 15, 0);
        QPointF muzzleDelta(44, 24);
        if(getAnimationDirection() == ANIM_LEFT)
        {
            bulletVelocity.setX(-bulletVelocity.x());
            muzzleDelta.setX(4);
        }
        Bullet* bullet = new Bullet(bulletVelocity, 50, game, true);
        bullet->setPos(pos() + muzzleDelta);
        shotSound->play();
    }

    if(getAnimation() == ANIM_BAZOOKA && getAnimationFrame() == 1 * getAnimationSpeed())
    {
        QPointF rocketVelocity(15,0);
        QPointF muzzleDelta(10, 16);
        if(getAnimationDirection() == ANIM_LEFT)
        {
            rocketVelocity.setX(-rocketVelocity.x());
            muzzleDelta.setX(10);
        }
        Rocket* rocket = new Rocket(rocketVelocity, 50, game, true);
        rocket->setPos(pos() + muzzleDelta);
        bazookaSound->play();
    }
}

void Player::postUpdate()
{
    if(dying)
    {
        if(getAnimationFrame() == 4* getAnimationSpeed() - 1)
        {
            setAnimation(ANIM_DEAD, getAnimationDirection());
        }
        return;
    }

    if(getAnimation() == ANIM_BAZOOKA && getAnimationFrame() == 4 * getAnimationSpeed() - 1)
    {
        canMove = true;
    }

    if(canMove)
    {
        AnimationDirection direction = getAnimationDirection();
        int animation = getAnimation();

        if(movingLeft)
        {
            direction = ANIM_LEFT;
            animation = shooting ? ANIM_WALK_SHOOT : ANIM_WALK;
        }
        else if(movingRight)
        {
            direction = ANIM_RIGHT;
            animation = shooting ? ANIM_WALK_SHOOT : ANIM_WALK;
        }
        else
        {
            animation = shooting ? ANIM_STAND_SHOOT : ANIM_STAND;
        }

        if(!onGround)
        {
            animation = shooting ? ANIM_JUMP_SHOOT : ANIM_JUMP;
        }

        setAnimation(animation, direction);
    }
}

void Player::hitFloor()
{
    Infantry::hitFloor();
    jumping = false;
}
