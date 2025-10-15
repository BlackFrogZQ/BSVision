#pragma once
#include "featureParaEdit.h"
class CNavigationBar;
class CEditTemplateWidget;
class CEditRoiWindow;
class CEditGrayWindow;
class CEditPositionWindow;
class QStackedWidget;
class QPushButton;
struct CSingleTemplate;
class CShapeTemplateParaEdit : public IFeatureParaEdit
{
    Q_OBJECT
public:
    CShapeTemplateParaEdit(QWidget *parent = nullptr);
    ~CShapeTemplateParaEdit();
    // void setSingleTemplate(CSingleTemplate *p_features);

protected slots:
    void slotSave();
    void slotChangePage(int p_oldId,int p_newId);

protected:
    void initLayout();
    virtual void closeEvent(QCloseEvent *event) override;

protected:
    CNavigationBar *m_pNavigationBar;
    CEditPositionWindow *m_pEditPositionWindow;
    CEditRoiWindow *m_pEditRoiWindow;
    CEditGrayWindow *m_pEditGrayWindow;
    CEditTemplateWidget *m_pEditTemplateWindow;

    QStackedWidget *m_pStackedWidget;
    QPushButton *m_pCancelButton;
    QPushButton *m_pSaveButton;
};
