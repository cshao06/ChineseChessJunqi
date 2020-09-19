#ifndef PIECE_H
#define PIECE_H

#include <QString>
#include <QPoint>
#include <QGraphicsPixmapItem>

class Board;
class Piece;

class PieceType
{
public:
    PieceType(QString type, QString help): type_(type), help_(help) {}
    virtual ~PieceType() {}
    QString type() const { return type_; }
    QString help() const { return help_; }
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const = 0;
private:
    QString type_;
    QString help_;
};

class King : public PieceType
{
public:
    King(): PieceType("king", "A king moves in the ordinary way.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
};

class Guard : public PieceType
{
public:
    Guard(): PieceType("guard", "A guard moves in the ordinary way.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
};

class Knight : public PieceType
{
public:
    Knight(): PieceType("knight", "A knight can move even though it's blocked.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
private:
};

class Rook : public PieceType
{
public:
    Rook(): PieceType("rook", "A rook moves in the ordinary way.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
private:
};

class Cannon : public PieceType
{
public:
    Cannon(): PieceType("cannon", "A cannon moves in the ordinary way.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
private:
};

class Pawn : public PieceType
{
public:
    Pawn(): PieceType("pawn", "A pawn moves like a king.") {}
    virtual bool MoveRule(Board& board, const QPoint& loc, const QPoint& target) const;
private:
};

class Piece : public QGraphicsPixmapItem
{
public:
    Piece(const QString& owner, Board& board, const QPoint& location, PieceType* piece_type, QGraphicsItem* parent = 0);
    ~Piece();
    Board& board() { return board_; }
    QPoint location() const { return location_; }
    QString MyType() { return piece_type_->type(); }
    QString MyHelp() { return piece_type_->help(); }
    QString owner() const { return owner_; }
    //void set_location(const QPoint& loc) { location_ = loc; }
    void set_piece_type(PieceType* piece_type) { piece_type_ = piece_type; }
    bool CanMove(const QPoint& loc);
    void MoveTo(const QPoint& loc);
    void MoveBack();

private:
    QString owner_;
    Board& board_;
    QPoint location_;
    PieceType* piece_type_;
    QList<QPoint> passed_points_;
};

#endif
