#include "stdWorkWidget.h"
#include "ui/imageWidget/itemImageWidget.h"

#include "pluginCoreDef.h"

#include "Ui/uiHelper.h"
#include "Ui/widget/toolButton.h"
#include "Ui/widget/menu.h"

#include "para/paraManager.h"
#include "fileHelper/appHelper.h"
#include "service/serviceManager.h"
#include "message/msgManager.h"
#include "system/toolFun.h"

#include "system/camera.h"
#include "cameraCreate.h"

#include <QMenu>
#include <QToolBar>
#include <QWidgetAction>
#include <QToolButton>
#include <QHBoxLayout>

CStdWorkWidget *stdWorkWindow()
{
    return qobject_cast<CStdWorkWidget *>(BS_QT_Service::CServiceManager::GetService(cStdWorkWidget));
}

using namespace BS_QT_Service;
using namespace BS_QT_Framework;
using namespace BS_QT_FlieHelper;
using namespace BS_Hal_Camera;

CStdWorkWidget::CStdWorkWidget(QWidget *p_pParent) : QWidget(p_pParent)
{
    setObjectName(cStdWorkWidget);
    CServiceManager::RegistService(cStdWorkWidget, this);
}

CStdWorkWidget::~CStdWorkWidget()
{
}

void CStdWorkWidget::init()
{
    // 图片显示
    {
        newImageWidget();
        m_pImageWidget->setDrawCenterLine(CParaManager::GetValue<bool>(cShowCenterLine), CParaManager::getVarColor(cShowCenterLineColor));
        m_pImageWidget->setUsedItem(CParaManager::GetValue<bool>(cUsedMarkItemEnable));
        // 小窗
        {
            m_pMinImageWidget = new CShowAutoImageWidget(this);
            m_pMinImageWidget->setBorder(1);
            m_pMinImageWidget->hide();
            auto pCloseOrderImage = getToolBtn("", QIcon(cCloseIcon));
            pCloseOrderImage->setToolButtonStyle(Qt::ToolButtonIconOnly);
            pCloseOrderImage->setFixedSize(pCloseOrderImage->iconSize());
            connect(pCloseOrderImage, &QToolButton::clicked, this, [this]
                    { m_pMinImageWidget->hide(); });
            QHBoxLayout *layoutTemp = new QHBoxLayout;
            layoutTemp->addStretch();
            layoutTemp->addWidget(pCloseOrderImage);
            layoutTemp->setSpacing(0);
            layoutTemp->setMargin(0);
            QVBoxLayout *layoutTemp2 = new QVBoxLayout;
            layoutTemp2->addLayout(layoutTemp);
            layoutTemp2->addStretch();
            layoutTemp2->setSpacing(0);
            layoutTemp2->setMargin(0);
            m_pMinImageWidget->setViewLayout(layoutTemp2);
        }
        QHBoxLayout *layoutTemp = new QHBoxLayout;
        layoutTemp->addWidget(m_pMinImageWidget);
        layoutTemp->addStretch();
        layoutTemp->setSpacing(0);
        layoutTemp->setMargin(0);
        QVBoxLayout *layoutTemp2 = new QVBoxLayout;
        layoutTemp2->addLayout(layoutTemp);
        layoutTemp2->addStretch();
        layoutTemp2->setSpacing(0);
        layoutTemp2->setMargin(0);
        m_pImageWidget->setViewLayout(layoutTemp2);
        connect(
            m_pImageWidget, &CItemImageWidget::sigResize, this, [pOrderImageLabel = m_pMinImageWidget](QSize newSize, QSize oldSize)
            { if (pOrderImageLabel->isVisible()) { pOrderImageLabel->setFixedSize(pOrderImageLabel->size().scaled(newSize * 0.4, Qt::KeepAspectRatio)); } },
            Qt::QueuedConnection);
    }
    // 工具栏
    {
        m_pAcquireButton = BS_QT_Ui::CUIHelper::NewToolButton(cStdMainWidgetToolBtn, tr("Cont.Shoot"), "", cCamContinuousIcon, cCamContinuousIcon, "", "", Qt::ToolButtonTextUnderIcon, QRect(), QSize(), this);
        connect(m_pAcquireButton, &QToolButton::clicked, this, &CStdWorkWidget::slotAcquirePB);

        connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigStateChanged, this, &CStdWorkWidget::slotCameraStateChanged, Qt::QueuedConnection);
        connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigUpdateImage, this, &CStdWorkWidget::slotUpdateImage);

        m_pSaveCurrentImagePb = getToolBtn(tr("Save Image"), QIcon(cCamSaveImageIcon));
        connect(m_pSaveCurrentImagePb, &QToolButton::clicked, this, &CStdWorkWidget::slotSaveCurrentImage);

        m_pCameParaPb = getToolBtn(tr("Cam Params"), QIcon(cCamAdjustmentParamsIcon));
        m_pCameParaPb->setPopupMode(QToolButton::InstantPopup);
        auto pParaMenu = new BS_QT_Ui::CCustomMenu(m_pCameParaPb);
        pParaMenu->setLeaveCloseEnable(true);
        m_pParaMenu = pParaMenu;
        QWidgetAction *action = new QWidgetAction(m_pParaMenu);
        action->setDefaultWidget(CCameraCreater::newConfigCameraParaWidget(VisionCameraID,Qt::Vertical, this));
        m_pParaMenu->addAction(action);
        m_pCameParaPb->setMenu(m_pParaMenu);
    }
    m_pSaveCurrentImagePb->setEnabled(false);
    m_pAcquireButton->setEnabled(false);
    m_pCameParaPb->hide();
    slotCameraStateChanged(visionCamera()->state());
}

