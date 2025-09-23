#include "createShapeWizard.h"

#include "shapeExpandParaWidget.h"

#include "../graphics/booleanScene.h"
#include "ui/graphicsView/regionView.h"
#include "template/shapeTemplate/shapeDef.h"

#include "Ui/widget/toolButton.h"
#include "Ui/widget/comboBox.h"
#include "Ui/widget/rangeSlider.h"
#include "Ui/widget/slider.h"
#include "Ui/widget/menu.h"
#include "message/msgManager.h"

#include "pluginCoreDef.h"
#include <QEvent>
#include <QLabel>
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QSpacerItem>
namespace BS_2DVision_Template
{
    using namespace TIGER_Graphics;
    using namespace BS_QT_Ui;
    CCreateShapeWizard::CCreateShapeWizard(QWidget *parent) : QWizardPage(parent)
    {
        setWindowTitle(tr("Create Shape"));

        {
            m_pTitleLabel = new QLabel(tr("Create Shape"), this);
            m_pTitleLabel->setWordWrap(true);

            auto newToolBtn = [](QWidget *p, QIcon p_icon = QIcon(), QString p_text = QString())
            {
                auto pToolBtn = new BS_QT_Ui::CCustomToolButton(p);
                pToolBtn->setObjectName(cStdMainWidgetToolBtn2);
                pToolBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                pToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                pToolBtn->setText(p_text);
                pToolBtn->setToolTip(p_text);
                pToolBtn->setIcon(p_icon);
                pToolBtn->setCheckable(true);
                pToolBtn->setChecked(false);
                pToolBtn->setClickChangeState(false);
                pToolBtn->setIconSize(QSize(30, 30));
                return pToolBtn;
            };

            auto newDoubleSlider = [](QWidget *p, int p_min, int p_max, int p_valueLower, int p_valueUpper, QString p_pre = QString())
            {
                auto pSlider = new BS_QT_Ui::CCustomRangeSlider(Qt::Horizontal, BS_QT_Ui::CCustomRangeSlider::DoubleHandles, p);
                pSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                pSlider->setDecimals(0);
                pSlider->setSingleStep(1);
                pSlider->setRange(p_min, p_max);
                pSlider->setLowerValue(p_valueLower);
                pSlider->setUpperValue(p_valueUpper);
                return pSlider;
            };

            auto newSlider = [](QWidget *p, int p_min, int p_max, int p_value, QString p_pre = QString())
            {
                auto pSlider = new BS_QT_Ui::CCustomSlider(p);
                pSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                pSlider->setDecimals(0);
                pSlider->setPrefix(p_pre);
                pSlider->setRange(p_min, p_max);
                pSlider->setValue(p_value);
                return pSlider;
            };

            m_pUpdateImageBtn = newToolBtn(this, QIcon(cShapeGrabImageIcon), tr("Grab Image"));
            m_pClearBtn = newToolBtn(this, QIcon(cClearIcon), tr("Clear"));
            m_pClearBtn->setCheckable(false);

            m_pDrawTypeCB = new BS_QT_Ui::CCustomComboBox(this);
            m_pDrawTypeCB->setIconSize(m_pUpdateImageBtn->iconSize());
            for (auto key : cBooleanStrMap.keys())
            {
                m_pDrawTypeCB->addItem(QIcon(cBooleanIconMap[key]), cBooleanStrMap[key], (int)key);
            }

            m_pRectBtn = newToolBtn(this, QIcon(cGraphicsRectIcon), tr("Draw Rect"));
            m_pRectBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

            m_pEllipseBtn = newToolBtn(this, QIcon(cGraphicsEllipseIcon), tr("Draw Ellipse"));
            m_pEllipseBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

            m_pPathBtn = newToolBtn(this, QIcon(cGraphicsPathIcon), tr("Draw Path"));
            m_pPathBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            m_pPathBtn->setText("");

            m_pPathWidthSlider = newSlider(this, 2, 20, 5);
            auto pMenu = new BS_QT_Ui::CCustomMenu(m_pPathBtn);
            pMenu->setLeaveCloseEnable(true);
            m_pPathParaMenu = pMenu;
            QWidgetAction *action = new QWidgetAction(m_pPathParaMenu);
            action->setDefaultWidget(m_pPathWidthSlider);
            m_pPathParaMenu->addAction(action);

            m_pShapeContrastSlider = newDoubleSlider(this, 1, 160, 10, 15, tr("Contrast") + ":");
            m_pExpandParasBtn = newToolBtn(this, QIcon(), tr("Other Paras"));
            m_pExpandParasBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
            m_pExpandParasBtn->setObjectName(cStdPushBtnStyleToolBtn);
            auto pExpandParaMenu = new BS_QT_Ui::CCustomMenu(m_pExpandParasBtn);
            m_pExpandParaWidget = new CShapeExpandParaWidget(this);
            action = new QWidgetAction(pExpandParaMenu);
            action->setDefaultWidget(m_pExpandParaWidget);
            pExpandParaMenu->addAction(action);
            m_pExpandParasBtn->setMenu(pExpandParaMenu);
            m_pExpandParasBtn->setPopupMode(QToolButton::InstantPopup);

            m_pView = new CRegionView(this);
            m_pBooleanScene = new CBooleanScene(m_pView, this);
            m_pView->setScene(m_pBooleanScene);
            connect(m_pBooleanScene, &CBooleanScene::sigItemChanged, this, &CCreateShapeWizard::creatShape, Qt::QueuedConnection);
        }

        QHBoxLayout *itemLayout = new QHBoxLayout;
        itemLayout->addWidget(m_pTitleLabel);
        itemLayout->addSpacing(20);
        itemLayout->addWidget(m_pUpdateImageBtn);
        itemLayout->addWidget(m_pClearBtn);
        itemLayout->addSpacing(20);
        itemLayout->addWidget(m_pDrawTypeCB);
        itemLayout->addWidget(m_pRectBtn);
        itemLayout->addWidget(m_pEllipseBtn);
        itemLayout->addWidget(m_pPathBtn);
        itemLayout->addStretch();
        itemLayout->setSpacing(3);
        itemLayout->setMargin(0);

        QHBoxLayout *paraLayout = new QHBoxLayout;
        paraLayout->addWidget(m_pShapeContrastSlider, 1);
        paraLayout->addWidget(m_pExpandParasBtn);
        paraLayout->setSpacing(3);
        paraLayout->setMargin(0);

        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->setSpacing(3);
        pLayout->setMargin(0);
        pLayout->addLayout(itemLayout);
        pLayout->addWidget(m_pView, 1);
        pLayout->addLayout(paraLayout);
        connect(m_pUpdateImageBtn, &QToolButton::clicked, this, &CCreateShapeWizard::grabImage);
        connect(m_pRectBtn, &QToolButton::clicked, this, &CCreateShapeWizard::setDrawRect);
        connect(m_pEllipseBtn, &QToolButton::clicked, this, &CCreateShapeWizard::setDrawEllipse);
        connect(m_pPathBtn, &QToolButton::clicked, this, &CCreateShapeWizard::setDrawPath);
        connect(m_pClearBtn, &QToolButton::clicked, this, [this]
                {m_pBooleanScene->clearMyItem(); m_pView->resetView(); });
    }

