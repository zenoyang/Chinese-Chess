#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Chessman.h"
#include "Step.h"
#include <QPoint>
#include <QVector>

class Board : public QWidget
{
    Q_OBJECT
public:
    int r; //棋子的半径、格子一半的宽度
    int selectid;   //被选中的棋子
    bool beRedTurn; //交换玩家
    bool beSide;

    QVector<Step *> steps;
    Chessman chessman[32];

    explicit Board(QWidget *parent = 0);
    void init(bool beRedSide);

    /* 绘制 */
    virtual QSize sizeHint () const;
    //所有的绘制都在paintEvent中
    void paintEvent(QPaintEvent *);
    //将棋子在棋盘中的坐标转换成在窗口中的坐标
    QPoint center(int row, int col);
    QPoint center(int id);
    //将窗口中的位置转换成棋盘上的位置
    QPoint windowCenter(int row, int col);
    void drawChessman(QPainter & painter, int id);

    //鼠标释放时有效
    void mouseReleaseEvent(QMouseEvent *);
    bool getClickRowCol(QPoint pt, int &row, int &col);  //注意这里是引用
    int getChessmanid(int row, int col);
    int getChessmanCountAtLine(int _row, int _col, int row, int col);
    void click(QPoint point);
    void tryMove();
    virtual void click(int id, int row, int col);
    void trySelectChessman(int id);
    void tryMoveChessman(int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void killChessman(int id);
    void reliveChessman(int id);
    void moveChessman(int moveid, int row, int col);
    void moveChessman(int moveid, int killid, int row, int col);

    /* 规则 */
    bool canMove(int moveid, int row, int col, int killid);
    bool canMoveJiang(int moveid, int row, int col, int);
    bool canMoveShi(int moveid, int row, int col, int);
    bool canMoveXiang(int moveid, int row, int col, int);
    bool canMoveChe(int moveid, int row, int col, int);
    bool canMoveMa(int moveid, int row, int col, int);
    bool canMovePao(int moveid, int row, int col, int killid);
    bool canMoveBing(int moveid, int row, int col, int);

    void backOne();
    void back(Step* step);
    virtual void back();

public slots:
    void slotBack();
};

#endif // BOARD_H
