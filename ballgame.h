#ifndef BALLGAME_H
#define BALLGAME_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QPushButton>
#include <QLabel>
#include "ball.h"

/**
 * @brief 小球碰撞游戏主类
 * 
 * 负责管理整个游戏的逻辑、UI界面、碰撞检测和游戏循环
 */
class BallGame : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件，默认为nullptr
     */
    explicit BallGame(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~BallGame();

protected:
    /**
     * @brief 重写绘制事件
     * @param event 绘制事件对象
     */
    void paintEvent(QPaintEvent *event) override;
    
    /**
     * @brief 重写鼠标点击事件
     * @param event 鼠标事件对象
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 重写窗口大小变化事件
     * @param event 窗口大小变化事件对象
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 游戏循环槽函数
     * 
     * 每帧调用一次，负责更新球的位置、检测碰撞和更新游戏状态
     */
    void gameLoop();
    
    /**
     * @brief 开始/暂停游戏槽函数
     * 
     * 处理开始按钮点击事件，切换游戏运行状态
     */
    void startGame();
    
    /**
     * @brief 重置游戏槽函数
     * 
     * 处理重置按钮点击事件，重新初始化游戏状态
     */
    void resetGame();

private:
    /**
     * @brief 初始化游戏状态
     * 
     * 创建球对象，设置初始位置和速度，重置游戏变量
     */
    void initGame();
    
    /**
     * @brief 初始化用户界面
     * 
     * 创建按钮、标签和布局，设置窗口属性
     */
    void initUI();
    
    /**
     * @brief 检查球与圆圈的碰撞
     * @param ball 需要检查碰撞的球对象
     * 
     * 处理球与游戏区域边界圆圈的碰撞逻辑，包括反弹和添加连接线
     */
    void checkCircleCollision(Ball *ball);
    
    /**
     * @brief 检查球与球的碰撞
     * @param ball1 第一个球对象
     * @param ball2 第二个球对象
     * 
     * 处理两个球之间的碰撞逻辑，包括速度交换和位置调整
     */
    void checkBallCollision(Ball *ball1, Ball *ball2);
    
    /**
     * @brief 检查球是否碰到连接线
     * 
     * 检查所有球是否碰到其他球的连接线，处理连接线转移逻辑
     */
    void checkLineCollision();
    
    /**
     * @brief 检查游戏结束条件
     * @return 如果游戏结束返回true，否则返回false
     * 
     * 检查是否只剩一个球或有球达到100条连接线
     */
    bool checkGameOver();
    
    /**
     * @brief 更新游戏状态显示
     * 
     * 更新分数标签和状态信息
     */
    void updateGameState();
    
    /**
     * @brief 绘制游戏状态信息
     * @param painter 用于绘制的QPainter对象
     */
    void drawGameStatus(QPainter *painter);

private:
    QList<Ball*> m_balls;      // 所有球对象列表
    QTimer *m_timer;           // 游戏计时器
    bool m_isRunning;          // 游戏是否运行
    qreal m_circleRadius;      // 圆圈半径
    QPointF m_circleCenter;    // 圆圈中心
    qreal m_gameSpeed;         // 游戏速度
    QPushButton *m_startButton; // 开始按钮
    QLabel *m_statusLabel;     // 状态标签
    QList<QLabel*> m_scoreLabels; // 分数标签列表
};

#endif // BALLGAME_H
