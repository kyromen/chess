//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include<conio.h>
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TInefMethod) (System::TObject* Sender, int a);

class Comm : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        TInefMethod MyMethod;
        __fastcall Comm(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
 