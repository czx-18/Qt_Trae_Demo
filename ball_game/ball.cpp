#include "ball.h"
#include <QPainter>

Ball::Ball(QObject *parent) : QObject(parent)
    , m_position(QPointF())
    , m_velocity(QPointF())
    , m_color(Qt::red)
    , m_radius(10.0)
    , m_id(-1)
    , m_eliminated(false)
{}

Ball::~Ball()
{}

void Ball::setPosition(const QPointF &position)
{
    m_position = position;
}

QPointF Ball::position() const
{
    return m_position;
}

void Ball::setVelocity(const QPointF &velocity)
{
    m_velocity = velocity;
}

QPointF Ball::velocity() const
{
    return m_velocity;
}

void Ball::setColor(const QColor &color)
{
    m_color = color;
}

QColor Ball::color() const
{
    return m_color;
}

void Ball::setRadius(qreal radius)
{
    m_radius = radius;
}

qreal Ball::radius() const
{
    return m_radius;
}

void Ball::setId(int id)
{
    m_id = id;
}

int Ball::id() const
{
    return m_id;
}

void Ball::addConnection(const QPointF &circlePoint)
{
    m_connections.append(circlePoint);
    m_eliminated = false; // 添加连接后不再被淘汰
}

void Ball::removeConnection(int index)
{
    if (index >= 0 && index < m_connections.size()) {
        m_connections.removeAt(index);
        // 如果没有连接线了，则被淘汰
        if (m_connections.isEmpty()) {
            m_eliminated = true;
        }
    }
}

// 修改updateConnection方法，确保线的更新逻辑正确
void Ball::updateConnection(int index, const QPointF &newBallPoint)
{
    // 注意：在我们的实现中，连接线存储的是圆上的点
    // 球端的点始终是球的当前位置，所以这里不需要更新存储的点
    // 但可以添加一个日志或者调试信息
    Q_UNUSED(index);
    Q_UNUSED(newBallPoint);
    // 实际的绘制会自动使用球的当前位置
}

int Ball::connectionCount() const
{
    return m_connections.size();
}

QList<QPointF> Ball::connections() const
{
    return m_connections;
}

bool Ball::isEliminated() const
{
    return m_eliminated;
}

void Ball::updatePosition(qreal deltaTime)
{
    if (!m_eliminated) {
        m_position += m_velocity * deltaTime;
    }
}

void Ball::draw(QPainter *painter)
{
    if (m_eliminated) {
        return; // 被淘汰的球不绘制
    }

    painter->save();
    painter->setBrush(m_color);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(m_position, m_radius, m_radius);
    
    // 绘制球ID或连接数
    painter->setPen(Qt::white);
    painter->drawText(m_position.x() - 5, m_position.y() + 5, QString::number(m_connections.size()));
    painter->restore();
}

void Ball::drawConnections(QPainter *painter)
{
    if (m_eliminated) {
        return;
    }

    painter->save();
    painter->setPen(QPen(m_color, 1.5));
    
    // 绘制每条连接线（从球的当前位置到圆上的点）
    for (const QPointF &circlePoint : m_connections) {
        painter->drawLine(m_position, circlePoint);
    }
    
    painter->restore();
}