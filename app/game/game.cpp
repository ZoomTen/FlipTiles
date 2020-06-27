#include "game.h"
#include "ui_game.h"

#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

struct GamePrivate{
    QList<ReversiTile*> reversiGrid;
    QPalette emptyPalette;
    QPalette darkPalette;
    QPalette lightPalette;

    int currentPlayer;
    QList<bool> legalMoves;
    QList<bool> flipList;

    QElapsedTimer gameTime;
    QTimer* timerUpdater;
};

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    d = new GamePrivate();
    d->timerUpdater = new QTimer;

    // initialize palettes
    QBrush emptyColor(QColor(127, 127, 127, 255));
    QBrush darkColor(QColor(0, 0, 0, 255));
    QBrush lightColor(QColor(255, 255, 255, 255));

    d->emptyPalette.setBrush(QPalette::Active, QPalette::Window, emptyColor);
    d->emptyPalette.setBrush(QPalette::Inactive, QPalette::Window, emptyColor);
    d->emptyPalette.setBrush(QPalette::Disabled, QPalette::Window, emptyColor);

    d->darkPalette.setBrush(QPalette::Active, QPalette::Window, darkColor);
    d->darkPalette.setBrush(QPalette::Inactive, QPalette::Window, darkColor);
    d->darkPalette.setBrush(QPalette::Disabled, QPalette::Window, darkColor);

    d->lightPalette.setBrush(QPalette::Active, QPalette::Window, lightColor);
    d->lightPalette.setBrush(QPalette::Inactive, QPalette::Window, lightColor);
    d->lightPalette.setBrush(QPalette::Disabled, QPalette::Window, lightColor);

    newGame();
}

Game::~Game()
{
    // clear game timer
    d->timerUpdater->deleteLater();
    // clear reversi tiles
    for (int i = 0; i < d->reversiGrid.length(); ++i){
        d->reversiGrid[i]->deleteLater();
    }
    delete d;
    delete ui;
}

void Game::reset(){
    // empty all tiles
    for (int i = 0; i < 64; ++i){
        d->reversiGrid[i]->setType(ReversiTile::Empty);
    }

    // default positions
    d->reversiGrid[27]->setType(ReversiTile::Light);
    d->reversiGrid[28]->setType(ReversiTile::Dark);
    d->reversiGrid[35]->setType(ReversiTile::Dark);
    d->reversiGrid[36]->setType(ReversiTile::Light);

    updatePalettes();

    // dark goes first
    updatePlayer(ReversiTile::Dark);
    d->legalMoves = legalMoves(d->currentPlayer);

    d->gameTime.start();
    d->timerUpdater->setInterval(1000);
    connect(d->timerUpdater, &QTimer::timeout,
            this,            &Game::updateStatusTimer);
    d->timerUpdater->start();

    updateStatus();
}

void Game::newGame(){
    // make new tiles
    for (int i = 0; i < 64; ++i){
        d->reversiGrid.append(new ReversiTile(this, i));
        connect(d->reversiGrid[i], &ReversiTile::emitID,
                this, &Game::processID);
    }

    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 8; ++col){
            ui->gameGrid->addWidget(d->reversiGrid[8*row+col], row, col);
        }
    }

    // default positions
    d->reversiGrid[27]->setType(ReversiTile::Light);
    d->reversiGrid[28]->setType(ReversiTile::Dark);
    d->reversiGrid[35]->setType(ReversiTile::Dark);
    d->reversiGrid[36]->setType(ReversiTile::Light);

    updatePalettes();

    // dark goes first
    updatePlayer(ReversiTile::Dark);
    d->legalMoves = legalMoves(d->currentPlayer);

    updateStatus();

    d->gameTime.start();
    d->timerUpdater->setInterval(1000);
    connect(d->timerUpdater, &QTimer::timeout,
            this,            &Game::updateStatusTimer);
    d->timerUpdater->start();
}

