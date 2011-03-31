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

#ifndef ENEMYHEAVYTANK_H
#define ENEMYHEAVYTANK_H

#include "enemytank.h"

class EnemyHeavyTank : public EnemyTank
{
public:
    EnemyHeavyTank(Game* game);

    void preUpdate();
    void postUpdate();

protected:
    virtual void switchDirection();
    virtual void shoot();

private:
    static int const MAX_LIFE = 500;

    AutoMachineGun* machineGun1;
    AutoMachineGun* machineGun2;
    AutoMachineGun* machineGun3;
    AutoMachineGun* machineGun4;

};

#endif // ENEMYHEAVYTANK_H
