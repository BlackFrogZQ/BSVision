#pragma once
#include "iCameraCtrl.h"

enum CCameraIdDef:int
{
    VisionCameraID = 0
};

#define visionCamera() camera(VisionCameraID)
