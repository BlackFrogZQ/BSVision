#pragma once
#include "fitToolItem.h"
namespace TIGER_Graphics
{
    class CFitLineItem;
    class CFitLineCenterToolItem : public IFitToolItem
    {
        Q_OBJECT
    public:
        CFitLineCenterToolItem(QGraphicsItem *parent = nullptr);
        ~CFitLineCenterToolItem() = default;
        int fitType() const override;
        QVector<QVector<CFitRect>> fitRects() const override;

    public slots:
        void setToolRect(QRectF p_rect) override;

    protected slots:
        void itemPosChanged();

    protected:
        CFitLineItem *m_pLeftItem = nullptr;
        CFitLineItem *m_pTopItem = nullptr;
        CFitLineItem *m_pRightItem = nullptr;
    };
}