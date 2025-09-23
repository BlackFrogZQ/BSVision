#include "fitRectToolItem.h"
#include "fitLineItem.h"
namespace TIGER_Graphics
{
    CFitRectToolItem::CFitRectToolItem(QGraphicsItem *parent) : IFitToolItem(parent)
    {
        m_pTopItem = new CFitLineItem(this);
        m_pTopItem->setFreeEndPt(true);
        m_pTopItem->m_startPt = QPointF(0, 0);
        m_pTopItem->m_endPt = QPointF(100, 0);
        m_pTopItem->updatePtPos();

        m_pRightItem = new CFitLineItem(this);
        m_pRightItem->setAdjustPtFlags(CFitLineItem::ExtendAdjustPt);
        m_pRightItem->setFreeEndPt(false);
        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = QPointF(100, 100);
        m_pRightItem->updatePtPos();

        m_pBottomItem = new CFitLineItem(this);
        m_pBottomItem->setAdjustPtFlags(CFitLineItem::NoneAdjustPt);

        m_pLeftItem = new CFitLineItem(this);
        m_pLeftItem->setAdjustPtFlags(CFitLineItem::NoneAdjustPt);

        itemPosChanged();

        addBasicItem(m_pTopItem);
        addBasicItem(m_pRightItem);
        addToGroup(m_pBottomItem);
        addToGroup(m_pLeftItem);
        connect(m_pTopItem, &IBasicFitItem::sigPtPosChanged, this, &CFitRectToolItem::itemPosChanged);
        connect(m_pRightItem, &IBasicFitItem::sigPtPosChanged, this, &CFitRectToolItem::itemPosChanged);
    }

    int CFitRectToolItem::fitType() const
    {
        return RectCenterFitTool;
    }

    QVector<QVector<CFitRect>> CFitRectToolItem::fitRects() const
    {
        QVector<QVector<CFitRect>> rects;
        rects << m_pBottomItem->fitRects();
        rects << m_pRightItem->fitRects();
        rects << m_pTopItem->fitRects();
        rects << m_pLeftItem->fitRects();
        return rects;
    }

    void CFitRectToolItem::setToolRect(QRectF p_rect)
    {
        m_pTopItem->blockSignals(true);
        m_pRightItem->blockSignals(true);
        m_pTopItem->m_startPt = QPointF(p_rect.left(), p_rect.top());
        m_pTopItem->m_endPt = QPointF(p_rect.right(), p_rect.top());
        m_pTopItem->updatePtPos();

        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = QPointF(p_rect.right(), p_rect.bottom());
        m_pRightItem->updatePtPos();

        m_pTopItem->blockSignals(false);
        m_pRightItem->blockSignals(false);
        itemPosChanged();
        emit sigFitRectChanged();
    }

    void CFitRectToolItem::itemPosChanged()
    {
        auto h = QLineF(m_pRightItem->m_startPt, m_pRightItem->m_endPt).length();
        auto w = QLineF(m_pTopItem->m_startPt, m_pTopItem->m_endPt).length();
        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = m_pRightItem->m_startPt - m_pTopItem->m_lineNor * h;

        m_pBottomItem->m_startPt = m_pRightItem->m_endPt;
        m_pBottomItem->m_endPt = m_pBottomItem->m_startPt - m_pTopItem->m_lineDir * w;

        m_pLeftItem->m_startPt = m_pBottomItem->m_endPt;
        m_pLeftItem->m_endPt = m_pTopItem->m_startPt;
        m_pTopItem->m_fitRectHalHeight = std::min(m_pTopItem->m_fitRectHalHeight, h / 2);
        for (auto pItem : QVector<CFitLineItem *>{m_pTopItem, m_pRightItem, m_pBottomItem, m_pLeftItem})
        {
            pItem->blockSignals(true);
            pItem->m_fitRectHalHeight = m_pTopItem->m_fitRectHalHeight;
            pItem->m_fitRectHalWidth = m_pTopItem->m_fitRectHalWidth;
            pItem->m_fitRectHalfCount = m_pTopItem->m_fitRectHalfCount;
            pItem->updatePtPos();
            pItem->blockSignals(false);
        }
        m_fitPos = (m_pTopItem->m_startPt + m_pBottomItem->m_startPt) * 0.5;
    }

}