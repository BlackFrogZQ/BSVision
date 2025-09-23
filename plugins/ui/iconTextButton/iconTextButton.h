#pragma once
#include <QLabel>
class CEditLabel;
class CIconTextButton : public QWidget
{
    Q_OBJECT
public:
    CIconTextButton(int *p_id, QWidget *parent = nullptr);
    virtual ~CIconTextButton();
    void setCanEdit(bool p_canEdit = false);
    void setIconSize(QSize p_size);
    void setIcon(QPixmap p_icon);
    void setText(QString p_text);
    QString text() const;
    QPixmap icon() const;
    int id() const;
    void resetText();
    void setDefText(QString p_text);
    void updateIcon();
    void setSelect(bool p_select);
    static QPixmap getIcon(QPixmap p_icon,QSize p_iconSize,int id);
signals:
    void sigClicked(int id);
    void sigMouseDoubleClick(int id);
    void sigTextChanged(int id, QString p_text);

protected:
    virtual void mousePressEvent(QMouseEvent *p_event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *p_event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void updateWidth();
protected:
    QPixmap m_icon;
    int *m_pId;
    QSize m_iconSize;
    QColor m_color;
    QLabel *m_pIconLabel;
    CEditLabel *m_pTitleLabel;
    bool m_select;
    bool m_canEdit;
    bool m_isDoubleClickd;
};

#include <QProxyStyle>

class CMenuIconStyle : public QProxyStyle
{
    Q_OBJECT

public:
    CMenuIconStyle();
    ~CMenuIconStyle();

    void setCustomSize(int);
    // protected:
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;

private:
    int m_nSize;
};