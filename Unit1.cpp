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
struct WhereGo
{
  int d;
  int g;
};
struct Shape
{
  struct Point point;
  int imageIndex;
  int player;
  int shapeId;
  bool isActive;
  struct WhereGo whereGo;
};

struct Point cursor;
struct Shape shapes[2][16];

struct Point *isPosibleSteps = (struct Point *)malloc(32*sizeof(struct Point));
struct Point *isPosibleEats = (struct Point *)malloc(8*sizeof(struct Point));
int countOfPosible[2];

struct Shape * currentShape = NULL;
int currentPlayer = 1;

Graphics::TBitmap *backgroundLayer = new Graphics::TBitmap;
Graphics::TBitmap *shapesLayer = new Graphics::TBitmap;

Graphics::TBitmap *bmpCurrent;

int x, y; //readability ++

TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        DoubleBuffered=true;
        backgroundLayer->Width=448;
        backgroundLayer->Height=448;
        shapesLayer->Width=448;
        shapesLayer->Height=448;
        shapesLayer->Transparent=true;
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
                case VK_ESCAPE: Close(); break;
                case 82: Startuem->Visible=true; break;
                case VK_SPACE:
                        Shape * objectUnderCursor;
                        objectUnderCursor = GetObjectUnderCursor();
                        if (objectUnderCursor != NULL) //shape under cursor
                        {
                                if (objectUnderCursor->player == currentPlayer)
                                {
                                        if (currentShape == NULL)
                                        {
                                                currentShape = objectUnderCursor;
                                                GetPosibleSteps();
                                        }
                                        else
                                        {
                                                countOfPosible[0] = 0;
                                                countOfPosible[1] = 0;
                                                if (cursor.x == currentShape->point.x && cursor.y == currentShape->point.y)
                                                {
                                                        currentShape = NULL;
                                                }
                                                else
                                                {
                                                        currentShape = objectUnderCursor;
                                                        GetPosibleSteps();
                                                }
                                        }
                                }
                                else if (objectUnderCursor->player != currentPlayer && currentShape != NULL)
                                {
                                        for (int i=0; i<countOfPosible[1]; i++)
                                        {
                                                x = isPosibleEats[i].x;
                                                y = isPosibleEats[i].y;
                                                if (cursor.x == x && cursor.y == y)
                                                {
                                                        objectUnderCursor->isActive=false; //delete shape
                                                        MoveShape();
                                                }
                                        }
                                }
                        }
                        else
                        {
                                if (currentShape != NULL && currentShape->player == currentPlayer)
                                {
                                        for (int i=0; i<countOfPosible[0]; i++)
                                        {
                                                x = isPosibleSteps[i].x;
                                                y = isPosibleSteps[i].y;
                                                if (cursor.x == x && cursor.y == y)
                                                {
                                                        MoveShape();
                                                }
                                        }
                                }
                        }
        }
        Draw();
}


void __fastcall TForm1::StartuemClick(TObject *Sender)
{
        //hide button
        Startuem->Visible=false;
        Form1->Refresh();

        Initialize();

        RedrawShapes();
        Draw();
}


void TForm1::Initialize(void)
{
        //white player
        currentPlayer = 1;

        //cursor
        cursor.x = 0, cursor.y = 0;

        //desk
        int wb = 1;
        for (int i=0; i<8; i++)
        {
                wb = (wb + 1) % 2;
                for (int j=0; j<8; j++)
                {
                        bmpCurrent = new Graphics::TBitmap;
                        DeskSprites->GetBitmap(wb*8 + rand()%8, bmpCurrent);
                        backgroundLayer->Canvas->CopyRect(Rect(i*56,j*56,(i+1)*56,(j+1)*56), bmpCurrent->Canvas, Rect(0, 0, 56, 56));
                        wb = (wb + 1) % 2;
                        delete bmpCurrent;
                }
        }

        //shapes
        //pawns
        for (int i=0; i<2; i++)
        {
                for (int j=0; j<8; j++)
                {
                       shapes[i][j].point.x = j;
                       shapes[i][j].point.y = abs(7*i - 1);
                       shapes[i][j].imageIndex = 6*i;
                       shapes[i][j].isActive = true;
                       shapes[i][j].player = i;
                       shapes[i][j].shapeId = 0;
                }
        }

        //knights, bishops, rooks
        for (int i=0; i<2; i++)
        {
                for (int j=0; j<3; j++)
                {
                        shapes[i][8+j].point.x = j;
                        shapes[i][8+j].point.y = 7*i;
                        shapes[i][8+j].imageIndex = 6*i + 3 - j%3;
                        shapes[i][8+j].isActive = true;
                        shapes[i][8+j].player = i;
                        shapes[i][8+j].shapeId = 3 - j%3;
                        if (j == 2)
                        {
                                shapes[i][8+j].whereGo.d = 2;
                                shapes[i][8+j].whereGo.g = 0;
                        }
                        if (j == 0)
                        {
                                shapes[i][8+j].whereGo.d = 0;
                                shapes[i][8+j].whereGo.g = 2;
                        }
                }
                for (int j=0; j<3; j++)
                {
                        shapes[i][11+j].point.x = 7 - j;
                        shapes[i][11+j].point.y = 7*i;
                        shapes[i][11+j].imageIndex = 6*i + 3 - j%3;
                        shapes[i][11+j].isActive = true;
                        shapes[i][11+j].player = i;
                        shapes[i][11+j].shapeId = 3 - j%3;
                        if (j == 2)
                        {
                                shapes[i][11+j].whereGo.d = 2;
                                shapes[i][11+j].whereGo.g = 0;
                        }
                        if (j == 0)
                        {
                                shapes[i][11+j].whereGo.d = 0;
                                shapes[i][11+j].whereGo.g = 2;
                        }
                }
        }

        //kings, queens
        for (int i=0; i<2; i++)
        {
                for (int j=0; j<2; j++)
                {
                        shapes[i][14+j].point.x = 3 + j;
                        shapes[i][14+j].point.y = 7*i;
                        shapes[i][14+j].imageIndex = 6*i + 5 - (i+j)%2;
                        shapes[i][14+j].isActive = true;
                        shapes[i][14+j].player = i;
                        shapes[i][14+j].shapeId = 5 - (i+j)%2;
                        if ((i+j)%2 == 1)
                        {
                                shapes[i][14+j].whereGo.d = 2;
                                shapes[i][14+j].whereGo.g = 2;
                        }
                }
        }
}


