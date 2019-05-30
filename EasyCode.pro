#-------------------------------------------------
#
# Project created by QtCreator 2018-03-23T21:29:28
#
#-------------------------------------------------

QT       += core gui
QT += printsupport
RC_ICONS = Icon/book.ico
TRANSLATIONS=EasyCode.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyCode
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    TextEdit.cpp \
    MainWindowSlot.cpp \
    AppConfig.cpp \
    AboutDialog.cpp \
    FindDialog.cpp

HEADERS += \
        MainWindow.h \
    TextEdit.h \
    AppConfig.h \
    AboutDialog.h \
    FindDialog.h

FORMS += \
        MainWindow.ui

RESOURCES += \
    res.qrc

Debug:LIBS += C:\\AAStudy\\QT\\EasyCode\\MSVC\\qscintilla2_qt5d.lib
Release:LIBS += C:\\AAStudy\\QT\\EasyCode\\MSVC\\qscintilla2_qt5.lib
