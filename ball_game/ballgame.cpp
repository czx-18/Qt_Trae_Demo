#include "ballgame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QRandomGenerator>
#include <cmath>

BallGame::BallGame(QWidget *parent) : QWidget(parent)
    , m_isRunning(false)
    , m_circleRadius(200)
    , m_gameSpeed(100.0)
{
    // 设置窗口大小和标题
    setMinimumSize(500, 500);
    setWindowTitle(QStringLiteral("小球碰撞游戏"));
    
    // 初始化UI
    initUI();
    
    // 初始化游戏
    initGame();
    
    // 设置游戏循环计时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &BallGame::gameLoop);
    m_timer->setInterval(16); // 约60fps
}

BallGame::~BallGame()
{
    // 清理资源
    qDeleteAll(m_balls);
    delete m_timer;
}

// 修改initUI函数，优化布局
void BallGame::initUI()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建顶部控制区
    QHBoxLayout *controlLayout = new QHBoxLayout;
    
    // 创建开始按钮
    m_startButton = new QPushButton(QStringLiteral("开始游戏"), this);
    connect(m_startButton, &QPushButton::clicked, this, &BallGame::startGame);
    controlLayout->addWidget(m_startButton);
    
    // 创建重置按钮
    QPushButton *resetButton = new QPushButton(QStringLiteral("重置游戏"), this);
    connect(resetButton, &QPushButton::clicked, this, &BallGame::resetGame);
    controlLayout->addWidget(resetButton);
    
    // 创建状态标签
    m_statusLabel = new QLabel(QStringLiteral("准备开始"), this);
    controlLayout->addWidget(m_statusLabel);
    
    mainLayout->addLayout(controlLayout);
    
    // 创建分数显示区，使用网格布局更好地排列标签
    QGridLayout *scoreLayout = new QGridLayout;
    scoreLayout->setHorizontalSpacing(20); // 增加水平间距
    scoreLayout->setVerticalSpacing(5);    // 设置垂直间距
    
    // 创建三个分数标签，对应三个球
    for (int i = 0; i < 3; i++) {
        m_scoreLabels.append(new QLabel(this));
        scoreLayout->addWidget(m_scoreLabels.last(), 0, i);
    }
    
    // 添加分隔符
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    
    mainLayout->addLayout(scoreLayout);
    mainLayout->addWidget(separator); // 添加分隔线
    
    // 创建游戏区域容器
    QWidget *gameContainer = new QWidget(this);
    gameContainer->setMinimumHeight(400); // 设置最小高度
    gameContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // 创建游戏区域布局
    QVBoxLayout *gameAreaLayout = new QVBoxLayout(gameContainer);
    gameAreaLayout->setAlignment(Qt::AlignCenter); // 居中布局
    
    mainLayout->addWidget(gameContainer, 1); // 添加游戏容器，权重为1确保它占据剩余空间
    
    // 设置主布局边距
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
}

/**
 * @brief 初始化游戏状态
 * 
 * 重置游戏区域，创建新球并设置初始位置和速度
 * 修复了重置游戏后圆圈大小变化的问题，确保圆圈大小基于游戏区域而非整个窗口
 */
