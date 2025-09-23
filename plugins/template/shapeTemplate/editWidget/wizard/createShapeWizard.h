#pragma once
#include <QPainterPath>
#include <QWizardPage>
#include "iCameraDef.h"
class QMenu;
class QLabel;
class QComboBox;
class QToolButton;
namespace BS_QT_Ui
{
    class IGraphicsBasicItem;
    class CCustomRangeSlider;
    class CCustomSlider;
}
namespace TIGER_Graphics
{
    class CRegionView;
    class CBooleanScene;
};
namespace BS_2DVision_Template
{
    class CShape;
    class CShapeExpandParaWidget;
    class CCreateShapeWizard : public QWizardPage
    {
        Q_OBJECT
    public:
        CCreateShapeWizard(QWidget *parent = nullptr);
        ~CCreateShapeWizard() = default;
        bool isComplete() const override;
        void initializePage() override;
        void setShape(CShape *pShape);

    protected slots:
        void grabImage(bool p_grab);
        void drawItem(QToolButton *pToolBtn, BS_QT_Ui::IGraphicsBasicItem *pItem);
        void setEditWidget(QWidget *p_w);
        void setDrawRect(bool p_drawing);
        void setDrawEllipse(bool p_drawing);
        void setDrawPath(bool p_drawing);
        void creatShape();

    protected:
        bool editing() const;

    protected:
        QPainterPath m_lastPath;

    protected:
        TIGER_Graphics::CRegionView *m_pView = nullptr;
        TIGER_Graphics::CBooleanScene *m_pBooleanScene = nullptr;

        CShape *m_pShape = nullptr;
        QLabel *m_pTitleLabel = nullptr;

        QToolButton *m_pUpdateImageBtn = nullptr;
        QToolButton *m_pClearBtn = nullptr;
        QComboBox *m_pDrawTypeCB = nullptr;

        QToolButton *m_pRectBtn = nullptr;
        QToolButton *m_pEllipseBtn = nullptr;
        QToolButton *m_pPathBtn = nullptr;
        BS_QT_Ui::CCustomSlider *m_pPathWidthSlider = nullptr;

        BS_QT_Ui::CCustomRangeSlider *m_pShapeContrastSlider = nullptr;
        QToolButton *m_pExpandParasBtn = nullptr;

        CShapeExpandParaWidget* m_pExpandParaWidget = nullptr;

        BS_Hal_Camera::CCameraParaInfo m_oriCameraParas;
        QMetaObject::Connection m_cameraImageConnection;
        QMetaObject::Connection m_drawFinishedConnection;
        QMenu *m_pCameraParaMenu = nullptr;
        QMenu *m_pPathParaMenu = nullptr;
    };
}