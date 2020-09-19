#include "piece.h"
#include <cstdlib>
#include "board.h"

inline int Max(int a, int b) { return a > b? a : b; }
inline int Min(int a, int b) { return a > b? b : a; }

Piece::Piece(const QString& owner, Board& board, const QPoint& location, PieceType* piece_type, QGraphicsItem* parent):
             QGraphicsPixmapItem(parent), owner_(owner), board_(board), location_(location), piece_type_(piece_type)
{
    setPos(location_.x() * board_.grid_length(), location_.y() * board_.grid_length());
    setPixmap(QPixmap(":/images/" + owner_ + "/" + piece_type_->type() + ".png"));
}

Piece::~Piece()
{
    delete piece_type_;
}

void Piece::MoveTo(const QPoint& loc)
{
    passed_points_.append(location_);
    location_ = loc;
    setPos(location_.x() * board_.grid_length(), location_.y() * board_.grid_length());
}

void Piece::MoveBack()
{
    if (!passed_points_.isEmpty())
    {
        location_ = passed_points_.last();
        setPos(location_.x() * board_.grid_length(), location_.y() * board_.grid_length());
        passed_points_.removeLast();
    }
}

bool Piece::CanMove(const QPoint& loc)
{
    QGraphicsItem* piece = board_.Get(loc);
    if (piece != nullptr && ((Piece*)piece)->owner() == owner_)
    {
        return false;
    }
    return piece_type_->MoveRule(board_, location_, loc);
}

bool King::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
    if (abs(loc.x() - target.x()) + abs(loc.y() - target.y()) == 1)
	{
		return true;
	}
	return false;
}

bool Guard::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
    if (abs(loc.x() - target.x()) == 1 && abs(loc.y() - target.y()) == 1)
	{
		return true;
	}
	return false;
}

bool Knight::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
    if (abs(loc.x() - target.x()) + abs(loc.y() - target.y()) == 3 &&
        loc.x() != target.x() && loc.y() != target.y())
	{
		return true;
	}			
	return false;
}

bool Rook::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
    if (loc == target || (loc.x() != target.x() && loc.y() != target.y()))
	{
		return false;
	}
    else if (loc.x() == target.x())
	{
        for (int i = Min(loc.y(), target.y()) + 1; i < Max(loc.y(), target.y()); i++)
		{
            if (board.Get(QPoint(loc.x(), i)) != nullptr)
			{
				return false;
			}
		}
	}
	else
	{
        for (int j = Min(loc.x(), target.x()) + 1; j < Max(loc.x(), target.x()); j++)
		{
            if (board.Get(QPoint(j, loc.y())) != nullptr)
			{
				return false;
			}
		}
	}
	return true;
}

bool Cannon::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
	int count = 0;
    if (loc == target || (loc.x() != target.x() && loc.y() != target.y()))
	{
		return false;
	}
    else if (loc.x() == target.x())
	{
        for (int i = Min(loc.y(), target.y()) + 1; i < Max(loc.y(), target.y()); i++)
		{
            if (board.Get(QPoint(loc.x(), i)) != nullptr)
			{
				count++;
			}
		}
	}
	else
	{
        for (int j = Min(loc.x(), target.x()) + 1; j < Max(loc.x(), target.x()); j++)
		{
            if (board.Get(QPoint(j, loc.y())) != nullptr)
			{
				count++;
			}
		}
	}
    if (board.Get(target) == nullptr)
	{
		return count == 0;
	}
	else
	{
		return count == 1;
	}
}

bool Pawn::MoveRule(Board& board, const QPoint& loc, const QPoint& target) const
{
    if (abs(loc.x() - target.x()) + abs(loc.y() - target.y()) == 1)
	{
		return true;
	}
	return false;
}
