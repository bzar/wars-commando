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

#include "enemymediumtank.h"
#include "cannonball.h"

EnemyMediumTank::EnemyMediumTank(Game* game) :
    EnemyTank(":/img/mediumTank.txt", game)
{
    machineGun1 = new AutoMachineGun(this, game, 45);
    machineGun2 = new AutoMachineGun(this, game, 115);
    machineGun1->setPos(30, 74);
    machineGun2->setPos(30, 54);
    machineGun1->setRotation(45);
    machineGun2->setRotation(25);
    life = MAX_LIFE;
}

void EnemyMediumTank::preUpdate()
{
    EnemyTank::preUpdate();
}

void EnemyMediumTank::postUpdate()
{
    if(!frozen && state != STATE_DYING)
    {
        machineGun1->update();
        machineGun2->update();
    }

    EnemyTank::postUpdate();
}

void EnemyMediumTank::switchDirection()
{
    EnemyTank::switchDirection();
    machineGun1->switchDirection();
    machineGun2->switchDirection();
}

void EnemyMediumTank::shoot()
{
    QPointF shotVelocity(10,0);
    QPointF muzzleDelta(pixmap().width() - 30, 12);
    if(getAnimationDirection() == ANIM_LEFT)
    {
        shotVelocity.setX(-shotVelocity.x());
        muzzleDelta.setX(30);
    }
    CannonBall* cannonBall = new CannonBall(shotVelocity, 50, game, CannonBall::MEDIUM);
    cannonBall->setPos(pos() + muzzleDelta);
    cannonSound->play();
}
