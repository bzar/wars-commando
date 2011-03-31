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

#ifndef LEVEL_H
#define LEVEL_H

class Game;

#include "abstractblock.h"
#include <QList>
#include <QPointF>
#include <QGraphicsItem>
#include <QPixmap>

class Level
{
public:
    Level(QString const& mapFilename, QString const& blockFilename, Game* game);

    AbstractBlock* getBlockAt(QPointF point);

    int getWidth();
    int getHeight();

private:
    Game* game;
    QList<QPixmap> blockImages;
    QList<QList<AbstractBlock*> > blocks;

    int width;
    int height;

    static int const BLOCK_SIZE = 48;
};

#endif // LEVEL_H
