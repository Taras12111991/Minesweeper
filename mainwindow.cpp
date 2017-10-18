#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "minefield.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MineField* pmf = new MineField;
    setCentralWidget(pmf);
    resize(pmf->size());
    connect(pmf, SIGNAL(signalSizeChanged(QSize)), this, SLOT(slotResize(QSize)));

    QMenu* pGameMenu = new QMenu("Game");
    pGameMenu->addAction("New Game", pmf, SLOT(slotNewGame()));
    pGameMenu->addAction("Settings", pmf, SLOT(slotSettings()));
    menuBar()->addMenu(pGameMenu);
    menuBar()->addAction("Help");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotResize(QSize size)
{
    resize(size);
}
