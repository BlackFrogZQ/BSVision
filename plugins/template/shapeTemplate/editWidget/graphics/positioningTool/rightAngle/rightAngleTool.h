#pragma once
#include "../positioningTool.h"
namespace TIGER_Graphics
{
    class CRightAngleItem;
    class CRightAngleTool : public IPositioningToolItem
    {
    public:
        CRightAngleTool(QGraphicsItem *parent = nullptr);
        virtual QPointF getPosition() override;
        virtual QList<QPainterPath> getToolPaths() const override;
        virtual void setShapeSize(QPointF center, double w, double h) override;

    protected:
        virtual QPainterPath drawShape() const override;

    protected:
        CRightAngleItem *m_pRightAngleItem;
    };
}