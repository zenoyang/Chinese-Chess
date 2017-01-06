#include "Board.h"
#include "Chessman.h"
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>


Board::Board(QWidget *parent) : QWidget(parent)
{
    this->r = 30;   //棋子半径
    init(true);
}

void Board::init(bool beRedSide)
{
    //给棋子初始化信息
    for (int i = 0; i < 32; ++i)
    {
        chessman[i].init(i);
    }
//    if (!beRedSide)
//    {
//        for (int i  = 0; i < 32; ++i)
//        {
//            chessman[i].rotate();
//        }
//    }
    selectid = -1;
    beRedTurn = true;
    beSide = beRedSide;
    //update();
}

//设置窗口默认大小
QSize Board::sizeHint() const
{
    int d = 60; //一个单元的大小
    return QSize(10*d, 11*d);
}

//所有绘制都在paintEvent中
void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 2 * this->r; //一个单元的大小

    painter.setPen(QPen(Qt::black, 2));

    //画横线
    for(int i = 1; i <= 10; ++i)
    {
        painter.drawLine(QPoint(d, i*d), QPoint(9*d, i*d));
    }

    //画竖线
    for(int i = 1; i <=9; ++i)
    {
        if (i == 1 || i == 9)
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 10*d));
        else
        {
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 5*d));
            painter.drawLine(QPoint(i*d, 6*d), QPoint(i*d, 10*d));
        }
    }

    //画九宫格
    painter.drawLine(QPoint(4*d, d), QPoint(6*d, 3*d));
    painter.drawLine(QPoint(4*d, 3*d), QPoint(6*d, d));
    painter.drawLine(QPoint(4*d, 8*d), QPoint(6*d, 10*d));
    painter.drawLine(QPoint(4*d, 10*d), QPoint(6*d, 8*d));

    //写字
    painter.setFont(QFont("华文隶书", 26));
    painter.drawText(QPoint(2*d, 5.7*d)," 楚 河               汉 界" );

    //绘制32颗棋子
    for (int i = 0; i < 32; i++)
    {
        drawChessman(painter, i);
    }
}

void Board::mouseReleaseEvent(QMouseEvent * event)
{
    //鼠标点击得到的位置
    QPoint point = event->pos();

    click(point);

}

void Board::click(QPoint point)
{
    int row, col;

    if (!getClickRowCol(point, row, col))   //如果在棋盘外,不做任何反应
        return;

    int id = getChessmanid(row, col);
    click(id, row, col);
}

void Board::click(int clickid, int row, int col)
{
    if(selectid == -1)  //如果之前没有棋子被选中
    {
        trySelectChessman(clickid);
    }
    else
    {
        tryMoveChessman(clickid, row, col);
    }
}

void Board::tryMoveChessman(int killid, int row, int col)
{
    if (canMove(selectid, row, col, killid))
    {
        //这里的selectid是第一次点击选中的棋子
        chessman[selectid].row = row;
        chessman[selectid].col = col;
        if (killid != -1)  //确保第二次也选中了棋子
            chessman[killid].dead = true;
        else
            trySelectChessman(killid);
        update();

        //交换玩家
        beRedTurn = !beRedTurn;
        selectid = -1;
    }
}

void Board::trySelectChessman(int id)
{
    if (id == -1)
        return ;

    if (beRedTurn == chessman[id].red)
    {
        selectid = id;
        update();
    }
}

//将棋子在棋盘中的坐标转换成在窗口中的坐标
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * 2 * r;   //注意，这里要赋值给QPoint类中的rx()ry()函数，而非x y
    ret.ry() = (row + 1) * 2 * r;
    return ret;
}

QPoint Board::center(int id)
{
    return Board::center(chessman[id].row, chessman[id].col);
}

//将窗口中的位置转换成棋盘上的位置
QPoint Board::windowCenter(int row, int col)
{
    QPoint ret;
    ret.rx() = (row - 1) * 2 * r;//注意，这里要赋值给QPoint类中的rx()ry()函数，而非x y
    ret.ry() = (col - 1) * 2 * r;
    return ret;
}

