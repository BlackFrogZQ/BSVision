#include "measureImageWidget.h"
#include "Ui/graphics/imageScene.h"
#include "../graphicsView/regionView.h"
#include "Ui/graphics/ruler/rulerWidget.h"
#include "Ui/graphics/ruler/rulerItem.h"
#include "pluginCoreDef.h"
#include "para/paraManager.h"
#include "message/msgManager.h"
#include "fileHelper/jsomHelper.h"

#include <QPen>
#include <QMouseEvent>

using namespace BS_QT_Framework;
using namespace BS_QT_FlieHelper;
CMeasureImageWidget::CMeasureImageWidget(QWidget *p_pParent)
    : CItemImageWidget(p_pParent), m_measure(false)
{
    if (!CParaManager::containsVar(cMeasurePixSize))
    {
        Out_Warning << tr("No scale information available.");
        CParaManager::SetValue(cMeasurePixSize, 1.0);
    }

    m_pMeasureItem = new BS_QT_Ui::CMeasureItem(CParaManager::getVarValueConstPtr(cMeasurePixSize));

    auto measureColor = CParaManager::getVarColor(cMeasureColor);
    connect(this, &CMeasureImageWidget::sigMeasureChanged, this, &CMeasureImageWidget::slotUpdateItemState);
    m_pScene->addItem(m_pMeasureItem);
    m_pMeasureItem->setPos(0, 0);
    m_pMeasureItem->hide();
    m_pMeasureItem->setPen(QPen(measureColor, 0, Qt::DashDotLine));
    m_pMeasureItem->setTextPen(QPen(measureColor, 0));
    m_pMeasureItem->setTextFont(this->font());
}

CMeasureImageWidget::~CMeasureImageWidget()
{
}

void CMeasureImageWidget::setMeasure(bool p_measure)
{
    m_measure = p_measure;
    if (!p_measure && m_pMeasureItem->isVisible())
    {
        m_pMeasureItem->hide();
        Out_Info << tr("Cancel Measurement") + ".";
    }
    p_measure ? setCursor(QCursor(Qt::ArrowCursor)) : unsetCursor();
    emit sigMeasureChanged(m_measure);
}

bool CMeasureImageWidget::isMeasureing() const
{
    return m_measure;
}

void CMeasureImageWidget::mousePressEvent(QMouseEvent *ev)
{
    auto scenePos = [this](QPoint globalPos)
    {
        return m_pView->mapToScene(m_pView->mapFromGlobal(globalPos));
    };

    if (m_measure && (ev->button() & Qt::LeftButton))
    {
        if (m_pMeasureItem->isVisible())
        {
            auto p2 = scenePos(ev->globalPos());
            auto pixSize = m_pMeasureItem->pixSize();
            QLineF measureLine = m_pMeasureItem->line();
            measureLine.setP2(p2);
            auto angle = measureLine.angle();
            angle -= angle > 180 ? 360 : 0;
            disconnect(m_pScene, &BS_QT_Ui::CImageScene::sigMouseMove, this, &CMeasureImageWidget::viewMousemove);
            disconnect(m_pView, &TIGER_Graphics::CRegionView::sigViewChange, this, &CMeasureImageWidget::viewChange);
            Out_Info << tr("End(Pix):(%1,%2).")
                            .arg(p2.x())
                            .arg(p2.y());
            Out_Info << tr("Measurement results(mm) -> Length:%1, Angle:%2°, dx:%3, dy:%4")
                                .arg(dToStr(measureLine.length() * pixSize, 3))
                                .arg(dToStr(angle, 3))
                                .arg(dToStr(measureLine.dx() * pixSize, 3))
                                .arg(dToStr(measureLine.dy() * pixSize, 3)) +
                            ".";
        }
        else
        {
            auto p1 = scenePos(ev->globalPos());
            QLineF measureLine(p1, p1);
            measureLine.setP1(p1);
            measureLine.setP2(p1);
            connect(m_pScene, &BS_QT_Ui::CImageScene::sigMouseMove, this, &CMeasureImageWidget::viewMousemove, Qt::UniqueConnection);
            connect(m_pView, &TIGER_Graphics::CRegionView::sigViewChange, this, &CMeasureImageWidget::viewChange, Qt::UniqueConnection);
            Out_Info << tr("Start(Pix):(%1,%2).")
                            .arg(p1.x())
                            .arg(p1.y());
            m_pMeasureItem->setLine(measureLine);
        }
        m_pMeasureItem->setVisible(!m_pMeasureItem->isVisible());
        update();
        return;
    }

    CItemImageWidget::mousePressEvent(ev);
}

void CMeasureImageWidget::viewMousemove(double x, double y)
{
    if (m_pMeasureItem->isVisible())
    {
        auto pixSize = m_pMeasureItem->pixSize();
        QLineF measureLine = m_pMeasureItem->line();
        measureLine.setP2(QPointF(x, y));
        m_pMeasureItem->setLine(measureLine);
    }
}

void CMeasureImageWidget::viewChange()
{
    if (m_pMeasureItem->isVisible())
    {
        auto pos = m_pView->mapToScene(m_pView->mapFromGlobal(QCursor::pos()));
        viewMousemove(pos.x(), pos.y());
    }
}

void CMeasureImageWidget::slotUpdateItemState()
{
    CItemImageWidget::slotUpdateItemState();
    if (m_measure && m_usedItem)
    {
        if (m_pixelLinkage == false)
        {
            setContextMenuPolicy(Qt::NoContextMenu);
        }

        if (m_itemsEnabled)
        {
            for (auto pItem : m_pScene->myItems())
            {
                pItem->setFlags(pItem->flags() & ~QGraphicsItem::ItemIsSelectable);
            }
        }
    }
}