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

#include "ambientsound.h"

AmbientSound::AmbientSound(Sound* sound) :
    sound(sound), channel(-1), playing(false)
{
}

void AmbientSound::play(qreal distance, qreal position)
{
    channel = sound->play(distance, position, -1);
    playing = true;
}

void AmbientSound::stop()
{
    Mix_Pause(channel);
    playing = false;
}

bool AmbientSound::isPlaying()
{
    return playing;
}

void AmbientSound::setPosition(qreal distance, qreal position)
{
    Mix_SetPosition(channel, static_cast<int>(360 + position*90) % 360, static_cast<int>(distance * 255));
}
