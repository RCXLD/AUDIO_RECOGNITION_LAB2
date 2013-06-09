/**
 * @file    iFly_REM.h
 * @brief   iFLY REM SDK header file
 *
 * This file contains the REM application programming interface(API) declarations
 * of iFLYTEK. REM developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    2004 - 2006 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author    Speech Dept. iFLYTEK.
 * @version   1.0
 * @date      2006/08/15
 *
 * @see       
 * 
 * @par History:
 * <table border=1>
 *  <tr> <index> <th>version  <th>date       <th>author  <th>notes             </tr>
 *  <tr> <0>     <td>1.0      <td>2006/08/15 <td>hjye    <td>Create this file  </tr>
 * </table>
 */


#ifndef __IFLY_REM_H__
#define __IFLY_REM_H__

#include "isr_rec.h"
#include "isr_ep.h"
#include "isr_ve.h"

#ifdef __cplusplus
extern "C" {
#endif

/* REM export symbols */
#if defined(WIN32)
# ifdef REM_EXPORTS
#  define REMAPI __declspec(dllexport)
# else
#  define REMAPI __declspec(dllimport)
# endif /* ifdef REM_EXPORTS */
#else    /* defined(WIN32) */
#  define REMAPI
#endif /* defined(WIN32) */

/*
 * Basic Data Types
 */

typedef void*               HREMHANDLE;

/*
 * REM common defines
 */
#define REM_DATESTR_LEN_MAX                 20
#define REM_ENGINENAME_LEN_MAX              64
#define REM_LANG_MAX                        16
#define REM_IP_MAXLEN                       32

/* engine find filters */
static const char * REM_ENGINE_FIND_FILTER[] =
{
	"Common.ServerIp",
	"Common.EngineType",
	"Common.Language",
	"Common.EngineName",
	"Common.Status",

	"Synth.Voice",
	"Synth.Gender",
	"Synth.Age",
	"Synth.SampleRate",

	"Recog.Mode",
	"Recog.Enrollment"
};

/* Instance status */
enum
{
	REM_INST_STATUS_UNKN,                       /* Unknown status */
	REM_INST_STATUS_CREATE,                     /* Connecting */
	REM_INST_STATUS_DESTROY,				    /* Disconnecting */
	REM_INST_STATUS_RECV,                       /* Receiving data */
	REM_INST_STATUS_SEND,                       /* Sending data */
	REM_INST_STATUS_RECOG,                      /* Synthesizing */
	REM_INST_STATUS_IDLE,                       /* Idle */
};

/* Engine status */
enum
{
    REM_ENGINE_STATUS_UNKN          = 0x0000,   /* Unknown state */
    REM_ENGINE_STATUS_REC_LOAD      = 0x0001,   /* Rec Engine loaded */
	REM_ENGINE_STATUS_EP_LOAD       = 0x0010,   /* Ep Engine loaded */
    REM_ENGINE_STATUS_REC_UNLOAD    = 0x0002,   /* Rec Engine unloaded */
	REM_ENGINE_STATUS_EP_UNLOAD     = 0x0020,   /* Ep Engine unloaded */
    REM_ENGINE_STATUS_DISABLE       = 0x8000,   /* Service disabled */
};

/* Information type of engine queried  */
enum
{
    /* Static engine informations */
    REM_QUERY_EINFO_NAME            = 1,        /* [Str] Query engine name */
    REM_QUERY_EINFO_VERSION         = 2,        /* [Str] Query engine version */
    REM_QUERY_EINFO_PRODUCER        = 3,        /* [Str] Query producer name of engine */
    REM_QUERY_EINFO_INSTPATH        = 4,        /* [Str] Query engine install path */
    REM_QUERY_EINFO_LICENSE_REC     = 5,        /* [unsigned long] Query rec license of engine */
	REM_QUERY_EINFO_LICENSE_EP      = 6,        /* [unsigned long] Query ep license of engine */
	REM_QUERY_EINFO_STDEFFICENCY    = 7,        /* [unsigned long] Query benchmark efficency of engine */
	REM_QUERY_EINFO_USERLIB         = 8,        /* [Str] Query user library engine supported */
    REM_QUERY_EINFO_IP              = 9,        /* [Str] Query engine's ip address */

    /* Dynamic engine informations */
    REM_QUERY_EINFO_STATUS          = 20,       /* [unsigned long] Query engine status */
    REM_QUERY_EINFO_TOTALSVC_REC    = 21,       /* [unsigned long] Query total rec service count since startup */
	REM_QUERY_EINFO_TOTALSVC_EP     = 22,       /* [unsigned long] Query total ep service count since startup */
	REM_QUERY_EINFO_TOTALSVC_VE     = 23,       /* [unsigned long] Query total ve service count since startup */
    REM_QUERY_EINFO_TOTALERR_REC    = 24,       /* [unsigned long] Query total rec error count since startup */
	REM_QUERY_EINFO_TOTALERR_EP     = 25,       /* [unsigned long] Query total ep error count since startup */
	REM_QUERY_EINFO_TOTALERR_VE     = 26,       /* [unsigned long] Query total ve error count since startup */
    REM_QUERY_EINFO_TOTALRECV_REC   = 27,       /* [double] Query total rec received bytes since startup */
	REM_QUERY_EINFO_TOTALRECV_EP    = 28,       /* [double] Query total ep received bytes since startup */
	REM_QUERY_EINFO_TOTALRECV_VE    = 29,       /* [double] Query total ve received bytes since startup */
    REM_QUERY_EINFO_TOTALSEND_REC   = 30,       /* [double] Query total rec sent bytes since startup */
	REM_QUERY_EINFO_TOTALSEND_EP    = 31,       /* [double] Query total ep sent bytes since startup */
	REM_QUERY_EINFO_TOTALSEND_VE    = 32,       /* [double] Query total ve sent bytes since startup */
    REM_QUERY_EINFO_ACTINSTINFO_REC = 33,       /* [TInstInfo Array] Query active rec instances of engine */
	REM_QUERY_EINFO_ACTINSTINFO_EP  = 34,       /* [TInstInfo Array] Query active ep instances of engine */
	REM_QUERY_EINFO_ACTINSTINFO_VE  = 35,       /* [TInstInfo Array] Query active ve instances of engine */
    REM_QUERY_EINFO_CURSVC_REC      = 36,       /* [unsigned long] Query total rec service count in last time interval (minute/hour/...) */
	REM_QUERY_EINFO_CURSVC_EP       = 37,       /* [unsigned long] Query total ep service count in last time interval (minute/hour/...) */
	REM_QUERY_EINFO_CURSVC_VE       = 38,       /* [unsigned long] Query total ve service count in last time interval (minute/hour/...) */
    REM_QUERY_EINFO_CURRECVRATE_REC = 39,       /* [unsigned long] Query current rec receive bytes (kb) per-second */
	REM_QUERY_EINFO_CURRECVRATE_EP  = 40,       /* [unsigned long] Query current ep receive bytes (kb) per-second */
	REM_QUERY_EINFO_CURRECVRATE_VE  = 41,       /* [unsigned long] Query current ve receive bytes (kb) per-second */
    REM_QUERY_EINFO_CURSENDRATE_REC = 42,       /* [unsigned long] Query current rec send bytes (kb) per-second */
	REM_QUERY_EINFO_CURSENDRATE_EP  = 43,       /* [unsigned long] Query current ep send bytes (kb) per-second */
	REM_QUERY_EINFO_CURSENDRATE_VE  = 44,       /* [unsigned long] Query current ve send bytes (kb) per-second */
    REM_QUERY_EINFO_CURERR_REC      = 45,       /* [unsigned long] Query total rec error count in last time interval (minute/hour/...) */
	REM_QUERY_EINFO_CURERR_EP       = 46,       /* [unsigned long] Query total ep error count in last time interval (minute/hour/...) */
	REM_QUERY_EINFO_CURERR_VE       = 47,       /* [unsigned long] Query total ve error count in last time interval (minute/hour/...) */
    REM_QUERY_EINFO_INSTCOUNT_REC   = 48,       /* [unsigned long] Query active rec instances count of engine */
	REM_QUERY_EINFO_INSTCOUNT_EP    = 49,       /* [unsigned long] Query active ep instances count of engine */
	REM_QUERY_EINFO_INSTCOUNT_VE    = 50,       /* [unsigned long] Query active ve instances count of engine */
	REM_QUERY_EINFO_TOTALEFF        = 51,       /* [unsigned long] Query total efficency of engine since startup */
	REM_QUERY_EINFO_ACTEFF          = 52,       /* [unsigned long] Query current active efficency of engine */

	/* Extended static engine informations */
	REM_QUERY_EINFO_LANGS           = 201,      /* [unsigned long Array] Query support languages */
	REM_QUERY_EINFO_SCORE           = 202,      /* [unsigned long] Query score of engine */
};

/* Engine control command */
enum
{
    REM_ENGINE_CTRL_REC_LOAD            = 1,    /* Load rec engine */
	REM_ENGINE_CTRL_EP_LOAD,                    /* Load ep engine */
    REM_ENGINE_CTRL_REC_UNLOAD,                 /* Unload rec engine */
	REM_ENGINE_CTRL_EP_UNLOAD,                  /* Unload ep engine */
    REM_ENGINE_CTRL_ENABLE,                     /* Enable engine */
    REM_ENGINE_CTRL_DISABLE,                    /* Disable engine */
};

/* Engine UNLOAD command flag */
enum
{
	REM_ECTRL_UNLOAD_FORCE          = 0x0001,   /* Unload regardless there have any active instance */
};

/*
* REM Data Structures
*/

#pragma pack(4)

/* Instance information structure */
typedef struct
{
	unsigned long        cbSize;                     /* [in] Size of structure */
	unsigned long        dwInstID;                   /* [out] Instance handle (Service ID) */
	unsigned long        dwErrorCode;                /* [out] Current error code of instance */
	unsigned long        dwErrorCount;               /* [out] Count of instance error occurred */
	unsigned long        dwInstStatus;               /* [out] Current instance status */
	unsigned long        dwClientIP;                 /* [out] Instance client address (host byte order) */
	char                 szStartTime[REM_DATESTR_LEN_MAX];   /* [out] Instance start time */
	double               dbRecvBytes;                /* [out] Instance total received bytes */
	double               dbSendBytes;                /* [out] Instance total sent bytes */
	unsigned long        dwEfficency;                /* [out] Instance current recognize efficency */
	unsigned long        dwReserved;                 /* [in/out] Reserved: must set to zero */
} RInstInfo, *PRInstInfo;

/* Filters mask used in FIND ENGINE operator */
enum
{
	REM_EFIND_FILTER_LANGUAGE       = 0x0001,
	REM_EFIND_FILTER_IPADDR         = 0x0002,
	REM_EFIND_FILTER_STATUS         = 0x0004,
	REM_EFIND_FILTER_ENGINENAME     = 0x0008,
};

/* Filters structure used in FIND ENGINE operator */
typedef struct
{
	unsigned long        cbSize;                     /* [in] Size of structure */
	unsigned long        dwFilterMask;               /* [in] Filter mask flag */
	char                 szIP[REM_IP_MAXLEN];        /* [in] Server IP address */
	unsigned long        dwLanguage;                 /* [in] Language ID */
	unsigned long        dwStatus;                   /* [in] Engine status */
	char                 szEngineName[REM_ENGINENAME_LEN_MAX];   /* [in] Engine name, wildcard '*' and '?' available */
	unsigned long        dwReserved;                 /* [in/out] Reserved: must set to zero */
} REngineFindFilter, *PREngineFindFilter;

#pragma pack()

/*==========================================================================
    Declaration of REM Interface Functions
 ==========================================================================*/

/** 
 * @brief   REMFindFirstEngine
 *
 * Find the first engine with specific filters
 *
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   const char * pFindFilter	   - [in] Pointer to the filters structure, or NULL if no filter is specified
 * @param   unsigned long *pdwEngineID     - [out] Pointer on a unsigned long that will receive the ID of the engine found
 * @param   HREMHANDLE *phFindHandle       - [out] Pointer on a search handle to be used in subsequent calls to REMFindNextEngine
 * @see     REMFindNextEngine, REMFindClose
 */
int REMAPI REMFindFirstEngine(PREngineFindFilter pFindFilter, unsigned long *pdwEngineID, HREMHANDLE *phFindHandle);
typedef int (*Proc_REMFindFirstEngine)(PREngineFindFilter pFindFilter, unsigned long *pdwEngineID, HREMHANDLE *phFindHandle);

/** 
 * @brief   REMFindNextEngine
 * 
 * Find the next engine with filters specified in the FindFirstEngine call
 *
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HREMHANDLE hFindHandle - [in] Handle obtained by a call to REMFindFirstEngine
 * @param   unsigned long *dwEngindID   - [out] Pointer on a unsigned long that will receive the ID of the engine found
 * @see     REMFindFirstEngine, REMFindClose
 */
int REMAPI REMFindNextEngine(HREMHANDLE hFindHandle, unsigned long *dwEngineID);
typedef int (*Proc_REMFindNextEngine)(HREMHANDLE hFindHandle, unsigned long *dwEngineID);

/** 
 * @brief    REMFindClose
 * 
 * This function closes the search handle used to find a engine or a voice
 * 
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HREMHANDLE hFindHandle - [in] Handle obtained by a call to REMFindFirstEngine or REMFindFirstVoice
 * @see     REMFindFirstEngine, REMFindFirstVoice
 */
int REMAPI REMFindClose(HREMHANDLE hFindHandle);
typedef int (*Proc_REMFindClose)(HREMHANDLE hFindHandle);

/** 
 * @brief   REMGetEngineInfo
 * 
 * Get specific information about an engine
 *
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   unsigned long dwEngineID - [in] The ID of the engine
 * @param   unsigned long dwInfoType - [in] The type of the information to be got
 * @param   PTTSVOID pBuffer    - [in/out] The buffer to return the information required
 * @param   unsigned long *pdwBufLen - [in/out] The buffer length/unit[in] or wrote bytes/unit[out], according to dwInfoType
 * @see     
 */
int REMAPI REMGetEngineInfo(unsigned long dwEngineID, unsigned long dwInfoType, void *pBuffer, unsigned long *pdwBufLen);
typedef int (*Proc_REMGetEngineInfo)(unsigned long dwEngineID, unsigned long dwInfoType, void *pBuffer, unsigned long *pdwBufLen);

/** 
 * @brief    REMEngineCtrl
 * 
 * Control a remote or local engine to do some work
 * 
 * @return    int - Return 0 in success, otherwise return error code.
 * @param     unsigned long dwEngineID - [in] The ID of the engine
 * @param     unsigned long dwCommand  - [in] The control command
 * @param     unsigned long wParam     - [in/out] The first parameter of the command
 * @param     unsigned long lParam     - [in/out] The second parameter of the command
 * @see       
 */
int REMAPI REMEngineCtrl(unsigned long dwEngineID, unsigned long dwCommand, unsigned long wParam, unsigned long lParam);
typedef int (*Proc_REMEngineCtrl)(unsigned long dwEngineID, unsigned long dwCommand, unsigned long wParam, unsigned long lParam);


#ifdef __cplusplus
}    /* extern "C" */
#endif

#endif /* __IFLY_REM_H__ */
