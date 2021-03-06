//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "proba_form1.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PERFGRAP"
#pragma resource "*.dfm"

#include "grtdxutils.h"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        BE_CONFIG bc;
        bc.dwConfig=BE_CONFIG_MP3;
        bc.format.mp3.dwSampleRate=44100;
        bc.format.mp3.byMode=BE_MP3_MODE_STEREO;
        bc.format.mp3.wBitrate=160;
        bc.format.mp3.bCopyright=false;
        bc.format.mp3.bCRC=false;
        bc.format.mp3.bOriginal=false;
        bc.format.mp3.bPrivate=false;
        beInitStream(&bc, &dwInputBufferLength, &dwOutputBufferLength, &hStream);
        Mp3Buffer=new char[dwOutputBufferLength];
        InputBuffer=new short[dwInputBufferLength];
        fout=fopen("c:\\proba.mp3","wb");
}
//---------------------------------------------------------------------------

STDMETHODIMP IMySGCB::SampleCB( double SampleTime, IMediaSample * pSample )
{
  unsigned char p;
  pSample->GetTime(&Form1->starttime,&Form1->endtime);
  int bfsize;
  bfsize=pSample->GetActualDataLength();
  if (bfsize>0)
  {
    if (Form1->StaticText1->Color==0x0)
    {
      Form1->StaticText1->Color=0x0000FF;
    } else {
      Form1->StaticText1->Color=0x000000;
    }
    Form1->bufsize=bfsize;
    Form1->SG->GetCurrentBuffer(&Form1->bufsize, NULL);
    Form1->SG->GetCurrentBuffer(&Form1->bufsize, (long*)&Form1->buffer);
//---------------------------------------------------------------------------
    int i=0;
    while (i<Form1->bufsize/2)
    {
        Form1->InputBuffer[Form1->dwInputBufPos]=Form1->buffer[i];

        i++;
        Form1->dwInputBufPos++;

        if (Form1->dwInputBufPos==Form1->dwInputBufferLength)
        {
                DWORD dwOutBufLen=0;
                Form1->dwInputBufPos=0;
                if (beEncodeChunk(Form1->hStream, Form1->dwInputBufferLength,
                              Form1->InputBuffer, (BYTE*)Form1->Mp3Buffer, &dwOutBufLen)==BE_ERR_SUCCESSFUL)
                {
                        Form1->Caption="MP3CODING";
                } else {
                        Form1->Caption="ERR";
                }
                fwrite(Form1->Mp3Buffer,1,dwOutBufLen,Form1->fout);
        }
    }
//---------------------------------------------------------------------------
  }
  return 0;
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  WideString w;
  IBaseFilter *BF;

  MC->StopWhenReady();

//  Source=CreateBaseFilterByCatAndName(CLSID_AudioInputDeviceCategory,ListBox1->Items->Strings[ListBox1->ItemIndex]);
  FL->AddFilter(Source,L"Source");

  BF=CreateBaseFilterByCLSID(CLSID_SampleGrabber);
  BF->QueryInterface(IID_ISampleGrabber,(void**)&SG);
  FL->AddFilter(BF,L"SampleGrabber");
  SG->SetOneShot(false);
  SG->SetBufferSamples(true);

  pSGCB=new IMySGCB();
  SG->SetCallback(pSGCB,0);

  FL->Connect(FindFreeOutputPin(Source),    FindFreeInputPin(BF));

  MC->Run();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  ICreateDevEnum *SysDev;
  IEnumMoniker *Enum;
  IMoniker *aMoniker;
  IPropertyBag *pbag;
  VARIANT s;
  AnsiString ss;
  IBaseFilter *result;

//  ListBox1->Clear();
  CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&SysDev);
  SysDev->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &Enum, 0);

  result=NULL;
  if (Enum!=NULL)
  {
    while (Enum->Next(1,&aMoniker,NULL)==S_OK)
    {
      if (aMoniker!=NULL)
      {
        aMoniker->BindToStorage(NULL,NULL,IID_IPropertyBag, (void**)&pbag);
        s.vt=VT_BSTR;
        pbag->Read(L"FriendlyName",&s,NULL);
        ss=s.bstrVal;
//        ListBox1->Items->Add(ss);
      }
    }
    Enum->Release();
  }
  SysDev->Release();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&FL);
  FL->QueryInterface(IID_IMediaControl, (void**)&MC);
  FL->QueryInterface(IID_IMediaSeeking, (void**)&MS);
  B=new Graphics::TBitmap();
  B->Width=200;
  B->Height=30;
  mupr=32768;
  mupl=32768;
  pgr=32768;
  pgl=32768;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  __int64 now,max;
  double temp;
  long state;
  int i,j,lft,rht;
  int *integer;
  IMediaSample *IMP;
  float temp1;
  int bn = 0;

  MC->GetState(0,&state);
  lft=0;
  rht=0;
  B->Canvas->Pen->Color=0x000000;
  B->Canvas->Brush->Color=0x000000;
  B->Canvas->Rectangle(0,0,400,30);
  {
    if (bufsize>0)
    {
      for (i=0;i<bufsize/4;i++)
      {
        if (i>(bufsize/4)-200)
        {
          if (abs(buffer[i*2])>lft)   lft=abs(buffer[i*2]);
          if (abs(buffer[i*2+1])>rht) rht=abs(buffer[i*2+1]);
        }
      }
    }
  }
  pgl=((pgl<lft) ? pgl+(lft-pgl)*0.2 : pgl+(lft-pgl)*0.05);
  pgr=((pgr<rht) ? pgr+(rht-pgr)*0.2 : pgr+(rht-pgr)*0.05);

  int imax=190;
  for (i=imax;i>0;i=i-2)
  {
    int ucolor = 0x00ffff;
    int ncolor;
    ncolor=0x003300;
    ucolor=0x00ff00;
    if (i*90/imax>65) { ncolor=0x003333; ucolor=0x00ffff; }
    if (i*90/imax>80) { ncolor=0x000033; ucolor=0x0000ff; }

    B->Canvas->Pen->Color=(((pgl*imax/32768)>i) ? ucolor : ncolor );
    B->Canvas->MoveTo(5+i,4);
    B->Canvas->LineTo(5+i,14);
    B->Canvas->Pen->Color=(((pgr*imax/32768)>i) ? ucolor : ncolor );
    B->Canvas->MoveTo(5+i,16);
    B->Canvas->LineTo(5+i,26);
  }
  Canvas->Draw(370,10,B);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  MC->Stop();
  DWORD dwNumberOfSamplesEncoded;
  beDeinitStream(hStream,(BYTE*)Mp3Buffer,&dwNumberOfSamplesEncoded);
  beCloseStream(hStream);
  delete Mp3Buffer;
  delete InputBuffer;
}
//---------------------------------------------------------------------------


