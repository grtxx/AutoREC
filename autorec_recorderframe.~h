//---------------------------------------------------------------------------


#ifndef autorec_recorderframeH
#define autorec_recorderframeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "c:\DXSDK\Include\dshow.h"
#include "c:\DXSDK\Include\ks.h"
#include "c:\DXSDK\Include\ksmedia.h"
#include "c:\DXSDK\Include\uuids.h"
#include <stdio.h>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "c:\DXSDK\Include\strmif.h"
#include "c:\DXSDK\Include\qedit.h"
#include <Graphics.hpp>
#define _BLADEDLL
#include "blade_enc.h"
#include "grtdxutils.h"
#include "grtvumeter.h"
#include "gsockets.h"
#include "winsock2.h"
#include "ds1_recordingthread.h"
#include "ds1_encoderthread.h"
#include "ds1_mp3savethread.h"

//---------------------------------------------------------------------------
const AnsiString filenameformat = "yyyy-mm-dd_hh-nn-ss";

//---------------------------------------------------------------------------

class Tmp3record : public TFrame
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TTimer *Timer1;
        TButton *Button4;
        TImage *Image1;
        TLabel *Label1;
        TLabel *SignalIndicator;
        TLabel *StatusIndicator;
		TTimer *Timer2;
		void __fastcall Button3Click(TObject *Sender);
		void __fastcall Timer1Timer(TObject *Sender);
		void __fastcall Button1Click(TObject *Sender);
		void __fastcall Button2Click(TObject *Sender);
		void __fastcall Button4Click(TObject *Sender);
		void __fastcall Timer2Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
		int ConfigNumber;

		IGraphBuilder *FL;
		IMediaControl *MC;
		IMediaSeeking *MS;
		ISampleGrabber *sgGrabber;
		IBaseFilter *Source;
		IBaseFilter *bfSampleGrabber;

		IDirectSoundCapture *CapDS;
		tRecordThread *RT;
		tEncoderThread *EN;
                tSaveThread *SV;

		AnsiString InputDeviceGuid;
		AnsiString InputDeviceName;
		AnsiString mp3dir;
		AnsiString mp3dir2;
		DWORD dwSampleRate;
		WORD wBitRate;
		int deletedays;
		int pgl, pgr;
		TDateTime LastSignal;
		int Status;
		TStringList *Changes;
		int LastTme;
		VUMeter *VU;

		bool email_do;
		AnsiString email_from;
		AnsiString email_rcpt;
		AnsiString email_smtp;

		bool http_do;
		AnsiString http_url;
		
		__fastcall Tmp3record(TComponent* Owner);
		void __fastcall SendAlert(AnsiString txt, int code);
		void __fastcall Dirdelete(AnsiString dir, AnsiString limit);
		__fastcall Free();
        __fastcall LoadConfig(int aconfignumber);
        __fastcall Start();
		__fastcall Stop();
};
//---------------------------------------------------------------------------
extern PACKAGE Tmp3record *mp3record;
//---------------------------------------------------------------------------
#endif
