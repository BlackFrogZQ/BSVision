#include "fitToolItem.h"
#include "Ui/widget/menu.h"
#include <QPainter>
namespace TIGER_Graphics
{
    IFitToolItem::IFitToolItem(QGraphicsItem *parent)
        : QObject(nullptr), QGraphicsItemGroup(parent)
    {
        this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemContainsChildrenInShape);
        this->setHandlesChildEvents(false);
    }

    QPointF IFitToolItem::fitPos() const
    {
        return m_fitPos;
    }
    void IFitToolItem::setFitPos(QPointF p)
    {
        if (m_fitPos != p)
        {
            m_fitPos = p;
            update();
        }
    }

    QRectF IFitToolItem::boundingRect() const
    {
        auto path = toolDrawPath();
        path.addPath(crossPath(m_fitPos));
        return childrenBoundingRect().united(this->mapFromScene(path).boundingRect());
    }

    void IFitToolItem::paint(QPainter *p_painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        QGraphicsItemGroup::paint(p_painter, option, widget);
        p_painter->save();
        p_painter->setRenderHint(QPainter::Antialiasing, true);
        p_painter->setBrush(Qt::NoBrush);
        p_painter->setPen(QPen(Qt::green, 0, Qt::SolidLine));
        p_painter->drawPath(this->mapFromScene(crossPath(m_fitPos)));
        p_painter->setPen(QPen(Qt::green, 0, Qt::DashLine));
        p_painter->drawPath(this->mapFromScene(toolDrawPath()));
        p_painter->restore();
    }

    QPainterPath IFitToolItem::crossPath(QPointF p)
    {
        const double cLen = 5;
        QPainterPath path;
        path.moveTo(-cLen, -cLen);
        path.lineTo(cLen, cLen);
        path.moveTo(-cLen, cLen);
        path.lineTo(cLen, -cLen);
        path.translate(p);
        return path;
    }

    void IFitToolItem::addBasicItem(IBasicFitItem *pItem)
    {
        assert(pItem != nullptr);
        QGraphicsItemGroup::addToGroup(pItem);
        disconnect(pItem, &IBasicFitItem::sigPtPosChangeFinished, this, nullptr);
        connect(pItem, &IBasicFitItem::sigPtPosChangeFinished, this, &IFitToolItem::sigFitRectChanged);
        emit sigFitRectChanged();
    }

    QPainterPath IFitToolItem::toolDrawPath() const
    {
        QPainterPath path;
        return path;
    }

    void IFitToolItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
    {
        BS_QT_Ui::CCustomMenu menu;
        menu.setLeaveCloseEnable(true);
        menu.addAction(tr("Del"), this, &IFitToolItem::sigDel);
        menu.exec(QCursor::pos());
    }
}

#include "fitCircleItem.h"
namespace TIGER_Graphics
{
    class CEllipseCenterFitTool : public IFitToolItem
    {
    public:
        CEllipseCenterFitTool(QGraphicsItem *parent = nullptr) : IFitToolItem(parent)
        {
            m_pFitCircleItem = new TIGER_Graphics::CFitCircleItem(this);
            this->addBasicItem(m_pFitCircleItem);
        }
        ~CEllipseCenterFitTool() = default;
        void setToolRect(QRectF p_rect) override
        {
            auto center = p_rect.center();
            m_pFitCircleItem->m_centerPt = center;
            m_pFitCircleItem->m_majorAxisPt = QPointF(p_rect.right(), center.y());
            m_pFitCircleItem->m_minorAxisLen = p_rect.height() * 0.5;
            m_pFitCircleItem->updatePtPos();
        }
        int fitType() const override
        {
            return EllipseCenterFitTool;
        }
        QVector<QVector<CFitRect>> fitRects() const override
        {
            QVector<QVector<CFitRect>> rects;
            rects << m_pFitCircleItem->fitRects();
            return rects;
        }

    protected:
        TIGER_Graphics::CFitCircleItem *m_pFitCircleItem = nullptr;
    };