void Game::updateStatus(){
    int darkCount = 0;
    int lightCount = 0;
    int blankCount = 0;

    int legalCount = 0;

    int curTileType;
    for(int i = 0; i < 64; ++i){
        curTileType = d->reversiGrid[i]->getType();
        switch(curTileType){
        case ReversiTile::Empty:
            blankCount++;
            break;
        case ReversiTile::Dark:
            darkCount++;
            break;
        case ReversiTile::Light:
            lightCount++;
            break;
        default:
            break;
        }

        if(d->legalMoves[i]){
            legalCount++;
        }
    }
    ui->darkCount->setText(tr("Dark: %1").arg(darkCount));
    ui->lightCount->setText(tr("Light: %1").arg(lightCount));

    if ((blankCount == 0) || (legalCount == 0)){
        // wait a while, then declare game over
        disconnect(d->timerUpdater, &QTimer::timeout,
                this,            &Game::updateStatusTimer);
        QString statusTimer = QTime::fromMSecsSinceStartOfDay(d->gameTime.elapsed()).toString("mm:ss");
        d->gameTime.invalidate();
        ui->noMoreMovesTxt->setText(tr("No more moves left!"));
        QTimer::singleShot(3000, this, [=]{
            if (darkCount < lightCount){
                emit gameOver(ReversiTile::Light, lightCount, statusTimer);
            } else {
                emit gameOver(ReversiTile::Dark, darkCount, statusTimer);
            }
        });
    } else {
        ui->noMoreMovesTxt->setText("");
    }
}

void Game::processID(int id)
{
    qDebug() << "Pressed tile" << id;
    if (d->legalMoves[id]){
        qDebug() << "Legal move";

        // place tile
        d->reversiGrid[id]->setType(d->currentPlayer);

        // flip tiles in the general direction
        flipTiles(id, d->currentPlayer);

        // switch player
        if (d->currentPlayer == ReversiTile::Dark){
            updatePlayer(ReversiTile::Light);
        } else {
            updatePlayer(ReversiTile::Dark);
        }

        // update stuff
        updatePalettes();
        d->legalMoves = legalMoves(d->currentPlayer);
        updateStatus();
    } else {
        qDebug() << "Illegal move";
    }
}

void Game::updateStatusTimer()
{
    QTime statusTimer = QTime::fromMSecsSinceStartOfDay(d->gameTime.elapsed());
    ui->timeElapsed->setText(tr("Time: %1").arg(statusTimer.toString("mm:ss")));
}

void Game::updatePalette(ReversiTile *tile){
    switch(tile->getType()){
        case ReversiTile::Empty:
            tile->setPalette(d->emptyPalette);
            break;
        case ReversiTile::Dark:
            tile->setPalette(d->darkPalette);
            break;
        case ReversiTile::Light:
            tile->setPalette(d->lightPalette);
            break;
        default:
            break;
    };
    tile->resetStylesheet();
}

void Game::updatePalettes(){
    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 8; ++col){
            updatePalette(d->reversiGrid[8*row+col]);
            updatePalette(d->reversiGrid[8*row+col]); // weird not updating bug, idk why
            // reset legal state
            d->reversiGrid[8*row+col]->setLegal(false);
        }
    }
}

void Game::updatePlayer(int player)
{
    d->currentPlayer = player;
    QString playerString;
    switch(player){
    case ReversiTile::Dark:
        playerString = tr("Dark");
        break;
    case ReversiTile::Light:
        playerString = tr("Light");
        break;
    default:
        playerString = tr("Empty");
        break;
    }
    ui->playerIndicator->setText(tr("Currently playing: %1").arg(playerString));
}

QList<bool> Game::legalMoves(int type)
{
    // https://www.youtube.com/watch?v=7zkl31VtHG4
    // initialize list
    QList<bool> validList;
    for (int i=0; i < 8*8; ++i){
        validList.append(false);
    }

    // check everything
    int index;
    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 8; ++col){
            index = 8*row+col;
            if (d->reversiGrid[index]->getType() == ReversiTile::Empty){
                // check directional valid moves
                // first should be in index 19 (tile 20)
                bool nwValid = isValid(type, -1, -1, row, col); // northwest
                bool nnValid = isValid(type, -1,  0, row, col); // north
                bool neValid = isValid(type, -1,  1, row, col); // northeast
                bool swValid = isValid(type,  1, -1, row, col); // southwest
                bool ssValid = isValid(type,  1,  0, row, col); // south
                bool seValid = isValid(type,  1,  1, row, col); // southeast
                bool wwValid = isValid(type,  0, -1, row, col); // west
                bool eeValid = isValid(type,  0,  1, row, col); // east

                bool anyValid = nwValid || nnValid || neValid ||
                                swValid || ssValid || seValid ||
                                wwValid || eeValid;

                if (anyValid){
                    validList[index] = true;

                    // add hover in ui
                    d->reversiGrid[index]->setLegal(true);
                }
            }
        }
    }
    qDebug() << d->currentPlayer << "Legal moves:" << validList;
    return validList;
}

