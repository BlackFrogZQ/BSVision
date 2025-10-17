#include "editTemplateWidget.h"
#include "ui/basic.h"
#include "ui/graphicsView/graphics/itemScene/templateScene.h"
#include "system/camera.h"
#include "previewImageDialog.h"
#include "template/shapeTemplate/shapeDef.h"
#include "setTemplateParaWindow/setCreateParaWindow.h"

#include "iCameraDef.h"
#include "iCameraCtrl.h"
#include "message/msgManager.h"
#include "Ui/graphics/item/itemDef.h"
#include "Ui/graphics/item/path/pathItem.h"

#include <QToolButton>
#include <QPushButton>
#include <QComboBox>
#include <QToolBar>
#include <QWidgetAction>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

using namespace BS_2DVision_Template;
using namespace TIGER_Graphics;
CEditTemplateWidget::CEditTemplateWidget(QWidget *parent)
    : IGraphicsWidget(parent),
      m_pDrawTypeCB(nullptr),
      m_pExposureTimeButton(nullptr),
      m_pClearButton(nullptr),
      m_pMaxContrast(nullptr),
      m_pItemScene(nullptr),
      m_pTemplateCreateParas(nullptr)
{
    m_pView = new TIGER_Graphics::CRegionView(this);
    QRectF rect(0, 0, 800, 600);
    m_pView->setSceneRect(rect);
    m_pItemScene = new TIGER_Graphics::CTemplateScene(m_pView, this);
    m_pView->setScene(m_pItemScene);
    m_pScene = m_pItemScene;
    connect(m_pItemScene, &TIGER_Graphics::CTemplateScene::sigDrawFinished, this, &CEditTemplateWidget::slotDrawShapeFinished);
    connect(m_pItemScene, &TIGER_Graphics::CTemplateScene::sigItemChanged, this, &CEditTemplateWidget::slotProcess);

    m_pDrawTypeCB = new QComboBox;
    m_pDrawTypeCB->setStyleSheet("QComboBox{padding:0px;}");
    m_pDrawTypeCB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pDrawTypeCB->setFixedHeight(30);
    m_pDrawTypeCB->setIconSize(QSize(28, 28));
    m_pDrawTypeCB->setToolTip(tr("布尔运算类型"));
    for (size_t i = 0; i < cDrawTypeStr.size(); i++)
    {
        m_pDrawTypeCB->addItem(QIcon(cDrawTypePixmap[i]), cDrawTypeStr[i]);
    }
    m_pPathWidthSlider = getSlider(2, 20, 5, 0);
    m_pPathWidthSlider->setSingleStep(1);

    m_pMaxContrast = getSlider(5, 160, 15, 0);
    m_pMinContrast = getSlider(1, 160, 10, 0);
    m_pExpandParas = getPushButton(tr("其他拓展参数"));

    m_pTemplateCreateParas = new CCreateTemplateParasOld;

    initToolActions();
    setParaEnabled(true);
}

CEditTemplateWidget::~CEditTemplateWidget()
{
}

void CEditTemplateWidget::setTemplateParas(CCreateTemplateParasOld *p_pTemplateCreateParas)
{
}

void CEditTemplateWidget::updateResult()
{
}

