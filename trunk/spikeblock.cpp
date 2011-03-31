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

#include "spikeblock.h"

SpikeBlock::SpikeBlock(QPixmap image, QGraphicsScene* scene, Orientation orientation) :
    AbstractBlock(scene), image(image), orientation(orientation)
{
    setPixmap(image);
    setTransformOriginPoint(image.width()/2.0, image.height()/2.0);
    setRotation(orientation * 90);
}


bool SpikeBlock::containsPoint(QPointF const& point)
{
    if(point.x() < pos().x() || point.x() > pos().x() + pixmap().width() ||
       point.y() < pos().y() || point.y() > pos().y() + pixmap().height())
    {
        return false;
    }

    switch(orientation)
    {
    case BOTTOM: return point.y() >= pos().y() + pixmap().height() - SPIKE_HEIGHT;
    case TOP: return point.y() <= pos().y() + SPIKE_HEIGHT;
    case RIGHT: return point.x() >= pos().x() + pixmap().width() - SPIKE_HEIGHT;
    case LEFT: return point.x() <= pos().x() + SPIKE_HEIGHT;
    default: return false;
    }
}

qreal SpikeBlock::getTop(qreal x)
{
    switch(orientation)
    {
    case BOTTOM: return pos().y() + pixmap().height() - SPIKE_HEIGHT;
    case TOP: return pos().y();
    case RIGHT: return pos().y();
    case LEFT: return pos().y();
    default: return 0.0;
    }
}

qreal SpikeBlock::getBottom(qreal x)
{
    switch(orientation)
    {
    case BOTTOM: return pos().y() + pixmap().height();
    case TOP: return pos().y() + SPIKE_HEIGHT;
    case RIGHT: return pos().y() + pixmap().height();
    case LEFT: return pos().y() + pixmap().height();
    default: return 0.0;
    }
}

qreal SpikeBlock::getLeft(qreal y)
{
    switch(orientation)
    {
    case BOTTOM: return pos().x();
    case TOP: return pos().x();
    case RIGHT: return pos().x() + pixmap().width() - SPIKE_HEIGHT;
    case LEFT: return pos().x();
    default: return 0.0;
    }
}

qreal SpikeBlock::getRight(qreal y)
{
    switch(orientation)
    {
    case BOTTOM: return pos().x() + pixmap().width();
    case TOP: return pos().x() + pixmap().width();
    case RIGHT: return pos().x() + pixmap().width();
    case LEFT: return pos().x() + SPIKE_HEIGHT;
    default: return 0.0;
    }
}

bool SpikeBlock::hurts()
{
    return true;
}
