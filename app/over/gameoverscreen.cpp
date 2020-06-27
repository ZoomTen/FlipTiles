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
