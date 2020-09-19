#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <list>

//const int kGridWidth = QPixmap(":/images/select.png").width();
//const int kGridHeight = QPixmap(":/images/select.png").height();

class Piece;
class Board : public QGraphicsView
{
    Q_OBJECT
public:
    Board(int row_num, int col_num, int grid_length, QWidget *parent = 0);
	int row_num() const { return row_num_; }
	int col_num() const { return col_num_; }
    int grid_length() const { return grid_length_; }
    int board_width() const { return board_width_; }
    int board_height() const { return board_height_; }
    Piece* Add(Piece* item);
    Piece* Get(const QPoint& loc);
    Piece* Remove(const QPoint& loc);
    Piece* Remove(Piece* piece);
	void Swap(const QPoint& loc1, const QPoint& loc2);
    void Swap(Piece* p1, Piece* p2);
    void SetSelect(const QPoint& loc);
    void SetSelect(Piece* piece);
    void RemoveSelect();
    void Initiate();

signals:
    void SignalMouseLeft(QMouseEvent* event);
    void SignalMouseRight(QMouseEvent* event);

protected:
    virtual void resizeEvent(QResizeEvent* event);
	virtual void drawForeground(QPainter* painter, const QRectF& rect);
    virtual void mousePressEvent(QMouseEvent* event);

private:
	int row_num_;
	int col_num_;
    int grid_length_;
    int board_width_;
	int board_height_;
    QGraphicsScene* scene_;
    QGraphicsPixmapItem select_pic_;
};

#endif
