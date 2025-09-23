#pragma once
#include "Ui/graphics/item/iItem.h"
namespace TIGER_Graphics
{
    class CRightAngleItem : public BS_QT_Ui::IGraphicsItem
    {
    public:
        using BS_QT_Ui::IGraphicsItem::IGraphicsItem;
        enum CKeyPoint : uint8_t
        {
            ckptVertical,
            ckptRightAngle,
            ckptHorizontal,
            ckptMax
        };
        virtual int getType() const { return CItemDef::cgitTool; };
        virtual bool isDrawFinished() const override;
        virtual QPainterPath drawShape() const override;
    };
}