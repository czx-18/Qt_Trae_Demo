/**
 * @file gameboard.cpp
 * @brief 游戏主界面实现文件
 * 
 * 该文件实现了GameBoard类的所有方法，包括游戏循环、绘制、键盘事件处理等功能。
 * GameBoard类负责贪吃蛇游戏的主界面渲染、游戏逻辑控制和用户交互处理。
 */
#include "gameboard.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QApplication>

/**
 * @brief GameBoard类构造函数
 * @param parent 父窗口部件指针
 * 
 * 初始化游戏界面和相关参数，设置窗口属性，连接计时器信号和游戏循环槽。
 */
GameBoard::GameBoard(QWidget *parent) : QWidget(parent)
{
    // 设置窗口属性
    setFocusPolicy(Qt::StrongFocus);  // 设置焦点策略为强焦点，确保能接收键盘事件
    setBackgroundRole(QPalette::Base);  // 设置背景角色
    setAutoFillBackground(true);  // 启用自动填充背景
    
    // 初始化游戏参数
    m_fieldWidth = 30;  // 游戏区域宽度（方块数量）
    m_fieldHeight = 20;  // 游戏区域高度（方块数量）
    m_speed = 200;  // 默认速度200ms
    m_score = 0;  // 初始分数为0
    m_isGameRunning = false;  // 游戏初始状态为未运行
    m_gameOver = false;  // 游戏初始状态为未结束
    
    // 连接计时器信号和游戏循环槽
    connect(&m_gameTimer, &QTimer::timeout, this, &GameBoard::gameLoop);
    
    // 重置游戏状态
    resetGame();
}

/**
 * @brief 开始游戏
 * 
 * 如果游戏已结束，则先重置游戏；然后设置游戏为运行状态，并启动游戏计时器。
 */
void GameBoard::startGame()
{
    if (m_gameOver) {
        resetGame();
    }
    
    m_isGameRunning = true;
    m_gameTimer.start(m_speed);
}

/**
 * @brief 暂停游戏
 * 
 * 设置游戏为暂停状态，停止游戏计时器，并触发重绘以显示暂停状态。
 */
void GameBoard::pauseGame()
{
    m_isGameRunning = false;
    m_gameTimer.stop();
    update();  // 触发重绘以显示暂停状态
}

/**
 * @brief 重置游戏
 * 
 * 重置蛇的状态，分数归零，重新生成食物，并触发重绘。
 */
void GameBoard::resetGame()
{
    m_snake.reset();  // 重置蛇的状态
    m_score = 0;  // 重置分数为0
    m_gameOver = false;  // 重置游戏结束标志
    m_isGameRunning = false;  // 重置游戏运行标志
    generateFood();  // 生成新食物
    update();  // 触发重绘
}

/**
 * @brief 获取当前分数
 * @return 当前游戏分数
 */
int GameBoard::getScore() const
{
    return m_score;
}

/**
 * @brief 重写绘制事件处理函数
 * @param event 绘制事件对象指针
 * 
 * 负责绘制游戏界面的所有元素，包括蛇、食物、游戏状态和分数。
 */
void GameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // 未使用的参数
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿渲染
    
    // 计算方块尺寸
    int squareSize = getSquareSize();
    
    // 绘制蛇
    QVector<QPoint> body = m_snake.getBody();
    for (int i = 0; i < body.size(); ++i) {
        QPoint pos = gameToWindow(body.at(i));  // 将游戏坐标转换为窗口坐标
        
        if (i == 0) {  // 蛇头
            painter.fillRect(pos.x(), pos.y(), squareSize, squareSize, Qt::red);  // 蛇头用红色
        } else {  // 蛇身
            painter.fillRect(pos.x(), pos.y(), squareSize, squareSize, Qt::green);  // 蛇身用绿色
        }
        
        // 绘制边框
        painter.setPen(Qt::black);
        painter.drawRect(pos.x(), pos.y(), squareSize, squareSize);
    }
    
    // 绘制食物
    QPoint foodPos = gameToWindow(m_food);  // 将食物的游戏坐标转换为窗口坐标
    painter.fillRect(foodPos.x(), foodPos.y(), squareSize, squareSize, Qt::blue);  // 食物用蓝色
    painter.drawRect(foodPos.x(), foodPos.y(), squareSize, squareSize);  // 绘制食物边框
    
    // 绘制游戏状态文本
    // 在paintEvent函数中修改以下几处
    
    // 修改1：游戏未开始状态
    if (!m_isGameRunning && !m_gameOver) {
        painter.setPen(Qt::black);
        QFont font;
        font.setFamily(QStringLiteral("SimHei"));
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("按空格键开始游戏"));
    }
    // 修改2：游戏暂停状态
    else if (!m_isGameRunning && m_gameOver == false) {
        painter.setPen(Qt::black);
        QFont font;
        font.setFamily(QStringLiteral("SimHei"));
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("游戏暂停\n按空格键继续"));
    }
    // 修改3：游戏结束状态
    else if (m_gameOver) {
        painter.setPen(Qt::black);
        QFont font;
        font.setFamily(QStringLiteral("SimHei"));
        font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(rect(), Qt::AlignCenter,
                         QStringLiteral("游戏结束\n分数: %1\n按空格键重新开始").arg(m_score));
    }
    
    // 修改4：分数显示
    painter.setPen(Qt::black);
    QFont font;
    font.setFamily(QStringLiteral("SimHei"));
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(10, 20, QStringLiteral("分数: %1").arg(m_score));
}

/**
 * @brief 重写键盘事件处理函数
 * @param event 键盘事件对象指针
 * 
 * 处理用户的键盘输入，包括方向键控制蛇的移动方向、空格键控制游戏开始/暂停/重启，以及ESC键退出游戏。
 */
