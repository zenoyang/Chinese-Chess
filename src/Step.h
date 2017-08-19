#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    int moveid;
    int killid;
    int RowFrom;
    int ColFrom;
    int RowTo;
    int ColTo;

signals:

public slots:
};

#endif // STEP_H
