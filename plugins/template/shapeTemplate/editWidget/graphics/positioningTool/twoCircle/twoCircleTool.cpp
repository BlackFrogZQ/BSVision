#include "twoCircleTool.h"
#include "Ui/graphics/item/ellipse/ellipseItem.h"

namespace TIGER_Graphics
{
    using namespace BS_QT_Ui;
    CTwoCircleTool::CTwoCircleTool(QGraphicsItem *parent) : IPositioningToolItem(parent)
    {
        IGraphicsItem *pEllipse1 = new CEllipseItem;
        pEllipse1->addKeyPoints({{-50, -50}, {0, 0}});
        this->add(pEllipse1);
        IGraphicsItem *pEllipse2 = new CEllipseItem;
        pEllipse2->addKeyPoints({{50, 50}, {0, 0}});
        this->add(pEllipse2);
    }

    QPointF CTwoCircleTool::getPosition()
    {
        return drawShape().boundingRect().center();
    }

    void CTwoCircleTool::setShapeSize(QPointF center, double w, double h)
    {
        auto pEllipseItem1 = m_child[0];
        auto pEllipseItem2 = m_child[1];
        auto points1 = pEllipseItem1->getKeyPoints();
        auto points2 = pEllipseItem1->getKeyPoints();
        auto offset = QPointF(w * 0.25, h * 0.25);
        pEllipseItem1->setKeyPoint(CEllipseItem::ckptCenter, center - offset);
        pEllipseItem2->setKeyPoint(CEllipseItem::ckptCenter, center + offset);
        pEllipseItem1->setKeyPoint(CEllipseItem::ckptTopRight, center - offset + QPointF(w * 0.15, h * 0.15));
        pEllipseItem2->setKeyPoint(CEllipseItem::ckptTopRight, center + offset + QPointF(w * 0.15, h * 0.15));
    }

    QPainterPath CTwoCircleTool::drawShape() const
    {
        assert(m_child.size() == 2);
        IGraphicsItem *pItem = m_child[0];
        QPainterPath path;
        path.moveTo(pItem->mapToScene(pItem->drawShape()).boundingRect().center());
        pItem = m_child[1];
        path.lineTo(pItem->mapToScene(pItem->drawShape()).boundingRect().center());
        return path;
    }
};