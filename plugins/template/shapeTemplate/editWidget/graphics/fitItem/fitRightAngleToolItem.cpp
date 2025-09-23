#include "fitRightAngleToolItem.h"
#include "fitLineItem.h"
namespace TIGER_Graphics
{
    CFitRightAngleToolItem::CFitRightAngleToolItem(QGraphicsItem *parent) : IFitToolItem(parent)
    {
        m_pTopItem = new CFitLineItem(this);
        m_pTopItem->setFreeEndPt(true);
        m_pTopItem->m_startPt = QPointF(0, 0);
        m_pTopItem->m_endPt = QPointF(100, 0);
        m_pTopItem->updatePtPos();

        m_pLeftItem = new CFitLineItem(this);
        m_pLeftItem->setAdjustPtFlags(CFitLineItem::ExtendAdjustPt);
        m_pLeftItem->setFreeEndPt(false);
        m_pLeftItem->setReverseNormal(true);
        m_pLeftItem->m_startPt = QPointF(0, 0);
        m_pLeftItem->m_endPt = QPointF(0, 100);
        m_pLeftItem->updatePtPos();

        itemPosChanged();

        addBasicItem(m_pTopItem);
        addBasicItem(m_pLeftItem);

        connect(m_pTopItem, &IBasicFitItem::sigPtPosChanged, this, &CFitRightAngleToolItem::itemPosChanged);
        connect(m_pLeftItem, &IBasicFitItem::sigPtPosChanged, this, &CFitRightAngleToolItem::itemPosChanged);
    }

    int CFitRightAngleToolItem::fitType() const
    {
        return RightAngleFitTool;
    }

    QVector<QVector<CFitRect>> CFitRightAngleToolItem::fitRects() const
    {
        QVector<QVector<CFitRect>> rects;
        rects << m_pTopItem->fitRects();
        rects << m_pLeftItem->fitRects();
        return rects;
    }

    void CFitRightAngleToolItem::setToolRect(QRectF p_rect)
    {
        m_pTopItem->blockSignals(true);
        m_pLeftItem->blockSignals(true);
        m_pTopItem->m_startPt = QPointF(p_rect.left(), p_rect.top());
        m_pTopItem->m_endPt = QPointF(p_rect.right(), p_rect.top());
        m_pTopItem->updatePtPos();

        m_pLeftItem->m_startPt = m_pTopItem->m_startPt;
        m_pLeftItem->m_endPt = QPointF(p_rect.left(), p_rect.bottom());
        m_pLeftItem->updatePtPos();

        m_pTopItem->blockSignals(false);
        m_pLeftItem->blockSignals(false);
        itemPosChanged();
        emit sigFitRectChanged();
    }

    void CFitRightAngleToolItem::itemPosChanged()
    {
        auto h = QLineF(m_pLeftItem->m_startPt, m_pLeftItem->m_endPt).length();
        auto w = QLineF(m_pTopItem->m_startPt, m_pTopItem->m_endPt).length();
        m_pLeftItem->m_startPt = m_pTopItem->m_startPt;
        m_pLeftItem->m_endPt = m_pLeftItem->m_startPt - m_pTopItem->m_lineNor * h;

        for (auto pItem : QVector<CFitLineItem *>{m_pLeftItem})
        {
            pItem->blockSignals(true);
            pItem->m_fitRectHalHeight = m_pTopItem->m_fitRectHalHeight;
            pItem->m_fitRectHalWidth = m_pTopItem->m_fitRectHalWidth;
            pItem->m_fitRectHalfCount = m_pTopItem->m_fitRectHalfCount;
            pItem->updatePtPos();
            pItem->blockSignals(false);
        }
        m_fitPos = m_pTopItem->m_startPt;
    }

}