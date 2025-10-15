#include "templateScene.h"

#include "Ui/graphics/graphicsView.h"
#include "Ui/graphics/item/iItem.h"
#include "Ui/graphics/tool/graphicsToolFun.h"
#include "message/msgManager.h"

#include <QImage>
#include <QGraphicsSceneHelpEvent>
#include <QToolTip>
#include <QPainter>
#include <QTime>

using namespace BS_QT_Ui;
namespace TIGER_Graphics
{
    CTemplateScene::CTemplateScene(CImageView *p_pView, QObject *parent)
        : IItemScene(parent),
          m_pView(p_pView),
          m_isDrawing(false)
    {
        assert(m_pView != nullptr);
    }

    CTemplateScene::~CTemplateScene()
    {
        if (m_isDrawing)
        {
            delPtr(m_drawShape.pItem);
            stopDrawShape();
        }
    }

    void CTemplateScene::addShape(CGraphicsBasicShape p_shape)
    {
        if (m_isDrawing)
        {
            delPtr(m_drawShape.pItem);
            stopDrawShape();
        }

        BS_QT_Ui::IGraphicsBasicItem *pItem = p_shape.pItem;
        assert(pItem != nullptr && pItem->isDrawFinished());
        pItem->disconnect(pItem, &BS_QT_Ui::IGraphicsBasicItem::sigShapeChanged, this, &CTemplateScene::sigItemChanged);
        connect(pItem, &BS_QT_Ui::IGraphicsBasicItem::sigShapeChanged, this,
                [this]
                {
                    updateCurrentPath();
                    emit sigItemChanged();
                });
        pItem->setToolTip(QObject::tr("%2,布尔运算:%1").arg(cDrawTypeStr[p_shape.type]).arg(pItem->toolTip()));
        m_shapes.append(p_shape);
        addItemShape(pItem);
        // 合并
        updateUnitedPath();
        updateCurrentPath();
    }

    void CTemplateScene::removeMyItem(QGraphicsItem *p_item)
    {
        p_item = parentItem(p_item);
        for (auto pItem : m_shapes)
        {
            if (pItem.pItem == p_item)
            {
                m_shapes.removeOne(pItem);
            }
        }
        updateCurrentPath();
        emit sigRemoveItem(p_item);
        this->removeItem(p_item);
        delPtr(p_item);
        emit sigItemChanged();
    }

