/************************************************************************/
/* qisr_example:                                                        */
/*     This example shows how to use quick ISR interface (qisr.dll)     *
/*     to perform automatic speech recognizition task.                  */
/* Headers:                                                             */
/*     qisr.h                                                           */
/* Library:                                                             */
/*      qisr.lib                                                        */
/* DLL:                                                                 */
/*      qisr.dll, isr_api.dll                                           */
/************************************************************************/

#include <string.h>
#include <stdio.h>
#include "qisr.h"
#include "windows.h"

/* option_proc gets grammar file path and wave file path from user input */
int option_proc(int argc , char *argv[], char grammarfile[], char wavefile[])
{
	int index = 0;

	if (argc < 5) return -1;

	while (index < argc)
	{
		if ((strcmp(argv[index], "-g") == 0) && ((index - 1) < argc))
			strcpy(grammarfile, argv[++index]);
		if ((strcmp(argv[index], "-w") == 0) && ((index - 1) < argc))
			strcpy(wavefile, argv[++index]);
		index++;
	}
	return 0;
}

/* main function */
int main(int argc, char *argv[])
{
	int ret = 0;

	char grammar_file[1024];		/* input grammar file    */
	char audiowave_file[1024];		/* input audio wave file */

	const char *params = "isr_rec_best_result_count = 3,isr_ep_audio_media_type=audio/L16;rate=8000";	/* set recognizer give out 3 best result */
	const char *session_id = 0;				/* session ID identifies the session     */
	const char *grammar_type = "uri";		/* grammar type is URI                   */
	int         grammar_weight = 10;
	const char *xml_result = NULL;          /* XML form recognizing result           */

	HMODULE qisr_mod = NULL;

	Proc_QISRInit				QISRInit = NULL;
	Proc_QISRSessionBegin		QISRSessionBegin = NULL;
	Proc_QISRGrammarActivate	QISRGrammarActivate = NULL;
	Proc_QISRSessionEnd			QISRSessionEnd = NULL;
	Proc_QISRAudioWrite			QISRAudioWrite = NULL;
	Proc_QISRGetResult			QISRGetResult = NULL;
	Proc_QISRFini				QISRFini = NULL;

	if (option_proc(argc, argv, grammar_file, audiowave_file))
	{
		printf("Usage: qisr_example.exe -g grammarfile -w wavefile\n");
		return -1;
	}

	/* get dll function addrees */
	qisr_mod = LoadLibraryW(L"qisr.dll");
	if (qisr_mod == NULL)
	{
		printf("load qisr.dll failed with code=%d.\n",GetLastError());
		return;
	}
	QISRInit = (Proc_QISRInit)	  GetProcAddress(qisr_mod,"QISRInit");
	QISRSessionBegin	=  (Proc_QISRSessionBegin)	  GetProcAddress(qisr_mod,"QISRSessionBegin");
	QISRGrammarActivate =  (Proc_QISRGrammarActivate) GetProcAddress(qisr_mod,"QISRGrammarActivate");
	QISRSessionEnd		=  (Proc_QISRSessionEnd)	  GetProcAddress(qisr_mod,"QISRSessionEnd");
	QISRAudioWrite		=  (Proc_QISRAudioWrite)	  GetProcAddress(qisr_mod,"QISRAudioWrite");
	QISRGetResult	=  (Proc_QISRGetResult)	  GetProcAddress(qisr_mod,"QISRGetResult");
	QISRFini	=  (Proc_QISRFini)	  GetProcAddress(qisr_mod,"QISRFini");
	

	/* the first function called must be QISRSessionBegin      */
	/* you can pass grammar file path by first parameter or    */
	/* pass NULL and call QISRGrammarActivate later            */
	ret = QISRInit(0);
	if (ret)
	{
		printf("QISRInit failed with code=%d.\n", ret);
		return ret;
	}

	session_id = QISRSessionBegin(NULL, params, &ret);
	if (ret)
	{
		printf("QISRSessionBegin failed with code=%d.\n", ret);
		return ret;
	}

	/* before recognizing, grammar file should be activated.   */
	/* if you supply grammar file in QISRSesionBegin this      */
	/* step can omitted, or QISRGrammarActivate must be called */
	ret = QISRGrammarActivate(session_id, grammar_file, grammar_type, grammar_weight);
	if (ret)
	{
		printf("QISRGrammarActivate failed with code=%d.\n", ret);
		QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
		return ret;
	}

	{
		/* This example shows how to read wave file */
		/* then put wave data to recognizer         */
		char    bufffer[800];
		size_t  readno = 0;         /* read data length       */
		FILE    *fp = NULL;         /* wave file              */
		int     first_clip = 1;     /* first clip of the file */
		int     audio_status;
		int     ep_status;
		int     recog_status;

		if ((fp = fopen(audiowave_file, "rb")) == NULL)
		{
			printf("open audio wave file: %s failed.\n", audiowave_file);
			QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
			return ret;
		}
		
		while((readno = fread(bufffer, 1, sizeof(bufffer), fp)) != 0)
		{
			if (first_clip)
			{
				audio_status = ISR_AUDIO_SAMPLE_FIRST;
				first_clip = 0;
			}
			else
			{
				if(readno < sizeof(bufffer))
					audio_status =  ISR_AUDIO_SAMPLE_LAST;
				else
					audio_status =  ISR_AUDIO_SAMPLE_CONTINUE;
			}

			/* there calls QISRAudioWrite, you should check the status */
			/* to see if recognizer give out the result                */
			ret = QISRAudioWrite(session_id, bufffer, readno, audio_status, &ep_status, &recog_status);
			if(ret)
			{
				printf("QISRAudioWrite failed with code=%d.\n", ret);
				QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
				fclose(fp);
				return ret;
			}

			/* check EP status */
			if(ep_status == ISR_EP_TIMEOUT || ep_status == ISR_EP_ERROR)
			{
				printf("QISRAudioWrite EP failed with status=%d.\n", ep_status);
				QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
				fclose(fp);
				return ret;
			}

			/* check recog_status, if recog_status equals one of     */
			/* following values, it means recognizer gave the result */
			if(recog_status == ISR_REC_STATUS_NO_MATCH 
				|| recog_status == ISR_REC_STATUS_MAX_SPEECH
				|| recog_status == ISR_REC_STATUS_SUCCESS)

			{
				break;
			}
		} /* end of wave input */

		fclose(fp);
	}

	/* now XML result can be fetched */
	xml_result = QISRGetResult(session_id);
	if(!xml_result)
	{
		printf("QISRGetXMLResult failed with code=%d.\n", ret);
		QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
		return ret;
	}

	/* print XML result */
	printf("%s\n", xml_result);

	/* last call QISRSessionEnd */
	QISRSessionEnd(session_id, "ISR_REC_STOP_SUCCESS");

	QISRFini();
	/* free dll */
	FreeLibrary(qisr_mod);

	
	return 0;
}



