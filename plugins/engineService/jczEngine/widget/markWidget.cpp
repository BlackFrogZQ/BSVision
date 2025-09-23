#include "markWidget.h"
#include "../qssDef.h"
#include "workWidget.h"
#include "markItemWidget.h"

#include "template/templateHelper.h"

#include "Ui/uiHelper.h"
#include "message/msgManager.h"
#include "para/paraManager.h"

#include "system/toolFun.h"

#include <QSettings>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>

using namespace BS_QT_Ui;
using namespace BS_2DVision_Template;
using namespace BS_QT_Framework;
CJczMarkWidget::CJczMarkWidget(CJczWorkWidget *p)
    : QFrame(p), m_pWorkWidget(p)
{
    assert(m_pWorkWidget != nullptr);
    setObjectName(cJczMarkWidget);
    initLayout();
    itemChanged(-1);

    m_pEnableRoiItem->setChecked(false);
    m_pEnableRoiItem->toggled(false);

    connect(templateHelper(), &BS_2DVision_Template::CTemplateHelper::sigLoadFromFile, this, &CJczMarkWidget::updateFileMark, Qt::QueuedConnection);
    updateFileMark();

    connect(m_pLaserMoveToCameraBtn, &QPushButton::clicked, this, [this]
            {
                templateHelper()->m_marks.append(templateHelper()->m_globalMark);
                updateFileMark(); });
    connect(m_pCameraMoveToLaserBtn, &QPushButton::clicked, this, [this]
            { removMark(0); });
}

CJczMarkWidget::~CJczMarkWidget()
{
}

void CJczMarkWidget::updateFileMark()
{
    m_pListWidget->clear();
    m_marks.clear();

    for (auto &mark : templateHelper()->m_marks)
    {
        addMark(&mark);
    }

    updateInfo(0);
    if (m_pListWidget->currentItem() == nullptr)
    {
        m_pListWidget->setCurrentRow(m_marks.size() - 1);
    }
    else if (m_marks.size() >= 2)
    {
        itemChanged(m_pListWidget->currentRow());
    }

    m_pUsedGlobalMark->setChecked(templateHelper()->m_usedGlobalMark);
    m_pUsedGlobalMark->toggled(m_pUsedGlobalMark->isChecked());
}

void CJczMarkWidget::addMark(CMarkPoint *pMark)
{
    auto pItem = new CMarkItemWidget;
    auto pListItem = new QListWidgetItem(m_pListWidget);
    m_pListWidget->addItem(pListItem);
    m_pListWidget->setItemWidget(pListItem, pItem);
    m_marks.append({pMark, pItem});
}

void CJczMarkWidget::removMark(int id)
{
    if (id >= 0 && id < markCount())
    {
        QListWidgetItem *listItem = m_pListWidget->takeItem(id);
        if (listItem != nullptr)
        {
            templateHelper()->m_marks.removeAt(id);
            delete listItem;
            m_marks.removeAt(id);
            updateInfo(id);
            itemChanged(m_pListWidget->currentRow());
        }
    }
}

int CJczMarkWidget::markCount() const
{
    assert(m_marks.size() == m_pListWidget->count() && templateHelper()->m_marks.size() == m_marks.size());
    return m_pListWidget->count();
}

