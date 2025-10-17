#ifndef BALL_H
#define BALL_H

#include <QObject>
#include <QPointF>
#include <QColor>
#include <QList>
#include <QPainter>

/**
 * @brief 小球类，代表游戏中的单个球对象
 * 
 * 负责处理单个球的状态、位置、速度、颜色、连接线等属性和行为
 */
class Ball : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象，默认为nullptr
     */
    explicit Ball(QObject *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~Ball();

    /**
     * @brief 设置球的位置
     * @param position 球的新位置坐标
     */
    void setPosition(const QPointF &position);
    
    /**
     * @brief 获取球的当前位置
     * @return 球的位置坐标
     */
    QPointF position() const;

    /**
     * @brief 设置球的速度
     * @param velocity 球的速度向量
     */
    void setVelocity(const QPointF &velocity);
    
    /**
     * @brief 获取球的当前速度
     * @return 球的速度向量
     */
    QPointF velocity() const;

    /**
     * @brief 设置球的颜色
     * @param color 球的颜色
     */
    void setColor(const QColor &color);
    
    /**
     * @brief 获取球的颜色
     * @return 球的颜色
     */
    QColor color() const;

    /**
     * @brief 设置球的半径
     * @param radius 球的半径
     */
    void setRadius(qreal radius);
    
    /**
     * @brief 获取球的半径
     * @return 球的半径
     */
    qreal radius() const;

    /**
     * @brief 设置球的ID
     * @param id 球的唯一标识符
     */
    void setId(int id);
    
    /**
     * @brief 获取球的ID
     * @return 球的唯一标识符
     */
    int id() const;

    /**
     * @brief 添加连接线（连接球和圆圈的碰撞点）
     * @param circlePoint 圆圈上的碰撞点坐标
     */
    void addConnection(const QPointF &circlePoint);
    
    /**
     * @brief 移除指定索引的连接线
     * @param index 要移除的连接线索引
     * @exception 如果索引无效，可能导致未定义行为
     */
    void removeConnection(int index);
    
    /**
     * @brief 更新连接线（当其他球碰到线时，球端的点转移到碰撞球）
     * @param index 要更新的连接线索引
     * @param newBallPoint 新的球端连接点
     * @exception 如果索引无效，可能导致未定义行为
     */
    void updateConnection(int index, const QPointF &newBallPoint);
    
    /**
     * @brief 获取连接线数量
     * @return 球当前的连接线数量
     */
    int connectionCount() const;
    
    /**
     * @brief 获取所有连接线（圆上的点）
     * @return 包含所有连接线圆上点的列表
     */
    QList<QPointF> connections() const;

    /**
     * @brief 检查球是否被淘汰（没有连接线）
     * @return 如果球没有连接线则返回true，否则返回false
     */
    bool isEliminated() const;

    /**
     * @brief 更新球的位置
     * @param deltaTime 时间间隔（秒）
     */
    void updatePosition(qreal deltaTime);

    /**
     * @brief 绘制球
     * @param painter 用于绘制的QPainter对象
     */
    void draw(QPainter *painter);

    /**
     * @brief 绘制连接线
     * @param painter 用于绘制的QPainter对象
     */
    void drawConnections(QPainter *painter);

private:
    QPointF m_position;      // 位置
    QPointF m_velocity;      // 速度
    QColor m_color;          // 颜色
    qreal m_radius;          // 半径
    int m_id;                // 球ID
    QList<QPointF> m_connections; // 与圆圈的连接点
    bool m_eliminated;       // 是否被淘汰
};

#endif // BALL_H
