#include "projectEditWidget.h"
#include "pluginCoreDef.h"
#include "stationWidget/stationWidget.h"
#include "ui/iconTextButton/iconTextButton.h"

#include "Ui/uiHelper.h"

#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>

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
        // connect(m_pNewStationButton, &QToolButton::clicked, this, &CProjectEditWindow::newStation);

        m_pCancelButton = getToolPb(tr("cancel"), cProjectEditCancelIcon);
        connect(m_pCancelButton, &QToolButton::clicked, this, [this](){});
    }
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

void CProjectEditWidget::initToolBarLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(m_pToolBar->layout());
    QHBoxLayout *layoutTemp = new QHBoxLayout;
    layoutTemp->addWidget(m_pSaveButton);
    layoutTemp->addWidget(m_pSaveAsButton);
    layoutTemp->addWidget(m_pCancelButton);
    layoutTemp->addWidget(m_pStationToolBar);
    layoutTemp->setSpacing(5);
    layoutTemp->setMargin(3);
    m_pToolBar->setLayout(layoutTemp);
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