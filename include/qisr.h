/**
 * @file    qisr.h
 * @brief   iFLY Speech Recognizer Header File
 * 
 *  This file contains the quick application programming interface (API) declarations 
 *  of ISR. Developer can include this file in your project to build applications.
 *  For more information, please read the developer guide.
 
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Co,LTD.
 *  and the licensee of this software.  Please refer to the license
 *  agreement for license use rights and restrictions.
 *
 *  Copyright (C)    1999 - 2007 by ANHUI USTC iFLYTEK, Co,LTD.
 *                   All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.0
 * @date    2008/12/12
 * 
 * @see        
 * 
 * History:
 * index    version        date            author        notes
 * 0        1.0            2008/12/12      Speech        Create this file
 */

#ifndef __QISR_H__
#define __QISR_H__

#include "isr_errors.h"
#include "isr_types.h"
#include "isr_ep.h"
#include "isr_rec.h"

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

/** 
 * @fn		QISRSessionBegin
 * @brief	Begin a Recognizer Session
 * 
 *  Create a recognizer session to recognize audio data
 * 
 * @return	return sessionID of current session, NULL is failed.
 * @param	const char* grammarList		- [in] grammars list, inline grammar support only one.
 * @param	const char* params			- [in] parameters when the session created.
 * @param	int *result					- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* ISRAPI QISRSessionBegin(const char* grammarList, const char* params, int *result);
typedef const char* (ISRAPI *Proc_QISRSessionBegin)(const char* grammarList, const char* params, int *result);
const wchar_t* ISRAPI QISRSessionBeginW(const wchar_t* grammarList, const wchar_t* params, int *result);
typedef const wchar_t* (ISRAPI *Proc_QISRSessionBeginW)(const wchar_t* grammarList, const wchar_t* params, int *result);

/** 
 * @fn		QISRGrammarActivate
 * @brief	Activate a grammar
 * 
 *  Activate a string or file grammar
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] the session id
 * @param	const char* grammar	- [in] grammar uri or string to activate
 * @param	const char* type	- [in] grammar type
 * @param	int weight	- [in] grammar weight
 * @see		
 */
int ISRAPI QISRGrammarActivate(const char* sessionID, const char* grammar, const char* type, int weight);
typedef int (ISRAPI *Proc_QISRGrammarActivate)(const char* sessionID, const char* grammar, const char* type, int weight);
int ISRAPI QISRGrammarActivateW(const wchar_t* sessionID, const wchar_t* grammar, const wchar_t* type, int weight);
typedef int (ISRAPI *Proc_QISRGrammarActivateW)(const wchar_t* sessionID, const wchar_t* grammar, const wchar_t* type, int weight);

/** 
 * @fn		QISRAudioWrite
 * @brief	Write Audio Data to Recognizer Session
 * 
 *  Writing binary audio data to recognizer.
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] The session id returned by recog_begin
 * @param	const void* waveData	- [in] Binary data of waveform
 * @param	unsigned int waveLen	- [in] Waveform data size in bytes
 * @param	int audioStatus			- [in] Audio status, can be 
 * @param	int *epStatus			- [out] ISRepState
 * @param	int *recogStatus		- [out] ISRrecRecognizerStatus, see isr_rec.h
 * @see		
 */
int ISRAPI QISRAudioWrite(const char* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *recogStatus);
typedef int (ISRAPI *Proc_QISRAudioWrite)(const char* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *recogStatus);
int ISRAPI QISRAudioWriteW(const wchar_t* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *recogStatus);
typedef int (ISRAPI *Proc_QISRAudioWriteW)(const wchar_t* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *recogStatus);

/** 
 * @fn		QISRGetResult
 * @brief	Get Recognize Result in Specified Format
 * 
 *  Get recognize result in Specified format.
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id returned by session begin
 * @param	const char** recResult	- [out] recognize result string in XML format
 * @see		
 */
const char * ISRAPI QISRGetResult(const char* sessionID);
typedef const char * (ISRAPI *Proc_QISRGetResult)(const char* sessionID);
const wchar_t* ISRAPI QISRGetResultW(const wchar_t* sessionID);
typedef const wchar_t* (ISRAPI *Proc_QISRGetResultW)(const wchar_t* sessionID);

/** 
 * @fn		QISRSessionEnd
 * @brief	End a Recognizer Session
 * 
 *  End the recognizer session, release all resource.
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id string to end
 * @param	const char* hints	- [in] user hints to end session, hints will be logged to CallLog
 * @see		
 */
int ISRAPI QISRSessionEnd(const char* sessionID, const char* hints);
typedef int (ISRAPI *Proc_QISRSessionEnd)(const char* sessionID, const char* hints);
int ISRAPI QISRSessionEndW(const wchar_t* sessionID, const wchar_t* hints);
typedef int (ISRAPI *Proc_QISRSessionEndW)(const wchar_t* sessionID, const wchar_t* hints);

/** 
 * @fn		QISRWaveformRecog
 * @brief	Recognize a Waveform File
 * 
 *  Recognize a waveform file and return XML result
 * 
 * @return	const char*	- Return rec result in string format, NULL if failed, result is error code.
 * @param	const char* waveFile	- [in] waveform file uri to recognize
 * @param	const char* waveFmt		- [in] waveform format string
 * @param	const char* grammarList	- [in] grammars used by recognizer
 * @param	const char* params		- [in] parameters
 * @param	int *recogStatus		- [out] recognize status
 * @param	int *result				- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* ISRAPI QISRWaveformRecog(const char* sessionID, const char* waveFile, const char* waveFmt, const char* grammarList, const char* params, int *recogStatus, int *result);
typedef const char* (ISRAPI *Proc_QISRWaveformRecog)(const char* sessionID, const char* waveFile, const char* waveFmt, const char* grammarList, const char* params, int *recogStatus, int *result);
const wchar_t* ISRAPI QISRWaveformRecogW(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* grammarList, const wchar_t* params, int *recogStatus, int *result);
typedef const wchar_t* (ISRAPI *Proc_QISRWaveformRecogW)(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* grammarList, const wchar_t* params, int *recogStatus, int *result);

/*
 * Initialize and fini, these functions is optional.
 * To call them in some cases necessarily.
 */

/** 
 * @fn		QISRInit
 * @brief	Initialize API
 * 
 *  Load API module with specified configurations.
 * 
 * @date	2009/11/26
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* configs	- [in] configurations to initialize
 * @see		
 */
int ISRAPI QISRInit(const char* configs);
typedef int (ISRAPI *Proc_QISRInit)(const char* configs);
int ISRAPI QISRInitW(const wchar_t* configs);
typedef int (ISRAPI *Proc_QISRInitW)(const wchar_t* configs);

/** 
 * @fn		QISRFini
 * @brief	Uninitialize API
 * 
 *  Unload API module, the last function to be called.
 * 
 * @author	jdyu
 * @date	2009/11/26
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @see		
 */
int ISRAPI QISRFini(void);
typedef int (ISRAPI *Proc_QISRFini)(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

#endif /* __QISR_H__ */
