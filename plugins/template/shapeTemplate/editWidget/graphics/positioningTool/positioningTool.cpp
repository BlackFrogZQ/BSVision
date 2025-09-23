#include "positioningTool.h"

#include "Ui/graphics/item/itemContextMenu.h"
#include "Ui/graphics/item/rect/rectItem.h"
#include "Ui/graphics/item/ellipse/ellipseItem.h"
#include "Ui/inputDialog/inputDialog.h"
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
namespace TIGER_Graphics
{
    using namespace BS_QT_Ui;
    IPositioningToolItem::IPositioningToolItem(QGraphicsItem *parent)
        : QGraphicsItemGroup(parent),
          m_pContextMenu(nullptr),
          m_selectable(true),
          m_dx(0),
          m_dy(0)
    {
        this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemContainsChildrenInShape);
        this->setHandlesChildEvents(false);
        m_pContextMenu = new CItemContextMenu(this);
        m_pContextMenu->setActionFlags(CItemContextMenu::catfRemove);
        m_pOffsetAction = new QAction(QObject::tr("偏移定位点"));
        connect(
            m_pOffsetAction, &QAction::triggered, this, [this]
            {
                    const CInputDoublePara cParasX(QObject::tr("X偏移"),m_dx,-30000.0,30000.0,1.0,1);
                    const CInputDoublePara cParasY(QObject::tr("Y偏移"),-m_dy,-30000.0,30000.0,1.0,1);
                    CCustomInputDouble2Dialog double2Dialog(QObject::tr("定位点偏移"),cParasX,cParasY);
                    double dx=m_dx;
                    double dy=m_dy;
                    connect(&double2Dialog,&CCustomInputDouble2Dialog::sigValueChanged,this,[this](double p_dx,double p_dy)
                    {
                        m_dx = p_dx;
                        m_dy = -p_dy;
                        emit sigGroupShapeOffset(m_dx,m_dy);
                    });
                    if (double2Dialog.getValue(m_dx,m_dy))
                    {
                        m_dy = -m_dy;
                    }
                    else
                    {
                        m_dx = dx;
                        m_dy = dy;
                    }
                    emit sigGroupShapeOffset(m_dx,m_dy); },
            Qt::QueuedConnection);
    }

    IPositioningToolItem::~IPositioningToolItem()
    {
        delPtr(m_pContextMenu);
        delPtr(m_pOffsetAction);
    }

    void IPositioningToolItem::setEnableOffset(bool p_enable)
    {
        m_pOffsetAction->setEnabled(p_enable);
    }

    QRectF IPositioningToolItem::boundingRect() const
    {
        QPainterPath path;
        if (this->isEnabled() && flags().testFlag(QGraphicsItem::ItemIsSelectable))
        {
            path.addPath(getCrossPosition());
            path.addPath(drawShape());
            path.addRect(this->childrenBoundingRect());
        }
        return path.boundingRect();
    }

    QPainterPath IPositioningToolItem::shape() const
    {
        QPainterPath path;
        path.addRect(this->childrenBoundingRect());
        return path;
    }

    void IPositioningToolItem::updatePath()
    {
        this->prepareGeometryChange();
        m_path = QPainterPath();
        for (auto p : m_child)
        {
            m_path.addPath(p->mapToScene(p->drawShape()));
        }
        m_position = getPosition() + QPointF(m_dx, m_dy);
        boundingRect();
        m_path.addPath(getCrossPosition());
        m_path.addPath(drawShape());
    }
    void IPositioningToolItem::setOffset(double p_dx, double p_dy)
    {
        m_dx = p_dx;
        m_dy = p_dy;
        update();
    }
    void IPositioningToolItem::getOffset(double &p_dx, double &p_dy) const
    {
        p_dx = m_dx;
        p_dy = m_dy;
    }

    void IPositioningToolItem::setSelectable(bool p_isSelectable)
    {
        m_selectable = p_isSelectable;
        auto flagsVar = flags();
        setFlags(p_isSelectable ? (flagsVar | QGraphicsItem::ItemIsSelectable) : (flagsVar & ~QGraphicsItem::ItemIsSelectable));
        setSelected(p_isSelectable);
        unsetCursor();
    }

    QPainterPath IPositioningToolItem::drawShape() const
    {
        return QPainterPath();
    }

    void IPositioningToolItem::paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget)
    {
        updatePath();
        for (auto p : m_child)
        {
            // Draw the item
            p_painter->save();
            p_painter->setTransform(p->sceneTransform(), true);
            p->paint(p_painter, p_option, p_widget);
            p_painter->restore();
        }
        p_painter->save();
        p_painter->setRenderHint(QPainter::Antialiasing, true);
        p_painter->setBrush(Qt::NoBrush);
        p_painter->setPen(QPen(Qt::red, this->isSelected() ? 1 : 0, this->isSelected() ? Qt::SolidLine : Qt::DashLine));
        p_painter->drawPath(drawShape());
        p_painter->setPen(QPen(Qt::green, 1, Qt::SolidLine));
        p_painter->drawPath(getCrossPosition());
        p_painter->restore();
    }

    void IPositioningToolItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *p_event)
    {
        if (m_selectable)
        {
            this->setSelected(true);
            QMenu *pMenu = m_pContextMenu->getMenu();
            if (m_pOffsetAction->isEnabled())
            {
                pMenu->addAction(m_pOffsetAction);
            }
            pMenu->exec(QCursor::pos());
            delPtr(pMenu);
        }
        else
        {
            QGraphicsItemGroup::contextMenuEvent(p_event);
        }
    }

    QPointF IPositioningToolItem::getPosition()
    {
        QPainterPath path;
        for (auto p : m_child)
        {
            path.addPath(p->mapToScene(p->drawShape()));
        }
        return path.boundingRect().center();
    }

    QPainterPath IPositioningToolItem::getToolShape() const
    {
        return m_path;
    }

    void IPositioningToolItem::setLineWidth(int p_w)
    {
        for (auto child : m_child)
        {
            child->setLineWidth(p_w);
        }
    }
    void IPositioningToolItem::updateKeyPointRect()
    {
        for (auto child : m_child)
        {
            child->updateKeyPointRect();
        }
    }

    QList<QPainterPath> IPositioningToolItem::getToolPaths() const
    {
        assert(m_child.size() > 0);
        QList<QPainterPath> paths;
        for (auto child : m_child)
        {
            paths.append(child->mapToScene(child->drawShape()));
        }
        return paths;
    }

    void IPositioningToolItem::add(IGraphicsItem *item)
    {
        assert(item != nullptr);
        QGraphicsItemGroup::addToGroup(item);
        item->setContextMenuFlags(CItemContextMenu::CActionTypeFlag::None);
        item->setZValue(m_child.size());
        m_child.append(item);
        connect(item, &IGraphicsItem::sigShapeChanged, this, &IPositioningToolItem::sigGroupShapeChanged);
        emit sigGroupShapeChanged();
    }

    void IPositioningToolItem::remove(IGraphicsItem *item)
    {
        assert(item != nullptr);
        if (m_child.contains(item))
        {
            QGraphicsItemGroup::removeFromGroup(item);
            m_child.removeOne(item);
            delPtr(item);
            emit sigGroupShapeChanged();
        }
    }

    QPainterPath IPositioningToolItem::getCrossPosition() const
    {
        const double cLen = 5;
        QPainterPath path;
        path.moveTo(-cLen, -cLen);
        path.lineTo(cLen, cLen);
        path.moveTo(-cLen, cLen);
        path.lineTo(cLen, -cLen);
        path.translate(m_position);
        return path;
    }
};

