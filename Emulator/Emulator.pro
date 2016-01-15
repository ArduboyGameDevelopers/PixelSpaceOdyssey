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

INCLUDEPATH += Platform

SOURCES += main.cpp\
    DisplayWidget.cpp \
    EditorPanTool.cpp \
    EditorTool.cpp \
    Emulator.cpp \
    GameInput.cpp \
    MainWindow.cpp \
    EditorDrawTool.cpp \
    EditorState.cpp \
    Level.cpp \
    TilesWidget.cpp \
    Tileset.cpp \
    Object.cpp \
    Settings.cpp \
    Constants.cpp \
    Input2Dialog.cpp \
    EditorCharTool.cpp \
    CharacterButton.cpp \
    CharacterListView.cpp \
    CharacterInfo.cpp \
    ParamCheckBox.cpp \
    ResizeDialog.cpp

HEADERS  += \
    DisplayWidget.h \
    EditorPanTool.h \
    EditorTool.h \
    Emulator.h \
    GameInput.h \
    MainWindow.h \
    EditorDrawTool.h \
    EditorState.h \
    Level.h \
    TilesWidget.h \
    Tileset.h \
    Object.h \
    EditorTools.h \
    Settings.h \
    Constants.h \
    Input2Dialog.h \
    EditorCharTool.h \
    CharacterButton.h \
    CharacterListView.h \
    Version.h \
    CharacterInfo.h \
    ParamCheckBox.h \
    Params.h \
    ResizeDialog.h

FORMS    += \
    input2dialog.ui \
    ResizeDialog.ui \
    MainWindow.ui

RESOURCES += \
    Images/images.qrc
