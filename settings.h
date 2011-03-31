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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QKeySequence>

namespace Settings
{
    QKeySequence const PLAYER_LEFT_KEY("Left");
    QKeySequence const PLAYER_RIGHT_KEY("Right");
    QKeySequence const PLAYER_UP_KEY("Up");
    QKeySequence const PLAYER_DOWN_KEY("Down");

#ifdef PANDORA
    QKeySequence const PLAYER_JUMP_KEY("Page down");
    QKeySequence const PLAYER_SHOOT_KEY("Home");
    QKeySequence const PLAYER_GRENADE_KEY("End");
    QKeySequence const PLAYER_BAZOOKA_KEY("Page up");

    QKeySequence const OK_KEY("Page down");
    QKeySequence const PAUSE_KEY("Q");
#else
    QKeySequence const PLAYER_JUMP_KEY("S");
    QKeySequence const PLAYER_SHOOT_KEY("D");
    QKeySequence const PLAYER_GRENADE_KEY("A");
    QKeySequence const PLAYER_BAZOOKA_KEY("W");

    QKeySequence const OK_KEY("S");
    QKeySequence const PAUSE_KEY("Escape");
#endif

    QKeySequence const YES_KEY("Y");
    QKeySequence const NO_KEY("N");

}
#endif // SETTINGS_H
