#pragma once
#include "ui/basic.h"
class ISetTemplatePParaWindow : public TIGER_UIBasic::INoExcCancelDialog
{
public:
    ISetTemplatePParaWindow(QWidget *parent = nullptr);
    virtual ~ISetTemplatePParaWindow();
    bool isSave();

protected:
    void initLayout();
    virtual QLayout *initPataLayout() = 0;
    virtual void initPatas() = 0;
    virtual void updatePatas() = 0;

protected:
    bool m_isSave;
};



