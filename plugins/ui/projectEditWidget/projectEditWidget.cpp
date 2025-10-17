#include "projectEditWidget.h"
#include "pluginCoreDef.h"
#include "./featureParaEdit/featureParaEdit.h"
#include "stationWidget/stationWidget.h"
#include "ui/iconTextButton/iconTextButton.h"

#include "Ui/uiHelper.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QEvent>
#include <QScrollBar>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QAction>

CProjectEditWidget::CProjectEditWidget(QWidget *p_pParent) : QWidget(p_pParent)
{
    init();
    initToolBarLayout();
    initLayout();
}

CProjectEditWidget::~CProjectEditWidget()
{

}

void CProjectEditWidget::init()
{
    m_pStationWidget = new CStationWidget(this);

    m_pToolBar = new QWidget(this);
    m_pToolBar->setObjectName(cStdWorkWidgetToolBar);
    m_pToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_pStationToolBar = new QWidget(this);
    m_pStationToolBar->setObjectName(cStdWorkWidgetToolBar);
    m_pStationToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // 工具栏
    {
        auto getToolPb = [](QString p_name, QString p_icon, bool buttonStyle = true)
        {
            QToolButton *pToolButton = new QToolButton;
            pToolButton->setObjectName(cStdMainWidgetToolBtn);
            pToolButton->setFocusPolicy(Qt::ClickFocus);
            pToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            pToolButton->setIcon(QIcon(p_icon));
            pToolButton->setText(p_name);
            return pToolButton;
        };

        m_pSaveButton = getToolPb(tr("save"), cProjectEditSaveIcon);
        connect(m_pSaveButton, &QToolButton::clicked, this, [this](){});

        m_pSaveAsButton = getToolPb(tr("save as"), cProjectEditSaveAsIcon);
        connect(m_pSaveAsButton, &QToolButton::clicked, this, [this](){});

        m_pNewStationButton = getToolPb(tr("new mark"), cProjectEditNewIcon, false);
        connect(m_pNewStationButton, &QToolButton::clicked, this, &CProjectEditWidget::newStation);

        m_pCancelButton = getToolPb(tr("cancel"), cProjectEditCancelIcon);
        connect(m_pCancelButton, &QToolButton::clicked, this, [this](){});
    }

    connect(m_pStationWidget, &CStationWidget::sigEditFeature, this, &CProjectEditWidget::slotEditFeaturePara);
}

void CProjectEditWidget::initToolBarLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(m_pToolBar->layout());
    QHBoxLayout *layoutTemp = new QHBoxLayout;
    layoutTemp->addWidget(m_pSaveButton);
    layoutTemp->addWidget(m_pSaveAsButton);
    layoutTemp->addWidget(m_pCancelButton);
    layoutTemp->addWidget(m_pStationToolBar);
    layoutTemp->addWidget(m_pNewStationButton);
    layoutTemp->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layoutTemp->setSpacing(5);
    layoutTemp->setMargin(3);
    m_pToolBar->setLayout(layoutTemp);
}

void CProjectEditWidget::initLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
    QVBoxLayout *layoutTemp = new QVBoxLayout;
    layoutTemp->addWidget(m_pToolBar);
    layoutTemp->addWidget(m_pStationWidget);
    layoutTemp->setSpacing(0);
    layoutTemp->setMargin(0);
    this->setLayout(layoutTemp);
    m_pToolBar->show();
    m_pStationWidget->show();
}

void CProjectEditWidget::updateStationToolLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(m_pStationToolBar->layout());
    QHBoxLayout *layout = new QHBoxLayout;
    for (auto w : m_pStationToolButtons)
    {
        layout->addWidget(w);
    }
    layout->addWidget(m_pNewStationButton);
    layout->addStretch();
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setMargin(3);
    layout->setSpacing(5);
    m_pStationToolBar->setLayout(layout);
}

void CProjectEditWidget::newStation()
{
    QMetaObject::invokeMethod(m_pStationWidget, &CStationWidget::slotNewFeature, Qt::QueuedConnection);
}

void CProjectEditWidget::slotEditFeaturePara(IFeatureParaEdit *p_pFeatureParaEdit)
{
    connect(p_pFeatureParaEdit, &IFeatureParaEdit::sigClose, this, [this]()
            {this->initLayout();});
    BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
    m_pToolBar->hide();
    m_pStationWidget->hide();
    QVBoxLayout *layoutTemp2 = new QVBoxLayout;
    layoutTemp2->addWidget(p_pFeatureParaEdit);
    layoutTemp2->setSpacing(0);
    layoutTemp2->setMargin(0);
    this->setLayout(layoutTemp2);
}

void CProjectEditWidget::stationContextMenu(CIconTextButton *pIconTextButton)
{
}

bool CProjectEditWidget::eventFilter(QObject *obj, QEvent *e)
{
    if (obj != this)
    {
        if (e->type() == QEvent::ContextMenu && m_canEdit)
        {
            stationContextMenu(static_cast<CIconTextButton *>(obj));
            return true;
        }
    }
    return QWidget::eventFilter(obj, e);
}

void CProjectEditWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}