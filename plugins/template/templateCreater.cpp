#include "templateCreater.h"
#include "templateHelper.h"
namespace BS_2DVision_Template
{
    CTemplateHelper *gTemplateHelper = nullptr;

    void CTemplateCreater::createTemplate()
    {
        if (gTemplateHelper == nullptr)
        {
            gTemplateHelper = new CTemplateHelper;
            gTemplateHelper->init();
        }
    }

    void CTemplateCreater::destroyTemplate()
    {
        if (gTemplateHelper != nullptr)
        {
            delete gTemplateHelper;
            gTemplateHelper = nullptr;
        }
    }

    CTemplateHelper *CTemplateCreater::templateInstance()
    {
        return gTemplateHelper;
    }
}