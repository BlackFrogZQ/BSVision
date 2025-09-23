#pragma once
#include "Ui/graphics/item/iItem.h"
namespace TIGER_Graphics
{
    class CLineCenterItem : public BS_QT_Ui::IGraphicsItem
    {
    public:
        using BS_QT_Ui::IGraphicsItem::IGraphicsItem;
        enum CKeyPoint : uint8_t
        {
            ckptVertical,
            ckptRightAngle,
            ckptRightAngle2,
            ckptVertical2,
            ckptMax
        };
        virtual int getType() const { return CItemDef::cgitTool; };
        virtual bool isDrawFinished() const override;
        virtual QPainterPath drawShape() const override;
    };
}