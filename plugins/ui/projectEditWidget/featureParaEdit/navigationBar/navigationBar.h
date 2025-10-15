#pragma once
#include <QWidget>
class QPushButton;
class QLabel;
class CNavigationBar : public QWidget
{
    Q_OBJECT
public:
    CNavigationBar(QWidget *parent = nullptr);
    ~CNavigationBar();
    int getPageCount() const;
    int getCurrentPage() const;
    int getEditMaxPage() const;
    void addPage(const QString &p_name);
    void insertPage(const int &p_id, const QString &p_name);
    void removePage(const int &p_id);
    bool getIsEditFinished(const int &p_id);
    void setEditState(int p_id, bool p_editFinished, bool sendSig = true);
    void setCurrentPage(int p_id);

signals:
    void sigCurrentId(int oldId,int p_id);
    void sigTrySave(bool p_isSave);
protected slots:
    void slotButtonGroupClicked(int p_id);

protected:
    void initLayout();

protected:
    struct CPage
    {
        QString text;
        QPushButton *pPb;
    };

protected:
    QList<QLabel *> m_pLabelList;
    QList<CPage> m_pPbList;
    int m_currentId;
    int m_editMaxId;
};
