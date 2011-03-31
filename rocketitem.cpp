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

#include "rocketitem.h"

#include "player.h"

RocketItem::RocketItem(Game* game) :
    Particle(":/img/rocketItem.txt", game), life(100), game(game)
{
    game->addParticle(this);
    pickupSound = game->getResourceManager().getSound("pickup");
}

void RocketItem::update()
{
    --life;
    animate();

    QPointF centerDelta(pixmap().width()/2, pixmap().height()/2);
    if(game->getPlayer()->isHitting(pos() + centerDelta))
    {
        game->addRockets();
        life = 0;
        pickupSound->play();
    }
}

bool RocketItem::alive()
{
    return life > 0;
}

