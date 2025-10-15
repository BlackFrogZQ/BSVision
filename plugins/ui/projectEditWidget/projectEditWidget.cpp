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

    m_pToolBar = new QLabel(this);
    m_pToolBar->setObjectName("ProjectEditToolBarBar");
    m_pToolBar->setStyleSheet("QLabel#ProjectEditToolBarBar{border:0px;border-bottom: 1px solid rgb(218, 219, 220);}");
    m_pToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_pToolBar->setMinimumHeight(56);

    m_pStationToolBar = new QLabel(this);
    m_pStationToolBar->setObjectName("ProjectEditStationToolBar");
    m_pStationToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_pStationToolBar->setMinimumHeight(50);

    // 工具栏
    {
        auto getToolPb = [](QString p_name, QString p_icon, bool buttonStyle = true)
        {
            QToolButton *pToolButton = new QToolButton;
            pToolButton->setFocusPolicy(Qt::ClickFocus);
            pToolButton->setStyleSheet(
                QString("%1QToolButton:hover:!checked{background-color:rgb(210,220,230);border-radius:5px;}")
                    .arg(buttonStyle ? "QToolButton{padding:0px;}" : "QToolButton{background:transparent;border:none;border-radius:5px;padding:0px;}"));
            pToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            pToolButton->setIcon(QIcon(p_icon));
            pToolButton->setIconSize(QSize(25, 25));
            pToolButton->setText(p_name);
            return pToolButton;
        };

        m_pSaveButton = getToolPb(tr("save"), cProjectEditSaveIcon);
        connect(m_pSaveButton, &QToolButton::clicked, this, [this]()
                {});

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
    layout->setMargin(0);
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
    QPainter paint;
    paint.begin(this);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.fillRect(this->rect(), QColor(235, 244, 255));
    QWidget::paintEvent(event);
}