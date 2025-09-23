#include "stationWidget.h"
#include "featureIconButton.h"
#include "pluginCoreDef.h"

#include "Ui/uiHelper.h"

#include <QToolButton>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>

CStationWidget::CStationWidget(QWidget *p) : QWidget(p), m_station(nullptr)
{
    this->setFocusPolicy(Qt::ClickFocus);
    m_pFeatureToolBar = new QLabel(this);
    m_pFeatureToolBar->setFixedHeight(50);
    // m_pShowAutoImageLabel = new CShowAutoImageLabel(this);
    // m_pShowAutoImageLabel->init();
    // m_pShowAutoImageLabel->setBackground(QBrush(QColor(235, 244, 255)));
    m_pAddFeature = new QToolButton;
    m_pAddFeature->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_pAddFeature->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_pAddFeature->setIconSize(QSize(25, 25));
    m_pAddFeature->setText(tr("new feature"));
    m_pAddFeature->setIcon(QIcon(cProjectEditNewIcon));
    m_pAddFeature->setStyleSheet("QToolButton{background:transparent;border:none;border-radius:5px;}"
                                 "QToolButton:hover:!checked{background-color:rgb(203,217,235);}");
    connect(m_pAddFeature, &QToolButton::clicked, this, &CStationWidget::slotNewFeature);
    setStation(nullptr);
}

CStationWidget::~CStationWidget()
{
}

void CStationWidget::setStation(CStation *p_station)
{
    qDeleteAll(m_featureIconButtons);
    m_featureIconButtons.clear();

    updateToolBarLayout();
    BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
    QVBoxLayout *pLayout = new QVBoxLayout;

    // m_pShowAutoImageLabel->hide();
    m_pFeatureToolBar->hide();

    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    setLayout(pLayout);
    setCanEdit(true);
}

void CStationWidget::updateToolBarLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(m_pFeatureToolBar->layout());
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addStretch();
    for (auto pW : m_featureIconButtons)
    {
        pLayout->addWidget(pW);
    }
    pLayout->addWidget(m_pAddFeature);
    pLayout->addStretch();
    pLayout->setMargin(0);
    m_pFeatureToolBar->setLayout(pLayout);
}

void CStationWidget::setCanEdit(bool p_edit)
{
    m_pAddFeature->setVisible(p_edit);
    for (auto pW : m_featureIconButtons)
    {
        pW->setCanEdit(p_edit);
    }
}

void CStationWidget::slotNewFeature()
{
}

void CStationWidget::slotEditFeature(int p_id)
{
}

void CStationWidget::paintEvent(QPaintEvent *event)
{
    QPainter paint;
    paint.begin(this);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.fillRect(this->rect(), QColor(235, 244, 255));
    QWidget::paintEvent(event);
}