#ifndef __TGCLIENTSDK_H__
#define	__TGCLIENTSDK_H__

#include <stdio.h>
#include "stdint.h"

#ifdef WIN32
#ifdef TGCLIENTNETSDK_EXPORTS
#define TG_CLIENT_API extern "C"  __declspec(dllexport) 
#else 
#define TG_CLIENT_API extern "C"  __declspec(dllimport) 
#endif
#else
#define TG_CLIENT_API
#endif
  
  
//#define TG_FRAME_VIDEO_I           1
//#define TG_FRAME_VIDEO_P           2
//#define TG_FRAME_VIDEO_B           3
//#define TG_FRAME_AUDIO             4
  
 
typedef enum
{
	TG_FRAME_VIDEO_I =1,
	TG_FRAME_VIDEO_P =2,
	TG_FRAME_VIDEO_B =3,
	TG_FRAME_AUDIO =4,
}TG_NET_DEVICE_FRAMETYPE;  //dataType

typedef enum
{
	TG_DATA_TYPE_YUV420 = 0x01,
	TG_DATA_TYPE_RGB24,
	TG_DATA_TYPE_RGB565,
	TG_DATA_TYPE_H264 =0xA,

	TG_DATA_TYPE_PCM = 0x65,
	TG_DATA_TYPE_AAC =0x66,
	TG_DATA_TYPE_G711A =0x67,
}TG_DEVICE_DATATYPE;  //type

//video format
//#define TG_DATA_TYPE_YUV420        1
//#define TG_DATA_TYPE_RGB24         2
//#define TG_DATA_TYPE_H264          10
//  
////audio format
//#define TG_DATA_TYPE_PCM           101
//#define TG_DATA_TYPE_AAC           102
//#define TG_DATA_TYPE_G711A          103
struct TG_FRAME_INFO_S
{
	long dataType;
	long type;
	union
	{
		long num;//video
		long samples_per_sec;//audio
	};
	union
	{
		long width;//video
		long bits_per_sample;//audio
	};
	union
	{
		long height;//video
		long channels;//audio
	};
	long long pts;
};

enum TGLoginType
{
	TGLOGIN_Person = 0,
	TGLOGIN_Company = 1,
    TGLOGIN_ES = 2,
    TGLOGIN_TV = 3,
};

enum PTZCMD
{
	TILT_UP = 21,   //
	TILT_DOWN,		//
	PAN_LEFT,		//
	PAN_RIGHT,		//
	UP_LEFT,		//
	UP_RIGHT,		//
	DOWN_LEFT,		//
	DOWN_RIGHT,		//
	PAN_AUTO,		//
};

enum TGRatio
{
	TG_DRAW_ASPECT_RATION_DEFAULT = 0,  //default
	TG_DRAW_ASPECT_RATION_FULL,			//full
	TG_DRAW_ASPECT_RATION_4_3,			//4:3
	TG_DRAW_ASPECT_RATION_16_9			//16:9
};



typedef void(*MsgCallBack)(const char* jsonStr, void* context);
/*
{
"msgType":1,   //
"errCode":0,    //
"data":
	{
		"clientHandle":123,
		"nodeid":1234567,
		"camJson":
				{
					"esName":"tongguan",
					
				}
	}
}
*/
typedef void(*TGDecCallBack) (long hStream,char *buf,long size,TG_FRAME_INFO_S frameInfo,long reserve, void* context);
typedef void(*TGNotify) (long handle,char *key,long value,long context);

TG_CLIENT_API bool TGClient_Init();
TG_CLIENT_API bool TGClient_Destroy();

TG_CLIENT_API void TGClient_SetLogPrint(int logType);   //LOG_ERROR:1; LOG_WARNING:2; LOG_INFO:3; LOG_DEBUG:4; LOG_MAX_LEVEL:5

//loginType: 0-Person，1-Company
TG_CLIENT_API long TGClient_AsyncLogin(char* serverIP, unsigned short serverPort, const char* user, const char* pwd, int64_t nodeID, MsgCallBack cb, void *context, TGLoginType loginType = TGLOGIN_Person);

TG_CLIENT_API long TGClient_GetDeviceList(long loginHandle);

//clientType, queryVolLimited 由makefile macro define
TG_CLIENT_API long TGClient_Logout(long loginHandle);

//long GetOrgnTreeInfo(int currentTreeNode, int startIndex, int limited);
TG_CLIENT_API long TGClient_StartRealPlay(long loginHandle, void *displayHandle, int64_t llIpcId, int chnID, TGDecCallBack decCB, TGDecCallBack yuvCB,TGNotify cb, void* context);
TG_CLIENT_API long TGClient_StopRealPlay(long loginHandle, long realHandle);
TG_CLIENT_API long TGClient_StartDemoVideo(long loginHandle, char *pIp, short nPort, int64_t llIpcId, int16_t nChannel, int32_t hWnd, TGDecCallBack decCB, TGDecCallBack yuvCB,TGNotify cb, void* context);
TG_CLIENT_API long TGClient_StopDemoVideo(long loginHandle, long DemoVideHandle);

