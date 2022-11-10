QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += main.cpp \
    \
    client/baseaccount.cpp \
    client/baseroom.cpp \
    client/clientclass.cpp \
    client/clientnamespace.cpp \
    \
    # main window uies
    ui/back/widgets/showaccountinfo.cpp \
    ui/back/widgets/startmenu.cpp \
    ui/back/widgets/mainwindow.cpp \
    ui/back/widgets/messagingmenu.cpp \
    \
    # message items (pricture and text)
    ui/back/messageitem/basemsgitem.cpp \
    \
    # dialogs
    ui/back/popupDialogs/dialoggroup.cpp \
    ui/back/popupDialogs/dialogusername.cpp \
    \
    # member ui, list, data
    ui/back/widgets/memberlist.cpp \
    ui/back/widgets/groupmemberinfo.cpp \
    \
    # group ui, list, data
    ui/back/widgets/roomlist.cpp \
    ui/back/widgets/groupinfo.cpp \
    \
    # parser
    parser/parser.cpp \
    parser/commands/commands.cpp \
    parser/commands/data/commanddata.cpp \
    parser/commands/data/parameterdata.cpp \
    parser/commands/data/parseddata.cpp \
    \
    # socket connection
    connectionhandler/connectionhandler.cpp \
    connectionhandler/buffer.cpp

HEADERS += \
    \
    client/baseaccount.h \
    client/baseroom.h \
    client/clientclass.h \
    client/clientnamespace.h \
    \
    # main window uies
    ui/back/widgets/showaccountinfo.h \
    ui/back/widgets/startmenu.h \
    ui/back/widgets/mainwindow.h \
    ui/back/widgets/messagingmenu.h \
    \
    # message items (pricture and text)
    ui/back/messageitem/basemsgitem.h \
    ui/back/messageitem/pictureitem.h \
    ui/back/messageitem/messageitem.h \
    \
    # dialogs
    ui/back/popupDialogs/dialoggroup.h \
    ui/back/popupDialogs/dialogusername.h \
    \
    # member ui, list, data
    ui/back/widgets/memberlist.h \
    ui/back/widgets/groupmemberinfo.h \
    \
    # group ui, list, data
    ui/back/widgets/roomlist.h \
    ui/back/widgets/groupinfo.h \
    \
    # parser
    parser/parser.h \
    parser/commands/commands.h \
    parser/commands/data/commanddata.h \
    parser/commands/data/parameterdata.h \
    parser/commands/data/parseddata.h \
    \
    # socket connection
    connectionhandler/connectionhandler.h \
    connectionhandler/buffer.h

FORMS += \
    ui/front/groupmemberinfo.ui \
    ui/front/groupmemberlist.ui \
    ui/front/mainwindow.ui \
    ui/front/showaccountinfo.ui \
    ui/front/messagingmenu.ui \
    ui/front/dialoggroup.ui \
    ui/front/dialogusername.ui \
    ui/front/startmenu.ui \
    ui/front/grouplist.ui \
    ui/front/groupinfo.ui 

RESOURCES += \
    images.qrc
