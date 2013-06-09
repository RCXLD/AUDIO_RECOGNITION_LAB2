/**
 * @file    isr_rec.h
 * @brief   iFLY Speech Recognizer Header File
 * 
 *  This file contains the special and not opened declarations 
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

#ifndef __ISR_SPEC_H__
#define __ISR_SPEC_H__

#include "isr_types.h"

/**
 * Interpret results for the sentence using current activated grammars.
 *
 * The results data can be got in XML result from the ISRrecGetXMLResult.
 *
 * @param rec Recognizer handle
 * @param sent Sentence to interpret by recognizer
 * @param status Status of recognition upon completion
 * @param type Type of results returned.
 * @param resultData Result vector containing results of current
 *                   recognition including, recognized string,
 *                   confidence, key/value list w/ confidences.
 *                   Results are only returned if result type is
 *                   PARTIAL or COMPLETE.
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_REC_GRAMMAR_ERROR
 *         run-time error during grammar parsing.
 * @return ISR_ERROR_INVALIDPARA
 *         input parameter sent is NULL or empty.
 */
int ISRAPI ISRrecRecognizerInterpret(ISR_REC_INST rec, const wchar_t *sent, ISRrecRecognizerStatus *status, ISRrecResultType *type, ISR_REC_RESULT *resultData);
typedef int (ISRAPI * Proc_ISRrecRecognizerInterpret)(ISR_REC_INST rec, const wchar_t *sent, ISRrecRecognizerStatus *status, ISRrecResultType *type, ISR_REC_RESULT *resultData);


#endif /* __ISR_SPEC_H__ */
