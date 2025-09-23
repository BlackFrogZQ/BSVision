#pragma once
#include <QLabel>
class QStackedWidget;
class QLineEdit;

class CEditLabel : public QWidget
{
    Q_OBJECT
public:
    CEditLabel(const QString &text = QString(), QWidget *parent=nullptr);
    ~CEditLabel();
    void setText(QString p_text);
    QString text() const;
    void resetText();
    void setDefText(QString p_text);
    QString getDefText() const;
    void setAlignment(Qt::Alignment p_aalignment = Qt::AlignCenter);
    void setCanEdit(bool p_can  = true);

signals:
    void editingFinished();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *e) override;

protected:
    QString m_text;
    QStackedWidget *m_stackWidget;
    QLabel *m_label;
    QLineEdit *m_lineEdit;
    bool m_canEdit;
};


