#include "markItemWidget.h"
#include "../qssDef.h"

#include "Ui/uiHelper.h"
#include "message/msgManager.h"

#include <QMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
using namespace BS_QT_Ui;
CMarkItemWidget::CMarkItemWidget(QWidget *p)
    : QFrame(p)
{
    initLayout();
    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pGroupBox);
    pLayout->setMargin(3);
    pLayout->setSpacing(0);
    pLayout->setAlignment(Qt::AlignCenter);
    setOriPos(10000, 10000);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

CMarkItemWidget::~CMarkItemWidget()
{
}

QString CMarkItemWidget::title() const
{
    return m_pGroupBox->title();
}

void CMarkItemWidget::initLayout()
{
    m_pGroupBox = new QGroupBox(this);
    m_pGroupBox->setObjectName(cJczMarkWidgetMarkItemGroupBox);
    m_pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_pGroupBox->setAlignment(Qt::AlignCenter);
    auto getLabel = [this](QString text = "")
    {
        auto pLabel = new QLabel(text);
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        return pLabel;
    };
    m_pMoveToOri = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Design Pos"), tr("Go to the design pos"), QRect(), this);
    m_pMoveToOri->installEventFilter(this);
    m_pMoveToOri->setEnabled(false);
    connect(m_pMoveToOri, &QPushButton::clicked, this, [this]
            { Out_Info << QString("Go to the design pos:%1").arg(m_pOriPos->text()); });

    m_pMoveToNew = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Real Pos"), tr("Go to the real pos"), QRect(), this);
    m_pMoveToNew->installEventFilter(this);
    m_pMoveToNew->setEnabled(false);
    connect(m_pMoveToNew, &QPushButton::clicked, this, [this]
            { Out_Info << QString("Go to the real pos:%1").arg(m_pNewPos->text()); });
    m_pOriPos = getLabel();
    m_pNewPos = getLabel();

    QGridLayout *pLayout = new QGridLayout(m_pGroupBox);
    pLayout->addWidget(m_pMoveToOri, 0, 0, 1, 1);
    pLayout->addWidget(m_pOriPos, 0, 1, 1, 2);
    pLayout->addWidget(m_pMoveToNew, 1, 0, 1, 1);
    pLayout->addWidget(m_pNewPos, 1, 1, 1, 2);
    pLayout->setMargin(5);
    pLayout->setSpacing(3);
}

void CMarkItemWidget::setOriPos(double x, double y)
{
    m_pOriPos->setText(QString("%1,%2").arg(dToStr(x, 3)).arg(dToStr(y, 3)));
    if (!m_pMoveToOri->isEnabled())
    {
        m_pMoveToOri->setEnabled(true);
    }
}

void CMarkItemWidget::setNewPos(double x, double y)
{
    m_pNewPos->setText(QString("%1,%2").arg(dToStr(x, 3)).arg(dToStr(y, 3)));
    if (!m_pMoveToNew->isEnabled())
    {
        m_pMoveToNew->setEnabled(true);
    }
}

void CMarkItemWidget::setTitle(QString p_title)
{
    m_pGroupBox->setTitle(p_title);
}

bool CMarkItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        const QList<QWidget *> cPbs =
            {
                m_pMoveToOri,
                m_pMoveToNew};
        auto pMouseEvent = static_cast<QMouseEvent *>(event);
        for (auto pW : cPbs)
        {
            if (pW == watched)
            {
                QApplication::sendEvent(this, pMouseEvent);
                break;
            }
        }
    }
    return QFrame::eventFilter(watched, event);
}