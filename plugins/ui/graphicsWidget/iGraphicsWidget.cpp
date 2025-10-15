#include "iGraphicsWidget.h"
#include "ui/basic.h"
#include <QPushButton>
#include <QAction>
#include <QToolButton>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QMenu>
#include <QToolBar>

IGraphicsWidget::IGraphicsWidget(QWidget *parent) : QWidget(parent), m_pScene(nullptr), m_pView(nullptr)
{
}

IGraphicsWidget::~IGraphicsWidget()
{
}

void IGraphicsWidget::initLayout()
{
    QHBoxLayout *pWidgetLayout = new QHBoxLayout;
    pWidgetLayout->addWidget(m_pView, 3);
    auto pRightLayout = getRightLayout();
    if (pRightLayout != nullptr)
    {
        pWidgetLayout->addLayout(pRightLayout, 2);
    }
    pWidgetLayout->setSpacing(5);
    pWidgetLayout->setMargin(0);

    auto pToolBar = getToolBar();
    pToolBar->setFixedHeight(30);
    pToolBar->setStyleSheet("QToolBar{spacing:5px;}");
    pToolBar->setFloatable(false);
    pToolBar->setMovable(false);
    pToolBar->setAllowedAreas(Qt::LeftToolBarArea);
    pToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    pToolBar->layout()->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pToolBar);
    pLayout->addLayout(pWidgetLayout);
    auto pBottomLayout = getBottomLayout();
    if (pBottomLayout != nullptr)
    {
        pLayout->addLayout(pBottomLayout);
    }
    pLayout->setContentsMargins(5, 0, 5, 0);
    pLayout->setSpacing(5);
    this->setLayout(pLayout);
}

QLayout *IGraphicsWidget::getRightLayout()
{
    return nullptr;
}

QLayout *IGraphicsWidget::getBottomLayout()
{
    return nullptr;
}

void IGraphicsWidget::contextMenuEvent(QContextMenuEvent *)
{
    if (!m_menuActions.isEmpty())
    {
        QCursor cur = this->cursor();
        QMenu menu;
        menu.addActions(m_menuActions);
        menu.exec(cur.pos());
    }
}

QAction *IGraphicsWidget::addPushButtonAction(QPushButton *p_pPb)
{
    auto pAction = new QAction(p_pPb->icon(), p_pPb->text());
    pAction->setToolTip(p_pPb->toolTip());
    m_menuActions.append(pAction);
    connect(pAction, &QAction::triggered, p_pPb, &QPushButton::clicked);
    return pAction;
}

QPushButton *IGraphicsWidget::getPushButton(const QString &p_toolTip)
{
    auto pToolButton = TIGER_UIBasic::noKeyPushButton();
    pToolButton->setStyleSheet("QPushButton{padding:3px;}");
    pToolButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pToolButton->setIconSize(QSize(28, 28));
    pToolButton->setToolTip(p_toolTip);
    pToolButton->setFixedHeight(30);
    return pToolButton;
}

QToolButton *IGraphicsWidget::getToolPb(QString p_name, QString p_icon)
{
    auto pToolButton = new QToolButton;
    pToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pToolButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pToolButton->setIconSize(QSize(28, 28));
    pToolButton->setIcon(QIcon(p_icon));
    pToolButton->setToolTip(p_name);
    pToolButton->setFixedHeight(30);
    return pToolButton;
}

BS_QT_Ui::CCustomSlider *IGraphicsWidget::getSlider(double min, double max, double value, int precision)
{
    auto pSlider = new BS_QT_Ui::CCustomSlider;
    pSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pSlider->setDecimals(precision);
    pSlider->setRange(min, max);
    pSlider->setValue(value);
    return pSlider;
}