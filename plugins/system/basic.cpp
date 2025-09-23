#include "basic.h"
#include "camera.h"
#include "cameraCreate.h"
#include "service/enumMap.h"
#include "para/paraManager.h"
#include "fileHelper/appHelper.h"

using namespace BS_QT_Framework;
using namespace BS_QT_Service;
using namespace BS_QT_FlieHelper;
using namespace BS_Hal_Camera;
void initCamera()
{
    CParaManager::AddParaInfo(CAppFileHelper::appDirPath() + "/plugins/camera.ParaDef.json");
    CParaManager::AddParaInfo(CAppFileHelper::appDirPath() + "/plugins/app.ParaDef.json");
    CParaManager::load();
    BS_Hal_Camera::CCameraCreater::createCamera(VisionCameraID, CEnumManager::getEnumInfo<BSHalCamera::CCameraType>(CParaManager::GetValue<QString>(cCameraConfigType(VisionCameraID)), BSHalCamera::cctVirtual));
}

void updateCameraParas()
{
    visionCamera()->updateParas();
}

void releaseCamera()
{
    BS_Hal_Camera::CCameraCreater::destroyCameraControl(VisionCameraID);
}