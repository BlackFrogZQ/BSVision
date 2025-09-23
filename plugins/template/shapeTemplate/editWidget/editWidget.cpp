#include "editWidget.h"
#include "shapesWidget.h"
#include "idToolButton.h"

#include "pluginCoreDef.h"

#include "../shapeTemplate.h"
#include "bsBasic.h"
#include "Ui/uiHelper.h"
#include "Ui/widget/toolButton.h"
#include "Ui/widget/messageBox.h"

#include "../shapeResDef.h"
#include <QLabel>
#include <QEvent>
#include <QMenu>
#include <QButtonGroup>
#include <QVBoxLayout>
namespace BS_2DVision_Template
{
    CShapeTemplateEditWidget::CShapeTemplateEditWidget(QVector<CShapes> &p_shapes, QWidget *p_pParent)
        : m_shapesList(p_shapes),
          ITemplateEditWidget(p_pParent)
    {
        init();
        updateShapes();
        normalLayout();
    }

    CShapeTemplateEditWidget::~CShapeTemplateEditWidget()
    {
    }

    void CShapeTemplateEditWidget::init()
    {
        m_pButtonGroup = new QButtonGroup(this);
        m_pButtonGroup->setExclusive(true);

        m_pToolBar = new QWidget(this);
        m_pToolBar->setObjectName(cStdWorkWidgetToolBar);

        m_pNewShapesBtn = new BS_QT_Ui::CCustomToolButton(this);
        m_pNewShapesBtn->setObjectName(cStdMainWidgetToolBtn);
        m_pNewShapesBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pNewShapesBtn->setIcon(QIcon(cNewFileIcon));
        m_pNewShapesBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_pNewShapesBtn->setText(tr("New"));

        m_pShapeWidget = new CShapesWidget(this);
        m_pShapeWidget->init();

        connect(m_pShapeWidget, &CShapesWidget::sigEditWidget, this, &CShapeTemplateEditWidget::onlyEditWidgetLayout);
        connect(m_pShapeWidget, &CShapesWidget::sigEditFinished, this, &CShapeTemplateEditWidget::normalLayout);
        connect(m_pNewShapesBtn, &QToolButton::clicked, this, &CShapeTemplateEditWidget::newShapes);
    }

