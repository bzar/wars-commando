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

#include "music.h"

Music::Music(QString filename, QObject *parent) :
    QObject(parent)
{
    buffer = Mix_LoadMUS(filename.toAscii());
    if(!buffer)
    {
        qDebug(Mix_GetError());
    }
}

Music::~Music()
{
    Mix_FreeMusic(buffer);
}

void Music::play()
{
    if(Mix_PlayMusic(buffer, -1) == -1)
    {
        qDebug("Error playing music");
        qDebug(Mix_GetError());
    }
}


void Music::stop()
{
    Mix_HaltMusic();
}
