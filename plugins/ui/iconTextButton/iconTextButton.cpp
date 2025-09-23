#include "iconTextButton.h"
#include "ui/editLabel/editLabel.h"

#include "bsBasic.h"

#include <QLabel>
#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QTimer>

CIconTextButton::CIconTextButton(int *p_id, QWidget *parent)
    : QWidget(parent), m_pId(p_id), m_iconSize(25, 25), m_select(false), m_color(229, 243, 255, 0)
{
    assert(m_pId != nullptr);
    this->setFocusPolicy(Qt::ClickFocus);
    m_pIconLabel = new QLabel(this);
    m_pIconLabel->setAlignment(Qt::AlignCenter);
    m_pTitleLabel = new CEditLabel;
    m_pTitleLabel->setFont(this->font());
    connect(m_pTitleLabel, &CEditLabel::editingFinished, this, [this]()
            {
                updateWidth();
                emit sigTextChanged(*m_pId, m_pTitleLabel->text()); });

    QVBoxLayout *pLayout = new QVBoxLayout;
    {
        QHBoxLayout *pLableLayout = new QHBoxLayout;
        pLableLayout->addStretch();
        pLableLayout->addWidget(m_pIconLabel);
        pLableLayout->addStretch();
        pLableLayout->setSpacing(0);
        pLableLayout->setMargin(0);
        pLayout->addLayout(pLableLayout);
    }
    pLayout->addWidget(m_pTitleLabel);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    this->setLayout(pLayout);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setCanEdit();
}

CIconTextButton::~CIconTextButton()
{
}
void CIconTextButton::setCanEdit(bool p_canEdit)
{
    m_pTitleLabel->setCanEdit(p_canEdit);
}

void CIconTextButton::setIconSize(QSize p_size)
{
    m_iconSize = p_size;
    updateIcon();
}

void CIconTextButton::setIcon(QPixmap p_icon)
{
    m_icon = p_icon;
    updateIcon();
}

void CIconTextButton::setText(QString p_text)
{
    m_pTitleLabel->setText(p_text);
    updateWidth();
}

QString CIconTextButton::text() const
{
    return m_pTitleLabel->text();
}

int CIconTextButton::id() const
{
    return *m_pId;
}

QPixmap CIconTextButton::icon() const
{
    return *(m_pIconLabel->pixmap());
}

void CIconTextButton::setDefText(QString p_text)
{
    m_pTitleLabel->setDefText(p_text);
    updateWidth();
}

void CIconTextButton::resetText()
{
    m_pTitleLabel->resetText();
}

void CIconTextButton::updateIcon()
{
    if (m_icon.isNull() || m_iconSize.isEmpty())
    {
        m_pIconLabel->setPixmap(QPixmap());
    }
    QPixmap icon = getIcon(m_icon, m_iconSize, *m_pId + 1);
    m_pIconLabel->setFixedSize(icon.size());
    m_pIconLabel->setPixmap(icon);
    m_pTitleLabel->setFixedHeight(m_pTitleLabel->fontMetrics().height());
    updateWidth();
}

QPixmap CIconTextButton::getIcon(QPixmap p_icon, QSize p_iconSize, int id)
{
    assert(!(p_icon.isNull() || p_iconSize.isEmpty()));
    QPixmap icon = p_icon.scaledToHeight(p_iconSize.height());
    QPainter painter(&icon);
    painter.setPen(Qt::black);
    painter.drawText(icon.rect(), iToStr(id), QTextOption(Qt::AlignCenter));
    painter.end();
    return icon;
}

void CIconTextButton::updateWidth()
{
    this->setFixedWidth(qMax(m_pIconLabel->width(), m_pTitleLabel->fontMetrics().width(m_pTitleLabel->text())) + 10);
}

void CIconTextButton::setSelect(bool p_select)
{
    m_select = p_select;
    if (p_select)
    {
        m_color = QColor(210, 220, 230, 255);
    }
    else
    {
        m_color = QColor(255, 255, 255, 0);
    }
    update();
}

void CIconTextButton::mousePressEvent(QMouseEvent *p_event)
{
    if ((p_event->button() & Qt::LeftButton || p_event->button() & Qt::RightButton))
    {
        m_isDoubleClickd = false;
        QTimer::singleShot(200, this, [this]()
                           {
                if ((!m_select) && (!m_isDoubleClickd))
                {
                    setSelect(true);
                    emit sigClicked(*m_pId);
                } });
    }
    QWidget::mousePressEvent(p_event);
}
void CIconTextButton::mouseDoubleClickEvent(QMouseEvent *p_event)
{
    if (p_event->button() & Qt::LeftButton || p_event->button() & Qt::RightButton)
    {
        m_isDoubleClickd = true;
        emit sigMouseDoubleClick(*m_pId);
        return;
    }
}
void CIconTextButton::enterEvent(QEvent *event)
{
    m_color = QColor(210, 220, 230, 255);
    QWidget::enterEvent(event);
    update();
}

void CIconTextButton::leaveEvent(QEvent *event)
{
    setSelect(m_select);
    QWidget::leaveEvent(event);
    update();
}

void CIconTextButton::paintEvent(QPaintEvent *event)
{
    QPainter paint;
    paint.begin(this);
    paint.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(this->rect(), 5, 5);
    paint.fillPath(path, m_color);
    QWidget::paintEvent(event);
}

CMenuIconStyle::CMenuIconStyle()
    : QProxyStyle(), m_nSize(35)
{
}

CMenuIconStyle::~CMenuIconStyle()
{
}

void CMenuIconStyle::setCustomSize(int size)
{
    m_nSize = size;
}

int CMenuIconStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int s = QCommonStyle::pixelMetric(metric, option, widget);
    if (metric == QStyle::PM_SmallIconSize)
    {
        s = m_nSize;
    }
    return s;
}