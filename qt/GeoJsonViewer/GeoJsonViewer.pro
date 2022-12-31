QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commondefines.cpp \
    commonfunctions.cpp \
    featuredelegate.cpp \
    featuretablemodel.cpp \
    graphicsview.cpp \
    main.cpp \
    mainwindow.cpp \
    parsegenjson.cpp \
    rastergraphicsview.cpp \
    vectorgraphicsview.cpp

HEADERS += \
    commondefines.h \
    commonfunctions.h \
    featuredelegate.h \
    featuretablemodel.h \
    graphicsview.h \
    mainwindow.h \
    parsegenjson.h \
    rastergraphicsview.h \
    vectorgraphicsview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    GeoJsonViewer.qrc
