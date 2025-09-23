#include "markParaDef.h"

namespace BS_2DVision_Template
{
    IMarkTemplateParaWidget::IMarkTemplateParaWidget(QWidget *p)
        : QFrame(p), m_pMarkTemplatePara(nullptr)
    {
    }

    IMarkTemplateParaWidget::~IMarkTemplateParaWidget()
    {
    }

    void IMarkTemplateParaWidget::attachMarkPara(QVariant *pMarkPara)
    {
        assert(pMarkPara != nullptr);
        m_pMarkTemplatePara = pMarkPara;
    }

    bool CMarkPoint::operator==(const CMarkPoint &other) const
    {
        return usedRoi == other.usedRoi &&
               roi == other.roi &&
               oriPos == other.oriPos &&
               newPos == other.newPos &&
               templateType == other.templateType &&
               templateParas == other.templateParas;
    }
}

#include "service/typeRegistrar.h"
RegisterType(BS_2DVision_Template, CMarkPoint);
RegStreamOperator(BS_2DVision_Template, CMarkPoint);
RegistEqualsComparator(BS_2DVision_Template, CMarkPoint);
QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CMarkPoint &mark)
{
    uint16_t ver = 1;
    uint32_t flag = qHash(QString("::CMarkPoint"));
    arch
        << flag
        << ver;
    // ver = 1
    arch << mark.adjustRoi;

    // ver = 0
    arch << mark.usedRoi
         << mark.roi
         << mark.oriPos
         << mark.newPos
         << mark.templateType
         << mark.templateParas;
    return arch;
}

QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CMarkPoint &mark)
{
    uint32_t flag = 0;
    arch >> flag;
    if (flag == qHash(QString("::CMarkPoint")))
    {
        uint16_t ver = 0;
        arch >> ver;
        switch (ver)
        {
        case 1:
            arch >> mark.adjustRoi;
        case 0:
            arch >> mark.usedRoi >> mark.roi >> mark.oriPos >> mark.newPos >> mark.templateType >> mark.templateParas;
            break;
        default:
            arch.setStatus(QDataStream::ReadCorruptData);
            break;
        }
    }
    else
    {
        arch.setStatus(QDataStream::ReadCorruptData);
    }
    return arch;
}