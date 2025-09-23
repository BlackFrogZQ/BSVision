#include "lineCenterTool.h"
#include "lineCenterItem.h"
#include <QVector2D>
namespace TIGER_Graphics
{
    CLineCenterTool::CLineCenterTool(QGraphicsItem *parent) : IPositioningToolItem(parent)
    {
        m_pLineCenterItem = new CLineCenterItem;
        m_pLineCenterItem->addKeyPoints({{0, -100}, {0, 0}, {100, 0}, {100, -100}});
        this->add(m_pLineCenterItem);
    }

    QPointF CLineCenterTool::getPosition()
    {
        assert(m_child.size() == 1);
        auto points = m_pLineCenterItem->getSceneKeyPoints();
        return (points[CLineCenterItem::ckptRightAngle] + points[CLineCenterItem::ckptRightAngle2]) / 2;
    }

    QList<QPainterPath> CLineCenterTool::getToolPaths() const
    {
        auto points = m_pLineCenterItem->getSceneKeyPoints();
        QList<QPainterPath> paths;
        QPainterPath path;
        path.moveTo(points[CLineCenterItem::ckptVertical]);
        path.lineTo(points[CLineCenterItem::ckptRightAngle]);
        paths.append(path);
        path = QPainterPath();
        path.moveTo(points[CLineCenterItem::ckptRightAngle]);
        path.lineTo(points[CLineCenterItem::ckptRightAngle2]);
        paths.append(path);
        path = QPainterPath();
        path.moveTo(points[CLineCenterItem::ckptRightAngle2]);
        path.lineTo(points[CLineCenterItem::ckptVertical2]);
        paths.append(path);
        return paths;
    }
    void CLineCenterTool::setShapeSize(QPointF center, double w, double h)
    {
        auto points = m_pLineCenterItem->getSceneKeyPoints();
        w *= (points[CLineCenterItem::ckptRightAngle2].x() - points[CLineCenterItem::ckptRightAngle].x()) > 0 ? 1 : -1;
        h *= (points[CLineCenterItem::ckptVertical2].y() - points[CLineCenterItem::ckptRightAngle2].y()) > 0 ? 1 : -1;
        auto rightAngle = QPointF(center.x() - 0.5 * w, center.y() - 0.5 * h);
        auto rightAngle2 = QPointF(center.x() + 0.5 * w, center.y() - 0.5 * h);
        m_pLineCenterItem->setKeyPoint(CLineCenterItem::ckptVertical, QPointF(rightAngle.x(), rightAngle.y() + h));
        m_pLineCenterItem->setKeyPoint(CLineCenterItem::ckptRightAngle, rightAngle);
        m_pLineCenterItem->setKeyPoint(CLineCenterItem::ckptRightAngle2, rightAngle2);
        m_pLineCenterItem->setKeyPoint(CLineCenterItem::ckptVertical2, QPointF(rightAngle2.x(), rightAngle2.y() + h));
    }

    QPainterPath CLineCenterTool::drawShape() const
    {
        auto points = m_pLineCenterItem->getSceneKeyPoints();
        QPointF v1 = points[CLineCenterItem::ckptRightAngle];
        QPointF v2 = points[CLineCenterItem::ckptRightAngle2];
        QVector2D xExpend = QVector2D(v2 - v1);
        double minLen = xExpend.length();
        xExpend.normalize();
        QVector2D yExpend1 = QVector2D(v1 - points[CLineCenterItem::ckptVertical]);
        if (minLen > yExpend1.length())
        {
            minLen = yExpend1.length();
        }
        yExpend1.normalize();
        QVector2D yExpend2 = QVector2D(v2 - points[CLineCenterItem::ckptVertical2]);
        if (minLen > yExpend2.length())
        {
            minLen = yExpend2.length();
        }
        yExpend2.normalize();
        assert(minLen > 0);
        minLen = qMin(5.0, minLen * 0.1);
        minLen = qMax(1.0, minLen);
        QPainterPath path;

        path.moveTo(v1);
        path.lineTo(v1 - xExpend.toPointF() * minLen);
        path.moveTo(v1);
        path.lineTo(v1 + yExpend1.toPointF() * minLen);
        path.moveTo(v2);
        path.lineTo(v2 + xExpend.toPointF() * minLen);
        path.moveTo(v2);
        path.lineTo(v2 + yExpend2.toPointF() * minLen);
        return path;
    }
};