TG_CLIENT_API long TGClient_PlaySmooth( long loginHandle ,long realHandle,int bPlaySmooth);


TG_CLIENT_API long TGClient_StartVodPlay(long loginHandle, void * displayHandle, int64_t llIpcId, int chnID, const char* startTime, const char* endTime, int recordType, TGDecCallBack decCB,  TGDecCallBack yuvCB,TGNotify cb, void* context);
// time: "yyyy-MM-dd HH:mm:ss"
// recordType: 1 - AutoRecord, 2 - abstract video, 3 - AlarmRecord ...
TG_CLIENT_API long TGClient_StopVodPlay(long loginHandle, long vodHandle);

//nMinCacheMilliSec:   ，nMaxCacheMilliSec：
TG_CLIENT_API long TGClient_SetCacheSize(long loginHandle, long playHandle, long nMinCacheMilliSec, long nMaxCacheMilliSec);

//playType: 0-realstream，1-vodstream，2-cloudstream
TG_CLIENT_API long TGClient_Snapshot(long loginHandle, long playHandle, const char* fullFileName, int playType = 0);
TG_CLIENT_API long TGClient_RemoteSnap(long loginHandle, const char* user,int64_t nodeid, int nChnId=0);

TG_CLIENT_API long TGClient_StartRecord(long loginHandle, long playHandle, const char* fullFileName, int playType = 0); //
TG_CLIENT_API long TGClient_StopRecord(long loginHandle, long playHandle, int playType = 0);
//
TG_CLIENT_API long TGClient_GetTreeNodes(long loginHandle, int currNodeid, int startIndex, int limit);

TG_CLIENT_API long TGClient_GetCamerInfo(long loginHandle, int startIndex, int limit,char *js=NULL);

//nTurnOff: 1-Off, 0-On
TG_CLIENT_API long TGClient_SwitchCamer(long loginHandle, int nTurnOff, int64_t llIpcId);

TG_CLIENT_API long TGClient_GetTime(long loginHandle);

TG_CLIENT_API long TGClient_GetAlarmNotify(long loginHandle, int startIndex, int limit, char* startTime, char* endTime);
TG_CLIENT_API long TGClient_ConfirmAlarmNotify(long loginHandle, int msgid);
TG_CLIENT_API long TGClient_RealAlarmNotify(long loginHandle); ////

// return: >= 0 - status(0 , 1, 2), -1 - error 
TG_CLIENT_API long TGClient_GetAlarmStatus(long loginHandle, int msgid);

TG_CLIENT_API long TGClient_FeedBack(long loginHandle, char *szTime, char* szSoftVersion, char* szBackType, char* szContent);

TG_CLIENT_API long TGClient_SetOnlineNum(long loginHandle, int onlineNum);

TG_CLIENT_API long TGClient_StartPTZ(long loginHandle, int64_t llIpcId, int32_t nChnId, PTZCMD cmd);
TG_CLIENT_API long TGClient_StopPTZ(long loginHandle, int64_t llIpcId, int32_t nChnId, PTZCMD cmd);

TG_CLIENT_API long TGClient_StartTalkBack(long loginHandle, int64_t llIPCId, int nCamerId);
TG_CLIENT_API long TGClient_StopTalkBack(long loginHandle, int64_t llIPCId, int nCamerId);

//0-close， 1-open
TG_CLIENT_API long TGClient_SetCloud(long loginHandle, int64_t llIpcId, int32_t nChnId, int8_t cmd);

TG_CLIENT_API long TGClient_StartCloudVodFileStream(long loginHandle,  long long ipcId, unsigned long fileId, long hwnd, TGDecCallBack cbEnc, TGDecCallBack cbDec, TGNotify cb, long context);
TG_CLIENT_API long TGClient_StartCloudVodTimeStream(long loginHandle, long long ipcId, char *startTime, char *endTime, long hwnd, TGDecCallBack cbEnc, TGDecCallBack cbDec, TGNotify cb, long context);
TG_CLIENT_API long TGClient_StopCloudVodStream( long loginHandle, long vodHandle );

//type: 0， 1
TG_CLIENT_API long TGClient_GetVodPeriod(long loginHandle, int64_t llIpcId, int32_t nChnId, char* szBeginTime, char* szEndTime, int recordType, int type);

// defalut = -1
TG_CLIENT_API long TGClient_SeekVodStream(long loginHandle, long vodHandle, char *seekTime, long filePercent, long filePos, long fileTime);