    void CCreateShapeWizard::setShape(CShape *pShape)
    {
        m_pShape = pShape;
        m_pExpandParaWidget->setParas(pShape != nullptr ? &(pShape->createParas) : nullptr);
        initializePage();
        emit completeChanged();
    }

    bool CCreateShapeWizard::isComplete() const
    {
        return !(m_pView->getShowPoints().isEmpty() || editing());
    }

    void CCreateShapeWizard::initializePage()
    {
        m_pView->clearRegion();
        if (m_pShape != nullptr && !m_pShape->createImage.isNull())
        {
            m_pView->showPoints(m_pShape->shapePts().toList(), -m_pShape->createImage.rect().center());
            m_pBooleanScene->showImage(QPixmap::fromImage(m_pShape->createImage), true);
        }
        m_pView->resetView();
    }

    void CCreateShapeWizard::setDrawRect(bool p_drawing)
    {
        if (p_drawing)
        {
            if (m_pBooleanScene->isDrawing())
            {
                m_pBooleanScene->stopDrawShape();
            }
            setEditWidget(nullptr);
        }
        else
        {
            setEditWidget(m_pRectBtn);
            drawItem(m_pRectBtn, CGraphicsItemCreater::createGraphicsItem(CItemDef::cgitRect));
        }
        m_pRectBtn->setChecked(!p_drawing);
    }
    void CCreateShapeWizard::setDrawEllipse(bool p_drawing)
    {
        if (p_drawing)
        {
            if (m_pBooleanScene->isDrawing())
            {
                m_pBooleanScene->stopDrawShape();
            }
            setEditWidget(nullptr);
        }
        else
        {
            setEditWidget(m_pEllipseBtn);
            drawItem(m_pEllipseBtn, CGraphicsItemCreater::createGraphicsItem(CItemDef::cgitEllipse));
        }
        m_pEllipseBtn->setChecked(!p_drawing);
    }

