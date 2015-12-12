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
    $${PROJ_PATH}/tilemap.cpp \
    $${PROJ_PATH}/tiles_lair_01.cpp \
    $${PROJ_PATH}/tiles_tb_caverns.cpp \
    $${PROJ_PATH}/spider_large.cpp

HEADERS  += $${PROJ_PATH}/animation.h \
    $${PROJ_PATH}/bridge.h \
    $${PROJ_PATH}/character.h \
    $${PROJ_PATH}/common.h \
    $${PROJ_PATH}/drawing.h \
    $${PROJ_PATH}/game.h \
    $${PROJ_PATH}/player_animations.h \
    $${PROJ_PATH}/tilemap.h \
    $${PROJ_PATH}/tiles_lair_01.h \
    $${PROJ_PATH}/tiles_tb_caverns.h \
    $${PROJ_PATH}/spider_large.h \
    $${PROJ_PATH}/enemies.h
