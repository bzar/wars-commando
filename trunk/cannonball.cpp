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

#include "cannonball.h"
#include "actor.h"
#include "player.h"
#include "bullet.h"

#include <QtCore/qmath.h>

QString const definitionFiles[3] = {":/img/smallCannonBall.txt", ":/img/mediumCannonBall.txt",":/img/largeCannonBall.txt"};
int const CannonBall::EXPLOSION_BULLETS[3] = {8, 12, 16};

CannonBall::CannonBall(QPointF velocity, int life, Game* game, Size size) :
    Particle(definitionFiles[size], game), game(game), velocity(velocity), life(life), size(size)
{
    explosionSound = game->getResourceManager().getSound("grenade");
    game->addParticle(this);
}

void CannonBall::update()
{
    setPos(pos() + velocity);
    --life;

    QPointF centerDelta(pixmap().width()/2, pixmap().height()/2);
    QPointF center = pos() + centerDelta;
    AbstractBlock* block = game->getLevel()->getBlockAt(center);


    if(!life || (block && ((velocity.y() > 0  && block->getTop(pos().x()) != -1) ||
                 (velocity.y() < 0  && block->getBottom(pos().x()) != -1) ||
                 (velocity.x() > 0  && block->getLeft(pos().y()) != -1) ||
                 (velocity.x() < 0  && block->getRight(pos().y()) != -1))))
    {
        explode();
    }

    if(alive())
    {
        Player* player = game->getPlayer();
        QPointF playerPos(player->pos().x() + player->pixmap().width()/2, player->pos().y() + player->pixmap().height()/2);
        QPointF myPos(pos().x() + pixmap().width()/2, pos().y() + pixmap().height()/2);
        QPointF diff = playerPos - myPos;

        if(diff.x() * diff.x() * 2 + diff.y() * diff.y() <= pixmap().width()/2 * pixmap().width()/2 + player->pixmap().height()/2 * player->pixmap().height()/2)
        {
            game->getPlayer()->hitByCannonBall(this);
        }
    }
}

bool CannonBall::alive()
{
    return life > 0;
}

void CannonBall::destroy()
{
    life = 0;
}

void CannonBall::explode()
{
    QPointF centerDelta(pixmap().width() - 4, pixmap().height()/2);
    if(velocity.x() < 0)
    {
        centerDelta.setX(4);
    }
    QPointF center = pos() + centerDelta;

    life = 0;

    qreal position = (pos().x() - game->getPlayer()->pos().x()) / 800.0;
    position = position < -1 ? -1 : position > 1 ? 1 : position;
    qreal distance = position < 0 ? -position : position;
    explosionSound->play(distance, position);

    QPointF explosionPos(center.x() - velocity.x(), center.y());
    velocity.setX(0);
    velocity.setY(0);
    int numBullets = EXPLOSION_BULLETS[size];
    for(qreal i = 0; i < numBullets; ++i)
    {
        Bullet* bullet = new Bullet(QPointF(20*qFastCos(2*M_PI*i/numBullets), 20*qFastSin(2*M_PI*i/numBullets)), 10, game, false);
        bullet->setPos(explosionPos);

    }
}