    void CCreateShapeWizard::setDrawPath(bool p_drawing)
    {
        disconnect(m_pPathWidthSlider, &CCustomSlider::sigValueChanged, this, nullptr);
        if (p_drawing)
        {
            m_pPathBtn->setMenu(nullptr);
            m_pPathBtn->setPopupMode(QToolButton::DelayedPopup);
            if (m_pBooleanScene->isDrawing())
            {
                m_pBooleanScene->stopDrawShape();
            }
            setEditWidget(nullptr);
        }
        else
        {
            m_pPathBtn->setMenu(m_pPathParaMenu);
            m_pPathBtn->setPopupMode(QToolButton::MenuButtonPopup);
            setEditWidget(m_pPathBtn);
            auto pPath = CGraphicsItemCreater::createGraphicsItem(CItemDef::cgitPath);
            QMetaObject::invokeMethod(pPath, "setWidth", Q_ARG(double, m_pPathWidthSlider->value()));
            connect(m_pPathWidthSlider, &CCustomSlider::sigValueChanged, this, [this, pPath](double p_value)
                    { QMetaObject::invokeMethod(pPath, "setWidth", Q_ARG(double, p_value)); });
            drawItem(m_pPathBtn, pPath);
        }
        m_pPathBtn->setChecked(!p_drawing);
    }

    void CCreateShapeWizard::creatShape()
    {
        auto maxContrast = (int)m_pShapeContrastSlider->getUpperValue();
        auto minContrast = (int)m_pShapeContrastSlider->getMinimun();
        const auto &creatParas = m_pShape->createParas;
        auto path = m_pBooleanScene->getPath();
        if (m_lastPath == path && minContrast == creatParas.limitMinContrast && maxContrast == creatParas.contrast)
        {
            return;
        }
        m_lastPath = path;
        if (path.isEmpty())
        {
            m_pView->clearPoints();
            m_pShape->clearTemplate();
        }
        else
        {
            path.translate(m_pShape->createImage.rect().center());
            m_pView->showPoints(m_pShape->previewTemplate(path).toList(), -m_pShape->createImage.rect().center());
        }
        emit completeChanged();
    }
}