    QList<QGraphicsItem *> CTemplateScene::myGraphicsItems() const
    {
        QList<QGraphicsItem *> pItems;
        for (auto pShape : m_shapes)
        {
            pItems.append(pShape.pItem);
        }
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

    void CTemplateScene::drawShape(CGraphicsBasicShape p_shape)
    {
        assert(p_shape.pItem != nullptr && !p_shape.pItem->isDrawFinished());
        if (m_isDrawing)
        {
            delPtr(m_drawShape.pItem);
            stopDrawShape();
        }
        m_connectDrawFinish = connect(
            p_shape.pItem, &BS_QT_Ui::IGraphicsBasicItem::sigDrawFinished, this, [this, shape = p_shape]
            {
                assert(shape.pItem->isDrawFinished());
                m_isDrawing = false;
                emit sigDrawFinished(shape);
                stopDrawShape();
                disconnect(m_connectDrawFinish); });
        connect(p_shape.pItem, &BS_QT_Ui::IGraphicsBasicItem::sigShapeChanged, this, &CTemplateScene::sigItemChanged);
        CGraphicsToolFun::setOverrideCursor(Qt::CrossCursor);
        m_drawShape = p_shape;
        m_isDrawing = true;
        this->clearSelection();
    }

    void CTemplateScene::stopDrawShape()
    {
        CGraphicsToolFun::restoreOverrideCursor();
        m_drawShape.clear();
        m_isDrawing = false;
    }

    bool CTemplateScene::isDrawing() const
    {
        return m_isDrawing;
    }

    void CTemplateScene::clearMyItem()
    {
        m_shapes.clear();
        if (m_isDrawing)
        {
            delPtr(m_drawShape.pItem);
            stopDrawShape();
        }
        assert(m_drawShape.pItem == nullptr);
        m_initPath = QPainterPath();
        m_currentPath.clear();
        m_currentUnitedPath.clear();
    }

    void CTemplateScene::updateUnitedPath(int p_unitedStep)
    {
        if (m_shapes.size() >= (p_unitedStep + 1))
        {
            for (size_t i = 0; i < (m_shapes.size() - p_unitedStep); i++)
            {
                auto &pShape = m_shapes.first();
                m_currentUnitedPath = booleanPath(m_currentUnitedPath, pShape.pItem->mapToScene(pShape.pItem->drawShape()), pShape.type);
                delPtr(pShape.pItem);
                m_shapes.removeFirst();
            }
        }
    }

    void CTemplateScene::updateCurrentPath()
    {
        m_currentPath = m_currentUnitedPath;
        for (size_t i = 0; i < m_shapes.size(); i++)
        {
            auto &pShape = m_shapes[i];
            m_currentPath = booleanPath(m_currentPath, pShape.pItem->mapToScene(pShape.pItem->drawShape()), pShape.type);
        }
    }

    void CTemplateScene::updateViewShowPoints(const QPainterPath &p_previewShapePath) const
    {
        assert(m_drawShape.type == CSceneDef::cltSubtracted || m_drawShape.type == CSceneDef::cltIntersected);
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
        auto path = TIGER_ClipperTool::toPath(p_previewShapePath);
        if (m_drawShape.type == CSceneDef::cltSubtracted)
        {
            showPoins = poins;
            for (int i = ids.size() - 1; i >= 0; i--)
            {
                const auto &id = ids[i];
                if (TIGER_ClipperTool::contains(path, showPoins[id]))
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

    Clipper2Lib::PathsD CTemplateScene::booleanPath(const Clipper2Lib::PathsD &path1, const QPainterPath &path2, const CSceneDef::CDrawType &type)
    {
        Clipper2Lib::PathsD path = path1;
        if (path2.isEmpty())
        {
            return path;
        }
        switch (type)
        {
        case CSceneDef::cltUnited:
            path = TIGER_ClipperTool::Union(path, path2, 2);
            break;
        case CSceneDef::cltIntersected:
            path = TIGER_ClipperTool::Intersect(path, path2, 2);
            break;
        case CSceneDef::cltSubtracted:
            path = TIGER_ClipperTool::Difference(path, path2, 2);
            break;
        default:
            Out_SyncMsgBox << type << "is no join boolean";
            break;
        }
        return path;
    }

    QPainterPath CTemplateScene::getPath() const
    {
        if (!m_initPath.isEmpty())
        {
            return m_initPath;
        }
        if (!m_isDrawing || m_drawShape.pItem == nullptr)
        {
            return TIGER_ClipperTool::toQPath(m_currentPath);
        }

        auto currentPath = m_currentPath;
        assert(!m_drawShape.pItem->isDrawFinished());
        // 画的时候是没有变换的,不用map
        QPainterPath drawingShapePath = m_drawShape.pItem->getPreviewShape();
        if (!drawingShapePath.isEmpty())
        {
            // 封闭轮廓进行运算,开放轮廓直接画.否则开放轮廓会被直接抹去
            currentPath = booleanPath(currentPath, drawingShapePath, drawingShapePath.toFillPolygon().isClosed() ? m_drawShape.type : CSceneDef::cltUnited);
        }
        if (m_drawShape.type != CSceneDef::cltUnited &&
            m_drawShape.pItem->getType() == CItemDef::CGraphicsItemShape::cgitPath &&
            m_drawShape.pItem->isDrawing())
        {
            updateViewShowPoints(drawingShapePath);
        }
        return TIGER_ClipperTool::toQPath(currentPath);
    }

    void CTemplateScene::drawForeground(QPainter *painter, const QRectF &rect)
    {
        IItemScene::drawForeground(painter, rect);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        // painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
        painter->setBrush(QBrush(QColor(193, 255, 193, 127), Qt::SolidPattern));

        if (m_isDrawing && m_drawShape.pItem != nullptr)
        {
            if (m_drawShape.pItem->getType() != CItemDef::cgitPath)
            {
                QPainterPath drawingPath = m_drawShape.pItem->getPreviewShape();
                painter->setPen(QPen(Qt::magenta, 1, Qt::SolidLine));
                painter->setBrush(Qt::NoBrush);
                painter->drawPath(drawingPath);
            }
        }
        painter->restore();
    }

    void CTemplateScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_drawShape.pItem != nullptr)
        {
            m_drawShape.pItem->drawingMousePressEvent(event);
            this->update();
            return;
        }
        IItemScene::mousePressEvent(event);
    }

    void CTemplateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_drawShape.pItem != nullptr)
        {
            m_drawShape.pItem->drawingMouseMoveEvent(event);
            this->update();
            return;
        }
        IItemScene::mouseMoveEvent(event);
    }

    void CTemplateScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (m_isDrawing && m_drawShape.pItem != nullptr)
        {
            m_drawShape.pItem->drawingMouseReleaseEvent(event);
            this->update();
            return;
        }
        IItemScene::mouseReleaseEvent(event);
    }

    void CTemplateScene::keyPressEvent(QKeyEvent *event)
    {
        if (m_isDrawing && m_drawShape.pItem != nullptr)
        {
            m_drawShape.pItem->drawingKeypressevent(event);
            this->update();
            return;
        }
        IItemScene::keyPressEvent(event);
    }
}
