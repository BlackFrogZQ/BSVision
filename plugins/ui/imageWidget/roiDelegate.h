#pragma once
#include <QObject>
#include <QColor>
#include <QRectF>

namespace BS_QT_Ui
{
    class IMyItemScene;
    class CRectItem;
};
class QGraphicsScene;

class CRoiDelegate : public QObject
{
    Q_OBJECT
public:
    CRoiDelegate(QGraphicsScene *p_pScene);
    QRectF roiRect(QPointF p_offset = QPointF(0, 0)) const;

public slots:
    void setLimitRoi(QRectF p_limitRect);
    void setRoi(QRectF p_rect, QPointF p_offset = QPointF(0, 0));
    void setUsedRoi(bool p_used);
    void setAdjustRoi(bool p_adjust);
    void setColor(QColor p_color = Qt::red);

signals:
    void sigAdjusRoiChanged(bool p_adjust);
    void sigRoiChanged(QRectF p_roi);
    void sigUsedRoiChanged(bool p_used);

protected slots:
    void slotResize();

protected:
    QGraphicsScene *m_pScene;
    BS_QT_Ui::CRectItem *m_pRoiItem;
    QRectF m_lastRoiRect;
    QRectF m_limitRect;
};
