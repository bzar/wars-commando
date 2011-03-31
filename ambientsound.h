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

#ifndef AMBIENTSOUND_H
#define AMBIENTSOUND_H

#include "sound.h"

class AmbientSound
{
public:
    explicit AmbientSound(Sound* sound);

    void play(qreal distance = 0.0, qreal position = 0.0);
    void stop();
    bool isPlaying();
    void setPosition(qreal distance = 0.0, qreal position = 0.0);

private:
    int channel;
    Sound* sound;
    bool playing;
};

#endif // AMBIENTSOUND_H
