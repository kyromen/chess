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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImageList *DeskSprites;
        TImageList *ShapesSprites;
        TButton *Startuem;
        TImage *View;
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall StartuemClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void Draw(void);
        void RedrawShapes(void);
        void MoveShape(void);
        void EatShape(struct Shape *);
        Shape * GetObjectUnderCursor(void);
        void GetPosibleSteps(void);
        int CheckPoint(struct Point);
        void Initialize(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
