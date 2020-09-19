#include "board.h"
#include <QtWidgets>
#include "piece.h"

Board::Board(int row_num, int col_num, int grid_length, QWidget* parent):
             QGraphicsView(parent), row_num_(row_num), col_num_(col_num), grid_length_(grid_length),
             board_width_(col_num * grid_length_), board_height_(row_num_ * grid_length_),
             select_pic_(QPixmap(":/images/select.png"))
{
    //select_pic_.setAcceptedMouseButtons(Qt::NoButton);
    scene_ = new QGraphicsScene(0, 0, board_width_, board_height_);
    scene_->setBackgroundBrush(QBrush(QPixmap(":/images/bg.png")));
    scene_->setForegroundBrush(QBrush(Qt::CrossPattern));
    setScene(scene_);
	setViewportUpdateMode(FullViewportUpdate);
}

void Board::resizeEvent(QResizeEvent* event)
{
    fitInView(scene_->sceneRect());
	QGraphicsView::resizeEvent(event);
}

void Board::drawForeground(QPainter* painter, const QRectF& rect)
{
    for (int x = 1; x < rect.width(); x += grid_length_)
	{
		painter->drawLine(x, 1, x, rect.height());
	}
    for (int y = 1; y < rect.height(); y += grid_length_)
	{
		painter->drawLine(1, y, rect.width(), y);
	}
}

void Board::mousePressEvent(QMouseEvent* event)
{
    ;
    if (itemAt(event->pos()) == &select_pic_)
    {
        return;
    }
    if (event->button() == Qt::LeftButton)
    {
        emit SignalMouseLeft(event);
    }
    else if (event->button() == Qt::RightButton)
    {
        emit SignalMouseRight(event);
    }
}

Piece* Board::Add(Piece* item)
{
    if (item->scene() == nullptr)
    {
        scene_->addItem(item);
    }
    return item;
}

Piece* Board::Get(const QPoint& loc)
{
    return (Piece*)itemAt((loc * grid_length_) + QPoint(grid_length_ / 2, grid_length_ / 2));
}

Piece* Board::Remove(const QPoint& loc)
{
    Piece* temp = Get(loc);
    if (temp != nullptr && temp->scene() != nullptr)
    {
            scene_->removeItem((QGraphicsItem*)temp);
    }
    return temp;
}

Piece* Board::Remove(Piece* piece)
{
    if (piece != nullptr && piece->scene() != nullptr)
    {
            scene_->removeItem((QGraphicsItem*)piece);
    }
    return piece;
}

void Board::Swap(const QPoint& loc1, const QPoint& loc2)
{
}

void Board::Swap(Piece* p1, Piece* p2)
{
    QPoint temp(((Piece*)p1)->location());
    ((Piece*)p1)->MoveTo(((Piece*)p2)->location());
    ((Piece*)p2)->MoveTo(temp);
}

void Board::SetSelect(const QPoint& loc)
{
    select_pic_.setPos(loc);
    scene_->addItem(&select_pic_);
}

void Board::SetSelect(Piece* piece)
{
    select_pic_.setPos(piece->location() * grid_length_);
    scene_->addItem(&select_pic_);
}

void Board::RemoveSelect()
{
    scene_->removeItem(&select_pic_);
}

void Board::Initiate()
{
    scene_->clear();
}
