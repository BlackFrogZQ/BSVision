#pragma once
#include "fitDef.h"
#include "template/shapeTemplate/shapeResDef.h"
#include <QGraphicsItemGroup>
namespace TIGER_Graphics
{
    class IFitToolItem : public QObject, public QGraphicsItemGroup
    {
        Q_OBJECT
    public:
        enum CFitToolType : int
        {
            TemplateCenterFitTool = 0,
            RectCenterFitTool,
            EllipseCenterFitTool,
            TwoCircleCenterFitTool,
            LineCenterFitTool,
            RightAngleFitTool
        };
        Q_ENUM(CFitToolType)
    public:
        IFitToolItem(QGraphicsItem *parent = nullptr);
        virtual ~IFitToolItem() = default;
        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

        virtual int fitType() const = 0;
        virtual QVector<QVector<CFitRect>> fitRects() const = 0;
        QPointF fitPos() const;

    public slots:
        virtual void setToolRect(QRectF p_rect) = 0;
        void setFitPos(QPointF p);
        void setFitPos(double px, double py) { setFitPos(QPointF(px, py)); };

    signals:
        void sigFitRectChanged();
        void sigDel();

    protected:
        void addBasicItem(IBasicFitItem *pItem);
        virtual QPainterPath toolDrawPath() const;
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    protected:
        static QPainterPath crossPath(QPointF p);

    protected:
        QPointF m_fitPos;
    };

    const QMap<int, QString> cFitToolStrMap =
        {
            {IFitToolItem::TemplateCenterFitTool, QObject::tr("Template Center")},
            {IFitToolItem::RectCenterFitTool, QObject::tr("Rect Center")},
            {IFitToolItem::EllipseCenterFitTool, QObject::tr("Ellipse Center")},
            {IFitToolItem::TwoCircleCenterFitTool, QObject::tr("Two Circle Center")},
            {IFitToolItem::LineCenterFitTool, QObject::tr("Line Center")},
            {IFitToolItem::RightAngleFitTool, QObject::tr("Right Angle")}};
    const QMap<int, QString> cFitToolIconMap =
        {
            {IFitToolItem::TemplateCenterFitTool, cPosToolTemplateCenterIcon},
            {IFitToolItem::RectCenterFitTool, cPosToolRectCenterIcon},
            {IFitToolItem::EllipseCenterFitTool, cPosToolEllipseCenterIcon},
            {IFitToolItem::TwoCircleCenterFitTool, cPosToolTwoCircleCenterIcon},
            {IFitToolItem::LineCenterFitTool, cPosToolLineCenterIcon},
            {IFitToolItem::RightAngleFitTool, cPosToolRightAngleCenterIcon}};
}

class CGraphicsFitToolItemCreater
{
public:
    static TIGER_Graphics::IFitToolItem *createToolItem(const TIGER_Graphics::IFitToolItem::CFitToolType &p_type);
};