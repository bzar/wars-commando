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

#ifndef SOUND_H
#define SOUND_H

#include <QObject>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sound : public QObject
{
    Q_OBJECT
public:
    explicit Sound(QString filename, QObject *parent = 0);
    ~Sound();

    static void init();
    static void exit();

public slots:
    int play(qreal distance = 0.0, qreal position = 0.0, int loops = 0);

signals:

private:
    Mix_Chunk *buffer;
};

#endif // SOUND_H
