#pragma once
#include "../positioningTool.h"
namespace TIGER_Graphics
{
    class CTwoCircleTool : public IPositioningToolItem
    {
    public:
        CTwoCircleTool(QGraphicsItem *parent = nullptr);
        virtual QPointF getPosition() override;
        virtual void setShapeSize(QPointF center, double w, double h) override;
    protected:
        virtual QPainterPath drawShape() const override;
    };
}