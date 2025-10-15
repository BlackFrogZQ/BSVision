#include "navigationBar.h"
#include "ui/basic.h"

#include "bsBasic.h"
#include "Ui/uiHelper.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

const QString cPbNormalStyle =
    "";
const QString cPbCurrentStyle =
    "background:rgb(193, 255, 193);";

CNavigationBar::CNavigationBar(QWidget *parent)
    : QWidget(parent), m_currentId(-1), m_editMaxId(0)
{
    this->setFixedHeight(30);
    this->setStyleSheet(cPbNormalStyle);
    initLayout();
}

CNavigationBar::~CNavigationBar()
{
}

int CNavigationBar::getPageCount() const
{
    return m_pPbList.size();
}

int CNavigationBar::getCurrentPage() const
{
    return m_currentId;
}

int CNavigationBar::getEditMaxPage() const
{
    return m_editMaxId;
}

void CNavigationBar::addPage(const QString &p_name)
{
    if (getPageCount() != 0)
    {
        QLabel *pLabel = new QLabel("==>");
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pLabelList.append(pLabel);
    }

    QPushButton *pButton = TIGER_UIBasic::noKeyPushButton(p_name);
    pButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(pButton, &QPushButton::clicked, this, [id = m_pPbList.size(), this]
            { emit slotButtonGroupClicked(id); });
    m_pPbList.append({p_name, pButton});
    initLayout();
    setEditState(m_editMaxId, false);
}

void CNavigationBar::insertPage(const int &p_id, const QString &p_name)
{
    if (getPageCount() != 0)
    {
        QLabel *pLabel = new QLabel("==>");
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pLabelList.append(pLabel);
    }
    QPushButton *pButton = TIGER_UIBasic::noKeyPushButton(p_name);
    pButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pPbList.insert(p_id, {p_name, pButton});
    for (int i = 0; i < m_pPbList.size(); i++)
    {
        m_pPbList[i].pPb->disconnect(SIGNAL(clicked()));
        connect(m_pPbList[i].pPb, &QPushButton::clicked, this, [id = i, this]
                { emit slotButtonGroupClicked(id); });
    }
    initLayout();
    setEditState(p_id, false);
}

void CNavigationBar::removePage(const int &p_id)
{
    assert(p_id >= 0 && p_id < getPageCount());
    if (p_id <= m_editMaxId)
    {
        m_editMaxId--;
    }
    delPtr(m_pPbList[p_id].pPb);
    m_pPbList.removeAt(p_id);
    for (int i = 0; i < m_pPbList.size(); i++)
    {
        m_pPbList[i].pPb->disconnect();
        connect(m_pPbList[i].pPb, &QPushButton::clicked, this, [id = i, this]
                { emit slotButtonGroupClicked(id); });
    }
    if (p_id < m_pLabelList.size())
    {
        delPtr(m_pLabelList[p_id]);
        m_pLabelList.removeAt(p_id);
    }
    initLayout();
}

bool CNavigationBar::getIsEditFinished(const int &p_id)
{
    assert(p_id >= 0 && p_id < getPageCount());
    return m_pPbList[p_id].pPb->isEnabled();
}

void CNavigationBar::setEditState(int p_id, bool p_editFinished, bool sendSig)
{
    assert(p_id >= 0 && p_id < getPageCount());
    if (p_editFinished)
    {
        if (p_id < (getPageCount() - 1))
        {
            m_pPbList[p_id + 1].pPb->setEnabled(p_editFinished);
        }
        m_editMaxId = qMax(m_editMaxId, p_id + 1);
    }
    else
    {
        m_editMaxId = p_id;
        for (int i = p_id + 1; i < getPageCount(); i++)
        {
            m_pPbList[i].pPb->setEnabled(p_editFinished);
        }
    }
    if (sendSig)
    {
        emit sigTrySave(m_editMaxId == getPageCount());
    }
}

void CNavigationBar::setCurrentPage(int p_id)
{
    assert(p_id >= 0 && p_id < getPageCount());
    if (p_id <= m_editMaxId)
    {
        slotButtonGroupClicked(p_id);
    }
}

void CNavigationBar::initLayout()
{
    BS_QT_Ui::CUIHelper::deleteLayout(layout());
    QHBoxLayout *pLayout = new QHBoxLayout;
    for (size_t i = 0; i < getPageCount(); i++)
    {
        if (i > 0)
        {
            pLayout->addWidget(m_pLabelList[i - 1], 1);
        }
        pLayout->addWidget(m_pPbList[i].pPb, 1);
    }
    pLayout->setMargin(0);
    this->setLayout(pLayout);
}

void CNavigationBar::slotButtonGroupClicked(int p_id)
{
    if (m_currentId != p_id)
    {
        int oldId = m_currentId;
        m_currentId = p_id;
        for (auto pW : m_pPbList)
        {
            pW.pPb->setStyleSheet(cPbNormalStyle);
            pW.pPb->setText(pW.text);
        }
        m_pPbList[m_currentId].pPb->setStyleSheet(cPbCurrentStyle);
        m_pPbList[m_currentId].pPb->setText(m_pPbList[m_currentId].text + "*");
        emit sigCurrentId(oldId, m_currentId);
    }
}