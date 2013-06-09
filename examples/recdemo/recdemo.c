/** 
 * @file	recdemo.c
 * @brief	C/C++ example code of iFLY Speech Recognizer
 * 
 *  Copyright(R) 1999-2007 by iFLYTEK. All rights reserved. 
 * 
 *  iFLYTEK Source Code Files
 *  Notice and Disclaimer of Liability
 *
 *  This source code software is provided to the user as an example of an
 *  application which utilizes iFLY Speech Recognizer Software to assist the user to
 *  develop its own application software. This source code has not been fully
 *  tested and may contain errors.  This software is not subject to any warranty
 *  or maintenance terms as may be contained in the Software License Agreement
 *  between ScanSoft and User.  iFLYTEK PROVIDES THIS SOFTWARE ON AN
 *  "AS IS' BASIS, AND DISCLAIMS ALL WARRANTIES, WHETHER EXPRESS OR IMPLIED,
 *  INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *  PURPOSE AND NON-INFRINGEMENT.
 *
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Inc.
 *  and the licensee of this software.  Please refer to the license agreement for
 *  license use rights and restrictions.
 * 
 * @author	Speech Dept.
 * @version	1.0
 * @date	2007-9-10
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-9-10	<td>Speech	<td>Create this file</tr>
 * </table>
 * 
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

//标准开发接口头文件
#include "../../include/isr_rec.h"
#include "../../include/isr_ep.h"

/**************************************************************************/
/* recognize test function */
int rec_test();
int rec_compute(ISR_REC_INST rec,
					ISR_REC_RESULT *resultData, int *finalStatus, int dtmf);

/* test -helper function */
void check_rc (char * func_name, int rc);
void check_rec_status (ISRrecRecognizerStatus status);
void check_parse_status (ISRrecParseStatus status);
void write_file(const char *name, void *buffer, int length);

int main(int argc, char ** argv)
	{
	int rc;
	setlocale(LC_ALL, ".ACP");
	
	/* Set the Baseline configuration file */
	printf ("\n Calling ISRrecInitialize.\n");
	rc = ISRrecInitialize(0,0);//L"..\\..\\..\\", 0);
	check_rc ("ISRrecInitialize", rc);
	
	printf ("\n Calling ISRepOpen.\n");
	if(ISRepOpen(0, 0) != ISR_SUCCESS)
		{
		printf("\n ISRepOpen failed\n");
		exit(1);
		}

	//测试代码
	rec_test();

	printf ("\n Calling ISRrecUninitialize.\n");
	rc = ISRrecUninitialize();
	check_rc ("ISRrecUninitialize", rc);
	
	return 0;
	}

/**************************************************************************/
/* Async audio input.
 *
 * We are using ulaw files here for the audio, but other types are supported.
 * You may use any of these types in your application.  Please see the
 * iFLY Speech Recognizer Reference Manual description for ISRrecAudioWrite()
 * for more details.
 */
#define VOICE_WEEKDAY			"..\\wav\\kdxf.wav"
#define MAX_AUDIO_BUF_LEN		2000   
#define MAX_EP_AUDIO_BUF_LEN	2000
#define NUM_AUDIO_BUFS			5

#define SESSION_NAME			L"recdemo"

typedef struct tagAudioThr audioThr;
int start_audio_write(ISR_REC_INST, audioThr **, const char *);
int finish_audio_write(audioThr *);

