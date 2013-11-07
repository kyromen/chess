//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
int x,y;
int desk[8][8];
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}


void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        switch(Key)
        {
                case VK_LEFT : if (x > 0){x-=1;} break;
                case VK_UP : if (y > 0){y-=1;} break;
                case VK_RIGHT : if (x < 7){x+=1;} break;
                case VK_DOWN : if (y < 7){y+=1;} break;
                case 82: Startuem->Visible=true; break;
                case 27: Close(); break;
        }
        Draw();
}
//---------------------------------------------------------------------------


void TForm1::Draw(void)
{
        Graphics::TBitmap * bmpCurrent = new Graphics::TBitmap;
        int wb = 1;
        for (int i=0; i<8; i++)
        {
                wb = (wb + 1) % 2;
                for (int j=0; j<8; j++)
                {
                deskSprites->GetBitmap(desk[i][j], bmpCurrent);
                Canvas->Draw(i*56, j*56, bmpCurrent);
                wb = (wb + 1) % 2;
                }
        }
        Form1->Canvas->Brush->Color=clYellow;
        Form1->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
}

void __fastcall TForm1::StartuemClick(TObject *Sender)
{
        Startuem->Visible=false;
        int wb = 1;
        for (int i=0; i<8; i++)
        {
                wb = (wb + 1) % 2;
                for (int j=0; j<8; j++)
                {
                        desk[i][j] = wb*8 + rand()%8;
                        wb = (wb + 1) % 2;
                }
        }
        x = 0, y = 0;
        Draw();
}
//---------------------------------------------------------------------------