namespace TIGER_Graphics
{
    class CRectCenterTool : public IPositioningToolItem
    {
    public:
        CRectCenterTool(QGraphicsItem *parent = nullptr) : IPositioningToolItem(parent)
        {
            m_pRectItem = new TIGER_Graphics::CRectItem;
            m_pRectItem->addKeyPoints({{0, 0}, {30, 30}});
            this->add(m_pRectItem);
        }
        virtual void setShapeSize(QPointF center, double w, double h) override
        {
            auto points = m_pRectItem->getKeyPoints();
            QRectF rect(0, 0, w, h);
            rect.moveCenter(center);
            m_pRectItem->setKeyPoint(CRectItem::ckptTopLfet, rect.topLeft());
            m_pRectItem->setKeyPoint(CRectItem::ckptBottomRight, rect.bottomRight());
        }

    protected:
        TIGER_Graphics::CRectItem *m_pRectItem;
    };
    class CEllipseCenterFitTool : public IPositioningToolItem
    {
    public:
        CEllipseCenterFitTool(QGraphicsItem *parent = nullptr) : IPositioningToolItem(parent)
        {
            m_pEllipseItem = new TIGER_Graphics::CEllipseItem;
            m_pEllipseItem->addKeyPoints({{0, 0}, {30, 30}});
            this->add(m_pEllipseItem);
        }

        virtual void setShapeSize(QPointF center, double w, double h) override
        {
            m_pEllipseItem->setKeyPoint(CEllipseItem::ckptCenter, center);
            m_pEllipseItem->setKeyPoint(CEllipseItem::ckptTopRight, center + QPointF(w * 0.5, h * 0.5));
        }

    protected:
        TIGER_Graphics::CEllipseItem *m_pEllipseItem;
    };
};

#include "twoCircle/twoCircleTool.h"
#include "lineCenter/lineCenterTool.h"
#include "rightAngle/rightAngleTool.h"
TIGER_Graphics::IPositioningToolItem *CGraphicsToolItemCreater::createGraphicsToolItem(const TIGER_Graphics::CPosToolType &p_type)
{
    TIGER_Graphics::IPositioningToolItem *pTool = nullptr;
    switch (p_type)
    {
    case TIGER_Graphics::RectCenterPosTool:
        pTool = new TIGER_Graphics::CRectCenterTool;
        pTool->setEnableOffset(false);
        break;
    case TIGER_Graphics::EllipseCenterPosTool:
        pTool = new TIGER_Graphics::CEllipseCenterFitTool;
        pTool->setEnableOffset(false);
        break;
    case TIGER_Graphics::TwoCircleCenterPosTool:
        pTool = new TIGER_Graphics::CTwoCircleTool;
        break;
    case TIGER_Graphics::LineCenterPosTool:
        pTool = new TIGER_Graphics::CLineCenterTool;
        break;
    case TIGER_Graphics::RightAnglePosTool:
        pTool = new TIGER_Graphics::CRightAngleTool;
        break;
    default:
        break;
    }
    assert(pTool != nullptr);
    pTool->setToolTip(TIGER_Graphics::cPosToolStrMap[p_type]);
    return pTool;
}