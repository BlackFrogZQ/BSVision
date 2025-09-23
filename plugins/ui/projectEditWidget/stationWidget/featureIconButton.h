#pragma once
#include "ui/iconTextButton/iconTextButton.h"
class QToolButton;
class CFeatureIconButton : public CIconTextButton
{
    Q_OBJECT
public:
    CFeatureIconButton(int *p_id, QWidget *parent = nullptr);
    ~CFeatureIconButton();
    void setCanEdit(bool p_edit);
signals:
    void sigDelFeature(int id);
    void sigEditFeature(int id);

protected:
    void initLyout();
    virtual void updateWidth() override;

protected:
    QToolButton *m_pDel;
    QToolButton *m_pEdit;
};