#pragma once
#include "../graphicsView/regionView.h"

#include "Ui/graphics/imageScene.h"
#include "Ui/graphics/graphicsView.h"
#include "Ui/widget/slider.h"

#include <QWidget>

class QToolBar;
class QToolButton;
class QPushButton;
class QSpinBox;
class QDoubleSpinBox;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class IGraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    IGraphicsWidget(QWidget *parent);
    ~IGraphicsWidget();
    void initLayout();

protected:
    virtual QToolBar *getToolBar() = 0;
    virtual QLayout *getRightLayout();
    virtual QLayout *getBottomLayout();
    virtual void contextMenuEvent(QContextMenuEvent *) override;
    QAction *addPushButtonAction(QPushButton *p_pPb);

public:
    static QPushButton *getPushButton(const QString &p_toolTip = "");
    static QToolButton* getToolPb(QString p_name, QString p_icon);
    static BS_QT_Ui::CCustomSlider *getSlider(double min, double max, double value = 0.5, int precision = 1);

protected:
    QList<QAction *> m_menuActions;
    TIGER_Graphics::CRegionView *m_pView;
    BS_QT_Ui::CImageScene *m_pScene;
};