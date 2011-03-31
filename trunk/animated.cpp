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

#include "animated.h"
#include <QFile>
#include <QTextStream>
#include <QBitmap>

Animated::Animated(QString const& definitionFilename, ResourceManager const& resources, QGraphicsScene* scene) :
    QGraphicsPixmapItem(0, scene), pixmaps(), frame(-1), animation(0), animationDirection(ANIM_RIGHT), animationSpeed(1), flashing(false)
{
    QFile definitionFile(definitionFilename);
    definitionFile.open(QIODevice::ReadOnly);
    QTextStream definitionStream(&definitionFile);

    int width = 0;
    int height = 0;
    QPixmap imageSheet;

    QString propertyName;
    while(!definitionStream.atEnd())
    {
        definitionStream >> propertyName;

        if(propertyName == "sheet")
        {
            QString imageName;
            definitionStream >> imageName;
            imageSheet = resources.getImage(imageName);
            if(imageSheet.isNull())
            {
                qDebug(QString("Animation sheet not found: %1").arg(imageName).toAscii());
            }
        }
        else if(propertyName == "width")
        {
            definitionStream >> width;
        }
        else if(propertyName == "height")
        {
            definitionStream >> height;
        }
        else if(propertyName == "speed")
        {
            definitionStream >> animationSpeed;
        }
        else if(propertyName == "animation")
        {
            QString name;
            int x = 0;
            int y = 0;
            int n = 0;
            definitionStream >> name >> x >> y >> n;
            addAnimation(imageSheet, width, height, x, y, n);
        }
    }
    animate();
}

void Animated::animate()
{
    frame = (frame + 1) % (pixmaps.at(animation).at(animationDirection).size() * animationSpeed);
    setPixmap(pixmaps.at(animation).at(animationDirection).at(frame/animationSpeed));

    if(flashing)
    {
        QPixmap copy = pixmap().copy();
        QBitmap bitMask = copy.mask();
        copy.fill(Qt::white);
        copy.setMask(bitMask);
        setPixmap(copy);
        flashing = false;
    }

}

void Animated::setAnimation(int const& animationIndex, AnimationDirection direction)
{
    animation = animationIndex;
    animationDirection = direction;
}

void Animated::turnAnimation()
{
    if(animationDirection == ANIM_LEFT)
    {
        animationDirection = ANIM_RIGHT;
    }
    else
    {
        animationDirection = ANIM_LEFT;
    }
}

int Animated::getAnimationLength(int animationIndex)
{
    return pixmaps.at(animationIndex).at(0).size();
}

void Animated::flash() {
    flashing = true;
}

void Animated::addAnimation(QPixmap sheet, int w, int h, int x, int y, int n)
{
    QList< QList<QPixmap> > directions;
    QList<QPixmap> leftFrames;
    QList<QPixmap> rightFrames;

    for(int i = 0; i < n; ++i)
    {
        rightFrames.append(sheet.copy((x+i)*w, y*h, w, h));
        leftFrames.append(QPixmap::fromImage(sheet.copy((x+i)*w, y*h, w, h).toImage().mirrored(true, false)));
    }

    directions.append(leftFrames);
    directions.append(rightFrames);
    pixmaps.append(directions);
}

int Animated::getAnimation() const
{
    return animation;
}

Animated::AnimationDirection Animated::getAnimationDirection() const
{
    return animationDirection;
}

void Animated::setAnimationDirection(AnimationDirection direction)
{
    animationDirection = direction;
}

int Animated::getAnimationFrame() const
{
    return frame;
}

void Animated::setAnimationFrame(int value)
{
    frame = value;
}

int Animated::getAnimationSpeed() const
{
    return animationSpeed;
}

void Animated::setAnimationSpeed(int value)
{
    animationSpeed = value;
}
