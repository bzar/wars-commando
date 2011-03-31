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

#include "slopeblock.h"

SlopeBlock::SlopeBlock(QPixmap image, qreal h1, qreal h2, QGraphicsScene* scene) :
    AbstractBlock(scene), image(image), h1(h1), h2(h2)
{
    setPixmap(image);
}

bool SlopeBlock::containsPoint(QPointF const& point)
{
    QPointF diff = point - pos();
    return diff.x() >= 0 && diff.y() >= 0 && diff.x() <= pixmap().width() && getTop(point.x()) <= point.y();
}

qreal SlopeBlock::getTop(qreal x)
{
    qreal rx = (x - pos().x())/pixmap().width();
    qreal ry = h1 + rx * (h2 - h1);
    return pos().y() + ry * pixmap().height();
}

qreal SlopeBlock::getBottom(qreal x)
{
    return pos().y() + pixmap().height();
}

qreal SlopeBlock::getLeft(qreal y)
{
    if((y - pos().y()) >= h1 * pixmap().height())
    {
        return pos().x();
    }
    else if((y - pos().y()) >= h2 * pixmap().height())
    {
        qreal h = (y - pos().y()) / pixmap().height();
        qreal rh =  (h - h1) / (h2 - h1);
        return pos().x() + rh * pixmap().width();
    }
    else
    {
        return -1;
    }
}

qreal SlopeBlock::getRight(qreal y)
{
    if((y - pos().y()) >= h2 * pixmap().height())
    {
        return pos().x() + pixmap().width();
    }
    else if((y - pos().y()) >= h1 * pixmap().height())
    {
        qreal h = (y - pos().y()) / pixmap().height();
        qreal rh =  (h - h1) / (h2 - h1);
        return pos().x() + rh * pixmap().width();
    }
    else
    {
        return -1;
    }
}

