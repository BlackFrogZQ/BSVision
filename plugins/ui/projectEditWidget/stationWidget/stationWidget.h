#pragma once
#include <QWidget>
class IFeatureParaEdit;
class CFeatureIconButton;
class CShowAutoImageLabel;
struct CStation;
struct CFeature;

class QToolButton;
class QLabel;
class CStationWidget : public QWidget
{
    Q_OBJECT
public:
    CStationWidget(QWidget *p = nullptr);
    ~CStationWidget();
    void setStation(CStation *p_station);
    void setCanEdit(bool p_edit);

signals:
    void sigFeatureChanged();
    void sigEditFeature(IFeatureParaEdit *p_editFeature);

public slots:
    void slotNewFeature();

protected slots:
    void slotEditFeature(int p_id);
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    void updateToolBarLayout();
    void showFeature(CFeature *pFeature);
    void updateFeatureIcon();
    CFeatureIconButton *addFeatureToolButton(CFeature *pFeature);

protected:
    QList<CFeatureIconButton *> m_featureIconButtons;
    CStation *m_station;
    QToolButton *m_pAddFeature;
    QLabel *m_pFeatureToolBar;
    CShowAutoImageLabel *m_pShowAutoImageLabel;
};
