#include "positionScene.h"
#include "../sceneDef.h"
#include "template/shapeTemplate/editWidget/graphics/positioningTool/positioningTool.h"

#include "Ui/graphics/tool/graphicsToolFun.h"

#include <QGraphicsView>

namespace TIGER_Graphics
{
    void CPositionScene::addTool(IPositioningToolItem *p_pTool)
    {
        p_pTool->disconnect(p_pTool, &IPositioningToolItem::sigGroupShapeChanged, this, &CPositionScene::sigItemChanged);
        double w = INT32_MIN, h = INT32_MIN;
        QPointF center;
        for (auto pView : this->views())
        {
            auto rect = pView->mapToScene(pView->rect()).boundingRect();
            auto viewSize = rect.size() * 0.5;
            center = rect.center();
            if (w < viewSize.width())
            {
                w = viewSize.width();
            }
            if (h < viewSize.height())
            {
                h = viewSize.height();
            }
            w = qMax(20.0, w);
            h = qMax(20.0, h);
        }
        w = qMin(m_pImage->width() * 0.5, w);
        h = qMin(m_pImage->height() * 0.5, h);
        p_pTool->setShapeSize(center, w, h);
        p_pTool->updateKeyPointRect();
        connect(p_pTool, &IPositioningToolItem::sigGroupShapeChanged, this, &CPositionScene::sigItemChanged);
        m_toolItems.append(p_pTool);
        addItemShape(p_pTool);
    }

    void CPositionScene::removeMyItem(QGraphicsItem *p_item)
    {
        p_item = parentItem(p_item);
        assert(p_item->type() == QGraphicsItemGroup::Type);
        for (auto pItem : m_toolItems)
        {
            if (pItem == p_item)
            {
                m_toolItems.removeOne(pItem);
            }
        }
        emit sigRemoveItem(p_item);
        this->removeItem(p_item);
        delPtr(p_item);
        emit sigItemChanged();
    }

    QList<QGraphicsItem *> CPositionScene::myGraphicsItems() const
    {
        QList<QGraphicsItem *> pItems;
        for (auto pItem : m_toolItems)
        {
            pItems.append(pItem);
        }
        return pItems;
    }

    void CPositionScene::clearMyItem()
    {
        m_toolItems.clear();
        m_initPath = QPainterPath();
    }

}