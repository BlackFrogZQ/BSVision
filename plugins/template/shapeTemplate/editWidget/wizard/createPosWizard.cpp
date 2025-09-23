#include "createPosWizard.h"
#include "template/shapeTemplate/shapeDef.h"
#include "ui/graphicsView/regionView.h"
#include "Ui/graphics/imageScene.h"
#include "Ui/widget/toolButton.h"
#include "Ui/widget/comboBox.h"
#include "Ui/itemDelegate/validatorDelegate.h"

#include "../graphics/fitItem/fitToolItem.h"
#include "pluginCoreDef.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QDoubleValidator>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QLabel>
namespace BS_2DVision_Template
{
    using namespace TIGER_Graphics;
    using namespace BS_QT_Ui;
    CCreatePosWizard::CCreatePosWizard(QWidget *parent) : QWizardPage(parent)
    {
        setWindowTitle(tr("Create Pos"));

        {
            m_pTitleLabel = new QLabel(tr("Create Pos"), this);
            m_pTitleLabel->setWordWrap(true);
            auto newToolBtn = [](QWidget *p, QIcon p_icon = QIcon(), QString p_text = QString())
            {
                auto pToolBtn = new BS_QT_Ui::CCustomToolButton(p);
                pToolBtn->setObjectName(cStdMainWidgetToolBtn2);
                pToolBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                pToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                pToolBtn->setText(p_text);
                pToolBtn->setToolTip(p_text);
                pToolBtn->setIcon(p_icon);
                pToolBtn->setIconSize(QSize(30, 30));
                return pToolBtn;
            };
            m_pClearBtn = newToolBtn(this, QIcon(cClearIcon), tr("Clear"));
            m_pAddtypeTypeCB = new BS_QT_Ui::CCustomComboBox(this);
            m_pAddtypeTypeCB->setIconSize(m_pClearBtn->iconSize());
            for (auto key : cFitToolStrMap.keys())
            {
                m_pAddtypeTypeCB->addItem(QIcon(cFitToolIconMap[key]), cFitToolStrMap[key], (int)key);
            }
            m_pAddBtn = newToolBtn(this, QIcon(), tr("Add"));
            m_pAddBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);

            m_pPosPtTableWidget = new QTableWidget(this);
            m_pPosPtTableWidget->setItemDelegate(
                new CValidatorDelegate(new QDoubleValidator(-99999.0, 99999.0, 3), m_pPosPtTableWidget));
            m_pPosPtTableWidget->setColumnCount(2);
            m_pPosPtTableWidget->setHorizontalHeaderLabels(QStringList() << "X" << "Y");
            m_pPosPtTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
            m_pPosPtTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
            m_pPosPtTableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
            m_pPosPtTableWidget->verticalHeader()->setVisible(true);
            m_pPosPtTableWidget->setSortingEnabled(false);
            m_pPosPtTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            m_pPosPtTableWidget->setFixedWidth(200);
            connect(m_pPosPtTableWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
                    this, &CCreatePosWizard::idChanged);
        }

        QHBoxLayout *pTitleLayout = new QHBoxLayout;
        pTitleLayout->setSpacing(3);
        pTitleLayout->setMargin(0);
        pTitleLayout->addWidget(m_pTitleLabel);
        pTitleLayout->addSpacing(20);
        pTitleLayout->addWidget(m_pClearBtn);
        pTitleLayout->addWidget(new QLabel(tr("Pt Type") + ":", this));
        pTitleLayout->addWidget(m_pAddtypeTypeCB);
        pTitleLayout->addWidget(m_pAddBtn);
        pTitleLayout->addStretch();

        m_pView = new CRegionView(this);
        m_pImageScene = new CImageScene(this);
        m_pView->setScene(m_pImageScene);

        QGridLayout *pGrid = new QGridLayout;
        pGrid->setSpacing(3);
        pGrid->setMargin(0);
        pGrid->addWidget(m_pView, 0, 0, 3, 3);
        pGrid->addWidget(m_pPosPtTableWidget, 0, 3, 2, 1);
        pGrid->addWidget(new QLabel, 2, 3, 1, 1);

        QVBoxLayout *pLayout = new QVBoxLayout(this);
        pLayout->addLayout(pTitleLayout);
        pLayout->addLayout(pGrid, 1);
        pLayout->setSpacing(3);
        pLayout->setMargin(0);

