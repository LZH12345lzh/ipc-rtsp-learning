QT += core network
CONFIG += console c++14
CONFIG -= app_bundle

TEMPLATE = app
TARGET = RtspClientDemo

INCLUDEPATH += $$PWD/cpp \
               $$PWD/cpp/network \
               $$PWD/cpp/protocol \
               $$PWD/cpp/media

SOURCES += \
    $$files(cpp/protocol/*.cpp) \
    $$files(cpp/network/*.cpp) \
    $$files(cpp/media/*.cpp) \
    $$files(cpp/*.cpp)

HEADERS += \
    $$files(cpp/protocol/*.h) \
    $$files(cpp/network/*.h) \
    $$files(cpp/media/*.h)