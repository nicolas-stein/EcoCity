QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game/Building/building.cpp \
    Game/Building/residentialbuilding.cpp \
    Game/Grid/roadsquare.cpp \
    Game/Grid/zonesquare.cpp \
    Game/gamelogicthread.cpp \
    Game/Grid/gridsquare.cpp \
    Game/Grid/terrainsquare.cpp \
    Game/buildcursor.cpp \
    Scenes/maingamescene.cpp \
    Scenes/mainmenuscene.cpp \
    customgraphicsview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Constants.h \
    Game/Building/building.h \
    Game/Building/residentialbuilding.h \
    Game/Grid/roadsquare.h \
    Game/Grid/zonesquare.h \
    Game/PerlinNoise.hpp \
    Game/gamelogicthread.h \
    Game/Grid/gridsquare.h \
    Game/Grid/terrainsquare.h \
    Game/buildcursor.h \
    Scenes/maingamescene.h \
    Scenes/mainmenuscene.h \
    customgraphicsview.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
