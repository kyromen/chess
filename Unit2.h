//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class Comm : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        TNotifyEvent MyMethod;
        __fastcall Comm(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
 