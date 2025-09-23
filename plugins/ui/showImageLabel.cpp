#include "showImageLabel.h"
#include <QMouseEvent>
#include <QPainter>

CDragMainWidgetLabel::CDragMainWidgetLabel(QWidget *p_pParent)
    : QLabel(p_pParent), m_canDragMove(true), m_isDragMove(false), m_isOnlyShowImage(false)
{
    setAlignment(Qt::AlignCenter);
}

CDragMainWidgetLabel::~CDragMainWidgetLabel()
{
}

void CDragMainWidgetLabel::setCanMove(bool p_canMove)
{
    m_canDragMove = p_canMove;
}

void CDragMainWidgetLabel::slotSetOnlyShowImage(bool p_isOnlyShowImage)
{
    m_isOnlyShowImage = p_isOnlyShowImage;
    emit sigOnlyShowImage(m_isOnlyShowImage);
}

void CDragMainWidgetLabel::leaveEvent(QEvent *event)
{
    m_isDragMove = false;
}

void CDragMainWidgetLabel::mousePressEvent(QMouseEvent *ev)
{
    if (m_canDragMove && (ev->button() & Qt::LeftButton) && (!m_isDragMove))
    {
        m_isDragMove = true;
        m_lastPoint = ev->globalPos();
        return;
    }
    QLabel::mousePressEvent(ev);
}

void CDragMainWidgetLabel::mouseMoveEvent(QMouseEvent *p_event)
{
    if (m_canDragMove && m_isDragMove && (p_event->buttons() & Qt::LeftButton))
    {
        emit sigDragMove(p_event->globalPos() - m_lastPoint);
        m_lastPoint = p_event->globalPos();
        return;
    }
    QLabel::mouseMoveEvent(p_event);
}

void CDragMainWidgetLabel::mouseReleaseEvent(QMouseEvent *p_event)
{
    if (m_isDragMove && (p_event->button() & Qt::LeftButton))
    {
        m_isDragMove = false;
        return;
    }
    QLabel::mouseReleaseEvent(p_event);
}

void CDragMainWidgetLabel::mouseDoubleClickEvent(QMouseEvent *p_event)
{
    if ((p_event->buttons() & Qt::LeftButton))
    {
        slotSetOnlyShowImage(!m_isOnlyShowImage);
        emit sigMouseDoubleClick();
        return;
    }
    QLabel::mouseDoubleClickEvent(p_event);
}

void CDragMainWidgetLabel::resizeEvent(QResizeEvent *p_event)
{
    QLabel::resizeEvent(p_event);
    emit sigResize(p_event->size(), p_event->oldSize());
}

// #include "graphicsView/regionView.h"
// #include "Ui/graphics/imageScene.h"
// #include "bsBasic.h"
// #include <QHBoxLayout>
// #include <QTimer>
// CShowAutoImageLabel::CShowAutoImageLabel(QWidget *p_pParent)
//     : CDragMainWidgetLabel(p_pParent),
//       m_pView(nullptr),
//       m_pScene(nullptr)
// {
// }

// CShowAutoImageLabel::~CShowAutoImageLabel()
// {
//     delPtr(m_pScene);
// }

// void CShowAutoImageLabel::init()
// {
//     m_pScene = new TIGER_Graphics::CImageScene(this);
//     m_pView = new TIGER_Graphics::CRegionView(m_pScene);
//     m_pView->setBorder(0.95);
//     QHBoxLayout *pLayout = new QHBoxLayout;
//     QGridLayout *testLayout = new QGridLayout;
//     // testLayout->addWidget(rulerWidget, 0, 1);
//     // testLayout->addWidget(rulerWidget2, 1, 0);
//     testLayout->addWidget(m_pView, 1, 1);
//     testLayout->setMargin(0);
//     testLayout->setSpacing(0);
//     pLayout->addLayout(testLayout);
//     pLayout->setMargin(0);
//     pLayout->setSpacing(0);
//     this->setLayout(pLayout);
// }

// void CShowAutoImageLabel::setLabelLayout(QLayout *pLaout)
// {
//     m_pView->setLayout(pLaout);
// }

// void CShowAutoImageLabel::setBorder(double p_border)
// {
//     m_pView->setBorder(p_border);
// }

// void CShowAutoImageLabel::setDrawCenterLine(bool p_isDrawLine, bool p_widgetCenterLine, double p_circle)
// {
//     m_pView->setDrawCenterLine(p_isDrawLine, Qt::red);
//     m_pView->setDrawWidgetCenterLine(p_widgetCenterLine, p_circle, Qt::red);
//     update();
// }

// QPixmap CShowAutoImageLabel::currentPixmap() const
// {
//     return m_pScene->getImage();
// }

// void CShowAutoImageLabel::clear()
// {
//     m_pScene->clearImage();
//     m_pView->clearRegion();
// }
// void CShowAutoImageLabel::setBackground(QBrush p_brush)
// {
//     m_pScene->setBackgroundFill(p_brush);
// }

// void CShowAutoImageLabel::showMsg(int p_alignFlags, QString p_msg, double p_wScale, QColor p_color)
// {
//     m_pView->drawText(p_alignFlags, p_msg, p_wScale, p_color);
// }

// void CShowAutoImageLabel::setAutoScaleImage(const QImage &p_image, QString p_msg, bool p_resetSacle, QColor p_color)
// {
//     m_pView->clearRegion();
//     m_pScene->showImage(QPixmap::fromImage(p_image));
//     m_pView->drawText(Qt::AlignLeft | Qt::AlignTop, p_msg, 0.1, p_color);
//     m_pView->setDefineSceneRect(m_pScene->sceneRect(), p_resetSacle);
// }

// void CShowAutoImageLabel::showRegionPoints(QList<TIGER_ProcessTool::COutFeatureShape> p_regionPoints)
// {
//     auto cSize = m_pScene->getImage().size();
//     m_pView->showRegionPoints(p_regionPoints, QPointF(-cSize.width() * 0.5, -cSize.height() * 0.5));
// }