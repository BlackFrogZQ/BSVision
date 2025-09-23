#include "fitLineCenterToolItem.h"
#include "fitLineItem.h"
namespace TIGER_Graphics
{
    CFitLineCenterToolItem::CFitLineCenterToolItem(QGraphicsItem *parent) : IFitToolItem(parent)
    {
        m_pTopItem = new CFitLineItem(this);
        m_pTopItem->setFreeEndPt(true);
        m_pTopItem->m_startPt = QPointF(0, 0);
        m_pTopItem->m_endPt = QPointF(100, 0);
        m_pTopItem->updatePtPos();

        m_pLeftItem = new CFitLineItem(this);
        m_pLeftItem->setAdjustPtFlags(CFitLineItem::ExtendAdjustPt);
        m_pLeftItem->setReverseNormal(true);
        m_pLeftItem->setFreeEndPt(false);
        m_pLeftItem->m_startPt = m_pTopItem->m_startPt;
        m_pLeftItem->m_endPt = QPointF(0, 100);
        m_pLeftItem->updatePtPos();

        m_pRightItem = new CFitLineItem(this);
        m_pRightItem->setAdjustPtFlags(CFitLineItem::ExtendAdjustPt);
        m_pRightItem->setFreeEndPt(false);
        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = QPointF(100, 100);
        m_pRightItem->updatePtPos();

        itemPosChanged();

        addBasicItem(m_pTopItem);
        addBasicItem(m_pRightItem);
        addBasicItem(m_pLeftItem);

        connect(m_pTopItem, &IBasicFitItem::sigPtPosChanged, this, &CFitLineCenterToolItem::itemPosChanged);
        connect(m_pRightItem, &IBasicFitItem::sigPtPosChanged, this, &CFitLineCenterToolItem::itemPosChanged);
        connect(m_pLeftItem, &IBasicFitItem::sigPtPosChanged, this, &CFitLineCenterToolItem::itemPosChanged);
    }

    int CFitLineCenterToolItem::fitType() const
    {
        return LineCenterFitTool;
    }

    QVector<QVector<CFitRect>> CFitLineCenterToolItem::fitRects() const
    {
        QVector<QVector<CFitRect>> rects;
        rects << m_pLeftItem->fitRects();
        rects << m_pTopItem->fitRects();
        rects << m_pRightItem->fitRects();
        return rects;
    }

    void CFitLineCenterToolItem::setToolRect(QRectF p_rect)
    {
        m_pRightItem->blockSignals(true);
        m_pTopItem->blockSignals(true);
        m_pLeftItem->blockSignals(true);

        m_pLeftItem->m_startPt = m_pTopItem->m_startPt;
        m_pLeftItem->m_endPt = QPointF(p_rect.left(), p_rect.bottom());
        m_pLeftItem->updatePtPos();

        m_pTopItem->m_startPt = QPointF(p_rect.left(), p_rect.top());
        m_pTopItem->m_endPt = QPointF(p_rect.right(), p_rect.top());
        m_pTopItem->updatePtPos();

        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = QPointF(p_rect.right(), p_rect.bottom());
        m_pRightItem->updatePtPos();

        m_pRightItem->blockSignals(false);
        m_pTopItem->blockSignals(false);
        m_pLeftItem->blockSignals(false);
        itemPosChanged();
        emit sigFitRectChanged();
    }

    void CFitLineCenterToolItem::itemPosChanged()
    {
        auto leftH = QLineF(m_pLeftItem->m_startPt, m_pLeftItem->m_endPt).length();
        auto rightH = QLineF(m_pRightItem->m_startPt, m_pRightItem->m_endPt).length();
        auto w = QLineF(m_pTopItem->m_startPt, m_pTopItem->m_endPt).length();
        m_pLeftItem->m_startPt = m_pTopItem->m_startPt;
        m_pLeftItem->m_endPt = m_pLeftItem->m_startPt - m_pTopItem->m_lineNor * leftH;
        m_pRightItem->m_startPt = m_pTopItem->m_endPt;
        m_pRightItem->m_endPt = m_pRightItem->m_startPt - m_pTopItem->m_lineNor * rightH;
        m_pTopItem->m_fitRectHalHeight = std::min(m_pTopItem->m_fitRectHalHeight, w / 2);
        for (auto pItem : QVector<CFitLineItem *>{m_pTopItem, m_pLeftItem, m_pRightItem})
        {
            pItem->blockSignals(true);
            pItem->m_fitRectHalHeight = m_pTopItem->m_fitRectHalHeight;
            pItem->m_fitRectHalWidth = m_pTopItem->m_fitRectHalWidth;
            pItem->m_fitRectHalfCount = m_pTopItem->m_fitRectHalfCount;
            pItem->updatePtPos();
            pItem->blockSignals(false);
        }
        m_fitPos = (m_pTopItem->m_startPt + m_pTopItem->m_endPt) * 0.5;
    }

}