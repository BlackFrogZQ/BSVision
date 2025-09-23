#include "regionView.h"
namespace TIGER_Graphics
{
    void CRegionView::showRegionPoints(QList<CGraphicsRegion> p_featurePoints, QPointF p_offset)
    {
        m_regionPoints = p_featurePoints;
        for (auto &p : m_regionPoints)
        {
            for (auto &point : p.points)
            {
                point += p_offset;
            }
        }
    }

    void CRegionView::clearRegion()
    {
        m_regionPoints.clear();
    }

    void CRegionView::drawForeground(QPainter *painter, const QRectF &rect)
    {
        CImageView::drawForeground(painter, rect);
        for (const auto &p : m_regionPoints)
        {
            drawPoints(painter, p.points, p.color);
        }
    }
    void CRegionView::paintEvent(QPaintEvent *event)
    {
        CImageView::paintEvent(event);
        if (!m_regionPoints.isEmpty())
        {
            QPainter painter(this->viewport());
            QFont font;
            auto w = this->rect().width();
            auto h = this->rect().height();
            font.setPixelSize(qMax(w * m_fontScale * 0.35, 12.0));
            QFontMetrics fontMetrics(font);
            painter.setFont(font);
            const int fontH = fontMetrics.height();
            int currentW = 0;
            int currentH = h - fontH;
            for (const auto &p : m_regionPoints)
            {
                const int fontW = fontMetrics.width(p.name) + fontMetrics.maxWidth()*0.25;
                if ((currentW + fontW) > w)
                {
                    currentH -= fontH;
                    currentW = 0;
                }
                painter.setPen(p.color);
                painter.drawText(QRect(currentW, currentH, fontW, fontH), Qt::AlignCenter, p.name);
                currentW += fontW;
            }
        }
    }
}