//绘制一个棋子
void Board::drawChessman(QPainter & painter, int id)
{
    if (chessman[id].dead)
        return;

    QPoint c = center(chessman[id].row, chessman[id].col);
    QRect rect = QRect(c.x()-r, c.y()-r, 2*r, 2*r);

    if (id == selectid)
        painter.setBrush(Qt::gray); //被选中的棋子为灰色
    else
        painter.setBrush(QColor(255,193,37));

    //painter.setFont(QFont("华文隶书", 26));

    if (chessman[id].red)
    {
        painter.setPen(QPen(Qt::red,2));
        painter.drawEllipse(center(id), r, r);  //_r长半径 _r短半径

        painter.drawText(rect,chessman[id].getText(),QTextOption(Qt::AlignCenter));
        painter.setPen(QPen(Qt::black,2));
    }
    else
    {
        painter.setPen(QPen(Qt::black,2));
        painter.drawEllipse(center(id), r, r);  //_r长半径 _r短半径

        painter.drawText(rect,chessman[id].getText(),QTextOption(Qt::AlignCenter));
    }
    //关于drawEllipse画圆  http://blog.csdn.net/zcf1002797280/article/details/7885651
    //关于setPen画线  http://blog.csdn.net/dong_zhihong/article/details/7878246
    //关于setFont设置字体  http://baidu.com
}


bool Board::canMove(int moveid, int row, int col, int killid)
{
    //颜色一样，换选择，避免吃掉自家棋子
    if (chessman[moveid].red == chessman[killid].red)
    {
        selectid = killid;
        update();
        return false;
    }

    switch(chessman[moveid].type)
    {
    case Chessman::JIANG:
        return canMoveJiang(moveid, row, col, killid);
        break;
    case Chessman::SHI:
        return canMoveShi(moveid, row, col, killid);
        break;
    case Chessman::CHE:
        return canMoveChe(moveid, row, col, killid);
        break;
    case Chessman::MA:
        return canMoveMa(moveid, row, col, killid);
        break;
    case Chessman::PAO:
        return canMovePao(moveid, row, col, killid);
        break;
    case Chessman::BING:
        return canMoveBing(moveid, row, col, killid);
        break;
    case Chessman::XIANG:
        return canMoveXiang(moveid, row, col, killid);
        break;
    }
    return true;
}

bool Board::canMoveJiang(int moveid, int row, int col, int)//这里吧killid删了，可以避免警告
{
    //只能在九宫格内走
    //一次只能走一格
    //特殊情况：与对方将对视时可以直接吃掉对方的将     ***还没实现
    if (chessman[moveid].red)
    {
        if (row < 7)
            return false;
    }
    else    //黑棋
    {
        if (row > 2)
            return false;
    }

    if (col < 3 || col > 5)
        return false;

    //这个b值可以控制棋子的飞行方式，如一格、日字、田字等
    int b = abs(row - chessman[moveid].row) * 10 + abs(col - chessman[moveid].col);
    if (b == 10 || b == 1)
        return true;
    else
        return false;
}

bool Board::canMoveShi(int moveid, int row, int col, int)
{
    //只能在九宫格内走
    //一次只能走一格的对角线
    if (chessman[moveid].red)
    {
        if (row < 7)
            return false;
    }
    else    //黑棋
    {
        if (row > 2)
            return false;
    }

    if (col < 3 || col > 5)
        return false;

    //这个b的取值可以控制棋子的飞行方式，如一格、日字、田字等
    int b = abs(row - chessman[moveid].row) * 10 + abs(col - chessman[moveid].col);
    if (b == 11)
        return true;
    else
        return false;
}

bool Board::canMoveXiang(int moveid, int row, int col, int)
{
    //只能在自家走
    //一次只能走一个田子
    //田子中间有棋子时不能走   ***还没实现
    if (chessman[moveid].red)
    {
        if (row < 5)
            return false;
    }
    else    //黑棋
    {
        if (row > 4)
            return false;
    }

    int rowEye = (row + chessman[moveid].row)/2;
    int colEye = (col + chessman[moveid].col)/2;
    if(getChessmanid(rowEye, colEye) != -1)
        return false;

    //这个b的取值可以控制棋子的飞行方式，如一格、日字、田字等
    int b = abs(row - chessman[moveid].row) * 10 + abs(col - chessman[moveid].col);
    if (b == 22)
        return true;
    else
    return false;
}

bool Board::canMoveChe(int moveid, int row, int col, int)
{
    //走直线、两点中间不能有棋子
    if (getChessmanCountAtLine(chessman[moveid].row, chessman[moveid].col, row, col) == -1)
        return false;
    if (getChessmanCountAtLine(chessman[moveid].row, chessman[moveid].col, row, col) > 0)
        return false;

    return true;
}

