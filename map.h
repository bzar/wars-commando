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

#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QVector>
#include <QGraphicsScene>
#include <QTimeLine>

#include "resourcemanager.h"
#include "animated.h"
#include "music.h"

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = 0);
    QGraphicsScene* getScene();

    void enter();
    void exit();

    bool ready();

public slots:
    void reset();
    void update();
    void nextLevel();
    bool hasNextLevel();
    void setLives(int const lives);
    void skipAnimation();

private slots:
    void setStandingAnimations();
    void animationEnd();

private:

    QGraphicsScene* scene;
    ResourceManager resources;

    Music* music;

    int currentLevel;

    QGraphicsPixmapItem* speechBubble;
    QGraphicsPixmapItem* speechBubbleText;
    QVector<QPixmap> texts;

    QGraphicsPixmapItem* banner;
    QGraphicsPixmapItem* levelNumber;
    QGraphicsPixmapItem* livesNumber;

    Animated* player;
    Animated* copter;

    Animated* enemy1;
    Animated* enemy2;
    Animated* enemy3;
    Animated* enemy4;
    Animated* enemy5;
    Animated* enemy6;
    Animated* enemy7;
    Animated* enemy8;

    bool animationFinished;
    QTimeLine* tl;
};

#endif // MAP_H
