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

#include "endstate.h"
#include <QGraphicsItemAnimation>

#include <QGraphicsItem>

EndState::EndState(GameWindow* window) :
    ApplicationState(window), window(window), scene(new QGraphicsScene(this)), music(0), tl(0)
{
    scene->setBackgroundBrush(QBrush(QPixmap(":/img/end.png")));
    cheater = scene->addPixmap(QPixmap(":/img/cheater.png"));
    cheater->setPos(100, 200);
    music = new Music("music/01.ogg", this);
}

void EndState::enter()
{
    window->setSceneRect(0, 0, 800, 480);
    music->play();
}

void EndState::exit()
{
    music->stop();
}

void EndState::handleKeyPressEvent(QKeyEvent *event)
{
    window->reset();
}

void EndState::handleKeyReleaseEvent(QKeyEvent *event)
{

}

void EndState::update()
{

}

QGraphicsScene* EndState::getScene() const
{
    return scene;
}

void EndState::setCheater(bool value)
{
    if(value)
    {
        cheater->show();
        tl = new QTimeLine(3000, this);
        QGraphicsItemAnimation* anim = new QGraphicsItemAnimation(tl);
        anim->setTimeLine(tl);
        anim->setItem(cheater);
        anim->setPosAt(0, QPointF(100, 100));
        anim->setPosAt(0.5, QPointF(100, 300));
        anim->setPosAt(1, QPointF(100, 100));
        tl->setCurveShape(QTimeLine::SineCurve);
        tl->setLoopCount(0);
        tl->start();
    }
    else
    {
        if(tl)
        {
            delete tl;
        }
        cheater->hide();
    }
}
