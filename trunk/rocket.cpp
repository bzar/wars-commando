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

#include "rocket.h"
#include "actor.h"
#include "player.h"
#include "bullet.h"

#include <QtCore/qmath.h>

Rocket::Rocket(QPointF velocity, int life, Game* game, bool playerRocket) :
    Particle(":/img/rocket.txt", game), game(game), velocity(velocity), life(life), playerRocket(playerRocket)
{
    explosionSound = game->getResourceManager().getSound("grenade");
    game->addParticle(this);
    if(velocity.x() < 0)
    {
        setAnimationDirection(ANIM_LEFT);
    }
}

void Rocket::update()
{
    if(abs(velocity.x()) < 30)
    {
        velocity.setX(velocity.x() * 1.05);
    }
    setPos(pos() + velocity);
    --life;

    QPointF centerDelta(pixmap().width() - 4, pixmap().height()/2);
    if(velocity.x() < 0)
    {
        centerDelta.setX(4);
    }
    QPointF center = pos() + centerDelta;
    AbstractBlock* block = game->getLevel()->getBlockAt(center);


    if(block && ((velocity.y() > 0  && block->getTop(pos().x()) != -1) ||
                 (velocity.y() < 0  && block->getBottom(pos().x()) != -1) ||
                 (velocity.x() > 0  && block->getLeft(pos().y()) != -1) ||
                 (velocity.x() < 0  && block->getRight(pos().y()) != -1)))
    {
        explode();
    }

    if(alive())
    {
        if(playerRocket)
        {
            Actor* enemy = game->getEnemyAt(center);
            if(enemy)
            {
                enemy->hitByRocket(this);
            }
        }
        else
        {
            if(game->getPlayer()->isHitting(center))
            {
                game->getPlayer()->hitByRocket(this);
            }
        }
    }

    animate();
}

bool Rocket::alive()
{
    return life > 0;
}

void Rocket::destroy()
{
    life = 0;
}

void Rocket::explode()
{
    QPointF centerDelta(pixmap().width() - 4, pixmap().height()/2);
    if(velocity.x() < 0)
    {
        centerDelta.setX(4);
    }
    QPointF center = pos() + centerDelta;

    life = 0;

    qreal position = (pos().x() - game->getPlayer()->pos().x()) / 1600.0;
    position = position < -1 ? -1 : position > 1 ? 1 : position;
    qreal distance = position < 0 ? -position : position;
    explosionSound->play(distance, position);

    QPointF explosionPos(center.x() - velocity.x(), center.y());
    velocity.setX(0);
    velocity.setY(0);
    for(qreal i = 0; i < 8; ++i)
    {
        Bullet* bullet = new Bullet(QPointF(20*qFastCos(2*M_PI*i/8), 20*qFastSin(2*M_PI*i/8)), 10, game, playerRocket);
        bullet->setPos(explosionPos);

    }
}

QPointF Rocket::getVelocity()
{
    return velocity;
}
