/** 
* @file	sapidemo.cpp
* @brief	C/C++ example code of iFLY Speech Recognizer
* 
*  Copyright(R) 1999-2009 by iFLYTEK. All rights reserved. 
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
* @date	2009-6-29
* 
* @see		
* 
* <b>History:</b><br>
* <table>
*  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
*  <tr> <td>1.0		<td>2009-6-29	<td>Speech	<td>Create this file</tr>
* </table>
* 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#include "isr_api.h"

#include <assert.h>

#include <windows.h>
#include <process.h>


int read_wave(const char* wave_file,unsigned char*& wave_data,int& len);
int rec_compute(ISR_INST svc,ISRrecRecognizerStatus *status, ISRrecResultType *type, ISR_REC_RESULT *resultData);

//the audio write thread's arguments
typedef struct tagAudioThr audioThr;
int start_audio_write(ISR_INST rec, audioThr ** atdpp,unsigned char* audio,int len,const wchar_t* type);
int finish_audio_write(audioThr *atd);

//the compute thread's arguments
typedef struct tagComputeThr computeThr;
int start_compute(ISR_INST rec, computeThr ** ctdpp, ISR_REC_RESULT* result_data);
int finish_compute(computeThr* ctd);

void check_rc (char * func_name, int rc);

int main(int argc, char ** argv)
{
	setlocale(LC_ALL, ".ACP");

	//in this example,assume that the all wave is 8k 16bit pcm
	const char* audio_file		= "..\\data\\test.wav";
	const wchar_t* audio_format	= L"audio/L16;rate=8000";

	const wchar_t* grammar_uri	= L"builtin:grammar/weekday.abnf?language=zh-cn";

	int ret = ISR_SUCCESS;

	//init the InterReco engine
	const wchar_t * engine = 0;//L"..\\..\\..\\";
	ret = ISRInitialize(engine,0);
	check_rc("ISRInitialize",ret);


	//read audio from wave file
	unsigned char * audio = NULL;
	int audio_len = 0;
	ret = read_wave(audio_file,audio,audio_len);
	check_rc("read_wave",ret);

	//create a recognition instance
	ISR_INST svc;
	ret = ISRRecognizerCreate(&svc,L"channel_test",NULL);
	check_rc("ISRRecognizerCreate",ret);

	//active an uri grammar
	ISRrecGrammarData voice_grammar = {L"uri", grammar_uri , NULL };
	ret = ISRGrammarActivate(svc,&voice_grammar,1,"grammar_test");
	check_rc("ISRGrammarActivate",ret);


	//start the recognition
	ret = ISRRecognizerStart(svc);
	check_rc("ISRRecognizerStart",ret);

	//In general, a recognition service contains two threads. one is record thread which calls ISRAudioWrite,
	//and another is compute thread,which calls ISRRecognizerCompute ,ISRGetXMLResult ,etc.


	
	//simulate a record thread. Receive audio data from audio source, and write it to the recognizer
	audioThr*	aarg = NULL;
	start_audio_write(svc,&aarg,audio,audio_len,audio_format);

	//create a compute thread which try to get the recognition result!
	ISR_REC_RESULT	result_data;	//the result of recognizer
	computeThr*	carg = NULL;
	ret = start_compute(svc,&carg,&result_data);
	check_rc("start_compute",ret);


	//wait until compute thread end 
	printf ("\n Calling finish_compute.\n");
	ret = finish_compute(carg);
	check_rc ("finish_compute", ret);


	/* XML result string */
	const wchar_t *myXML = NULL;
	printf ("\n Calling ISRGetXMLResult.\n");
	ret = ISRGetXMLResult(result_data, L"application/x-vnd.iflytek.emma+xml", &myXML);
	check_rc ("ISRGetXMLResult", ret);
	wprintf(L"\n XML result is = %s \n", myXML);

	printf ("\n Calling finish_audio_write.\n");
	ret = finish_audio_write(aarg);
	check_rc ("finish_audio_write", ret);

	
	printf ("\n Calling ISRRecognizerStop.\n");
	ret = ISRRecognizerStop(svc, ISR_REC_STOP_SPEECH);
	check_rc ("ISRRecognizerStop", ret);

	printf ("\n Calling ISRrecGrammarFree.\n");
	ret = ISRGrammarDeactivate(svc, &voice_grammar);
	check_rc ("ISRGrammarDeactivate", ret);

	printf ("\n Calling ISRrecRecognizerDestroy.\n");
	ret = ISRRecognizerDestroy(svc);
	check_rc ("ISRRecognizerDestroy", ret);

	//free the memory
	delete [] audio;
	audio = NULL;

	printf ("\n Calling ISRUninitialize.\n");
	ret = ISRUninitialize();
	check_rc ("ISRUninitialize", ret);

	return 0;
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

