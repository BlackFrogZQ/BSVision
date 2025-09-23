#pragma once
#include <QWidget>

#include "Ui/graphics/imageScene.h"
#include "../graphicsView/regionView.h"
class QMouseEvent;


class CShowAutoImageWidget : public QWidget
{
    Q_OBJECT
public:
    CShowAutoImageWidget(QWidget *p_pParent = nullptr);
    ~CShowAutoImageWidget();
    TIGER_Graphics::CRegionView* view() const;
    BS_QT_Ui::CImageScene* scene() const;

    void setBorder(double p_border = 0.95);
    QPixmap currentPixmap() const;
    void setBackground(QBrush p_brush);

    void showMsg(int alignmentFlags, QString p_msg, QColor p_color);
    // 会继承颜色
    void showMsg(int alignmentFlags, QString p_msg = QString());

    void setAutoScaleImage(const QImage &p_image, QString p_msg = QString(), bool p_resetSacle = true, QColor p_color = Qt::red);
    void showRegions(QList<TIGER_Graphics::CGraphicsRegion> p_regions);
    void setDrawCenterLine(bool p_isDrawLine, QColor p_color = Qt::red);
    void setViewLayout(QLayout *pLaout);
    // 不会清除item
    void clearImageRegion();

signals:
    void sigResize(QSize p_newSize, QSize p_oldSize);

protected:
    void resizeEvent(QResizeEvent *event) override;

protected:
    TIGER_Graphics::CRegionView *m_pView;
    BS_QT_Ui::CImageScene *m_pScene;
};