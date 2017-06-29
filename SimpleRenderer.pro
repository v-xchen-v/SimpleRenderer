#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T15:16:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleRenderer
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=

LIBS += -L../SDL/lib/x86 -lSDL2
LIBS += -L../SDL/lib/x86 -lSDL2main
LIBS += -L../SDL/lib/x86 -lSDL2test