void CEditTemplateWidget::initToolActions()
{

    m_pExposureTimeButton = getPushButton(tr("更新图像"));
    m_pExposureTimeButton->setText(tr("采集"));
    m_pExposureTimeButton->setIcon(QIcon(":/res/graphics/toolBar/grabImage.png"));
    connect(m_pExposureTimeButton, &QPushButton::clicked, this, &CEditTemplateWidget::slotSetExposureTime);
    auto pButtonAction = addPushButtonAction(m_pExposureTimeButton);
    pButtonAction->setText(tr("更新曝光"));

    m_pClearButton = getPushButton(tr("清除所有图形"));
    m_pClearButton->setIcon(QIcon(":/res/ClearResult.png"));
    m_pClearButton->setText(tr("清空"));
    connect(m_pClearButton, &QPushButton::clicked, this, &CEditTemplateWidget::slotReset);
    addPushButtonAction(m_pClearButton);

    const QList<CItemDef::CGraphicsItemShape> cShape = {CItemDef::cgitRect, CItemDef::cgitEllipse, CItemDef::cgitPath};
    for (auto shape : cShape)
    {
        QToolButton *pToolButton = getToolPb(cGraphicsItemShapeStr[shape], cGraphicsItemShapePixmap[shape]);
        pToolButton->setIconSize(QSize(28, 28));
        m_toolButtons.append(pToolButton);
        if (shape == CItemDef::cgitPath)
        {
            QWidgetAction *action = new QWidgetAction(pToolButton);
            action->setDefaultWidget(m_pPathWidthSlider);
            pToolButton->addAction(action);
            pToolButton->setPopupMode(QToolButton::MenuButtonPopup);
            connect(pToolButton, &QToolButton::clicked, this, [this]
                    {
                        auto pShape = TIGER_Graphics::CGraphicsBasicShape(CGraphicsItemCreater::createGraphicsItem(CItemDef::cgitPath),CSceneDef::CDrawType(m_pDrawTypeCB->currentIndex()));
                        BS_QT_Ui::CPathItem* pPathItem = static_cast< BS_QT_Ui::CPathItem*>(pShape.pItem);
                        pPathItem->setWidth(m_pPathWidthSlider->value());
                        connect(m_pPathWidthSlider,&BS_QT_Ui::CCustomSlider::sigValueChanged,pPathItem,&BS_QT_Ui::CPathItem::setWidth);
                        m_pItemScene->drawShape(pShape); });
        }
        else
        {
            connect(pToolButton, &QToolButton::clicked, this, [itemShape = shape, cb = m_pDrawTypeCB, pScene = m_pItemScene]
                    { pScene->drawShape({CGraphicsItemCreater::createGraphicsItem(itemShape), CSceneDef::CDrawType(cb->currentIndex())}); });
        }
    }
}

QToolBar *CEditTemplateWidget::getToolBar()
{
    QToolBar *pToolBar = new QToolBar;
    pToolBar->setIconSize(m_pClearButton->iconSize());
    pToolBar->addWidget(m_pExposureTimeButton);
    pToolBar->addWidget(m_pClearButton);
    pToolBar->addWidget(m_pDrawTypeCB);
    for (auto pAction : m_toolButtons)
    {
        pToolBar->addWidget(pAction);
    }
    return pToolBar;
}

QLayout *CEditTemplateWidget::getBottomLayout()
{
    QGridLayout *pLayout = new QGridLayout;
    const double cSize = qMax(30.0, 30 * CUiBasic::getScale());
    m_pMaxContrast->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(tr("模板对比度"), 30), 0, 0, 1, 1);
    pLayout->addWidget(m_pMaxContrast, 0, 1, 1, 5);
    m_pMinContrast->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(tr("匹配对比度"), 30), 0, 6, 1, 1);
    pLayout->addWidget(m_pMinContrast, 0, 7, 1, 5);

    m_pExpandParas->setText(tr("拓展参数"));
    m_pExpandParas->setFixedHeight(cSize);
    connect(m_pExpandParas, &QPushButton::clicked, this, [this]
            {
                CSetCreateParaWindow setPara(this);
                auto templateCreateParas = *m_pTemplateCreateParas;
                setPara.setCreateParas(&templateCreateParas);
                if (setPara.isSave())
                {
                    *m_pTemplateCreateParas = templateCreateParas;
                    slotProcess();
                } });
    pLayout->addWidget(m_pExpandParas, 0, 13, 1, 1);
    pLayout->setMargin(0);
    pLayout->setSpacing(2);
    return pLayout;
}

