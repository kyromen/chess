//---------------------------------------------------------------------------

#include <vcl.h>
#include <winbase.h>
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

__fastcall Comm::Comm(bool CreateSuspended)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall Comm::Execute()
{
        AnsiString Port = "COM4"; 
        COMMCONFIG CommConfig;
        DWORD dwSize = sizeof(CommConfig);

        GetDefaultCommConfig
        (
                Port.c_str(),
                &CommConfig,
                &dwSize
        );
        AnsiString Mode = Port+":4800,n,8,1";
        BuildCommDCB(Mode.c_str(), &CommConfig.dcb);
        CommConfig.dcb.fBinary = TRUE;
        CommConfig.dcb.fDtrControl = DTR_CONTROL_ENABLE;
        CommConfig.dcb.fOutX = FALSE;
        CommConfig.dcb.fInX = FALSE;

        HANDLE  HandlerCom = CreateFile
        (
        Port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,    // exclusive access
        NULL, // no security attrs
        OPEN_EXISTING,
        0, /* not overlapped I/O */
        NULL
        );

        SetCommState(HandlerCom, &CommConfig.dcb);

        EscapeCommFunction
        (
        HandlerCom,      // handle to communications device
        SETDTR            // extended function to perform
        );

        char data = 0;
        DWORD actlen = 1;
        BOOL ret;

        while (1)
        {
         if (ReadFile( HandlerCom, &data, 1, &actlen, NULL ) && actlen)
        {
                if (MyMethod!=NULL) this->MyMethod(this, int(data));
                getch();
        }
  }


  // ???????? ????? 
        /*HANDLE hCom;
        char *pcCommPort = "COM3:4800,S,7,1,CSO,DSO,CD0,LF";

        hCom = CreateFile( pcCommPort,
                    GENERIC_READ,
                    0,    // must be opened with exclusive-access
                    NULL, // no security attributes
                    OPEN_EXISTING, // must use OPEN_EXISTING
                    FILE_ATTRIBUTE_READONLY,    // not overlapped I/O
                    NULL  // hTemplate must be NULL for comm devices
                    );

        if (hCom == INVALID_HANDLE_VALUE)
        {
        // Handle the error.
                printf("CreateFile failed with error %d.\n", GetLastError());
                return ;
        }

        /*COMMTIMEOUTS ct;
        ct.ReadIntervalTimeout = MAXDWORD;
        ct.ReadTotalTimeoutConstant = 0;
        ct.ReadTotalTimeoutMultiplier = 0;
        ct.WriteTotalTimeoutConstant = 0;
        ct.WriteTotalTimeoutMultiplier = 0;
        if (!SetCommTimeouts (hCom, &ct)) printf ("failed to set to\n");
                    */
        //getch();

        /*char simbol;
        DWORD nRead = 1;
        while (1)
        {
                while (ReadFile (hCom, &simbol, 1, &nRead, NULL) && nRead)
                {
                        if (MyMethod!=NULL) this->MyMethod(this, (int)simbol);
                }
        }*/
}
//---------------------------------------------------------------------------
