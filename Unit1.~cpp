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
  int player;
  int shapeId;
  bool isActive;
};

struct Point cursor;
struct Shape shapes[2][16];

struct Point *possibleSteps = (struct Point *)malloc(32*sizeof(struct Point));
struct Point *possibleEats = (struct Point *)malloc(8*sizeof(struct Point));
int countOfPossible[2];
bool castling;

struct Shape * currentShape = NULL;
int currentPlayer = 1;

bool isShah = false;
bool isStandoff = false;
bool isWin = false;
bool firstStep[2][3];

Graphics::TBitmap *backgroundLayer = new Graphics::TBitmap;
Graphics::TBitmap *shapesLayer = new Graphics::TBitmap;
Graphics::TBitmap *transferLayer = new Graphics::TBitmap;
Graphics::TBitmap *resizeLayer = new Graphics::TBitmap;
Graphics::TBitmap *popupList = new Graphics::TBitmap;
Graphics::TBitmap *popup = new Graphics::TBitmap;
Graphics::TBitmap *bmpCurrent;

int side = 448;
int x, y; //readability++

Comm *threadCom = new Comm(True);

TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        threadCom->MyMethod = ReadProc;
        threadCom->Resume();
        DoubleBuffered = true;
        Form1->ClientWidth = 448;
        Form1->ClientHeight = 448;
        backgroundLayer->Width = 448;
        backgroundLayer->Height = 448;
        shapesLayer->Width = 448;
        shapesLayer->Height = 448;
        shapesLayer->Transparent = true;
        transferLayer->Width = 448;
        transferLayer->Height = 448;
        resizeLayer->Width = 448;
        resizeLayer->Width = 448;
        popupList->LoadFromFile("graphics/popup.bmp");
        popupList->TransparentColor = clBlack;
        popupList->Transparent = true;
        popup->Width = 190;
        popup->Height = 81;
        popup->Transparent = true;
        NewGame();
}

void __fastcall TForm1::CreateParams(TCreateParams &Params)
{
        //form without title
        TForm::CreateParams(Params);
        Params.Style &= ~WS_DLGFRAME;
        Params.Style |= WS_POPUP;
}

//moveable form
void __fastcall TForm1::ViewMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        if(Button == mbLeft)
        {
                long SC_DRAGMOVE = 0xF012;
                ReleaseCapture();
                SendMessage(Form1->Handle, WM_SYSCOMMAND, SC_DRAGMOVE, 0);
        }
}

void __fastcall TForm1::ReadProc(TObject *Sender)
{
        int a = 0;
}

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        switch(Key)
        {
                case VK_LEFT : if (cursor.x > 0){cursor.x -= 1;} break;
                case VK_UP : if (cursor.y > 0){cursor.y -= 1;} break;
                case VK_RIGHT : if (cursor.x < 7){cursor.x += 1;} break;
                case VK_DOWN : if (cursor.y < 7){cursor.y += 1;} break;
                case VK_ESCAPE: Close(); break;
                case 82: NewGame(); break;
                case VK_SPACE:
                        Shape * objectUnderCursor;
                        objectUnderCursor = GetObjectFromPoint(cursor);
                        if (objectUnderCursor != NULL) //shape under cursor
                        {
                                if (objectUnderCursor->player == currentPlayer)
                                {
                                        if (currentShape == NULL)
                                        {
                                                currentShape = objectUnderCursor;
                                                CheckSteps();
                                        }
                                        else
                                        {
                                                countOfPossible[0] = 0;
                                                countOfPossible[1] = 0;
                                                if (cursor.x == currentShape->point.x && cursor.y == currentShape->point.y && !castling)
                                                {
                                                        currentShape = NULL;
                                                }
                                                else if (castling && objectUnderCursor->shapeId == 5) {MoveShape();}
                                                else if (castling && currentShape->shapeId == 3) {currentShape = NULL; castling = false;}
                                                else {currentShape = objectUnderCursor; CheckSteps();}
                                        }
                                }
                                else if (objectUnderCursor->player != currentPlayer && currentShape != NULL)
                                {
                                        for (int i=0; i<countOfPossible[1]; i++)
                                        {
                                                x = possibleEats[i].x;
                                                y = possibleEats[i].y;
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
                                        for (int i=0; i<countOfPossible[0]; i++)
                                        {
                                                x = possibleSteps[i].x;
                                                y = possibleSteps[i].y;
                                                if (cursor.x == x && cursor.y == y)
                                                {
                                                        MoveShape();
                                                }
                                        }
                                }
                        }
        }
        Draw();
        isShah = false;
}

