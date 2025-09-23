#include "itemImageWidget.h"
#include "Ui/graphics/imageScene.h"
#include "../graphicsView/regionView.h"

#include "Ui/graphics/ruler/rulerWidget.h"
#include "Ui/graphics/ruler/rulerItem.h"

#include "message/msgManager.h"

#include "pluginCoreDef.h"
#include "para/paraManager.h"
#include "fileHelper/appHelper.h"
#include <QSettings>
#include <QMouseEvent>
#include <QMenu>
#include <QPen>

using namespace BS_QT_Ui;
using namespace BS_QT_Framework;
CItemImageWidget::CItemImageWidget(QWidget *p_pParent)
    : CShowAutoImageWidget(p_pParent),
      m_pixelLinkage(false), m_itemsEnabled(false), m_usedItem(false)
{
    setContextMenuPolicy(Qt::NoContextMenu);
    initItemsContextMenu();
    connect(this, &CItemImageWidget::sigItemsEnableChanged, this, &CItemImageWidget::slotUpdateItemState);
    connect(this, &CItemImageWidget::sigUsedItemChanged, this, &CItemImageWidget::slotUpdateItemState);
    connect(this, &CItemImageWidget::sigPixelLinkageChanged, this, &CItemImageWidget::slotUpdateItemState);
    loadItemParas();
}

CItemImageWidget::~CItemImageWidget()
{
    saveItemParas();
    qDeleteAll(m_manuItemActions);
    delPtr(m_pItemsEnabledAction);
}

void CItemImageWidget::loadItemParas()
{
    QSettings settings(BS_QT_FlieHelper::CAppFileHelper::configDirPath() + "/markItemConfig.ini", QSettings::IniFormat);
    int itemSize = settings.beginReadArray("Items");
    for (int i = 0; i < itemSize; i++)
    {
        settings.setArrayIndex(i);
        int type = settings.value("type", -1).toInt();
        QList<QPointF> points;
        int pointSize = settings.beginReadArray("points");
        for (int j = 0; j < pointSize; j++)
        {
            settings.setArrayIndex(j);
            points.append(settings.value("point", QPointF(0, 0)).toPointF());
        }
        settings.endArray();
        BS_QT_Ui::IGraphicsItem *pItems = nullptr;
        switch (type)
        {
        case BS_QT_Ui::HorLineRuler:
            pItems = new BS_QT_Ui::CRulerLineItem(m_pView, Qt::Horizontal);
            break;
        case BS_QT_Ui::verHLineRuler:
            pItems = new BS_QT_Ui::CRulerLineItem(m_pView, Qt::Vertical);
            break;
        case BS_QT_Ui::RectRuler:
            pItems = new BS_QT_Ui::CRulerRectItem;
            break;
        case BS_QT_Ui::CircularRuler:
            pItems = new BS_QT_Ui::CRulerCircularItem;
            break;
        default:
            continue;
            break;
        }
        assert(pItems != nullptr && pItems->getType() == type);
        pItems->addKeyPoints(points);
        pItems->setZValue(settings.value("z", i).toDouble());
        pItems->setViewAngle(settings.value("rot", 0.0).toDouble());
        pItems->setTransform(settings.value("transform").value<QTransform>(), false);
        if (!pItems->isDrawFinished())
        {
            delPtr(pItems);
            continue;
        }
        addItem_pri(pItems);
        if (type == BS_QT_Ui::HorLineRuler || type == BS_QT_Ui::verHLineRuler)
        {
            QMetaObject::invokeMethod(pItems, "updateCenterOffset");
        }
    }
    settings.endArray();
    setItemsEnable(settings.value("itemsEnabled", true).toBool());
}

