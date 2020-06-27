#ifndef REVERSITILE_H
#define REVERSITILE_H

#include <QWidget>

struct ReversiTilePrivate;
class ReversiTile : public QWidget
{
    Q_OBJECT

public:
    explicit ReversiTile(QWidget *parent = nullptr, int id=-1, int gridtype = Empty);
    ~ReversiTile();
    void setType(int type);
    int getType();
    void setLegal(bool legal);
    void resetStylesheet();

    enum GridType{
        Empty,
        Dark,
        Light
    };

private:
    ReversiTilePrivate *d;



protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void paintEvent(QPaintEvent* event);

signals:
    void emitID(int id);

};

#endif // REVERSITILE_H