void TForm1::NewGame()
{
        //white player
        currentPlayer = 1;
        
        isWin = false;
        isShah = false;
        isStandoff = false;
        castling = false;
        for (int i=0; i<2; i++)
        {
                for (int j=0; j<3; j++)
                {
                        firstStep[i][j] = false;
                }
        }

        //cursor
        cursor.x = 3, cursor.y = 4;

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
                }
                for (int j=0; j<3; j++)
                {
                        shapes[i][11+j].point.x = 7 - j;
                        shapes[i][11+j].point.y = 7*i;
                        shapes[i][11+j].imageIndex = 6*i + 3 - j%3;
                        shapes[i][11+j].isActive = true;
                        shapes[i][11+j].player = i;
                        shapes[i][11+j].shapeId = 3 - j%3;
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
                }
        }

        RedrawShapes();
        Draw();
}


void TForm1::Draw()
{
        //desk
        transferLayer->Canvas->CopyRect(Rect(0,0,448,448), backgroundLayer->Canvas, Rect(0, 0, 448, 448));

        //current shape
        if (currentShape != NULL)
        {
                x = currentShape->point.x;
                y = currentShape->point.y;
                transferLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
        }

        //posible steps
        if (countOfPossible[0] != 0)
        {
                transferLayer->Canvas->Brush->Color=clGreen;
                for (int i=0; i<countOfPossible[0]; i++)
                {
                        x = possibleSteps[i].x;
                        y = possibleSteps[i].y;
                        transferLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
                }
        }
        if (countOfPossible[1] != 0)
        {
                transferLayer->Canvas->Brush->Color=clRed;
                for (int i=0; i<countOfPossible[1]; i++)
                {
                        x = possibleEats[i].x;
                        y = possibleEats[i].y;
                        transferLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
                }
        }

        //castling
        if (castling && !firstStep[currentPlayer][0])
        {
                transferLayer->Canvas->Brush->Color=clNavy;
                x = shapes[currentPlayer][14+currentPlayer].point.x;
                y = shapes[currentPlayer][14+currentPlayer].point.y;
                transferLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));
        }

        //cursor
        x = cursor.x;
        y = cursor.y;
        transferLayer->Canvas->Brush->Color=clYellow;
        transferLayer->Canvas->FillRect(Rect(x*56, y*56, (x+1)*56, (y+1)*56));

        //shapes
        transferLayer->Canvas->Draw(0,0, shapesLayer);

        //popup
        if (isWin) {
                popup->Canvas->CopyRect(Rect(0,0,190,81), popupList->Canvas, Rect(0,0*81,190,1*81));
        }
        else if (isShah) {
                popup->Canvas->CopyRect(Rect(0,0,190,81), popupList->Canvas, Rect(0,1*81,190,2*81));
        }
        else if (isStandoff) {
                popup->Canvas->CopyRect(Rect(0,0,190,81), popupList->Canvas, Rect(0,2*81,190,3*81));

        }
        if (isWin || isShah || isStandoff) {transferLayer->Canvas->Draw(448/2-190/2,448/2-81/2,popup);}

        transferLayer->Canvas->CopyRect(Rect(0,0,side,side),transferLayer->Canvas,Rect(0,0,448,448));

        //all on view
        View->Canvas->Draw(0, 0, transferLayer);
}

void TForm1::RedrawShapes()
{
        //fill bitmap
        shapesLayer->Canvas->Brush->Color=clSilver;
        shapesLayer->Canvas->FillRect(Rect(0, 0, 448, 448));

        for (int i=0; i<2; i++)
        {
                for (int j=0; j<16; j++)
                {
                        if (shapes[i][j].isActive == true)
                        {
                                bmpCurrent = new Graphics::TBitmap;
                                
                                ShapesSprites->GetBitmap(shapes[i][j].imageIndex, bmpCurrent);
                                bmpCurrent->TransparentColor=clSilver;
                                bmpCurrent->Transparent=true;
                                x = shapes[i][j].point.x;
                                y = shapes[i][j].point.y;
                                shapesLayer->Canvas->Draw(x*56, y*56, bmpCurrent);
                                delete bmpCurrent;
                        }
                }
        }
}

