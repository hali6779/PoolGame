#pragma once

#include <QVector2D>
#include <QPainter>
#include <QBrush>
#include <cmath>

class Pocket
{
    double m_radius;
    QVector2D m_pos;
    // default rendering colour for pockets is just black
    QBrush m_pocketBrush = QBrush(QColor("black"));

    size_t m_sunk = 0;
public:
    Pocket(double radius, QVector2D pos) : m_radius(radius), m_pos(pos) {}

    /**
     * @brief render - draw the pocket to the screen with the provided brush and offset
     * @param painter - the brush to paint with
     * @param offset - the offset from the window
     */
    void render(QPainter& painter, const QVector2D& offset);

    /// whether this pocket contains the circle defined by the arguments
    bool contains(const QVector2D& center, const double& radius) {
        return ((center-m_pos).length() < fabs(radius - m_radius));
    }

    /** add whether this pocket has sunk a ball */
    void incrementSunk() { ++m_sunk; }
};
