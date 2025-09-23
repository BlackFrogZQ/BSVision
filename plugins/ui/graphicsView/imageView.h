#pragma once
#include "Ui/graphics/imageView.h"
// 中键按住视图移动、双击复原
namespace TIGER_Graphics
{
    class CImageView : public BS_QT_Ui::CImageView
    {
        friend class CRulerWidget;
        Q_OBJECT
    public:
        using BS_QT_Ui::CImageView::CImageView;
        ~CImageView() = default;
        void setPointColor(QColor color = Qt::red);
        // 坐标系为Scene下
        void showPoints(QList<QPointF> p_features, QPointF p_offset = QPointF(0, 0));
        QList<QPointF> getShowPoints() const;
        void clearPoints();

    protected:
        void drawPoints(QPainter *painter, QList<QPointF> p_points, QColor color);

    protected:
        virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

    protected:
        QList<QPointF> m_features;
        QColor m_featureColor = Qt::red;
    };
};
