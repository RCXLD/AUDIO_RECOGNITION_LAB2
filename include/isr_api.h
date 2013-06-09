/** 
 * @file	isr_api.h
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
 * @date	2008/11/26
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/11/26	<td>Speech	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __ISR_API_H__
#define __ISR_API_H__

#include "isr_errors.h"
#include "isr_types.h"
#include "isr_ep.h"
#include "isr_rec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Opaque data types
 */

typedef struct ISRRecognizer { void *internal_; } ISRRecognizer, * ISR_INST;

/* -- start recognizer interface -- */

/*
 * The following are generic errors that should always be checked for:
 * ISR_ERROR_NOTINIT if ISRrecInitialize hasn't been called
 * ISR_ERROR_INVALID_PARA if any required parameters are NULL or
 *   zero-length strings, etc
 * ISR_ERROR_OUT_OF_MEMORY if a memory allocation failed
 * Other codes not in isr_error.h  if some
 *   non-memory-allocation OS request fails
 *
 * ISR_ERROR_FAIL unspecified failure
 * ISR_ERROR_GENERAL unspecified error
 * ISR_ERROR_EXCEPTION unspecified fatal error
 */

/**
 * Global initialization.
 * Should be invoked once at process start-up.
 * This function can only be called once per process.
 *
 * @param engine Engine name or path to initialized.
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY
 *         if it has been called previously.
 * @return ISR_ERROR_INVALID_LANGUAGE if default language is invalid
 * @return ISR_ERROR_INITPROC 
 *      any unspecified error during initialization (e.g. license failure)
 */
int ISRAPI ISRInitialize(const wchar_t * engine, void * reserved);
typedef int (ISRAPI * Proc_ISRInitialize)(const wchar_t * engine, void * reserved);


/**
 * Global shutdown.
 * Should be invoked once at process termination.
 * This function can only be called once per process.
 * This function can be called even when there are outstanding recognitions.
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY
 *         if it has been called previously or ISRrecInit was never called
 */
int ISRAPI ISRUninitialize(void);
typedef int (ISRAPI * Proc_ISRUninitialize)(void);


/**
 * Create a recognizer resource.
 *
 * @param rec Recognizer handle
 * @param params  Not currently used in local version. Pass NULL.
 *				  In network version, can specify the recognize server address to use,
 *                the address may be "serverAddr=ip:port" or "serverAddr=name:port" format
 *                NULL or empty string will select a server automatic.
 *
 * @return ISR_ERROR_NO_LICENSE
 *         in "default" licensing mode, no more licenses are available
 * @return ISR_SUCCESS on success
 */
int ISRAPI ISRRecognizerCreate(ISR_INST *rec, const wchar_t *channelName, const wchar_t *params);
typedef int (ISRAPI * Proc_ISRRecognizerCreate)(ISR_INST *rec, const wchar_t *channelName, const wchar_t *params);


/**
 * Destroy and de-register a recognizer resource.
 *
 * @param rec Recognizer handle
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY
 *         if called while recognition is active.
 */
int ISRAPI ISRRecognizerDestroy(ISR_INST rec);
typedef int (ISRAPI * Proc_ISRRecognizerDestroy)(ISR_INST rec);


/**
 * Activate a grammar for subsequent recognition calls.
 * More than one grammar can be activated at any given time.
 * The grammar must have been loaded with GrammarLoad().
 * See GrammarLoad for error return codes when there is an error in the grammar
 * specification.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 * @param weight Relative weight to assign to this grammar vs. other activated
 *               grammars
 * @param grammar_id The ID that is returned for the ISR_grammarName key to
 *               identify which grammar each nbest answer the answer came from
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active.
 * @return ISR_ERROR_DUPLICATE_GRAMMAR
 *         if the same grammar has already been activated
 * @return ISR_ERROR_NO_SESSION_NAME if recognizer doesn't have session name
 * @return GrammarLoad-errors 
 *         Grammar activation may cause a grammar load so all those errors may 
 *         be returned.
 */
