#include "booleanScene.h"
#include "Ui/graphics/item/iItem.h"
#include "Ui/graphics/tool/graphicsToolFun.h"
#include "ui/graphicsView/imageView.h"
#include "message/msgManager.h"

#include <QImage>
#include <QGraphicsSceneHelpEvent>
#include <QGraphicsPixmapItem>
#include <QToolTip>
#include <QPainter>
#include <QTimer>
#include <QTime>
namespace TIGER_Graphics
{
    using namespace BS_QT_Ui;
    CBooleanScene::CBooleanScene(TIGER_Graphics::CImageView *p_pView, QObject *parent)
        : CBasicItemScene(parent),
          m_pView(p_pView)
    {
        assert(m_pView != nullptr);
        m_pImageItem = new QGraphicsPixmapItem;
        m_pImageItem->setFlags(m_pImageItem->flags() & ~(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable));
        m_pImageItem->setTransformationMode(Qt::FastTransformation);
        m_pImageItem->setZValue(-999);
        addItem(m_pImageItem);
    }

    CBooleanScene::~CBooleanScene()
    {
        if (m_isDrawing)
        {
            delPtr(m_pDrawShape);
            stopDrawShape();
        }
    }
    void CBooleanScene::clearImage()
    {
        m_pImageItem->setPixmap(QPixmap());
        m_pImageItem->hide();
    }

    void CBooleanScene::showImage(const QPixmap &p_pImage, bool p_resetRet)
    {
        m_pImageItem->setPixmap(p_pImage);
        const double dx = -p_pImage.width() * 0.5;
        const double dy = -p_pImage.height() * 0.5;
        if (p_pImage.isNull())
        {
            m_pImageItem->hide();
        }
        else
        {
            m_pImageItem->setPos(dx, dy);
            m_pImageItem->show();
            if (p_resetRet)
            {
                auto rect = p_pImage.rect();
                rect.translate(dx, dy);
                setSceneRect(rect);
                m_pView->resetView();
            }
        }
    }

    QPixmap CBooleanScene::getImage() const
    {
        return m_pImageItem->pixmap();
    }

    void CBooleanScene::setCurrUnitedPath(const BS_Qt_Solid::CPaths2d &p_paths)
    {
        m_currentUnitedPath = p_paths;
        updateCurrentPath();
    }

    void CBooleanScene::removeMyItem(QGraphicsItem *p_item)
    {
        BS_QT_Ui::IScene<IGraphicsBasicItem>::removeMyItem(p_item);
        updateCurrentPath();
        emit sigItemChanged();
    }

    QList<QGraphicsItem *> CBooleanScene::myGraphicsItems() const
    {
        auto pItems = IScene<IGraphicsBasicItem>::myGraphicsItems();
        if (pItems.size() > 1)
        {
            auto sort = [](const QGraphicsItem *item1, const QGraphicsItem *item2)
            {
                return item1->zValue() < item2->zValue();
            };
            qSort(pItems.begin(), pItems.end(), sort);
        }
        return pItems;
    }

    void CBooleanScene::drawShape(CBooleanType p_type, IGraphicsBasicItem *p_shape)
    {
        assert(p_shape != nullptr && !p_shape->isDrawFinished());
        setItemBooleanType(p_shape, p_type);
        p_shape->setAcceptedHoverMode(IGraphicsBasicItem::SelectedAcceptedHover);
        stopDrawShape();
        m_isDrawing = true;
        disconnect(m_connectDrawFinish);
        disconnect(p_shape, &IGraphicsBasicItem::sigDrawFinished, this, nullptr);
        m_connectDrawFinish = connect(
            p_shape, &IGraphicsBasicItem::sigDrawFinished, this, [this, pShape = p_shape]
            {
                disconnect(m_connectDrawFinish);
                if (m_isDrawing)
                {
                    m_isDrawing = false;
                    stopDrawShape();
                    assert(pShape->isDrawFinished());
                    m_myItems << pShape;
                    disconnect(pShape, &IGraphicsBasicItem::sigShapeChanged, this, nullptr);
                    disconnect(pShape, &IGraphicsBasicItem::sigShapeChangeFinished, this, &CBooleanScene::sigItemChanged);
                    connect(pShape, &IGraphicsBasicItem::sigShapeChanged, this, &CBooleanScene::updateCurrentPath);
                    connect(pShape, &IGraphicsBasicItem::sigShapeChangeFinished, this, &CBooleanScene::sigItemChanged);
                    pShape->setToolTip(tr("%2,boolean type:%1").arg(cBooleanStrMap[getItemBooleanType(pShape)]).arg(pShape->toolTip()));
                    this->clearSelection();
                    // 合并
                    updateUnitedPath();
                    updateCurrentPath();
                    QTimer::singleShot(0, this,&CBooleanScene::sigDrawFinished);
                    QTimer::singleShot(0, this,&CBooleanScene::sigItemChanged);
                } });

        CGraphicsToolFun::setOverrideCursor(Qt::CrossCursor);
        addItem(p_shape);
        m_pDrawShape = p_shape;
        this->clearSelection();
    }

