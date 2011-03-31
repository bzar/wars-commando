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

#include "enemyheavytank.h"
#include "cannonball.h"

EnemyHeavyTank::EnemyHeavyTank(Game* game) :
    EnemyTank(":/img/heavyTank.txt", game)
{
    machineGun1 = new AutoMachineGun(this, game, 45, 7);
    machineGun2 = new AutoMachineGun(this, game, 115, 10);
    machineGun3 = new AutoMachineGun(this, game, 180, 18);
    machineGun4 = new AutoMachineGun(this, game, 225, 40);

    machineGun2->setStep(0);
    machineGun2->setStep(5);
    machineGun3->setStep(6);
    machineGun4->setStep(30);

    machineGun1->setPos(50, 40);
    machineGun2->setPos(130, 100);
    machineGun3->setPos(30, 130);
    machineGun4->setPos(90, 100);

    machineGun1->setRotation(22.5);
    machineGun2->setRotation(45);
    machineGun3->setRotation(0);
    machineGun4->setRotation(22.5);
    life = MAX_LIFE;
}

void EnemyHeavyTank::preUpdate()
{
    EnemyTank::preUpdate();
}

void EnemyHeavyTank::postUpdate()
{
    if(!frozen && state != STATE_DYING)
    {
        machineGun1->update();
        machineGun2->update();
        machineGun3->update();
        machineGun4->update();
    }

    EnemyTank::postUpdate();
}

void EnemyHeavyTank::switchDirection()
{
    EnemyTank::switchDirection();
    machineGun1->switchDirection();
    machineGun2->switchDirection();
    machineGun3->switchDirection();
    machineGun4->switchDirection();
}

void EnemyHeavyTank::shoot()
{
    QPointF shotVelocity(10,0);
    QPointF muzzleDelta(pixmap().width() - 64, 16);
    if(getAnimationDirection() == ANIM_LEFT)
    {
        shotVelocity.setX(-shotVelocity.x());
        muzzleDelta.setX(0);
    }
    CannonBall* cannonBall = new CannonBall(shotVelocity, 50, game, CannonBall::LARGE);
    cannonBall->setPos(pos() + muzzleDelta);
    cannonSound->play();
}
