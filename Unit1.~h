//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImageList *DeskSprites;
        TImageList *ShapesSprites;
        TImage *View;
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void Draw();
        void RedrawShapes();
        void MoveShape();
        void GetPossibleSteps();
        int CheckPoint(struct Point);
        void NewGame();
        void CheckShah();
        void CheckSteps();
        void CheckWinOrStandoff();
        struct Shape* GetObjectFromPoint(struct Point);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
