#include <QApplication>
#include <gamewindow.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GameWindow w;
    w.show();
    w.setWindowTitle("中国象棋");

    return app.exec();
}

