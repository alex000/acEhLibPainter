//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBGridEh.hpp"
#include "GridsEh.hpp"
#include "sSkinManager.hpp"
#include "sSkinProvider.hpp"
#include "MemTableDataEh.hpp"
#include "MemTableEh.hpp"
#include <DB.hpp>
#include <Db.hpp>
#include "sButton.hpp"
#include "sComboBox.hpp"
#include "sSpeedButton.hpp"
#include <Buttons.hpp>
#include "sComboEdit.hpp"
#include "sCustomComboEdit.hpp"
#include "sMaskEdit.hpp"
#include <Mask.hpp>
#include "DBCtrlsEh.hpp"
#include "sCurrEdit.hpp"
#include "sCurrencyEdit.hpp"
#include "sDBCalcEdit.hpp"
#include <ComCtrls.hpp>
#include "sEdit.hpp"
#include "sSpinEdit.hpp"
#include "sUpDown.hpp"
#include "sStatusBar.hpp"
#include <ADODB.hpp>
#include "sGroupBox.hpp"
#include "DBGridEhGrouping.hpp"
#include "DBLookupEh.hpp"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TsSkinManager *sSkinManager1;
	TsSkinProvider *sSkinProvider1;
	TDBGridEh *DBGridEh1;
	TMemTableEh *MemTableEh1;
	TDataSource *DataSource1;
	TStringField *MemTableEh1aaa;
	TCheckBox *CheckBox1;
	TBooleanField *MemTableEh1bbb;
	TMemTableEh *MemTableEh2;
	TStringField *StringField1;
	TBooleanField *BooleanField1;
	TDataSource *DataSource2;
	TIntegerField *MemTableEh1indx;
	TStringField *MemTableEh1lok;
	TIntegerField *MemTableEh2indx;
	TsStatusBar *sStatusBar1;
	TsGroupBox *sGroupBox1;
	TsComboBox *sComboBox1;
	TsComboEdit *agLogoComboEdit;
	TsComboBox *sComboBox2;
	TsComboBox *sComboBox3;
	TsComboEdit *sComboEdit1;
	TsComboEdit *sComboEdit2;
	TsSpinEdit *sSpinEdit1;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox2;
	TsSpinEdit *sSpinEdit2;
	TsSpinEdit *sSpinEdit3;
	TsGroupBox *sGroupBox2;
	TDBEditEh *DBEditEh1;
	TDBCheckBoxEh *DBCheckBoxEh1;
	TDBEditEh *DBEditEh2;
	TDBEditEh *DBEditEh3;
	TDBComboBoxEh *DBComboBoxEh1;
	TDBCheckBoxEh *DBCheckBoxEh2;
	TDBComboBoxEh *DBComboBoxEh2;
	TDBComboBoxEh *DBComboBoxEh3;
	TDBLookupComboboxEh *DBLookupComboboxEh1;
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall sSkinManager1AfterChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