/**************************************************************************/
int rec_test()
	{
	/* loop counter */
	unsigned int num_rec;
	unsigned int max_rec = 3;
	
	/* Results */
	wchar_t **keyList = NULL;
	unsigned int numKeys = 0;
	unsigned int asSize =0;
	unsigned int numAnswers = 0;
	
	unsigned int grammarSize = 0;
	
	/* audio state */
	static char asBuffer[1000];
	
	/* session parameters */
	wchar_t *channelName = L"1";
	wchar_t *applicationName = SESSION_NAME;
	
	/* isrrec */
	int rc;
	ISR_REC_INST rec = NULL;
	ISRrecGrammarData dtmf_grammar = {L"uri", L"digits.abnf", NULL };
	//默认载入语法文件
	ISRrecGrammarData voice_grammar = {L"uri", L"C:\\yufa\\yufa.abnf", NULL };
	ISR_REC_RESULT resultData = NULL;
	int status = -1;
	
	const char *test_filename;
	/* audio thread data */
	audioThr *atd = NULL;
	
	/* waveform */
	unsigned char *waveBuf;
	unsigned int waveLen = 0;
	
	/* XML result string */
	wchar_t *myXML = NULL;
	
	/********/
	
	printf ("\n Calling ISRrecRecognizerCreate.\n");

	/******************************001*******************************/
	//experiment step1 识别器生成    ISRrecRecognizer接口调用，返回给rc
	rc = ISRrecRecognizerCreate(&rec, "serverAddr=192.168.60.177");

	check_rc ("ISRrecRecognizerCreate", rc);

	printf("\n Calling ISRrecRecognizerSetParameter to enable get waveform.\n");
	rc = ISRrecRecognizerSetParameter(rec, L"isr_rec_return_waveform", L"true");
	check_rc ("ISRrecRecognizerSetParameter", rc);
	
	printf ("\n Calling ISRrecSessionBegin.\n");
	/*******************************002******************************/
	//experiment step2 会话开始，ISRrecSessionBegin接口调用，返回给rc，通道参数为channelname变量
	
	rc = ISRrecSessionBegin(rec, channelName, NULL);

	check_rc ("ISRrecSessionBegin", rc);
	
	//printf ("\n Calling ISRrecGrammarLoad on DTMF grammar.\n");
	//rc = ISRrecGrammarLoad(rec, &dtmf_grammar);
	//check_rc ("ISRrecGrammarLoad", rc);
	
	/* Make digits active always */
	printf ("\n Calling ISRrecGrammarActivate on voice grammar.\n");

	/*****************************003************************************/
	//experiment step3 激活语法 ISRrecGrammarActivate接口调用 返回给rc
	
	check_rc ("ISRrecGrammarActivate", rc);

	printf ("\n Calling ISRrecGrammarActivate on digits grammar.\n");
	rc = ISRrecGrammarActivate(rec, &dtmf_grammar, 1, "dtmf_grammar");
	check_rc ("ISRrecGrammarActivate", rc);

	for (num_rec = 0; num_rec < 1; num_rec++)
		{
		if (num_rec == 0)
			{ // firstly, we test voice grammar
			printf ("\n Firstly: time grammar.\n");
			test_filename = VOICE_WEEKDAY;
			}
		else if (num_rec == 1)
			{ // secondly, we test DTMF grammar
			printf ("\n Secondly: DTMF grammar.\n");
			test_filename = VOICE_WEEKDAY; /* not needed since it's DTMF but this is realistic. */
			}
		
		/* This should always be called when re-initializing per-line rec */
		printf ("\n Calling ISRrecAcousticStateReset. \n");
		rc = ISRrecAcousticStateReset(rec);
		check_rc ("ISRrecAcousticStateReset", rc);

		
		if (!num_rec)
			{
			printf ("\n Calling ISRrecRecognizerStart.\n");
			/**********************************004**********************************/
			//experiment step4 启动识别器 ISRrecRecognizerStart接口调用 识别器为rec

			rc = ISRrecRecognizerStart(rec);
			
			check_rc ("ISRrecRecognizerStart", rc);

			printf ("\n Calling start_audio_write.\n");
			rc = start_audio_write(rec, &atd, test_filename);
			check_rc ("start_audio_write", rc);
			}
		
		printf ("\n Calling ISRrecRecognizerCompute.\n");
	    /**********************************************005******************************/
		//experiment step5 识别执行，计算步骤，rec_compute接口调用

		rc = rec_compute(rec, &resultData, &status, 0);
		
		check_rc ("ISRrecRecognizerCompute", rc);
		
		if (num_rec == 1)
			check_parse_status((ISRrecParseStatus)status);
		else
			{
			check_rec_status((ISRrecRecognizerStatus)status);
			
			/* GetWaveform is irrelevant for DTMF, so call it here... */
			printf ("\n Calling ISRrecGetWaveform. \n");
			rc = ISRrecGetWaveform(resultData, NULL, (void **)&waveBuf, &waveLen );
			check_rc ("ISRrecGetWaveform", rc);
			printf("\n Waveform Len = %d \n", waveLen);
			write_file("rec.pcm", waveBuf, waveLen);
			}

		if (!num_rec)
			{
			printf ("\n Calling ISRrecRecognizerStop.\n");
			rc = ISRrecRecognizerStop(rec, ISR_REC_STOP_DTMF);
			check_rc ("ISRrecRecognizerStop", rc);
			}

		printf ("\n Calling ISRrecGetXMLResult.\n");
		rc = ISRrecGetXMLResult(resultData, L"application/x-vnd.iflytek.emma+xml", &myXML);
		check_rc ("ISRrecGetXMLResult", rc);
		wprintf(L"\n XML result is = %s \n", myXML);
		
		if (!num_rec)
			{
			printf ("\n Calling finish_audio_write.\n");
			rc = finish_audio_write(atd);
			check_rc ("finish_audio_write", rc);
			}
		} /* END for loop */
  
	printf ("\n Calling ISRrecLogEvent.\n");
	rc = ISRrecLogEvent(rec, L"ISRendp", L"BRGN|BTIM|UCPU=0|SCPU=0");
	check_rc ("ISRrecLogEvent", rc);

	/* Proper code will always free grammars no longer needed.
	Please do this to ensure cache integrity/efficacy */

	printf ("\n Calling ISRrecGrammarFree on voice.\n");
	rc = ISRrecGrammarFree(rec, &voice_grammar);
	check_rc ("ISRrecGrammarFree", rc);

	//printf ("\n Calling ISRrecGrammarFree on dtmf.\n");
	//rc = ISRrecGrammarFree(rec, &dtmf_grammar);
	//check_rc ("ISRrecGrammarFree", rc);

	printf ("\n Calling ISRrecRecognizerDestroy.\n");
	/******************************************006************************/
	//识别器销毁  ISRrecRecognizerDestroy接口调用
	
	check_rc ("ISRrecRecognizerDestroy", rc);

	return 0;
	}


