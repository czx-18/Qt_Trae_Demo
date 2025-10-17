#ifndef BALLGAME_H
#define BALLGAME_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QPushButton>
#include <QLabel>
#include "ball.h"

class BallGame : public QWidget
{
    Q_OBJECT
public:
    explicit BallGame(QWidget *parent = nullptr);
    ~BallGame();

protected:
    // 重写绘制事件
    void paintEvent(QPaintEvent *event) override;
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override;
    // 窗口大小变化事件
    void resizeEvent(QResizeEvent *event) override;

private slots:
    // 游戏循环
    void gameLoop();
    // 开始游戏
    void startGame();
    // 重置游戏
    void resetGame();

private:
    // 初始化游戏
    void initGame();
    // 初始化UI
    void initUI();
    // 检查球与圆圈的碰撞
    void checkCircleCollision(Ball *ball);
    // 检查球与球的碰撞
    void checkBallCollision(Ball *ball1, Ball *ball2);
    // 检查球是否碰到线
    void checkLineCollision();
    // 检查游戏结束条件
    bool checkGameOver();
    // 更新游戏状态
    void updateGameState();
    // 绘制游戏状态
    void drawGameStatus(QPainter *painter);

private:
    QList<Ball*> m_balls;      // 所有球
    QTimer *m_timer;           // 游戏计时器
    bool m_isRunning;          // 游戏是否运行
    qreal m_circleRadius;      // 圆圈半径
    QPointF m_circleCenter;    // 圆圈中心
    qreal m_gameSpeed;         // 游戏速度
    QPushButton *m_startButton; // 开始按钮
    QLabel *m_statusLabel;     // 状态标签
    QList<QLabel*> m_scoreLabels; // 分数标签


};

#endif // BALLGAME_H
