#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QWidget>
#include <QGridLayout>
#include "minebutton.h"

class MineField : public QWidget
{
    Q_OBJECT
public:
    explicit MineField(QWidget *parent = 0);
    ~MineField();
signals:
    void signalSizeChanged(QSize);
public slots:
    void slotNewGame();
    void slotSettings();
    void slotSetBeginner();
    void slotSetMaster();
    void slotSetPro();
    void slotGameOver();
    void slotVictory(MineButton *);
    void slotZeroClicked(int i, int j);
    void slotDoubleClicked(int i, int j);
    void slotSettingsChanged();
private:
    MineButton*** _field;
    int _height, h;
    int _width, w;
    int _bombCnt, b;
    QGridLayout* _pgl;
    void showAllBombs();
    void setBombs();
    void setNeighbourBombsCnt();
    int _openButtonsCnt;
    void recursZero(int, int);
    void settings(int, int, int);
};

#endif // MINEFIELD_H
