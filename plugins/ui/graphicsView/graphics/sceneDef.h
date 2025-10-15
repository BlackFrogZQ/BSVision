#pragma once
#include "system/basic.h"
#include <QObject>
class CSceneDef
{
    Q_GADGET
public:
    enum CDrawType : uint8_t
    {
        cltUnited,
        cltIntersected,
        cltSubtracted,
    };
    Q_ENUM(CDrawType)
};
#define cDrawTypeStr (QStringList() << QObject::tr("联合") << QObject::tr("相交") << QObject::tr("做差"))

const QStringList cDrawTypePixmap = {
    ":/res/graphics/booleanOperation/union.png",
    ":/res/graphics/booleanOperation/intersection.png",
    ":/res/graphics/booleanOperation/subtraction.png"};
class QGraphicsItem;
namespace BS_QT_Ui
{
    class IGraphicsBasicItem;
    class IGraphicsItem;
}
namespace TIGER_Graphics
{
    template <class Item>
    class IGraphicsShape
    {
    public:
        CSceneDef::CDrawType type;
        Item *pItem;
        IGraphicsShape<Item>(Item *p_pItem = nullptr, const CSceneDef::CDrawType &p_type = CSceneDef::cltUnited)
            : pItem(p_pItem), type(p_type){};
        // template <typename ItemType, typename = typename std::enable_if<std::is_same<ItemType, Item>::value, ItemType>::type>
        // IGraphicsShape<Item>(ItemType *p_pItem, const CSceneDef::CDrawType &p_type)
        //     : pItem(p_pItem), type(p_type){};
        void clear()
        {
            pItem = nullptr;
            type = CSceneDef::cltUnited;
        }
        bool operator==(const IGraphicsShape<Item> &t)
        {
            return pItem == t.pItem && type == t.type;
        }
        // template <typename ItemType, typename = typename std::enable_if<std::is_same<ItemType, Item>::value, ItemType>::type>
        IGraphicsShape<BS_QT_Ui::IGraphicsBasicItem> toBasicShape() const
        {
            static_assert(std::is_same<BS_QT_Ui::IGraphicsBasicItem, Item>::value || std::is_same<BS_QT_Ui::IGraphicsItem, Item>::value, "IGraphicsShape type error");
            return IGraphicsShape<BS_QT_Ui::IGraphicsBasicItem>(this->pItem, this->type);
        }
    };
    using CGraphicsShape = IGraphicsShape<BS_QT_Ui::IGraphicsItem>;
    using CGraphicsBasicShape = IGraphicsShape<BS_QT_Ui::IGraphicsBasicItem>;
};
class QGraphicsItem;
QGraphicsItem *parentItem(QGraphicsItem *p_pItem);