    class CTwoCircleFitTool : public IFitToolItem
    {
    public:
        CTwoCircleFitTool(QGraphicsItem *parent = nullptr) : IFitToolItem(parent)
        {
            m_pFitCircleItem1 = new TIGER_Graphics::CFitCircleItem(this);
            m_pFitCircleItem1->setUsedMinorAxis(false);
            m_pFitCircleItem1->setFreeMinorAxis(false);
            m_pFitCircleItem1->m_centerPt = QPointF(-50, -50);
            m_pFitCircleItem1->m_majorAxisPt = QPointF(0, 0);
            m_pFitCircleItem2 = new TIGER_Graphics::CFitCircleItem(this);
            m_pFitCircleItem2->setUsedMinorAxis(false);
            m_pFitCircleItem2->setFreeMinorAxis(false);
            m_pFitCircleItem2->m_centerPt = QPointF(50, 50);
            m_pFitCircleItem2->m_majorAxisPt = QPointF(0, 0);
            this->addBasicItem(m_pFitCircleItem1);
            this->addBasicItem(m_pFitCircleItem2);
        }
        ~CTwoCircleFitTool() = default;

        void setToolRect(QRectF p_rect) override
        {
            auto offset = QPointF(p_rect.width() * 0.25, p_rect.height() * 0.25);
            auto center = p_rect.center();
            m_pFitCircleItem1->m_centerPt = center - offset;
            m_pFitCircleItem1->m_majorAxisPt = center - offset + QPointF(offset.x(), 0);
            m_pFitCircleItem1->updatePtPos();
            m_pFitCircleItem2->m_centerPt = center + offset;
            m_pFitCircleItem2->m_majorAxisPt = center + offset + QPointF(offset.x(), 0);
            m_pFitCircleItem2->updatePtPos();
        }
        int fitType() const override
        {
            return TwoCircleCenterFitTool;
        }
        QVector<QVector<CFitRect>> fitRects() const override
        {
            QVector<QVector<CFitRect>> rects;
            rects << m_pFitCircleItem1->fitRects();
            rects << m_pFitCircleItem2->fitRects();
            return rects;
        }
        QPainterPath toolDrawPath() const override
        {
            QPainterPath path;
            path.moveTo(m_pFitCircleItem1->m_centerPt);
            path.lineTo(m_pFitCircleItem2->m_centerPt);
            return path;
        }

    protected:
        TIGER_Graphics::CFitCircleItem *m_pFitCircleItem1 = nullptr;
        TIGER_Graphics::CFitCircleItem *m_pFitCircleItem2 = nullptr;
    };
}

#include "fitRectToolItem.h"
#include "fitRightAngleToolItem.h"
#include "fitLineCenterToolItem.h"
TIGER_Graphics::IFitToolItem *CGraphicsFitToolItemCreater::createToolItem(const TIGER_Graphics::IFitToolItem::CFitToolType &p_type)
{
    TIGER_Graphics::IFitToolItem *pTool = nullptr;
    switch (p_type)
    {
    case TIGER_Graphics::IFitToolItem::RectCenterFitTool:
        pTool = new TIGER_Graphics::CFitRectToolItem;
        break;
    case TIGER_Graphics::IFitToolItem::EllipseCenterFitTool:
        pTool = new TIGER_Graphics::CEllipseCenterFitTool;
        break;
    case TIGER_Graphics::IFitToolItem::TwoCircleCenterFitTool:
        pTool = new TIGER_Graphics::CTwoCircleFitTool;
        break;
    case TIGER_Graphics::IFitToolItem::LineCenterFitTool:
        pTool = new TIGER_Graphics::CFitLineCenterToolItem;
        break;
    case TIGER_Graphics::IFitToolItem::RightAngleFitTool:
        pTool = new TIGER_Graphics::CFitRightAngleToolItem;
        break;
    default:
        break;
    }
    assert(pTool != nullptr && pTool->fitType() == p_type);
    pTool->setToolTip(TIGER_Graphics::cFitToolStrMap[p_type]);
    return pTool;
}