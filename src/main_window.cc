#include "main_window.h"

#include <QtWidgets>


MainWindow::MainWindow(QWidget* central_widget)
{
    setCentralWidget(central_widget);
	CreateActions();
	CreateMenus();
	CreateStatusBar();
    emit SignalReadSettings();
    setWindowIcon(QIcon(":/images/icon.png"));
	resize(sizeHint());
}

void MainWindow::CreateActions()
{
    new_action_ = new QAction(tr("&New Game"), this);
    new_action_->setIcon(QIcon(":/images/new.png"));
    new_action_->setShortcut(QKeySequence::New);
    new_action_->setStatusTip(tr("Start a new game"));
    connect(new_action_, &QAction::triggered, this, &MainWindow::SignalNew);

    exit_action_ = new QAction(tr("E&xit"), this);
    exit_action_->setShortcut(tr("Ctrl+Q"));
    exit_action_->setStatusTip(tr("Exit the game"));
    connect(exit_action_, &QAction::triggered, this, &MainWindow::close);

    about_action_ = new QAction(tr("&About"), this);
    about_action_->setStatusTip(tr("Show the information of the game"));
    connect(about_action_, &QAction::triggered, this, &MainWindow::SignalAbout);

    help_action_ = new QAction(tr("&Help"), this);
    help_action_->setShortcut(tr("Ctrl+H"));
    help_action_->setStatusTip(tr("Get help about the game"));
    connect(help_action_, &QAction::triggered, this, &MainWindow::SignalHelp);
}

void MainWindow::CreateMenus()
{
    game_menu_ = menuBar()->addMenu(tr("&Game"));
    game_menu_->addAction(new_action_);
    game_menu_->addSeparator();
    game_menu_->addAction(exit_action_);

    edit_menu_ = menuBar()->addMenu(tr("&Edit"));

    help_menu_ = menuBar()->addMenu(tr("&Help"));
    help_menu_->addAction(help_action_);
    help_menu_->addAction(about_action_);
}

void MainWindow::CreateStatusBar()
{
	statusBar();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit SignalCloseWindow(event);
}

void MainWindow::SetNewActionEnabled(bool enable)
{
    if (enable)
    {
        connect(new_action_, &QAction::triggered, this, &MainWindow::SignalNew);
    }
    else
    {
        disconnect(new_action_, &QAction::triggered, this, &MainWindow::SignalNew);
    }
}

MainFrame::MainFrame(QWidget* board, QWidget* parent): QFrame(parent)
{
    name_label_[RED] = new QLabel(tr("RED"));
    name_label_[BLACK] = new QLabel(tr("BLACK"));
    pic_label_[RED] = new QLabel;
    pic_label_[RED]->setPixmap(QPixmap(":/images/p1.png"));
    pic_label_[BLACK] = new QLabel;
    pic_label_[BLACK]->setPixmap(QPixmap(":/images/p2.png"));
    actor_label_[RED] = new QLabel;
    actor_label_[RED]->setPixmap(QPixmap(":/images/play.png"));
    actor_label_[RED]->hide();
    actor_label_[BLACK] = new QLabel;
    actor_label_[BLACK]->setPixmap(QPixmap(":/images/play.png"));
    actor_label_[BLACK]->hide();
    new_button_ = new QPushButton(tr("&New"));
    connect(new_button_, &QPushButton::clicked, this, &MainFrame::SignalNew);
    done_button_ = new QPushButton(tr("&Done"));
    done_button_->setEnabled(false);
    connect(done_button_, &QPushButton::clicked, this, &MainFrame::SignalDone);
    undo_button_ = new QPushButton(tr("&Undo"));
    undo_button_->setEnabled(false);
    connect(undo_button_, &QPushButton::clicked, this, &MainFrame::SignalUndo);
    draw_button_ = new QPushButton(tr("&Draw"));
    draw_button_->setEnabled(false);
    connect(draw_button_, &QPushButton::clicked, this, &MainFrame::SignalDraw);
    surrender_button_ = new QPushButton(tr("&Surrender"));
    surrender_button_->setEnabled(false);
    connect(surrender_button_, &QPushButton::clicked, this, &MainFrame::SignalSurrender);

    QVBoxLayout* left_layout = new QVBoxLayout;
    left_layout->addWidget(name_label_[BLACK]);
    left_layout->addWidget(pic_label_[BLACK]);
    left_layout->addWidget(actor_label_[BLACK]);
    left_layout->addStretch();
    left_layout->addWidget(new_button_);
    left_layout->addWidget(done_button_);
    left_layout->addWidget(undo_button_);
    left_layout->addWidget(draw_button_);
    left_layout->addWidget(surrender_button_);
    left_layout->addStretch();
    left_layout->addWidget(name_label_[RED]);
    left_layout->addWidget(pic_label_[RED]);
    left_layout->addWidget(actor_label_[RED]);
	
	QGridLayout* main_layout = new QGridLayout;
	main_layout->addLayout(left_layout, 0, 0);
    main_layout->addWidget(board, 0, 1);
	setLayout(main_layout);
}

void MainFrame::Initiate()
{
    SetButtonsEnabled(false);
    new_button_->setEnabled(true);
    actor_label_[RED]->hide();
    actor_label_[BLACK]->hide();
}

void MainFrame::ChangePlayer()
{
    actor_label_[RED]->setVisible(!actor_label_[RED]->isVisible());
    actor_label_[BLACK]->setVisible(!actor_label_[BLACK]->isVisible());
}

void MainFrame::SetButtonsEnabled(bool enable)
{
    new_button_->setEnabled(enable);
    done_button_->setEnabled(enable);
    undo_button_->setEnabled(enable);
    draw_button_->setEnabled(enable);
    surrender_button_->setEnabled(enable);
}