    void CBooleanScene::stopDrawShape()
    {
        CGraphicsToolFun::restoreOverrideCursor();
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            removeItem(m_pDrawShape);
            m_pDrawShape->deleteLater();
        }
        m_pDrawShape = nullptr;
        m_isDrawing = false;
    }

    bool CBooleanScene::isDrawing() const
    {
        return m_isDrawing;
    }

    void CBooleanScene::clearMyItem()
    {
        clearSelection();
        qDeleteAll(m_myItems);
        m_myItems.clear();
        stopDrawShape();
        assert(m_pDrawShape == nullptr);
        m_currentPath.clear();
        m_currentUnitedPath.clear();
        emit sigItemChanged();
    }

    void CBooleanScene::updateUnitedPath(int p_unitedStep)
    {
        while (m_myItems.size() > p_unitedStep)
        {
            auto pShape = m_myItems[0];
            pShape->setSelected(false);
            m_myItems.removeAt(0);
            removeItem(pShape);
            m_currentUnitedPath = booleanPath(m_currentUnitedPath, pShape->mapToScene(pShape->drawShape()), getItemBooleanType(pShape));
            delPtr(pShape);
        }
    }

    void CBooleanScene::updateCurrentPath()
    {
        m_currentPath = m_currentUnitedPath;
        for (auto &pShape : m_myItems)
        {
            m_currentPath = booleanPath(m_currentPath, pShape->mapToScene(pShape->drawShape()), getItemBooleanType(pShape));
        }
    }

    void CBooleanScene::updatePathPreview(const QPainterPath &p_previewShapePath) const
    {
        auto type = getItemBooleanType(m_pDrawShape);
        assert(type == SubtractedBoolean || type == IntersectedBoolean);
        auto getIds = [](const QRectF &p_rect, QList<QPointF> p_points)
        {
            QVector<int> ids;
            for (int i = 0; i < p_points.size(); i++)
            {
                if (p_rect.contains(p_points[i]))
                {
                    ids << i;
                }
            }
            return ids;
        };
        auto poins = m_pView->getShowPoints();
        QList<QPointF> showPoins;
        QVector<int> ids = getIds(p_previewShapePath.boundingRect(), poins);
        auto paths = m_currentPath.fromQPath(p_previewShapePath);
        if (type == SubtractedBoolean)
        {
            showPoins = poins;
            for (int i = ids.size() - 1; i >= 0; i--)
            {
                const auto &id = ids[i];
                if (paths.contains(BS_Qt_Solid::CPoint2d(showPoins[id].x(), showPoins[id].y()), true))
                {
                    showPoins.removeAt(id);
                }
            }
            m_pView->showPoints(showPoins);
        }
        else
        {
            // for (int i = ids.size() - 1; i >= 0; i--)
            // {
            //     const auto &id = ids[i];
            //     if (TIGER_ClipperTool::contains(path, poins[id]))
            //     {
            //         showPoins.append(poins[id]);
            //     }
            // }
        }
        // m_pView->showPoints(showPoins);
    }

    QPainterPath CBooleanScene::getPath() const
    {
        auto currentPath = m_currentPath;
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            assert(!m_pDrawShape->isDrawFinished());
            auto type = getItemBooleanType(m_pDrawShape);
            // 画的时候是没有变换的,不用map
            QPainterPath drawingShapePath = m_pDrawShape->getPreviewShape();
            if (!drawingShapePath.isEmpty())
            {
                // 封闭轮廓进行运算,开放轮廓直接画.否则开放轮廓会被直接抹去
                currentPath = booleanPath(currentPath, drawingShapePath, drawingShapePath.toFillPolygon().isClosed() ? type : UnitedBoolean);
            }
            if (type != UnitedBoolean &&
                m_pDrawShape->getType() == CItemDef::CGraphicsItemShape::cgitPath &&
                m_pDrawShape->isDrawing())
            {
                updatePathPreview(drawingShapePath);
            }
        }
        return currentPath.toQPath();
    }

    BS_Qt_Solid::CPaths2d CBooleanScene::booleanPath(const BS_Qt_Solid::CPaths2d &path1, const QPainterPath &path2, const CBooleanType &type)
    {
        auto path = path1;
        if (path2.isEmpty())
        {
            return path;
        }
        auto paths2d = path.fromQPath(path2);
        switch (type)
        {
        case UnitedBoolean:
            path = path.Union(paths2d);
            break;
        case IntersectedBoolean:
            path = path.Intersect(paths2d);
            break;
        case SubtractedBoolean:
            path = path.Difference(paths2d);
            break;
        default:
            Out_Warning << QString("CBooleanScene unkown type(%1)").arg(type) << "is no join boolean";
            break;
        }
        return path;
    }

    void CBooleanScene::helpEvent(QGraphicsSceneHelpEvent *event)
    {
        QString srt;
        for (auto pItem : this->items())
        {
            if (pItem->isSelected() && pItem->contains(pItem->mapFromScene(event->scenePos())))
            {
                if (pItem->toolTip().size() != 0)
                {
                    srt += pItem->toolTip();
                    srt.append("\n");
                }
            }
        }
        srt.size() > 1 ? QToolTip::showText(event->screenPos(), srt.left(srt.size() - 1)) : QToolTip::hideText();
    }

    void CBooleanScene::drawForeground(QPainter *painter, const QRectF &rect)
    {
        BS_QT_Ui::IScene<IGraphicsBasicItem>::drawForeground(painter, rect);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(m_pathBrush);
        painter->drawPath(getPath());

        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            if (m_pDrawShape->getType() != CItemDef::cgitPath)
            {
                // painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                QPainterPath drawingPath = m_pDrawShape->getPreviewShape();
                painter->setPen(QPen(Qt::magenta, 0, Qt::SolidLine));
                painter->setBrush(Qt::NoBrush);
                painter->drawPath(drawingPath);
            }
        }
        painter->restore();
    }

    void CBooleanScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            m_pDrawShape->drawingMousePressEvent(event);
            this->update();
            return;
        }
        BS_QT_Ui::IScene<IGraphicsBasicItem>::mousePressEvent(event);
    }

    void CBooleanScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            m_pDrawShape->drawingMouseMoveEvent(event);
            this->update();
            return;
        }
        BS_QT_Ui::IScene<IGraphicsBasicItem>::mouseMoveEvent(event);
    }

    void CBooleanScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            m_pDrawShape->drawingMouseReleaseEvent(event);
            this->update();
            return;
        }
        BS_QT_Ui::IScene<IGraphicsBasicItem>::mouseReleaseEvent(event);
    }

    void CBooleanScene::keyPressEvent(QKeyEvent *event)
    {
        if (m_isDrawing && m_pDrawShape != nullptr)
        {
            m_pDrawShape->drawingKeypressevent(event);
            this->update();
            return;
        }
        BS_QT_Ui::IScene<IGraphicsBasicItem>::keyPressEvent(event);
    }

#define __BooleanTypeKeyStr "BooleanScene_BooleanType"
    CBooleanType CBooleanScene::getItemBooleanType(BS_QT_Ui::IGraphicsBasicItem *p_shape) const
    {
        auto type = p_shape->property(__BooleanTypeKeyStr);
        return type.isValid() ? CBooleanType(type.toInt()) : UnitedBoolean;
    }

    void CBooleanScene::setItemBooleanType(BS_QT_Ui::IGraphicsBasicItem *p_shape, CBooleanType p_type) const
    {
        p_shape->setProperty(__BooleanTypeKeyStr, (int32_t)p_type);
    }
#undef __BooleanTypeKeyStr
}
