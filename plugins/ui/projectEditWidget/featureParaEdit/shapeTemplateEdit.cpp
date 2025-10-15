#include "shapeTemplateEdit.h"
#include "./navigationBar/navigationBar.h"
#include "ui/graphicsWidget/editTemplateWidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStackedWidget>

CShapeTemplateParaEdit::CShapeTemplateParaEdit(QWidget *parent) : IFeatureParaEdit(parent)
{
    initLayout();
}

CShapeTemplateParaEdit::~CShapeTemplateParaEdit()
{
}

void CShapeTemplateParaEdit::initLayout()
{
    const double cSize = 30;

    m_pCancelButton = TIGER_UIBasic::noKeyPushButton(tr("取消"));
    m_pCancelButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pCancelButton->setFixedHeight(cSize);
    connect(m_pCancelButton, &QPushButton::clicked, this, &CShapeTemplateParaEdit::close);

    m_pSaveButton = TIGER_UIBasic::noKeyPushButton(tr("确定"));
    m_pSaveButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_pSaveButton->setFixedHeight(cSize);
    m_pSaveButton->setEnabled(false);
    connect(m_pSaveButton, &QPushButton::clicked, this, &CShapeTemplateParaEdit::slotSave);

    m_pNavigationBar = new CNavigationBar;
    enum CBarButtonType
    {
        cbbtTemplate = 0,
        cbbtEditGray,
        cbbtEditRoi,
        cbbtEditPosition,
    };
    m_pNavigationBar->addPage(tr("模板划分"));
    m_pNavigationBar->addPage(tr("灰度划分"));
    m_pNavigationBar->addPage(tr("划分匹配区域"));
    m_pNavigationBar->addPage(tr("定位"));
    m_pNavigationBar->setCurrentPage(0);
    connect(m_pNavigationBar, &CNavigationBar::sigTrySave, this, [pSaveButton = m_pSaveButton](bool p_isSave)
            { pSaveButton->setEnabled(p_isSave); });
    connect(m_pNavigationBar, &CNavigationBar::sigCurrentId, this, &CShapeTemplateParaEdit::slotChangePage);
    // connect(m_pNavigationBar, &CNavigationBar::sigCurrentId, m_pEditGrayWindow, [this](int oldId,int p_id)
    //         {
    //             if (oldId == cbbtEditGray)
    //             {
    //                 m_pEditGrayWindow->stopDrawing();
    //             } });
    // connect(m_pNavigationBar, &CNavigationBar::sigCurrentId, m_pEditTemplateWindow, [this](int oldId,int p_id)
    //         {
    //             if (oldId == cbbtTemplate)
    //             {
    //                 m_pEditTemplateWindow->stopDrawing();
    //             } });

    m_pEditTemplateWindow = new CEditTemplateWidget(this);
    m_pEditTemplateWindow->initLayout();

    m_pStackedWidget = new QStackedWidget(this);
    m_pStackedWidget->addWidget(m_pEditTemplateWindow);

    QHBoxLayout *pButtonLayout = new QHBoxLayout;
    auto pLabel = new QLabel("|");
    auto font = QFont("SimSun", 13);
    font.setBold(true);
    pLabel->setFont(font);
    pButtonLayout->addWidget(m_pNavigationBar);
    pButtonLayout->addWidget(pLabel);
    pButtonLayout->addWidget(m_pSaveButton);
    pButtonLayout->addWidget(m_pCancelButton);
    pButtonLayout->setMargin(0);
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addStretch();
    pMainLayout->addLayout(pButtonLayout);
    pMainLayout->addWidget(m_pStackedWidget);
    pMainLayout->addStretch();
    pMainLayout->setMargin(2);
    pMainLayout->setSpacing(0);
    this->setLayout(pMainLayout);
}

void CShapeTemplateParaEdit::slotSave()
{
    m_isSave = true;
    this->close();
}

void CShapeTemplateParaEdit::slotChangePage(int p_oldId, int p_newId)
{
    assert(p_newId >= 0 && p_newId < m_pStackedWidget->count());
    // if (p_oldId >= 0 && m_pStackedWidget->widget(p_oldId) == m_pEditTemplateWindow)
    // {
    //     m_pEditTemplateWindow->updateResult();
    // }
    m_pStackedWidget->setCurrentIndex(p_newId);
}

void CShapeTemplateParaEdit::closeEvent(QCloseEvent *event)
{
    if (m_isSave)
    {
        // m_pEditGrayWindow->updateResult();
        // m_pEditRoiWindow->updateResult();
        // m_pEditPositionWindow->updateResult();
    }
    // m_pEditTemplateWindow->updateResult();
    QWidget::closeEvent(event);
    emit sigClose();
}