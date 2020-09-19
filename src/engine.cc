#include "engine.h"
#include <QtWidgets>
#include "piece.h"
#include "main_window.h"
#include "board.h"

Engine::Engine(MainWindow* main_window, MainFrame* main_frame, Board* board):
               main_window_(main_window), main_frame_(main_frame), board_(board),
               state_(UNINIT), turn_(RED), sub_state_(SELECT),
               selected_item_(nullptr)
{
    connect(main_window_, &MainWindow::SignalNew, this, &Engine::New);
    connect(main_window_, &MainWindow::SignalHelp, this, &Engine::Help);
    connect(main_window_, &MainWindow::SignalAbout, this, &Engine::About);
    connect(main_window_, &MainWindow::SignalCloseWindow, this, &Engine::ProcessCloseWindow);
    connect(main_frame_, &MainFrame::SignalNew, this, &Engine::New);
    connect(main_frame_, &MainFrame::SignalDone, this, &Engine::Done);
    connect(main_frame_, &MainFrame::SignalUndo, this, &Engine::Undo);
    connect(main_frame_, &MainFrame::SignalDraw, this, &Engine::Draw);
    connect(main_frame_, &MainFrame::SignalSurrender, this, &Engine::Surrender);
    connect(board_, &Board::SignalMouseLeft, this, &Engine::MouseLeftPressed);
    connect(board_, &Board::SignalMouseRight, this, &Engine::MouseRightPressed);
}

void Engine::New()
{
    if (state_ != UNINIT)
    {
        return;
    }
    state_ = ARRANGE;
    InitRed();
    main_frame_->actor_label(MainFrame::RED)->show();
    main_frame_->new_button()->setEnabled(false);
    main_window_->SetNewActionEnabled(false);
    main_frame_->done_button()->setEnabled(true);
    loop_.exec();
    FinishTurn();

    ChangePlayer();
    QGraphicsItemGroup* red_pieces = board_->scene()->createItemGroup(board_->items());
    red_pieces->hide();
    InitBlack();
    loop_.exec();
    FinishTurn();

    ChangePlayer();
    main_frame_->SetButtonsEnabled(true);
    main_frame_->new_button()->setEnabled(false);
    main_frame_->done_button()->setEnabled(false);
    red_pieces->show();
    board_->scene()->destroyItemGroup(red_pieces);
    state_ = FIGHT;
}


void Engine::Help()
{
    QMessageBox::about(main_window_, tr("Help"), tr("To see the rule of moving pieces,\n"
                                             "please right-click on the piece."));
}

void Engine::About()
{
    QMessageBox::about(main_window_, tr("About"), tr("<h2>SCKChess</h2>"
                                             "<p>A new chess game"
                                             "<p>By SCK"));
}

