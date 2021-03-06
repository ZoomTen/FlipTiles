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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

struct MainWindowPrivate;
class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private slots:
        // app actions
        void on_actionExit_triggered();
        void on_actionAbout_triggered();
        void on_actionSources_triggered();
        void on_actionFileBug_triggered();

        void switchToGameOver(int winningplayer, int winningcount, QString timestring);
        void startNewGame();

        void on_actionNew_triggered();

        void on_actionGive_Up_triggered();

private:
        Ui::MainWindow* ui;
        MainWindowPrivate* d;
};
#endif // MAINWINDOW_H
