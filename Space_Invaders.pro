LIBS += -lncurses
LIBS += -lpanel
LIBS += -lmenu
LIBS += -lform
LIBS += -pthread

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    game.c \
    frame.c

DISTFILES +=
