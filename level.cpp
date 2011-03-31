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

#include "level.h"
#include "game.h"

#include "basicblock.h"
#include "slopeblock.h"
#include "jumpthroughblock.h"
#include "spikeblock.h"
#include "propblock.h"

#include "player.h"
#include "enemyinfantrywalker.h"
#include "enemyinfantryshooter.h"
#include "enemyinfantrygrenadier.h"
#include "enemyinfantrybazooka.h"
#include "enemylighttank.h"
#include "enemymediumtank.h"
#include "enemyheavytank.h"
#include "automachinegun.h"

#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>

Level::Level(QString const& mapFilename, QString const& blockFilename, Game* game) :
    game(game), width(0), height(0)
{
    QGraphicsScene* scene = game->getScene();
    QPixmap pixmap(blockFilename);
    for(int i = 0; i < pixmap.width()/BLOCK_SIZE; ++i)
    {
        blockImages.append(pixmap.copy(i*BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE));
    }

    QFile levelFile(mapFilename);

    if(!levelFile.open(QIODevice::ReadOnly))
    {
        qDebug("Could not open level file");
    }

    QTextStream levelStream(&levelFile);
    int y = 0;
    bool nextUnitIsTheBoss = false;
    bool nextUnitHasBackground = false;

    while(!levelStream.atEnd()) {
        QString line = levelStream.readLine();

        ++height;
        if(line.length() > width)
        {
            width = line.length();
        }


        QList<AbstractBlock*> row;

        for(int x = 0; x < line.length(); ++x)
        {
            Actor* enemy = 0;
            AbstractBlock* block = 0;
            if(line.at(x) == '*')
            {
                nextUnitIsTheBoss = true;

                if(nextUnitHasBackground)
                {
                    block = new PropBlock(blockImages.at(6), scene);
                    block->setZValue(-1);
                }
            }
            if(line.at(x) == 'G')
            {
                block = new BasicBlock(blockImages.at(0), scene);
            }
            else if(line.at(x) == 'R')
            {
                block = new JumpThroughBlock(blockImages.at(4), scene);
            }
            else if(line.at(x) == '-')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new JumpThroughBlock(blockImages.at(4), scene);
            }
            else if(line.at(x) == 'D')
            {
                block = new BasicBlock(blockImages.at(3), scene);
            }
            else if(line.at(x) == '/')
            {
                block = new SlopeBlock(blockImages.at(1), 1.0, 0.0, scene);
            }
            else if(line.at(x) == '\\')
            {
                block = new SlopeBlock(blockImages.at(2), 0.0, 1.0, scene);
            }

            else if(line.at(x) == 'A')
            {
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::BOTTOM);
            }
            else if(line.at(x) == 'V')
            {
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::TOP);
            }
            else if(line.at(x) == '<')
            {
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::RIGHT);
            }
            else if(line.at(x) == '>')
            {
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::LEFT);
            }

            else if(line.at(x) == '8')
            {
                block = new BasicBlock(blockImages.at(8), scene);
                AutoMachineGun* gun = new AutoMachineGun(block, game, 90);
                gun->setRotation(-45);
                gun->setPos(18, 18);
                game->addUpdatable(gun);
            }
            else if(line.at(x) == '2')
            {
                block = new BasicBlock(blockImages.at(8), scene);
                AutoMachineGun* gun = new AutoMachineGun(block, game, 90);
                gun->setRotation(135);
                gun->setPos(18, 18);
                game->addUpdatable(gun);
            }
            else if(line.at(x) == '4')
            {
                block = new BasicBlock(blockImages.at(8), scene);
                AutoMachineGun* gun = new AutoMachineGun(block, game, 90);
                gun->setRotation(-135);
                gun->setPos(18, 18);
                game->addUpdatable(gun);
            }
            else if(line.at(x) == '6')
            {
                block = new BasicBlock(blockImages.at(8), scene);
                AutoMachineGun* gun = new AutoMachineGun(block, game, 90);
                gun->setRotation(45);
                gun->setPos(18, 18);
                game->addUpdatable(gun);
            }

            else if(line.at(x) == '^')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::BOTTOM);
            }
            else if(line.at(x) == 'W')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::TOP);
            }
            else if(line.at(x) == '{')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::RIGHT);
            }
            else if(line.at(x) == '}')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new SpikeBlock(blockImages.at(7), scene, SpikeBlock::LEFT);
            }

            else if(line.at(x) == 'O')
            {
                block = new PropBlock(blockImages.at(5), scene);
            }
            else if(line.at(x) == '0')
            {
                AbstractBlock* newBlock = new PropBlock(blockImages.at(6), scene);
                newBlock->setZValue(-1);
                newBlock->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                block = new PropBlock(blockImages.at(5), scene);
            }

            else if(line.at(x) == '.')
            {
                block = new PropBlock(blockImages.at(6), scene);
                block->setZValue(-1);
            }
            else if(line.at(x) == ',')
            {
                block = new PropBlock(blockImages.at(6), scene);
                block->setZValue(-1);
                nextUnitHasBackground = true;
            }

            else if(line.at(x) == '.')
            {
                block = new PropBlock(blockImages.at(6), scene);
            }
            else if(line.at(x) == 'e')
            {
                enemy = new EnemyInfantryWalker(game);
            }
            else if(line.at(x) == 'f')
            {
                enemy = new EnemyInfantryWalker(game, true);
            }
            else if(line.at(x) == 's')
            {
                enemy = new EnemyInfantryShooter(game);
            }
            else if(line.at(x) == 'h')
            {
                enemy = new EnemyInfantryShooter(game, false, true);
            }
            else if(line.at(x) == 'o')
            {
                enemy = new EnemyInfantryShooter(game, true);
                enemy->setVelocity(QPointF(-3,0));
            }
            else if(line.at(x) == 'r')
            {
                enemy = new EnemyInfantryShooter(game, false, true);
                enemy->setVelocity(QPointF(nextUnitIsTheBoss ? -4 :-3, 0));
            }
            else if(line.at(x) == 'g')
            {
                enemy = new EnemyInfantryGrenadier(game);
            }
            else if(line.at(x) == 'b')
            {
                enemy = new EnemyInfantryBazooka(game);
            }
            else if(line.at(x) == 'l')
            {
                enemy = new EnemyLightTank(game);
            }
            else if(line.at(x) == 'm')
            {
                enemy = new EnemyMediumTank(game);
            }
            else if(line.at(x) == 'a')
            {
                enemy = new EnemyHeavyTank(game);
            }

            else if(line.at(x) == 'p')
            {
                Player* player = new Player(game);
                player->setPos(x * BLOCK_SIZE, y * BLOCK_SIZE - 1);
                game->setPlayer(player);
            }

            if(enemy)
            {
                enemy->setPos(x * BLOCK_SIZE, y * BLOCK_SIZE - 1);
                if(nextUnitIsTheBoss)
                {
                    enemy->setBoss(true);
                    QGraphicsPixmapItem* emblem = scene->addPixmap(game->getResourceManager().getImage("bossEmblem"));
                    emblem->setParentItem(enemy);
                    emblem->setPos(enemy->pixmap().width()/2 - emblem->pixmap().width()/2, -emblem->pixmap().height());
                    nextUnitIsTheBoss = false;
                }


                if(nextUnitHasBackground)
                {
                    block = new PropBlock(blockImages.at(6), scene);
                    block->setZValue(-1);
                }

                nextUnitHasBackground = false;
                game->addActor(enemy);
            }

            if(block)
            {
                block->setPos(x*BLOCK_SIZE, y*BLOCK_SIZE);
                row.append(block);
            }
            else
            {
                row.append(0);
            }

        }

        blocks.append(row);

        ++y;
    }
}

AbstractBlock* Level::getBlockAt(QPointF point)
{
    int x = static_cast<int>(point.x()) / BLOCK_SIZE;
    int y = static_cast<int>(point.y()) / BLOCK_SIZE;

    if(y < 0 || y >= blocks.size() ||
       x < 0 || x >= blocks.at(y).size())
    {
        return 0;
    }


    if(blocks.at(y).at(x) && blocks.at(y).at(x)->containsPoint(point))
    {
        return blocks.at(y).at(x);
    }
    else
    {
        return 0;
    }

}

int Level::getWidth()
{
    return width;
}

int Level::getHeight()
{
    return height;
}
