#pragma once

#include <QPoint>
#include <cmath>
#include <QPainter>
#include <QVector2D>

class Ball {
protected:
    QBrush m_brush;
    QVector2D m_pos;
    QVector2D m_velocity;
    double m_mass;
    int m_radius;
    QString color;

    // if movement is slower than this, then we're considered at a stand-still
    static constexpr double MovementEpsilon = 1;
public:
    virtual ~Ball() {}
    Ball(QColor colour, QVector2D position,
         QVector2D velocity, double mass, int radius) :
        m_brush(colour), m_pos(position), m_velocity(velocity),
        m_mass(mass), m_radius(radius), color(colour.name()){}
    Ball() {}

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    virtual void render(QPainter &painter, const QVector2D& offset) = 0;
    /**
     * @brief translate - Move the ball's position by provided vector
     * @param vec - vector
     */
    virtual void translate(QVector2D vec) { m_pos += vec; }

    virtual QVector2D getVelocity() const { return m_velocity; }
    virtual void setVelocity(QVector2D v) { m_velocity = v; }
    /**
     * @brief changeVelocity - modify speed by a constant amount
     * @param delta - change in velocity (x,y)
     */
    virtual void changeVelocity(const QVector2D& delta) { m_velocity += delta; }
    /**
     * @brief multiplyVelocity - apply vector multiplicatively
     * @param vel - vector
     */
    virtual void multiplyVelocity(const QVector2D& vel) { m_velocity *= vel; }

    virtual double getMass() const { return m_mass; }
    virtual double getRadius() const { return m_radius; }
    virtual QVector2D getPosition() const { return m_pos; }
    virtual void setPosition(QVector2D p) { m_pos = p; }
    virtual QColor getColor() const {return m_brush.color();}
    // whether the ball will break, and handle accordingly
    // for base ball, do nothing. insert into rhs if necessary
    virtual bool applyBreak(const QVector2D&, std::vector<Ball*>&) { return false; }
};

class StageOneBall : public Ball {
public:
    StageOneBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius) :
        Ball(colour, position, velocity, mass, radius) {}
    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;
};

class CompositeBall : public Ball {
protected:
    std::vector<Ball*> m_children;
    bool m_renderChildren = true;
    void recursiveRender(QPainter& painter, const QVector2D& offset);
    // default is unbreakable (i.e. inf str)
    double m_strength = std::numeric_limits<double>::max();
public:
    CompositeBall(QColor colour, QVector2D position,
                 QVector2D velocity, double mass, int radius, double strength) :
        Ball(colour, position, velocity, mass, radius), m_strength(strength) { }

    /**
     * @brief render - draw the ball to the screen
     * @param painter - QPainter that is owned by the dialog
     */
    void render(QPainter &painter, const QVector2D& offset) override;
    double getStrength() const{return m_strength;}
    std::vector<Ball*> getChildren(){return m_children;}

    /* add a child ball to this composite ball */
    void addChild(Ball* b) { m_children.push_back(b); }

    /**
     * @brief applyBreak - check and resolve breaking balls
     * @param deltaV - the change in velocity
     * @param parentlist - the list of balls that we'll need to add to if we break anything
     * @return whether the ball broke or not
     */
    virtual bool applyBreak(const QVector2D& deltaV, std::vector<Ball*>& parentlist) override;
};
