//---------------------------------------------------------------------------

#ifndef ds1_mp3savethreadH
#define ds1_mp3savethreadH
//---------------------------------------------------------------------------

#include <windows.h>
#include <classes.hpp>
#include "circbuffer.h"

class tSaveThread : public TThread
{
        void __fastcall Execute();
public:
        __fastcall ~tSaveThread();
        __fastcall tSaveThread(tCircularBuffer *pbuffer);
        bool eof;
        bool bStartNew;
        AnsiString filenameformat;
        AnsiString Directory1;
        AnsiString Directory2;
        HANDLE endEvent;
        bool err;
protected:
        tCircularBuffer *streambuffer;
};

#endif