        connect(m_pAddBtn, &QToolButton::clicked, this, &CCreatePosWizard::addPt);
    }

    bool CCreatePosWizard::isComplete() const
    {
        return (m_pShape != nullptr) && (!m_pShape->isEmpty()) && (!m_pShape->anchorPt.isEmpty());
    }

    void CCreatePosWizard::setUpdatePtTable(int p_id, double px, double py)
    {
        if (p_id >= 0 && p_id < m_toolItems.size())
        {
            m_pPosPtTableWidget->item(p_id, 0)->setText(dToStr(px, 3));
            m_pPosPtTableWidget->item(p_id, 1)->setText(dToStr(py, 3));
            m_toolItems[p_id]->setFitPos(px, py);
        }
    }

    void CCreatePosWizard::slotFitTool(int p_id)
    {
        if (p_id >= 0 && p_id < m_toolItems.size())
        {
        }
    }

    void CCreatePosWizard::initializePage()
    {
        if (m_pShape != nullptr)
        {
            m_pView->clearRegion();
            m_pImageScene->showImage(QPixmap::fromImage(m_pShape->createImage));
            QRectF rect = m_pShape->createImage.rect();
            rect.moveCenter(QPointF(0, 0));
            m_pView->setDefineSceneRect(rect, true);
        }
    }

    void CCreatePosWizard::setShape(CShape *pShape)
    {
        m_pShape = pShape;
        initializePage();
    }
}

namespace BS_2DVision_Template
{
    void CCreatePosWizard::addPt()
    {
        auto pItem = CGraphicsFitToolItemCreater::createToolItem(IFitToolItem::CFitToolType(m_pAddtypeTypeCB->itemData(m_pAddtypeTypeCB->currentIndex()).toInt()));
        // 计算初始大小
        {
            auto rect = m_pView->mapToScene(m_pView->rect()).boundingRect();
            auto center = rect.center();
            auto viewSize = rect.size() * 0.5;
            rect.setSize(QSizeF(std::min(m_pImageScene->sceneRect().width() * 0.5, std::max(20.0, viewSize.width())), std::min(m_pImageScene->sceneRect().height() * 0.5, std::max(20.0, viewSize.height()))));
            rect.moveCenter(center);
            pItem->setToolRect(rect);
        }

        m_pImageScene->clearSelection();
        m_pImageScene->addItem(pItem);
        m_toolItems << pItem;

        auto newItem = [this](double p_value)
        {
            QTableWidgetItem *pItem = new QTableWidgetItem(dToStr(p_value, 3));
            pItem->setTextAlignment(Qt::AlignCenter);
            return pItem;
        };
        int row = m_pPosPtTableWidget->rowCount();
        auto pos = pItem->fitPos();
        m_pPosPtTableWidget->insertRow(row);
        m_pPosPtTableWidget->setItem(row, 0, newItem(pos.x()));
        m_pPosPtTableWidget->setItem(row, 1, newItem(pos.y()));
        m_pPosPtTableWidget->selectRow(row);
        connect(pItem, &IFitToolItem::sigFitRectChanged, this, [this, pItem]
                { slotFitTool(m_toolItems.indexOf(pItem)); });
        connect(pItem, &IFitToolItem::sigDel, this, [this, row]
                {
                    auto pItem = m_toolItems.takeAt(row);
                    m_pImageScene->removeItem(pItem);
                    pItem->deleteLater();
                    m_pPosPtTableWidget->removeRow(row);
                    if (m_pPosPtTableWidget->rowCount() > 0)
                    {
                        m_pPosPtTableWidget->selectRow(row - 1);
                    } });
    }

    void CCreatePosWizard::idChanged(const QModelIndex &current, const QModelIndex &previous)
    {
        int currentRow = current.row();
        if (currentRow >= 0 && currentRow < m_toolItems.size())
        {
            m_toolItems[currentRow]->setVisible(true);
            m_toolItems[currentRow]->setSelected(true);
        }
        int previousRow = previous.row();
        if (previousRow >= 0 && previousRow < m_toolItems.size())
        {
            m_toolItems[previousRow]->setVisible(false);
        }
    }
}