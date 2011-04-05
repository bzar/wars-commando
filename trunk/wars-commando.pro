#-------------------------------------------------
#
# Project created by QtCreator 2011-01-28T20:21:44
#
#-------------------------------------------------

QT       += core gui


#CONFIG += opengl
#CONFIG += fullscreen

unix:!symbian {
    maemo5 {
        CONFIG += fullscreen
        #CONFIG -= opengl
    }

    pandora {
        CONFIG += fullscreen
        CONFIG -= opengl
        DEFINES += PANDORA
    }
}

opengl {
    QT += opengl
    DEFINES += OPENGL
}

fullscreen {
    DEFINES += FULLSCREEN
}

!opengl {
    TARGET = wars-commando
} else {
    TARGET = wars-commando-opengl
}
TEMPLATE = app


SOURCES += main.cpp\
        gamewindow.cpp \
    player.cpp \
    level.cpp \
    updatable.cpp \
    abstractblock.cpp \
    basicblock.cpp \
    animated.cpp \
    actor.cpp \
    slopeblock.cpp \
    bullet.cpp \
    game.cpp \
    enemyinfantry.cpp \
    enemyinfantryshooter.cpp \
    enemyinfantrywalker.cpp \
    grenade.cpp \
    sound.cpp \
    music.cpp \
    jumpthroughblock.cpp \
    rocket.cpp \
    propblock.cpp \
    enemyinfantrygrenadier.cpp \
    enemyinfantrybazooka.cpp \
    grenadeitem.cpp \
    rocketitem.cpp \
    vehicle.cpp \
    enemylighttank.cpp \
    infantry.cpp \
    cannonball.cpp \
    ambientsound.cpp \
    automachinegun.cpp \
    enemytank.cpp \
    enemymediumtank.cpp \
    gamestate.cpp \
    spikeblock.cpp \
    enemyheavytank.cpp \
    mapstate.cpp \
    map.cpp \
    resourcemanager.cpp \
    particle.cpp \
    heartitem.cpp \
    startstate.cpp \
    endstate.cpp

HEADERS  += gamewindow.h \
    player.h \
    level.h \
    updatable.h \
    abstractblock.h \
    basicblock.h \
    animated.h \
    actor.h \
    slopeblock.h \
    bullet.h \
    particle.h \
    game.h \
    enemyinfantry.h \
    enemyinfantryshooter.h \
    enemyinfantrywalker.h \
    grenade.h \
    sound.h \
    music.h \
    jumpthroughblock.h \
    rocket.h \
    propblock.h \
    enemyinfantrygrenadier.h \
    enemyinfantrybazooka.h \
    grenadeitem.h \
    rocketitem.h \
    vehicle.h \
    enemylighttank.h \
    infantry.h \
    cannonball.h \
    ambientsound.h \
    automachinegun.h \
    enemytank.h \
    enemymediumtank.h \
    gamestate.h \
    applicationstate.h \
    spikeblock.h \
    enemyheavytank.h \
    mapstate.h \
    map.h \
    resourcemanager.h \
    heartitem.h \
    startstate.h \
    endstate.h \
    settings.h

RESOURCES += \
    resources.qrc

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}

unix:macx {
	LIBS += -framework CoreFoundation -framework SDL -framework SDL_mixer
	CONFIG += x86 ppc
} else {
	LIBS += -lSDL -lSDL_mixer
}

OTHER_FILES += \
    readme.txt
