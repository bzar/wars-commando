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

#include "jumpthroughblock.h"

JumpThroughBlock::JumpThroughBlock(QPixmap image, QGraphicsScene* scene) :
    AbstractBlock(scene), image(image)
{
    setPixmap(image);
}

bool JumpThroughBlock::containsPoint(QPointF const& point)
{
    QPointF diff = point - pos();
    return diff.x() >= 0 && diff.y() >= 0 && diff.x() < pixmap().width() && diff.y() < pixmap().height();
}

qreal JumpThroughBlock::getTop(qreal x)
{
    return pos().y();
}

qreal JumpThroughBlock::getBottom(qreal x)
{
    return -1;
}

qreal JumpThroughBlock::getLeft(qreal y)
{
    return -1;
}

qreal JumpThroughBlock::getRight(qreal y)
{
    return -1;
}
