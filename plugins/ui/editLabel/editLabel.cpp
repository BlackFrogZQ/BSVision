#include "editLabel.h"
#include <QStackedWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QApplication>
#include <QRegExpValidator>
#include <QEvent>
#include <QDebug>
CEditLabel::CEditLabel(const QString &text, QWidget *parent) : QWidget(parent), m_text(text), m_canEdit(true)
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    m_stackWidget = new QStackedWidget(this);
    m_stackWidget->setFont(this->font());
    m_label = new QLabel(text);
    m_label->setFont(this->font());
    // m_label->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    m_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_label->setStyleSheet("");
    m_lineEdit = new QLineEdit;
    m_lineEdit->setValidator(new QRegExpValidator(QRegExp("^((?![\\\\\\^%\\:&',/\\*\\!<>;\\|=\\?$\x22]).)*$"), this));
    m_lineEdit->setFont(this->font());
    m_lineEdit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_lineEdit->setMinimumSize(0, 0);
    m_lineEdit->setStyleSheet(
        "QLineEdit{border: none;background:transparent;color:rgb(0,0,0)}"
        "QLineEdit:focus{background:rgb(255,255,255);color:red}");
    m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    connect(m_lineEdit, &QLineEdit::editingFinished, this, [this]()
            {
                if (m_stackWidget->currentWidget() == m_lineEdit && m_lineEdit->text()!= m_label->text())
                {
                    this->setText(m_lineEdit->text());
                    emit editingFinished();
                }
                m_stackWidget->setCurrentWidget(m_label); });
    m_stackWidget->addWidget(m_label);
    m_stackWidget->addWidget(m_lineEdit);
    m_stackWidget->setCurrentWidget(m_label);
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(m_stackWidget);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    this->setLayout(pLayout);
    setAlignment();
    m_label->installEventFilter(this);
    setCanEdit();
    m_lineEdit->installEventFilter(this);
}

CEditLabel::~CEditLabel()
{
}

void CEditLabel::setText(QString p_text)
{
    m_label->setText(p_text);
    m_label->setStyleSheet((m_text != p_text) ? "QLabel{color:red}" : "");
}

void CEditLabel::resetText()
{
    setText(m_text);
}

void CEditLabel::setDefText(QString p_text)
{
    m_text = p_text;
    resetText();
}

QString CEditLabel::getDefText() const
{
    return m_text;
}

QString CEditLabel::text() const
{
    return m_label->text();
}

void CEditLabel::setCanEdit(bool p_can)
{
    m_canEdit = p_can;
}

void CEditLabel::setAlignment(Qt::Alignment p_aalignment)
{
    m_label->setAlignment(p_aalignment);
    m_lineEdit->setAlignment(p_aalignment);
}

bool CEditLabel::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == m_label)
    {
        if (e->type() == QEvent::MouseButtonDblClick && m_canEdit)
        {
            m_lineEdit->setText(m_label->text());
            m_stackWidget->setCurrentWidget(m_lineEdit);
        }
    }
    else if (m_lineEdit == obj)
    {
        if (e->type() == QEvent::FocusOut)
        {
            emit m_lineEdit->editingFinished();
            m_stackWidget->setCurrentWidget(m_label);
        }
    }

    return QWidget::eventFilter(obj, e);
}