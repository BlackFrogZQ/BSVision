#pragma once
#include "ui/basic.h"

#include "iCameraDef.h"
#include "Ui/widget/slider.h"

class CShowAutoImageWidget;
class CPreviewImageDialog : public TIGER_UIBasic::INoExcCancelDialog
{
    Q_OBJECT
public:
    CPreviewImageDialog(double &p_exposureTime, double &p_gainRaw, double &p_gamma, QWidget *parent = nullptr);
    ~CPreviewImageDialog();
    bool isGrabImage();

protected:
    void init();

protected slots:
    void slotCameraParaChanged(BS_Hal_Camera::CCameraParaInfo p_paraInfo);

protected:
    bool m_isGrabImage;
    CShowAutoImageWidget *m_pImageWidget;
    BS_QT_Ui::CCustomSlider *m_pExposureTimeSlider;
    BS_QT_Ui::CCustomSlider *m_pGainRawSlider;
    BS_QT_Ui::CCustomSlider *m_pGammaSlider;
    double &m_exposureTime;
    double &m_gainRaw;
    double &m_gamma;
};