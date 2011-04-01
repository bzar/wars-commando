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

#include "enemyinfantry.h"
#include "bullet.h"
#include "rocket.h"
#include "player.h"
#include "heartitem.h"

EnemyInfantry::EnemyInfantry(QString const& definitionFile, Game* game, bool dontFall) :
    Infantry(definitionFile, game), state(STATE_ALIVE), deathSound(0), life(MAX_LIFE), hurtCounter(0), dontFall(dontFall)
{
    deathSound = game->getResourceManager().getSound("die");
}

void EnemyInfantry::hitByBullet(Bullet* bullet)
{
    if(state == STATE_ALIVE || state == STATE_HURT)
    {
        bullet->destroy();
    }

    hurt();

    if(state == STATE_DYING)
    {
        velocity.setX(bullet->getVelocity().x());
        velocity.setY(bullet->getVelocity().y());
    }
}

void EnemyInfantry::hitByRocket(Rocket* rocket)
{
    if(state == STATE_ALIVE || state == STATE_HURT)
    {
        hitSound->play();
        die();
        velocity.setX(0);
        velocity.setY(0);
    }
}

void EnemyInfantry::hurt()
{
    if(state == STATE_ALIVE || state == STATE_HURT)
    {
        hitSound->play();
        --life;
        if(life == 0)
        {
            die();
        }
        else
        {
            if(!boss)
            {
                state = STATE_HURT;
                hurtCounter = HURT_FREEZE_TIME;
            }
            flash();
        }
    }

}

bool EnemyInfantry::alive()
{
    return state != STATE_DEAD;
}

void EnemyInfantry::setBoss(bool value)
{
    Infantry::setBoss(value);
    life = BOSS_MAX_LIFE;
}

void EnemyInfantry::preUpdate()
{
    QPointF diff = game->getPlayer()->pos() - pos();
    frozen = abs(diff.x()) > 800 || abs(diff.y()) > 500 || state == STATE_HURT;

    if(!frozen && state != STATE_DYING)
    {
        velocity.setY(velocity.y() + 1.8);
    }

    if(state == STATE_DYING)
    {
        setOpacity(1.0 - getAnimationFrame() / static_cast<qreal>(getAnimationLength(ANIM_DYING) * getAnimationSpeed()));

        if(getAnimationFrame() == getAnimationLength(ANIM_DYING) * getAnimationSpeed() - 1)
        {
            state = STATE_DEAD;
        }
    }
    else if(state == STATE_HURT)
    {
        if(--hurtCounter == 0)
        {
            state = STATE_ALIVE;
        }
    }
    else if(!frozen)
    {
        if(dontFall && onGround)
        {
            QPointF checkPosition = pos() + QPointF((velocity.x() < 0 ? 2 : 4) * pixmap().width()/5, pixmap().height() - 1) + velocity;
            checkPosition.setY(checkPosition.y() + pixmap().height()/2);
            AbstractBlock* block = game->getLevel()->getBlockAt(checkPosition);
            if(!block || block->hurts())
            {
                velocity.setX(-velocity.x());

            }
        }

        selectAnimation();
    }
}

void EnemyInfantry::postUpdate()
{
    Player* player = game->getPlayer();
    QPointF playerPos(player->pos().x() + player->pixmap().width()/2, player->pos().y() + player->pixmap().height()/2);
    QPointF myPos(pos().x() + pixmap().width()/2, pos().y() + pixmap().height()/2);
    QPointF diff = playerPos - myPos;
    if(state == STATE_ALIVE && diff.x() * diff.x()*2 + diff.y() * diff.y() < pixmap().height()/2 * pixmap().height()/2)
    {
        player->hitByEnemy(this);
    }
}

void EnemyInfantry::selectAnimation()
{
    Animation animation = ANIM_STAND;
    AnimationDirection direction = getAnimationDirection();

    if(velocity.x() > 0)
    {
        direction = ANIM_RIGHT;
        animation = ANIM_WALK;
    }
    else if(velocity.x() < 0)
    {
        direction = ANIM_LEFT;
        animation = ANIM_WALK;
    }

    if(!onGround)
    {
        animation = ANIM_JUMP;
    }

    setAnimation(animation, direction);
}

void EnemyInfantry::hitLeftWall()
{
    if(state == STATE_ALIVE)
    {
        velocity.setX(-velocity.x());
        turnAnimation();
    }
    else
    {
        velocity.setX(0);
    }
}

void EnemyInfantry::hitRightWall()
{
    if(state == STATE_ALIVE)
    {
        velocity.setX(-velocity.x());
        turnAnimation();
    }
    else
    {
        velocity.setX(0);
    }
}

void EnemyInfantry::hitFloor()
{
    if(state == STATE_ALIVE || state == STATE_HURT)
    {
        velocity.setY(0);
    }
}

void EnemyInfantry::die()
{
    state = STATE_DYING;
    setAnimationSpeed(2);
    setAnimation(ANIM_DYING, getAnimationDirection());
    setAnimationFrame(0);
    deathSound->play();
    if(!boss)
    {
        dropItem();
    }
}

void EnemyInfantry::dropItem()
{
    if(qrand() % 10 == 0 && !boss)
    {
        HeartItem* item = new HeartItem(game);
        item->setPos(pos().x() + pixmap().width()/2 - item->pixmap().width()/2, pos().y() + pixmap().height() - item->pixmap().height());
    }
}
