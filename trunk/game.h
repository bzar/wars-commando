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

#ifndef GAME_H
#define GAME_H

class Player;
class Actor;
class Rocket;
class Grenade;

#include "updatable.h"
#include "particle.h"
#include "level.h"

#include <QObject>
#include <QGraphicsScene>
#include <QList>
#include <QSet>

#include "resourcemanager.h"
#include "sound.h"
#include "music.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QString const& levelFilename, QObject *parent = 0);
    ~Game();

    void enter();
    void exit();

    int levelWidth();
    int levelHeight();
    bool isPaused();

    void loadLevel(QString const& levelFilename, bool emptyInventory = false);

    QGraphicsScene* getScene();
    Player* getPlayer();
    Level* getLevel();
    Actor* getEnemyAt(QPointF point);

    void setPlayer(Player* newPlayer);
    void addActor(Actor* newEnemy);
    void removeActor(Actor* enemy);
    void addUpdatable(Updatable* updatable);
    void addParticle(Particle* particle);
    void removeParticle(Particle* particle);

    ResourceManager const& getResourceManager();

    bool alive();
    int numHearts();
    void addHearts(int amount = 3);
    void removeHearts(int amount = 1);

    int numGrenades();
    void addGrenades(int amount = 3);
    void removeGrenades(int amount = 1);

    int numRockets();
    void addRockets(int amount = 1);
    void removeRockets(int amount = 1);

    void updateHUD(QPointF view);
signals:
    void win();
    void die();

public slots:
    void updateGame();
    void pause();
    void resume();

private:
    static int const DEFAULT_HEARTS = 3;
    static int const MAX_HEARTS = 5;
    static int const MAX_GRENADES = 9;
    static int const MAX_ROCKETS = 5;
    static int const RESET_DELAY = 100;
    static int const WIN_DELAY = 50;

    QGraphicsScene* scene;
    QSet<Updatable*> updatables;
    QSet<Actor*> actors;
    QSet<Particle*> particles;
    QList<QGraphicsPixmapItem*> hearts;
    QList<QGraphicsPixmapItem*> grenades;
    QList<QGraphicsPixmapItem*> rockets;

    Player* player;
    Level* level;

    Music* music;
    Sound* winSound;
    Sound* failSound;
    int resetTimer;
    int winTimer;

    bool paused;
    QGraphicsPixmapItem* exitToTitleText;

    ResourceManager resources;
};

#endif // GAME_H
