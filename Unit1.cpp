//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Themes.hpp"
#include "ToolCtrlsEh.hpp"
#include "sVCLUtils.hpp"
#include "acSBUtils.hpp"
#include "acntUtils.hpp"
#include "sGlyphUtils.hpp"
#include "sAlphaGraph.hpp"
#include "acEhLibPainter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "DBGridEh"
#pragma link "GridsEh"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "MemTableDataEh"
#pragma link "MemTableEh"
#pragma link "sButton"
#pragma link "sComboBox"
#pragma link "sSpeedButton"
#pragma link "sComboEdit"
#pragma link "sCustomComboEdit"
#pragma link "sMaskEdit"
#pragma link "DBCtrlsEh"
#pragma link "sCurrEdit"
#pragma link "sCurrencyEdit"
#pragma link "sDBCalcEdit"
#pragma link "sEdit"
#pragma link "sSpinEdit"
#pragma link "sUpDown"
#pragma link "sStatusBar"


#pragma link "sGroupBox"
#pragma link "DBGridEhGrouping"
#pragma link "DBLookupEh"
#pragma resource "*.dfm"
TForm1 *Form1;
#define TBitmap Graphics::TBitmap
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  sSkinManager1->Active = CheckBox1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::sSkinManager1AfterChange(TObject *Sender)
{
	Toolctrlseh::ClearButtonsBitmapCache();
	this->Invalidate();
	this->Repaint();
}
//---------------------------------------------------------------------------
