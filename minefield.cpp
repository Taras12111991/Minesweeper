#include "minefield.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QDialog>
#include <algorithm>
#include <random>
//#include <QGridLayout>

MineField::MineField(QWidget *parent) :
    QWidget(parent), _height(9), _width(9), _bombCnt(10), _pgl(new QGridLayout), _openButtonsCnt(0), _field(0)
{
    slotNewGame();
}

MineField::~MineField()
{
    for (int i = 0; i < _height; i++)
        for (int j = 0; j < _width; j++)
            {delete _field[i][j]; _field[i][j] = 0;}

    for (int i = 0; i < _height; i++)
    {
        delete[] _field[i];
    }
    delete[] _field;
    _field = 0;
}

void MineField::slotSettings()
{
    h = 9;
    w = 9;
    b = 10;
    QDialog* pdialog = new QDialog;
    pdialog->setWhatsThis("You can select game level");
    QRadioButton* prad1 = new QRadioButton;
    prad1->setChecked(true);
    QRadioButton* prad2 = new QRadioButton;
    QRadioButton* prad3 = new QRadioButton;

    QPushButton* pOk = new QPushButton("Ok");
    connect(pOk, SIGNAL(clicked()), pdialog, SLOT(close()));
    connect(pOk, SIGNAL(clicked()), this, SLOT(slotSettingsChanged()));

    connect(prad1, SIGNAL(clicked()), this, SLOT(slotSetBeginner()));
    connect(prad2, SIGNAL(clicked()), this, SLOT(slotSetMaster()));
    connect(prad3, SIGNAL(clicked()), this, SLOT(slotSetPro()));

    QVBoxLayout* pvbl = new QVBoxLayout;
    QHBoxLayout* phbl1 = new QHBoxLayout;
    QHBoxLayout* phbl2 = new QHBoxLayout;
    QHBoxLayout* phbl3 = new QHBoxLayout;

    QLabel* plbl1 = new QLabel("Beginner\n9*9\n10 mines");
    QLabel* plbl2 = new QLabel("Master\n16*16\n40 mines");
    QLabel* plbl3 = new QLabel("Pro\n16*30\n99 mines");

    phbl1->addWidget(prad1);
    phbl1->addWidget(plbl1);

    phbl2->addWidget(prad2);
    phbl2->addWidget(plbl2);

    phbl3->addWidget(prad3);
    phbl3->addWidget(plbl3);

    pvbl->addLayout(phbl1);
    pvbl->addLayout(phbl2);
    pvbl->addLayout(phbl3);
    pvbl->addWidget(pOk);

    pdialog->setLayout(pvbl);
    pdialog->setFixedSize(120, 180);
    pdialog->show();
}

void MineField::settings(int he, int wi, int bo)
{
    if (_field)
    {
        for (int i = 0; i < _height; i++)
        {
            for (int j = 0; j < _width; j++)
                if (_field[i][j])
                {delete _field[i][j]; _field[i][j] = 0;}
            delete[] _field[i];
        }
        delete[] _field;
        _field = 0;
    }

    _height = he;
    _width = wi;
    _bombCnt = bo;

    slotNewGame();
}

void MineField::slotSettingsChanged()
{
    settings(h, w, b);
}

void MineField::slotNewGame()
{
    _openButtonsCnt = 0;

    if (_field)
    {
        for (int i = 0; i < _height; i++)
        {
            for (int j = 0; j < _width; j++)
                if (_field[i][j])
                {delete _field[i][j]; _field[i][j] = 0;}
            delete[] _field[i];
        }
        delete[] _field;
        _field = 0;
    }

    _field = new MineButton**[_height];
        for (int i = 0; i < _height; i++)
        {
            _field[i] = new MineButton*[_width];
        }

        if (!_pgl)
            _pgl = new QGridLayout;

    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            _field[i][j] = new MineButton;
            _field[i][j]->setButtonCoordinats(i, j);
            _field[i][j]->setFixedSize(25, 25);
            connect(_field[i][j], SIGNAL(signalBombClicked()), this, SLOT(slotGameOver()));
            connect(_field[i][j], SIGNAL(thisClicked(MineButton*)), this, SLOT(slotVictory(MineButton*)));
            connect(_field[i][j], SIGNAL(signalZero(int, int)), this, SLOT(slotZeroClicked(int, int)));
            connect(_field[i][j], SIGNAL(signalDoubleClicked(int, int)), this, SLOT(slotDoubleClicked(int, int)));
            _pgl->addWidget(_field[i][j], i, j);
        }
    }

    setLayout(_pgl);
    setBombs();
    setNeighbourBombsCnt();
    setEnabled(true);
    setFixedSize(_width * _field[0][0]->width(),_height * _field[0][0]->height());
    emit signalSizeChanged(size());
}

