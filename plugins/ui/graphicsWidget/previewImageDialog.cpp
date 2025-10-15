#include "previewImageDialog.h"
#include "pluginCoreDef.h"
#include "system/camera.h"
#include "ui/imageWidget/showImageWidget.h"

#include "para/paraManager.h"
#include "iCameraCtrl.h"
#include "message/msgManager.h"

#include <QMenu>
#include <QPushButton>
#include <QHBoxLayout>

CPreviewImageDialog::CPreviewImageDialog(double &p_exposureTime, double &p_gainRaw, double &p_gamma, QWidget *parent)
    : TIGER_UIBasic::INoExcCancelDialog(parent), m_exposureTime(p_exposureTime), m_gainRaw(p_gainRaw), m_gamma(p_gamma), m_isGrabImage(false)
{
    setWindowTitle(tr("采集图像"));
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
    setWindowFlag(Qt::WindowStaysOnTopHint, CUiBasic::isTop);
    init();
}

CPreviewImageDialog::~CPreviewImageDialog()
{
}

void CPreviewImageDialog::init()
{
    m_pImageWidget = new CShowAutoImageWidget(this);
    m_pImageWidget->setBorder(0.98);
    m_pImageWidget->setDrawCenterLine(BS_QT_Framework::CParaManager::GetValue<bool>(cShowCenterLine), BS_QT_Framework::CParaManager::getVarColor(cShowCenterLineColor));

    const double cSize = 30;
    QPushButton *pGrabImage = TIGER_UIBasic::noKeyPushButton(tr("采集"));
    pGrabImage->setFixedHeight(cSize);
    connect(pGrabImage, &QPushButton::clicked, this, [this]
            {m_isGrabImage = true; this->close(); });
    QPushButton *pExit = TIGER_UIBasic::noKeyPushButton(tr("取消"));
    pExit->setFixedHeight(cSize);
    connect(pExit, &QPushButton::clicked, this, [this]
            {m_isGrabImage = false; this->close(); });

    // 曝光
    {
        m_pExposureTimeSlider = new BS_QT_Ui::CCustomSlider(this);
        m_pExposureTimeSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pExposureTimeSlider->setSuffix("");
        m_pExposureTimeSlider->setDecimals(2);
        m_pExposureTimeSlider->setSingleStep(1);
        connect(
            m_pExposureTimeSlider, &BS_QT_Ui::CCustomSlider::sigValueChanged, this, [this](double p_value)
            {
                    m_exposureTime = p_value*1000;
                    visionCamera()->setExposureTime(m_exposureTime); },
            Qt::QueuedConnection);
        m_pExposureTimeSlider->setFixedHeight(cSize);
    }
    // 对比度
    {
        m_pGainRawSlider = new BS_QT_Ui::CCustomSlider(this);
        m_pGainRawSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pGainRawSlider->setSuffix("");
        m_pGainRawSlider->setDecimals(2);
        m_pGainRawSlider->setSingleStep(0.01);
        connect(
            m_pGainRawSlider, &BS_QT_Ui::CCustomSlider::sigValueChanged, this, [this](double p_value)
            {
                    m_gainRaw = p_value;
                    visionCamera()->setGainRaw(m_gainRaw); },
            Qt::QueuedConnection);
        m_pGainRawSlider->setFixedHeight(cSize);
    }
    // 校正
    {
        m_pGammaSlider = new BS_QT_Ui::CCustomSlider(this);
        m_pGammaSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pGammaSlider->setSuffix("");
        m_pGammaSlider->setDecimals(2);
        m_pGammaSlider->setSingleStep(0.1);
        connect(
            m_pGammaSlider, &BS_QT_Ui::CCustomSlider::sigValueChanged, this, [this](double p_value)
            {
                    m_gamma = p_value;
                    visionCamera()->setGamma(m_gamma); },
            Qt::QueuedConnection);
        m_pGammaSlider->setFixedHeight(cSize);
    }

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addLayout(TIGER_UIBasic::getHLableLayout(tr("曝光"), m_pExposureTimeSlider));
    if (!BS_QT_Framework::CParaManager::GetValue<bool>(cUsedGainRaw))
    {
        pLayout->addLayout(TIGER_UIBasic::getHLableLayout(tr("增益"), m_pGainRawSlider));
    }
    else
    {
        m_pGainRawSlider->hide();
    }
    pLayout->addLayout(TIGER_UIBasic::getHLableLayout(tr("校正"), m_pGammaSlider));

    pLayout->setMargin(0);
    pLayout->setSpacing(2);

    QHBoxLayout *pLayout3 = new QHBoxLayout;
    pLayout3->addStretch();
    pLayout3->addWidget(pGrabImage);
    pLayout3->addWidget(pExit);
    pLayout3->setMargin(0);
    pLayout3->setSpacing(2);

    QVBoxLayout *pLayout2 = new QVBoxLayout;
    pLayout2->addWidget(m_pImageWidget);
    pLayout2->addLayout(pLayout);
    pLayout2->addLayout(pLayout3);
    pLayout2->setMargin(0);
    pLayout2->setSpacing(0);
    this->setLayout(pLayout2);
}

void CPreviewImageDialog::slotCameraParaChanged(BS_Hal_Camera::CCameraParaInfo p_paraInfo)
{
    QList<BS_QT_Ui::CCustomSlider *> uiSlider = {m_pGammaSlider, m_pGainRawSlider, m_pExposureTimeSlider};
    const QList<BS_Hal_Camera::CRangeValue<double>> cParas = {p_paraInfo.gamma, p_paraInfo.gainRaw, p_paraInfo.exposureTime};
    for (int i = 0; i < uiSlider.size(); i++)
    {
        uiSlider[i]->blockSignals(true);
        if (uiSlider[i] == m_pExposureTimeSlider)
        {
            uiSlider[i]->setRange(cParas[i].minValue / 1000, cParas[i].maxValue / 1000);
            uiSlider[i]->setValue(cParas[i].value / 1000);
        }
        else
        {
            uiSlider[i]->setRange(cParas[i].minValue, cParas[i].maxValue);
            uiSlider[i]->setValue(cParas[i].value);
        }
        uiSlider[i]->blockSignals(false);
    }
}

bool CPreviewImageDialog::isGrabImage()
{
    m_isGrabImage = false;
    connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigUpdateImage, this, [this](QImage p_image)
        { m_pImageWidget->setAutoScaleImage(p_image, "", m_pImageWidget->currentPixmap().isNull()); },
        Qt::QueuedConnection);
    connect(visionCamera(), &BS_Hal_Camera::ICameraControl::sigParaChanged, this, &CPreviewImageDialog::slotCameraParaChanged);
    visionCamera()->resendSig();
    assert(!visionCamera()->isAcquire());
    do
    {
        auto paras = visionCamera()->paras();
        auto exposureTime = paras.exposureTime.value;
        auto gainRaw = paras.gainRaw.value;
        auto gamma = paras.gamma.value;
        visionCamera()->setExposureTime(m_exposureTime);
        visionCamera()->setGainRaw(m_gainRaw);
        visionCamera()->setGamma(m_gamma);
        if (!(visionCamera()->isConnected() && visionCamera()->acquireChange()))
        {
            Out_SyncMsgBox << QString("grabImage error:%1").arg(visionCamera()->error());
            m_isGrabImage = false;
            break;
        }
        this->exec();
        visionCamera()->acquireChange();
        visionCamera()->setExposureTime(exposureTime);
        visionCamera()->setGainRaw(gainRaw);
        visionCamera()->setGamma(gamma);
    } while (0);
    visionCamera()->disconnect(this);
    return m_isGrabImage;
}