//---------------------------------------------------------------------------


#pragma hdrstop

#include "ds1_mp3savethread.h"
#include "stdio.h"

//---------------------------------------------------------------------------

__fastcall tSaveThread::tSaveThread(tCircularBuffer *pbuffer): TThread(true)
{
  streambuffer=pbuffer;
  eof=false;
  bStartNew=true;
  err=false;
  endEvent=NULL;
}

void __fastcall tSaveThread::Execute()
{
  unsigned char p;
  DWORD bfsize;
  char *buffer;
  FILE *fout1=NULL;
  FILE *fout2=NULL;

  streambuffer->in_event_id=CreateEvent(NULL,true,false,NULL);
  do
  {
        WaitForSingleObject(streambuffer->in_event_id,1000);
        ResetEvent(streambuffer->in_event_id);
        bfsize=streambuffer->nowInBuffer;
        if (bfsize>0)
        {
                buffer=new char[bfsize];
                streambuffer->getData(buffer, bfsize);

                if (bStartNew)
                {
                        AnsiString filename=FormatDateTime(filenameformat,Now());
                        AnsiString filename1;
                        AnsiString filename2;
                        filename1=Directory1+filename+".mp3";
                        filename2=Directory2+filename+".mp3";

                        if (fout1!=NULL) fclose(fout1);
                        if (fout2!=NULL) fclose(fout2);

                        fout1=fopen(filename1.c_str(),"wb");
                        if (Directory2!="") { fout2=fopen(filename2.c_str(),"wb"); } else { fout2=NULL; }
                        filename="";
                        filename1="";
                        filename2="";
                        bStartNew=false;
                }

                if (fout1!=NULL)
                {
                        if (fwrite(buffer,1,bfsize,fout1)<bfsize)
                        {
                                fclose(fout1);
                                fout1=NULL;
                                err=true;
                        }
                }
                if (fout2!=NULL)
                {
                        if (fwrite(buffer,1,bfsize,fout2)<bfsize)
                        {
                                fclose(fout2);
                                fout2=NULL;
                                err=true;
                        }
                }
                delete buffer;
        }
  } while (! eof);
  if (fout1!=NULL) fclose(fout1);
  if (fout2!=NULL) fclose(fout2);
  if (endEvent!=NULL) SetEvent(endEvent);
}

__fastcall tSaveThread::~tSaveThread()
{
}


#pragma package(smart_init)


