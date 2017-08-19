#ifndef     Chessman_H
#define Chessman_H

#include <QString>

class Chessman
{
public:
    Chessman(){}
    ~Chessman(){}

    enum TYPE{JIANG, CHE, PAO, MA, BING, SHI, XIANG};

    int row;
    int col;
    TYPE type;


    int id;
    bool dead;
    bool red;


    void init(int id)
    {
        struct
        {
                int row, col;
                Chessman::TYPE type;
        } pos[16] = {
                {0, 0, Chessman::CHE},
                {0, 1, Chessman::MA},
                {0, 2, Chessman::XIANG},
                {0, 3, Chessman::SHI},
                {0, 4, Chessman::JIANG},
                {0, 5, Chessman::SHI},
                {0, 6, Chessman::XIANG},
                {0, 7, Chessman::MA},
                {0, 8, Chessman::CHE},

                {2, 1, Chessman::PAO},
                {2, 7, Chessman::PAO},
                {3, 0, Chessman::BING},
                {3, 2, Chessman::BING},
                {3, 4, Chessman::BING},
                {3, 6, Chessman::BING},
                {3, 8, Chessman::BING},
            };

        this->id = id;
        this->dead = false;
        this->red = id >= 16;

        if(id < 16)
        {
            row = pos[id].row;
            col = pos[id].col;
            type = pos[id].type;
        }
        else
        {
            row = 9-pos[id-16].row;
            col = 8-pos[id-16].col;
            type = pos[id-16].type;
        }
    }

    void rotate()
    {
        col = 8 - col;
        row = 9 - row;
    }

    QString getText()
    {
        switch(this->type)
            {
            case CHE:
                return "车";
            case MA:
                return "马";
            case PAO:
                return "炮";
            case BING:
                return "兵";
            case JIANG:
                return "将";
            case SHI:
                return "士";
            case XIANG:
                return "相";
            }
            return "错误";
    }
};

#endif // Chessman_H
