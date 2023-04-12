#-------------------------------------------------
#
# Project created by QtCreator 19-01-2018
#
#-------------------------------------------------


### ------------------------------------->
QT += core gui opengl network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
CONFIG += console c++14
TARGET = mobilityVanet
QMAKE_CXXFLAGS += -Wno-sign-compare
release: DESTDIR = ./release
release: OBJECTS_DIR = ./obj
### -------------------------------------<


### ------------- TX PC -----------------<
INCLUDEPATH += ../../shapelib
LIBS += -L../../shapelib


### ------------------------------------->
INCLUDEPATH += "$$PWD/include" $$PWD/../core-master/include $$PWD/../gui-render_ogrclasses $$PWD/../osm-master
win32: INCLUDEPATH += c:/msys/1.0/local/include
else:unix: INCLUDEPATH += /usr/include/gdal
### -------------------------------------<


### ------------------------------------->
win32: DEPENDPATH += c:/msys/1.0/local/include $$PWD/../core-master/include $$PWD/../gui-render_ogrclasses $$PWD/../osm-master
### -------------------------------------<


### ------------------------------------->
win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgdal
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgdal
else:unix: LIBS += -lgdal

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -llibGeographic.dll
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -llibGeographic.dll
else:unix: LIBS += -lGeographic

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgeos
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lgeos

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lproj
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lproj

win32:CONFIG(release, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lshp
else:win32:CONFIG(debug, debug|release): LIBS += -Lc:/msys/1.0/local/lib/ -lshp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../geologic-core/release/ -lgeologic-core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../geologic-core/debug/ -lgeologic-core
else:unix: LIBS += -L$$PWD/../build-geologic-core-Desktop-Debug/release/ -lgeologic-core

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libgeologic-gui/release/ -lgeologic-gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libgeologic-gui/release/ -lgeologic-gui
else:unix: LIBS += -L$$PWD/../build-geologic-gui-Desktop-Debug/release/ -lgeologic-gui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../geologic-osm/release/ -lgeologic-osm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../geologic-osm/release/ -lgeologic-osm
else:unix: LIBS += -L$$PWD/../build-geologic-osm-Desktop-Debug/release/ -lgeologic-osm

win32:LIBS += -lopengl32 -lglu32 -lglut -lmingw32 -lm
else:unix: LIBS += -lGL -lglut -lGLU -lGeographic -lreadosm -lshp

### -------------------------------------<


### ------------------------------------->
HEADERS += \
    include/vanet.h \
    include/misc/parameters.h \
    include/misc/vehicule.h \
    include/misc/fleet.h \
    include/misc/antenna.h \
    include/misc/antennamap.h \
    include/misc/vectPoints.h \
    include/misc/alerteinfo.h \
    include/layers/clustermanagerlayer.h \
    include/layers/propagationlayer.h \
    include/displayClasses/clusterLinks.h \
    include/displayClasses/vehiclePosition.h \
    include/displayClasses/vehiclePropagCircle.h \
    include/displayClasses/vehiclePropagMeshes.h \
    include/displayClasses/vehicleTrace.h \
    include/clustering/propagclustering.h \
    include/clustering/testclustering.h


SOURCES += \
    src/launcher.cpp \
    src/vanet.cpp \
    src/misc/parameters.cpp \
    src/misc/vehicule.cpp \
    src/misc/fleet.cpp \
    src/misc/antenna.cpp \
    src/misc/antennamap.cpp \
    src/misc/vectPoints.cpp \
    src/misc/alerteinfo.cpp \
    src/layers/clustermanagerlayer.cpp \
    src/layers/propagationlayer.cpp \
    src/displayClasses/clusterLinks.cpp \
    src/displayClasses/vehiclePosition.cpp \
    src/displayClasses/vehiclePropagCircle.cpp \
    src/displayClasses/vehiclePropagMeshes.cpp \
    src/displayClasses/vehicleTrace.cpp \
    src/clustering/propagclustering.cpp \
    include/clustering/testclustering.cpp

### -------------------------------------<