void TForm1::Draw(void)
{
        //desk
        View->Canvas->CopyRect(Rect(0,0,448,448), backgroundLayer->Canvas, Rect(0, 0, 448, 448));

        //current shape
        if (currentShape != NULL)
        {
                x = currentShape->point.x;
                y = currentShape->point.y;
                View->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
        }

        //posible steps
        if (countOfPosible[0] != 0)
        {
                View->Canvas->Brush->Color=clGreen;
                for (int i=0; i < countOfPosible[0]; i++)
                {
                        x = isPosibleSteps[i].x;
                        y = isPosibleSteps[i].y;
                        View->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
                }
        }
        if (countOfPosible[1] != 0)
        {
                View->Canvas->Brush->Color=clRed;
                for (int i=0; i < countOfPosible[1]; i++)
                {
                        x = isPosibleEats[i].x;
                        y = isPosibleEats[i].y;
                        View->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
                }
        }

        //cursor
        x = cursor.x;
        y = cursor.y;
        View->Canvas->Brush->Color=clYellow;
        View->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));

        //shapes
        View->Canvas->Draw(0,0, shapesLayer);
}

void TForm1::RedrawShapes(void)
{
        //fill all
        shapesLayer->Canvas->Brush->Color=clSilver;
        shapesLayer->Canvas->FillRect(Rect(0, 0, 448, 448));

        for (int i=0; i<2; i++)
        {
                for (int j=0; j<16; j++)
                {
                        if (shapes[i][j].isActive == true)
                        {
                                bmpCurrent = new Graphics::TBitmap;

                                //shape
                                ShapesSprites->GetBitmap(shapes[i][j].imageIndex, bmpCurrent);
                                bmpCurrent->TransparentColor=clWhite;
                                bmpCurrent->Transparent=true;

                                //substrate
                                x = shapes[i][j].point.x;
                                y = shapes[i][j].point.y;
                                shapesLayer->Canvas->Brush->Color=clWhite;
                                shapesLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));

                                shapesLayer->Canvas->Draw(x*56, y*56, bmpCurrent);

                                delete bmpCurrent;
                        }
                }
        }
}

void TForm1::MoveShape(void)
{
        //move
        currentShape->point.x = cursor.x;
        currentShape->point.y = cursor.y;

        //upgrade
        if (currentShape->shapeId == 0 && currentShape->point.y == (7*((currentPlayer+1)%2)))
        {
                currentShape->shapeId = 4;
                currentShape->imageIndex = 6*currentPlayer + 4;
                currentShape->whereGo.d = 2;
                currentShape->whereGo.g = 2;
        }

        //fresh
        currentShape = NULL;
        currentPlayer = (currentPlayer+1) % 2;
        countOfPosible[0] = 0;
        countOfPosible[1] = 0;

        RedrawShapes();
}


Shape * TForm1::GetObjectUnderCursor(void)
{
        for (int i = 0; i < 2; i++)
        {
                for (int j = 0; j < 16; j++)
                {
                        if (shapes[i][j].isActive == true)
                        {
                                x = shapes[i][j].point.x;
                                y = shapes[i][j].point.y;
                                if (x == cursor.x && y == cursor.y)
                                {
                                        return &shapes[i][j];
                                }
                        }
                }
        }
        return NULL;
}

