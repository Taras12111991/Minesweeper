#include "minebutton.h"

#include <QMouseEvent>

MineButton::MineButton(QWidget *parent) :
    QPushButton(parent), _bomb(false), _neighbourBombsCnt(0), buttonClicked(false)
{
    setText("");
    connect(this, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    connect(this, SIGNAL(clicked()), this, SLOT(slotSetIcon()));
}

void MineButton::setBomb()
{
    _bomb = true;
}

void MineButton::setNeighbourBombsCnt(int n)
{
    _neighbourBombsCnt = n;
}

void MineButton::setButtonCoordinats(int i, int j)
{
    _row = i;
    _col = j;
}

void MineButton::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() == (Qt::LeftButton | Qt::RightButton) && buttonClicked)
        emit signalDoubleClicked(_row, _col);
    else if (e->button() == Qt::RightButton)
    {
        if (icon().isNull() && !buttonClicked)
            setIcon(QPixmap(":/resources/images/Add.png"));
        else
            setIcon(QPixmap(""));
    }
    else if (e->button() == Qt::LeftButton)
        emit clicked();
}

void MineButton::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    if (buttonClicked)
        emit signalDoubleClicked(_row, _col);
}

void MineButton::slotSetIcon()
{
    if (_bomb)
        setIcon(QPixmap(":/resources/images/clanbomber_2326.png"));
}

void MineButton::slotButtonClicked()
{
    emit thisClicked(_row, _col);
    emit thisClicked(this);
    buttonClicked = true;
    if (_bomb)
    {
        emit signalBombClicked();
    }
    else
    {
        if (!_neighbourBombsCnt)
        {
            emit signalZero(_row, _col);
        }

            setText(QString().number(_neighbourBombsCnt));
            switch(_neighbourBombsCnt){
            case 0:
            {
                setStyleSheet(QString("color: gray"));
                break;
            }
            case 1:
            {
                setStyleSheet(QString("color: blue"));
                break;
            }
            case 2:
            {
                setStyleSheet(QString("color: green"));
                break;
            }
            case 3:
            {
                setStyleSheet(QString("color: red"));
                break;
            }
            case 4:
            {
                setStyleSheet(QString("color: black"));
                break;
            }
            case 5:
            {
                setStyleSheet(QString("color: violet"));
                break;
            }
            case 6:
            {
                setStyleSheet(QString("color: dark blue"));
                break;
            }
            case 7:
            {
                setStyleSheet(QString("color: purple"));
                break;
            }
            case 8:
            {
                setStyleSheet(QString("color: cyan"));
                break;
            }
            }
    }
}

bool MineButton::isBomb()
{
    return _bomb;
}

int MineButton::neighbourBombsCnt()
{
    return _neighbourBombsCnt;
}







