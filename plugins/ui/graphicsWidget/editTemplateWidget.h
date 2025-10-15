#pragma once
#include "iGraphicsWidget.h"
#include "Ui/widget/slider.h"

const QStringList cGraphicsItemShapePixmap =
    {
    ":/res/graphics/drawShape/line.png",
    ":/res/graphics/drawShape/rect.png",
    "",
    ":/res/graphics/drawShape/ellipse.png",
    ":/res/graphics/drawShape/path.png",
    "",
    ""
    };

class QToolButton;
class QPushButton;
class QComboBox;
namespace TIGER_Graphics
{
    class CTemplateScene;
}
namespace BS_2DVision_Template
{
    class CCreateTemplateParas;
}
class CEditTemplateWidget : public IGraphicsWidget
{
    Q_OBJECT
public:
    CEditTemplateWidget(QWidget *parent = nullptr);
    ~CEditTemplateWidget();

public slots:
    void slotGrabImage();

protected slots:
    void slotSetExposureTime();
    void slotReset();

protected:
    void initToolActions();
    void setParaEnabled(bool enabled);
    virtual QToolBar *getToolBar() override;
    virtual QLayout *getBottomLayout()override;

protected:
    QList<QToolButton *> m_toolButtons;
    QPushButton* m_pClearButton;
    QPushButton* m_pExposureTimeButton;
    QPushButton* m_pExpandParas;
    QComboBox *m_pDrawTypeCB;
    BS_QT_Ui::CCustomSlider* m_pMaxContrast;
    BS_QT_Ui::CCustomSlider* m_pMinContrast;
    BS_QT_Ui::CCustomSlider *m_pPathWidthSlider;
    TIGER_Graphics::CTemplateScene* m_pItemScene;
    QPainterPath m_oldPath;
    BS_2DVision_Template::CCreateTemplateParas* m_pTemplateCreateParas;
};