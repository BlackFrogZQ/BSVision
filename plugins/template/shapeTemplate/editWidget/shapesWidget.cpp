#include "shapesWidget.h"
#include "pluginCoreDef.h"

#include "editWidget.h"
#include "Ui/uiHelper.h"
#include "Ui/widget/toolButton.h"
#include "Ui/widget/messageBox.h"
#include "ui/imageWidget/showImageWidget.h"

#include "../shapeResDef.h"

#include <QButtonGroup>
#include <QHBoxLayout>
namespace BS_2DVision_Template
{
    CShapeWidget::CShapeWidget(QButtonGroup *pGroup, QWidget *p)
        : CIdToolButton(pGroup, p)
    {
        auto newToolBtn = [](QWidget *p, QIcon p_icon = QIcon(), QString p_text = QString())
        {
            auto pToolBtn = new BS_QT_Ui::CCustomToolButton(p);
            pToolBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            pToolBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
            pToolBtn->setText(p_text);
            pToolBtn->setToolTip(p_text);
            pToolBtn->setIcon(p_icon);
            // TODO 移入提示
            pToolBtn->setStyleSheet(
                "QToolButton{padding:0px;background-color:transparent;qproperty-iconSize:23px 23px;};"
                "QToolButton:hover:enabled{padding:2px; }");
            return pToolBtn;
        };

        BS_QT_Ui::CUIHelper::deleteLayout(this->layout());

        pDelBtn = newToolBtn(this, QIcon(cShapeDelIcon), tr("Del"));
        pEditBtn = newToolBtn(this, QIcon(cShapeEditIcon), tr("Edit"));

        QGridLayout *pLayout = new QGridLayout(this);
        pLayout->addWidget(m_pSelectBtn, 0, 0, 2, 2);
        pLayout->addWidget(pDelBtn, 0, 2, 1, 1);
        pLayout->addWidget(pEditBtn, 1, 2, 1, 1);
        pLayout->setAlignment(Qt::AlignCenter);

        pLayout->setSpacing(0);
        pLayout->setMargin(0);

        connect(pDelBtn, &QToolButton::clicked, this, [this]
                {
                    m_pSelectBtn->setChecked(true);
                    emit sigDel(m_id); });
        connect(pEditBtn, &QToolButton::clicked, this, [this]
                {
                    m_pSelectBtn->setChecked(true);
                    emit sigEdit(m_id); });
    }
}

#include "tool/colorTool.h"
#include "wizard/wizard.h"
namespace BS_2DVision_Template
{
    void CShapesWidget::attachShapes(CShapes *p_pShapes)
    {
        m_pShapes = p_pShapes;
        updateShapeBar();
    }

    void CShapesWidget::newShape()
    {
        assert(m_pShapes != nullptr);
        m_pShapes->append(CShape());
        updateShapeBar();
    }

    void CShapesWidget::currIdChanged(int p_curr)
    {
        using namespace TIGER_Graphics;
        if (m_selectId != p_curr)
        {
            m_selectId = p_curr;
        }
        m_showShapeWidget->clearImageRegion();
        if (m_selectId >= 0)
        {
            const auto &shape = (*m_pShapes)[m_selectId];
            m_showShapeWidget->setAutoScaleImage(shape.createImage);
            m_showShapeWidget->showMsg(Qt::AlignCenter, "");
            // TODO 内聚化
            const auto &posPts = shape.posPts();
            const auto &colors = CBSColorToolFun::generateDistinctColors(posPts.size() + 1);
            auto regions = QList<CGraphicsRegion>();
            regions << CGraphicsRegion(tr("Shape"), colors[0], shape.shapePts().toList());
            for (int i = 0; i < posPts.size(); i++)
            {
                regions << CGraphicsRegion(tr("Pos") + "(" + iToStr(i + 1) + ")", colors[i + 1], posPts[i].toList());
            }
            m_showShapeWidget->showRegions(regions);
        }
        else
        {
            m_showShapeWidget->showMsg(Qt::AlignCenter, tr("Invalid Shape"));
        }

        m_pToolBar->setVisible(m_pShapes != nullptr);
    }

