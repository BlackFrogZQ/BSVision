#pragma once
#include "ui/graphicsView/graphics/sceneDef.h"
#include "imageProcess/engineDef.h"
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
class CEditTemplateWidget : public IGraphicsWidget
{
    Q_OBJECT
public:
    CEditTemplateWidget(QWidget *parent = nullptr);
    ~CEditTemplateWidget();
    void setTemplateParas(CCreateTemplateParasOld *p_pTemplateCreateParas);
    virtual void updateResult() override;

public slots:
    void slotGrabImage();

protected slots:
    void slotDrawShapeFinished(TIGER_Graphics::CGraphicsBasicShape p_shape);
    void slotSetExposureTime();
    void slotReset();
    void slotProcess();

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
    CCreateTemplateParasOld* m_pTemplateCreateParas;
};