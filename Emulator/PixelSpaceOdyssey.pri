macx {
    PROJ_PATH = PixelSpaceOdyssey
}
win32 {
    PROJ_PATH = ../PixelSpaceOdyssey
}

INCLUDEPATH += $${PROJ_PATH}

SOURCES += $${PROJ_PATH}/ch_bear.cpp \
    $${PROJ_PATH}/ch_bear_animations.cpp \
    $${PROJ_PATH}/ch_dog_animations.cpp \
    $${PROJ_PATH}/ch_hero_animations.cpp \
    $${PROJ_PATH}/ch_spider_large.cpp \
    $${PROJ_PATH}/ch_spider_large_animations.cpp \
    $${PROJ_PATH}/ch_spider_small.cpp \
    $${PROJ_PATH}/ch_spider_small_animations.cpp \
    $${PROJ_PATH}/character.cpp \
    $${PROJ_PATH}/dispatch.cpp \
    $${PROJ_PATH}/drawing.cpp \
    $${PROJ_PATH}/enemy_base.cpp \
    $${PROJ_PATH}/game.cpp \
    $${PROJ_PATH}/hud.cpp \
    $${PROJ_PATH}/images.cpp \
    $${PROJ_PATH}/tilemap.cpp \
    $${PROJ_PATH}/tiles_lair_01.cpp \
    $${PROJ_PATH}/tiles_merged.cpp

HEADERS  += $${PROJ_PATH}/animation.h \
    $${PROJ_PATH}/bridge.h \
    $${PROJ_PATH}/ch_bear.h \
    $${PROJ_PATH}/ch_bear_animations.h \
    $${PROJ_PATH}/ch_dog.h \
    $${PROJ_PATH}/ch_dog_animations.h \
    $${PROJ_PATH}/ch_hero_animations.h \
    $${PROJ_PATH}/ch_spider_large.h \
    $${PROJ_PATH}/ch_spider_large_animations.h \
    $${PROJ_PATH}/ch_spider_small.h \
    $${PROJ_PATH}/ch_spider_small_animations.h \
    $${PROJ_PATH}/character.h \
    $${PROJ_PATH}/common.h \
    $${PROJ_PATH}/debug.h \
    $${PROJ_PATH}/dispatch.h \
    $${PROJ_PATH}/drawing.h \
    $${PROJ_PATH}/enemies.h \
    $${PROJ_PATH}/enemy_base.h \
    $${PROJ_PATH}/game.h \
    $${PROJ_PATH}/hud.h \
    $${PROJ_PATH}/images.h \
    $${PROJ_PATH}/player.h \
    $${PROJ_PATH}/tilemap.h \
    $${PROJ_PATH}/tiles_lair_01.h \
    $${PROJ_PATH}/tiles_merged.h
