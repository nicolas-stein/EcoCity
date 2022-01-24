QT       += core gui openglwidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 debug_and_release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game/Building/ServiceBuilding/powerbuilding.cpp \
    Game/Building/ServiceBuilding/servicebuilding.cpp \
    Game/Building/ZoneBuilding/zonebuilding.cpp \
    Game/Building/building.cpp \
    Game/Building/ZoneBuilding/commercialbuilding.cpp \
    Game/Building/ZoneBuilding/industrialbuilding.cpp \
    Game/Building/ZoneBuilding/residentialbuilding.cpp \
    Game/Grid/roadsquare.cpp \
    Game/Grid/zonesquare.cpp \
    Game/audiomanager.cpp \
    Game/gamelogicthread.cpp \
    Game/Grid/gridsquare.cpp \
    Game/Grid/terrainsquare.cpp \
    Game/buildcursor.cpp \
    Game/mapmanager.cpp \
    Game/ressourcemanager.cpp \
    Scenes/maingamescene.cpp \
    Scenes/mainmenuscene.cpp \
    customgraphicspixmapitem.cpp \
    customgraphicsview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Game/Building/ServiceBuilding/powerbuilding.h \
    Game/Building/ServiceBuilding/servicebuilding.h \
    Game/Building/ZoneBuilding/zonebuilding.h \
    Game/audiomanager.h \
    Game/mapmanager.h \
    Game/ressourcemanager.h \
    constants.h \
    Game/Building/building.h \
    Game/Building/ZoneBuilding/commercialbuilding.h \
    Game/Building/ZoneBuilding/industrialbuilding.h \
    Game/Building/ZoneBuilding/residentialbuilding.h \
    Game/Grid/roadsquare.h \
    Game/Grid/zonesquare.h \
    Game/PerlinNoise.hpp \
    Game/gamelogicthread.h \
    Game/Grid/gridsquare.h \
    Game/Grid/terrainsquare.h \
    Game/buildcursor.h \
    Scenes/maingamescene.h \
    Scenes/mainmenuscene.h \
    customgraphicspixmapitem.h \
    customgraphicsview.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#RESOURCES += \
OTHER_FILES += \
    ressources_audio.qrc \
    ressources_graphics.qrc \
    ressources_others.qrc

DISTFILES +=
