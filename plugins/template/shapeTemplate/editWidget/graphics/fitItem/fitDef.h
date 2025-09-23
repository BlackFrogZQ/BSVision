#pragma once
#include <QPolygonF>
#include <QGraphicsItem>
namespace TIGER_Graphics
{
    struct CFitRect
    {
        CFitRect(double p_cx = 0,
                 double p_cy = 0,
                 double p_w = 0,
                 double p_h = 0,
                 double p_angle = 0);
        double cx = 0;
        double cy = 0;
        double w = 0;
        double h = 0;
        double angle = 0;
        QPolygonF toPolygon() const;
        CFitRect &moveCenter(double p_x, double p_y);
        CFitRect &moveCenter(const QPointF &p) { return moveCenter(p.x(), p.y()); };
    };

    class IBasicFitItem : public QObject, public QGraphicsItem
    {
        Q_OBJECT
        Q_INTERFACES(QGraphicsItem)
    public:
        using QGraphicsItem::QGraphicsItem;
        virtual ~IBasicFitItem() = default;
        QVector<CFitRect> fitRects() const;
        QRectF boundingRect() const override;

    signals:
        void sigPtPosChanged();
        void sigPtPosChangeFinished();

    protected:
        QRectF m_aabb;
        QVector<CFitRect> m_fitRects;
    };
}