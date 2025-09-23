#pragma once
#include <QMap>
#include <QFrame>
namespace BS_2DVision_Template
{
    class CTemplateDef
    {
        Q_GADGET
    public:
        enum
        {
            Subdivision_UnknownType = -1
        };
        enum CTemplateType : int32_t
        {
            ShapeTemplate = 0,
            ParaTemplate,
            UnknownTemplate,
            MaxTemplate = UnknownTemplate
        };
        Q_ENUM(CTemplateType)
    };

    class ITemplateEditWidget : public QFrame
    {
        Q_OBJECT
    public:
        using QFrame::QFrame;
        virtual ~ITemplateEditWidget() {};
    signals:
        void sigEditChanged(bool p_edit, QWidget *p);
    };

    const QMap<int, QString> cTemplateNames =
        {
            {CTemplateDef::ShapeTemplate, QObject::tr("ShapeTemplate")},
            {CTemplateDef::ParaTemplate, QObject::tr("ParaTemplate")},
            {CTemplateDef::UnknownTemplate, QObject::tr("UnknownTemplate")}};
}
