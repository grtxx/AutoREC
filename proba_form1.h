//---------------------------------------------------------------------------

#ifndef proba_form1H
#define proba_form1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//#include "types.hpp"
//#include "stdio.h"
//#include "mmreg.h"
//#include "fcntl.h"
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
//#include "PERFGRAP.h"
#include "math.h"

#define _BLADEDLL
#include "blade_enc.h"

//---------------------------------------------------------------------------

class IMySGCB : public ISampleGrabberCB
{
public:
    STDMETHODIMP_(ULONG) AddRef()  { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown)
        {
            *ppv = (void *) static_cast<ISampleGrabberCB *>(this);
            return NOERROR;
        }
        return E_NOINTERFACE;
    }

    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample );
    STDMETHODIMP BufferCB( double SampleTime, BYTE * pBuffer, long BufferLen )
    {
        return 0;
    }

};

typedef short buftype[200000];

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TTimer *Timer1;
        TStaticText *StaticText1;
        TButton *setupbutton;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        IGraphBuilder *FL;
        IMediaControl *MC;
        IMediaSeeking *MS;
        ISampleGrabber *SG;
        IMySGCB *pSGCB;
        IBaseFilter *Source;
        IBaseFilter *MP3Decoder;
        IBaseFilter *SampleGrabber;
        IBaseFilter *MP3Grabber;
        IBaseFilter *Tee;
        long loadedsamples;
        Graphics::TBitmap *B;

        buftype buffer;
        long bufsize;
        int mupl, mupr, pgl, pgr;
        FILE *fout;
        HBE_STREAM hStream;
        short *InputBuffer;
        char *Mp3Buffer;
        DWORD dwInputBufPos;
        DWORD dwInputBufferLength;
        DWORD dwOutputBufferLength;

        __int64 starttime, endtime;
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
