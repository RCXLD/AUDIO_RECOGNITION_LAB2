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
 * @date	2008��10��13��
 * 
 * @see		
 * 
 * @par �汾��¼��
 * <table border=1>
 *  <tr> <th>�汾	<th>����			<th>����	<th>��ע </tr>
 *  <tr> <td>1.0	<td>2008��10��13��	<td>hjma	<td>���� </tr>
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
	unsigned long    cbSize;                                /* �ṹ��С */
	unsigned long    dwType;								/* �������� */
	unsigned long    dwStatus;                              /* ����״̬ */
	char             szName[REM_ENGINENAME_LEN_MAX];        /* �������� */
	char             szVer[REM_ENGINEVER_LEN_MAX];          /* ����汾�� */
	char             szProducer[REM_PRODUCERNAME_LEN_MAX];  /* ���������� */
	char             szInstPath[REM_INSTPATH_LEN_MAX];      /* ���氲װ·�� */
	char             szIP[REM_IP_MAXLEN];                   /* ���ڷ�������IP��ַ */
	unsigned long    dwPid;                                 /* ���ڷ�������pid */
	unsigned long    dwLicense[REM_SVC_CNT];                /* ����Ӳ����Ȩ�� */
	unsigned long    dwStdEfficency;                        /* ����Ļ�׼Ч�� */
	char             szUserlib[REM_USERLIB_LEN_MAX];        /* ����֧�ֵĶ�����Դ������(��;�ָ�) */
	unsigned long    dwScore;                               /* ����ķ��� */
	unsigned long    dwLangCount;                           /* ����֧�ֵ��������� */
	unsigned long    dwLanguage[REM_LANG_MAX];              /* ����֧�ֵ�����ID���� */
	unsigned long    dwReserved;                            /* ���� */
} REngineInfoSta, *PREngineInfoSta;

/* Engine dynamic information structure */
typedef struct
{
	unsigned long    cbSize;                                /* �ṹ��С */
	unsigned long    dwStatus;                              /* ����״̬ */
	unsigned long    dwTotalSvc[REM_SVC_CNT];               /* ���������������ܷ���ʵ���� */
	unsigned long    dwTotalErr[REM_SVC_CNT];               /* ���������������ܳ������ */
	double           dbTotalRecv[REM_SVC_CNT];              /* ���������������ܽ����ֽ��� */
	double           dbTotalSend[REM_SVC_CNT];              /* ���������������ܷ����ֽ��� */
	unsigned long    dwTotalEff;                            /* ��������Ч�� */
	unsigned long    dwActEff;                              /* ���浱ǰЧ�� */
	unsigned long    dwCurSvc[REM_SVC_CNT];                 /* �������������� */
	unsigned long    dwCurRecvRate[REM_SVC_CNT];            /* �������ʱ��ν����ֽ����� */
	unsigned long    dwCurSendRate[REM_SVC_CNT];            /* �������ʱ��η����ֽ����� */
	unsigned long    dwCurErr[REM_SVC_CNT];                 /* �������������� */
	unsigned long    dwInstCount[REM_SVC_CNT];              /* ���浱ǰ�ʵ������ */
	unsigned long    dwReserved;                            /* ���� */
} REngineInfoDyn, *PREngineInfoDyn;

/* Engine statistic record structure */
typedef struct
{
	unsigned long    dwSvcCount[REM_SVC_CNT];               /* ��λʱ���ڷ���Ĵ��� */
	unsigned long    dwRecvBytes[REM_SVC_CNT];              /* ��λʱ���ڽ��յ������� */
	unsigned long    dwSendBytes[REM_SVC_CNT];              /* ��λʱ���ڷ��͵������� */
	unsigned long    dwErrCount[REM_SVC_CNT];               /* ��λʱ���ڳ���Ĵ��� */
} REngineStatRec, *PREngineStatRec;

#pragma pack()

#ifdef __cplusplus
	}
#endif

#endif // __REM_SPEC_H__
