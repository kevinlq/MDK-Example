include($$PWD/../qtav-mdk-demo.pri)

QT = core gui gui widgets qml quick quick quickcontrols2
greaterThan(QT_MAJOR_VERSION, 5): QT += opengl openglwidgets

TEMPLATE = app
TARGET = mdk-qml-demo
CONFIG += c++17 utf8_source

INCLUDEPATH +=$${MDK_SDK_PATH}/include

SOURCES +=\
    main.cpp \
    mdkplayer.cpp

HEADERS += \
    mdkplayer.h


RESOURCES += \
    $$PWD/qml.qrc


win32:{
    CONFIG(MinGW, MinGW|MinGW64|MSVC64):{
        LIBS += -L$${MDK_SDK_PATH}/bin/x86 -lmdk
    }
    else: CONFIG(MinGW64, MinGW|MinGW64|MSVC64):{
        LIBS += -L$${MDK_SDK_PATH}/bin/x64 -lmdk
    }
    else: CONFIG(MSVC64, MinGW|MinGW64|MSVC64):{
        LIBS += -L$${MDK_SDK_PATH}/lib/x64 -lmdk
    }
}
else:linux:{
    LIBS += -lX11
}

