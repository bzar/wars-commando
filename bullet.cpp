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

#include "bullet.h"
#include "actor.h"
#include "player.h"

Bullet::Bullet(QPointF velocity, int life, Game* game, bool playerBullet, bool shootInsideBlock) :
    Particle(":/img/bullet.txt", game), game(game), velocity(velocity), life(life), playerBullet(playerBullet), shootInsideBlock(shootInsideBlock)
{
    game->addParticle(this);
    setZValue(2);
}

void Bullet::update()
{
    AbstractBlock* prevBlock = game->getLevel()->getBlockAt(pos());
    setPos(pos() + velocity);
    QPointF centerDelta(pixmap().width()/2.0, pixmap().height()/2.0);
    AbstractBlock* block = game->getLevel()->getBlockAt(pos() + centerDelta);
    --life;

    if(block && !(block == prevBlock && shootInsideBlock)  &&
       ((velocity.y() > 0  && block->getTop(pos().x()) != -1) ||
        (velocity.y() < 0  && block->getBottom(pos().x()) != -1) ||
        (velocity.x() > 0  && block->getLeft(pos().y()) != -1) ||
        (velocity.x() < 0  && block->getRight(pos().y()) != -1)))
    {
        life = 0;
    }

    if(alive())
    {
        if(playerBullet)
        {
            Actor* enemy = game->getEnemyAt(pos() + centerDelta);
            if(enemy)
            {
                enemy->hitByBullet(this);
            }
        }
        else
        {
            if(game->getPlayer()->isHitting(pos() + centerDelta))
            {
                game->getPlayer()->hitByBullet(this);
            }
        }
    }
}

bool Bullet::alive()
{
    return life > 0;
}

void Bullet::destroy()
{
    life = 0;
}

QPointF Bullet::getVelocity()
{
    return velocity;
}
