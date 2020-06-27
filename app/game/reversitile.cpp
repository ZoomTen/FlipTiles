#include "reversitile.h"

#include <QPainter>
#include <QStyleOption>

struct ReversiTilePrivate{
    int id;
    int type;
    bool legal;
};

ReversiTile::ReversiTile(QWidget *parent, int id, int type) : QWidget(parent)
{
    d = new ReversiTilePrivate();
    d->id = id;
    d->type = type;
    d->legal = false;

    // round borders
    // https://stackoverflow.com/questions/35333236/how-to-round-qwidget-corners
    this->setFixedSize(32,32);
    this->setAutoFillBackground(true);
//    const int radius = 10;

//    QPainterPath path;
//    path.addRoundedRect(this->rect(), radius, radius);
//    QRegion mask = QRegion(path.toFillPolygon().toPolygon());
    //    this->setMask(mask);
}

ReversiTile::~ReversiTile(){
    delete d;
}

int ReversiTile::getType()
{
    return d->type;
}

void ReversiTile::setLegal(bool legal)
{
    d->legal = legal;
}

void ReversiTile::setType(int type)
{
    d->type = type;
}

void ReversiTile::mousePressEvent(QMouseEvent *event){
    emit emitID(d->id);
}

void ReversiTile::enterEvent(QEvent *e)
{
    if(d->legal){
        this->setStyleSheet("background-color:rgba(128,128,255,.55);");
    }
}

void ReversiTile::leaveEvent(QEvent *e)
{
    resetStylesheet();
}

void ReversiTile::resetStylesheet(){
    this->setStyleSheet("");
}

void ReversiTile::paintEvent(QPaintEvent* event)
{
 QStyleOption opt;
 opt.init(this);
 QPainter p(this);
 style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

 QWidget::paintEvent(event);
}
