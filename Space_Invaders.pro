LIBS += -L/home/william/NCURSES/ncurses_custom/include -lncurses
LIBS += -L/home/william/NCURSES/ncurses_custom/lib -lncurses
LIBS += -pthread

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    game.c \
    frame.c \
    shared.c

DISTFILES +=