int rec_compute(ISR_REC_INST rec,
			ISR_REC_RESULT *resultData, int *finalStatus, int dtmf)
	{
	int rc;
	const int maxComputeTime = -1;
	
#ifndef WIN32
	extern int usleep(int);
#endif
	
	ISRrecResultType type;
	ISRrecRecognizerStatus status;
	
	/* ISRrecRecognizerCompute() must be called a minimum of
	* THREE times.
	*/
	do {
		if (dtmf) /* Heard DTMF! */
			break;

		type = ISR_REC_RESULT_TYPE_COMPLETE;
		status = ISR_REC_STATUS_INCOMPLETE;
		*finalStatus = ISR_REC_STATUS_NO_MATCH;
		rc = ISRrecRecognizerCompute(rec, maxComputeTime, &status, &type,
			resultData);
		if (rc != ISR_SUCCESS)
			return rc;
		
		printf ("\n Compute Status = %d", status);
		printf ("\n Compute Type = %d\n", type);
		
		/******
		 * If you do not expect to receive one of the termination
		 * statuses below, you should call ISRrecRecognizerStop() to
		 * halt the recognizer.  Otherwise, any one of the statuses
		 * below will also halt the recognizer.  Do *not* call
		 * ISRrecRecognizerStop() if the recognizer has halted,
		 * or it will return ISR_REC_ERROR_INACTIVE.
		 *
		 * The code below illustrates using this function
		 * properly.  When "num_rec" is 2, we will use this as
	 	 * a simulated DTMF entry by the caller.
		 ******/
		

	} while ((status != ISR_REC_STATUS_SUCCESS) &&
		(status != ISR_REC_STATUS_NO_MATCH) &&
		(status != ISR_REC_STATUS_STOPPED) &&
		(status != ISR_REC_STATUS_MAX_CPU_TIME) &&
		(status != ISR_REC_STATUS_MAX_SPEECH)
		); /* These are recognizer termination statuses */
	
	if (dtmf)
		{
		ISRrecParseStatus dtmf_status;
		
		/* Call GetDTMF results.  Pretend that DTMF of
		   0551-5331800 was entered.
		 */
		printf ("\n Calling ISRrecParseDTMFResults.\n");
		rc = ISRrecParseDTMFResults(rec, "05515331800", &dtmf_status, resultData);
		check_rc ("ISRrecParseDTMFResults", rc);
		
		*finalStatus = dtmf_status;
		}
	else
		*finalStatus = status;
	
	return rc;
	}

void write_file(const char *name, void *buffer, int length)
	{
	FILE *fp = fopen(name, "wb");
	if (fp == NULL)
		printf("\nERROR. Cannot open output file %s\n", name);
	fwrite(buffer, 1, length, fp);
	fclose(fp);
	}