void BallGame::initGame()
{
    // 清理现有球
    qDeleteAll(m_balls);
    m_balls.clear();
    
    // 计算游戏区域的实际可用空间
    // 获取当前所有控件占用的总高度
    int totalControlsHeight = 120; // 固定值，包含按钮、标签、分隔线和间距
    
    // 计算游戏区域的矩形
    QRect gameRect = rect();
    gameRect.setTop(totalControlsHeight);
    gameRect.setBottom(rect().bottom() - 10);
    gameRect.setLeft(10);
    gameRect.setRight(rect().right() - 10);
    
    // 设置圆圈中心和半径，基于游戏区域而非整个窗口
    m_circleCenter = QPointF(gameRect.center());
    m_circleRadius = qMin(gameRect.width(), gameRect.height()) * 0.4;
    
    // 创建三个不同颜色的球
    QList<QColor> colors = {Qt::red, Qt::blue, Qt::green};
    
    for (int i = 0; i < 3; i++) {
        Ball *ball = new Ball(this);
        ball->setRadius(15.0);
        ball->setColor(colors[i]);
        ball->setId(i);
        
        // 随机设置初始位置（在圆圈内但不靠近边缘）
        qreal angle = QRandomGenerator::global()->generateDouble() * 2 * 3.1415;
        qreal distance = m_circleRadius * 0.3 + QRandomGenerator::global()->generateDouble() * m_circleRadius * 0.5;
        qreal x = m_circleCenter.x() + distance * cos(angle);
        qreal y = m_circleCenter.y() + distance * sin(angle);
        ball->setPosition(QPointF(x, y));
        
        // 随机设置初始速度
        qreal speed = 50.0 + QRandomGenerator::global()->generateDouble() * 100.0;
        qreal velocityAngle = QRandomGenerator::global()->generateDouble() * 2 * 3.1415;
        ball->setVelocity(QPointF(speed * cos(velocityAngle), speed * sin(velocityAngle)));
        
        m_balls.append(ball);
    }
    
    // 更新分数显示
    updateGameState();
    
    // 重绘
    update();
}

// 修改paintEvent函数
// void BallGame::paintEvent(QPaintEvent *event)
// {
//     Q_UNUSED(event);
    
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing);
    
//     // 绘制背景
//     painter.fillRect(rect(), QColor(240, 240, 240));
    
//     // 获取游戏区域的实际可用空间（减去控件占用的空间）
//     // 计算游戏区域的矩形（不包括按钮和标签区域）
//     QRect gameRect = rect();
    
//     // 粗略估计控件占用的高度（按钮约30px，标签约20px，分隔线约5px，边距约20px）
//     int controlsHeight = 100; // 设置一个合理的值
    
//     // 调整游戏区域
//     gameRect.setTop(controlsHeight);
//     gameRect.setBottom(rect().bottom() - 10);
//     gameRect.setLeft(10);
//     gameRect.setRight(rect().right() - 10);
    
//     // 根据实际游戏区域重新计算圆圈中心和半径
//     m_circleCenter = QPointF(gameRect.center());
//     m_circleRadius = qMin(gameRect.width(), gameRect.height()) * 0.4;
    
//     // 绘制圆圈
//     painter.setPen(QPen(Qt::black, 2));
//     painter.setBrush(Qt::NoBrush);
//     painter.drawEllipse(m_circleCenter, m_circleRadius, m_circleRadius);
    
//     // 绘制所有连接线
//     for (Ball *ball : qAsConst(m_balls)) {
//         ball->drawConnections(&painter);
//     }
    
//     // 绘制所有球
//     for (Ball *ball : qAsConst(m_balls)) {
//         ball->draw(&painter);
//     }
    
//     // 绘制游戏状态
//     drawGameStatus(&painter);
// }

void BallGame::mousePressEvent(QMouseEvent *event)
{
    // 可以添加鼠标交互逻辑
    Q_UNUSED(event);
}

