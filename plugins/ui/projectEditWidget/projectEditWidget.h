#pragma once
#include <QWidget>

class QLabel;
class QToolButton;
class CStationWidget;
class CIconTextButton;

class CProjectEditWidget : public QWidget
{
    Q_OBJECT
public:
    CProjectEditWidget(QWidget *p_pParent = nullptr);
    ~CProjectEditWidget();

protected:
    void init();
    void initLayout();
    void initToolBarLayout();
    void updateStationToolLayout();

protected:
    QLabel *m_pToolBar;
    QLabel *m_pStationToolBar;
    QToolButton *m_pNewStationButton;
    QToolButton *m_pSaveButton;
    QToolButton *m_pCancelButton;
    QToolButton *m_pSaveAsButton;
    CStationWidget *m_pStationWidget;
    QList<CIconTextButton*> m_pStationToolButtons;
};