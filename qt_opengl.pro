QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    dockwidgets/lightsettingwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    model/mesh.cpp \
    model/model.cpp \
    render.cpp

HEADERS += \
    camera.h \
    dockwidgets/lightsettingwidget.h \
    lights/directionlight.h \
    lights/pointlight.h \
    mainwindow.h \
    model/bbox.h \
    model/mesh.h \
    model/model.h \
    render.h

FORMS += \
    dockwidgets/lightsettingwidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

macx: LIBS += -L$$PWD/lib/ -lassimp.5

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

RESOURCES += \
    images/images.qrc \
    shaders/shaders.qrc
