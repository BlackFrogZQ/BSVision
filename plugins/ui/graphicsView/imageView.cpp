#include "imageView.h"
#include "Ui/graphics/tool/graphicsToolFun.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
namespace TIGER_Graphics
{

    void CImageView::showPoints(QList<QPointF> p_features, QPointF p_offset)
    {
        m_features = p_features;
        for (auto &p : m_features)
        {
            p += p_offset;
        }
    }

    QList<QPointF> CImageView::getShowPoints() const
    {
        return m_features;
    }

    void CImageView::clearPoints()
    {
        m_features.clear();
    }

    void CImageView::setPointColor(QColor color)
    {
        m_featureColor = color;
    }

    void CImageView::drawPoints(QPainter *painter, QList<QPointF> p_points, QColor color)
    {
        if (p_points.isEmpty())
        {
            return;
        }
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->resetMatrix();

        QVector<QPointF> drawPoints = p_points.toVector();
        for (auto &p : drawPoints)
        {
            p = mapFromScene(p);
        }

        QPolygonF polygon;
        polygon.append(drawPoints);
        auto polygonRect = polygon.boundingRect();
        const double cMin = 5;
        if (polygonRect.width() < cMin || polygonRect.height() < cMin)
        {
            polygonRect.setWidth(qMax(cMin, polygonRect.width()));
            polygonRect.setHeight(qMax(cMin, polygonRect.height()));
            painter->fillRect(polygonRect, color);
        }
        else
        {
            auto rect = mapToScene(QRect(0, 0, 1, 1)).boundingRect();
            m_scale < 1.0 ? painter->setPen(QPen(color, 2)) : painter->setPen(QPen(color, 1.0 / qMin(rect.width(), rect.height())));
            painter->drawPoints(drawPoints.data(), drawPoints.size());
        }
        painter->restore();
    }

    void CImageView::drawForeground(QPainter *painter, const QRectF &rect)
    {
        BS_QT_Ui::CImageView::drawForeground(painter, rect);
        drawPoints(painter, m_features, m_featureColor);
    }
};
