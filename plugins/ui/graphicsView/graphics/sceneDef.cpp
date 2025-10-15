#include "sceneDef.h"
#include <QGraphicsItem>
QGraphicsItem *parentItem(QGraphicsItem *p_pItem)
{
    while (p_pItem->parentItem() != nullptr)
    {
        p_pItem = p_pItem->parentItem();
    }
    return p_pItem;
}
