QT -= gui
QT += core network sql

CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    \
    db/database.cpp \
    db/datacrypting.cpp \
    db/data/dbdata.cpp \
    \
    connectiondata/connectiondata.cpp \
    connectiondata/room.cpp \
    \
    connectionhandler/connectionhandler.cpp \
    connectionhandler/buffer.cpp \
    \
    parser/commandparser.cpp \
    parser/parseddata.cpp \
    \
    parser/commands/data/parameterdata.cpp \
    parser/commands/data/commanddata.cpp \
    \
    parser/commands/commands.cpp \
    \
    server/servernamespace.cpp \
    server/server.cpp

HEADERS += db/database.h \
    \
    db/datacrypting.h \
    db/data/dbdata.h \
    \
    connectiondata/connectiondata.h \
    connectiondata/room.h \
    \
    connectionhandler/connectionhandler.h \
    connectionhandler/buffer.h \
    \
    parser/commandparser.h \
    parser/parsingexceptions.h \ 
    parser/parseddata.h \
    \
    parser/commands/data/parameterdata.h \
    parser/commands/data/commanddata.h \
    \
    parser/commands/commands.h \
    \
    server/servernamespace.h \
    server/server.h
