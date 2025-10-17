#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include "snake.h"

/**
 * @brief GameBoard类表示游戏的主界面和游戏逻辑控制器
 * 
 * 该类负责游戏区域的绘制、游戏循环控制、用户输入处理以及游戏规则的实现。
 * 它包含蛇对象、食物位置、游戏计时器等核心游戏元素。
 */
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口部件指针
     */
    explicit GameBoard(QWidget *parent = nullptr);
    
    /**
     * @brief 开始游戏
     * 
     * 启动游戏计时器，使蛇开始移动
     */
    void startGame();
    
    /**
     * @brief 暂停游戏
     * 
     * 停止游戏计时器，暂停蛇的移动
     */
    void pauseGame();
    
    /**
     * @brief 重置游戏
     * 
     * 将游戏恢复到初始状态，包括重置蛇的位置、分数和生成新的食物
     */
    void resetGame();
    
    /**
     * @brief 获取当前分数
     * @return 返回当前游戏分数
     */
    int getScore() const;

protected:
    /**
     * @brief 绘制游戏元素
     * @param event 绘制事件对象
     * 
     * 重写QWidget的paintEvent方法，用于绘制蛇、食物和游戏状态信息
     */
    void paintEvent(QPaintEvent *event) override;
    
    /**
     * @brief 处理键盘事件
     * @param event 键盘事件对象
     * 
     * 重写QWidget的keyPressEvent方法，用于处理用户的键盘输入控制
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief 调整窗口大小时重新绘制
     * @param event 窗口大小调整事件对象
     * 
     * 重写QWidget的resizeEvent方法，确保窗口大小改变时游戏画面能够正确缩放
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief 游戏循环
     * 
     * 游戏的核心逻辑循环，每间隔一定时间执行一次，处理蛇的移动、碰撞检测和分数更新
     */
    void gameLoop();

private:
    /**
     * @brief 生成食物
     * 
     * 在游戏区域内随机生成食物位置，确保不会出现在蛇身上
     */
    void generateFood();
    
    /**
     * @brief 检查是否吃到食物
     * @return 如果蛇头碰到食物则返回true，否则返回false
     */
    bool checkFoodCollision();
    
    /**
     * @brief 检查是否撞墙
     * @return 如果蛇头碰到边界墙则返回true，否则返回false
     */
    bool checkWallCollision();
    
    /**
     * @brief 获取方块尺寸
     * @return 返回游戏中方块的大小（像素）
     * 
     * 根据窗口尺寸和游戏区域大小计算适合的方块尺寸
     */
    int getSquareSize() const;
    
    /**
     * @brief 将游戏坐标转换为窗口坐标
     * @param gamePos 游戏坐标系中的点
     * @return 转换后的窗口坐标系中的点
     */
    QPoint gameToWindow(const QPoint &gamePos) const;
    
    /**
     * @brief 将窗口坐标转换为游戏坐标
     * @param windowPos 窗口坐标系中的点
     * @return 转换后的游戏坐标系中的点
     */
    QPoint windowToGame(const QPoint &windowPos) const;

private:
    Snake m_snake;              // 蛇对象
    QPoint m_food;              // 食物位置
    QTimer m_gameTimer;         // 游戏计时器（控制游戏速度）
    bool m_isGameRunning;       // 游戏是否正在运行
    int m_score;                // 当前分数
    int m_speed;                // 游戏速度（毫秒）
    int m_fieldWidth;           // 游戏区域宽度（格子数）
    int m_fieldHeight;          // 游戏区域高度（格子数）
    bool m_gameOver;            // 游戏是否结束
};

#endif // GAMEBOARD_H
