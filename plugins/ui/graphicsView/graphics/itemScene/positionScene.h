#pragma once
#include "iItemScene.h"

namespace TIGER_Graphics
{
    class IPositioningToolItem;

    class CPositionScene : public IItemScene
    {
        Q_OBJECT

    public:
        using IItemScene::IItemScene;
        void addTool(IPositioningToolItem *p_pTool);
        virtual void removeMyItem(QGraphicsItem *p_item) override;
        virtual QList<QGraphicsItem *> myGraphicsItems() const override;

    protected:
        virtual void clearMyItem() override;
        QList<IPositioningToolItem *> m_toolItems;
    };
}