bool Game::isValid(int who, int off_row, int off_col, int row, int col)
{
    // https://www.youtube.com/watch?v=7zkl31VtHG4
    int other;
    if (who == ReversiTile::Dark){
        other = ReversiTile::Light;
    } else if (who == ReversiTile::Light){
        other = ReversiTile::Dark;
    } else {
        return false;
    }

    int targetRow, targetCol, targetIndex;

    // path finding: check delta*1
    targetRow = row + (off_row*1);
    targetCol = col + (off_col*1);

    if (isInBoard(targetRow, targetCol))
    {
        targetIndex = 8 * targetRow + targetCol;
        if(d->reversiGrid[targetIndex]->getType() != other){
            // not the other player
            return false;
        }
        return checkLineMatch(who, off_row, off_col, row+(1*off_row), col+(1*off_col));
    } else {
        return false;
    }
}

bool Game::checkLineMatch(int who, int off_row, int off_col, int row, int col)
{
    // check for matching color
    int targetRow = row + off_row;
    int targetCol = col + off_col;
    int targetIndex = 8 * targetRow + targetCol;

    if (isInBoard(targetRow, targetCol))
    {
        if ( d->reversiGrid[targetIndex]->getType() == ReversiTile::Empty){
            return false;
        }
        if ( d->reversiGrid[targetIndex]->getType() == who){
            return true;
        }
        return checkLineMatch(who, off_row, off_col, row+off_row, col+off_col);
    } else {
        return false;
    }
}

void Game::flipTiles(int from, int who){
    // initialize list
    QList<bool> flipList;
    for (int i=0; i < 8*8; ++i){
        flipList.append(false);
    }
    d->flipList = flipList;
    int curRow = from / 8;
    int curCol = from % 8;
    tryFlip(who, -1, -1, curRow, curCol); // northwest
    tryFlip(who, -1,  0, curRow, curCol); // north
    tryFlip(who, -1,  1, curRow, curCol); // northeast
    tryFlip(who,  1, -1, curRow, curCol); // southwest
    tryFlip(who,  1,  0, curRow, curCol); // south
    tryFlip(who,  1,  1, curRow, curCol); // southeast
    tryFlip(who,  0, -1, curRow, curCol); // west
    tryFlip(who,  0,  1, curRow, curCol); // east
    qDebug() << d->flipList;

    // do flip tiles
    ReversiTile* curTile;
    for (int i=0; i < 8*8; ++i){
        if(d->flipList[i]){
            curTile = d->reversiGrid[i];
            if(curTile->getType() == ReversiTile::Dark){
                curTile->setType(ReversiTile::Light);
                continue;
            }
            if(curTile->getType() == ReversiTile::Light){
                curTile->setType(ReversiTile::Dark);
                continue;
            }
        }
    }
}

bool Game::isInBoard(int row, int col){
    if ( (row < 0) || (row > 7) ){
        // veer off the board (row)
        return false;
    }
    if ( (col < 0) || (col > 7) ){
        // veer off the board (col)
        return false;
    }
    return true;
}

bool Game::tryFlip(int who, int off_row, int off_col, int row, int col){
    int targetRow = row + off_row;
    int targetCol = col + off_col;
    int targetIndex = 8 * targetRow + targetCol;

    if (isInBoard(targetRow, targetCol))
    {
        if ( d->reversiGrid[targetIndex]->getType() == ReversiTile::Empty){
            return false;
        }
        if ( d->reversiGrid[targetIndex]->getType() == who){
            return true;
        } else {
            if (tryFlip(who, off_row, off_col, targetRow, targetCol)){
                d->flipList[targetIndex] = true;
                return true;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
}
