#ifndef ENGINE_H
#define ENGINE_H
#include <QEventLoop>
#include <QPoint>

class MainWindow;
class MainFrame;
class Board;
class Piece;
class QMouseEvent;
class QCloseEvent;
class TurnInfo;

class Engine : public QObject
{
    Q_OBJECT
public:
    enum State {UNINIT, ARRANGE, FIGHT, END};
    enum Turn {RED, BLACK};
    enum SubState {SELECT, TARGET};
    Engine(MainWindow* main_window, MainFrame* main_frame, Board* board);
    void Arrange(Piece* item);
    void Fight(Piece* item, const QPoint& loc);
    void FinishTurn();
    void InitRed();
    void InitBlack();
    void ChangePlayer();
    void EndGame();
    void DeleteAll();

public slots:
    void New();
    void Help();
    void About();
    void Done();
    void Undo();
    void Draw();
    void Surrender();
    void ReadSettings() {}
    void WriteSettings() {}
    void MouseLeftPressed(QMouseEvent* event);
    void MouseRightPressed(QMouseEvent* event);
    void ProcessCloseWindow(QCloseEvent* event);

private:
    MainWindow* main_window_;
    MainFrame* main_frame_;
    Board* board_;
    State state_;
    Turn turn_;
    SubState sub_state_;
    Piece* selected_item_;
    QList<TurnInfo> turn_info_;
    QList<Piece*> all_red_pieces_;
    QList<Piece*> all_black_pieces_;
    QEventLoop loop_;
};

class TurnInfo
{
public:
    TurnInfo(Piece* moved_piece, QPoint from, QPoint to, Piece* eaten_piece):
             moved_piece_(moved_piece), from_(from), to_(to), eaten_piece_(eaten_piece) {}
    Piece* eaten_piece() { return eaten_piece_; }
    Piece* moved_piece() { return moved_piece_; }
    bool AnyEaten() { return eaten_piece_ != nullptr; }
private:
    Piece* moved_piece_;
    QPoint from_;
    QPoint to_;
    Piece* eaten_piece_;
};

#endif // ENGINE_H
