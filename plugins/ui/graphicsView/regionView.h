
#pragma once
#include "imageView.h"
#include "regionDef.h"
// 中键按住视图移动、双击复原
namespace TIGER_Graphics
{
    class CRegionView : public CImageView
    {
    public:
        using CImageView::CImageView;
        ~CRegionView() = default;

        void showRegionPoints(QList<CGraphicsRegion> p_featurePoints, QPointF p_offset = QPointF(0, 0));
        void clearRegion();

    protected:
        virtual void drawForeground(QPainter *painter, const QRectF &rect) override;
        virtual void paintEvent(QPaintEvent *event) override;

    protected:
        QList<CGraphicsRegion> m_regionPoints;
    };
}
