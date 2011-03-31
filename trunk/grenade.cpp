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

#include "grenade.h"
#include "bullet.h"
#include "player.h"

#include <QtCore/qmath.h>

Grenade::Grenade(QPointF velocitya, Game* game, bool playerGrenade) :
    Infantry(":/img/grenade.txt", game), timer(MAX_TIME), playerGrenade(playerGrenade)
{
    velocity = velocitya;
    explosionSound = game->getResourceManager().getSound("grenade");
    setAnimation(ANIM_NORMAL, ANIM_RIGHT);
}

bool Grenade::alive()
{
    return timer != 0 || getAnimation() != ANIM_EXPLODE || getAnimationFrame() != 0;
}

void Grenade::preUpdate()
{
    if(timer)
    {
        velocity.setY(velocity.y() + 1.8);

        if(velocity.x() > 0)
        {
            setAnimation(ANIM_NORMAL, ANIM_RIGHT);
        }
        else if(velocity.x() < 0)
        {
            setAnimation(ANIM_NORMAL, ANIM_LEFT);
        }
        --timer;
    }
    else
    {
        if(getAnimationFrame() == 0)
        {
            qreal position = (pos().x() - game->getPlayer()->pos().x()) / 1600.0;
            position = position < -1 ? -1 : position > 1 ? 1 : position;
            qreal distance = position < 0 ? -position : position;
            explosionSound->play(distance, position);
        }
        velocity.setX(0);
        velocity.setY(0);
        QPointF center = pos();
        center.setX(center.x() + pixmap().width()/2);
        center.setY(center.y() + pixmap().height()/2);
        setAnimation(ANIM_EXPLODE, getAnimationDirection());
        for(qreal i = 0; i < 8; ++i)
        {
            Bullet* bullet = new Bullet(QPointF(20*qFastCos(2*M_PI*i/8), 20*qFastSin(2*M_PI*i/8)), 10, game, playerGrenade);
            bullet->setPos(center);

        }
    }

}

void Grenade::hitFloor()
{
    velocity.setX(velocity.x()/2);
    velocity.setY(-velocity.y()/2);
}

void Grenade::hitLeftWall()
{
    velocity.setX(-velocity.x()/2);
}

void Grenade::hitRightWall()
{
    velocity.setX(-velocity.x()/2);
}
