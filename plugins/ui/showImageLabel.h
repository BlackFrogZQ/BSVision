#pragma once
#include <QLabel>
class QMouseEvent;

class CDragMainWidgetLabel : public QLabel
{
    Q_OBJECT
public:
    CDragMainWidgetLabel(QWidget *p_pParent = nullptr);
    virtual ~CDragMainWidgetLabel();
    void setCanMove(bool p_canMove);
    QPixmap *pixmap() = delete;

public slots:
    void slotSetOnlyShowImage(bool p_isOnlyShowImage);

signals:
    void sigDragMove(QPoint p_move);
    void sigOnlyShowImage(bool p_isOnlyShowImage);
    void sigMouseDoubleClick();
    void sigResize(QSize p_newSize, QSize p_oldSize);

protected:
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseDoubleClickEvent(QMouseEvent *ev) override;
    void resizeEvent(QResizeEvent *event) override;

protected:
    bool m_canDragMove;
    bool m_isDragMove;
    bool m_isOnlyShowImage;
    QPoint m_lastPoint;
};

// namespace TIGER_Graphics
// {
//     class CRegionView;
// };
// namespace BS_QT_Ui
// {
//     class CImageScene;
// }
// #include "imageProcess/stationProcess/featureInfoDef.h"
// class CShowAutoImageLabel : public CDragMainWidgetLabel
// {
//     Q_OBJECT
// public:
//     CShowAutoImageLabel(QWidget *p_pParent = nullptr);
//     ~CShowAutoImageLabel();
//     void setBorder(double p_border = 0.95);
//     QPixmap currentPixmap() const;
//     void setBackground(QBrush p_brush);
//     virtual void init();
//     Q_INVOKABLE void showMsg(int p_alignFlags, QString p_msg = QString(), double p_wScale = 0, QColor p_color = Qt::red);

//     void setAutoScaleImage(const QImage &p_image, QString p_msg = QString(), bool p_resetSacle = true, QColor p_color = Qt::red);
//     void showRegionPoints(QList<TIGER_ProcessTool::COutFeatureShape> p_regionPoints);
//     void setDrawCenterLine(bool p_isDrawLine,bool p_widgetCenterLine = false,double p_circle = 10);
//     void setLabelLayout(QLayout *pLaout);
//     // 不会清除item
//     void clear();

// protected:
//     TIGER_Graphics::CRegionView *m_pView;
//     BS_QT_Ui::CImageScene *m_pScene;
// };