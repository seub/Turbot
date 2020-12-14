TEMPLATE = app
CONFIG += console
CONFIG += c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        piece.cpp \
        position.cpp \
        square.cpp \
        tools.cpp

HEADERS += \
    piece.h \
    position.h \
    square.h \
    tools.h \
    types.h

OTHER_FILES += \
    todo.md
