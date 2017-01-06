#ifndef SingleGame_H
#define SingleGame_H

#include "Board.h"
#include <QVector>
#include "Step.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    int _level;
    SingleGame();
    ~SingleGame();

    virtual void click(int id, int row, int col);

    //void computerMove();

    void getAllPossibleMove(QVector<Step *> &steps);
    Step * getBestMove();

    void fakeMove(Step * step);
    void unfakeMove(Step * step);
    int calcScore();
    int getMinScore(int level, int curMinScore);
    int getMaxScore(int level, int curMaxScore);

    void back();

public slots:   //用到槽时需要这个，并且上面加一个Q_OBJECT
    void computerMove();
};

#endif // SingleGame_H