//audio write thrad
struct tagAudioThr
{
	unsigned char * audio;		// the audio buffer
	int				len;		// the len of audio
	const wchar_t*	type;		// the type of audio

	ISR_INST		svc;		//the instance of recognizer
	Thread			thr;
};

DEFINE_THREAD_FUNC(audio_write_thread, _arg)
{
	audioThr* arg = (audioThr*)_arg;
	ISRAudioSamples as;
	
	as.type		= arg->type;
	
	ISRepState	ep_state;
	int	begin_sample = 0;
	int end_sample	 = 0;

	const int step = 800;

	int ret = ISR_SUCCESS;
	int i = 0 ;

	ISRepState last_state = ISR_EP_LOOKING_FOR_SPEECH;

	for(; i < arg->len/step; i++)
	{
		as.len		= step;
		as.samples	= arg->audio + i * step;
		as.status	= (0 == i) ? ISR_AUDIO_SAMPLE_FIRST : ISR_AUDIO_SAMPLE_CONTINUE;
		ret = ISRAudioWrite(arg->svc,&as,&ep_state,&begin_sample,&end_sample);

		assert(ret == ISR_SUCCESS);
		if(ret != ISR_SUCCESS)
		{
			printf("\n ISRAudioWrite return an error = %d",ret);
			return ret;
		}
		
		if(last_state == ISR_EP_LOOKING_FOR_SPEECH && ep_state == ISR_EP_IN_SPEECH)
		{
			printf("\n Speech Detected");
		}
		if(ep_state == ISR_EP_AFTER_SPEECH)
		{
			return 0;
		}
		else if(ep_state == ISR_EP_ERROR)
		{
			printf("\n ISRAudioWrite return an error = %d",ISR_EP_ERROR);
			return -1;
		}
		else if((ep_state == ISR_EP_TIMEOUT)|(ep_state == ISR_EP_MAX_SPEECH))
		{
			printf("\n ISRAudioWrite return an error = %d",ep_state);
			return 1;
		}
		last_state = ep_state;
	}

	//write the remain data. Because audio source is a file,  the type of remain audio must be set to ISR_AUDIO_SAMPLE_LAST
	int len = arg->len - step * i;
	
	if(len != 0)
	{
		as.samples	= arg->audio + step*i;
		as.len		= len;
		as.status	= ISR_AUDIO_SAMPLE_LAST;
		ret = ISRAudioWrite(arg->svc,&as,&ep_state,&begin_sample,&end_sample);
	}
	else
	{
		as.len = 2;
		as.status = ISR_AUDIO_SAMPLE_LAST;
		ret = ISRAudioWrite(arg->svc,&as,&ep_state,&begin_sample,&end_sample);
	}

	assert(ret == ISR_SUCCESS);
	if(ret != ISR_SUCCESS)
	{
		printf("\n ISRAudioWrite return an error = %d",ret);
		return ret;
	}

	if(last_state == ISR_EP_LOOKING_FOR_SPEECH && ep_state == ISR_EP_IN_SPEECH)
	{
		printf("Speech Detected");
	}
	if(ep_state == ISR_EP_AFTER_SPEECH)
	{
		return 0;
	}
	else if(ep_state == ISR_EP_ERROR)
	{
		printf("\n ISRAudioWrite return an error = %d",ISR_EP_ERROR);
		return -1;
	}
	else if((ep_state == ISR_EP_TIMEOUT)|(ep_state == ISR_EP_MAX_SPEECH))
	{
		printf("\n ISRAudioWrite return an error = %d",ep_state);
		return 1;
	}
	return 0;
}

