#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "SingleGame.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>


class GameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = 0);

    SingleGame * _game;

    QMenuBar * _pMenuBar;
    QMenu * _gameMenu;
    //QMenu * _helpMenu;
    //QToolBar * _toolBar;

signals:

public slots:
    void slotBack();
    void slotChooseLevel1();
    void slotChooseLevel2();
    void slotChooseLevel3();
    void slotAbout();
};

#endif // GAMEWINDOW_H
