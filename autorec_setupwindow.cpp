//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "autorec_setupwindow.h"

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
#include <Registry.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cdiroutl"
#pragma resource "*.dfm"
Tsetupwindow *setupwindow;
//---------------------------------------------------------------------------
__fastcall Tsetupwindow::Tsetupwindow(TComponent* Owner)
        : TForm(Owner)
{
	setupwindow=this;
}
//---------------------------------------------------------------------------
__fastcall Tsetupwindow::LoadCurrentSettings()
{
  TRegistry *R;
  AnsiString setupNum;

  try
  {
        R=new TRegistry;
        R->RootKey=HKEY_LOCAL_MACHINE;
        R->OpenKey("Software\\GRT\\AutoREC\\" + IntToStr(ConfigNumber), true);
		if (R->ValueExists("CaptureDeviceIndex"))
		{
				destdir->Text=R->ReadString("DestinationDirectory");
				destdir2->Text=R->ReadString("DestinationDirectory2");
				DevCombo->ItemIndex=R->ReadInteger("CaptureDeviceIndex");
				SampleRate->ItemIndex=R->ReadInteger("SampleRateIndex");
				BitRate->ItemIndex=R->ReadInteger("BitRateIndex");
				AutoDelete->Position=R->ReadInteger("AutoDelete");
				int count=R->ReadInteger("FileChangeCount");
				int i;
				FileChanges->Clear();
				for (i=0; i<count; i++)
				{
						FileChanges->Items->Add(R->ReadString("FileChange"+IntToStr(i)));
				}

				email_do->Checked=R->ReadBool("email_do");
				email_rcpt->Text=R->ReadString("email_rcpt");
				email_from->Text=R->ReadString("email_from");
				email_smtp->Text=R->ReadString("email_smtp");

				http_do->Checked=R->ReadBool("http_do");
				http_url->Text=R->ReadString("http_url");
		}
  }
  catch (...)
  {
  }
  R->Free();
}

int callbackcnt=0;

BOOL CALLBACK DSEnumCallback(LPGUID  lpGuid, LPCSTR  lpcstrDescription,
									 LPCSTR  lpcstrModule, LPVOID  lpContext)
{
  AnsiString s;
  s=lpcstrDescription;
  s=s+" (";
  s=s+lpcstrModule+")";
  setupwindow->DevCombo->Items->Add(s);
  setupwindow->sCardGUIDS[callbackcnt]=lpGuid;
  callbackcnt++;
  s="";
}

void __fastcall Tsetupwindow::FormCreate(TObject *Sender)
{
  ICreateDevEnum *SysDev;
  IEnumMoniker *Enum;
  IMoniker *aMoniker;
  IPropertyBag *pbag;
  VARIANT s;
  AnsiString ss;
  IBaseFilter *result;

  CoInitialize(NULL);

  DevCombo->Clear();
  callbackcnt=0;
  DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)(&DSEnumCallback), NULL);
  DevCombo->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall Tsetupwindow::Button3Click(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------

void __fastcall Tsetupwindow::Button2Click(TObject *Sender)
{
  TRegistry *R;
  AnsiString setupNum;

  try
  {
        R=new TRegistry;
        R->RootKey=HKEY_LOCAL_MACHINE;
        R->OpenKey("Software\\GRT\\AutoREC\\" + IntToStr(ConfigNumber), true);
        R->WriteString("DestinationDirectory", destdir->Text);
        R->WriteString("DestinationDirectory2", destdir2->Text);        
		R->WriteString("CaptureDevice", DevCombo->Items->Strings[DevCombo->ItemIndex]);
		if (sCardGUIDS[DevCombo->ItemIndex]!=NULL) {
			R->WriteString("CaptureDeviceGuid", Sysutils::GUIDToString(*sCardGUIDS[DevCombo->ItemIndex]));
		} else {
			R->WriteString("CaptureDeviceGuid", "");
        }
		R->WriteInteger("CaptureDeviceIndex", DevCombo->ItemIndex);
		R->WriteInteger("SampleRate", StrToInt(SampleRate->Items->Strings[SampleRate->ItemIndex]));
        R->WriteInteger("SampleRateIndex", SampleRate->ItemIndex);
        R->WriteInteger("BitRate", StrToInt(BitRate->Items->Strings[BitRate->ItemIndex]));
		R->WriteInteger("BitRateIndex", BitRate->ItemIndex);
		R->WriteInteger("AutoDelete", AutoDelete->Position);
		int i;
		R->WriteInteger("FileChangeCount",FileChanges->Items->Count);
		for (i=0; i<FileChanges->Items->Count; i++)
		{
				R->WriteString("FileChange"+IntToStr(i), FileChanges->Items->Strings[i]);
		}
		R->WriteBool("email_do", email_do->Checked);
		R->WriteString("email_rcpt", email_rcpt->Text);
		R->WriteString("email_from", email_from->Text);
		R->WriteString("email_smtp", email_smtp->Text);

		R->WriteBool("http_do", http_do->Checked);
		R->WriteString("http_url", http_url->Text);
  }
  __finally
  {
        R->Free();
        Close();        
  }
}

//---------------------------------------------------------------------------

AnsiString addZero(AnsiString s)
{
  while (s.Length()<2) s="0"+s;
  return s;
}

void __fastcall Tsetupwindow::Button4Click(TObject *Sender)
{
  FileChanges->Items->Add(addZero(Hour->Text)+":"+addZero(Minute->Text));
}
//---------------------------------------------------------------------------


void __fastcall Tsetupwindow::FileChangesKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_DELETE && FileChanges->ItemIndex!=-1)
  {
        FileChanges->Items->Delete(FileChanges->ItemIndex);
  }
}
//---------------------------------------------------------------------------







