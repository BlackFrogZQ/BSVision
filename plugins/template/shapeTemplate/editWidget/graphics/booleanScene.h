#pragma once
#include "booleanItemDef.h"
#include "Ui/graphics/basicItemScene.h"
#include "solid/path.h"
namespace TIGER_Graphics
{
    class CImageView;

    class CBooleanScene : public BS_QT_Ui::CBasicItemScene
    {
        Q_OBJECT

    public:
        CBooleanScene(TIGER_Graphics::CImageView *p_pView, QObject *parent = nullptr);
        ~CBooleanScene();

        void clearImage();
        void showImage(const QPixmap &p_pImage, bool p_resetRet = false);
        QPixmap getImage() const;

        void setCurrUnitedPath(const BS_Qt_Solid::CPaths2d& p_paths);

        void removeMyItem(QGraphicsItem *p_item) override;
        void clearMyItem() override;
        QList<QGraphicsItem *> myGraphicsItems() const override;
        void drawShape(CBooleanType p_type, BS_QT_Ui::IGraphicsBasicItem *p_pItem);
        void stopDrawShape();
        bool isDrawing() const;
        QPainterPath getPath() const;
        static BS_Qt_Solid::CPaths2d booleanPath(const BS_Qt_Solid::CPaths2d &path1, const QPainterPath &path2, const CBooleanType &type);

    signals:
        void sigDrawFinished();
        void sigItemChanged();

    protected:
        void updateCurrentPath();
        void updateUnitedPath(int p_unitedStep = 3);
        void updatePathPreview(const QPainterPath &p_previewShapePath) const;
        CBooleanType getItemBooleanType(BS_QT_Ui::IGraphicsBasicItem *p_shape) const;
        void setItemBooleanType(BS_QT_Ui::IGraphicsBasicItem *p_shape,CBooleanType p_type) const;

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
        virtual void keyPressEvent(QKeyEvent *event) override;
        virtual void helpEvent(QGraphicsSceneHelpEvent *event) override;
        virtual void drawForeground(QPainter *painter, const QRectF &rect) override;

    protected:
        BS_Qt_Solid::CPaths2d m_currentPath;
        BS_Qt_Solid::CPaths2d m_currentUnitedPath;
        BS_QT_Ui::IGraphicsBasicItem *m_pDrawShape = nullptr;
        QMetaObject::Connection m_connectDrawFinish;
        TIGER_Graphics::CImageView *m_pView = nullptr;
        QGraphicsPixmapItem *m_pImageItem = nullptr;
        QBrush m_pathBrush = QBrush(QColor(193, 255, 193, 127), Qt::SolidPattern);
        bool m_isDrawing = false;
    };
}
