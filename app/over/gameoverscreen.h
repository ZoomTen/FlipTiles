#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <QWidget>

namespace Ui {
class GameOverScreen;
}

class GameOverScreen : public QWidget
{
    Q_OBJECT

public:
    explicit GameOverScreen(QWidget *parent = nullptr);
    ~GameOverScreen();

    void setWinLabel(QString label);
    void setTimeLabel(QString label);

signals:
    void newGameTrigger();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GameOverScreen *ui;
};

#endif // GAMEOVERSCREEN_H
