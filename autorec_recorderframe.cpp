//---------------------------------------------------------------------------

#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop

#include "autorec_recorderframe.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "grtdxutils.h"
#include "autorec_setupwindow.h"
#include "autorec_alerter.h"


Tmp3record *mp3record;


//---------------------------------------------------------------------------
__fastcall Tmp3record::Free()
{
        Stop();
        Changes->Free();
        VU->Free();
        TFrame::Free();
}

__fastcall Tmp3record::Tmp3record(TComponent* Owner)
        : TFrame(Owner)
{
        DoubleBuffered=false;
		Changes=new TStringList;
		VU=new VUMeter(this);
		VU->Left=89;
		VU->Width=32;
		VU->Top=20;
		VU->Height=136;
		VU->showkind=2;
		VU->logaritmic=20;
		InsertControl(VU);
}

__fastcall Tmp3record::LoadConfig(int aconfignumber)
{
		TRegistry *R=new TRegistry;

		ConfigNumber=aconfignumber;
		InputDeviceName="";
		R->RootKey=HKEY_LOCAL_MACHINE;
		R->OpenKey("Software\\GRT\\AutoREC\\"+IntToStr(aconfignumber),true);
        try
        {
                if (R->ValueExists("SampleRate"))
                {
						InputDeviceName=R->ReadString("CaptureDevice");
						InputDeviceGuid=R->ReadString("CaptureDeviceGuid");
						dwSampleRate=R->ReadInteger("SampleRate");
                        wBitRate=R->ReadInteger("BitRate");
                        mp3dir=R->ReadString("DestinationDirectory");
                        mp3dir2=R->ReadString("DestinationDirectory2");                        
                        deletedays=R->ReadInteger("AutoDelete");
                        int count=R->ReadInteger("FileChangeCount");
                        int i;
                        Changes->Clear();
                        for (i=0; i<count; i++)
                        {
                                Changes->Add(R->ReadString("FileChange"+IntToStr(i)));
						}

						if (R->ValueExists("email_do")) email_do=R->ReadBool("email_do");
						email_rcpt=R->ReadString("email_rcpt");
						email_from=R->ReadString("email_from");
						email_smtp=R->ReadString("email_smtp");

						if (R->ValueExists("http_do")) http_do=R->ReadBool("http_do");
						http_url=R->ReadString("http_url");
				}
		}
		__finally
		{
				R->Free();
		}
		pgl=32768;
		pgr=32768;
		Label1->Caption="REC: "+IntToStr(ConfigNumber);
		return true;
}

__fastcall Tmp3record::Start()
{
  CoInitialize(NULL);
  if (InputDeviceName!="" && Status==0)
  {
	// Create directsoundcapture
	CapDS=NULL;
	GUID scard;

	if (InputDeviceGuid=="") {
		scard=DSDEVID_DefaultCapture;
	} else {
		scard=Sysutils::StringToGUID(InputDeviceGuid);
	}

	if FAILED ( DirectSoundCaptureCreate(&scard, &CapDS, NULL) )
	{
	  MessageBoxA(Handle, "Cannot create Directsound interface", "ERROR", 16);
	} else {

		HANDLE endevent=CreateEvent(NULL,true,false,NULL);

		// Init recordthread
		RT=new tRecordThread(CapDS, dwSampleRate, 2, 16);
	   	RT->Priority=tpTimeCritical;
		RT->endEvent=endevent;

		// Init encoderthread
		EN=new tEncoderThread(RT->cBuffer);
		EN->initEncoder(ENCODER_MP3, dwSampleRate, 2, wBitRate, "");
		EN->Priority=tpHigher;

                // Init savethread
                SV=new tSaveThread(EN->bitBuffer);
                SV->Priority=tpHigher;
                SV->filenameformat=filenameformat;
                SV->Directory1=mp3dir;
                SV->Directory2=mp3dir2;                

		RT->Resume();
		EN->Resume();
                SV->Resume();
                
		Status=2;
	}


  }
  return true;
}

__fastcall Tmp3record::Stop()
{
		if (Status=2)
		{
			Status=0;


                        HANDLE endevent=SV->endEvent;
                        SV->eof=true;
                        WaitForSingleObject(endevent,2000);  ResetEvent(endevent);
                        SV->Free();

                        endevent=EN->endEvent;
                        EN->eof=true;
                        WaitForSingleObject(endevent,2000);  ResetEvent(endevent);
                        EN->Free();

                        endevent=RT->endEvent;
			RT->eof=true;
			WaitForSingleObject(endevent,2000);  ResetEvent(endevent);
			RT->Free();
			RT=NULL;

			CapDS->Release();
			CapDS=NULL;
		}
		return true;
}
//---------------------------------------------------------------------------