// 修改resizeEvent函数，确保窗口大小变化时正确更新游戏状态
// 完全重写resizeEvent函数，改进窗口缩放逻辑
// 修复1：移除无效的isNull()检查
void BallGame::resizeEvent(QResizeEvent *event)
{
    // 计算游戏区域的实际可用空间
    // 获取当前所有控件占用的总高度
    int totalControlsHeight = 120; // 固定值，包含按钮、标签、分隔线和间距
    
    // 计算游戏区域的矩形
    QRect gameRect = rect();
    gameRect.setTop(totalControlsHeight);
    gameRect.setBottom(rect().bottom() - 10);
    gameRect.setLeft(10);
    gameRect.setRight(rect().right() - 10);
    
    // 保存旧的中心和半径用于计算缩放因子
    QPointF oldCenter = m_circleCenter;
    qreal oldRadius = m_circleRadius;
    
    // 设置新的中心和半径
    m_circleCenter = QPointF(gameRect.center());
    m_circleRadius = qMin(gameRect.width(), gameRect.height()) * 0.4;
    
    // 如果是第一次调整或者半径为0，不进行缩放计算
    if (oldRadius <= 0) {
        QWidget::resizeEvent(event);
        update();
        return;
    }
    
    // 计算缩放因子和位移
    qreal scaleFactor = m_circleRadius / oldRadius;
    QPointF translate = m_circleCenter - oldCenter;
    
    // 调整所有球的位置和连接线
    for (Ball *ball : qAsConst(m_balls)) {
        if (!ball->isEliminated()) {
            // 调整球的位置
            QPointF relativePos = ball->position() - oldCenter;
            QPointF newPos = m_circleCenter + relativePos * scaleFactor;
            ball->setPosition(newPos);
            
            // 备份连接线
            QList<QPointF> oldConnections = ball->connections();
            
            // 清除旧连接线
            while (ball->connectionCount() > 0) {
                ball->removeConnection(0);
            }
            
            // 重新添加调整后的连接线
            for (const QPointF &oldConn : oldConnections) {
                // 计算相对于旧中心的位置并进行缩放和平移
                QPointF relativeConn = oldConn - oldCenter;
                QPointF newConn = m_circleCenter + relativeConn * scaleFactor;
                // 重新添加连接
                ball->addConnection(newConn);
            }
        }
    }
    
    QWidget::resizeEvent(event);
    update();
}

// 修改paintEvent函数，移除其中对圆圈中心和半径的重新计算，避免冲突
// 修复2：删除重复的paintEvent函数
// 保留第二个更优化的paintEvent实现（第266行开始的版本），并删除第一个实现（第140行开始的版本）

// 删除以下整个函数块（大约在第140-190行）
/*
void BallGame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    // 获取游戏区域的实际可用空间（减去控件占用的空间）
    // 计算游戏区域的矩形（不包括按钮和标签区域）
    QRect gameRect = rect();
    
    // 粗略估计控件占用的高度（按钮约30px，标签约20px，分隔线约5px，边距约20px）
    int controlsHeight = 100; // 设置一个合理的值
    
    // 调整游戏区域
    gameRect.setTop(controlsHeight);
    gameRect.setBottom(rect().bottom() - 10);
    gameRect.setLeft(10);
    gameRect.setRight(rect().right() - 10);
    
    // 根据实际游戏区域重新计算圆圈中心和半径
    m_circleCenter = QPointF(gameRect.center());
    m_circleRadius = qMin(gameRect.width(), gameRect.height()) * 0.4;
    
    // 绘制圆圈
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(m_circleCenter, m_circleRadius, m_circleRadius);
    
    // 绘制所有连接线
    for (Ball *ball : qAsConst(m_balls)) {
        ball->drawConnections(&painter);
    }
    
    // 绘制所有球
    for (Ball *ball : qAsConst(m_balls)) {
        ball->draw(&painter);
    }
    
    // 绘制游戏状态
    drawGameStatus(&painter);
}
*/

// // 保留第二个更优化的paintEvent函数实现（第266行开始的版本）
void BallGame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    // 绘制圆圈 - 使用resizeEvent中计算好的中心和半径
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(m_circleCenter, m_circleRadius, m_circleRadius);
    
    // 绘制所有连接线
    for (Ball *ball : qAsConst(m_balls)) {
        ball->drawConnections(&painter);
    }
    
    // 绘制所有球
    for (Ball *ball : qAsConst(m_balls)) {
        ball->draw(&painter);
    }
    
    // 绘制游戏状态
    drawGameStatus(&painter);
}

void BallGame::gameLoop()
{
    if (!m_isRunning) {
        return;
    }
    
    // 更新所有球的位置
    for (Ball *ball : qAsConst(m_balls)) {
        if (!ball->isEliminated()) {
            ball->updatePosition(0.016); // 16ms
        }
    }
    
    // 检查球与圆圈的碰撞
    for (Ball *ball : qAsConst(m_balls)) {
        if (!ball->isEliminated()) {
            checkCircleCollision(ball);
        }
    }
    
    // 检查球与球的碰撞
    for (int i = 0; i < m_balls.size(); i++) {
        if (m_balls[i]->isEliminated()) {
            continue;
        }
        
        for (int j = i + 1; j < m_balls.size(); j++) {
            if (!m_balls[j]->isEliminated()) {
                checkBallCollision(m_balls[i], m_balls[j]);
            }
        }
    }
    
    // 检查球是否碰到线
    checkLineCollision();
    
    // 更新游戏状态
    updateGameState();
    
    // 检查游戏结束条件
    if (checkGameOver()) {
        m_isRunning = false;
        m_timer->stop();
        m_startButton->setText(QStringLiteral("开始游戏"));
    }
    
    // 重绘
    update();
}

