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

#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <QKeyEvent>
#include <QObject>
#include <QGraphicsScene>

class ApplicationState : public QObject
{
    Q_OBJECT
public:
    ApplicationState(QObject* parent = 0) : QObject(parent) {}

    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleKeyPressEvent(QKeyEvent *event) = 0;
    virtual void handleKeyReleaseEvent(QKeyEvent *event) = 0;
    virtual void update() {}
    virtual QGraphicsScene* getScene() const = 0;
};

#endif // APPLICATIONSTATE_H
