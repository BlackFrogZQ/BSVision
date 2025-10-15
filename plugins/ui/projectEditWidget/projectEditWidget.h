#pragma once
#include "../basic.h"
#include <QWidget>

class QToolButton;
class QLabel;
class CProjectManager;
class CIconTextButton;
class CStationWidget;
class IFeatureParaEdit;

class CProjectEditWidget : public QWidget
{
    Q_OBJECT
public:
    CProjectEditWidget(QWidget *p_pParent = nullptr);
    ~CProjectEditWidget();

public slots:
    void newStation();

protected slots:
    void slotEditFeaturePara(IFeatureParaEdit *p_pFeatureParaEdit);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *e) override;

protected:
    void init();
    void initLayout();
    void initToolBarLayout();
    void updateStationToolLayout();
    void stationContextMenu(CIconTextButton *pIconTextButton);

protected:
    QLabel *m_pToolBar;
    QLabel *m_pStationToolBar;

    QToolButton *m_pSaveButton;
    QToolButton *m_pSaveAsButton;
    QToolButton *m_pCancelButton;
    QToolButton *m_pNewStationButton;

    CStationWidget *m_pStationWidget;
    CProjectManager *m_pProjectManager;
    QList<CIconTextButton*> m_pStationToolButtons;

    bool m_bIsChanged;
    bool m_canEdit;
};