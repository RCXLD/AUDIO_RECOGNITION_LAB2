/**
 * @file    isr_ve.h
 * @brief   iFLY Speech Recognizer Header File
 * 
 *  This file contains the application programming interface (API) declarations 
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
 * @date    2007/04/17
 * 
 * @see        
 * 
 * History:
 * index    version        date            author        notes
 * 0        1.0            2007/04/17      Speech        Create this file
 */
#ifndef __ISR_VE_H__
#define __ISR_VE_H__

#include "isr_rec.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Voice Enrollment Handle */
typedef void* ISR_VE_INST;

/* const defines */
enum
{ 
	ISR_VE_VERSION = 1 
};

/**
 * ISRveBegin is called before starting to enroll a set of utterances
 * associated with a particular phrase.  Once enrollment is complete, call
 * ISRveEnd to cleanup.
 *
 *   ISR_VE_INST handle;
 *   ISRveBeginParameters params;
 *   params.meaning = L"call mom";
 *   rc = ISRveBegin(&handle, &params);
 *   ...
 *   ISRveEnd(&handle);
 *
 * @param params  Parameters for the current enrollment session.
 * @returns
 *  0:  
 *    Handle successfully created.
 *  -1:
 *    Could not create a handle.
 */
typedef struct
{
	int             version;    /* Structure version */
	const wchar_t*  meaning;    /* ISR_meaning to return for enrollment grammar */
	const wchar_t*  language;   /* Language to use for enrollment grammar */
	int             nprons;     /* Optional, set to 0 if not supplied.  List of */
	const wchar_t** prons;      /* prons already enrolled for this meaning when */
							    /* re-enrolling the same phrase (upgrades) */
	const wchar_t*	serverAddr; /* Server address used by network version */
} ISRveBeginParameters;


/** 
 * @fn		ISRveBegin
 * @brief	Begin Voice Enrollment
 * 
 *  Allocate enrollment resources
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	ISR_VE_INST *ve	- [out] 
 * @param	const ISRveBeginParameters* params	- [in] 
 */
int ISRAPI ISRveBegin(ISR_VE_INST *ve, const ISRveBeginParameters *params);
typedef int (ISRAPI *Proc_ISRveBegin)(ISR_VE_INST *ve, const ISRveBeginParameters *params);


/** 
 * @fn		ISRveEnd
 * @brief	End Voice Enrollment
 * 
 *  De-allocate enrollment resources.
 * 
 * @return	int ISRAPI	- Return 0 in success, otherwise return error code.
 * @param	ISR_VE_INST ve	- [in] 
 * @see		
 */
int ISRAPI ISRveEnd(ISR_VE_INST ve);
typedef int (ISRAPI *Proc_ISRveEnd)(ISR_VE_INST ve);

/* 
 * Called before enrolling an utterance (a single turn).  Given the current
 * state of the enrollment, returns a description of what grammars should
 * be activated using the ISRrecAPI.  
 *
 * The enrollment grammar must be activated with the weight and 
 * grammar id given.
 *
 * The (optional) confusion grammars are any grammars which should not be
 * confusable with the enrollment phrase.  These would be things like
 * command grammars and application commands separate from the enrollment
 * phrase.  They must be activated with the weight given, and can use any
 * id as long as it is different from the enrollment, consistency and clash
 * id's given by ISRve.
 *
 * The (optional) clash grammar is a list of already enrolled phrases.
 * ISRve will check whether the enrolled phrase clashes with any phrase
 * already enrolled using this grammar.  It must be activated with the
 * weight and grammar id given.
 *
 *   ISRveRecognitionParameters recParams;
 *   rc = ISRvePrepare(handle, &recParams);
 *   ISRrecGrammarActivate(rec, recParams.enrollmentGrammar,
 *     recParams.enrollmentWeight, recParams.enrollmentId);
 *   ISRrecGrammarActivate(rec, myCommandGrammar,
 *     recParams.confusionWeight, myCommandId);
 *   ISRrecGrammarActivate(rec, anotherGrammar,
 *     recParams.confusionWeight, anotherId);
 *   GetExistingPronsFromDatabase(&myClashingPronsListGrammar); // e.g.
 *   ISRrecGrammarActivate(rec, myClashingPronsListGrammar,
 *     recParams.clashWeight, recParams.clashId);
 *   .... recognize ....
 *
 * @param ve Handle returned by ISRveBegin()
 * @param params Parameters for processing.  The consistencyThreshold is
 *   The threshold used for determining how consistent a pron is from one
 *   turn of the enrollment to the next (0.0 always, 1.0 never).  The
 *   clashThreshold for testing for clashing prons from a previous
 *   enrollment (0.0 always, 1.0 never).
 * @param recGrams Pointer to ISRveRecognitionParameters structure to
 *   fill in with the description of how grammars should be activated for
 *   the next turn.
 *
 * @returns
 *  0:  
 *    recParams populated for the next turn
 *  -1:
 *    Some kind of fatal error occurred.  ISRveEnd() must be called to clean
 *    up.
 */
typedef struct
{
	float          consistencyThreshold;
	float          clashThreshold;
} ISRvePrepareParameters;

typedef struct
{
	const ISRrecGrammarData* enrollmentGrammar;
	int                      enrollmentWeight;
	const wchar_t *          enrollmentId;
	int                      confusionWeight;
	int                      clashWeight;
	const wchar_t *          clashId;
} ISRvePrepareGrammars;

int ISRAPI ISRvePrepare(ISR_VE_INST ve, const ISRvePrepareParameters *params, ISRvePrepareGrammars *recGrams);
typedef int (ISRAPI *Proc_ISRvePrepare)(ISR_VE_INST ve, const ISRvePrepareParameters *params, ISRvePrepareGrammars *recGrams);

/**
 * After recognizing against the grammars activated according to what
 * ISRvePrepare returned, pass the XML results produced by the recognizer
 * into ISRveProcess to get an enrollment XML result for the current state
 * of the enrollment.  
 *
 * @param ve            Handle as returned by ISRveBegin()
 * @param recXMLResults Results as returned by ISRrecGetXMLResult()
 * @param veXMLResults  Voice enrollment specific XML result.
 *
 * @returns
 *  0:
 *    Results were successfully generated.
 *  -1:
 *    Results could not be generated.  Fatal error, call ISRveEnd().
 */
int ISRAPI ISRveProcess(ISR_VE_INST ve, const wchar_t *recXMLResults, const wchar_t **veXMLResults);
typedef int (ISRAPI *Proc_ISRveProcess)(ISR_VE_INST ve, const wchar_t *recXMLResults, const wchar_t **veXMLResults);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ISR_VE_H__ */
