/****************************************
 *
 *   theApp - Example 3rd-party application for theShell
 *   Copyright (C) 2020 Zumi Daxuya
 *
 *   Based on the24 (C) 2020 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/

// UI includes
#include "mainwindow.h"

// Qt includes
#include <QDir>
#include <QStandardPaths>

// the-libs includes
#include <tsettings.h>
#include <tapplication.h>

// settings
#include <tsettings.h>

#ifndef APP_VERSION
    // emit compiler warning if app version isn't set from the makefile or qmake
    #warning "No version number set! Version number will be 'undefined' unless you specify it with -DAPP_VERSION=<version number>!"
    #define APP_VERSION "undefined"
#endif

int main(int argc, char* argv[]) {
    // start of our applicaation
    tApplication a(argc, argv);

    // application name
    #ifdef T_BLUEPRINT_BUILD
        a.setApplicationName("theReversi-Blueprint");
        a.setDesktopFileName("com.zumid.theReversi");
    #else
        a.setApplicationName("theReversi");
        a.setDesktopFileName("com.zumid.theReversi");
    #endif

    // version
    a.setApplicationVersion(APP_VERSION);

    // app icons
    a.setApplicationIcon(
                QIcon::fromTheme("thereversi",
                                 QIcon(":/icons/thereversi.svg")
                                 ));

    // splash screen graphic
    a.setAboutDialogSplashGraphic(
                a.aboutDialogSplashGraphicFromSvg(
                    ":/icons/aboutsplash.svg"));

    // theApp is a "... application" (generic name)
    a.setGenericName(QApplication::tr("Can you believe it?"));

    // license
    a.setApplicationLicense(tApplication::Gpl3OrLater);

    // copyright
    a.setCopyrightHolder("Zumi Daxuya");
    a.setCopyrightYear("2020");

    #ifdef GIT_VERSION
        // add git commit ID to the about page
        a.addCopyrightLine(
                    QApplication::tr("Git version %1").arg(GIT_VERSION)
                    );
    #endif

    // suite name
    a.setOrganizationName("theSuite Contrib");

    // set share path, this will be important for translations and such
    QString userSharePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString localSharePath = QDir::cleanPath(QApplication::applicationDirPath() + "/../share/theReversi/");
    QString systemSharePath = "/usr/share/theReversi"; // use global system dir (LINUX)

    if (QDir(userSharePath).exists()) {
        // if user share path exists, use that
        a.setShareDir(userSharePath);
    } else if (QDir(systemSharePath).exists()){
        // if not, use the system share path
        a.setShareDir(systemSharePath);
    } else {
        /* otherwise, use a share dir hack located one level above the
           executable location
        */
        a.setShareDir(localSharePath);
    }

    // set defaults file
    QString defaultsPath = QStandardPaths::writableLocation(
                QStandardPaths::AppDataLocation);
    defaultsPath += "/defaults.conf";
    tSettings::registerDefaults(defaultsPath);   // use local config dir
    tSettings::registerDefaults("/etc/theReversi/defaults.conf"); // use global config dir (LINUX)

    // setup i18n (it reads the translation files from <sharedir>/translations
    a.installTranslators();

    // run app
    MainWindow w;
    w.show();

    return a.exec();
}