#include "system/camera.h"
#include "cameraCreate.h"
namespace BS_2DVision_Template
{
    void CCreateShapeWizard::grabImage(bool p_grab)
    {
        assert(m_pShape != nullptr);
        auto stopGrab = [this](bool p_update)
        {
            disconnect(m_cameraImageConnection);
            m_pUpdateImageBtn->setMenu(nullptr);
            m_pUpdateImageBtn->setChecked(false);
            m_pUpdateImageBtn->setPopupMode(QToolButton::DelayedPopup);

            bool isAcquire = visionCamera()->isAcquire();
            if (visionCamera()->isConnected() && isAcquire)
            {
                auto cameraParas = visionCamera()->paras();
                visionCamera()->acquireChange();
                if (p_update)
                {
                    auto &createParas = m_pShape->createParas;
                    createParas.exposureTime = cameraParas.exposureTime.value;
                    createParas.gainRaw = cameraParas.gainRaw.value;
                    createParas.gamma = cameraParas.gamma.value;
                    m_pShape->createImage = m_pBooleanScene->getImage().toImage();
                    m_pView->resetView();
                }
                visionCamera()->setExposureTime(m_oriCameraParas.exposureTime.value);
                visionCamera()->setGainRaw(m_oriCameraParas.gainRaw.value);
                visionCamera()->setGamma(m_oriCameraParas.gamma.value);
            }
            setEditWidget(nullptr);
        };

        if (p_grab)
        {
            stopGrab(true);
        }
        else
        {
            m_pUpdateImageBtn->setChecked(true);
            bool isAcquire = visionCamera()->isAcquire();
            m_oriCameraParas = visionCamera()->paras();
            if (visionCamera()->isConnected() && !isAcquire)
            {
                setEditWidget(m_pUpdateImageBtn);
                if (m_pCameraParaMenu == nullptr)
                {
                    auto pMenu = new BS_QT_Ui::CCustomMenu(m_pPathBtn);
                    pMenu->setLeaveCloseEnable(true);
                    m_pCameraParaMenu = pMenu;
                    QWidgetAction *action = new QWidgetAction(m_pCameraParaMenu);
                    action->setDefaultWidget(BS_Hal_Camera::CCameraCreater::newConfigCameraParaWidget(VisionCameraID, Qt::Vertical, this));
                    m_pCameraParaMenu->addAction(tr("Cancel"), [this, stopGrab]()
                                                 { stopGrab(false); });
                    m_pCameraParaMenu->addSeparator();
                    m_pCameraParaMenu->addAction(action);
                }
                m_pUpdateImageBtn->setPopupMode(QToolButton::MenuButtonPopup);
                m_pUpdateImageBtn->setMenu(m_pCameraParaMenu);
                const auto &createParas = m_pShape->createParas;
                visionCamera()->setExposureTime(createParas.exposureTime);
                visionCamera()->setGainRaw(createParas.gainRaw);
                visionCamera()->setGamma(createParas.gamma);
                m_cameraImageConnection = connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigUpdateImage, this, [this](QImage p_image)
                                                  { m_pBooleanScene->showImage(QPixmap::fromImage(p_image), m_pBooleanScene->getImage().isNull()); });
                if (!visionCamera()->acquireChange())
                {
                    stopGrab(false);
                    Out_SyncHInt << tr("camera acquire fault") + ":" << visionCamera()->error();
                }
            }
        }
    }

    void CCreateShapeWizard::drawItem(QToolButton *pToolBtn, IGraphicsBasicItem *pItem)
    {
        disconnect(m_pBooleanScene, &CBooleanScene::sigDrawFinished, this, nullptr);
        m_drawFinishedConnection = connect(m_pBooleanScene, &CBooleanScene::sigDrawFinished, this, [this, pToolBtn]()
                                           {
                                                disconnect(m_drawFinishedConnection);
                                                emit pToolBtn->clicked(true); }, Qt::QueuedConnection);
        m_pBooleanScene->drawShape(CBooleanType(m_pDrawTypeCB->itemData(m_pDrawTypeCB->currentIndex()).toInt()), pItem);
    }

    void CCreateShapeWizard::setEditWidget(QWidget *p_w)
    {
        for (auto p : QList<QWidget *>{m_pUpdateImageBtn, m_pClearBtn, m_pDrawTypeCB, m_pRectBtn, m_pEllipseBtn, m_pPathBtn, m_pShapeContrastSlider, m_pExpandParasBtn})
        {
            p->setEnabled(p_w == nullptr || p_w == p);
        }
        wizard()->button(QWizard::CancelButton)->setEnabled(p_w == nullptr);
        emit completeChanged();
    }

    bool CCreateShapeWizard::editing() const
    {
        for (auto p : QList<QToolButton *>{m_pUpdateImageBtn, m_pRectBtn, m_pEllipseBtn, m_pPathBtn})
        {
            if (p->isChecked())
            {
                return true;
            }
        }
        return false;
    }
}
