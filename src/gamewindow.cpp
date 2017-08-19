#include "gamewindow.h"
#include <QAction>
#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent)
{
    /* 菜单栏 */
    _pMenuBar = menuBar();
    _gameMenu = _pMenuBar->addMenu("&游戏");
    //悔棋
    QAction * backAction = _gameMenu->addAction("&悔棋", this, SLOT(slotBack()), QKeySequence::Back);
    //选择难度
    QMenu * chooseLevelMenu = _gameMenu->addMenu("选择难度");
    chooseLevelMenu->addAction("&简单", this, SLOT(slotChooseLevel1()));
    chooseLevelMenu->addAction("&中等", this, SLOT(slotChooseLevel2()));
    chooseLevelMenu->addAction("&困难", this, SLOT(slotChooseLevel3()));
    _gameMenu->addMenu(chooseLevelMenu);
    //退出游戏
    _gameMenu->addSeparator();   //加一条横线
    _gameMenu->addAction("&退出", this, SLOT(close()), QKeySequence::Close);

    QMenu * _helpMenu = _pMenuBar->addMenu("&帮助");
    _helpMenu->addAction("&游戏说明", this, SLOT(slotAbout()), QKeySequence::HelpContents);
    _helpMenu->addSeparator();
    _helpMenu->addAction("&关于", this, SLOT(slotAbout()));


    /* 工具栏 */
    QToolBar * _toolBar = this->addToolBar("MyToolBar");
    //_toolBar->setAllowedAreas(Qt::RightToolBarArea);
    _toolBar->addAction(backAction);
    backAction->setToolTip("悔棋");

    /* 游戏界面 */
    _game = new SingleGame;
    this->setCentralWidget(_game);
}


void GameWindow::slotBack()
{

}

void GameWindow::slotChooseLevel1()
{
    _game->_level = _game->MAX_LEVEL - 2;
}

void GameWindow::slotChooseLevel2()
{
    _game->_level = _game->MAX_LEVEL - 1;
}

void GameWindow::slotChooseLevel3()
{
    _game->_level = _game->MAX_LEVEL;
}



void GameWindow::slotAbout()
{
    QMessageBox::about(this, "关于", "中国象棋\n(C)2016-2017 中国象棋\n作者：杨正\n联系：cookie.yz@qq.com");
}
