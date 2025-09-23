#pragma once
#include "workWidget.h"
#include "Ui/widget/tabWidget.h"

class CJczMainWidget : public QWidget
{
    Q_OBJECT
public:
    CJczMainWidget(QWidget *p = nullptr);
    ~CJczMainWidget();
    void init();

signals:
    void sigOnlyShow(bool p_only);

public slots:
    void onlyShowTabWidget(bool p_only,QWidget* pWnd = nullptr);

protected:
    void initLayout();

public:
    BS_QT_Ui::CCustomTabWidget *m_pTagWidget = nullptr;
    CJczWorkWidget *m_pWorkWindow = nullptr;
};

CJczMainWidget *jczMainWindow();
