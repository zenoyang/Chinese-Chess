#include <QApplication>
#include "SingleGame.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    SingleGame board;
    board.show();//这时会调用printEvent

    return app.exec();
}

