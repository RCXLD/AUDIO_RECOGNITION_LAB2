/** 
 * @file	REM_Spec.h
 * @brief	iFLY REM SDK inner header file
 * 
 * This file contains the inner REM application programming interface(API) declarations
 * of iFLYTEK. Advanced REM developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    1999 - 2004 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version	1.0
 * @date	2008年10月13日
 * 
 * @see		
 * 
 * @par 版本记录：
 * <table border=1>
 *  <tr> <th>版本	<th>日期			<th>作者	<th>备注 </tr>
 *  <tr> <td>1.0	<td>2008年10月13日	<td>hjma	<td>创建 </tr>
 * </table>
 */
#ifndef __REM_SPEC_H__
#define __REM_SPEC_H__

#include "iFly_REM.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* REM common defines
*/

#define REM_ENGINEVER_LEN_MAX				32
#define REM_PRODUCERNAME_LEN_MAX            64
#define REM_INSTPATH_LEN_MAX                1024
#define REM_USERLIB_LEN_MAX                 256

/* Engine type (domain) */
enum
{
	REM_ENGINE_TYPE_UNKN,                   /* Unknown engine type */
	REM_ENGINE_TYPE_UNIVERSAL,              /* Universal recognizer system */
};

/* Engine status */
enum
{
	REM_ENGINE_STATUS_RUNNING,              /* Service running */
	REM_ENGINE_STATUS_BUSY,                 /* Service overloaded */
};

/* Information type of engine queried  */
/*
*   1 -  19 : base static informations
*  20 -  39 : base dynamic informations
*  40 - 100 : reserved
* 101 - 120 : extended static informations
* 121 - 140 : extended dynamic informations
* 141 - 199 : reserved
* 200 - 299 : extended static engine informations
* 300 - 399 : advanced informations
*/
enum
{
	/*   1 -  19 : base static informations */
	REM_QUERY_EINFO_STATIC          = 19,   /* [REngineInfoSta] Query all engine static info */

	/*  20 -  39 : base dynamic informations */
	REM_QUERY_EINFO_DYNAMIC         = 69,   /* [REngineInfoDyn] Query all engine dynamic info */

	/* 101 - 120 : extended static informations */
	REM_QUERY_EINFO_TYPE            = 101,  /* [unsigned long] Query engine type */
	REM_QUERY_EINFO_BUILDDATE       = 102,  /* [String] Query build date of engine */

	/* 121 - 140 : extended dynamic informations */
	REM_QUERY_EINFO_LIMITLICENSE    = 121,  /* [unsigned long] Query soft limited license of engine */
	REM_QUERY_EINFO_THROUGHPUT      = 122,  /* [unsigned long] Query engine current throughput */
	REM_QUERY_EINFO_STATREC         = 123,  /* [REngineStatRec Array] Query all saved statistic records of engine */

	/* 300 - 399 : advanced informations */
	REM_QUERY_EINFO_STATIC_STR      = 300,  /* [String] Query all engine static info */
	REM_QUERY_EINFO_DYNAMIC_STR     = 301,  /* [REngineInfoDyn] Query all engine dynamic info */
	REM_QUERY_EINFO_VOICE_STR       = 302,  /* [String] Query all voice info in string format */
};

/*
* REM Svc Type
*/
enum REM_SVC_TYPE
{
	REM_SVC_REC,                            /* rec svc */
	REM_SVC_EP,                             /* ep svc */
	REM_SVC_VE,                             /* ve svc */
	REM_SVC_CNT
};

/*
* REM Data Structures
*/

#pragma pack(4)

/* Engine static information structure */
typedef struct
{
	unsigned long    cbSize;                                /* 结构大小 */
	unsigned long    dwType;								/* 引擎类型 */
	unsigned long    dwStatus;                              /* 引擎状态 */
	char             szName[REM_ENGINENAME_LEN_MAX];        /* 引擎名称 */
	char             szVer[REM_ENGINEVER_LEN_MAX];          /* 引擎版本号 */
	char             szProducer[REM_PRODUCERNAME_LEN_MAX];  /* 生产商名称 */
	char             szInstPath[REM_INSTPATH_LEN_MAX];      /* 引擎安装路径 */
	char             szIP[REM_IP_MAXLEN];                   /* 所在服务器的IP地址 */
	unsigned long    dwPid;                                 /* 所在服务器的pid */
	unsigned long    dwLicense[REM_SVC_CNT];                /* 引擎硬件授权数 */
	unsigned long    dwStdEfficency;                        /* 引擎的基准效率 */
	char             szUserlib[REM_USERLIB_LEN_MAX];        /* 引擎支持的定制资源包数组(以;分隔) */
	unsigned long    dwScore;                               /* 引擎的分数 */
	unsigned long    dwLangCount;                           /* 引擎支持的语种数量 */
	unsigned long    dwLanguage[REM_LANG_MAX];              /* 引擎支持的语种ID数组 */
	unsigned long    dwReserved;                            /* 保留 */
} REngineInfoSta, *PREngineInfoSta;

/* Engine dynamic information structure */
typedef struct
{
	unsigned long    cbSize;                                /* 结构大小 */
	unsigned long    dwStatus;                              /* 引擎状态 */
	unsigned long    dwTotalSvc[REM_SVC_CNT];               /* 引擎自启动以来总服务实例数 */
	unsigned long    dwTotalErr[REM_SVC_CNT];               /* 引擎自启动以来总出错次数 */
	double           dbTotalRecv[REM_SVC_CNT];              /* 引擎自启动以来总接收字节数 */
	double           dbTotalSend[REM_SVC_CNT];              /* 引擎自启动以来总发送字节数 */
	unsigned long    dwTotalEff;                            /* 引擎总体效率 */
	unsigned long    dwActEff;                              /* 引擎当前效率 */
	unsigned long    dwCurSvc[REM_SVC_CNT];                 /* 引擎最近服务次数 */
	unsigned long    dwCurRecvRate[REM_SVC_CNT];            /* 引擎最近时间段接收字节速率 */
	unsigned long    dwCurSendRate[REM_SVC_CNT];            /* 引擎最近时间段发送字节速率 */
	unsigned long    dwCurErr[REM_SVC_CNT];                 /* 引擎最近出错次数 */
	unsigned long    dwInstCount[REM_SVC_CNT];              /* 引擎当前活动实例数量 */
	unsigned long    dwReserved;                            /* 保留 */
} REngineInfoDyn, *PREngineInfoDyn;

/* Engine statistic record structure */
typedef struct
{
	unsigned long    dwSvcCount[REM_SVC_CNT];               /* 单位时间内服务的次数 */
	unsigned long    dwRecvBytes[REM_SVC_CNT];              /* 单位时间内接收的数据量 */
	unsigned long    dwSendBytes[REM_SVC_CNT];              /* 单位时间内发送的数据量 */
	unsigned long    dwErrCount[REM_SVC_CNT];               /* 单位时间内出错的次数 */
} REngineStatRec, *PREngineStatRec;

#pragma pack()

#ifdef __cplusplus
	}
#endif

#endif // __REM_SPEC_H__