void Engine::ProcessCloseWindow(QCloseEvent* event)
{
    if (state_ == UNINIT)
    {
        WriteSettings();
        event->accept();
    }
    else
    {
        int r = QMessageBox::question(main_window_, tr("Stop Game"), tr("The game hasn't ended.\n" "Are you sure to exit?"));
        if (r == QMessageBox::Yes)
        {
            WriteSettings();
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void Engine::Done()
{
    loop_.exit();
}

void Engine::Undo()
{
    if (turn_info_.isEmpty())
    {
        return;
    }
    FinishTurn();
    turn_info_.last().moved_piece()->MoveBack();
    if (turn_info_.last().AnyEaten())
    {
        board_->Add(turn_info_.last().eaten_piece());
    }
    turn_info_.removeLast();
    ChangePlayer();
}

void Engine::Draw()
{
    int r = QMessageBox::question(main_window_, tr("Draw"), tr("Do you agree to draw?"));
    if (r == QMessageBox::Yes)
    {
        QMessageBox::about(main_window_, tr("Game Over"), tr("Draw!"));
        EndGame();
    }
}

void Engine::Surrender()
{
    int r = QMessageBox::question(main_window_, tr("Surrender"), tr("Are you sure to surrender?"));
    if (r == QMessageBox::Yes)
    {
        if (turn_ == RED)
        {
            QMessageBox::about(main_window_, tr("Game Over"), tr("BLACK win!"));
        }
        else if (turn_ == BLACK)
        {
            QMessageBox::about(main_window_, tr("Game Over"), tr("RED win!"));
        }
        EndGame();
    }
}

void Engine::MouseLeftPressed(QMouseEvent* event)
{
    Piece* item = (Piece*)board_->itemAt(event->pos());
    switch (state_)
    {
    case FIGHT:
        Fight(item, event->pos());
        break;
    case ARRANGE:
        Arrange(item);
        break;
    default:
        break;
    }
}

void Engine::MouseRightPressed(QMouseEvent* event)
{
    Piece* item = (Piece*)board_->itemAt(event->pos());
    if (item != nullptr)
    {
        QMessageBox::about(main_window_, item->MyType(), item->MyHelp());
    }
}


void Engine::Arrange(Piece* item)
{
    if (item == nullptr)
    {
        return;
    }
    if (sub_state_ == SELECT)
    {
        selected_item_ = item;
        board_->SetSelect(item);
        sub_state_ = TARGET;
    }
    else
    {
        board_->RemoveSelect();
        board_->Swap(item, selected_item_);
        selected_item_ = nullptr;
        sub_state_ = SELECT;
    }
}

void Engine::Fight(Piece* item, const QPoint& loc)
{
    if (sub_state_ == SELECT && item != nullptr)
    {
        if ((turn_ == RED && item->owner() == "red") ||
            (turn_ == BLACK && item->owner() == "black"))
        {
            selected_item_ = item;
            board_->SetSelect(item);
            sub_state_ = TARGET;
        }
    }
    else if (sub_state_ == TARGET)
    {
        QPoint target(loc.x() / board_->grid_length(), loc.y() / board_->grid_length());
        if (item == nullptr)
        {
            if (selected_item_->CanMove(target))
            {
                turn_info_.append(TurnInfo(selected_item_, selected_item_->location(), target, item));
                selected_item_->MoveTo(target);
                board_->RemoveSelect();
                selected_item_ = nullptr;
                sub_state_ = SELECT;
                ChangePlayer();
            }
        }
        else
        {
            if ((turn_ == RED && item->owner() == "red") ||
                (turn_ == BLACK && item->owner() == "black"))
            {
                sub_state_ = SELECT;
                board_->RemoveSelect();
                selected_item_ = nullptr;
                Fight(item, loc);
            }
            else
            {
                if (selected_item_->CanMove(target))
                {
                    turn_info_.append(TurnInfo(selected_item_, selected_item_->location(), target, item));
                    selected_item_->MoveTo(target);
                    board_->RemoveSelect();
                    board_->Remove(item);
                    sub_state_ = SELECT;
                    ChangePlayer();
                    if (item->MyType() == "king")
                    {
                        QMessageBox::about(main_window_, tr("Game Over"), tr("%1 win!").arg(selected_item_->owner().toUpper()));
                        EndGame();
                    }
                    selected_item_ = nullptr;
                }
            }
        }
    }
}

void Engine::InitRed()
{
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(0, 9), new Rook())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(1, 9), new King())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(2, 9), new Guard())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(3, 9), new Rook())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(0, 8), new Knight())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(1, 8), new Guard())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(2, 8), new Guard())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(3, 8), new Knight())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(0, 7), new Guard())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(1, 7), new Cannon())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(2, 7), new Cannon())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(3, 7), new Pawn())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(0, 6), new Pawn())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(1, 6), new Pawn())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(2, 6), new Pawn())));
    all_red_pieces_.append(board_->Add(new Piece("red", *board_, QPoint(3, 6), new Pawn())));
}

void Engine::InitBlack()
{
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(0, 0), new Rook())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(1, 0), new King())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(2, 0), new Guard())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(3, 0), new Rook())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(0, 1), new Knight())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(1, 1), new Guard())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(2, 1), new Guard())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(3, 1), new Knight())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(0, 2), new Guard())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(1, 2), new Cannon())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(2, 2), new Cannon())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(3, 2), new Pawn())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(0, 3), new Pawn())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(1, 3), new Pawn())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(2, 3), new Pawn())));
    all_black_pieces_.append(board_->Add(new Piece("black", *board_, QPoint(3, 3), new Pawn())));
}

void Engine::ChangePlayer()
{
    main_frame_->ChangePlayer();
    turn_ = (Turn)(1 - turn_);
}

void Engine::EndGame()
{
    FinishTurn();
    DeleteAll();
    board_->Initiate();
    main_frame_->Initiate();
    all_red_pieces_.clear();
    all_black_pieces_.clear();
    turn_info_.clear();
    main_window_->SetNewActionEnabled(true);
    state_ = UNINIT;
    turn_ = RED;
}

void Engine::DeleteAll()
{
    QList<Piece*>::iterator i;
    for (i = all_red_pieces_.begin(); i != all_red_pieces_.end(); i++)
    {
        delete *i;
    }
    for (i = all_black_pieces_.begin(); i != all_black_pieces_.end(); i++)
    {
        delete *i;
    }
}

void Engine::FinishTurn()
{
    if (sub_state_ == TARGET)
    {
        selected_item_ = nullptr;
        board_->RemoveSelect();
        sub_state_ = SELECT;
    }
}