void check_rc (char * func_name, int rc)
{
	switch (rc)
		{
		case ISR_SUCCESS:
			printf("\n Function %s returned SUCCESS.\n", func_name);
			break;
		default:
			printf ("\n Function %s returned %d. \n", func_name, rc);
		}
	
	if (rc != ISR_SUCCESS)
		{
		printf ("\n An Error Was Returned.  Exit Test Application. \n");
		getchar();
		exit(1);
		}
	}

void check_rec_status (ISRrecRecognizerStatus status)
	{
	switch (status)
		{
		case ISR_SUCCESS:
			printf("\n Recognizer status returned was SUCCESS.\n");
			break;
		case ISR_REC_STATUS_NO_MATCH:
			printf("\n Recognizer status returned was NO_MATCH.\n");
			break;
		case ISR_REC_STATUS_INCOMPLETE:
			printf("\n Recognizer status returned was INCOMPLETE.\n");
			break;
		case ISR_REC_STATUS_NON_SPEECH_DETECTED:
			printf("\n Recognizer status returned was NON_SPEECH_DETECTED.\n");
			break;
		case ISR_REC_STATUS_SPEECH_DETECTED:
			printf("\n Recognizer status returned was SPEECH_DETECTED.\n");
			break;
		case ISR_REC_STATUS_SPEECH_COMPLETE:
			printf("\n Recognizer status returned was SPEECH_COMPLETE.\n");
			break;
		case ISR_REC_STATUS_MAX_CPU_TIME:
			printf("\n Recognizer status returned was MAX_CPU_TIME.\n");
			break;
		case ISR_REC_STATUS_MAX_SPEECH:
			printf("\n Recognizer status returned was MAX_SPEECH.\n");
			break;
		case ISR_REC_STATUS_STOPPED:
			printf("\n Recognizer status returned was STOPPED.\n");
			break;
		default: /* should never get here */
			printf ("\n Internal error: invalid status from Compute()!\n");
			break;
		}
	return;
	}

void check_parse_status (ISRrecParseStatus status)
	{
	switch (status)
		{
		case ISR_REC_PARSE_COMPLETE:
			printf("\n Recognizer status returned was COMPLETE.\n");
			break;
		case ISR_REC_PARSE_VALID_PREFIX:
			printf("\n Recognizer status returned was VALID PREFIX.\n");
			break;
		case ISR_REC_PARSE_INCOMPLETE:
			printf("\n Recognizer status returned was INCOMPLETE.\n");
			break;
		case ISR_REC_PARSE_INVALID:
			printf("\n Recognizer status returned was INVALID.\n");
			break;
		default: /* should never get here */
			printf ("\n Internal error: invalid status from ParseDTMF()!\n");
			break;
		}
	return;
	}


#ifdef WIN32 /* Windows NT */
#include <windows.h>
#include <process.h>

#define thr_ret_type unsigned int

typedef unsigned int (__stdcall *ThreadFunc)(void* userData);

#define usleep(x) Sleep(x / 1000)
#define DEFINE_THREAD_FUNC(funcName, userData) \
unsigned int (__stdcall funcName)(void *userData)
#define Thread HANDLE

int ThreadCreate(Thread* thread, ThreadFunc start_function, void* arg)
	{
	*thread = (Thread)_beginthreadex(NULL, 0, start_function, arg, 0, NULL);
	return 0;
	}
	
int ThreadJoin(Thread* thread, void* status)
	{
	int rc;

	switch (WaitForSingleObject(*thread, INFINITE))
		{
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
			rc = 0;
			break;
		case WAIT_TIMEOUT:
		default:
			rc = 1;
			break;
		}
	CloseHandle(*thread);
	return rc;
	}

#else /* Linux/Solaris */

#include <pthread.h>

#define thr_ret_type void*

#define DEFINE_THREAD_FUNC(funcName, userData) void* funcName(void* userData)

#define Thread pthread_t

typedef void* (*ThreadFunc)(void* userData);

int ThreadCreate(Thread* thread, ThreadFunc start_function, void* arg)
{
	pthread_attr_t thread_attr;
	int rc;
	if ((!thread) || (!start_function))
		return 1;
	/* construct thread attribute */
	rc = pthread_attr_init(&thread_attr);
	if (rc != 0) {
		return 1;
	}
	/* configure thread attribute */
	rc = pthread_create(thread, &thread_attr, start_function, arg);
	if (rc != 0) {
		pthread_attr_destroy(&thread_attr);
		return 1;
	}
	pthread_attr_destroy(&thread_attr);
	return 0;
}

