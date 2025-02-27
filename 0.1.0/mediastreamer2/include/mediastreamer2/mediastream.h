/*
mediastreamer2 library - modular sound and video processing and streaming
Copyright (C) 2006  Simon MORLAT (simon.morlat@linphone.org)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef MEDIASTREAM_H
#define MEDIASTREAM_H

#include "msfilter.h"
#include "msticker.h"
#include "mssndcard.h"
#include "ortp/ortp.h"

struct _AudioStream
{
	MSTicker *ticker;
	RtpSession *session;
	MSFilter *soundread;
	MSFilter *soundwrite;
	MSFilter *encoder;
	MSFilter *decoder;
	MSFilter *rtprecv;
	MSFilter *rtpsend;
	MSFilter *dtmfgen;
	MSFilter *ec;/*echo canceler*/
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AudioStream AudioStream;

struct _RingStream
{
	MSTicker *ticker;
	MSFilter *source;
	MSFilter *sndwrite;
};

typedef struct _RingStream RingStream;



/* start a thread that does sampling->encoding->rtp_sending|rtp_receiving->decoding->playing */
AudioStream *audio_stream_start (RtpProfile * prof, int locport, const char *remip,
				 int remport, int payload_type, int jitt_comp, bool_t echo_cancel);

AudioStream *audio_stream_start_with_sndcards(RtpProfile * prof, int locport, const char *remip4, int remport, int payload_type, int jitt_comp, MSSndCard *playcard, MSSndCard *captcard, bool_t echocancel);

int audio_stream_start_with_files (AudioStream * stream, RtpProfile * prof,
					    const char *remip, int remport,
					    int pt, int jitt_comp,
					    const char * infile,  const char * outfile);

void audio_stream_play(AudioStream *st, const char *name);
void audio_stream_record(AudioStream *st, const char *name);

void audio_stream_set_rtcp_information(AudioStream *st, const char *cname, const char *tool);

/* those two function do the same as audio_stream_start() but in two steps
this is useful to make sure that sockets are open before sending an invite; 
or to start to stream only after receiving an ack.*/
AudioStream *audio_stream_new(int locport, bool_t ipv6);
int audio_stream_start_now(AudioStream * stream, RtpProfile * prof,  const char *remip, int remport, int payload_type, int jitt_comp,MSSndCard *playcard, MSSndCard *captcard, bool_t echo_cancel);


/* stop the above process*/
void audio_stream_stop (AudioStream * stream);

RingStream *ring_start (const char * file, int interval, MSSndCard *sndcard);
RingStream *ring_start_with_cb(const char * file, int interval, MSSndCard *sndcard, MSFilterNotifyFunc func, void * user_data);
void ring_stop (RingStream * stream);


/* send a dtmf */
int audio_stream_send_dtmf (AudioStream * stream, char dtmf);

void audio_stream_set_default_card(int cardindex);


/*****************
  Video Support
 *****************/


struct _VideoStream
{
	MSTicker *ticker;
	RtpSession *session;
	MSFilter *source;
	MSFilter *predec;
	MSFilter *pixconv;
	MSFilter *tee;
	MSFilter *output;
	MSFilter *encoder;
	MSFilter *decoder;
	MSFilter *rtprecv;
	MSFilter *rtpsend;
};


typedef struct _VideoStream VideoStream;
VideoStream *video_stream_new(int locport, bool_t use_ipv6);
int video_stream_start(VideoStream * stream, RtpProfile *profile, const char *remip, int remport, int payload, int jitt_comp, const char *device);
void video_stream_set_rtcp_information(VideoStream *st, const char *cname, const char *tool);
/*function to call periodically to handle various events */
void video_stream_iterate(VideoStream *stream);
void video_stream_send_vfu(VideoStream *stream);
void video_stream_stop (VideoStream * stream);

VideoStream * video_preview_start(const char *device);
void video_preview_stop(VideoStream *stream);

int video_stream_recv_only_start(VideoStream * stream, RtpProfile *profile, const char *remip, int remport, int payload, int jitt_comp, const char *device);
int video_stream_send_only_start(VideoStream * stream, RtpProfile *profile, const char *remip, int remport, int payload, int jitt_comp, const char *device);
void video_stream_recv_only_stop(VideoStream *stream);
void video_stream_send_only_stop(VideoStream *stream);


bool_t ms_is_ipv6(const char *address);

#ifdef __cplusplus
}
#endif


#endif
