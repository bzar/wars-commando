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

#ifndef ENEMYMEDIUMTANK_H
#define ENEMYMEDIUMTANK_H

#include "enemytank.h"

class EnemyMediumTank : public EnemyTank
{
public:
    EnemyMediumTank(Game* game);

    void preUpdate();
    void postUpdate();

protected:
    virtual void switchDirection();
    virtual void shoot();

private:
    static int const MAX_LIFE = 250;

    AutoMachineGun* machineGun1;
    AutoMachineGun* machineGun2;
};

#endif // ENEMYMEDIUMTANK_H