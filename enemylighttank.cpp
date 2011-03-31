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

#include "enemylighttank.h"

EnemyLightTank::EnemyLightTank(Game* game) :
    EnemyTank(":/img/lightTank.txt", game)
{
    machineGun = new AutoMachineGun(this, game);
    machineGun->setPos(24, 24);

    life = MAX_LIFE;
}

void EnemyLightTank::preUpdate()
{
    EnemyTank::preUpdate();
}

void EnemyLightTank::postUpdate()
{
    if(!frozen && state != STATE_DYING)
    {
        machineGun->update();
    }

    EnemyTank::postUpdate();
}

void EnemyLightTank::switchDirection()
{
    EnemyTank::switchDirection();
    machineGun->switchDirection();
}