    void CShapesWidget::editShape(int p_id)
    {
        auto pWizard = new CWizard(this);
        auto pCopyShape = new CShape;
        *pCopyShape = m_pShapes->at(p_id).clone();
        connect(pWizard, &CWizard::accepted, this, [this, pCopyShape, p_id]()
                {
                    (*m_pShapes)[p_id] = *pCopyShape;
                    delete pCopyShape;
                    currIdChanged(p_id);
                    emit sigEditFinished(); });
        connect(pWizard, &CWizard::rejected, this, [this, pCopyShape]()
                {
                    delete pCopyShape;
                    emit sigEditFinished(); });
        pWizard->setShape(pCopyShape);
        emit sigEditWidget(pWizard);
    }

    void CShapesWidget::init()
    {
        m_pButtonGroup = new QButtonGroup(this);
        m_pButtonGroup->setExclusive(true);

        m_pToolBar = new QWidget(this);
        m_pToolBar->setObjectName(cStdWorkWidgetToolBar);

        m_showShapeWidget = new CShowAutoImageWidget(this);
        m_showShapeWidget->showMsg(Qt::AlignCenter, "", QColor(134, 134, 134, 200));

        m_pNewShapeBtn = new BS_QT_Ui::CCustomToolButton(this);
        m_pNewShapeBtn->setObjectName(cStdMainWidgetToolBtn);
        m_pNewShapeBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_pNewShapeBtn->setIcon(QIcon(cNewFileIcon));
        m_pNewShapeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_pNewShapeBtn->setText(tr("New"));

        connect(m_pNewShapeBtn, &QToolButton::clicked, this, &CShapesWidget::newShape);

        updateShapeBar();
        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addWidget(m_showShapeWidget, 1);
        pLayout->addWidget(m_pToolBar);
        pLayout->setSpacing(0);
        pLayout->setMargin(0);
    }

    void CShapesWidget::updateShapeBar()
    {
        BS_QT_Ui::CUIHelper::deleteLayout(m_pToolBar->layout());
        QHBoxLayout *pToolBarLayout = new QHBoxLayout(m_pToolBar);
        pToolBarLayout->setMargin(3);
        pToolBarLayout->setSpacing(5);
        pToolBarLayout->setAlignment(Qt::AlignCenter);
        pToolBarLayout->addStretch();
        pToolBarLayout->addWidget(m_pNewShapeBtn);

        int selectId = -1;
        if (m_pShapes != nullptr)
        {
            while (m_pShapeWidgets.size() > m_pShapes->size())
            {
                auto pToolBtn = m_pShapeWidgets.takeLast();
                pToolBtn->deleteLater();
            }
            selectId = m_selectId >= m_pShapeWidgets.size() ? (m_pShapeWidgets.size() - 1) : m_selectId;
            while (m_pShapeWidgets.size() < m_pShapes->size())
            {
                auto pToolBtn = new CShapeWidget(m_pButtonGroup, this);
                pToolBtn->setSrc(QImage(cShapeIcon), tr("Shape"));
                connect(pToolBtn, &CShapeWidget::sigSelected, this, &CShapesWidget::currIdChanged);
                connect(pToolBtn, &CShapeWidget::sigDel, this, [this](int id)
                        {
                            m_pShapes->removeAt(id);
                            updateShapeBar(); });

                connect(pToolBtn, &CShapeWidget::sigEdit, this, &CShapesWidget::editShape);

                m_pShapeWidgets << pToolBtn;
                pToolBtn->installEventFilter(this);
            }
        }
        else
        {
            qDeleteAll(m_pShapeWidgets);
            m_pShapeWidgets.clear();
        }

        auto font = this->font();
        font.setBold(true);
        font.setPointSize(12);
        for (int i = 0; i < m_pShapeWidgets.size(); i++)
        {
            m_pShapeWidgets[i]->setId(i);
            pToolBarLayout->addWidget(m_pShapeWidgets[i]);
            if (m_pShapeWidgets[i]->invokeSelectBtn(&QToolButton::isChecked))
            {
                selectId = i;
            }
        }
        pToolBarLayout->addStretch();

        if (selectId < 0 && m_pShapeWidgets.size() > 0)
        {
            selectId = 0;
        }

        if (selectId >= 0)
        {
            m_pShapeWidgets[selectId]->invokeSelectBtn(&QToolButton::setChecked, true);
        }
        currIdChanged(selectId);
    }
}