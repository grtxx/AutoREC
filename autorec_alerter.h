//---------------------------------------------------------------------------

#ifndef autorec_alerterH
#define autorec_alerterH
//---------------------------------------------------------------------------

#include "gsockets.h"
#include "winsock2.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>


class TAlerterThread : public TThread
{
public:
	AnsiString email_from;
	AnsiString email_rcpt;
	AnsiString email_smtp;
	AnsiString email_text;
	AnsiString http_url;
	int http_func_code;
	int ConfigNumber;	

	__fastcall TAlerterThread();
	void __fastcall Execute();
};

#endif