    void CShapeTemplateEditWidget::normalLayout()
    {
        BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addWidget(m_pToolBar);
        pLayout->addWidget(m_pShapeWidget, 1);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);
        m_pToolBar->show();
        m_pShapeWidget->show();
        emit sigEditChanged(false,this);
    }

    void CShapeTemplateEditWidget::onlyEditWidgetLayout(QWidget *p_widget)
    {
        assert(p_widget != nullptr);
        BS_QT_Ui::CUIHelper::deleteLayout(this->layout());
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addWidget(p_widget);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);
        m_pToolBar->hide();
        m_pShapeWidget->hide();
        emit sigEditChanged(true,this);
    }

    void CShapeTemplateEditWidget::newShapes()
    {
        m_shapesList.append(CShapes());
        updateShapes();
    }

    void CShapeTemplateEditWidget::updateShapes()
    {
        BS_QT_Ui::CUIHelper::deleteLayout(m_pToolBar->layout());
        while (m_pShapesBtns.size() > m_shapesList.size())
        {
            auto pToolBtn = m_pShapesBtns.takeLast();
            pToolBtn->deleteLater();
        }

        auto selectId = m_selectId >= m_pShapesBtns.size() ? (m_pShapesBtns.size() - 1) : m_selectId;

        while (m_pShapesBtns.size() < m_shapesList.size())
        {
            auto pToolBtn = new CIdToolButton(m_pButtonGroup,this);
            pToolBtn->setSrc(QImage(cShapesIcon),"Mark");
            connect(pToolBtn, &CIdToolButton::sigSelected, this, &CShapeTemplateEditWidget::currIdChanged);
            m_pShapesBtns << pToolBtn;
            pToolBtn->installEventFilter(this);
        }

        auto font = this->font();
        font.setBold(true);
        font.setPointSize(12);
        QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
        pToolBarLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        pToolBarLayout->addWidget(m_pNewShapesBtn);
        for (int i = 0; i < m_pShapesBtns.size(); i++)
        {
            m_pShapesBtns[i]->setId(i);
            pToolBarLayout->addWidget(m_pShapesBtns[i]);
            if (m_pShapesBtns[i]->invokeSelectBtn(&QToolButton::isChecked))
            {
                selectId = i;
            }
        }

        if (selectId < 0 && m_pShapesBtns.size() > 0)
        {
            selectId = 0;
        }

        if (selectId >= 0)
        {
            m_pShapesBtns[selectId]->invokeSelectBtn(&QToolButton::setChecked,true);
        }
        currIdChanged(selectId);
        pToolBarLayout->addStretch();
        pToolBarLayout->setMargin(3);
        pToolBarLayout->setSpacing(5);
    }

    void CShapeTemplateEditWidget::currIdChanged(int p_curr)
    {
        if (m_selectId != p_curr)
        {
            m_selectId = p_curr;
            if (m_selectId >= 0)
            {
                m_pShapeWidget->attachShapes(&m_shapesList[m_selectId]);
            }
            else
            {
                m_pShapeWidget->attachShapes(nullptr);
            }
        }
    }

    void CShapeTemplateEditWidget::shapesBtnContextMenu(CIdToolButton *pIconTextButton)
    {
        assert(pIconTextButton != nullptr);
        pIconTextButton->invokeSelectBtn(&QToolButton::setChecked,true);
        QMenu *pMenu = new QMenu(this);
        auto getAction = [](QObject *p, QString text, QIcon icon)
        {
            QAction *pAction = new QAction(p);
            pAction->setText(text);
            pAction->setIcon(icon);
            return pAction;
        };
        int id = m_pShapesBtns.indexOf(pIconTextButton);
        assert(id >= 0);
        {
            QAction *pAction = getAction(pMenu, tr("Del"), QIcon());
            connect(pAction, &QAction::triggered, this, [this, p_id = id]()
                    {
                        if (BS_QT_Ui::CCustomMessageBox::question(this,
                                tr("Del"),tr("Whether to delete %1?").arg(m_pShapesBtns[p_id]->invokeSelectBtn(&QToolButton::text)))
                            != QMessageBox::Yes)
                        {
                            return;
                        }
                        m_shapesList.removeAt(p_id);
                        updateShapes(); });
            pMenu->addAction(pAction);
        }
        for (int i = 0; i < m_pShapesBtns.size(); i++)
        {
            if (i != id)
            {
                auto pW = m_pShapesBtns[i];
                QAction *pAction = getAction(pMenu, tr("Copy from %1").arg(pW->invokeSelectBtn(&QToolButton::text)), pW->invokeSelectBtn(&QToolButton::icon));
                connect(pAction, &QAction::triggered, this, [this, srcId = i, id]()
                        {
                            if (BS_QT_Ui::CCustomMessageBox::question(this,tr("Copy"),
                                    tr("Copy from %1 to %2?")
                                        .arg(m_pShapesBtns[srcId]->invokeSelectBtn(&QToolButton::text))
                                        .arg(m_pShapesBtns[id]->invokeSelectBtn(&QToolButton::text)))
                                != QMessageBox::Yes)
                            {
                                return;
                            }
                            m_shapesList[id] = m_shapesList[srcId].clone();
                            updateShapes(); });
                pMenu->addAction(pAction);
            }
        }
        pMenu->exec(QCursor::pos());
        delete pMenu;
    }

    bool CShapeTemplateEditWidget::eventFilter(QObject *obj, QEvent *e)
    {
        if (obj != this)
        {
            if (e->type() == QEvent::ContextMenu)
            {
                shapesBtnContextMenu(qobject_cast<CIdToolButton *>(obj));
                return true;
            }
            if (e->type() == QEvent::ContextMenu)
            {
                shapesBtnContextMenu(qobject_cast<CIdToolButton *>(obj));
                return true;
            }
        }
        return QWidget::eventFilter(obj, e);
    }
}