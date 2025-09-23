#pragma once
#include "fitDef.h"

namespace BS_QT_Ui
{
    class CCtrlPtItem;
}

namespace TIGER_Graphics
{
    class CFitRectToolItem;
    class CFitLineCenterToolItem;
    class CFitRightAngleToolItem;
    class CFitLineItem : public IBasicFitItem
    {
        friend class CFitRectToolItem;
        friend class CFitLineCenterToolItem;
        friend class CFitRightAngleToolItem;
        Q_OBJECT
    public:
        enum CAdjustPtFlag : int32_t
        {
            NoneAdjustPt = 0,
            MoveAdjustPt = 1 << 0,
            ExtendAdjustPt = 1 << 1,
            FitRectHeightAdjustPt = 1 << 2,
            FitRectWidthAdjustPt = 1 << 3,
            FitRectAdjustPt = FitRectHeightAdjustPt | FitRectWidthAdjustPt,
            AllAdjustPt = MoveAdjustPt | ExtendAdjustPt | FitRectAdjustPt
        };
        Q_ENUM(CAdjustPtFlag)
        Q_DECLARE_FLAGS(CAdjustPtFlags, CAdjustPtFlag)
    public:
        CFitLineItem(QGraphicsItem *parent = nullptr);
        ~CFitLineItem();
        CAdjustPtFlags ctrlPtFlags() const;

        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    public slots:
        void setFreeEndPt(bool p_free);
        void setReverseNormal(bool p_reverse);
        void setHalfRectCount(int p_count);
        void setAdjustPtFlag(CAdjustPtFlag p_flag, bool p_on = true);
        void setAdjustPtFlags(CAdjustPtFlags p_flags);

    protected slots:
        void slotMove(QPointF p_scenePos, int p_buttons);
        void slotHalHeightMove(QPointF p_scenePos, int p_buttons);
        void slotHalWidthMove(QPointF p_scenePos, int p_buttons);
        void slotExtendMove(QPointF p_scenePos, int p_buttons);
        void updatePtPos();

    protected:
        BS_QT_Ui::CCtrlPtItem *m_pMovePtItem = nullptr;
        BS_QT_Ui::CCtrlPtItem *m_pExtendPtItem = nullptr;

        BS_QT_Ui::CCtrlPtItem *m_pRectHalfHeightPtItem = nullptr;
        BS_QT_Ui::CCtrlPtItem *m_pRectHalfWidthPtItem = nullptr;

        // 场景坐标
        QPointF m_startPt = QPointF(0, 0);
        QPointF m_endPt = QPointF(100, 100);
        QPointF m_latstPressPt;

        // y负向
        QPointF m_lineNor;
        QPointF m_lineDir;
        double m_lineAngle = 0;
        double m_fitRectInterval;

        bool m_freeEndPt = true;
        bool m_reverseNormal = false;
        double m_fitRectHalHeight = 20;
        double m_fitRectHalWidth = 5;
        int m_fitRectHalfCount = 5;
        CAdjustPtFlags m_ctrlPtFlags = AllAdjustPt;
    };
    Q_DECLARE_OPERATORS_FOR_FLAGS(CFitLineItem::CAdjustPtFlags)
}