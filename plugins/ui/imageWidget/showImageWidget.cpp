#include "showImageWidget.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
using namespace TIGER_Graphics;
CShowAutoImageWidget::CShowAutoImageWidget(QWidget *p_pParent)
    : QWidget(p_pParent),
      m_pView(nullptr),
      m_pScene(nullptr)
{
    m_pScene = new BS_QT_Ui::CImageScene(this);
    m_pView = new TIGER_Graphics::CRegionView(m_pScene);
    m_pView->setDefTextPara(Qt::red, 0.1);
    m_pView->setBorder(0.95);
    QHBoxLayout *pLayout = new QHBoxLayout;
    QGridLayout *testLayout = new QGridLayout;
    // testLayout->addWidget(rulerWidget, 0, 1);
    // testLayout->addWidget(rulerWidget2, 1, 0);
    testLayout->addWidget(m_pView, 1, 1);
    testLayout->setMargin(0);
    testLayout->setSpacing(0);
    pLayout->addLayout(testLayout);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    this->setLayout(pLayout);
}

CShowAutoImageWidget::~CShowAutoImageWidget()
{
    delPtr(m_pScene);
}

TIGER_Graphics::CRegionView *CShowAutoImageWidget::view() const
{
    return m_pView;
}

BS_QT_Ui::CImageScene *CShowAutoImageWidget::scene() const
{
    return m_pScene;
}

void CShowAutoImageWidget::setViewLayout(QLayout *pLaout)
{
    m_pView->setLayout(pLaout);
}

void CShowAutoImageWidget::setBorder(double p_border)
{
    m_pView->setBorder(p_border);
}

void CShowAutoImageWidget::setDrawCenterLine(bool p_isDrawLine, QColor p_color)
{
    m_pView->setDrawCenterLine(p_isDrawLine, p_color);
    update();
}

QPixmap CShowAutoImageWidget::currentPixmap() const
{
    return m_pScene->getImage();
}

void CShowAutoImageWidget::clearImageRegion()
{
    m_pScene->clearImage();
    m_pView->clearRegion();
}

void CShowAutoImageWidget::setBackground(QBrush p_brush)
{
    m_pScene->setBackgroundFill(p_brush);
}

void CShowAutoImageWidget::showMsg(int alignmentFlags, QString p_msg, QColor p_color)
{
    m_pView->drawText(alignmentFlags, BS_QT_Ui::CGraphicsViewText{p_msg, p_color, 0.1});
}

void CShowAutoImageWidget::showMsg(int alignmentFlags, QString p_msg)
{
    m_pView->drawText(alignmentFlags, p_msg);
}

void CShowAutoImageWidget::setAutoScaleImage(const QImage &p_image, QString p_msg, bool p_resetSacle, QColor p_color)
{
    m_pView->clearRegion();
    m_pScene->showImage(QPixmap::fromImage(p_image));
    m_pView->drawText(Qt::AlignTop | Qt::AlignLeft, BS_QT_Ui::CGraphicsViewText{p_msg, p_color});
    m_pView->setDefineSceneRect(m_pScene->sceneRect(), p_resetSacle);
}

void CShowAutoImageWidget::showRegions(QList<TIGER_Graphics::CGraphicsRegion> p_regionPoints)
{
    auto cSize = m_pScene->getImage().size();
    m_pView->showRegionPoints(p_regionPoints, QPointF(-cSize.width() * 0.5, -cSize.height() * 0.5));
}

void CShowAutoImageWidget::resizeEvent(QResizeEvent *p_event)
{
    QWidget::resizeEvent(p_event);
    emit sigResize(p_event->size(), p_event->oldSize());
}