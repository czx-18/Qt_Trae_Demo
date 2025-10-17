#include "snake.h"
#include <QDebug>

/**
 * @brief Snake类构造函数
 * @param parent 父对象指针
 * 
 * 初始化蛇对象，设置初始状态
 */
Snake::Snake(QObject *parent) : QObject(parent)
{
    // 调用重置函数初始化蛇的状态
    reset();
}

/**
 * @brief 获取蛇的身体部分
 * @return 返回包含蛇身体所有点的QVector
 */
QVector<QPoint> Snake::getBody() const
{
    return m_body;
}

/**
 * @brief 设置蛇的移动方向
 * @param dir 要设置的方向
 * 
 * 该函数会防止蛇直接180度转向，例如向上移动时不能直接改为向下
 */
void Snake::setDirection(Direction dir)
{
    // 防止180度转向：只有当新方向不是相反方向时才允许更改
    if ((dir == Up && m_direction != Down) ||
        (dir == Down && m_direction != Up) ||
        (dir == Left && m_direction != Right) ||
        (dir == Right && m_direction != Left)) {
        m_nextDirection = dir;
    }
}

/**
 * @brief 移动蛇
 * 
 * 根据当前方向移动蛇的头部，并根据是否需要增长决定是否移除尾部
 */
void Snake::move()
{
    // 更新方向（使用m_nextDirection确保平滑转向）
    m_direction = m_nextDirection;
    
    // 获取头部位置
    QPoint head = m_body.first();
    
    // 根据方向移动头部
    switch (m_direction) {
    case Up:
        head.setY(head.y() - 1);
        break;
    case Down:
        head.setY(head.y() + 1);
        break;
    case Left:
        head.setX(head.x() - 1);
        break;
    case Right:
        head.setX(head.x() + 1);
        break;
    }
    
    // 将新头部添加到身体
    m_body.prepend(head);
    
    // 如果不需要增长，则移除尾部
    if (!m_grow) {
        m_body.removeLast();
    } else {
        m_grow = false;  // 重置增长标志
    }
}

/**
 * @brief 检查蛇是否撞到自己
 * @return 如果头部与身体其他部分碰撞则返回true，否则返回false
 */
bool Snake::checkSelfCollision() const
{
    // 检查头部是否与身体其他部分碰撞
    QPoint head = m_body.first();
    for (int i = 1; i < m_body.size(); ++i) {
        if (head == m_body.at(i)) {
            return true;  // 发生碰撞
        }
    }
    return false;  // 没有碰撞
}

/**
 * @brief 增长蛇的长度
 * 
 * 标记蛇在下一次移动时需要增长
 */
void Snake::grow()
{
    m_grow = true;
}

/**
 * @brief 获取蛇头位置
 * @return 返回蛇头的坐标点
 */
QPoint Snake::getHeadPosition() const
{
    return m_body.first();
}

/**
 * @brief 重置蛇的状态
 * 
 * 将蛇恢复到初始状态，包括位置、长度和方向
 */
void Snake::reset()
{
    // 清空身体
    m_body.clear();
    
    // 初始化蛇的位置（3个点组成的初始长度）
    m_body.append(QPoint(10, 10));  // 头部
    m_body.append(QPoint(9, 10));   // 第一节身体
    m_body.append(QPoint(8, 10));   // 第二节身体
    
    // 默认向右移动
    m_direction = Right;
    m_nextDirection = Right;
    m_grow = false;
}
