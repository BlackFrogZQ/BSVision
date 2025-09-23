#pragma once
#include "../positioningTool.h"
namespace TIGER_Graphics
{
    class CLineCenterItem;
    class CLineCenterTool : public IPositioningToolItem
    {
    public:
        CLineCenterTool(QGraphicsItem *parent = nullptr);
        virtual QPointF getPosition() override;
        virtual QList<QPainterPath> getToolPaths() const override;
        virtual void setShapeSize(QPointF center,double w, double h) override;
    protected:
        virtual QPainterPath drawShape() const override;
    protected:
        CLineCenterItem* m_pLineCenterItem;
    };
}