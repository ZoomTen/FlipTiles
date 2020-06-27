#### Project Metadata ####

# app name and version
APP_NAME    = theReversi
APP_VERSION = 1.0

# for author data and such, see main.cpp...

# name to use for /usr/share/* directory, default is just the APP_NAME
SHARE_APP_NAME = $$APP_NAME

# executable file name, by default this is the APP_NAME turned lowercase
TARGET = $$lower($$APP_NAME)

# set git commit ID, useful for bug reports and such
GIT_VERSION = $$system(git rev-parse --short=8 HEAD)

#### Source file list ####

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainWindow.ui

# distribution files

unix {
    target.path = /usr/bin/

    defaults.files = defaults.conf
    defaults.path = /etc/theReversi/

    desktop.path = /usr/share/applications
    desktop.files = com.zumid.theReversi.desktop

    icon.path = /usr/share/icons/hicolor/scalable/apps/
    icon.files = icons/thereversi.svg

    INSTALLS += target desktop icon defaults
}

DISTFILES += \
    com.zumid.theReversi.desktop \
    defaults.conf

RESOURCES += \
    resources.qrc

#### Checking ####

# error out if APP_NAME is undefined
isEmpty(APP_NAME){
    error(Please set APP_NAME!)
}

# APP_VERSION will also be warned at compile time
isEmpty(APP_VERSION){
    warning(APP_VERSION will be \'undefined\'! Please set it!)
}

#### Qmake stuff ####

QT       += core gui thelib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# Include the-libs build tools
include(/usr/share/the-libs/pri/buildmaster.pri)

# apply the defines
!isEmpty(GIT_VERSION){
    DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
}
!isEmpty(APP_VERSION){
    DEFINES += APP_VERSION=\\\"$$APP_VERSION\\\"
}
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Turn off stripping as this causes the install to fail :(
QMAKE_STRIP = echo
