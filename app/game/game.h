#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "reversitile.h"

namespace Ui {
class Game;
}

struct GamePrivate;
class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void reset();
    void declareGameOver();

signals:
    void gameOver(int winningplayer, int winningcount, QString timeString);

private:
    Ui::Game *ui;
    GamePrivate *d;
    void updatePalette(ReversiTile* tile);
    void updatePalettes();
    void updatePlayer(int player);

    void flipTiles(int from, int who);
    bool tryFlip(int who, int off_row, int off_col, int row, int col);
    void updateStatus();
    void newGame();
    bool isInBoard(int row, int col);

    QList<bool> legalMoves(int type);
    bool isValid(int who, int off_row, int off_col, int row, int col);
    bool checkLineMatch(int who, int off_row, int off_col, int row, int col);

private slots:
    void processID(int id);
    void updateStatusTimer();
};

#endif // GAME_H
