QT += core
QT -= gui

CONFIG += c++11

TARGET = Alpha2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    leptonSPI.cpp \
    uart_socket.cpp

HEADERS += \
    leptonSPI.h \
    uart_socket.h