void CStdWorkWidget::initLayout()
{
    initToolBar();
    QVBoxLayout *pLauout = new QVBoxLayout(this);
    pLauout->addWidget(m_pToolBar);
    pLauout->addWidget(m_pImageWidget);
    pLauout->setSpacing(0);
    pLauout->setMargin(0);
}

void CStdWorkWidget::updateOrderImage(const QImage &p_image, QList<TIGER_Graphics::CGraphicsRegion> p_regions)
{
    QMetaObject::invokeMethod(
        this, [p_image, p_regions, this]
        {
            auto showFeature = [this,&p_regions,&p_image](CShowAutoImageWidget* p_autoImageLabel)
            {
                if (p_regions.isEmpty())
                {
                    p_autoImageLabel->setAutoScaleImage(p_image, "NG", true, Qt::red);
                }
                else
                {
                    p_autoImageLabel->setAutoScaleImage(p_image, "OK", true, Qt::green);
                    p_autoImageLabel->showRegions(p_regions);
                }
            };
            if (visionCamera()->isAcquire())
            {
                m_pMinImageWidget->show();
                m_pMinImageWidget->setFixedSize(p_image.size().scaled(m_pImageWidget->size()* 0.4, Qt::KeepAspectRatio));
                showFeature(m_pMinImageWidget);
            }
            else
            {
                m_pMinImageWidget->hide();
                showFeature(m_pImageWidget);
            } },
        Qt::QueuedConnection);
}

void CStdWorkWidget::setOnlyImageShow(bool p_onlyImage)
{
}

void CStdWorkWidget::newImageWidget()
{
    assert(m_pImageWidget == nullptr);
    m_pImageWidget = new CItemImageWidget(this);
}

void CStdWorkWidget::initToolBar()
{
    assert(m_pToolBar == nullptr);
    m_pToolBar = new QWidget(this);
    m_pToolBar->setObjectName(cStdWorkWidgetToolBar);
    QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
    pToolBarLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pToolBarLayout->addWidget(m_pSaveCurrentImagePb);
    pToolBarLayout->addWidget(m_pAcquireButton);
    pToolBarLayout->addWidget(m_pCameParaPb);
    pToolBarLayout->addStretch();
    pToolBarLayout->setMargin(3);
    pToolBarLayout->setSpacing(5);
}

void CStdWorkWidget::imageValidChanged(bool p_valid)
{
    if (m_pSaveCurrentImagePb->isEnabled() != p_valid)
    {
        m_pSaveCurrentImagePb->setEnabled(p_valid);
        m_pImageWidget->setEnabled(p_valid);
    }
}

QToolButton *CStdWorkWidget::getToolBtn(QString p_name, QIcon p_icon, QWidget *p)
{
    QToolButton *pToolButton = new QToolButton(p);
    pToolButton->setObjectName(cStdMainWidgetToolBtn);
    pToolButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pToolButton->setIcon(p_icon);
    pToolButton->setText(p_name);
    return pToolButton;
}

void CStdWorkWidget::slotAcquirePB()
{
    if (!(visionCamera()->isConnected() && visionCamera()->acquireChange()))
    {
        Out_Info << tr("Camera acquire error:%1").arg(visionCamera()->error());
    }
}

void CStdWorkWidget::slotCameraStateChanged(BS_Hal_Camera::CCameraStateInfo p_stateInfo)
{
    bool isConnected = p_stateInfo.isConnected;
    bool isAcquire = p_stateInfo.isAcquire;
    m_pAcquireButton->setEnabled(isConnected);
    m_pAcquireButton->setChecked(isAcquire);
    m_pCameParaPb->setVisible(isAcquire);
    if (!isAcquire && m_pParaMenu->isVisible())
    {
        m_pParaMenu->close();
    }

    if (p_stateInfo.type == BSHalCamera::cctVirtual)
    {
        m_pImageWidget->showMsg(Qt::AlignHCenter | Qt::AlignBottom, tr("Virtual Camera"), QColor(134, 134, 134, 200));
    }
    else
    {
        m_pImageWidget->showMsg(Qt::AlignHCenter | Qt::AlignBottom);
    }
}

void CStdWorkWidget::slotUpdateImage(const QImage &p_image)
{
    bool isValid = !p_image.isNull();
    auto currPixmap = m_pImageWidget->currentPixmap();
    m_pImageWidget->setAutoScaleImage(p_image, "",
                                      currPixmap.isNull() || (isValid && currPixmap.size() != p_image.size()));
    if (isValid != m_currentImageValid)
    {
        m_currentImageValid = isValid;
        imageValidChanged(isValid);
        emit sigImageValidChanged(isValid);
    }
}

void CStdWorkWidget::slotSaveCurrentImage()
{
    m_pSaveCurrentImagePb->setEnabled(false);
    saveImageToSysDir(m_pImageWidget->currentPixmap().toImage(), "grabImage", true);
    m_pSaveCurrentImagePb->setEnabled(true);
}
