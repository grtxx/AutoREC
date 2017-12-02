//---------------------------------------------------------------------------

#ifndef ds1_encoderthreadH
#define ds1_encoderthreadH
#define MP3ONLY
//---------------------------------------------------------------------------

#include <windows.h>
#include <classes.hpp>
#include "stdio.h"
#include "circbuffer.h"

#ifndef MP3ONLY
#include "frame.h"
#include "faac.h"
#endif

#define _BLADEDLL
#include "blade_enc.h"

#ifndef ENCODERENUM_DEFINED
#define ENCODERENUM_DEFINED
typedef enum { ENCODER_RAW, ENCODER_AAC, ENCODER_MP3 } tEncoderEnum;
#endif

typedef struct {
        int samplerate;
        int channels;
        int bitrate;
        tEncoderEnum encodingengine;
} tEncoderParams;

#ifndef MP3ONLY
typedef struct {
		faacEncStruct *hEncoder;
		unsigned long inputsamples;
		unsigned long outputsize;
		int channels;
} tAACParams;
#endif

typedef struct {
		BE_CONFIG mp3config;
		unsigned long dwInputBufferLength;
		unsigned long dwOutputBufferLength;
		unsigned long hStream;
		short *InputBuffer;
		char *Mp3Buffer;
} tMP3Params;

class tEncoderThread : public TThread
{
private:
        int hAACEncoder;
		tEncoderParams encoderParams;
		#ifndef MP3ONLY
		tAACParams AACParams;
		#endif
        tMP3Params MP3Params;
        AnsiString outFileName;
        HANDLE inbuffer_event;
        HANDLE outbuffer_event;
public:
        bool eof;
        tCircularBuffer *pcmBuffer;
        tCircularBuffer *bitBuffer;
        HANDLE endEvent;        
        __fastcall ~tEncoderThread();
        __fastcall tEncoderThread(tCircularBuffer *pbuffer);
        void __fastcall Execute();
        void __fastcall EncodeAAC();
        bool __fastcall InitAAC();
        bool __fastcall DeInitAAC();
        void __fastcall EncodeRAW();
        bool __fastcall InitRAW();
        bool __fastcall DeInitRAW();        
        void __fastcall EncodeMP3();
        bool __fastcall InitMP3();
        bool __fastcall DeInitMP3();
        bool __fastcall initEncoder(tEncoderEnum enctype, int samplerate, int channels, int bitrate, AnsiString filename);
};

#endif
