//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Comm::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall Comm::Comm(bool CreateSuspended, HANDLE handle)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall Comm::Execute()
{
        _COMSTAT comstat;
        int n;
        while (!Terminated)
        {
                Sleep(1);
                ClearCommError();
                n = comstat.cbInQue
                if (n != 0)
                {
                }
        }
        //---- Place thread code here ----
}
//---------------------------------------------------------------------------
 