void TForm1::MoveShape()
{
        //castling
        if (!firstStep[currentPlayer][0] && currentShape->shapeId == 3)
        {
                if (castling && cursor.x == shapes[currentPlayer][14+currentPlayer].point.x)
                {
                        shapes[currentPlayer][14+currentPlayer].point.x = 3 - (currentPlayer + 1)%2 + 2*currentShape->point.x/7;
                        firstStep[currentPlayer][0] = true;
                }
                firstStep[currentPlayer][1+currentShape->point.x/7] = true;
        }
        else if (currentShape->shapeId == 5) {firstStep[currentPlayer][0] = true;}

        //move
        currentShape->point.x = cursor.x;
        currentShape->point.y = cursor.y;

        //upgrade pawn
        y = 7*((currentPlayer+1)%2);
        if (currentShape->shapeId == 0 && currentShape->point.y == y)
        {
                currentShape->shapeId = 4;
                currentShape->imageIndex = 6*currentPlayer + 4;
        }

        currentPlayer = (currentPlayer+1) % 2;
        currentShape = (struct Shape *)malloc(sizeof(struct Shape));

        RedrawShapes();

        CheckShah();
        CheckWinOrStandoff();

        castling = false;
        currentShape = NULL;
        countOfPossible[0] = 0;
        countOfPossible[1] = 0;
}