int ISRAPI ISRGrammarActivate(ISR_INST rec, const ISRrecGrammarData *grammar, unsigned int weight, const char *grammarID);
typedef int (ISRAPI * Proc_ISRGrammarActivate)(ISR_INST rec, const ISRrecGrammarData *grammar, unsigned int weight, const char *grammarID);


/**
 * Deactivate a loaded grammar for subsequent recognition calls.
 * See GrammarLoad for error return codes when there is an error in the
 * grammar specification.
 *
 * @param rec Recognizer handle
 * @param grammar An object containing the URI and fetch params, or string
 *                NULL deactivates all grammars
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active.
 * @return ISR_ERROR_REC_GRAMMAR_NOT_ACTIVATED
 *         if the grammar is not currently active.
 */
int ISRAPI ISRGrammarDeactivate(ISR_INST rec, const ISRrecGrammarData *grammar);
typedef int (ISRAPI * Proc_ISRGrammarDeactivate)(ISR_INST rec, const ISRrecGrammarData *grammar);


/**
 * Set recognition parameters
 * Parameter values are valid for the during of a single recognition.
 * Must be called before RecognizerStart() and will take effect for the next
 * recognition.
 * Refer to the reference documentation for parameter descriptions
 * and legal values.
 *
 * @param rec Recognizer handle
 * @param param Parameter name
 * @param value String value to set
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active.
 * @return ISR_ERROR_INVALIDPARA
 *         if param is not a valid parameter.
 * @return ISR_ERROR_INVALIDPARA
 *         if value is not valid.
 */
int ISRAPI ISRSetParameter(ISR_INST rec, const wchar_t *param, const wchar_t *value);
typedef int (ISRAPI * Proc_ISRSetParameter)(ISR_INST rec, const wchar_t *param, const wchar_t *value);


/**
 * Get recognition parameters
 * Parameter values are valid for the during of a single recognition.
 * Not all parameter values may be retrieved
 * Refer to the reference documentation for parameter descriptions
 * and legal values.
 *
 * @param rec Recognizer handle
 * @param param Parameter name
 * @param value Pointer to parameter string retrieved
 * @param len Length of string buffer passed in;
 *             if too short, required length will be returned here
 *             and ISR_ERROR_NO_ENOUGH_BUFFER will be set
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_NO_ENOUGH_BUFFER when size of value string is too small
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active.
 * @return ISR_ERROR_INVALIDPARA if
 *         param is not a valid parameter.
 */
int ISRAPI ISRGetParameter(ISR_INST rec, const wchar_t *param, wchar_t *value, unsigned int *len);
typedef int (ISRAPI * Proc_ISRGetParameter)(ISR_INST rec, const wchar_t *param, wchar_t *value, unsigned int *len);


/**
 * Start recognition using currently active grammars, variable bindings,
 * and property values.  This function returns immediately.  Results can
 * be computed by calling RecognizerCompute().
 *
 * @param rec Recognizer handle
 *
 * @return ISR_SUCCESS on success
 * @return ISRrec_ERROR_NO_ACTIVE_GRAMMARS if no grammars have been activated.
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active (RecognizerStart has already
 *         been called without RecognizerStop being called or normal
 *         termination of recognition).
 * @return ISR_ERROR_NO_SESSION_NAME if SetSessionName has not been called.
 * @return ISR_ERROR_NO_LICENSE
 *         if license checkout request fails (JIT mode) or no license checked
 *         out (explicit mode)
 */
int ISRAPI ISRRecognizerStart(ISR_INST rec);
typedef int (ISRAPI * Proc_ISRRecognizerStart)(ISR_INST rec);


