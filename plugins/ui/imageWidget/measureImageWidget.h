#pragma once
#include "itemImageWidget.h"
namespace BS_QT_Ui
{
    class CMeasureItem;
};
class CMeasureImageWidget : public CItemImageWidget
{
    Q_OBJECT
    Q_PROPERTY(bool measure MEMBER m_measure WRITE setMeasure READ isMeasureing NOTIFY sigMeasureChanged)
public:
    CMeasureImageWidget(QWidget *p_pParent = nullptr);
    ~CMeasureImageWidget();
    bool isMeasureing() const;

public slots:
    void setMeasure(bool p_measure);

signals:
    void sigMeasureChanged(bool measuring);

protected slots:
    void viewMousemove(double x, double y);
    void viewChange();
    virtual void slotUpdateItemState() override;

protected:
    virtual void mousePressEvent(QMouseEvent *p_event) override;

protected:
    bool m_measure;
    BS_QT_Ui::CMeasureItem *m_pMeasureItem;
};