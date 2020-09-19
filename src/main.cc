#include <QApplication>
#include "main_window.h"
#include "board.h"
#include "engine.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    Board* board = new Board(10, 4, 57);
    MainFrame* frame = new MainFrame(board);
    MainWindow* window = new MainWindow(frame);
    Engine sckchess(window, frame, board);
    window->show();
	return app.exec();
}