/**
 * Send audio samples to recognizer during recognition.
 * This function returns immediately.
 * The recognizer will return an appropriate audio status event via
 * RecognizerCompute() as soon as one of the following conditions
 * is detected:
 *  - The audio input is definitely not speech.
 *  - The audio input is definitely speech.
 *  - An end-of-speech or max-speech condition has been reached.
 * Only one of the first two state will be returned per recognition.
 * AudioWrite can be called before RecognizerStart.  When RecognizerStart
 * is called, audio buffers are ignored until one is found with
 * ISR_AUDIO_SAMPLE_FIRST.
 *
 * @param rec Recognizer handle
 * @param samples Sample data object
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_INVALIDPARA
 *         if a. invalid status specified b. samples->len < 0 
 *         c. invalid combination of samples->len, samples->samples, and audio
 *         type
 * @return ISR_ERROR_NO_SESSION_NAME if recognizer doesn't have a session 
 *         name
 * @return ISR_ERROR_NO_ENOUGH_BUFFER samples->len > 4000
 * @return ISR_ERROR_INVALID_MEDIA_TYPE 
 *         if audio-media type is invalid
 */
int ISRAPI ISRAudioWrite(ISR_INST rec, ISRAudioSamples *samples, ISRepState *state, int *beginSample, int *endSample);
typedef int (ISRAPI * Proc_ISRAudioWrite)(ISR_INST rec, ISRAudioSamples *samples, ISRepState *state, int *beginSample, int *endSample);


/**
 * Called when the prompt is done playing; affects endpointer thresholds
 *
 * @param rec Recognizer handle
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_INACTIVE if endpointer is not active
 */
int ISRAPI ISRPromptDone(ISR_INST rec);
typedef int (ISRAPI *Proc_ISRPromptDone)(ISR_INST rec);


/**
 * Compute results for current recognition.
 * Blocks until recognizer produces complete or partial recognition results
 * (as requested); or is halted; or returns an audio status.
 *
 * The results data are valid until the next call to RecognizerStart(),
 * RecognizerCompute(), or ParseDTMFResults().  If the status returned by
 * RecognizerCompute() is SUCCESS, FAILURE, STOPPED, MAX_SPEECH, or
 * MAX_CPU_TIME, and complete results were requested, then subsequent calls
 * to RecognizerCompute() return the same status value until the next call to
 * RecognizerStart().
 *
 * @param rec Recognizer handle
 * @param maxComputeTime Maximum time to compute results during this call
 *                       (in ms of real time); -1 indicates block until
 *                       completion or change in state; 
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
 *         run-time error during grammar parsing (usually ECMAscript bug)
 * @return ISR_ERROR_INVALIDPARA
 *         input parameter maxComputeTime is 0
 * @return ISR_ERROR_REC_INACTIVE if
 *         RecognizerStart has not been called.
 */
int ISRAPI ISRRecognizerCompute(ISR_INST rec, int maxComputeTime, ISRrecRecognizerStatus *status, ISRrecResultType *type, ISR_REC_RESULT *resultData);
typedef int (ISRAPI * Proc_ISRRecognizerCompute)(ISR_INST rec, int maxComputeTime, ISRrecRecognizerStatus *status, ISRrecResultType *type, ISR_REC_RESULT *resultData);


/**
 * Retrieve the waveform for the last recognition utterance
 * This function will succeed only if isrrec_return_waveform was set for this
 * recognition
 * The waveform is in 8 Khz u-law format
 *
 * @param resultData Results data structure
 * @param type Parameter-value pair indicating what kind of transformation to 
 *             perform on waveform, e.g. 
 *             "silence_suppression=begin+end+interword"
 * @param waveform Pointer to waveform buffer within results data
 * @param len Length of waveform in bytes
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_NODATA
 *         if waveform saving was not enabled or recognition failed
 */
int ISRAPI ISRGetWaveform(ISR_REC_RESULT resultData, const wchar_t *type, void **waveform, unsigned int *len);
typedef int (ISRAPI * Proc_ISRGetWaveform)(ISR_REC_RESULT resultData, const wchar_t *type, void **waveform, unsigned int *len);


/**
 * Return an XML result
 *
 * As with other result functions, the results are valid until the
 * next call to RecognizerStart(), RecognizerCompute(), or this
 * function again
 *
 * @param resultData Result vector containing results of current recognition 
 *                   including, recognized string, confidence, key/value list
 *                   w/ confidences
 * @param format     Format of the XML grammar
 * @param xmlResult  XML result to be returned
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_INVALID_MEDIA_TYPE
 *         if format is invalid
 */
