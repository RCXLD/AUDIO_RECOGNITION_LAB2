/**
 * @file    isr_as.h
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
 *  Copyright (C)    1999 - 2007 by ANHUI USTC iFLYTEK Co,LTD.
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

#ifndef __ISR_AS_H__
#define __ISR_AS_H__

#include "base/srbase.h"
#include "isr_errors.h"
#include "isr_types.h"

#pragma warning(disable : 4996)

_BEGIN_NAMESPACE(as)

/*----------------------------------------------------------------------------
* Audio encoding:
*---------------------------------------------------------------------------*/

typedef enum {
	AudioFirstEncoding_ = 0,                    /* Internal use only */
	AudioEncodingUnknown = AudioFirstEncoding_, /* Unknown or don't care */
	AudioEncodingUlaw,                         /* u-law (misspelled) */
	AudioEncodingAlaw,                          /* a-law */
	AudioEncodingLinear,                        /* linear */
	AudioNumEncodings_                          /* Internal use only */
} audio_encoding;

/*----------------------------------------------------------------------------
* Audio format:
*---------------------------------------------------------------------------*/

typedef struct {
	int             rate;               /* Sample rate [samples/sec] */
	int             bits;               /* Sample resolution [bits/sample] */
	audio_encoding  encoding;           /* Encoding (alaw, ulaw, linear) */
} audio_format;

/* Parameter type */
typedef enum {
	ParaZero,
	ParaSimWavPath,		// 模拟as的输入音频文件的路径
	ParaSimWavMode,		// 模拟as的输入音频文件的读取方式（随机\顺序）
	ParaSimChnlNum,		// 模拟as的channel数
	ParaSimSampleSize,	// 模拟as的音频文件的每次读数据量（KB）

} as_para;


class as_channel_i
	: public sr::if_base
{
public:
	// 提示音播放结束通知;
	virtual	int		on_playback_done(void) = 0;

	// 录音数据到来，返回前请设置实际读取的数据长度;
	virtual int		on_sample_recorded(const void *buf, int len) = 0;

	// DTMF事件输入;
	virtual	int		on_dtmf_event(const char *dtmfstr) = 0;

	// 远端挂机通知;
	virtual	int		on_remote_hangup(void) = 0;

};

class as_mngr_i
	: public sr::if_base
{
public:
	// 电话呼入，如果接收，创建一个channel对象返回给as;
	virtual	int		on_incoming_call(as_channel_i **channel) = 0;

	// 电话呼出结束，如果成功建立，创建一个channel对象返回给as;
	virtual	int		on_outgoing_call_completed(int code, as_channel_i **channel) = 0;
};

class audsrc_i
	: public sr::if_base
{
public:
	virtual	int		init(void) = 0;
	virtual	int		fini(void) = 0;

	// 查询该as模块的特性，比如支不支持电话功能
	virtual int		feature(int type) const = 0;

	// 设置AudioSource模块的全局参数
	virtual int		set_param(int type, int value, const void *para1) = 0;
	virtual int		get_param(int type, int &value, void *para1) = 0;

	// 启动/停止AudioSource的运行
	virtual	int		start(as_mngr_i *mngr) = 0;
	virtual	int		stop(void) = 0;

	// 与通道相关的操作
	virtual int		set_param(as_channel_i *channel, int type, int value, const void *para1) = 0;	// 设置通道参数
	virtual int		get_param(as_channel_i *channel, int type, int &value, void *para1) = 0;		// 获取通道参数
	virtual int		start_record(as_channel_i *channel) = 0;						// 通道开始录音
	virtual int		stop_record(as_channel_i *channel) = 0;							// 通道停止录音
	virtual int		playback(as_channel_i *channel, const void *buf, int len) = 0;	// 通道播放提示音
	virtual int		kill_playback(as_channel_i *channel) = 0;						// 通道停止提示音播放
	virtual int		hangup(as_channel_i *channel) = 0;								// 通道挂机指令
	// 主动发起电话呼叫，异步，与on_outgoing_call_completed配合作用
	virtual int		initial_outgoing_call(const char *phone_no) = 0;
};


_END_NAMESPACE	// as

DEFINE_SHPTR(as::audsrc_i, "audsrc");
DEFINE_SHMOD(audsrc);

//////////////////////////////////////////////////////////////////////////


#endif /* __ISR_AP_H__ */
