#include "visionApplication.h"
#include <QProcess>
int main(int argc, char *argv[])
{
#if defined(BuildBSEXPORT)
    error
#endif
        // 配置
        // qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
        // qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
        // qputenv("BS_Debug_FrameWork","1");
        // qputenv("BS_Debug_Plugin","1");
        CVisionApplication instance(argc, argv);
    if (instance.init())
    {
        return instance.exec(false);
    }
    return 0;
}
