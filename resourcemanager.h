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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "sound.h"
#include <QPixmap>
#include <QMap>
#include <QObject>

class ResourceManager : public QObject
{
    Q_OBJECT

public:

    ResourceManager(QObject* parent = 0);
    void addImage(QString const& key, QString const& filename);
    void addSound(QString const& key, QString const& filename);
    QPixmap getImage(QString const& key) const;
    Sound* getSound(QString const& key) const;

private:
    QMap<QString, QPixmap> images;
    QMap<QString, Sound*> sounds;
};

#endif // RESOURCEMANAGER_H
