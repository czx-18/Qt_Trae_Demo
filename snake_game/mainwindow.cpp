#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // 创建GameBoard对象并设置为主窗口的中央部件
    m_gameBoard = new GameBoard(this);
    setCentralWidget(m_gameBoard);
    
    // 设置窗口标题
    setWindowTitle(QStringLiteral("贪吃蛇游戏"));
}

MainWindow::~MainWindow()
{
    delete ui;
    // GameBoard对象会在MainWindow销毁时自动被销毁，因为设置了this作为其父对象
}
