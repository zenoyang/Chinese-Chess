#include "SingleGame.h"
#include <QTimer>
#include <CtrlPanel.h>
#include <QHBoxLayout>

SingleGame::SingleGame()
{
    _level = 4;

//    CtrlPanel* panel = new CtrlPanel;
//    QHBoxLayout* hLay = new QHBoxLayout(this);
//    hLay->addWidget(panel);
//    connect(panel, SIGNAL(sigBack()), this, SLOT(slotBack()));
}

SingleGame::~SingleGame()
{

}

void SingleGame::click(int id, int row, int col)
{
    if (!beRedTurn)
        return ;
    Board::click(id, row, col);

    if (!beRedTurn)
        //延迟0.1秒调用computerMove()
         QTimer::singleShot(100, this, SLOT(computerMove()));
        //computerMove();
}

void SingleGame::computerMove()
{
    /*
     * 1.看看哪些位置可以走
     * 2.尝试着走一次
     * 3.评估走的结果
     * 4.取最好的结果
    */
    Step * step = getBestMove();
    moveChessman(step->moveid, step->killid, step->RowTo, step->ColTo);
    delete step;
    update();
}

int SingleGame::getMaxScore(int level, int curMaxScore)
{
    if (level == 0)
        return calcScore();

    QVector<Step*> steps1;
    getAllPossibleMove(steps1);
     int maxInAllMinScore = -1000000;

     while(steps1.count())
     {
         Step * step = steps1.last();
         steps1.removeLast();
         fakeMove(step);
         int minScore = getMinScore(level - 1, maxInAllMinScore);
         unfakeMove(step);
         delete step;

         if(minScore >= curMaxScore)
         {
             while(steps1.count())
             {
                 Step* step = steps1.last();
                 steps1.removeLast();
                 delete step;
             }
             return minScore;
         }

         if(minScore > maxInAllMinScore)
         {
             maxInAllMinScore = minScore;
         }

     }
     return maxInAllMinScore;
}

int SingleGame::getMinScore(int level, int curMinScore)
{
    if (level == 0)
        return calcScore();

    QVector<Step*> steps1;
    getAllPossibleMove(steps1);
     int minInAllMaxScore = 1000000;

     while(steps1.count())
     //for (QVector<Step *>::iterator it = steps1.begin(); it < steps1.end(); ++it)
     {
         Step * step = steps1.last();
         steps1.removeLast();
         //Step * step = *it;
         fakeMove(step);
         int maxScore = getMaxScore(level - 1, minInAllMaxScore);
         unfakeMove(step);
         delete step;

         //剪枝
         if (maxScore <= curMinScore)     //这里的= 优化了很多
         {
             while(steps1.count())
             {
                 Step* step = steps1.last();
                 steps1.removeLast();
                 delete step;
             }
             return maxScore;
         }

         if(maxScore < minInAllMaxScore)
         {
             minInAllMaxScore = maxScore;
         }
     }
     return minInAllMaxScore;
}

Step * SingleGame::getBestMove()
{
     QVector<Step*> steps1;
     getAllPossibleMove(steps1);
     int maxInAllMinScore = -1000000;
     Step * ret = NULL;

     while(steps1.count())
//    for (QVector<Step *>::iterator it = steps1.begin(); it < steps1.end(); ++it)
    {
       // Step * step = *it;
         Step * step = steps1.last();
         steps1.removeLast();
        fakeMove(step);
        int minScore = getMinScore(_level-1, maxInAllMinScore);
        unfakeMove(step);

        if (minScore > maxInAllMinScore)
        {
            maxInAllMinScore = minScore;
            if (ret)
                delete ret;
            ret = step;
        }
        else
        {
            delete step;
        }
    }

    return ret;
}

//评价局面分
int SingleGame::calcScore()
{
    int redTotScore = 0;
    int blackTotScore = 0;
     //    enum TYPE{JIANG, CHE, PAO, MA, BING, SHI, XIANG};
    static int chessScore[] = {1000, 100, 80, 60, 15, 20, 20};
    //黑棋总分-红棋总分
    for (int i = 0; i < 16; ++i)
    {
        if (chessman[i].dead)
            continue;
        blackTotScore += chessScore[chessman[i].type];
    }

    for (int i = 16; i < 32; ++i)
    {
        if (chessman[i].dead)
            continue;
        redTotScore += chessScore[chessman[i].type];
    }

    return blackTotScore - redTotScore;
}

void SingleGame::fakeMove(Step *step)
{
    killChessman(step->killid);
    moveChessman(step->moveid, step->RowTo, step->ColTo);
}

void SingleGame::unfakeMove(Step *step)
{
    reliveChessman(step->killid);
    moveChessman(step->moveid, step->RowFrom, step->ColFrom);
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps1)
{
    int min, max;
    if (this->beRedTurn)
    {
        min = 16;
        max = 32;
    }
    else
    {
        min = 0;
        max = 16;
    }

    for (int i = min; i < max; ++i)
    {
        if (chessman[i].dead)
            continue;
        for (int row = 0; row <= 9; ++row)
        {
            for (int col = 0; col <= 8; ++col)
            {
                int killid = getChessmanid(row, col);
                if (chessman[i].red == chessman[killid].red)
                    continue;
                if (canMove(i, row, col, killid))
                    saveStep(i, killid, row, col, steps1);
            }
        }
    }
}

void SingleGame::back()
{
    if(beRedTurn)
    {
        backOne();
        backOne();
    }
}
