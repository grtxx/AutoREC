//---------------------------------------------------------------------------

#ifndef grtdxutilsH
#define grtdxutilsH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "c:\DXSDK\Include\dshow.h"
#include "c:\DXSDK\Include\ks.h"
#include "c:\DXSDK\Include\ksmedia.h"
#include "c:\DXSDK\Include\uuids.h"
#include "c:\DXSDK\Include\strmif.h"
#include "c:\DXSDK\Include\qedit.h"

IBaseFilter* CreateBaseFilterByCLSID(System::TGUID cat);
IBaseFilter* CreateBaseFilterByCatAndName(System::TGUID cat, AnsiString Name);
IPin* FindPinByName(IBaseFilter *pfilter, wchar_t PinName);
IPin* FindFreePin(IBaseFilter *pfilter, int direction);
IPin* FindFreeInputPin(IBaseFilter *pfilter);
IPin* FindFreeOutputPin(IBaseFilter *pfilter);

#endif
