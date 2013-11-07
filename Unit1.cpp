//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
struct Point
{
  int x;
  int y;
};
struct Shape
{
  struct Point point;
  int imageIndex;
};
struct Point cursor;
struct Shape shapes[2][16];
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
                case VK_LEFT : if (cursor.x > 0){cursor.x-=1;} break;
                case VK_UP : if (cursor.y > 0){cursor.y-=1;} break;
                case VK_RIGHT : if (cursor.x < 7){cursor.x+=1;} break;
                case VK_DOWN : if (cursor.y < 7){cursor.y+=1;} break;
                case 82: Startuem->Visible=true; break;
                case 27: Close(); break;
        }
        Draw();
}
//---------------------------------------------------------------------------


void TForm1::Draw(void)
{
        Graphics::TBitmap ** bmpCurrent = new Graphics::TBitmap*[64];
        Graphics::TBitmap ** bmpShapes = new Graphics::TBitmap*[32];
        int wb = 1;
        for (int i=0; i<8; i++)
        {
                wb = (wb + 1) % 2;
                for (int j=0; j<8; j++)
                {
                        bmpCurrent[i*8 + j] = new Graphics::TBitmap;
                        deskSprites->GetBitmap(desk[i][j], bmpCurrent[i*8 + j]);
                        Canvas->Draw(i*56, j*56, bmpCurrent[i*8 + j]);
                        wb = (wb + 1) % 2;
                }
        }
        //������
        for (int i=0; i<2; i++)
        {
                for (int j=0; j<14; j++)
                {
                        bmpShapes[i*14 + j] = new Graphics::TBitmap;
                        shapesSprites->GetBitmap(shapes[i][j].imageIndex, bmpShapes[i*14 + j]);
                        Canvas->Draw(shapes[i][j].point.x*56, shapes[i][j].point.y*56, bmpShapes[i*14 + j]);
                }
        }
        Form1->Canvas->Brush->Color=clYellow;
        Form1->Canvas->FillRect(Rect(cursor.x*56, cursor.y*56, (cursor.x+1)*56, (cursor.y+1)*56));
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

        for (int i=0; i<2; i++)
        {
                for (int j=0; j<8; j++)
                {
                        shapes[i][j].point.x = j;
                        shapes[i][j].point.y = abs(7*i - 1);
                        shapes[i][j].imageIndex = 6*i;
                }
        }

        for (int i=0; i<2; i++)
        {
                for (int j=0; j<3; j++)
                {
                        shapes[i][8+j].point.x = j;
                        shapes[i][8+j].point.y = 7*i;
                        shapes[i][8+j].imageIndex = 6*i + 3 - j%3;
                }
                for (int j=0; j<3; j++)
                {
                        shapes[i][11+j].point.x = 7 - j;
                        shapes[i][11+j].point.y = 7*i;
                        shapes[i][11+j].imageIndex = 6*i + 3 - j%3;
                }
        }
        cursor.x = 0, cursor.y = 0;
        Draw();
}
//---------------------------------------------------------------------------


