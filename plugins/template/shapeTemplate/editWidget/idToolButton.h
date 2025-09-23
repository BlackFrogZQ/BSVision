
#pragma once
#include "bsBasic.h"
#include <QFrame>
class QToolButton;
class QButtonGroup;
namespace BS_2DVision_Template
{
    class CIdToolButton : public QFrame
    {
        Q_OBJECT
    public:
        CIdToolButton(QButtonGroup *pGroup, QWidget *p = nullptr);
        ~CIdToolButton() = default;

        void setSrc(QImage p_icon, QString p_pre = QString(), QString p_suff = QString());
        void setId(int p_id);

        Macro_InvokeMemberFun(SelectBtn, m_pSelectBtn);
        static QImage buildIdIcon(QImage p_icon, QSize p_iconSize, QFont p_font, int p_id);

    signals:
        void sigSelected(int p_id);

    protected:
        QToolButton *m_pSelectBtn = nullptr;
        QImage m_icon;
        QString m_pre;
        QString m_suff;
        int m_id = -1;
    };
}