int ISRAPI ISRGetXMLResult(ISR_REC_RESULT resultData, const wchar_t *format, const wchar_t **xmlResult);
typedef int (ISRAPI * Proc_ISRGetXMLResult)(ISR_REC_RESULT resultData, const wchar_t *format, const wchar_t **xmlResult);


/**
 * Parse a DTMF string against the currently active grammars
 * At least one active grammar must specify rules for parsing DTMF input
 * Aborts recognition of speech and discards all audio input
 *
 * The results data are valid until the next call to RecognizerStart(),
 * RecognizerCompute(), or ParseDTMFResults().
 *
 * @param rec Recognizer handle
 * @param str DTMF string
 * @param status Status of parse
 * @param resultData  Result vector containing results of current
 *                    recognition including, recognized string,
 *                    confidence, key/value list w/ confidences
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_BUSY if
 *         called while recognition is active.
 * @return ISR_ERROR_INVALIDPARA
 *         if str is not a valid DTMF string.
 */
int ISRAPI ISRParseDTMFResults(ISR_INST rec, const char *str, ISRrecParseStatus *status, ISR_REC_RESULT *resultData);
typedef int (ISRAPI * Proc_ISRParseDTMFResults)(ISR_INST rec, const char *str, ISRrecParseStatus *status, ISR_REC_RESULT *resultData);


/**
 * Write an event and related information into the recognizer event log
 *
 * @param rec Recognizer handle
 * @param event Event name
 * @param value String to be logged.  This value is recommended to be composed 
 *              of a token/value list which matches the recognizer logging
 *              format.
 *
 * @return ISR_SUCCESS on success
 */
int ISRAPI ISRLogEvent(ISR_INST rec, const wchar_t *event, const wchar_t *value);
typedef int (ISRAPI * Proc_ISRLogEvent)(ISR_INST rec, const wchar_t *event, const wchar_t *value);


/**
 * Stop current recognition.
 * Aborts recognition of speech, discards all audio input, and discard all
 * temporary recognition storage for this utterance.
 *
 * This function should be called to interrupt a currently active
 * ISRrecRecognizerCompute() or after ISRrecRecognizerCompute() returns if no
 * more recognition is required on the current utterance.
 * This function must be called before RecognizerStart() can be called unless
 * RecognizerCompute() returned a status (NOT return code) of:
 * SUCCESS, FAILURE, MAX_SPEECH, or MAX_CPU_TIME.
 * Subsequent calls to RecognizerCompute() will return a STOPPED status until
 * RecognizerStart() is called.  Subsequent calls to ParseDTMFResults() are
 * still valid.
 *
 * @param rec Recognizer handle
 * @param code Reason for the stop.  This should be one of: ISRrec_STOP_SPEECH,
 *             ISRrec_STOP_DTMF, ISRrec_STOP_HANGUP, ISRrec_STOP_TIMEOUT, 
 *             ISRrec_STOP_OTHER
 *
 * @return ISR_SUCCESS on success
 * @return ISR_ERROR_INACTIVE if recognition is not active.
 */
int ISRAPI ISRRecognizerStop(ISR_INST rec, ISRrecStopCode code);
typedef int (ISRAPI *Proc_ISRRecognizerStop)(ISR_INST rec, ISRrecStopCode code);


/**
 * Get the recognizer handle from api handle.
 * The handle is same as isr_rec library, can using with the isr_rec APIs.
 *
 * @param rec Recognizer handle
 */
ISR_REC_INST ISRAPI ISRGetRecHandle(ISR_INST rec);
typedef ISR_REC_INST (ISRAPI *Proc_ISRGetRecHandle)(ISR_INST rec);


/**
 * Get the end pointer handle from api handle.
 * The handle is same as isr_ep library, can using with the isr_ep APIs.
 *
 * @param rec Recognizer handle
 */
ISR_EP_INST ISRAPI ISRGetEpHandle(ISR_INST rec);
typedef ISR_EP_INST (ISRAPI *Proc_ISRGetEpHandle)(ISR_INST rec);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ISR_API_H__ */
