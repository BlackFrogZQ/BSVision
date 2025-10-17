#include "setTemplateParaWindow.h"
#include <QPushButton>
#include <QHBoxLayout>
ISetTemplatePParaWindow::ISetTemplatePParaWindow(QWidget *parent)
    : TIGER_UIBasic::INoExcCancelDialog(parent)
{
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint, true);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
}

ISetTemplatePParaWindow::~ISetTemplatePParaWindow()
{
}

void ISetTemplatePParaWindow::initLayout()
{
    auto pSave = TIGER_UIBasic::noKeyPushButton(QObject::tr("保存"));
    connect(pSave, &QPushButton::clicked, this, [this]()
            {updatePatas(); m_isSave = true;this->close(); });
    auto pCancel = TIGER_UIBasic::noKeyPushButton(QObject::tr("取消"));
    connect(pCancel, &QPushButton::clicked, this, [this]()
            {m_isSave = false;this->close(); });
    QHBoxLayout *pbLayout = new QHBoxLayout;
    pbLayout->addStretch();
    pbLayout->addWidget(pSave);
    pbLayout->addWidget(pCancel);
    pbLayout->setMargin(0);
    QVBoxLayout *hLayout = new QVBoxLayout;
    hLayout->addLayout(initPataLayout());
    hLayout->addLayout(pbLayout);
    hLayout->setMargin(5);
    hLayout->setSpacing(0);
    this->setLayout(hLayout);
}

bool ISetTemplatePParaWindow::isSave()
{
    m_isSave = false;
    initLayout();
    initPatas();
    this->exec();
    return m_isSave;
}
