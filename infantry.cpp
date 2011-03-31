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

#include "infantry.h"
#include "bullet.h"

Infantry::Infantry(QString const& definitionFile, Game* game) :
    Actor(definitionFile, game)
{
    setZValue(1);
}

void Infantry::update()
{
    preUpdate();
    if(!frozen)
    {
        Level* level = game->getLevel();

        QPointF nextPos(pos());

        nextPos.setX(nextPos.x() + velocity.x());

        if(velocity.x() > 0)
        {
            QPointF const rightDelta1 = QPointF(3*pixmap().width()/5+1, pixmap().height()/8);
            QPointF const rightDelta2 = QPointF(3*pixmap().width()/5+1, 7*pixmap().height()/8);
            AbstractBlock* block1 = level->getBlockAt(nextPos + rightDelta1);
            AbstractBlock* block2 = level->getBlockAt(nextPos + rightDelta2);
            if(block1)
            {
                qreal blockLeft1 = block1->getLeft(nextPos.y() + rightDelta1.y());
                if(blockLeft1 != -1 && blockLeft1 <= nextPos.x() + rightDelta1.x())
                {
                    nextPos.setX(blockLeft1 - rightDelta1.x() - 1);
                    hitRightWall();
                    if(block1->hurts()) hurt();
                }
            }
            if(block2)
            {
                qreal blockLeft2 = block2->getLeft(nextPos.y() + rightDelta2.y());
                if(blockLeft2 != -1 && blockLeft2 <= nextPos.x() + rightDelta2.x())
                {
                    nextPos.setX(blockLeft2 - rightDelta2.x() - 1);
                    hitRightWall();
                    if(block2->hurts()) hurt();
                }
            }
        }
        else if(velocity.x() < 0)
        {
            QPointF const leftDelta1 = QPointF(2*pixmap().width()/5-1, pixmap().height()/8);
            QPointF const leftDelta2 = QPointF(2*pixmap().width()/5-1, 7*pixmap().height()/8);
            AbstractBlock* block1 = level->getBlockAt(nextPos + leftDelta1);
            AbstractBlock* block2 = level->getBlockAt(nextPos + leftDelta2);
            if(block1)
            {
                qreal blockRight1 = block1->getRight(nextPos.y() + leftDelta1.y());
                if(blockRight1 != -1 && blockRight1 >= nextPos.x() + leftDelta1.x())
                {
                    nextPos.setX(blockRight1 - leftDelta1.x() + 1);
                    hitLeftWall();
                    if(block1->hurts()) hurt();
                }
            }
            if(block2)
            {
                qreal blockRight2 = block2->getRight(nextPos.y() + leftDelta2.y());
                if(blockRight2 != -1 && blockRight2 >= nextPos.x() + leftDelta2.x())
                {
                    nextPos.setX(blockRight2 - leftDelta2.x() + 1);
                    hitLeftWall();
                    if(block2->hurts()) hurt();
                }
            }
        }

        nextPos.setY(nextPos.y() + velocity.y());


        if(velocity.y() > 0)
        {
            QPointF const feetDelta = QPointF(2*pixmap().width()/5, pixmap().height() - 1);
            QPointF const feetDelta2 = QPointF(3*pixmap().width()/5, pixmap().height() - 1);
            AbstractBlock* prevBlock = level->getBlockAt(pos() + feetDelta);
            AbstractBlock* block = 0;

            // Allow jumping from downhill slopes
            QPointF const foo = QPointF(nextPos.x() + pixmap().width()/2, nextPos.y() + pixmap().height()+10);
            onGround = onGround && level->getBlockAt(foo);

            while((block = level->getBlockAt(nextPos + feetDelta)) && block != prevBlock)
            {
                prevBlock = block;
                qreal blockTop = block->getTop(nextPos.x() + feetDelta.x());
                if(blockTop != -1 && blockTop <= nextPos.y() + feetDelta.y() && blockTop + 10 > pos().y() + feetDelta.y())
                {
                    nextPos.setY(blockTop - feetDelta.y() - 1);
                    onGround = true;
                    hitFloor();
                    if(block->hurts()) hurt();
                }
            }
            while((block = level->getBlockAt(nextPos + feetDelta2)) && block != prevBlock)
            {
                prevBlock = block;
                qreal blockTop = block->getTop(nextPos.x() + feetDelta2.x());
                if(blockTop != -1 && blockTop <= nextPos.y() + feetDelta2.y() && blockTop + 10 > pos().y() + feetDelta2.y())
                {
                    nextPos.setY(blockTop - feetDelta2.y() - 1);
                    onGround = true;
                    hitFloor();
                    if(block->hurts()) hurt();
                }
            }
        }
        else if(velocity.y() < 0)
        {
            onGround = false;

            QPointF const headDelta = QPointF(2*pixmap().width()/5, 2);
            QPointF const headDelta2 = QPointF(3*pixmap().width()/5, 2);
            AbstractBlock* block = level->getBlockAt(nextPos + headDelta);
            if(block)
            {
                qreal blockBottom = block->getBottom(nextPos.x() + headDelta.x());
                if(blockBottom != -1 && blockBottom >= nextPos.y() + headDelta.y())
                {
                    nextPos.setY(blockBottom - headDelta.y());
                    hitCeiling();
                    if(block->hurts()) hurt();
                }
            }
            block = level->getBlockAt(nextPos + headDelta2);
            if(block)
            {
                qreal blockBottom = block->getBottom(nextPos.x() + headDelta2.x());
                if(blockBottom != -1 && blockBottom >= nextPos.y() + headDelta2.y())
                {
                    nextPos.setY(blockBottom - headDelta2.y());
                    hitCeiling();
                    if(block->hurts()) hurt();
                }
            }
        }

        setPos(nextPos);
    }

    postUpdate();
    animate();
}

bool Infantry::isHitting(QPointF point)
{
    QPointF centerDelta(pixmap().width()/2, pixmap().height()/2);
    QPointF diff = point - (pos() + centerDelta);
    qreal rSquared = pixmap().height()/2 * pixmap().height()/2;
    qreal dSquared = diff.x() * diff.x() * 2 + diff.y() * diff.y();
    return dSquared < rSquared;
}

void Infantry::hurt()
{

}

bool Infantry::alive()
{
    return true;
}

void Infantry::preUpdate()
{

}

void Infantry::postUpdate()
{

}

void Infantry::hitLeftWall()
{
    velocity.setX(0);
}

void Infantry::hitRightWall()
{
    velocity.setX(0);
}

void Infantry::hitCeiling()
{
    velocity.setY(0);
}

void Infantry::hitFloor()
{
    velocity.setY(0);
}

void Infantry::hitByBullet(Bullet* bullet)
{

}

void Infantry::hitByRocket(Rocket* rocket)
{

}