void CItemImageWidget::saveItemParas() const
{
    QSettings settings(BS_QT_FlieHelper::CAppFileHelper::configDirPath() + "/markItemConfig.ini", QSettings::IniFormat);
    settings.clear();
    auto pItems = m_pScene->myItems();
    settings.setValue("itemsEnabled", m_itemsEnabled);
    settings.beginWriteArray("Items");
    for (int i = 0; i < pItems.size(); ++i)
    {
        auto pItem = static_cast<BS_QT_Ui::IGraphicsItem *>(pItems[i]);
        settings.setArrayIndex(i);
        settings.setValue("type", pItem->getType());
        settings.setValue("z", pItem->zValue());
        settings.setValue("rot", pItem->getViewAngle());
        settings.setValue("transform", pItem->transform());
        settings.beginWriteArray("points");
        auto points = pItem->getKeyPoints();
        for (int j = 0; j < points.size(); j++)
        {
            settings.setArrayIndex(j);
            settings.setValue("point", points[j]);
        }
        settings.endArray();
    }
    settings.endArray();
}

void CItemImageWidget::addItem_pri(BS_QT_Ui::IGraphicsBasicItem *pItem)
{
    auto lineColor = CParaManager::getVarColor(cMarkItemLineColor);
    auto keyPointColor = CParaManager::getVarColor(cMarkItemPointColor);
    pItem->setAcceptedHoverMode(IGraphicsBasicItem::SelectedAcceptedHover);
    pItem->setLineColor(lineColor);
    pItem->setCtrlPointColor(keyPointColor);
    m_pScene->addMarkItem(pItem);
    m_pView->viewport()->update();
}

void CItemImageWidget::setPixelLinkage(bool p_pixelLinkage)
{
    m_pixelLinkage = p_pixelLinkage;
    emit sigPixelLinkageChanged(m_pixelLinkage);
}

void CItemImageWidget::setUsedItem(bool p_used)
{
    m_usedItem = p_used;
    emit sigUsedItemChanged(m_usedItem);
}

void CItemImageWidget::setItemsEnable(bool p_enable)
{
    m_itemsEnabled = p_enable;
    m_pItemsEnabledAction->setText(p_enable ? tr("Freeze Items") : tr("Unfreeze Items"));
    emit sigItemsEnableChanged(m_itemsEnabled);
}

bool CItemImageWidget::isPixelLink() const
{
    return m_pixelLinkage;
}

bool CItemImageWidget::usedItem() const
{
    return m_usedItem;
}

bool CItemImageWidget::itemsEnabled() const
{
    return m_itemsEnabled;
}

void CItemImageWidget::addLineItem(double p_offset, Qt::Orientation p_orientation)
{
    if (!m_usedItem)
    {
        return;
    }
    auto pLine = new BS_QT_Ui::CRulerLineItem(m_pView, p_orientation);
    pLine->setOffset(p_offset);
    addItem(pLine);
    pLine->updateLine();
}

void CItemImageWidget::addItem(BS_QT_Ui::IGraphicsBasicItem *pItem)
{
    if (!m_usedItem)
    {
        delete pItem;
        return;
    }
    addItem_pri(pItem);
}

void CItemImageWidget::clearItem()
{
    m_pScene->clearMyItem();
}

bool CItemImageWidget::pixelLinkPos(QPoint globalPos, double &x, double &y) const
{
    QPointF scenePos = m_pView->mapToScene(m_pView->mapFromGlobal(globalPos));
    scenePos.setY(-scenePos.y());
    const auto cImage = m_pScene->getImage();
    if (cImage.isNull() || abs(scenePos.x()) > cImage.width() * 0.5 || abs(scenePos.y()) > cImage.height() * 0.5)
    {
        Out_Info << tr("No click in the image area, invalid click.")
                 << tr("image area(-%1,-%2,%1,%2)").arg(cImage.width() * 0.5).arg(cImage.height() * 0.5)
                 << tr("cliked(%1,%2)").arg(scenePos.x()).arg(scenePos.y());
        return false;
    }
    x = scenePos.x();
    y = scenePos.y();
    return true;
}

void CItemImageWidget::mousePressEvent(QMouseEvent *ev)
{
    if (m_pixelLinkage && (ev->button() & Qt::LeftButton))
    {
        double x = 0, y = 0;
        if (pixelLinkPos(ev->globalPos(), x, y))
        {
            emit sigPixelLinkage(x, y);
        }
        return;
    }
    CShowAutoImageWidget::mousePressEvent(ev);
}

