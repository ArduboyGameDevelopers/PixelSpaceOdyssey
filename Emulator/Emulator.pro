#-------------------------------------------------
#
# Project created by QtCreator 2015-12-02T21:31:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Emulator
TEMPLATE = app
DEFINES += EMULATOR

include(PixelSpaceOdyssey.pri)
include(Arduboy.pri)

INCLUDEPATH += ../Arduboy-Emu-SDL/Platform

SOURCES += main.cpp\
    DisplayWidget.cpp \
    EditorPanTool.cpp \
    EditorTool.cpp \
    Emulator.cpp \
    GameInput.cpp \
    MainWindow+ToolBar.cpp \
    MainWindow.cpp \
    EditorDrawTool.cpp \
    EditorState.cpp \
    Level.cpp \
    MainWindow+Actions.cpp

HEADERS  += \
    DisplayWidget.h \
    EditorPanTool.h \
    EditorTool.h \
    Emulator.h \
    GameInput.h \
    MainWindow.h \
    EditorDrawTool.h \
    EditorState.h \
    Level.h

FORMS    += mainwindow.ui

RESOURCES += \
    Images/images.qrc