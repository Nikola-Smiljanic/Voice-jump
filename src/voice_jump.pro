QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = voice_jump
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lfftw3

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
    player.cpp \
    recorder.cpp \
    obstacle.cpp \
    score.cpp \
    game_over.cpp \
    flower.cpp

HEADERS += \
    player.h \
    recorder.h \
    obstacle.h \
    score.h \
    game_over.h \
    flower.h

FORMS +=

DISTFILES +=

RESOURCES += \
    resources.qrc
