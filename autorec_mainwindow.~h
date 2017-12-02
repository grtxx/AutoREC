//---------------------------------------------------------------------------

#ifndef autorec_mainwindowH
#define autorec_mainwindowH
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
#include <Graphics.hpp>

#define _BLADEDLL
#include "blade_enc.h"

//---------------------------------------------------------------------------

typedef short buftype[200000];

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TImage *Image2;
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        long loadedsamples;
        Graphics::TBitmap *B;

        buftype buffer;
        long bufsize;
        int mupl, mupr, pgl, pgr;
        TStringList *changes;

        __int64 starttime, endtime;
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
