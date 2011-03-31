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

#ifndef ANIMATED_H
#define ANIMATED_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "resourcemanager.h"

class Animated : public QGraphicsPixmapItem
{
public:
    enum AnimationDirection {ANIM_LEFT, ANIM_RIGHT};

    Animated(QString const& definitionFilename, ResourceManager const& resources, QGraphicsScene* scene);
    void animate();
    void setAnimation(int const& animationIndex, AnimationDirection direction);
    void turnAnimation();

    int getAnimation() const;
    AnimationDirection getAnimationDirection() const;
    void setAnimationDirection(AnimationDirection direction);
    int getAnimationFrame() const;
    void setAnimationFrame(int value);
    int getAnimationSpeed() const;
    void setAnimationSpeed(int value);
    int getAnimationLength(int animationIndex);
    void flash();

private:
    void addAnimation(QPixmap sheet, int w, int h, int x, int y, int n);

    // animation - direction - frames
    QList< QList< QList<QPixmap> > > pixmaps;

    int frame;
    int animation;
    AnimationDirection animationDirection;
    int animationSpeed;
    bool flashing;
};

#endif // ANIMATED_H