void TForm1::GetPosibleSteps(void)
{
        struct Point points[8];
        struct Point point;
        
        //left right up down \left \right /left /right
        struct Point stepDirection[8] = {{-1, 0},{1, 0},{0, -1},{0, 1},{-1, -1},{1, 1},{-1, 1}, {1, -1}};
        int posibleDirection[8];

        int index[2] = {0, 0};
        int rtrn;

        if (currentShape->shapeId == 0)
        {
                for (int i=0; i<2; i++)
                {
                        if ((abs(currentPlayer*7 - currentShape->point.y))*i < 2) //first step or not
                        {
                                point.x = currentShape->point.x;
                                point.y = currentShape->point.y + stepDirection[2].y*(1+i) - 2*((currentPlayer+1)%2)*stepDirection[2].y*(1+i); //mirror algoritm
                                rtrn = CheckPoint(point);
                                if (rtrn == -1) //empty cell
                                {
                                        isPosibleSteps[index[0]] = point;
                                        index[0] += 1;
                                }
                        }
                        point.x = currentShape->point.x + stepDirection[4+3*i].x;
                        point.y = currentShape->point.y + stepDirection[4+3*i].y - 2*((currentPlayer+1)%2)*stepDirection[4+3*i].y;
                        rtrn = CheckPoint(point);
                        if (rtrn != currentPlayer && rtrn != -1)
                        {
                                isPosibleEats[index[1]] = point;
                                index[1] += 1;
                        }
                }
        }
        else if (currentShape->shapeId == 2)
        {
                struct Point directionStep[8] = {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,1}};
                for (int i=0; i<8; i++)
                {
                        point.x = currentShape->point.x + directionStep[i].x;
                        point.y = currentShape->point.y + directionStep[i].y;

                        rtrn = CheckPoint(point);
                        if (rtrn == -1)
                        {
                                isPosibleSteps[index[0]] = point;
                                index[0] += 1;
                        }
                        else if (rtrn != currentPlayer)
                        {
                                isPosibleEats[index[1]] = point;
                                index[1] += 1;
                        }
                }
        }
        else if (currentShape->shapeId == 5)
        {
                for (int j=0; j<8; j++)
                {
                        point.x = currentShape->point.x + stepDirection[j].x;
                        point.y = currentShape->point.y + stepDirection[j].y;

                        if (point.x < 0 || point.x > 8 || point.y < 0 || point.y > 7)
                        {
                                continue;
                        }

                        rtrn = CheckPoint(point);
                        if (rtrn == -1)
                        {
                                isPosibleSteps[index[0]] = point;
                                index[0] += 1;
                        }
                        else if (rtrn != currentPlayer)
                        {
                                isPosibleEats[index[1]] = point;
                                index[1] += 1;
                        }
                }
        }
        else
        {
                for (int i=0; i<8; i++)
                {
                        if ((currentShape->shapeId == 3 || currentShape->shapeId == 4)&& i < 4)
                        {
                                posibleDirection[i] = 1;
                        }
                        else if ((currentShape->shapeId == 1 || currentShape->shapeId == 4) && i > 3)
                        {
                                posibleDirection[i] = 1;
                        }
                        else
                        {
                                posibleDirection[i] = 0;
                        }
                }
                for (int i=1; i<8; i++)
                {
                        for (int j=0; j<8; j++)
                        {
                                if (posibleDirection[j] == 1)
                                {
                                        point.x = currentShape->point.x + stepDirection[j].x*i;
                                        point.y = currentShape->point.y + stepDirection[j].y*i;
                                        points[j] = point;
                                }
                        }
                        
                        //check points
                        for (int j=0; j<8; j++)
                        {
                                if (points[j].x < 0 || points[j].x > 8 || points[j].y < 0 || points[j].y > 7)
                                {
                                        posibleDirection[j] = 0;
                                        continue;
                                }
                                if (posibleDirection[j] == 1)
                                {
                                        rtrn = CheckPoint(points[j]);
                                        if (rtrn == -1)
                                        {
                                                isPosibleSteps[index[0]] = points[j];
                                                index[0] += 1;
                                        }
                                        else
                                        {
                                                if (rtrn != currentPlayer)
                                                {
                                                        isPosibleEats[index[1]] = points[j];
                                                        index[1] += 1;
                                                }
                                                posibleDirection[j] = 0;
                                        }
                                }
                        }
                }
        }
        countOfPosible[0] = index[0];
        countOfPosible[1] = index[1];
}


int TForm1::CheckPoint(struct Point point)
{
        for (int i = 0; i < 2; i++)
        {
                for (int j = 0; j < 16; j++)
                {
                        if (shapes[i][j].isActive == true)
                        {
                                if (shapes[i][j].point.x == point.x && shapes[i][j].point.y == point.y)
                                {
                                        return shapes[i][j].player;
                                }
                        }
                }
        }
        return -1;
}
