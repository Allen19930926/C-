///
/// author: Feng Xiao (xiao.feng@horizon.ai)
/// date: 2019-10-23
///
/// Copyright (c) 2019 Horizon Robotics
///

#ifndef SDK_ADAS_SDK_ADAS_MSG_TYPES_H_
#define SDK_ADAS_SDK_ADAS_MSG_TYPES_H_

namespace HobotADAS {
  enum MsgTypes{
    SDK_MSG_TYPE_IMAGE = 0,
    SDK_MSG_TYPE_JPEG,
    SDK_MSG_TYPE_VEHICLE,
    SDK_MSG_TYPE_PEDESTRIAN,
    SDK_MSG_TYPE_LANE,
    SDK_MSG_TYPE_TRAFFIC_SIGN,
    SDK_MSG_TYPE_TRAFFIC_LIGHT,
    SDK_MSG_TYPE_SPEED_CAMERA,
    SDK_MSG_TYPE_WEATHER,
    SDK_MSG_TYPE_SCENE,
    SDK_MSG_TYPE_LIGHT,
    SDK_MSG_TYPE_TIME,
    SDK_MSG_TYPE_FREESPACE,
    SDK_MSG_TYPE_IHBC,
    SDK_MSG_TYPE_IMAGE_QUALITY,
    SDK_MSG_TYPE_FRAME,
    SDK_MSG_TYPE_DIAGNOSE,
    SDK_MSG_TYPE_ONLINECALIB,
    SDK_MSG_TYPE_FR_COC,
    SDK_MSG_TYPE_ASYN,
    SDK_MSG_TYPE_OBSTACLE,
    SDK_MSG_TYPE_USER_EVENT_IMAGE,
    SDK_MSG_TYPE_PARSINGBBOX,
    SDK_MSG_TYPE_TRAFFIC_SIGN_OBJECT,
    SDK_MSG_TYPE_TRAFFIC_LIGHT_OBJECT,
    SDK_MSG_TYPE_PARSINGBBOX_OBJECT,
    SDK_MSG_TYPE_COUNT
  };

}  // namespace HobotADAS

#endif  // SDK_ADAS_SDK_ADAS_MSG_CENTER_H_