void BallGame::startGame()
{
    if (m_isRunning) {
        // 暂停游戏
        m_isRunning = false;
        m_timer->stop();
        m_startButton->setText(QStringLiteral("继续游戏"));
        m_statusLabel->setText(QStringLiteral("游戏暂停"));
    } else {
        // 开始或继续游戏
        m_isRunning = true;
        m_timer->start();
        m_startButton->setText(QStringLiteral("暂停游戏"));
        m_statusLabel->setText(QStringLiteral("游戏进行中"));
    }
}

void BallGame::resetGame()
{
    // 停止游戏
    m_isRunning = false;
    m_timer->stop();
    m_startButton->setText(QStringLiteral("开始游戏"));
    m_statusLabel->setText(QStringLiteral("准备开始"));
    
    // 重新初始化游戏
    initGame();
}

// 修改checkCircleCollision函数，添加边界检查
void BallGame::checkCircleCollision(Ball *ball)
{
    QPointF pos = ball->position();
    QPointF centerToBall = pos - m_circleCenter;
    qreal distance = sqrt(centerToBall.x() * centerToBall.x() + centerToBall.y() * centerToBall.y());
    
    // 检查是否与圆圈碰撞（考虑球的半径）
    if (distance + ball->radius() >= m_circleRadius) {
        // 计算碰撞点
        QPointF collisionPoint = m_circleCenter + centerToBall * (m_circleRadius / distance);
        
        // 添加连接线
        ball->addConnection(collisionPoint);
        
        // 计算反弹后的速度
        // 法向量（指向圆心）
        QPointF normal = centerToBall / distance;
        
        // 速度在法向量方向的分量
        QPointF velocity = ball->velocity();
        qreal dotProduct = velocity.x() * normal.x() + velocity.y() * normal.y();
        
        // 反弹后的速度（保留切线方向的分量，反转法向量方向的分量）
        QPointF newVelocity = velocity - 2 * dotProduct * normal;
        
        // 设置新速度 - 移除速度衰减因子0.95，保持速度不变
        ball->setVelocity(newVelocity);  // 移除 * 0.95
        
        // 调整球的位置，防止卡在圆圈外
        QPointF newPosition = m_circleCenter + normal * (m_circleRadius - ball->radius());
        ball->setPosition(newPosition);
    }
}

// 修改checkBallCollision函数，移除速度衰减
void BallGame::checkBallCollision(Ball *ball1, Ball *ball2)
{
    QPointF pos1 = ball1->position();
    QPointF pos2 = ball2->position();
    QPointF delta = pos2 - pos1;
    qreal distance = sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    
    // 检查是否碰撞
    if (distance <= ball1->radius() + ball2->radius()) {
        // 计算碰撞后的速度（弹性碰撞）
        QPointF v1 = ball1->velocity();
        QPointF v2 = ball2->velocity();
        
        // 法向量
        QPointF normal = delta / distance;
        
        // 速度在法向量方向的分量
        qreal v1n = v1.x() * normal.x() + v1.y() * normal.y();
        qreal v2n = v2.x() * normal.x() + v2.y() * normal.y();
        
        // 交换法向量方向的速度分量（假设质量相同）
        QPointF v1n_new = normal * v2n;
        QPointF v2n_new = normal * v1n;
        
        // 计算切线方向的速度分量
        QPointF tangent(-normal.y(), normal.x());
        qreal v1t = v1.x() * tangent.x() + v1.y() * tangent.y();
        qreal v2t = v2.x() * tangent.x() + v2.y() * tangent.y();
        QPointF v1t_new = tangent * v1t;
        QPointF v2t_new = tangent * v2t;
        
        // 合成新速度 - 移除速度衰减因子0.9，保持速度不变
        QPointF newV1 = v1n_new + v1t_new;
        QPointF newV2 = v2n_new + v2t_new;
        
        // 设置新速度
        ball1->setVelocity(newV1);  // 移除 * 0.9
        ball2->setVelocity(newV2);  // 移除 * 0.9
        
        // 调整位置，防止球重叠
        qreal overlap = (ball1->radius() + ball2->radius() - distance) / 2.0;
        ball1->setPosition(pos1 - normal * overlap);
        ball2->setPosition(pos2 + normal * overlap);
    }
}

