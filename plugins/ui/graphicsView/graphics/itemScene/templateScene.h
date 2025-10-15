#pragma once
#include "iItemScene.h"
#include "../sceneDef.h"
#include "../../imageView.h"
#include "clipper2/clipperTool.h"

namespace TIGER_Graphics
{
    class CTemplateScene : public IItemScene
    {
        Q_OBJECT

    public:
        CTemplateScene(CImageView* p_pView,QObject *parent = nullptr);
        ~CTemplateScene();
        void addShape(CGraphicsBasicShape p_shape);
        virtual void removeMyItem(QGraphicsItem *p_item) override;
        virtual QList<QGraphicsItem *> myGraphicsItems() const override;
        // draw完成后不会自动添加到item列表，而是通过sigDrawFinished发送
        void drawShape(CGraphicsBasicShape p_shape);
        void stopDrawShape();
        bool isDrawing() const;
        virtual QPainterPath getPath() const override;
    signals:
        void sigDrawFinished(CGraphicsBasicShape finishShape);

    protected:
        void updateCurrentPath();
        void updateUnitedPath(int p_unitedStep = 3);
        void updateViewShowPoints(const QPainterPath& p_previewShapePath) const;
        static Clipper2Lib::PathsD booleanPath(const Clipper2Lib::PathsD &path1, const QPainterPath &path2, const CSceneDef::CDrawType &type);
    protected:
        virtual void clearMyItem() override;
        virtual void drawForeground(QPainter *painter, const QRectF &rect) override;
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void keyPressEvent(QKeyEvent *event) override;

    protected:
        Clipper2Lib::PathsD m_currentPath;
        Clipper2Lib::PathsD m_currentUnitedPath;
        CGraphicsBasicShape m_drawShape;
        QVector<CGraphicsBasicShape> m_shapes;
        QMetaObject::Connection m_connectDrawFinish;
        CImageView* m_pView;
        bool m_isDrawing;
    };
}