void CEditTemplateWidget::slotProcess()
{
    QPainterPath path = m_pItemScene->getPath();
    m_pItemScene->setInitPath(QPainterPath());
    if (path.isEmpty())
    {
        m_oldPath = QPainterPath();
        m_pView->clearPoints();
        emit sigReset();
        return;
    }
    int maxContrast = m_pMaxContrast->value();
    int minContrast = maxContrast > m_pMinContrast->value() ? m_pMinContrast->value() : maxContrast - 1;
    if (m_oldPath == path && maxContrast == m_pTemplateCreateParas->contrast && minContrast == m_pTemplateCreateParas->limitMinContrast)
    {
        return;
    }
    m_oldPath = path;
    m_pTemplateCreateParas->contrast = maxContrast;
    m_pTemplateCreateParas->limitMinContrast = minContrast;
    // CRegion shapeRegion;
    // if (m_templateCreate.previewTemplate(path, &shapeRegion))
    // {
    //     showShapeRegion(shapeRegion, false, _ShowRegionDilation);
    //     if (!m_pItemScene->isDrawing())
    //     {
    //         emit sigEditFinished();
    //     }
    // }
    // else
    // {
    //     m_pView->clearPoints();
    //     emit sigReset();
    // }
}

void CEditTemplateWidget::slotSetExposureTime()
{
    // CPreviewImageDialog pWidget(m_pTemplate->exposureTime, m_pTemplate->gainRaw, m_pTemplate->gamma, this);
    double exposureTime = 20.0;
    double gainRaw = 1.0;
    double gamma = 1.0;
    CPreviewImageDialog pWidget(exposureTime, gainRaw, gamma, this);
    pWidget.setFixedSize(m_pView->size());
    if (pWidget.isGrabImage())
    {
        slotGrabImage();
    }
}

void CEditTemplateWidget::slotGrabImage()
{
    QMetaObject::invokeMethod(
        this, [this]
        {
            auto paras = visionCamera()->paras();
            auto exposureTime = paras.exposureTime.value;
            auto gainRaw  = paras.gainRaw.value;
            auto gamma = paras.gamma.value;
            // visionCamera()->setExposureTime(m_pTemplate->exposureTime);
            // visionCamera()->setGainRaw(m_pTemplate->gainRaw);
            // visionCamera()->setGamma(m_pTemplate->gamma);
            if (visionCamera()->isConnected() && visionCamera()->singleFrame())
            {
                slotReset();
                // m_pTemplate->image = visionCamera()->lastImage();
                // assert(!m_pTemplate->image.isNull());
                // m_templateCreate.setOriginalImage(m_pTemplate->image);
                // m_pItemScene->setBackgroundImage(&visionCamera()->lastImage());
                resetView();
                this->update();
                emit sigReset();
            }
            else
            {
                Out_SyncMsgBox << QString("MatchingTool grabImage error:%1").arg(visionCamera()->error());
            }
            visionCamera()->setExposureTime(exposureTime);
            visionCamera()->setGainRaw(gainRaw);
            visionCamera()->setGamma(gamma); },
        Qt::QueuedConnection);
}

void CEditTemplateWidget::slotReset()
{
    m_pItemScene->clearItem();
    m_pView->clearPoints();
    emit sigReset();
    setParaEnabled(false);
}

void CEditTemplateWidget::slotDrawShapeFinished(TIGER_Graphics::CGraphicsBasicShape p_shape)
{
    m_pPathWidthSlider->disconnect(p_shape.pItem);
    m_pItemScene->addShape(p_shape);
    setParaEnabled(true);
}

void CEditTemplateWidget::setParaEnabled(bool enabled)
{
    const QList<QWidget *> cSetParaWidget = {m_pMaxContrast, m_pMinContrast, m_pExpandParas};
    for (auto p : cSetParaWidget)
    {
        if (p->isEnabled() != enabled)
        {
            p->setEnabled(enabled);
        }
    }
}