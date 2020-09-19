#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QFrame>

class QAction;
class QLabel;
class QPushButton;
class QGraphicsScene;
class Board;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget* central_widget);
    void SetNewActionEnabled(bool enable);

protected:
	void closeEvent(QCloseEvent* event);

signals:
    void SignalNew();
    void SignalHelp();
    void SignalAbout();
    void SignalCloseWindow(QCloseEvent* event);
    void SignalReadSettings();
    void SignalWriteSettings();

private:
	void CreateActions();
    void CreateMenus();
    void CreateStatusBar();
	bool OkToContinue();
    QAction* new_action_;
    QAction* exit_action_;
    QAction* help_action_;
    QAction* about_action_;
    QMenu* game_menu_;
    QMenu* edit_menu_;
    QMenu* help_menu_;
};

class MainFrame : public QFrame
{
	Q_OBJECT

public:
    enum Player {RED, BLACK};
    MainFrame(QWidget* board, QWidget* parent = 0);
    QLabel* actor_label(Player player) { return actor_label_[player]; }
    QPushButton* new_button() { return new_button_; }
    QPushButton* done_button() { return done_button_; }
    void SetButtonsEnabled(bool enable);
    void InitGame();
    void EndGame();
    void ChangePlayer();
    void Initiate();

signals:
    void SignalNew();
    void SignalDone();
    void SignalUndo();
    void SignalDraw();
    void SignalSurrender();

private:
    QLabel* name_label_[2];
    QLabel* pic_label_[2];
    QLabel* actor_label_[2];
    QPushButton* new_button_;
    QPushButton* done_button_;
    QPushButton* undo_button_;
    QPushButton* draw_button_;
    QPushButton* surrender_button_;
};

#endif