void CJczMarkWidget::initLayout()
{
    QWidget *pPbWidget = new QWidget(this);

    {
        m_pCameraMoveToLaserBtn = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Camera->Laser"), tr("Move the camera center to the current laser center."), QRect(), this);
        m_pLaserMoveToCameraBtn = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Laser->Camera"), tr("Move the laser center to the current camera center."), QRect(), this);
        QHBoxLayout *pPbayout = new QHBoxLayout(pPbWidget);
        pPbayout->addWidget(m_pLaserMoveToCameraBtn);
        pPbayout->addWidget(m_pCameraMoveToLaserBtn);
        pPbayout->setContentsMargins(3, 0, 3, 0);
        pPbayout->setSpacing(3);
        pPbWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    m_pRoiDelegate = new CRoiDelegate(m_pWorkWidget->m_pMeasureImageWidget->scene());
    m_pRoiDelegate->setColor(CParaManager::getVarColor(cRoiItemLineColor, Qt::red));
    connect(m_pRoiDelegate, &CRoiDelegate::sigRoiChanged, this, [this](QRectF p_roi)
            { if(m_currMarkPtr != nullptr) {m_currMarkPtr->roi = p_roi;} });
    connect(m_pRoiDelegate, &CRoiDelegate::sigUsedRoiChanged, this, [this](bool p_used)
            { if(m_currMarkPtr != nullptr) {m_currMarkPtr->usedRoi = p_used;} });
    connect(m_pRoiDelegate, &CRoiDelegate::sigUsedRoiChanged, this, [this](bool p_adjust)
            { if(m_currMarkPtr != nullptr) {m_currMarkPtr->adjustRoi = p_adjust;} });

    m_pUsedRoi = new QCheckBox(tr("ROI"), this);
    m_pUsedRoi->setCheckable(true);
    m_pUsedRoi->setObjectName(cJczMarkWidgetMarkCtrlCheckBox);
    m_pUsedRoi->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    m_pEnableRoiItem = new QCheckBox(tr("Adjust"), this);
    m_pEnableRoiItem->setCheckable(true);
    m_pEnableRoiItem->setEnabled(false);
    m_pEnableRoiItem->setObjectName(cJczMarkWidgetMarkCtrlCheckBox);

    connect(m_pUsedRoi, &QCheckBox::toggled, this, [this](bool p_on)
            {
                m_pEnableRoiItem->setEnabled(p_on);
                m_pRoiDelegate->setUsedRoi(p_on); });

    connect(m_pEnableRoiItem, &QCheckBox::toggled, this, [this](bool p_on)
            { m_pRoiDelegate->setAdjustRoi(p_on); });

    m_pUsedGlobalMark = new QCheckBox(tr("Used Global Mark"), this);
    m_pUsedGlobalMark->setCheckable(true);
    m_pUsedGlobalMark->setObjectName(cJczMarkWidgetMarkCtrlCheckBox);
    m_pUsedGlobalMark->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    connect(m_pUsedGlobalMark, &QCheckBox::toggled, this, [this](bool p_on)
            {
                templateHelper()->m_usedGlobalMark = p_on;
                m_pGlobalMarkParaWidget->setVisible(p_on);
                m_pMarkParaWidget->setVisible(!p_on);
                itemChanged(m_pListWidget->currentRow()); });

    m_pCurrentMarkCtrlBox = new QGroupBox(tr("Current mark"), this);
    m_pCurrentMarkCtrlBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    m_pCurrentMarkCtrlBox->setObjectName(cJczMarkWidgetMarkCtrlGroupBox);
    m_pCurrentMarkCtrlBox->setFlat(true);
    m_pCurrentMarkCtrlBox->setAlignment(Qt::AlignCenter);

    {
        m_pPreviousMarkBtn = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Previous"), tr("Previous mark point."), QRect(), this);
        m_pNextMarkBtn = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Next"), tr("Next mark point."), QRect(), this);
        m_pMoveToMarkCenter = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Move To Center"), tr("Move to mark center."), QRect(), this);
        m_pTestBtn = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Test"), tr("Test Current mark"), QRect(), this);
        m_pManualGetPos = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Manual Get Pos"), tr("Manually acquire mark point position."), QRect(), this);
        m_pManualGetPos->setCheckable(true);
        connect(m_pManualGetPos, &QPushButton::toggled, this, [this](bool checked)
                {
                    if (checked)
                    {
                        Out_Info << tr("%1->Start manual acquisition of real pos.").arg(m_marks[m_pListWidget->currentRow()].pItemWidget->title());
                        qApp->installEventFilter(this);
                        QApplication::setOverrideCursor(Qt::CrossCursor);
                    }
                    else
                    {
                        Out_Info << tr("%1->End manual acquisition of real pos.").arg(m_marks[m_pListWidget->currentRow()].pItemWidget->title());
                        qApp->removeEventFilter(this);
                        QApplication::restoreOverrideCursor();
                    } });
        m_pAlignFirstMarkCenter = CUIHelper::NewPushButton(cJczMarkWidgetBtn, tr("Coord Align Center"), tr("Coordinate align center point."), QRect(), this);

        m_pMarkParaWidget = new CMarkTemplateWidget(this);
        m_pMarkParaWidget->layout()->setMargin(0);
        m_pMarkParaWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        QGridLayout *pPbayout = new QGridLayout(m_pCurrentMarkCtrlBox);
        pPbayout->addWidget(m_pPreviousMarkBtn, 0, 0, 1, 3);
        pPbayout->addWidget(m_pMoveToMarkCenter, 0, 3, 1, 3);

        pPbayout->addWidget(m_pNextMarkBtn, 1, 0, 1, 3);
        pPbayout->addWidget(m_pManualGetPos, 1, 3, 1, 3);
        pPbayout->addWidget(m_pAlignFirstMarkCenter, 2, 1, 1, 4);

        pPbayout->addWidget(m_pUsedRoi, 3, 0, 1, 2);
        pPbayout->addWidget(m_pEnableRoiItem, 3, 2, 1, 2);
        pPbayout->addWidget(m_pTestBtn, 3, 4, 1, 2);
        pPbayout->addWidget(m_pMarkParaWidget, 4, 0, 1, 6);

        pPbayout->setContentsMargins(3, 0, 3, 0);
        pPbayout->setSpacing(3);
        m_pCurrentMarkCtrlBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        connect(m_pPreviousMarkBtn, &QPushButton::clicked, this, [this]
                {
                    auto count = markCount();
                    m_pListWidget->setCurrentRow((m_pListWidget->currentRow() - 1 + count)%count); });
        connect(m_pNextMarkBtn, &QPushButton::clicked, this, [this]
                {
                    auto count = markCount();
                    m_pListWidget->setCurrentRow((m_pListWidget->currentRow() + 1)%count); });
        connect(m_pTestBtn, &QPushButton::clicked, this, &CJczMarkWidget::testMark, Qt::QueuedConnection);
    }

    m_pListWidget = new QListWidget(this);
    m_pUsedRoi->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_pListWidget->setObjectName(cJczMarkWidgetListWidget);
    m_pListWidget->setItemAlignment(Qt::AlignCenter);
    m_pListWidget->setViewMode(QListView::ListMode);
    m_pListWidget->setMovement(QListView::Static);
    m_pListWidget->setResizeMode(QListView::ResizeMode::Adjust);
    m_pListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_pListWidget->setSpacing(3);
    connect(m_pListWidget, &QListWidget::currentRowChanged, this, &CJczMarkWidget::itemChanged);

    m_pGlobalMarkParaWidget = new CMarkTemplateWidget(this);
    m_pGlobalMarkParaWidget->layout()->setContentsMargins(3, 0, 3, 0);
    m_pGlobalMarkParaWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(m_pUsedGlobalMark);
    pLayout->setAlignment(m_pUsedGlobalMark, Qt::AlignCenter);
    pLayout->addWidget(m_pGlobalMarkParaWidget);
    pLayout->addWidget(m_pListWidget, 7);
    pLayout->addWidget(pPbWidget);
    pLayout->addWidget(m_pCurrentMarkCtrlBox);
    pLayout->setContentsMargins(0, 3, 0, 3);
    pLayout->setSpacing(3);
}

void CJczMarkWidget::updateInfo(int p_start)
{
    for (int i = p_start; i < markCount(); i++)
    {
        auto newPos = m_marks[i].pMark->newPos;
        auto oriPos = m_marks[i].pMark->oriPos;
        m_marks[i].pItemWidget->setNewPos(newPos.x(), newPos.y());
        m_marks[i].pItemWidget->setOriPos(oriPos.x(), oriPos.y());
        m_marks[i].pItemWidget->setTitle(tr("Mark%1").arg(i + 1));
    }
}

void CJczMarkWidget::itemChanged(int p_id)
{
    this->setEnabled(p_id >= 0);
    CMarkPoint *pMark = nullptr;
    CMarkTemplateWidget *pMarkWidget = nullptr;
    m_currMarkPtr = nullptr;

    if (p_id >= 0)
    {
        m_pAlignFirstMarkCenter->setVisible(p_id == 0);
        m_pCurrentMarkCtrlBox->setTitle(m_marks[p_id].pItemWidget->title());
        pMark = m_marks[p_id].pMark;
        pMarkWidget = m_pMarkParaWidget;
    }

    if (m_pUsedGlobalMark->isChecked())
    {
        pMark = &templateHelper()->m_globalMark;
        pMarkWidget = m_pGlobalMarkParaWidget;
    }

    if (pMark != nullptr)
    {
        pMarkWidget->attachMark(pMark);
        m_pRoiDelegate->setRoi(pMark->roi);
        m_pEnableRoiItem->setChecked(pMark->adjustRoi);
        m_pUsedRoi->setChecked(pMark->usedRoi);
    }
    m_currMarkPtr = pMark;
}

void CJczMarkWidget::testMark()
{
    using namespace TIGER_Graphics;
    using namespace BS_2DVision_Template;
    bool successful = templateHelper()->grabImageMatch(
        m_pUsedGlobalMark->isChecked()
            ? CTemplateHelper::GlobalMarkId
            : m_pListWidget->currentRow(),
        1);
    auto image = templateHelper()->matchResultImage();
    auto results = templateHelper()->matchResult();
    auto regions = QList<CGraphicsRegion>();

    if (successful)
    {
        auto region = CGraphicsRegion(tr("Result"), Qt::red);
        for (auto result : results)
        {
            region.points << result.pixPts.toList();
        }
        regions << region;
        Out_Info << tr("Vision result") + ":{" +
                        listToString(results.toList(), [](const BS_2DVision_Template::CMatchResult &p)
                                     { return QString("(Pos:{%1},A:%3°,S:%4)")
                                           .arg(listToString(p.pos.toList(), [](const QPointF &p)
                                                             { return QString("(X:%1,Y:%2").arg(dToStr(p.x(), 3)).arg(dToStr(p.y(), 3)); })
                                                    .join(";"))
                                           .arg(dToStr(p.angle, 3))
                                           .arg(dToStr(p.score, 3)); })
                            .join(",") +
                        "}.";
    }
    else
    {
        Out_Warning << templateHelper()->errorMsg();
    }
    saveImageToSysDir(image, "testMarkImage", false, regions, templateHelper()->norRoi());
    templateHelper()->clearCache();
    m_pWorkWidget->updateOrderImage(image, regions);
}

bool CJczMarkWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (m_pManualGetPos->isChecked())
    {
        if (event->type() == QEvent::KeyPress)
        {
            auto pKeyEvent = static_cast<QKeyEvent *>(event);
            if (pKeyEvent->key() == Qt::Key_Escape)
            {
                m_pManualGetPos->setChecked(false);
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            auto pMouseEvent = static_cast<QMouseEvent *>(event);
            if ((pMouseEvent->button() != Qt::LeftButton) || this->childAt(this->mapFromGlobal(pMouseEvent->globalPos())) == m_pManualGetPos)
            {
                return false;
            }
            double x = 0, y = 0;
            if (m_pWorkWidget->m_pImageWidget->pixelLinkPos(pMouseEvent->globalPos(), x, y))
            {
                Out_Info << tr("%1->Acquire real pos(%2,%3)").arg(m_marks[m_pListWidget->currentRow()].pItemWidget->title()).arg(dToStr(x, 3)).arg(dToStr(y, 3));
                // TODO 坐标转化
                m_marks[m_pListWidget->currentRow()].pItemWidget->setNewPos(x, y);
                m_marks[m_pListWidget->currentRow()].pMark->newPos = QPointF(x, y);
                m_pManualGetPos->setChecked(false);
            }
            return true;
        }
        switch (event->type())
        {
        case QEvent::HoverEnter:
        case QEvent::HoverLeave:
        case QEvent::HoverMove:
        case QEvent::ToolTip:
        case QEvent::Enter:
            return true;
            break;
        default:
            break;
        }
    }
    return QFrame::eventFilter(watched, event);
}
