#pragma once

namespace BS_2DVision_Template
{
    class CTemplateHelper;
    class CTemplateCreater
    {
    public:
        static void createTemplate();
        static void destroyTemplate();
        static CTemplateHelper *templateInstance();
    };
}