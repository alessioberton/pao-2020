QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Main.cpp \
    model/GalleriaVirtuale.cpp \
    model/Controller.cpp \
    model/Model.cpp \
    schema/Dipinto.cpp \
    schema/Mosaico.cpp \
    schema/Opera.cpp \
    schema/Quadro.cpp \
    schema/Scultura.cpp \
    utility/LoadEntryDataUtility.cpp

HEADERS += \
    model/GalleriaVirtuale.h \
    model/Controller.h \
    model/Model.h \
    schema/EnumCollection.h \
    schema/Mosaico.h \
    utility/Container.h \
    utility/DeepPtr.h \
    schema/EnumCollection.h \
    schema/Dipinto.h \
    schema/Opera.h \
    schema/Quadro.h \
    schema/Scultura.h \
    utility/LoadEntryDataUtility.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
