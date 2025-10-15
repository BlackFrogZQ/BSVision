#pragma once
#include "Ui/graphics/imageScene.h"
#include <QPainterPath>
class QImage;
namespace TIGER_Graphics
{
    class IItemScene : public BS_QT_Ui::CImageScene
    {
        Q_OBJECT

    public:
        IItemScene(QObject *parent = nullptr);
        virtual ~IItemScene();
        void clearItem();
        virtual QPainterPath getPath() const;
        virtual QList<QGraphicsItem *> myGraphicsItems() const override;
        virtual void removeMyItem(QGraphicsItem *pItem) override;
        void setBackgroundImage(QImage const *p_pImage);
        void setInitPath(const QPainterPath &p_path);

    signals:
        // 删除前发送
        void sigRemoveItem(QGraphicsItem *p_item);
        void sigItemChanged();

    protected:
        void addItemShape(QGraphicsItem *p_pItem);
        virtual void clearMyItem() override;

    protected:
        virtual void helpEvent(QGraphicsSceneHelpEvent *event) override;
        virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
        virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

    protected:
        QImage const *m_pImage;
        QPainterPath m_initPath;
    };
};