void __fastcall Tmp3record::Button3Click(TObject *Sender)
{
		Tsetupwindow *sw;
		sw=new Tsetupwindow(this);
		sw->ConfigNumber=ConfigNumber;
		sw->LoadCurrentSettings();
		sw->ShowModal();
		sw->Release();
		delete sw;
		LoadConfig(ConfigNumber);
}
//---------------------------------------------------------------------------

void __fastcall Tmp3record::SendAlert(AnsiString txt, int code)
{
	TAlerterThread *ta=new TAlerterThread();
	ta->ConfigNumber=ConfigNumber;
	if (email_do) {
		ta->email_from=email_from;
		ta->email_rcpt=email_rcpt;
		ta->email_smtp=email_smtp;
		ta->email_text=txt;
	} else {
        ta->email_smtp="";
    }
	if (http_do)
	{
		ta->http_url=http_url;
		ta->http_func_code=code;
	} else {
		ta->http_url="";
    }
	ta->Resume();
}

void __fastcall Tmp3record::Timer1Timer(TObject *Sender)
{
  if (Status==2) {
	pgl=((pgl<RT->leftvol) ? RT->leftvol : pgl+(RT->leftvol-pgl)*0.4);
	pgr=((pgr<RT->rightvol) ? RT->rightvol : pgr+(RT->rightvol-pgr)*0.4);
	VU->SetLEDs(pgl, pgr);

        if (RT->leftvol<1000 && RT->rightvol<1000  && Status==2)
        {
          if ( ((double)(Now())*86400-(double)LastSignal*86400 )>5)
          {
	        if (SignalIndicator->Caption=="" && Status==2) {
		        SendAlert("Az AutoREC csendet rögzít a következõ modulján: "+IntToStr(ConfigNumber),1);
	        }
	        SignalIndicator->Caption="NINCS JEL";
          }
        } else {
          LastSignal=Now();
          if ((RT->leftvol>32760 || RT->rightvol>32760) && Status==2)
          {
	        SignalIndicator->Caption="CSÚCS";
          } else {
		if (SignalIndicator->Caption=="NINCS JEL" && Status==2) {
			SendAlert("Az AutoREC hangot rögzít a következõ modulján: "+IntToStr(ConfigNumber),2);
		}
		SignalIndicator->Caption="";
          }
  }


  } else {
	VU->SetLEDs(0, 0);
  }

  if (Status==2) StatusIndicator->Caption="Rögzít";
  if (Status==0) StatusIndicator->Caption="Várakozik";
  if (Status==3) StatusIndicator->Caption="HIBA!";
}
//---------------------------------------------------------------------------

void __fastcall Tmp3record::Button1Click(TObject *Sender)
{
  Start();
}
//---------------------------------------------------------------------------

void __fastcall Tmp3record::Button2Click(TObject *Sender)
{
  Stop();
}
//---------------------------------------------------------------------------

void __fastcall Tmp3record::Button4Click(TObject *Sender)
{
  if (Status==2) SV->bStartNew=true;
}
//---------------------------------------------------------------------------

void __fastcall Tmp3record::Dirdelete(AnsiString dir, AnsiString limit)
{
	 TSearchRec drec;
     if (dir!="")
     {
        FindFirst(dir+"*.mp3", faAnyFile & !faDirectory, drec);
        while (drec.Name!="")
        {
                if (drec.Name<limit)
                {
						DeleteFile(dir+drec.Name);
                }
                drec.Name="";
                FindNext(drec);
        }
        FindClose(drec);
     }
}

void __fastcall Tmp3record::Timer2Timer(TObject *Sender)
{
  int tme=(int)((double)Time()*1440);
  if (LastTme!=tme)
  {
		int i;
        int ctm;
        for (i=0; i<Changes->Count; i++)
		{
          ctm=Changes->Strings[i].SubString(1,2).ToInt()*60+Changes->Strings[i].SubString(4,2).ToInt();
          if (tme==ctm && Status==2)
          {
                SV->bStartNew=true;
          }
        }
        LastTme=tme;
  }
  if ((tme % 1)==0 && deletedays>0 && Status==2)
  {
     AnsiString limit=FormatDateTime(filenameformat,(double)Now()-deletedays)+".mp3";
     Dirdelete(mp3dir, limit);
     Dirdelete(mp3dir2, limit);
  }
}
//---------------------------------------------------------------------------





