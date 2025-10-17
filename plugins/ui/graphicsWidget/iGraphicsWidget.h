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
    virtual void updateResult() = 0;

signals:
    void sigEditFinished();
    void sigReset();

protected:
    void resetView();

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
    static QSpinBox *getSpinBox(int min, int max, int value = 0, int step = 1);
    static QDoubleSpinBox *getDoubleSpinBox(double min, double max, double value = 0, double step = 0.1);

protected:
    QList<QAction *> m_menuActions;
    TIGER_Graphics::CRegionView *m_pView;
    BS_QT_Ui::CImageScene *m_pScene;
};