void GameBoard::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;  // 忽略自动重复的按键事件
    }
    
    switch (event->key()) {
    case Qt::Key_Up:
        m_snake.setDirection(Up);  // 设置蛇向上移动
        break;
    case Qt::Key_Down:
        m_snake.setDirection(Down);  // 设置蛇向下移动
        break;
    case Qt::Key_Left:
        m_snake.setDirection(Left);  // 设置蛇向左移动
        break;
    case Qt::Key_Right:
        m_snake.setDirection(Right);  // 设置蛇向右移动
        break;
    case Qt::Key_Space:
        if (m_gameOver) {
            resetGame();  // 游戏结束时，重新开始游戏
            startGame();
        } else if (m_isGameRunning) {
            pauseGame();  // 游戏运行时，暂停游戏
        } else {
            startGame();  // 游戏暂停时，继续游戏
        }
        break;
    case Qt::Key_Escape:
        qApp->quit();  // 退出应用程序
        break;
    }
}

/**
 * @brief 重写窗口大小调整事件处理函数
 * @param event 窗口大小调整事件对象指针
 * 
 * 在窗口大小调整时，调用基类的resizeEvent方法，并触发重绘。
 */
void GameBoard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);  // 调用基类的resizeEvent
    update();  // 调整窗口大小时重新绘制
}

/**
 * @brief 游戏主循环槽函数
 * 
 * 定时执行的游戏逻辑，包括移动蛇、检查碰撞、处理食物吃取、更新分数和游戏速度，以及触发重绘。
 */
void GameBoard::gameLoop()
{
    // 移动蛇
    m_snake.move();
    
    // 检查碰撞
    if (checkWallCollision() || m_snake.checkSelfCollision()) {
        m_gameOver = true;  // 设置游戏结束标志
        pauseGame();  // 暂停游戏
    }
    
    // 检查是否吃到食物
    if (checkFoodCollision()) {
        m_snake.grow();  // 蛇增长一节
        m_score += 10;  // 分数增加10分
        generateFood();  // 生成新食物
        
        // 随着分数增加，游戏速度加快
        if (m_score % 50 == 0 && m_speed > 50) {
            m_speed -= 10;  // 速度增加（间隔减少）
            if (m_isGameRunning) {
                m_gameTimer.setInterval(m_speed);  // 更新计时器间隔
            }
        }
    }
    
    // 触发重绘
    update();
}

/**
 * @brief 生成食物
 * 
 * 随机生成食物位置，确保食物不会出现在蛇身上。
 */
void GameBoard::generateFood()
{
    QVector<QPoint> body = m_snake.getBody();
    
    // 随机生成食物位置，确保不会出现在蛇身上
    do {
        int x = QRandomGenerator::global()->bounded(m_fieldWidth);  // 随机生成X坐标
        int y = QRandomGenerator::global()->bounded(m_fieldHeight);  // 随机生成Y坐标
        m_food = QPoint(x, y);
    } while (body.contains(m_food));  // 确保食物不在蛇身上
}

/**
 * @brief 检查是否吃到食物
 * @return 如果蛇头碰到食物返回true，否则返回false
 */
bool GameBoard::checkFoodCollision()
{
    return m_snake.getHeadPosition() == m_food;  // 检查蛇头位置是否与食物位置相同
}

/**
 * @brief 检查是否撞到墙壁
 * @return 如果蛇头超出游戏区域边界返回true，否则返回false
 */
bool GameBoard::checkWallCollision()
{
    QPoint head = m_snake.getHeadPosition();  // 获取蛇头位置
    
    // 检查是否超出游戏区域边界
    return head.x() < 0 || head.x() >= m_fieldWidth ||
           head.y() < 0 || head.y() >= m_fieldHeight;
}

/**
 * @brief 计算适合窗口尺寸的最大方块大小
 * @return 方块的像素尺寸
 */
int GameBoard::getSquareSize() const
{
    // 计算适合窗口尺寸的最大方块大小
    int width = this->width() / m_fieldWidth;  // 基于窗口宽度计算的方块大小
    int height = this->height() / m_fieldHeight;  // 基于窗口高度计算的方块大小
    return qMin(width, height);  // 返回两者中的较小值，确保所有方块都能显示在窗口内
}

/**
 * @brief 将游戏坐标系转换为窗口坐标系
 * @param gamePos 游戏坐标系中的位置
 * @return 窗口坐标系中的位置
 */
QPoint GameBoard::gameToWindow(const QPoint &gamePos) const
{
    // 将游戏坐标系转换为窗口坐标系
    int squareSize = getSquareSize();
    // 计算偏移量，使游戏区域居中显示
    int offsetX = (this->width() - m_fieldWidth * squareSize) / 2;
    int offsetY = (this->height() - m_fieldHeight * squareSize) / 2;
    
    // 转换坐标并加上偏移量
    return QPoint(gamePos.x() * squareSize + offsetX, gamePos.y() * squareSize + offsetY);
}

/**
 * @brief 将窗口坐标系转换为游戏坐标系
 * @param windowPos 窗口坐标系中的位置
 * @return 游戏坐标系中的位置
 */
QPoint GameBoard::windowToGame(const QPoint &windowPos) const
{
    // 将窗口坐标系转换为游戏坐标系
    int squareSize = getSquareSize();
    // 计算偏移量，使游戏区域居中显示
    int offsetX = (this->width() - m_fieldWidth * squareSize) / 2;
    int offsetY = (this->height() - m_fieldHeight * squareSize) / 2;
    
    // 减去偏移量并转换为游戏坐标
    int x = (windowPos.x() - offsetX) / squareSize;
    int y = (windowPos.y() - offsetY) / squareSize;
    
    return QPoint(x, y);
}
