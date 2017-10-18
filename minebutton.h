#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>

class MineButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MineButton(QWidget *parent = 0);
    void setBomb();
    void setNeighbourBombsCnt(int);
    bool isBomb();
    int neighbourBombsCnt();
    void setButtonCoordinats(int i, int j);
    bool buttonClicked;
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
private:
    bool _bomb;
    int _neighbourBombsCnt;
    int _row, _col;
public slots:
    void slotSetIcon();
    void slotButtonClicked();
signals:
    void signalBombClicked();
    void signalZero(int, int);
    void signalDoubleClicked(int, int);
    void thisClicked(int, int);
    void thisClicked(MineButton*);
};

#endif // MINEBUTTON_H
