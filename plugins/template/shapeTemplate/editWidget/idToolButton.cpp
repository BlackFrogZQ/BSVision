#include "idToolButton.h"

#include "pluginCoreDef.h"

#include "Ui/widget/toolButton.h"

#include <QPainter>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QToolButton>
#include <QGridLayout>
namespace BS_2DVision_Template
{
    CIdToolButton::CIdToolButton(QButtonGroup *pGroup, QWidget *p)
        : QFrame(p)
    {
        auto newToolBtn = [](QWidget *p, QString p_text = QString())
        {
            auto pToolBtn = new BS_QT_Ui::CCustomToolButton(p);
            pToolBtn->setObjectName(cStdMainWidgetToolBtn);
            pToolBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            pToolBtn->setText(p_text);
            pToolBtn->setToolTip(p_text);
            pToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            pToolBtn->setCheckable(true);
            return pToolBtn;
        };

        m_pSelectBtn = newToolBtn(this);

        if (pGroup)
        {
            pGroup->addButton(m_pSelectBtn);
        }

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pSelectBtn);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);

        connect(m_pSelectBtn, &QToolButton::toggled, this, [this](bool p)
                {
                    if (p)
                    {
                        emit sigSelected(m_id);
                    } });
    }

    void CIdToolButton::setSrc(QImage p_icon, QString p_pre, QString p_suff)
    {
        if (p_icon != m_icon ||
            p_pre != m_pre ||
            p_suff != m_suff)
        {
            m_icon = p_icon;
            if (m_icon.format() != QImage::Format_RGBA8888)
            {
                m_icon.convertTo(QImage::Format_RGBA8888);
            }
            m_pre = p_pre;
            m_suff = p_suff;
            m_pSelectBtn->setText(m_pre + iToStr(m_id + 1) + m_suff);
            auto font = this->font();
            font.setBold(true);
            font.setPointSize(12);
            m_pSelectBtn->setIcon(QIcon(QPixmap::fromImage(buildIdIcon(m_icon, QSize(30, 30), font, m_id + 1))));
        }
    }

    void CIdToolButton::setId(int p_id)
    {
        if (m_id != p_id)
        {
            m_id = p_id;
            m_pSelectBtn->setText(m_pre + iToStr(m_id + 1) + m_suff);
            auto font = this->font();
            font.setBold(true);
            font.setPointSize(12);
            m_pSelectBtn->setIcon(QIcon(QPixmap::fromImage(buildIdIcon(m_icon, QSize(30, 30), font, m_id + 1))));
        }
    }

    QImage CIdToolButton::buildIdIcon(QImage p_icon, QSize p_iconSize, QFont p_font, int p_id)
    {
        if (p_icon.isNull() || p_iconSize.isEmpty())
        {
            return QImage();
        }
        auto icon = p_icon.scaledToHeight(p_iconSize.height());
        if (p_icon.format() != QImage::Format_RGBA8888)
        {
            p_icon.convertTo(QImage::Format_RGBA8888);
        }
        QPainter painter(&icon);
        painter.setFont(p_font);
        painter.setPen(Qt::black);
        painter.drawText(icon.rect(), iToStr(p_id), QTextOption(Qt::AlignCenter));
        painter.end();
        return icon;
    }
}