void BallGame::checkLineCollision()
{
    // 检查每个球是否碰到其他球的连接线
    for (Ball *ball : qAsConst(m_balls)) {
        if (ball->isEliminated()) {
            continue;
        }
        
        QPointF ballPos = ball->position();
        qreal ballRadius = ball->radius();
        
        // 检查其他球的每条连接线
        for (Ball *otherBall : qAsConst(m_balls)) {
            if (otherBall == ball || otherBall->isEliminated()) {
                continue;
            }
            
            // 检查每条连接线
            QList<QPointF> connections = otherBall->connections();
            for (int i = 0; i < connections.size(); i++) {
                QPointF circlePoint = connections[i];
                QPointF lineStart = otherBall->position();
                QPointF lineEnd = circlePoint;
                
                // 计算球到线段的最短距离
                QPointF lineVector = lineEnd - lineStart;
                QPointF pointVector = ballPos - lineStart;
                
                qreal t = qMax(0.0, qMin(1.0, 
                    (pointVector.x() * lineVector.x() + pointVector.y() * lineVector.y()) / 
                    (lineVector.x() * lineVector.x() + lineVector.y() * lineVector.y())));
                
                QPointF closestPoint = lineStart + lineVector * t;
                QPointF distanceVector = ballPos - closestPoint;
                qreal distance = sqrt(distanceVector.x() * distanceVector.x() + distanceVector.y() * distanceVector.y());
                
                // 如果球碰到了线
                if (distance <= ballRadius) {
                    // 移除原球的连接线
                    otherBall->removeConnection(i);
                    
                    // 在新球上添加相同的连接线（圆上的点不变，球端变为新球）
                    ball->addConnection(circlePoint);
                    
                    // 跳出循环，因为连接线已经被移除
                    break;
                }
            }
        }
    }
}

bool BallGame::checkGameOver()
{
    int activeBalls = 0;
    
    for (Ball *ball : qAsConst(m_balls)) {
        if (!ball->isEliminated()) {
            activeBalls++;
            
            // 检查是否有球达到100条连接线
            if (ball->connectionCount() >= 100) {
                m_statusLabel->setText(QStringLiteral("游戏结束！球%1获胜（达到100条线）").arg(ball->id() + 1));
                return true;
            }
        }
    }
    
    // 检查是否只剩一个球
    if (activeBalls <= 1) {
        for (Ball *ball : qAsConst(m_balls)) {
            if (!ball->isEliminated()) {
                m_statusLabel->setText(QStringLiteral("游戏结束！球%1获胜（最后存活）").arg(ball->id() + 1));
                break;
            }
        }
        return true;
    }
    
    return false;
}

void BallGame::updateGameState()
{
    // 更新分数显示
    for (int i = 0; i < m_scoreLabels.size() && i < m_balls.size(); i++) {
        Ball *ball = m_balls[i];
        QString status = ball->isEliminated() ? QStringLiteral("已淘汰") : QStringLiteral("连接线: %1").arg(ball->connectionCount());
        m_scoreLabels[i]->setText(QStringLiteral("球%1 (%2): %3").arg(i + 1).arg(ball->color().name()).arg(status));
    }
}

void BallGame::drawGameStatus(QPainter *painter)
{
    // 可以在这里添加额外的游戏状态绘制
    Q_UNUSED(painter);
}
