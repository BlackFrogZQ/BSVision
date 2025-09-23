#pragma once
#include <QFrame>
class QPushButton;
class QListWidget;
class QGroupBox;
class QLabel;
class CMarkItemWidget : public QFrame
{
    Q_OBJECT
public:
    CMarkItemWidget(QWidget *p = nullptr);
    ~CMarkItemWidget();
    QString title() const;

public slots:
    void setOriPos(double x, double y);
    void setNewPos(double x, double y);
    void setTitle(QString p_title);

protected:
    void initLayout();
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    QPushButton *m_pMoveToOri = nullptr;
    QPushButton *m_pMoveToNew = nullptr;
    QLabel *m_pOriPos = nullptr;
    QLabel *m_pNewPos = nullptr;
    QGroupBox *m_pGroupBox;
};
