#pragma once
#include "guiDef.h"

#define cImageType "jpeg"


#pragma region "内置对象名"
    #define cSystemUnsavedMsgBox "SystemUnsavedMsgBox"

    #define cInputDialogName "CInputDialog"

    #define cStdMainWidgetTabWidget "StdMainWidgetTabWidget"
    #define cStdMainWidgetToolBtn "StdMainWidgetToolBtn"
    #define cStdMainWidgetToolBtn2 "StdMainWidgetToolBtn2"
    #define cStdPushBtnStyleToolBtn "StdPushBtnStyleToolBtn"

    #define cStdWorkWidget "StdWorkWidget"
    #define cStdWorkWidgetToolBar "StdWorkWidgetToolBar"

#pragma endregion

#pragma region "资源文件定义"
        #define cCamContinuousIcon ":res/Camera/startAcquire.png"
        #define cCamSaveImageIcon ":res/Camera/saveImage.png"
        #define cCamAdjustmentParamsIcon ":res/Camera/adjustmentParams.png"

        #define cStationTestIcon ":res/Station/test.png"

        #define cNewFileIcon ":res/newFile.png"

        #define cWorkWidgetLinkageIcon ":res/WorkWidget/Linkage.png"
        #define cWorkWidgetMeasureIcon ":res/WorkWidget/Measure.png"
        #define cWorkWidgetMarkCtrlIcon ":res/WorkWidget/MarkCtrl.png"

        #define cGraphicsRectIcon ":res/graphics/drawShape/rect.png"
        #define cGraphicsEllipseIcon ":res/graphics/drawShape/ellipse.png"
        #define cGraphicsPathIcon ":res/graphics/drawShape/path.png"
        #define cGraphicsLineIcon ":res/graphics/drawShape/line.png"

        #define cProjectEditCancelIcon ":res/projectEditWidget/Cancel.png"
        #define cProjectEditNewIcon ":res/projectEditWidget/new.png"
        #define cProjectEditSaveIcon ":res/projectEditWidget/save.png"
        #define cProjectEditSaveAsIcon ":res/projectEditWidget/saveAs.png"
        #define cProjectEditEditIcon ":res/projectEditWidget/edit.png"
        #define cProjectEditDelIcon ":res/projectEditWidget/del.png"
        #define cProjectEditFeatureIcon ":res/projectEditWidget/feature.png"
        #define cProjectEditStationIcon ":res/projectEditWidget/station.png"
        #pragma endregion

#pragma region "core参数"
    #define cEngineType "globaldef.a_sys_paras.a_software_paras.b_app_vision_engine"
    #define cSaveImagePath "globaldef.a_sys_paras.b_a_save_image_paras.a_save_path"
    #define cSaveImageMaxCount "globaldef.a_sys_paras.b_a_save_image_paras.b_save_max_count"
    #define cSaveImageMaxTime "globaldef.a_sys_paras.b_a_save_image_paras.c_save_max_time"

    #define cShowCenterLine "globaldef.b_ui.a_graphics.a_show_center_line"
    #define cShowCenterLineColor "globaldef.b_ui.a_graphics.b_show_center_line_color"
    #define cUsedMarkItemEnable "globaldef.b_ui.a_graphics.c_used_mark_item"
    #define cMarkItemLineColor "globaldef.b_ui.a_graphics.d_mark_item_line_color"
    #define cMarkItemPointColor "globaldef.b_ui.a_graphics.e_mark_item_point_color"
    #define cRoiItemLineColor "globaldef.b_ui.a_graphics.f_roi_line_color"

    #define cUsedGainRaw "globaldef.c_camera.d_gainRaw.d_used_gainRaw"

    // 像素联动/测量
    #define cMeasureColor "globaldef.b_ui.b_measure_paras.a_color"
    #define cMeasurePixSize "globaldef.b_ui.b_measure_paras.b_pixSize"
#pragma endregion

#pragma region "临时参数"
    #define cPluginDir "PluginDir"
#pragma endregion
