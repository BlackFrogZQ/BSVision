#pragma once
#include <QWidget>
#include "ui/graphicsView/regionDef.h"
#include "ui/imageWidget/itemImageWidget.h"
#include "ui/imageWidget/roiDelegate.h"

#include "iCameraDef.h"

class CItemImageWidget;
class CShowAutoImageWidget;
class QToolBar;
class QToolButton;
class QMenu;

class CStdWorkWidget : public QWidget
{
    Q_OBJECT
public:
    CStdWorkWidget(QWidget *p_pParent = nullptr);
    virtual ~CStdWorkWidget();
    virtual void init();
    virtual void initLayout();

public slots:
    void setOnlyImageShow(bool p_onlyImage);
    void updateOrderImage(const QImage &p_image, QList<TIGER_Graphics::CGraphicsRegion> p_regions);

signals:
    void sigImageValidChanged(bool p_valid);

protected slots:
    void slotAcquirePB();
    void slotCameraStateChanged(BS_Hal_Camera::CCameraStateInfo p_stateInfo);
    void slotUpdateImage(const QImage &p_image);
    void slotSaveCurrentImage();

protected:
    virtual void newImageWidget();
    virtual void initToolBar();
    virtual void imageValidChanged(bool p_valid);

    static QToolButton *getToolBtn(QString p_name, QIcon p_icon,QWidget* p = nullptr);

public:
    CItemImageWidget *m_pImageWidget = nullptr;
    CShowAutoImageWidget *m_pMinImageWidget = nullptr;
    QWidget *m_pToolBar = nullptr;
    QToolButton *m_pAcquireButton = nullptr;
    QToolButton *m_pCameParaPb = nullptr;
    QToolButton *m_pSaveCurrentImagePb = nullptr;
    QMenu *m_pParaMenu = nullptr;
    bool m_currentImageValid = false;
};

CStdWorkWidget *stdWorkWindow();