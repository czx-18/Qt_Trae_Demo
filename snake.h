#ifndef SNAKE_H
#define SNAKE_H

#include <QObject>
#include <QVector>
#include <QPoint>

// 蛇的移动方向枚举
enum Direction {
    Up,    // 向上移动
    Down,  // 向下移动
    Left,  // 向左移动
    Right  // 向右移动
};

/**
 * @brief Snake类表示游戏中的蛇对象
 * 
 * 该类负责管理蛇的状态、移动、碰撞检测等核心功能。
 * 蛇由一系列的点组成，通过QVector<QPoint>来存储。
 */
class Snake : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    explicit Snake(QObject *parent = nullptr);
    
    /**
     * @brief 获取蛇的身体部分
     * @return 返回包含蛇身体所有点的QVector
     */
    QVector<QPoint> getBody() const;
    
    /**
     * @brief 设置蛇的移动方向
     * @param dir 要设置的方向
     * 
     * 注意：该函数会防止蛇直接180度转向（例如向上移动时不能直接改为向下）
     */
    void setDirection(Direction dir);
    
    /**
     * @brief 移动蛇
     * 
     * 根据当前方向移动蛇的头部，并根据是否需要增长决定是否移除尾部
     */
    void move();
    
    /**
     * @brief 检查蛇是否撞到自己
     * @return 如果头部与身体其他部分碰撞则返回true，否则返回false
     */
    bool checkSelfCollision() const;
    
    /**
     * @brief 增长蛇的长度
     * 
     * 标记蛇在下一次移动时需要增长
     */
    void grow();
    
    /**
     * @brief 获取蛇头位置
     * @return 返回蛇头的坐标点
     */
    QPoint getHeadPosition() const;
    
    /**
     * @brief 重置蛇的状态
     * 
     * 将蛇恢复到初始状态，包括位置、长度和方向
     */
    void reset();

private:
    QVector<QPoint> m_body;       // 蛇的身体部分（一系列坐标点）
    Direction m_direction;        // 当前移动方向
    Direction m_nextDirection;    // 下一个移动方向（用于平滑转向）
    bool m_grow;                  // 是否需要增长（吃到食物后为true）
};

#endif // SNAKE_H
