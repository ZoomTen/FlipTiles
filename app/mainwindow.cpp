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

// window
#include "mainwindow.h"
#include "ui_mainWindow.h"

// menu items
#include <QMenu>

// opening URLs
#include <QDesktopServices>
#include <QUrl>

// about dialog
#include <taboutdialog.h>

// the-libs CSD
#include <tcsdtools.h>

// stacked widget transition
#include <tstackedwidget.h>

// game funcs
#include "game/game.h"

struct MainWindowPrivate {
    tCsdTools csd;
    QList<QString> playerNamesList;
};

MainWindow::MainWindow(QWidget* parent):
  QMainWindow(parent), ui(new Ui::MainWindow) {
    // setup window
    ui->setupUi(this);
    d = new MainWindowPrivate();
    
    // for CSD: you might want to check autoFillBackground for most widgets
    // register CSD actions
    d->csd.installMoveAction(ui->topWidget);
    d->csd.installResizeAction(this);

    d->playerNamesList = {tr("Empty"), tr("Dark"), tr("Light")};
    
    // CSD layout
    QWidget* csd_widget = d->csd.csdBoxForWidget(this);
    
    if (tCsdGlobal::windowControlsEdge() == tCsdGlobal::Left) {
        // left window controls
        ui->leftCsdLayout->insertWidget(0,csd_widget);
    } else {
        // right window controls
        ui->rightCsdLayout->addWidget(csd_widget);
    }

    // make menu that opens when you click the icon
    QMenu* menu = new QMenu(this);

    // create Help submenu
    QMenu* helpMenu = new QMenu(this);
    helpMenu->setTitle(tr("Help"));
    helpMenu->addAction(ui->actionFileBug); // File a bug
    helpMenu->addAction(ui->actionSources); // Source code link
    helpMenu->addSeparator();
    helpMenu->addAction(ui->actionAbout);   // call About page
    menu->addMenu(helpMenu);                // add Help submenu
    
    menu->addAction(ui->actionExit);        // Exit item

    // fix icon size
    ui->menuButton->setIconSize(SC_DPI_T(QSize(24, 24), QSize));
    ui->menuButton->setFixedSize(SC_DPI(42), QWIDGETSIZE_MAX);

    // attach menu to icon button
    ui->menuButton->setMenu(menu);

    connect(ui->game, &Game::gameOver,
            this,     &MainWindow::switchToGameOver);

    ui->stackedWidget->setCurrentAnimation(tStackedWidget::SlideHorizontal);
}

MainWindow::~MainWindow() {
    // upon unload
    delete d;
    delete ui;
}

/***** actions *****/

// exit
void MainWindow::on_actionExit_triggered() {
    QApplication::exit();
}

// help actions
void MainWindow::on_actionSources_triggered() {
    // open source code page
    QDesktopServices::openUrl(QUrl("http://github.com/ZoomTen/FlipTiles"));
}

void MainWindow::on_actionFileBug_triggered() {
    // open issue tracker
    QDesktopServices::openUrl(QUrl("http://github.com/ZoomTen/FlipTiles/issues"));
}

void MainWindow::on_actionAbout_triggered() {
    // open the library-provided about dialog
    tAboutDialog d;
    d.exec();
}

/***** events *****/
void MainWindow::switchToGameOver(int winningplayer, int winningcount, QString timeString){
    ui->over->setWinLabel(tr("And the winner is %1, with a total of %2 tiles!").arg(d->playerNamesList[winningplayer]).arg(winningcount));
    ui->over->setTimeLabel(tr("Game Time: %1").arg(timeString));
    ui->stackedWidget->setCurrentIndex(1);
    connect(ui->over, &GameOverScreen::newGameTrigger,
            this,     &MainWindow::startNewGame);
}

void MainWindow::startNewGame(){
    disconnect(ui->over, &GameOverScreen::newGameTrigger,
            this,     &MainWindow::startNewGame);
    ui->game->reset();
    ui->stackedWidget->setCurrentIndex(0);
}