bool Board::canMoveMa(int moveid, int row, int col, int)
{
    int b = abs(row - chessman[moveid].row) * 10 + abs(col - chessman[moveid].col);
    if (b != 21 && b != 12)
        return false;

    if(b == 12)
    {
        if(getChessmanid(chessman[moveid].row, (col+chessman[moveid].col)/2) != -1)
            return false;
    }
    else
    {
        if(getChessmanid((row+chessman[moveid].row)/2, chessman[moveid].col) != -1)
            return false;
    }

    return true;

}

bool Board::canMovePao(int moveid, int row, int col, int killid)
{
    //走直线、两点中间必须有一个棋子
    int count = getChessmanCountAtLine(chessman[moveid].row, chessman[moveid].col, row, col);
    if ( count == -1)
        return false;

    if(killid != -1)
    {
        if(count == 1) return true;
    }
    else
    {
        if(count == 0) return true;
    }
    return false;

}

bool Board::canMoveBing(int moveid, int row, int col, int)
{
    //一次只能走一个格子
    //不能后退
    //过河前不能左右移动

    //这个b的取值可以控制棋子的飞行方式，如一格、日字、田字等
    int b = abs(row - chessman[moveid].row) * 10 + abs(col - chessman[moveid].col);
    if (b != 10 && b != 1)
        return false;

    if(chessman[moveid].red)
    {
        if(row > chessman[moveid].row)
            return false;
        if(chessman[moveid].row >= 5 && row == chessman[moveid].row)
            return false;
    }
    else
    {
        if(row < chessman[moveid].row)
            return false;
        if(chessman[moveid].row <= 4 && row == chessman[moveid].row)
            return false;
    }

    return true;
}

//如果不在一条线上就返回-1， 如果在一条线上就返回两个位置之间的棋子数量
//车、炮 用到该函数
int Board::getChessmanCountAtLine(int _row, int _col, int row, int col)
{
    //_row_col是棋子的位置 rowcol是要移到的位置
    int count = 0;
    if (_row != row && _col != col)
        return -1;
    if (_row == row && _col == col)
        return -1;

    if (_row == row)
    {
        int min = _col < col ? _col : col;
        int max = _col < col ? col : _col;
        for (int col1 = min+1; col1 < max; ++col1)
        {
            if (getChessmanid(row, col1) != -1)
                count ++;
        }
    }
    else
    {
        int min = _row < row ? _row : row;
        int max = _row < row ? row : _row;
        for (int row1 = min+1; row1 < max; ++row1)
        {
            if (getChessmanid(row1, col) != -1)
                count ++;
        }
    }

    return count;
}


//返回一个位置上棋子的id，如果该位置上没有棋子，返回-1
//马、炮、车、象 用到
int Board::getChessmanid(int row, int col)
{
    for (int i = 0; i < 32; ++i)
    {
        if (chessman[i].row == row && chessman[i].col == col && !chessman[i].dead)
            return i;
    }

    return -1;
}

//这里先有一个很笨的方法，效率不高
bool Board::getClickRowCol(QPoint pt, int &row, int &col)  //引用
{
    for (row = 0; row <= 9; ++row)
    {
        for (col = 0; col <= 8; ++col)
        {
            QPoint c = center(row, col);
            int dist = (pt.x()-c.x())*(pt.x()-c.x()) + (pt.y()-c.y())*(pt.y()-c.y());
            if (dist < r*r)
                return true;
        }
    }
    return false;   //在棋盘外
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps1)
{
    int row1 = chessman[moveid].row;
    int col1 = chessman[moveid].col;

    Step* step = new Step;
    step->ColFrom = col1;
    step->ColTo = col;
    step->RowFrom = row1;
    step->RowTo = row;
    step->moveid = moveid;
    step->killid = killid;

    steps1.append(step);
}

void Board::reliveChessman(int id)
{
    if(id==-1) return;
    chessman[id].dead = false;
}

void Board::killChessman(int id)
{
    if(id==-1) return;
    chessman[id].dead = true;
}

void Board::moveChessman(int moveid, int row, int col)
{
    chessman[moveid].row = row;
    chessman[moveid].col = col;

    beRedTurn = !beRedTurn;
}

void Board::moveChessman(int moveid, int killid, int row1, int col1)
{
    saveStep(moveid, killid, row1, col1, steps);
    killChessman(killid);
    moveChessman(moveid, row1, col1);
}

void Board::back(Step *step)
{
    if(step->killid==-1) return;
    chessman[step->killid].dead = false;

    chessman[step->moveid].row = step->RowFrom;
    chessman[step->moveid].col = step->ColFrom;

    beRedTurn = !beRedTurn;
}

void Board::backOne()
{
    if(this->steps.size() == 0) return;

    Step* step = this->steps.last();
    steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