void TForm1::GetPossibleSteps()
{
        struct Point point;
        struct Point points[8];

        //left right up down \left \right /left /right
        struct Point stepDirection[8] = {{-1, 0},{1, 0},{0, -1},{0, 1},{-1, -1},{1, 1},{-1, 1}, {1, -1}};
        bool possibleDirection[8];

        int index[2] = {0, 0}; //count of possible steps (steps/eats)
        int rtrn = 0;

        if (currentShape->shapeId == 0)
        {
                bool stop = false;
                for (int i=0; i<2; i++)
                {
                        if (((abs(currentPlayer*7 - currentShape->point.y))*i < 2) && !stop) //first step or not
                        {
                                point.x = currentShape->point.x;
                                point.y = currentShape->point.y + stepDirection[2].y*(1+i) - 2*((currentPlayer+1)%2)*stepDirection[2].y*(1+i); //mirror algoritm
                                rtrn = CheckPoint(point);
                                if (rtrn == -1) //empty cell
                                {
                                        possibleSteps[index[0]] = point;
                                        index[0] += 1;
                                }
                                else {stop = true;}
                        }
                        point.x = currentShape->point.x + stepDirection[4+3*i].x;
                        point.y = currentShape->point.y + stepDirection[4+3*i].y - 2*((currentPlayer+1)%2)*stepDirection[4+3*i].y;
                        rtrn = CheckPoint(point);
                        if (rtrn != currentPlayer && rtrn != -1)
                        {
                                possibleEats[index[1]] = point;
                                index[1] += 1;
                        }
                }
        }
        else if (currentShape->shapeId == 2)
        {
                struct Point directionStep[8] = {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
                for (int i=0; i<8; i++)
                {
                        point.x = currentShape->point.x + directionStep[i].x;
                        point.y = currentShape->point.y + directionStep[i].y;

                        rtrn = CheckPoint(point);
                        if (rtrn == -1)
                        {
                                possibleSteps[index[0]] = point;
                                index[0] += 1;
                        }
                        else if (rtrn != currentPlayer)
                        {
                                possibleEats[index[1]] = point;
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

                        if (point.x < 0 || point.x > 7 || point.y < 0 || point.y > 7) {continue;}

                        rtrn = CheckPoint(point);
                        if (rtrn == -1)
                        {
                                possibleSteps[index[0]] = point;
                                index[0] += 1;
                        }
                        else if (rtrn != currentPlayer)
                        {
                                possibleEats[index[1]] = point;
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
                                possibleDirection[i] = 1;
                        }
                        else if ((currentShape->shapeId == 1 || currentShape->shapeId == 4) && i > 3)
                        {
                                possibleDirection[i] = 1;
                        }
                        else
                        {
                                possibleDirection[i] = 0;
                        }
                }
                for (int i=1; i<8; i++)
                {
                        for (int j=0; j<8; j++)
                        {
                                if (possibleDirection[j] == 1)
                                {
                                        point.x = currentShape->point.x + stepDirection[j].x*i;
                                        point.y = currentShape->point.y + stepDirection[j].y*i;
                                        points[j] = point;
                                }
                        }
                        
                        //check points
                        for (int j=0; j<8; j++)
                        {
                                if (points[j].x < 0 || points[j].x > 7 || points[j].y < 0 || points[j].y > 7)
                                {
                                        possibleDirection[j] = 0;
                                        continue;
                                }
                                if (possibleDirection[j] == 1)
                                {
                                        rtrn = CheckPoint(points[j]);
                                        if (rtrn == -1)
                                        {
                                                possibleSteps[index[0]] = points[j];
                                                index[0] += 1;
                                        }
                                        else
                                        {
                                                if (rtrn != currentPlayer)
                                                {
                                                        possibleEats[index[1]] = points[j];
                                                        index[1] += 1;
                                                }
                                                else if (currentShape->point.y == currentPlayer*7 && currentShape->point.x%7 == 0)
                                                {
                                                        if (currentShape->shapeId == 3)
                                                        {
                                                                if (!firstStep[currentPlayer][0] && !firstStep[currentPlayer][1 + currentShape->point.x/7])
                                                                {
                                                                        x = shapes[currentPlayer][14+currentPlayer].point.x;
                                                                        y = shapes[currentPlayer][14+currentPlayer].point.y;
                                                                        if (x == points[j].x && y == points[j].y) {castling = true;}
                                                                }
                                                        }
                                                }
                                                possibleDirection[j] = 0;
                                        }
                                }
                        }
                }
        }
        countOfPossible[0] = index[0];
        countOfPossible[1] = index[1];
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

void TForm1::CheckShah()
{
        isShah = false;

        //store global state
        struct Point *savedPossibleSteps = (struct Point *)malloc(countOfPossible[0]*sizeof(struct Point));
        struct Point *savedPossibleEats = (struct Point *)malloc(countOfPossible[1]*sizeof(struct Point));
        int savedCountOfPossible[2];
        int savedCurrentPlayer = currentPlayer;

        for (int i=0; i<countOfPossible[0]; i++)
        {
                savedPossibleSteps[i] = possibleSteps[i];
        }
        for (int i=0; i<countOfPossible[1]; i++)
        {
                savedPossibleEats[i] = possibleEats[i];
        }
        savedCountOfPossible[0] = countOfPossible[0];
        savedCountOfPossible[1] = countOfPossible[1];

        currentPlayer = (currentPlayer+1)%2;
        for (int j=0; j<16; j++)
        {
                *currentShape = shapes[currentPlayer][j];
                if (currentShape->isActive == false){continue;}
                GetPossibleSteps();
                //king
                x = shapes[(currentPlayer+1)%2][14+(currentPlayer+1)%2].point.x;
                y = shapes[(currentPlayer+1)%2][14+(currentPlayer+1)%2].point.y;

                for (int index=0; index<countOfPossible[1]; index++)
                {
                        if (possibleEats[index].x == x && possibleEats[index].y == y)
                        {
                                isShah = true;
                                break;
                        }
                }
        }

        //restore global state
        currentPlayer = savedCurrentPlayer;

        for (int i=0; i<savedCountOfPossible[0]; i++)
        {
                possibleSteps[i] = savedPossibleSteps[i];
        }
        for (int i=0; i<savedCountOfPossible[1]; i++)
        {
                possibleEats[i] = savedPossibleEats[i];
        }
        countOfPossible[0] = savedCountOfPossible[0];
        countOfPossible[1] = savedCountOfPossible[1];

        free(savedPossibleSteps);
        free(savedPossibleEats);
}

void TForm1::CheckSteps()
{
        struct Shape *emptyAddress = (struct Shape *)malloc(sizeof(struct Shape));
        struct Point *updatePossibleSteps = (struct Point *)malloc(32*sizeof(struct Point));
        struct Point *updatePossibleEats = (struct Point *)malloc(8*sizeof(struct Point));
        struct Shape *killedShape = (struct Shape *)malloc(sizeof(struct Shape));
        int index[2] = {0,0};
        int copyX;

        //store global state
        struct Shape *savedCurrentShapeAddress = currentShape;
        struct Shape *savedCurrentShapeValue = (struct Shape *)malloc(sizeof(struct Shape));
        *savedCurrentShapeValue = *currentShape;
        bool savedShah = isShah;
        bool savedCastling;
        int savedCurrentPlayer = currentPlayer;

        GetPossibleSteps();

        savedCastling = castling;

        if (castling)
        {
                currentShape = savedCurrentShapeAddress;
                copyX = shapes[currentPlayer][14+currentPlayer].point.x;
                shapes[currentPlayer][14+currentPlayer].point.x = 3 - (currentPlayer + 1)%2 + 2*currentShape->point.x/7;
                currentShape->point.x = copyX;
                currentShape = emptyAddress;
                CheckShah();
                shapes[currentPlayer][14+currentPlayer].point.x = copyX;
                if (isShah) {castling = false;}
        }

        if (countOfPossible[0] != 0)
        {
                for (int i=0; i<countOfPossible[0]; i++)
                {
                        currentShape = savedCurrentShapeAddress;
                        currentShape->point.x = possibleSteps[i].x;
                        currentShape->point.y = possibleSteps[i].y;
                        currentShape = emptyAddress;
                        CheckShah();
                        if (!isShah) {updatePossibleSteps[index[0]] = possibleSteps[i]; index[0]++;}
                }
        }
        if (countOfPossible[1] != 0)
        {
                for (int i=0; i<countOfPossible[1]; i++)
                {
                        killedShape = GetObjectFromPoint(possibleEats[i]);
                        if (killedShape != NULL){killedShape->isActive=false;}
                        currentShape = savedCurrentShapeAddress;
                        currentShape->point.x = possibleEats[i].x;
                        currentShape->point.y = possibleEats[i].y;
                        currentShape = emptyAddress;
                        CheckShah();
                        if (killedShape != NULL){killedShape->isActive=true;}
                        if (!isShah) {updatePossibleEats[index[1]] = possibleEats[i]; index[1]++;}
                }
        }

        for (int i=0; i<index[0]; i++)
        {
                possibleSteps[i] = updatePossibleSteps[i];
        }
        for (int i=0; i<index[1]; i++)
        {
                possibleEats[i] = updatePossibleEats[i];
        }

        countOfPossible[0] = index[0];
        countOfPossible[1] = index[1];

        //restore global state
        currentShape = savedCurrentShapeAddress;
        *currentShape = *savedCurrentShapeValue;
        currentPlayer = savedCurrentPlayer;
        isShah = savedShah;
        if (!savedCastling) {castling = false;}

        free(killedShape);
        free(updatePossibleSteps);
        free(updatePossibleEats);
}

struct Shape* TForm1::GetObjectFromPoint(struct Point point)
{
        for (int i = 0; i < 2; i++)
        {
                for (int j = 0; j < 16; j++)
                {
                        if (shapes[i][j].isActive == true)
                        {
                                x = shapes[i][j].point.x;
                                y = shapes[i][j].point.y;
                                if (x == point.x && y == point.y)
                                {
                                        return &shapes[i][j];
                                }
                        }
                }
        }
        return NULL;
}

void TForm1::CheckWinOrStandoff()
{
        bool saveShah = isShah;

        for (int i=0; i<16; i++)
        {
                currentShape = &shapes[currentPlayer][i];
                if (!currentShape->isActive) {continue;}
                CheckSteps();
                if (countOfPossible[0] == 0 && countOfPossible[1] == 0) {isStandoff = true;}
                else {isStandoff = false; break;}
        }
        isShah = saveShah;
        if (isStandoff && isShah) {isStandoff = false; isWin = true;}
}

void __fastcall TForm1::FormResize(TObject *Sender)
{
        //square
        if (Form1->ClientWidth > Form1->ClientHeight) {side = Form1->ClientWidth; Form1->Height = Form1->Width;}
        else {side = Form1->ClientHeight; Form1->Width = Form1->Height;}

        //form
        Form1->ClientWidth = side;
        Form1->ClientHeight = side;

        //view
        View->Width = side;
        View->Height = side;
        View->Picture->Bitmap->Width = side;
        View->Picture->Bitmap->Height = side;

        if (side > 448)
        {
                transferLayer->Width = side;
                transferLayer->Height = side;
        }

        //redraw
        Draw();
}
