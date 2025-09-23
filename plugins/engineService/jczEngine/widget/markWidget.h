#pragma once
#include <QFrame>
#include "markTemplateWidget.h"
#include "ui/imageWidget/roiDelegate.h"
#include "template/markParaDef.h"

class QPushButton;
class QGroupBox;
class QComboBox;
class QCheckBox;
class QStackedWidget;
class QListWidget;
class CJczWorkWidget;
class CMarkItemWidget;
class CMarkTemplateWidget;

class CJczMarkWidget : public QFrame
{
    Q_OBJECT
public:
    CJczMarkWidget(CJczWorkWidget *p = nullptr);
    ~CJczMarkWidget();
    int markCount() const;

protected slots:
    void addMark(BS_2DVision_Template::CMarkPoint* pMark);
    void removMark(int id);

protected:
    void initLayout();
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected slots:
    void updateFileMark();
    void updateInfo(int p_start = 0);
    void itemChanged(int p_id);
    void testMark();

protected:
    struct CMarkPointInfoWidget
    {
        BS_2DVision_Template::CMarkPoint* pMark = nullptr;
        CMarkItemWidget* pItemWidget = nullptr;
    };

protected:
    QGroupBox *m_pCurrentMarkCtrlBox = nullptr;
    QPushButton *m_pPreviousMarkBtn = nullptr;
    QPushButton *m_pNextMarkBtn = nullptr;
    QPushButton *m_pMoveToMarkCenter = nullptr;
    QPushButton *m_pManualGetPos = nullptr;
    QPushButton *m_pAlignFirstMarkCenter = nullptr;
    QCheckBox *m_pUsedRoi = nullptr;
    QCheckBox *m_pEnableRoiItem = nullptr;
    CRoiDelegate *m_pRoiDelegate = nullptr;

    QPushButton *m_pLaserMoveToCameraBtn = nullptr;
    QPushButton *m_pCameraMoveToLaserBtn = nullptr;

    QPushButton *m_pTestBtn = nullptr;

    QListWidget *m_pListWidget = nullptr;
    QList<CMarkPointInfoWidget> m_marks;
    CJczWorkWidget *m_pWorkWidget;

    CMarkTemplateWidget *m_pMarkParaWidget;
    CMarkTemplateWidget *m_pGlobalMarkParaWidget;

    QCheckBox *m_pUsedGlobalMark = nullptr;
    BS_2DVision_Template::CMarkPoint* m_currMarkPtr = nullptr;
};
