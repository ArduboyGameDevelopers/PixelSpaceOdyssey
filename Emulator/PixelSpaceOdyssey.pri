macx {
    PROJ_PATH = PixelSpaceOdyssey
}
win32 {
    PROJ_PATH = ../PixelSpaceOdyssey
}

INCLUDEPATH += $${PROJ_PATH}

SOURCES += $${PROJ_PATH}/character.cpp \
    $${PROJ_PATH}/drawing.cpp \
    $${PROJ_PATH}/game.cpp \
    $${PROJ_PATH}/player_animations.cpp \
    $${PROJ_PATH}/bear_animations.cpp \
    $${PROJ_PATH}/spider_animations.cpp \
    $${PROJ_PATH}/tilemap.cpp \
    $${PROJ_PATH}/tiles_lair_01.cpp \
    $${PROJ_PATH}/tiles_merged.cpp \
    $${PROJ_PATH}/spider_large.cpp \
    $${PROJ_PATH}/spider_large_animations.cpp \
    $${PROJ_PATH}/dog.cpp \
    $${PROJ_PATH}/dog_animations.cpp \
    $${PROJ_PATH}/enemy_base.cpp \
    $${PROJ_PATH}/dispatch.cpp \
    $${PROJ_PATH}/hud.cpp \
    $${PROJ_PATH}/images.cpp \
    $${PROJ_PATH}/bear.cpp

HEADERS  += $${PROJ_PATH}/animation.h \
    $${PROJ_PATH}/bridge.h \
    $${PROJ_PATH}/character.h \
    $${PROJ_PATH}/common.h \
    $${PROJ_PATH}/drawing.h \
    $${PROJ_PATH}/game.h \
    $${PROJ_PATH}/player_animations.h \
    $${PROJ_PATH}/bear_animations.h \
    $${PROJ_PATH}/spider_animations.h \
    $${PROJ_PATH}/tilemap.h \
    $${PROJ_PATH}/tiles_lair_01.h \
    $${PROJ_PATH}/tiles_merged.h \
    $${PROJ_PATH}/spider_large.h \
    $${PROJ_PATH}/spider_large_animations.h \
    $${PROJ_PATH}/dog.h \
    $${PROJ_PATH}/dog_animations.h \
    $${PROJ_PATH}/enemies.h \
    $${PROJ_PATH}/enemy_base.h \
    $${PROJ_PATH}/dispatch.h \
    $${PROJ_PATH}/hud.h \
    $${PROJ_PATH}/images.h \
    $${PROJ_PATH}/debug.h \
    $${PROJ_PATH}/player.h \
    $${PROJ_PATH}/bear.h
