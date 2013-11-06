//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TForm1::StartuemClick(TObject *Sender)
{
    Graphics::TBitmap * bmpCurrent = new Graphics::TBitmap;
    int wb = 1;
    for (int i=0; i<8; i++)
    {
        wb = (wb + 1) % 2;
        for (int j=0; j<8; j++)
        {
                board->GetBitmap(wb*8 + rand()%8, bmpCurrent);
                Canvas->Draw(i*56, j*56, bmpCurrent);
                wb = (wb + 1) % 2;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        switch(Key)
        {
                case VK_LEFT : ShowMessage("1"); break;
                case VK_UP : ShowMessage("2"); break;
                case VK_RIGHT : ShowMessage("3");break;
                case VK_DOWN : ShowMessage("4"); break;
                case 27: Close(); break;
        }
}
//---------------------------------------------------------------------------
