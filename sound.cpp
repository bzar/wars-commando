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

#include "sound.h"

Sound::Sound(QString filename, QObject *parent) :
    QObject(parent)
{
    buffer = Mix_LoadWAV(filename.toAscii());
    if(!buffer)
    {
        qDebug(QString("Error loading sound: %1").arg(filename).toAscii());
        qDebug(Mix_GetError());
    }
}

Sound::~Sound()
{
    Mix_FreeChunk(buffer);
}

void Sound::init()
{
    SDL_Init(SDL_INIT_AUDIO);

    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 2048;

    Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
}

void Sound::exit()
{
    Mix_CloseAudio();
    SDL_Quit();
}

int Sound::play(qreal distance, qreal position, int loops)
{
    int channel = Mix_PlayChannel(-1, buffer, loops);
    Mix_SetPosition(channel, static_cast<int>(360 + position*90) % 360, static_cast<int>(distance * 255));
    return channel;
}
