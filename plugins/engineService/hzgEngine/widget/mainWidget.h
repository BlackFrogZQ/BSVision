#pragma once
#include <QWidget>
#include "Ui/widget/tabWidget.h"

class CProjectEditWidget;
class CStdWorkWidget;
class CHzgMainWidget : public QWidget
{
    Q_OBJECT
public:
    CHzgMainWidget(QWidget *p = nullptr);
    ~CHzgMainWidget();

    void init();

protected:
    void initLayout();

public slots:
    void showOnlyEditWindow(bool p_only);

public:
    BS_QT_Ui::CCustomTabWidget *m_pTagWidget;
    CStdWorkWidget *m_pWorkWindow;
    CProjectEditWidget *m_pProjectEditWidget;
};

CHzgMainWidget *hzgMainWindow();
