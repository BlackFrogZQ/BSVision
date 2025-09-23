#include "roiDelegate.h"
#include "Ui/graphics/item/rect/rectItem.h"
#include <QGraphicsScene>
using namespace BS_QT_Ui;

CRoiDelegate::CRoiDelegate(QGraphicsScene *p_pScene)
    : QObject(p_pScene),
      m_pScene(p_pScene),
      m_pRoiItem(nullptr)
{
    assert(m_pScene != nullptr);
    m_pRoiItem = new CRectItem;
    m_pRoiItem->setControlFlags(
        CGraphicsItemControlDef::resizeTopLeft |
        CGraphicsItemControlDef::resizeTopRight |
        CGraphicsItemControlDef::resizeBottomLeft |
        CGraphicsItemControlDef::resizeBottomRight |
        CGraphicsItemControlDef::changeTopEdge |
        CGraphicsItemControlDef::changeBottomEdge |
        CGraphicsItemControlDef::changeLeftEdge |
        CGraphicsItemControlDef::changeRightEdge |
        CGraphicsItemControlDef::move);
    m_pRoiItem->addKeyPoints(QList<QPointF>() << QPoint(-100, -100) << QPointF(100, 100));
    m_pRoiItem->setContextMenuFlags(CItemContextMenu::None);
    m_pScene->addItem(m_pRoiItem);
    m_pRoiItem->setVisible(false);
    connect(m_pRoiItem, &CRectItem::sigShapeChanged, this, &CRoiDelegate::slotResize);
}

QRectF CRoiDelegate::roiRect(QPointF p_offset) const
{
    auto roi = m_pRoiItem->rect();
    if (p_offset != QPointF(0, 0))
    {
        roi.translate(p_offset);
    }
    return roi;
}

void CRoiDelegate::setLimitRoi(QRectF p_limitRect)
{
    if (p_limitRect.isValid() && m_limitRect != p_limitRect)
    {
        m_limitRect = p_limitRect;
        slotResize();
    }
}

void CRoiDelegate::setRoi(QRectF p_rect, QPointF p_offset)
{
    if (p_offset != QPointF(0, 0))
    {
        p_rect.translate(p_offset);
    }
    if (p_rect.isValid())
    {
        m_pRoiItem->setRect(p_rect);
    }
}

void CRoiDelegate::setUsedRoi(bool p_used)
{
    if (m_pRoiItem->isVisible() != p_used)
    {
        m_pRoiItem->setVisible(p_used);
        emit sigUsedRoiChanged(p_used);
    }
}

void CRoiDelegate::setAdjustRoi(bool p_adjust)
{
    auto flags = m_pRoiItem->flags();
    if (flags.testFlag(QGraphicsItem::ItemIsSelectable) != p_adjust)
    {
        m_pRoiItem->setFlags(
            p_adjust ? (flags | QGraphicsItem::ItemIsSelectable)
                     : (flags & ~QGraphicsItem::ItemIsSelectable));
        m_pRoiItem->setSelected(p_adjust);
        emit sigAdjusRoiChanged(p_adjust);
    }
}

void CRoiDelegate::setColor(QColor p_color)
{
    m_pRoiItem->setLineColor(p_color);
}

void CRoiDelegate::slotResize()
{
    auto rect = m_pRoiItem->rect();
    if (m_limitRect.isValid() && !m_limitRect.contains(rect))
    {
        m_pRoiItem->setRect(m_lastRoiRect);
        return;
    }
    m_lastRoiRect = rect;
    emit sigRoiChanged(m_lastRoiRect);
}