TG_CLIENT_API long TGClient_SetDrawAspectRatio(long loginHandle, long hStream, TGRatio ratio);

TG_CLIENT_API long TGClient_OpenAudio(long loginHandle, long hStream);
TG_CLIENT_API long TGClient_CloseAudio(long loginHandle, long hStream);


// gpsinfolist
TG_CLIENT_API long TGClient_GetGpsList(long loginHandle, int64_t nodeid, int32_t nChnId, char *startTime, char *endTime);

//

#ifdef WIN32
TG_CLIENT_API long TGClient_SetVolume(long loginHandle, long hStream, long volume);
TG_CLIENT_API long TGClient_GetVolume(long loginHandle, long hStream);
TG_CLIENT_API long TGClient_SetDisPlayHandle(long loginHandle, long hStream,void *displayHandle);
#endif

//data:， len: ， frameType: ， frameNum: ， decType: ， pts:
TG_CLIENT_API long TGClient_SendRealStream(long handle,char *data,long len,long frameType,long frameNum,long decType,int64_t pts);

//type:
//	0：普通权限
//	1：管理员权限
TG_CLIENT_API long TGClient_AddUserDevice(long loginHandle, int64_t llIpcId, int32_t nChnId, char *serrialNum, char *deviceName,int type);
TG_CLIENT_API long TGClient_ModifyUserDevice(long loginHandle, int64_t llIpcId, int32_t nChnId, char *deviceName);
TG_CLIENT_API long TGClient_DelUserDevice(long loginHandle, int64_t llIpcId, int32_t nChnId);
TG_CLIENT_API long TGClient_DeviceCtrl(long loginHandle, int64_t llIpcId, int32_t nChnId, long cmd);

TG_CLIENT_API long TGClient_AddCapturePicTask(long loginHandle, int64_t llIpcId, int32_t nChnId, char *time, char *week, char *name);
TG_CLIENT_API long TGClient_ModifyCapturePicTask(long loginHandle, int32_t taskid, char *time, char *week, char *name);
TG_CLIENT_API long TGClient_DelCapturePicTask(long loginHandle, int32_t taskid);
TG_CLIENT_API long TGClient_GetCapturePicTaskList(long loginHandle, int64_t llIpcId, int32_t nChnId);
TG_CLIENT_API long TGClient_AddCapturePicFile(long loginHandle, int64_t llIpcId, int32_t nChnId, char *filename);
TG_CLIENT_API long TGClient_GetCapturePicFileList(long loginHandle, int64_t llIpcId, int32_t nChnId, char *starttime, char *endtime, long type);

TG_CLIENT_API long TGClient_StartMeeting(long loginHandle, char *username, long channel, unsigned long long fromuserid, long mode);//mode:0视频，1：音频
TG_CLIENT_API long TGClient_StopMeeting(long loginHandle, char *username, long channel, unsigned long long userid);
TG_CLIENT_API long TGClient_SendMeetingResponse(long loginHandle, char *username, long channel, unsigned long long userid, long status, long context);//status:0接听，1拒绝，2忙，3视频通话，4音频通话

/*
*  param：loginHandle 
*　　　　llIpcId －  device ID
*　　　　nChnId　－　camera ID
*/
TG_CLIENT_API long TGClient_CapturePic(long loginHandle, int64_t llIpcId, int32_t nChnId);



TG_CLIENT_API long TGClient_SendAlarmInfo(long loginhandle, uint64_t llIpcId, uint32_t nChnId, 
			const char *alarmtime, uint32_t alarmtype, const char *alarmcontext);


TG_CLIENT_API long  TGClient_GetOnlineStatus(long loginhandle);

/***********
 发送录像请求
*******************/
// recordType: 1 - AutoRecord, 2 - abstract video, 3 - AlarmRecord ...4 远程
TG_CLIENT_API long TGClient_SendVideoList(long loginhandle,int64_t llIpcId, int32_t nChnId, char *starttime, char *endtime, long type,long context);


//设置AS日志等级
TG_CLIENT_API long TGClient_SetLogLevel(long loginhandle,int64_t llIpcId,int32_t nChnId,int level); 

/****上传日志到日志服务器  
//level 日志等级 0 - 7 priority:unix facilities available 
//具体请查阅 
http://wenku.baidu.com/link?url=sD6jMvXrDP8e8xpBMtX_ZKe5GcpIKZPZsoIIBv6tuCYLhJKM2v23uViB3HjjIdpp5Icuz9q94GKEPYLwWX_ZDFS1ZRRfxrTnq6pYzclclGO&qq-pf-to=pcqq.c2c
*********/

TG_CLIENT_API long TGClient_UploadLog(long loginhandle,int level,char* context,int priority = 1);

#endif