#include "gameoverscreen.h"
#include "ui_gameoverscreen.h"

GameOverScreen::GameOverScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOverScreen)
{
    ui->setupUi(this);
}

GameOverScreen::~GameOverScreen()
{
    delete ui;
}

void GameOverScreen::setWinLabel(QString label)
{
    ui->winningLabel->setText(label);
}

void GameOverScreen::setTimeLabel(QString label)
{
    ui->gameTimeLabel->setText(label);
}

void GameOverScreen::on_pushButton_clicked()
{
    emit newGameTrigger();
}