void CItemImageWidget::contextMenuEvent(QContextMenuEvent *p_event)
{
    m_pItemsContextMenu->clear();
    if (m_itemsEnabled)
    {
        m_pItemsContextMenu->addActions(m_manuItemActions);
        if (!m_pScene->myItems().isEmpty())
        {
            m_pItemsContextMenu->addAction(m_pItemsEnabledAction);
        }
    }
    else
    {
        m_pItemsContextMenu->addAction(m_pItemsEnabledAction);
    }
    m_pItemsContextMenu->exec(QCursor::pos());
}

void CItemImageWidget::initItemsContextMenu()
{
    auto addAction = [this](QString p_name)
    {
        auto pAction = new QAction(p_name);
        pAction->setIconVisibleInMenu(false);
        m_manuItemActions.append(pAction);
        return pAction;
    };
    m_pItemsContextMenu = new QMenu(this);
    auto pAction = addAction(tr("Add horizontal line"));
    connect(pAction, &QAction::triggered, this, [this]
            {
                auto pos = m_pView->mapToScene(m_pView->mapFromGlobal(QCursor::pos())) ;
                addLineItem(pos.y(),Qt::Orientation::Horizontal); });
    pAction = addAction(tr("Add vertical line"));
    connect(pAction, &QAction::triggered, this, [this]
            {
                auto pos = m_pView->mapToScene(m_pView->mapFromGlobal(QCursor::pos())) ;
                addLineItem(pos.x(),Qt::Orientation::Vertical); });
    pAction = addAction(tr("Add rectangle"));
    connect(pAction, &QAction::triggered, this, [this]
            {
                auto rect = m_pView->mapToScene(m_pView->rect()).boundingRect();
                rect.adjust(rect.width()/4,rect.height()/4,-rect.width()/4,-rect.height()/4);
                auto pRectItem = new BS_QT_Ui::CRulerRectItem;
                pRectItem->addKeyPoints(QList<QPointF>() << rect.topLeft() << rect.bottomRight());
                addItem(pRectItem); });
    pAction = addAction(tr("Add circle"));
    connect(pAction, &QAction::triggered, this, [this]
            {
                auto rect = m_pView->mapToScene(m_pView->rect()).boundingRect();
                rect.adjust(rect.width()/4,rect.height()/4,-rect.width()/4,-rect.height()/4);
                auto pRectItem = new BS_QT_Ui::CRulerCircularItem;
                pRectItem->addKeyPoints(QList<QPointF>() <<  rect.center() << rect.bottomRight());
                addItem(pRectItem); });
    pAction = addAction(tr("Clear"));
    connect(pAction, &QAction::triggered, this, [this]
            { clearItem(); });

    m_pItemsEnabledAction = new QAction("");
    connect(m_pItemsEnabledAction, &QAction::triggered, this, [this]
            { setItemsEnable(!m_itemsEnabled); });
}

void CItemImageWidget::slotUpdateItemState()
{
    for (auto pItem : m_pScene->myItems())
    {
        pItem->setVisible(m_usedItem);
    }
    if (m_usedItem)
    {
        for (auto pItem : m_pScene->myItems())
        {
            auto flags = pItem->flags();
            pItem->setFlags(
                m_itemsEnabled && (m_pixelLinkage == false) ? (flags | QGraphicsItem::ItemIsSelectable)
                                                            : (flags & ~QGraphicsItem::ItemIsSelectable));
            pItem->setSelected(false);
        }
    }
    if (m_usedItem && (m_pixelLinkage == false))
    {
        setContextMenuPolicy(Qt::DefaultContextMenu);
    }
    else
    {
        setContextMenuPolicy(Qt::NoContextMenu);
        if (m_pItemsContextMenu->isVisible())
        {
            m_pItemsContextMenu->close();
        }
    }
}