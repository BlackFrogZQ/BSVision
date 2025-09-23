#pragma once
#include "positioningToolDef.h"
#include <QGraphicsItemGroup>
class QAction;
namespace BS_QT_Ui
{
    class IGraphicsItem;
    class CItemContextMenu;
}
class CGraphicsToolItemCreater;
namespace TIGER_Graphics
{
    class IPositioningToolItem : public QObject, public QGraphicsItemGroup
    {
        friend class ::CGraphicsToolItemCreater;
        Q_OBJECT
    public:
        IPositioningToolItem(QGraphicsItem *parent = nullptr);
        virtual ~IPositioningToolItem();
        virtual QRectF boundingRect() const override;
        virtual QPainterPath shape() const override;
        virtual void paint(QPainter *p_painter, const QStyleOptionGraphicsItem *p_option, QWidget *p_widget) override;
        virtual QPointF getPosition();
        virtual QList<QPainterPath> getToolPaths() const;
        virtual void setShapeSize(QPointF center, double w, double h) = 0;
        QPainterPath getToolShape() const;
        void setLineWidth(int p_w);
        void updateKeyPointRect();
        void setEnableOffset(bool p_enable);
        void setOffset(double p_dx, double p_dy);
        void getOffset(double &p_dx, double &p_dy) const;
        void setSelectable(bool p_isSelectable);

    signals:
        void sigGroupShapeChanged();
        void sigGroupShapeOffset(double p_dx, double p_dy);

    protected:
        void add(BS_QT_Ui::IGraphicsItem *item);
        void remove(BS_QT_Ui::IGraphicsItem *item);
        void updatePath();
        QPainterPath getCrossPosition() const;
        virtual QPainterPath drawShape() const;
        virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    protected:
        QList<BS_QT_Ui::IGraphicsItem *> m_child;
        QPointF m_position;
        QPainterPath m_path;
        BS_QT_Ui::CItemContextMenu *m_pContextMenu = nullptr;
        QAction *m_pOffsetAction;
        bool m_selectable = true;
        double m_dx = 0.0;
        double m_dy = 0.0;
    };
};

class CGraphicsToolItemCreater
{
public:
    static TIGER_Graphics::IPositioningToolItem *createGraphicsToolItem(const TIGER_Graphics::CPosToolType &p_type);
};