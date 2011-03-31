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

#include "automachinegun.h"
#include "bullet.h"
#include "player.h"

#include <cmath>

AutoMachineGun::AutoMachineGun(QGraphicsItem* parent, Game* game, qreal angle, int numSteps) :
    Animated(":/img/machineGun.txt", game->getResourceManager(), game->getScene()), shootingTimer(SHOOTING_INTERVAL), game(game), shotSound(0), step(0), numSteps(numSteps), angle(angle)
{
    setParentItem(parent);
    shotSound = game->getResourceManager().getSound("shot");
    setZValue(3);
    setTransformOriginPoint(5.5, 5.5);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}


void AutoMachineGun::update()
{
    QPointF diff = game->getPlayer()->pos() - (pos() + parentItem()->pos());
    if(abs(diff.x()) > 600 || abs(diff.y()) > 400)
    {
        return;
    }

    shootingTimer = (shootingTimer + 1) % SHOOTING_INTERVAL;
    if(shootingTimer == 0)
    {
        if(getAnimation() == ANIM_NORMAL)
        {
            setAnimation(ANIM_SHOOTING, getAnimationDirection());
            setAnimationFrame(0);
            shootingTimer = SHOOTING_INTERVAL - SHOOTING_TIME;
        }
        else
        {
            setAnimation(ANIM_NORMAL, getAnimationDirection());
        }
    }

    animate();

    if(getAnimation() == ANIM_SHOOTING && getAnimationFrame() % getAnimationSpeed() == 0 && (getAnimationFrame() / getAnimationSpeed()) % 2 == 1)
    {
        if(step < numSteps)
        {
            if(getAnimationDirection() == ANIM_RIGHT)
            {
                setRotation(rotation() - angle/numSteps);
            }
            else
            {
                setRotation(rotation() + angle/numSteps);
            }
        }
        else
        {
            if(getAnimationDirection() == ANIM_RIGHT)
            {
                setRotation(rotation() + angle/numSteps);
            }
            else
            {
                setRotation(rotation() - angle/numSteps);
            }
        }

        step = (step + 1) % (numSteps * 2);

        qreal bulletSpeed = 15;

        QPointF muzzleDelta(5, 2);
        if(getAnimationDirection() == ANIM_LEFT)
        {
            muzzleDelta.setX(19);
            bulletSpeed = -bulletSpeed;
        }

        QPointF bulletVelocity(cos(rotation() * 3.14/180) * bulletSpeed, sin(rotation() * 3.14/180) * bulletSpeed);

        Bullet* bullet = new Bullet(bulletVelocity, 50, game, false, true);
        bullet->setPos(parentItem()->pos() + pos() + muzzleDelta);

        qreal position = (parentItem()->pos().x() + pos().x() - game->getPlayer()->pos().x()) / 1600.0;
        position = position < -1 ? -1 : position > 1 ? 1 : position;
        qreal distance = position < 0 ? -position : position;
        shotSound->play(distance, position);
    }
}

void AutoMachineGun::switchDirection()
{
    setAnimationDirection(getAnimationDirection() == ANIM_LEFT ? ANIM_RIGHT : ANIM_LEFT);
    setRotation(-rotation());
    setTransformOriginPoint(getAnimationDirection() == ANIM_LEFT ? 24 - 5.5 : 5.5, 5.5);
}

void AutoMachineGun::setStep(int newStep)
{
    step = newStep;
}