int ThreadJoin(Thread* thread, void* status)
{
	int rc;
	if (! status)
		return 1;
	rc = pthread_join(*thread, &status);
	if (rc != 0) {
		return 1;
	}
	
	return 0;
}
#endif /* NT/Unix differences */

/**************************************************************************/
/*** Async audio ****/
struct tagAudioThr
	{
	Thread				thr;
	ISR_REC_INST		rec;
	const char *		filename;
	int					ret;
	};

DEFINE_THREAD_FUNC(audio_write, rec);

/* doc that thread create/destroy for recognition is optional */

int start_audio_write(ISR_REC_INST rec, audioThr ** atdpp,
								   const char *filename)
	{
	audioThr *atd = calloc(sizeof(audioThr), 1);
	atd->rec = rec;
	atd->filename = filename;
	ThreadCreate(&(atd->thr), audio_write, atd);
	*atdpp = atd;
	return ISR_SUCCESS;
	}

int finish_audio_write(audioThr *atd)
	{
	ThreadJoin(&(atd->thr), &atd);
	free(atd);
	return ISR_SUCCESS;
	}

/*	return 1 on any failure
	return 0 on success
*/
DEFINE_THREAD_FUNC(audio_write, arg)
	{
	audioThr *atd = (audioThr *)arg;
	int rc;
	FILE *fp;
	unsigned char buf[MAX_AUDIO_BUF_LEN];
	int len;
	ISRAudioSamples *samplesStruct;
	ISRAudioSamples epSamplesStruct;
	ISR_EP_INST isrep_handle;
	int eprc;
	ISRepState isrep_state;
	int begin, end;
	int num_samples_read = 0;
	int before_speech = 1;

	printf("\nAUDIO THREAD=> ENTER.\n");
	samplesStruct = (void *) malloc(sizeof(ISRAudioSamples));
	samplesStruct->samples = (void *) malloc(sizeof(unsigned char)*MAX_EP_AUDIO_BUF_LEN);
	samplesStruct->type = L"audio/basic";
	
	if(ISRepDetectorCreate(&isrep_handle,0) != ISR_SUCCESS)
		{
		printf("AUDIO THREAD=> ERROR. Cannot create speech detector\n");
		atd->ret = ISR_ERROR_GENERAL;
		return (thr_ret_type)1;
		}
	if(ISRepSessionBegin(isrep_handle, SESSION_NAME, NULL) != ISR_SUCCESS)
		{
		printf("AUDIO THREAD=> ERROR. Cannot start session on EP detector\n");
		atd->ret = ISR_ERROR_GENERAL;
		return (thr_ret_type)1;
		}
	ISRepStateReset(isrep_handle);
	
	if(ISRepStart(isrep_handle) != ISR_SUCCESS)
		{
		printf("AUDIO THREAD=> ERROR. Cannot start endpointer\n");
		atd->ret = ISR_ERROR_GENERAL;
		ISRepDetectorDestroy(isrep_handle);
		return (thr_ret_type)1;
		}
	/* otherwise, act as if prompt is done */
	if(ISRepPromptDone(isrep_handle) != ISR_SUCCESS)
		{ /* no prompt.. */
		printf("AUDIO THREAD=> ERROR. Cannot set prompt done for endpointer\n");
		atd->ret = ISR_ERROR_GENERAL;
		ISRepDetectorDestroy(isrep_handle);
		return (thr_ret_type)1;
		}

	/* Read in Audio File */
	fp = fopen(atd->filename, "rb");
	if (fp == NULL)
		{
		printf("AUDIO THREAD=> ERROR. Cannot open prompt file %s\n", atd->filename);
		atd->ret = ISR_ERROR_GENERAL;
		ISRepDetectorDestroy(isrep_handle);
		return (thr_ret_type)1;
		}
	
	/* loop while there are still buffers to be picked up from file */
	while((len = fread(buf, 1, MAX_AUDIO_BUF_LEN, fp)) > 0)
		{
		/* call ep even after begin so it can adapt to audio */
		epSamplesStruct.samples = (void *) buf;
		epSamplesStruct.len     = len;
		epSamplesStruct.type    = L"audio/L16;rate=8000";
		epSamplesStruct.status  = (0 == num_samples_read) ? ISR_AUDIO_SAMPLE_FIRST : ISR_AUDIO_SAMPLE_CONTINUE;
		
		num_samples_read += len;
		printf("AUDIO THREAD=> Read %d samples; total now %d\n", len, num_samples_read);
		
		/* begin and end values passed back from epWrite means nothing in epRead mode */
		if((eprc = ISRepWrite(isrep_handle,  &epSamplesStruct, &isrep_state, &begin, &end)) != ISR_SUCCESS)
		{
				printf("AUDIO THREAD=> ERROR. ISRepWrite returned %d\n", eprc);
				atd->ret = ISR_ERROR_GENERAL;
				ISRepDetectorDestroy(isrep_handle);
				return (thr_ret_type)1;
		}
		if(isrep_state == ISR_EP_AFTER_SPEECH)
		{
			break;
		}
		if(isrep_state == ISR_EP_TIMEOUT)
			{
			printf("AUDIO THREAD=> ALERT: beginning of speech not found before BOS_timeout\n");
			atd->ret = ISR_ERROR_GENERAL;
			ISRepDetectorDestroy(isrep_handle);
			return (thr_ret_type)1;
			} 
		else if(isrep_state == ISR_EP_ERROR)
			{
			printf("AUDIO THREAD=> ERROR: isrep audio error\n");
			atd->ret = ISR_ERROR_GENERAL;
			ISRepDetectorDestroy(isrep_handle);
			return (thr_ret_type)1;
			}
		else
			{
			/* impl ep_read */
			
			printf ("AUDIO THREAD=>  Calling ISRepRead.\n");
			eprc =  ISRepRead(isrep_handle,samplesStruct,&isrep_state,MAX_EP_AUDIO_BUF_LEN);
			if(isrep_state == ISR_EP_IN_SPEECH)
				{
				printf("epstate = IN SPEECH\n");
				}
			if(samplesStruct->status == ISR_AUDIO_SAMPLE_FIRST)
				{
				printf("AUDIO THREAD=> Begin of speech found at sample %d\n", begin);
				}
			if (samplesStruct->status == ISR_AUDIO_SAMPLE_NEW_CHUNK)
				{
				printf("AUDIO THREAD=> new chunk found. \n");
				}
			if(eprc == ISR_SUCCESS &&
				(samplesStruct->len != 0 || samplesStruct->status != 0) )
				{
				printf ("AUDIO THREAD=>  Calling ISRrecAudioWrite.\n");
				before_speech = 0;
				rc = ISRrecAudioWrite(atd->rec, samplesStruct);
				}
			}
		}
	fclose(fp);
	
	/* have to write the end packet for file */
	if(!before_speech)
		{
		/* flush out the epRead buffer */
		while(ISRepRead(isrep_handle,samplesStruct,&isrep_state,MAX_EP_AUDIO_BUF_LEN) == ISR_SUCCESS)
			{
			if(samplesStruct->len != 0 || samplesStruct->status != 0)
				{
				printf ("AUDIO THREAD=>  Calling ISRrecAudioWrite.\n");
				rc = ISRrecAudioWrite(atd->rec, samplesStruct);
				}
			else
				break;
			}
		printf ("AUDIO THREAD=>  Calling ISRrecAudioWrite to indicate end.\n");
		((unsigned char *)samplesStruct->samples)[0] = 0;
		samplesStruct->len = 1;
		samplesStruct->type = L"audio/basic";
		samplesStruct->status = ISR_AUDIO_SAMPLE_LAST;
		printf ("AUDIO THREAD=>  Calling ISRrecAudioWrite.\n");
		rc = ISRrecAudioWrite(atd->rec, samplesStruct);
		atd->ret = rc;
		}
	if(ISRepStop(isrep_handle, ISR_EP_STOP_SPEECH, 0) != ISR_SUCCESS)
		{
		printf("AUDIO THREAD=> ERROR. Cannot stop endpointer\n");
		atd->ret = ISR_ERROR_GENERAL;
		}
	
	/* destroy normally happens when the channel is done, not once per utterance */
	ISRepDetectorDestroy(isrep_handle);
	
	/* SAMPLE_LAST won't stop the recognizer automatically
	* if we never detected BOS.  This would be true for all-noise
	* utterances, for example.  So, we manually Stop(); otherwise,
	* Compute(-1) will appear to hang waiting for its data.
	*/
	if (before_speech)
		{
		printf("AUDIO THREAD=> ERROR: BOS never found\n");
		atd->ret = ISR_ERROR_GENERAL;
		ISRrecRecognizerStop(atd->rec, ISR_REC_STOP_OTHER);
		}
	/* free struct */
	free(samplesStruct->samples);
	free(samplesStruct);
	return (thr_ret_type)0;
	}
