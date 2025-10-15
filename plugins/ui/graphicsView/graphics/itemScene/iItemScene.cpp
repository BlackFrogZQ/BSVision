#include "iItemScene.h"
#include <QImage>
#include <QGraphicsItem>
#include <QGraphicsSceneHelpEvent>
#include <QToolTip>
#include <QPainter>

using namespace BS_QT_Ui;
namespace TIGER_Graphics
{
    IItemScene::IItemScene(QObject *parent)
        : CImageScene(parent),
          m_pImage(nullptr)
    {
        setBackgroundBrush(Qt::transparent);
    }

    IItemScene::~IItemScene()
    {
        clearItem();
    }

    void IItemScene::setInitPath(const QPainterPath &p_path)
    {
        m_initPath = p_path;
        this->update();
    }

    QPainterPath IItemScene::getPath() const
    {
        return m_initPath;
    }

    void IItemScene::clearItem()
    {
        clearMyItem();
        for (auto pItems : items())
        {
            this->removeItem(pItems);
            delPtr(pItems);
        }
        this->update();
    }

    void IItemScene::clearMyItem()
    {
        m_initPath = QPainterPath();
    }

    QList<QGraphicsItem *> IItemScene::myGraphicsItems() const
    {
        return QList<QGraphicsItem *>();
    }

    void IItemScene::removeMyItem(QGraphicsItem *pItem)
    {
        Q_UNUSED(pItem);
    }

    void IItemScene::addItemShape(QGraphicsItem *p_pItem)
    {
        assert(p_pItem != nullptr);
        this->addItem(p_pItem);
        this->clearSelection();
        emit sigItemChanged();
    }

    void IItemScene::setBackgroundImage(QImage const *p_pImage)
    {
        assert(p_pImage != nullptr);
        m_pImage = p_pImage;
        const double dx = m_pImage->width() * 0.5;
        const double dy = m_pImage->height() * 0.5;
        setSceneRect(-dx, -dy, m_pImage->width(), m_pImage->height());
    }

    void IItemScene::helpEvent(QGraphicsSceneHelpEvent *event)
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

    void IItemScene::drawBackground(QPainter *painter, const QRectF &rect)
    {
        IScene::drawBackground(painter,rect);
        if (m_pImage != nullptr && (!m_pImage->isNull()))
        {
            painter->save();
            painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
            const double dx = m_pImage->width() * 0.5;
            const double dy = m_pImage->height() * 0.5;
            painter->drawImage(-dx, -dy, *m_pImage);
            painter->restore();
        }
    }

    void IItemScene::drawForeground(QPainter *painter, const QRectF &rect)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        // painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        // painter->setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
        painter->setPen(QPen(Qt::NoPen));
        painter->setBrush(QBrush(QColor(193, 255, 193, 127), Qt::SolidPattern));
        painter->drawPath(getPath());
        painter->restore();
    }
};