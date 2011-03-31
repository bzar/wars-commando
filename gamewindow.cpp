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

#include "gamewindow.h"

#include "gamestate.h"
#include "mapstate.h"
#include "startstate.h"
#include "endstate.h"

#include "game.h"
#include "player.h"
#include "sound.h"

#include <QImage>
#include <QPainter>
#include <QApplication>

GameWindow::GameWindow(QWidget *parent) :
    QGraphicsView(parent), lives(0), currentState(0), startState(0), gameState(0), mapState(0)
{
    Sound::init();

    resize(800, 480);

#ifdef FULLSCREEN
    showFullScreen();
#endif

    setStyleSheet( "QGraphicsView { border-style: none; }" );
    setFrameStyle(0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCacheMode(QGraphicsView::CacheNone);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewport()->setAttribute(Qt::WA_NoSystemBackground);
    viewport()->setAttribute(Qt::WA_PaintUnclipped);
    viewport()->setAttribute(Qt::WA_TranslucentBackground, false);

    setCursor(Qt::BlankCursor);

    startState = new StartState(this);
    gameState = new GameState(this);
    mapState = new MapState(this);
    endState = new EndState(this);
    setState(startState);

    startTimer(33);
}

GameWindow::~GameWindow()
{
    Sound::exit();
}


void GameWindow::timerEvent(QTimerEvent*)
{
    if(currentState)
    {
        currentState->update();
        //viewport()->update();
    }

}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_P)
    {
        QImage screenshot(viewport()->size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&screenshot);
        render(&painter);
        screenshot.save("screenshot.png");
    }
    else if(event->key() == Qt::Key_F && event->modifiers() & Qt::ControlModifier)
    {
        if(isFullScreen())
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
    }
    else
    {
        currentState->handleKeyPressEvent(event);
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    currentState->handleKeyReleaseEvent(event);
}

void GameWindow::newGame()
{
    lives = 3;
    mapState->getMap()->reset();
    mapState->getMap()->setLives(lives);
    gameState->firstLevel();
    setState(mapState);
}

void GameWindow::showGame()
{
    setState(gameState);
}

void GameWindow::nextLevel()
{
    if(mapState->getMap()->hasNextLevel())
    {
        ++lives;
        mapState->getMap()->setLives(lives);
        gameState->nextLevel();
        setState(mapState);
        mapState->getMap()->nextLevel();
    }
    else
    {
        endState->setCheater(gameState->getCheater());
        setState(endState);
    }
}

void GameWindow::loseLife()
{
    --lives;
    if(lives >= 0)
    {
        gameState->resetLevel();
        setState(mapState);
        mapState->getMap()->setLives(lives);
    }
    else
    {
        gameState->getGame()->pause();
        reset();
    }
}

void GameWindow::quit()
{
    QApplication::quit();
}

void GameWindow::setState(ApplicationState *state)
{
    if(currentState)
    {
        currentState->exit();
    }

    currentState = state;
    state->enter();
    setScene(currentState->getScene());
}

void GameWindow::reset()
{
    setState(startState);
    mapState->reset();
    gameState->firstLevel();
}
