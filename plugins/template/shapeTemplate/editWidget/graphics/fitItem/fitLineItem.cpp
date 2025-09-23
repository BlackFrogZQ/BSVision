#include "fitLineItem.h"
#include "Ui/graphics/item/fixedItem/ctrlPtItem.h"
#include "solid/AABB.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
namespace TIGER_Graphics
{
#define cHeightDirOffsetScale 1.1
#define cMoveHeightOffsetScale 0.5
    using namespace BS_QT_Ui;
    CFitLineItem::CFitLineItem(QGraphicsItem *parent)
        : IBasicFitItem(parent)
    {
        setFlag(QGraphicsItem::ItemIsSelectable);
        m_pMovePtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pMovePtItem->ptItem()->setHoverCursor(QCursor(Qt::ClosedHandCursor));
        m_pMovePtItem->ptItem()->setBrush(QBrush(Qt::red));
        m_pExtendPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pExtendPtItem->ptItem()->setBrush(QBrush(Qt::red));

        m_pRectHalfHeightPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pRectHalfHeightPtItem->ptItem()->setBrush(QBrush(Qt::green));

        m_pRectHalfWidthPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pRectHalfWidthPtItem->ptItem()->setBrush(QBrush(Qt::blue));

        updatePtPos();

        auto funMousePress = [this](QPointF p_scenePos, int p_buttons)
        {
            m_latstPressPt = p_scenePos;
        };

        connect(m_pMovePtItem, &CCtrlPtItem::sigMouseMove, this, &CFitLineItem::slotMove);
        connect(m_pMovePtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pExtendPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitLineItem::sigPtPosChangeFinished);

        connect(m_pRectHalfHeightPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitLineItem::slotHalHeightMove);
        connect(m_pRectHalfHeightPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pExtendPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitLineItem::sigPtPosChangeFinished);

        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitLineItem::slotHalWidthMove);
        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitLineItem::sigPtPosChangeFinished);

        connect(m_pExtendPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitLineItem::slotExtendMove);
        connect(m_pExtendPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pExtendPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitLineItem::sigPtPosChangeFinished);
    }

    CFitLineItem::~CFitLineItem()
    {
    }

    CFitLineItem::CAdjustPtFlags CFitLineItem::ctrlPtFlags() const
    {
        return m_ctrlPtFlags;
    }

    QRectF CFitLineItem::boundingRect() const
    {
        return m_aabb;
    }

    QPainterPath CFitLineItem::shape() const
    {
        QPainterPath path;
        path.moveTo(this->mapFromScene(m_startPt));
        path.lineTo(this->mapFromScene(m_endPt));
        return path;
    }

    void CFitLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        auto getArrowPath = [this](const QLineF &p_sceneLine)
        {
            auto arrowPath = QPainterPath();
            auto nor = p_sceneLine.normalVector().unitVector();
            auto dir = p_sceneLine.unitVector();
            auto dirPt = QPointF(dir.dx(), dir.dy());
            auto norPt = QPointF(nor.dx(), nor.dy());
            auto drawNorEnd = p_sceneLine.p2();
            auto drawNorArrowStart = drawNorEnd - dirPt * m_fitRectHalHeight * 0.1 * 0.7;
            auto drawNorArrow1 = drawNorArrowStart - norPt * m_fitRectHalHeight * 0.1 * 0.15;
            auto drawNorArrow2 = drawNorArrowStart + norPt * m_fitRectHalHeight * 0.1 * 0.15;

            arrowPath.moveTo(this->mapFromScene(p_sceneLine.p1()));
            arrowPath.lineTo(this->mapFromScene(drawNorEnd));

            arrowPath.moveTo(this->mapFromScene(drawNorEnd));
            arrowPath.lineTo(this->mapFromScene(drawNorArrow1));
            arrowPath.lineTo(this->mapFromScene(drawNorArrow2));
            arrowPath.closeSubpath();
            return arrowPath;
        };

        painter->save();
        {
            painter->setPen(QPen(Qt::green, 0));
            painter->setBrush(Qt::green);
            painter->drawPath(getArrowPath(QLineF((m_startPt + m_endPt) / 2, this->mapToScene(m_pRectHalfHeightPtItem->pos()))));
        }

        {
            painter->setPen(QPen(Qt::red, 0));
            painter->setBrush(Qt::red);
            painter->drawPath(getArrowPath(QLineF(m_startPt, this->mapToScene(m_pExtendPtItem->pos()))));
        }

        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::blue, 0));
        QPainterPath rects;
        for (const auto &rect : m_fitRects)
        {
            rects.addPolygon(rect.toPolygon());
        }
        painter->drawPath(this->mapFromScene(rects));
        painter->restore();
    }

    void CFitLineItem::setFreeEndPt(bool p_free)
    {
        if (m_freeEndPt != p_free)
        {
            m_freeEndPt = p_free;
        }
    }

    void CFitLineItem::setReverseNormal(bool p_reverse)
    {
        if (m_reverseNormal != p_reverse)
        {
            m_reverseNormal = p_reverse;
            updatePtPos();
        }
    }

    void CFitLineItem::setHalfRectCount(int p_count)
    {
        auto norCount = std::min(1, p_count);
        if (m_fitRectHalfCount != norCount)
        {
            m_fitRectHalfCount = norCount;
            updatePtPos();
        }
    }

    void CFitLineItem::setAdjustPtFlag(CAdjustPtFlag p_flag, bool p_on)
    {
        setAdjustPtFlags(p_on ? (m_ctrlPtFlags | p_flag) : (m_ctrlPtFlags & (~p_flag)));
    }

    void CFitLineItem::setAdjustPtFlags(CAdjustPtFlags p_flags)
    {
        if (m_ctrlPtFlags != p_flags)
        {
            m_ctrlPtFlags = p_flags;
            const auto &cItemMap = QMap<CAdjustPtFlag, CCtrlPtItem *>{
                {{MoveAdjustPt, m_pMovePtItem},
                 {ExtendAdjustPt, m_pExtendPtItem},
                 {FitRectHeightAdjustPt, m_pRectHalfHeightPtItem},
                 {FitRectWidthAdjustPt, m_pRectHalfWidthPtItem}}};
            for (auto flag : cItemMap.keys())
            {
                cItemMap[flag]->setVisible(m_ctrlPtFlags.testFlag(flag));
            }
        }
    }

    void CFitLineItem::updatePtPos()
    {
        QPainterPath aabbPath;
        auto itemLine = QLineF(m_startPt, m_endPt);

        auto nor = itemLine.normalVector().unitVector();
        auto dir = itemLine.unitVector();
        m_lineDir = QPointF(dir.dx(), dir.dy());
        m_lineNor = (m_reverseNormal ? -1 : 1) * QPointF(nor.dx(), nor.dy());
        m_lineAngle = itemLine.angle();

        m_fitRectInterval = itemLine.length() / 2 / m_fitRectHalfCount;
        m_pMovePtItem->setPos(this->mapFromScene(itemLine.center() - m_lineNor * m_fitRectHalHeight * cMoveHeightOffsetScale));
        m_pRectHalfHeightPtItem->setPos(this->mapFromScene(itemLine.center() + m_lineNor * m_fitRectHalHeight * cHeightDirOffsetScale));
        m_pRectHalfWidthPtItem->setPos(this->mapFromScene(itemLine.center() + m_lineNor * m_fitRectHalHeight * cMoveHeightOffsetScale + m_lineDir * m_fitRectHalWidth));
        m_pExtendPtItem->setPos(this->mapFromScene(m_endPt + m_lineDir * 2 * m_fitRectHalWidth));

        // 刷新 m_fitRects
        {
            m_fitRects.clear();
            auto step = m_fitRectInterval * m_lineDir;
            auto itemCenter = (m_startPt + m_endPt) * 0.5;
            // y向下 需要+180
            CFitRect info(0.0, 0.0, m_fitRectHalWidth * 2, m_fitRectHalHeight * 2, -m_lineAngle + (m_reverseNormal ? 0 : 180));
            for (int i = 0; i <= m_fitRectHalfCount; i++)
            {
                m_fitRects << info.moveCenter(itemCenter + i * step);
                m_fitRects << info.moveCenter(itemCenter - i * step);
            }
        }

        // 刷新 aabb
        {
            QPainterPath rects;
            rects.moveTo(m_pRectHalfHeightPtItem->pos());
            rects.lineTo(m_pExtendPtItem->pos());
            for (const auto &rect : m_fitRects)
            {
                rects.addPolygon(rect.toPolygon());
            }
            m_aabb = rects.boundingRect();
        }
        emit sigPtPosChanged();
    }

    void CFitLineItem::slotMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = p_scenePos - m_latstPressPt;
        m_startPt += add;
        m_endPt += add;
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitLineItem::slotHalHeightMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = QPointF::dotProduct(p_scenePos - (m_startPt + m_endPt) * 0.5, m_lineNor);
        m_fitRectHalHeight = std::max(1.0, add / cHeightDirOffsetScale);
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitLineItem::slotHalWidthMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = QPointF::dotProduct(p_scenePos - (m_startPt + m_endPt) * 0.5, m_lineDir);
        m_fitRectHalWidth = add;
        m_fitRectHalWidth = std::min(m_fitRectHalWidth, m_fitRectInterval / 2);
        m_fitRectHalWidth = std::max(1.0, m_fitRectHalWidth);
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitLineItem::slotExtendMove(QPointF p_scenePos, int p_buttons)
    {
        auto formatPt = p_scenePos - m_lineDir * 2 * m_fitRectHalWidth;
        if (m_freeEndPt)
        {
            if (QLineF(m_startPt, formatPt).length() > m_fitRectHalWidth * 2 * m_fitRectHalfCount * 2)
            {
                m_endPt = formatPt;
            }
        }
        else
        {
            auto add = QPointF::dotProduct(formatPt - m_startPt, m_lineDir);
            m_endPt = std::max(m_fitRectHalWidth * 2 * m_fitRectHalfCount * 2, add) * m_lineDir + m_startPt;
        }

        m_latstPressPt = p_scenePos;
        updatePtPos();
    }
}
#undef cHeightDirOffsetScale
#undef cMoveHeightOffsetScale