void MineField::slotSetBeginner()
{
    h = 9;
    w = 9;
    b = 10;
}

void MineField::slotSetMaster()
{
    h = 16;
    w = 16;
    b = 40;
}

void MineField::slotSetPro()
{
    h = 16;
    w = 30;
    b = 99;
}

void MineField::slotGameOver()
{
    _openButtonsCnt = 0;
    showAllBombs();
    setDisabled(true);
    QLabel* plbl = new QLabel("Game Over!!");
    plbl->resize(120, 50);
    plbl->show();
}

void MineField::showAllBombs()
{
    for (int i = 0; i < _height; i++)
        for (int j = 0; j < _width; j++)
            _field[i][j]->slotSetIcon();
}

void MineField::setBombs()
{
    bool* arr = new bool[_height*_width];
    for (int i = 0; i < _bombCnt; i++)
    {
        arr[i] = false;
    }
    std::random_shuffle(arr, &arr[_height*_width - 1]);

    for (int i = 0; i < _height; i++)
        for (int j = 0; j < _width; j++)
            if (!arr[i*_height + j])
                _field[i][j]->setBomb();
}

void MineField::setNeighbourBombsCnt()
{
    int number = 0;
    int iMin;
    int iMax;
    int jMin;
    int jMax;
    for (int i = 0; i < _height;  i++)
        for (int j = 0; j < _width; j++)
        {
            if (!_field[i][j]->isBomb())
            {
                if (i == 0)
                {
                    iMin = 0;
                    iMax = 1;
                }
                else if (i == _height - 1)
                {
                    iMin = -1;
                    iMax = 0;
                }
                else
                {
                    iMin = -1;
                    iMax = 1;
                }

                if (j == 0)
                {
                    jMin = 0;
                    jMax = 1;
                }
                else if (j == _width - 1)
                {
                    jMin = -1;
                    jMax = 0;
                }
                else
                {
                    jMin = -1;
                    jMax = 1;
                }
                for (int n = iMin; n <= iMax; n++)
                    for (int m = jMin; m <= jMax; m++)
                    {
                        if (_field[i + n][j + m]->isBomb())
                            number++;
                    }
                _field[i][j]->setNeighbourBombsCnt(number);
                number = 0;
            }
        }
}

void MineField::slotVictory(MineButton* button)
{
    if (!button->buttonClicked && !button->isBomb())
        _openButtonsCnt++;
    if (_openButtonsCnt == _height * _width - _bombCnt)
    {
        setEnabled(false);
        _openButtonsCnt = 0;
        QLabel* plbl = new QLabel("Victory!!!");
        plbl->resize(120,50);
        plbl->show();
    }
}

void MineField::slotZeroClicked(int i, int j)
{
    recursZero(i, j);
}

void MineField::recursZero(int i, int j)
{
    int iMin;
    int iMax;
    int jMin;
    int jMax;
    if (i == 0)
    {
        iMin = 0;
        iMax = 1;
    }
    else if (i == _height - 1)
    {
        iMin = -1;
        iMax = 0;
    }
    else
    {
        iMin = -1;
        iMax = 1;
    }

    if (j == 0)
    {
        jMin = 0;
        jMax = 1;
    }
    else if (j == _width - 1)
    {
        jMin = -1;
        jMax = 0;
    }
    else
    {
        jMin = -1;
        jMax = 1;
    }

    for (int n = iMin; n <= iMax; n++)
        for (int m = jMin; m <= jMax; m++)
            if (!_field[i + n][j + m]->buttonClicked && !_field[i + n][j + m]->isBomb())
            {
                emit _field[i + n][j + m]->clicked();
                if (!_field[i + n][j + m]->neighbourBombsCnt())
                {
                    recursZero(i + n, j + m);
                }
            }
}

void MineField::slotDoubleClicked(int i, int j)
{
    if (_field[i][j]->buttonClicked)
    {
        int iMin;
        int iMax;
        int jMin;
        int jMax;
        if (i == 0)
        {
            iMin = 0;
            iMax = 1;
        }
        else if (i == _height - 1)
        {
            iMin = -1;
            iMax = 0;
        }
        else
        {
            iMin = -1;
            iMax = 1;
        }

        if (j == 0)
        {
            jMin = 0;
            jMax = 1;
        }
        else if (j == _width - 1)
        {
            jMin = -1;
            jMax = 0;
        }
        else
        {
            jMin = -1;
            jMax = 1;
        }

        for (int n = iMin; n <= iMax; n++)
            for (int m = jMin; m <= jMax; m++)
            {
                if (!_field[i + n][j + m]->buttonClicked && _field[i + n][j + m]->icon().isNull()
                        && !(n == 0 && m == 0))
                    emit _field[i + n][j + m]->clicked();
            }
    }
}



