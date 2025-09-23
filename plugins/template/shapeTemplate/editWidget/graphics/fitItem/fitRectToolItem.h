#pragma once
#include "fitToolItem.h"
namespace TIGER_Graphics
{
    class CFitLineItem;
    class CFitRectToolItem : public IFitToolItem
    {
        Q_OBJECT
    public:
        CFitRectToolItem(QGraphicsItem *parent = nullptr);
        ~CFitRectToolItem() = default;
        int fitType() const override;
        QVector<QVector<CFitRect>> fitRects() const override;

    public slots:
        void setToolRect(QRectF p_rect) override;

    protected slots:
        void itemPosChanged();

    protected:
        // m_pTopItem 控制顶边方向和大小 和 m_pRightItem 决定侧边大小，其他仅显示
        CFitLineItem *m_pTopItem = nullptr;
        CFitLineItem *m_pRightItem = nullptr;
        CFitLineItem *m_pBottomItem = nullptr;
        CFitLineItem *m_pLeftItem = nullptr;
    };
}