int read_wave(const char* wave_file,unsigned char*& wave_data,int& len)
{
	FILE* fp = fopen(wave_file, "rb");
	assert(fp);
	if(!fp)
		return ISR_ERROR_FILE_NOT_FOUND;
	fseek(fp,0,SEEK_END);
	len = ftell(fp);


	const int wave_head_size = 44;
	len -= wave_head_size;
	if(len <= 0 )
	{
		fclose(fp);
		return ISR_ERROR_INVALID_DATA;
	}

	wave_data = new unsigned char[len];
	if(!wave_data)
		return ISR_ERROR_OUT_OF_MEMORY;
	fseek(fp,wave_head_size,SEEK_SET);
	fread(wave_data,sizeof(char),len,fp);
	fclose(fp);

	return ISR_SUCCESS;
}

//compute thread
struct tagComputeThr
{
	ISR_REC_RESULT*	result_data;	//the result of recognizer
	ISR_INST			svc;			//the instance of recognizer
	Thread				thr;
};

DEFINE_THREAD_FUNC(compute_thread, _arg)
{
	computeThr* arg = (computeThr*)_arg;
	//try to get the recognition result
	ISRrecRecognizerStatus	rec_status;
	ISRrecResultType			result_type;

	return rec_compute(arg->svc, &rec_status, &result_type, arg->result_data);
}
int rec_compute(ISR_INST svc,ISRrecRecognizerStatus *finalStatus, ISRrecResultType *type, ISR_REC_RESULT *resultData)
{
	printf ("\n Calling ISRrecRecognizerCompute.\n");

	ISRrecRecognizerStatus status;

	int ret;

	const int maxComputeTime = 200;

	do {
		*type = ISR_REC_RESULT_TYPE_COMPLETE;
		status = ISR_REC_STATUS_INCOMPLETE;
		*finalStatus = ISR_REC_STATUS_NO_MATCH;
		ret = ISRRecognizerCompute(svc, maxComputeTime, &status, type,
			resultData);
		if (ret != ISR_SUCCESS)
			return ret;

		printf ("\n Compute Status = %d", status);
		printf ("\n Compute Type = %d\n", *type);

		/******
		* If you do not expect to receive one of the termination
		* statuses below, you should call ISRrecRecognizerStop() to
		* halt the recognizer.  Otherwise, any one of the statuses
		* below will also halt the recognizer.  Do *not* call
		* ISRrecRecognizerStop() if the recognizer has halted,
		* or it will return ISR_REC_ERROR_INACTIVE.
		*
		******/

	} while ((status != ISR_REC_STATUS_SUCCESS) &&
		(status != ISR_REC_STATUS_NO_MATCH) &&
		(status != ISR_REC_STATUS_STOPPED) &&
		(status != ISR_REC_STATUS_MAX_CPU_TIME) &&
		(status != ISR_REC_STATUS_MAX_SPEECH)
		); 

	return ret;
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

	assert(rc == ISR_SUCCESS);
	
	if (rc != ISR_SUCCESS)
	{		
		printf ("\n An Error Was Returned.  Exit Test Application. \n");
		ISRUninitialize();
		exit(1);
	}
}


/* doc that thread create/destroy for recognition is optional */

int start_audio_write(ISR_INST rec, audioThr ** atdpp,unsigned char* audio,int len,const wchar_t* type)
{
	audioThr *atd = (audioThr*)calloc(sizeof(audioThr), 1);
	if(!atd)
		return ISR_ERROR_OUT_OF_MEMORY;
	atd->svc	= rec;
	atd->audio	= audio;
	atd->len	= len;
	atd->type	= type;
	ThreadCreate(&(atd->thr), audio_write_thread, atd);
	*atdpp = atd;
	return ISR_SUCCESS;
}

int finish_audio_write(audioThr *atd)
{
	ThreadJoin(&(atd->thr), &atd);
	free(atd);
	return ISR_SUCCESS;
}

int start_compute(ISR_INST rec, computeThr ** ctdpp, ISR_REC_RESULT* result_data)
{
	computeThr *ctd = (computeThr*)calloc(sizeof(computeThr), 1);
	if(!ctd)
		return ISR_ERROR_OUT_OF_MEMORY;
	ctd->svc			= rec;
	ctd->result_data	= result_data;
	ThreadCreate(&(ctd->thr), compute_thread, ctd);
	*ctdpp = ctd;
	return ISR_SUCCESS;
}

int finish_compute(computeThr *ctd)
{
	ThreadJoin(&(ctd->thr), &ctd);
	free(ctd);
	return ISR_SUCCESS;
}

