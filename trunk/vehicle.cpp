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

#include "vehicle.h"

Vehicle::Vehicle(QString const& definitionFile, Game* game) :
    Actor(definitionFile, game)
{
    setZValue(1);
}

void Vehicle::update()
{
    preUpdate();
    if(!frozen)
    {
        Level* level = game->getLevel();

        QPointF nextPos(pos());

        nextPos.setX(nextPos.x() + velocity.x());

        if(velocity.x() > 0)
        {
            QPointF const rightDelta1 = QPointF(pixmap().width() - 1, pixmap().height()/8);
            QPointF const rightDelta2 = QPointF(pixmap().width() - 1, 7*pixmap().height()/8);
            AbstractBlock* block1 = level->getBlockAt(nextPos + rightDelta1);
            AbstractBlock* block2 = level->getBlockAt(nextPos + rightDelta2);
            if(block1)
            {
                qreal blockLeft1 = block1->getLeft(nextPos.y() + rightDelta1.y());
                if(blockLeft1 != -1 && blockLeft1 <= nextPos.x() + rightDelta1.x())
                {
                    nextPos.setX(blockLeft1 - rightDelta1.x() - 1);
                    hitRightWall();
                }
            }
            if(block2)
            {
                qreal blockLeft2 = block2->getLeft(nextPos.y() + rightDelta2.y());
                if(blockLeft2 != -1 && blockLeft2 <= nextPos.x() + rightDelta2.x())
                {
                    nextPos.setX(blockLeft2 - rightDelta2.x() - 1);
                    hitRightWall();
                }
            }
        }
        else if(velocity.x() < 0)
        {
            QPointF const leftDelta1 = QPointF(0, pixmap().height()/8);
            QPointF const leftDelta2 = QPointF(0, 7*pixmap().height()/8);
            AbstractBlock* block1 = level->getBlockAt(nextPos + leftDelta1);
            AbstractBlock* block2 = level->getBlockAt(nextPos + leftDelta2);
            if(block1)
            {
                qreal blockRight1 = block1->getRight(nextPos.y() + leftDelta1.y());
                if(blockRight1 != -1 && blockRight1 >= nextPos.x() + leftDelta1.x())
                {
                    nextPos.setX(blockRight1 - leftDelta1.x() + 1);
                    hitLeftWall();
                }
            }
            if(block2)
            {
                qreal blockRight2 = block2->getRight(nextPos.y() + leftDelta2.y());
                if(blockRight2 != -1 && blockRight2 >= nextPos.x() + leftDelta2.x())
                {
                    nextPos.setX(blockRight2 - leftDelta2.x() + 1);
                    hitLeftWall();
                }
            }
        }

        nextPos.setY(nextPos.y() + velocity.y());


        if(velocity.y() > 0)
        {
            QPointF feetDelta = QPointF(0, pixmap().height() - 1);

            onGround = false;

            int numPoints = (pixmap().width() - 1)/47 + 2;
            for(int i = 0; i < numPoints + 1; ++i)
            {
                feetDelta.setX(i * (pixmap().width() - 1) / numPoints);
                AbstractBlock* block = level->getBlockAt(nextPos + feetDelta);
                if(block)
                {
                    qreal blockTop = block->getTop(nextPos.x() + feetDelta.x());
                    if(blockTop != -1 && blockTop <= nextPos.y() + feetDelta.y() && blockTop + 10 > pos().y() + feetDelta.y())
                    {
                        nextPos.setY(blockTop - feetDelta.y() - 1);
                        onGround = true;
                        hitFloor();
                    }
                }
            }
        }
        else if(velocity.y() < 0)
        {
            onGround = false;

            QPointF const headDelta1 = QPointF(0, 0);
            QPointF const headDelta2 = QPointF(pixmap().width() - 1, 0);

            AbstractBlock* block1 = level->getBlockAt(nextPos + headDelta1);
            if(block1)
            {
                qreal blockBottom = block1->getBottom(nextPos.x() + headDelta1.x());
                if(blockBottom != -1 && blockBottom >= nextPos.y() + headDelta1.y())
                {
                    nextPos.setY(blockBottom - headDelta1.y());
                    hitCeiling();
                }
            }

            AbstractBlock* block2 = level->getBlockAt(nextPos + headDelta2);
            if(block2)
            {
                qreal blockBottom = block2->getBottom(nextPos.x() + headDelta2.x());
                if(blockBottom != -1 && blockBottom >= nextPos.y() + headDelta2.y())
                {
                    nextPos.setY(blockBottom - headDelta2.y());
                    hitCeiling();
                }
            }
        }

        setPos(nextPos);
    }

    postUpdate();
    animate();
}

bool Vehicle::isHitting(QPointF point)
{
    QPointF centerDelta(pixmap().width()/2, pixmap().height()/2);
    QPointF diff = point - (pos() + centerDelta);
    qreal rSquared = pixmap().height()/2 * pixmap().height()/2;
    qreal dSquared = diff.x() * diff.x() + diff.y() * diff.y();
    return dSquared < rSquared;
}

void Vehicle::hitLeftWall()
{
    velocity.setX(0);
}

void Vehicle::hitRightWall()
{
    velocity.setX(0);
}

void Vehicle::hitCeiling()
{
    velocity.setY(0);
}

void Vehicle::hitFloor()
{
    velocity.setY(0);
}
