#include "fitCircleItem.h"
#include "Ui/graphics/item/fixedItem/ctrlPtItem.h"
#include <QtMath>
#include <QPainterPathStroker>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
namespace TIGER_Graphics
{
#define cHeightDirOffsetScale 1.1
#define cMoveHeightOffsetScale 0.5
    using namespace BS_QT_Ui;
    CFitCircleItem::CFitCircleItem(QGraphicsItem *parent)
        : IBasicFitItem(parent)
    {
        setFlag(QGraphicsItem::ItemIsSelectable);
        m_pMovePtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pMovePtItem->ptItem()->setBrush(QBrush(Qt::red));
        m_pMovePtItem->ptItem()->setHoverCursor(QCursor(Qt::ClosedHandCursor));

        m_pExtendMajorPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pExtendMajorPtItem->ptItem()->setBrush(QBrush(Qt::red));
        m_pExtendMinorPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pExtendMinorPtItem->ptItem()->setBrush(QBrush(Qt::green));

        m_pRectHalfHeightPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pRectHalfHeightPtItem->ptItem()->setBrush(QBrush(Qt::blue));
        m_pRectHalfWidthPtItem = new CCtrlPtItem(0, 0, 5, this);
        m_pRectHalfWidthPtItem->ptItem()->setBrush(QBrush(Qt::blue));

        updatePtPos();

        auto funMousePress = [this](QPointF p_scenePos, int p_buttons)
        {
            m_latstPressPt = p_scenePos;
        };

        connect(m_pMovePtItem, &CCtrlPtItem::sigMouseMove, this, &CFitCircleItem::slotMove);
        connect(m_pMovePtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pMovePtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitCircleItem::sigPtPosChangeFinished);

        connect(m_pRectHalfHeightPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitCircleItem::slotHalHeightMove);
        connect(m_pRectHalfHeightPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pRectHalfHeightPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitCircleItem::sigPtPosChangeFinished);

        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitCircleItem::slotHalWidthMove);
        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pRectHalfWidthPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitCircleItem::sigPtPosChangeFinished);

        connect(m_pExtendMajorPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitCircleItem::slotMajorExtendMove);
        connect(m_pExtendMajorPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pExtendMajorPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitCircleItem::sigPtPosChangeFinished);

        connect(m_pExtendMinorPtItem, &CCtrlPtItem::sigMouseMove, this, &CFitCircleItem::slotMinorExtendMove);
        connect(m_pExtendMinorPtItem, &CCtrlPtItem::sigMousePress, this, funMousePress);
        connect(m_pExtendMinorPtItem, &CCtrlPtItem::sigMouseRelease, this, &CFitCircleItem::sigPtPosChangeFinished);
    }

    CFitCircleItem::~CFitCircleItem()
    {
    }

    QPainterPath CFitCircleItem::drawCirclePath() const
    {
        QPainterPath path;
        path.addEllipse(QPointF(0, 0), m_majorAxisLen, m_minorAxisLen);
        QTransform transform;
        transform.rotate(-QLineF(QPointF{0, 0}, m_lineDir).angle());
        path = this->mapFromScene(transform.map(path).translated(m_centerPt.x(), m_centerPt.y()));
        return path;
    }

    QPainterPath CFitCircleItem::shape() const
    {
        QPainterPathStroker stroker(Qt::NoPen);
        stroker.setWidth(0);
        stroker.setJoinStyle(Qt::MiterJoin);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setDashPattern(Qt::SolidLine);
        QPainterPath path = stroker.createStroke(drawCirclePath());
        path.setFillRule(Qt::FillRule::WindingFill);
        return path;
    }

    void CFitCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(drawCirclePath());

            painter->setBrush(Qt::green);
            painter->drawPath(getArrowPath(QLineF(m_centerPt, this->mapToScene(m_pExtendMinorPtItem->pos()))));
        }

        {
            painter->setPen(QPen(Qt::red, 0));
            painter->setBrush(Qt::red);
            painter->drawPath(getArrowPath(QLineF(m_centerPt, this->mapToScene(m_pExtendMajorPtItem->pos()))));
        }

        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::blue, 0));

        QPainterPath rects;
        for (auto rect : fitRects())
        {
            rects.addPolygon(rect.toPolygon());
        }
        painter->drawPath(this->mapFromScene(rects));
        painter->restore();
    }

    void CFitCircleItem::updatePtPos()
    {
        auto itemLine = QLineF(m_centerPt, m_majorAxisPt);

        auto nor = itemLine.normalVector().unitVector();
        auto dir = itemLine.unitVector();
        m_lineDir = QPointF(dir.dx(), dir.dy());
        m_lineNor = QPointF(nor.dx(), nor.dy());
        m_majorAxisLen = itemLine.length();

        if (!m_usedMinorAxis)
        {
            m_minorAxisLen = itemLine.length();
        }

        // Ramanujan第二公式
        auto caluLen = [](double p_a, double p_b)
        {
            auto ab = p_a + p_b;
            auto h_3 = 3 * (p_a - p_b) * (p_a - p_b) / (ab * ab);
            return M_PI * ab * (1 + h_3 / (10 + std::sqrt(4 - h_3)));
        };

        m_fitRectInterval = caluLen(m_majorAxisLen, m_minorAxisLen) / m_fitRectCount;
        m_pRectHalfHeightPtItem->setPos(this->mapFromScene(m_centerPt + m_lineNor * (m_minorAxisLen - m_fitRectHalHeight)));
        m_pRectHalfWidthPtItem->setPos(this->mapFromScene(m_centerPt + m_lineNor * m_minorAxisLen + m_lineDir * m_fitRectHalWidth));
        m_pMovePtItem->setPos(this->mapFromScene(m_centerPt));
        m_pExtendMajorPtItem->setPos(this->mapFromScene(m_centerPt + m_lineDir * (m_majorAxisLen + m_fitRectHalHeight * cHeightDirOffsetScale)));
        m_pExtendMinorPtItem->setPos(this->mapFromScene(m_centerPt + m_lineNor * (m_minorAxisLen + m_fitRectHalHeight * cHeightDirOffsetScale)));

        // 刷新 m_fitRects
        {
            m_fitRects.clear();
            CFitRect info(0.0, 0.0, m_fitRectHalWidth * 2, m_fitRectHalHeight * 2);

            auto angleStep = 360.0 / m_fitRectCount;
            for (int i = 0; i < m_fitRectCount; i++)
            {
                auto angleRad = qDegreesToRadians(i * angleStep + 90);
                auto pt = m_centerPt + m_lineDir * m_majorAxisLen * std::cos(angleRad) + m_lineNor * m_minorAxisLen * std::sin(angleRad);
                info.angle = -90 - QLineF(m_centerPt, pt).angle();
                m_fitRects << info.moveCenter(pt);
            }
        }

        // 刷新 aabb
        {
            QPainterPath rects;
            rects.addEllipse(m_centerPt, m_majorAxisLen, m_minorAxisLen);
            rects.moveTo(m_pExtendMinorPtItem->pos());
            rects.lineTo(m_pExtendMajorPtItem->pos());
            for (const auto &rect : m_fitRects)
            {
                rects.addPolygon(rect.toPolygon());
            }
            m_aabb = rects.boundingRect();
        }
        emit sigPtPosChanged();
    }

    void CFitCircleItem::setRectCount(int p_count)
    {
        auto norCount = std::min(3, p_count);
        if (m_fitRectCount != norCount)
        {
            m_fitRectCount = norCount;
            updatePtPos();
        }
    }
    void CFitCircleItem::setUsedMinorAxis(bool p_used)
    {
        if (m_usedMinorAxis != p_used)
        {
            m_usedMinorAxis = p_used;
            setAdjustPtFlag(ExtendMinorAxisAdjustPt, m_usedMinorAxis);
        }
    }

    void CFitCircleItem::setFreeMinorAxis(bool p_free)
    {
        if (m_freeMajorAxis != p_free)
        {
            m_freeMajorAxis = p_free;
        }
    }

    void CFitCircleItem::setAdjustPtFlag(CAdjustPtFlag p_flag, bool p_on)
    {
        setAdjustPtFlags(p_on ? (m_ctrlPtFlags | p_flag) : (m_ctrlPtFlags & (~p_flag)));
    }

    void CFitCircleItem::setAdjustPtFlags(CAdjustPtFlags p_flags)
    {
        if (m_ctrlPtFlags != p_flags)
        {
            m_ctrlPtFlags = p_flags;
            const auto &cItemMap = QMap<CAdjustPtFlag, CCtrlPtItem *>{
                {{MoveAdjustPt, m_pMovePtItem},
                 {ExtendMajorAxisAdjustPt, m_pExtendMajorPtItem},
                 {ExtendMinorAxisAdjustPt, m_pExtendMinorPtItem},
                 {FitRectHeightAdjustPt, m_pRectHalfHeightPtItem},
                 {FitRectWidthAdjustPt, m_pRectHalfWidthPtItem}}};
            for (auto flag : cItemMap.keys())
            {
                cItemMap[flag]->setVisible(m_ctrlPtFlags.testFlag(flag));
            }
        }
    }

    void CFitCircleItem::slotMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = p_scenePos - m_latstPressPt;
        m_centerPt += add;
        m_majorAxisPt += add;
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitCircleItem::slotHalHeightMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = QPointF::dotProduct((m_centerPt + m_lineNor * m_minorAxisLen) - p_scenePos, m_lineNor);
        m_fitRectHalHeight = std::max(1.0, add);
        m_fitRectHalHeight = std::min(m_fitRectHalHeight, m_minorAxisLen);
        m_fitRectHalHeight = std::min(m_fitRectHalHeight, m_majorAxisLen);
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitCircleItem::slotHalWidthMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = QPointF::dotProduct(p_scenePos - m_centerPt, m_lineDir);
        m_fitRectHalWidth = std::max(add, 0.0);
        m_fitRectHalWidth = std::min(m_fitRectHalWidth, m_fitRectInterval / 2);
        m_fitRectHalWidth = std::max(1.0, m_fitRectHalWidth);
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitCircleItem::slotMajorExtendMove(QPointF p_scenePos, int p_buttons)
    {
        auto formatPt = p_scenePos - m_lineDir * m_fitRectHalHeight * cHeightDirOffsetScale;
        if (m_freeMajorAxis)
        {
            if (QLineF(m_centerPt, formatPt).length() > m_fitRectHalHeight)
            {
                m_majorAxisPt = formatPt;
            }
        }
        else
        {
            auto add = QPointF::dotProduct(formatPt - m_centerPt, m_lineDir);
            m_majorAxisPt = std::max(m_fitRectHalHeight, add) * m_lineDir + m_centerPt;
        }
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }

    void CFitCircleItem::slotMinorExtendMove(QPointF p_scenePos, int p_buttons)
    {
        auto add = QPointF::dotProduct(p_scenePos - m_centerPt - m_lineNor * m_fitRectHalHeight * cHeightDirOffsetScale, m_lineNor);
        m_minorAxisLen = std::max(m_fitRectHalHeight, add);
        m_latstPressPt = p_scenePos;
        updatePtPos();
    }
}
#undef cHeightDirOffsetScale
#undef cMoveHeightOffsetScale