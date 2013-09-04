//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "acEhLibPainter.h"
#include "sVCLUtils.hpp"
#include "acSBUtils.hpp"
#include "acntUtils.hpp"
#include "sGlyphUtils.hpp"
#include "sAlphaGraph.hpp"
#include "ToolCtrlsEh.hpp"
#include "DBGridEh.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolCtrlsEh.obj"
#pragma link "Dbctrlseh.obj"

#define TBitmap Graphics::TBitmap

namespace acEhLibPainter
{

class THacedCustomDBGridEh : public Dbgrideh::TCustomDBGridEh
{
 public:
	Byte inline __fastcall GetUpdateLock() {return UpdateLock;}
	virtual bool __fastcall IsSelectionActive(void);
	__property Color  = {default=-16777211};
	inline __fastcall THacedCustomDBGridEh(Classes::TComponent* AOwner) : Dbgrideh::TCustomDBGridEh(AOwner) { };
};
	Graphics::TColor inline __fastcall GetRightHighlightColor(void)
	{
		if ((DefaultManager->GetHighLightColor() != DefaultManager->GetActiveEditColor())&&(DefaultManager->GetHighLightColor() != Graphics::clHighlight))
			return DefaultManager->GetHighLightColor();
		else if ((DefaultManager->GetHighLightColor() == DefaultManager->GetActiveEditColor())&&(DefaultManager->GetActiveEditColor() == clBlack))
		 return	ChangeBrightness( AverageColor(DefaultManager->GetHighLightColor(),DefaultManager->GetGlobalColor()),25); // слишком темно
		else
			return AverageColor(DefaultManager->GetHighLightColor(),DefaultManager->GetGlobalColor());
		return Graphics::clHighlight;
	};

class TMyDBGridEhStyle : public TDBGridEhStyle
{
protected:
	 virtual bool __fastcall HighlightDataCellColor(TCustomDBGridEh* AGrid, int ACol, int ARow, int DataCol, int DataRow, const AnsiString Value, Gridseh::TGridDrawState AState, bool InMultiSelect, Graphics::TColor &AColor, Graphics::TFont* AFont)
	 {
		if ((!DefaultManager)||(!DefaultManager->SkinData->Active))
		 return	TDBGridEhStyle::HighlightDataCellColor(AGrid, ACol, ARow, DataCol, DataRow, Value, AState, InMultiSelect, AColor, AFont);

		bool Result   = false;
		bool AFocused = false;

		if ((AGrid->DataSource)&&(AGrid->DataSource->DataSet)&&(AGrid->DataSource->DataSet->Active))
			Result = AGrid->Selection->DataCellSelected(DataCol, AGrid->DataSource->DataSet->Bookmark);
		if (!Result)
		{
			AFocused = AGrid->Focused() && AGrid->OptionsEh.Contains(dghHighlightFocus);
			if ((AGrid->OptionsEh.Contains(dghRowHighlight)) && (DataRow + AGrid->TopDataOffset == AGrid->Row) &&
			 /*	(AGrid->Selection->SelectionType == gstNon) && ¬ мульти-выборе - мешало  */ ! (DataCol + AGrid->IndicatorOffset == AGrid->Col))
			{
				AFocused = true;
				AState   = AState << Gridseh::gdSelected;
			}
			Result = (AState.Contains(Gridseh::gdSelected)  || ((ARow) == AGrid->Row) && (AGrid->Options.Contains(Dbgrideh::dgRowSelect)))
				&& ((AGrid->Options.Contains(Dbgrideh::dgAlwaysShowSelection)) || (AFocused))
				 //	{ updatelock eliminates flicker when tabbing between rows }
			&& ((((THacedCustomDBGridEh*)AGrid)->GetUpdateLock() == 0) || (AGrid->Options.Contains(Dbgrideh::dgRowSelect)));
		}
		if (Result)
		{
			if (((THacedCustomDBGridEh*)AGrid)->IsSelectionActive())
			{
				if (LuminateSelection /*&& (FScreenNumColors == -1)*/ &&
					 ! ((ARow == AGrid->Row) && ((ACol == AGrid->Col) || (AGrid->Options.Contains(Dbgrideh::dgRowSelect)))))
				{
					if (InMultiSelect)
					{
						AColor = AverageColor(AverageColor(GetRightHighlightColor(),((THacedCustomDBGridEh*)AGrid)->Color),((THacedCustomDBGridEh*)AGrid)->Color); //MixColors(DefaultManager->GetHighLightColor(),((THacedCustomDBGridEh*)AGrid)->Color,DefDisabledBlend); //GetNearestColor(AGrid->Canvas->Handle, LightenColor(AColor, DefaultManager->GetHighLightColor(), True));
						AFont->Style = AFont->Style << fsBold;
					}
					else
					AColor = AverageColor(GetRightHighlightColor(),((THacedCustomDBGridEh*)AGrid)->Color); //MixColors(DefaultManager->GetHighLightColor(),((THacedCustomDBGridEh*)AGrid)->Color,DefDisabledBlend); //GetNearestColor(AGrid->Canvas->Handle, LightenColor(AColor, DefaultManager->GetHighLightColor(), True));
				}
				else
				{
					if (InMultiSelect)
						AColor    = ChangeBrightness(GetRightHighlightColor(),-25);
					else
					AColor        = GetRightHighlightColor();
					AFont->Color  = DefaultManager->GetHighLightFontColor();
				}
			} else if ((ARow == AGrid->Row) &&
									((ACol == AGrid->Col) || (AGrid->Options.Contains(Dbgrideh::dgRowSelect))) &&
									((AGrid->Options.Contains(Dbgrideh::dgAlwaysShowSelection)) || AFocused))
			{
				AColor       = DefaultManager->GetActiveEditColor(); //clBtnShadow;
				AFont->Color = DefaultManager->GetActiveEditFontColor();
					if (AColor == AFont->Color)
						AFont->Color = DefaultManager->GetHighLightFontColor();
			} else /*if (FScreenNumColors == -1)*/
				AColor = GetNearestColor(AGrid->Canvas->Handle, LightenColor(AColor, DefaultManager->GetActiveEditColor()/*clBtnShadow*/, False));
	//		else
	//			AColor = clBtnFace;
		}

			return Result;
	 };
	 virtual bool __fastcall HighlightNoDataCellColor(TCustomDBGridEh* AGrid, int ACol, int ARow, int DataCol, int DataRow, TCellAreaTypeEh CellType, Gridseh::TGridDrawState AState, bool InMultiSelect, Graphics::TColor &AColor, Graphics::TFont* AFont)
	 {
		 bool ret = TDBGridEhStyle::HighlightNoDataCellColor(AGrid, ACol, ARow, DataCol, DataRow, CellType, AState, InMultiSelect, AColor, AFont);
		 if ((DefaultManager)&&(DefaultManager->SkinData->Active)&&(AColor == clBtnFace))
			 AColor = DefaultManager->GetGlobalColor();

		 if ((DefaultManager)&&(DefaultManager->SkinData->Active)/*&&(AFont->Color == clWhite)*/)
			 AFont->Color = DefaultManager->GetGlobalFontColor();

		 // Enabled = false, рисовать заголовок серым
		 if (!AGrid->Enabled)
			 AFont->Color = AverageColor(AFont->Color,AColor);
		 if ((AGrid->IsUseMultiTitle())&&(AGrid->TitleFont->Color != AFont->Color))  // глюк грида - при MultiTitle=true - AFont->Color не действует (
		 {
				Classes::TNotifyEvent tmp = AGrid->TitleFont->OnChange;
				AGrid->TitleFont->OnChange = 0;
				AGrid->TitleFont->Color = AFont->Color;
				AGrid->TitleFont->OnChange = tmp;
		 }

		 if (CellType.VertType == Dbgrideh::vctSubTitleEh)
		 {
			 AColor = ChangeBrightness(AGrid->FixedColor,+40);
			 AFont->Color = ChangeBrightness(AGrid->FixedColor,-60);
		 }

			return ret;
	 };
	 virtual void __fastcall StyleWndProc(Messages::TMessage &Msg){TDBGridEhStyle::StyleWndProc(Msg);};
	 virtual void __fastcall SysColorChanged(void){TDBGridEhStyle::SysColorChanged();};
public:
	 __fastcall TMyDBGridEhStyle(void) : TDBGridEhStyle(){};
};


	void __fastcall DrawDropDownButton(HDC DC, const Types::TRect &R, bool Enabled, bool Flat, bool Active, bool Down)
	{
		if ((!DefaultManager)||(!DefaultManager->Active))
		{
			const int DownFlags[2]    = {0, DFCS_PUSHED };
			const int FlatFlags[2]    = {0, DFCS_FLAT};
			const int EnabledFlags[2] = {DFCS_INACTIVE, 0};

			int Flags = DownFlags[Down] | FlatFlags[Flat] | EnabledFlags[Enabled];
			DrawFrameControl(DC, (tagRECT*)&R, DFC_SCROLL, Flags | DFCS_SCROLLCOMBOBOX);
			return;
		}

		TBitmap *TmpBmp = CreateBmp24(R.Width(), R.Height());

		int Mode;

		if (Down)
			Mode = 2;
		else if (Active)
			 Mode = 1;
		 else
			 Mode = 0;

		 if (!Enabled)
			 Mode = 0;

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetActiveEditColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,R.Width(), R.Height()));




			if (DefaultManager->ConstData.ComboBtnIndex > -1 )
			{
				PaintItem(DefaultManager->ConstData.ComboBtnIndex, s_ComboBtn, MakeCacheInfo(TmpBmp),
					true, Mode, TRect(0,0,R.Width(), R.Height()), Point(0, 0), TmpBmp, DefaultManager, DefaultManager->ConstData.ComboBtnBG, DefaultManager->ConstData.ComboBtnBGHot);
			}

		int glIndex = DefaultManager->ConstData.ComboGlyph;

		int ButtonHeight = HeightOf(DefaultManager->ma[glIndex].R) / (1 + DefaultManager->ma[glIndex].MaskType);
				DrawSkinGlyph(TmpBmp,
				Point(R.Left + (WidthOf(R)+ - WidthOf(DefaultManager->ma[glIndex].R) / DefaultManager->ma[glIndex].ImageCount) / 2,
							(R.Height() - ButtonHeight) / 2), Mode, (Enabled)?1:0, DefaultManager->ma[DefaultManager->ConstData.ComboGlyph],MakeCacheInfo(TmpBmp));

		BitBlt(DC, R.Left, R.Top, R.Width(), R.Height(), TmpBmp->Canvas->Handle, R.Left, R.Top, SRCCOPY);

		delete TmpBmp;
	}

	//---------------------------------------------------------------------------

void __fastcall DrawEllipsisButton(Graphics::TCanvas* Canvas, const Types::TRect &ARect, bool Enabled, bool Active, bool Flat, bool Pressed)
	{
		if ((!DefaultManager)||(!DefaultManager->Active))
		{
			const int PressedFlags[2] = {EDGE_RAISED, EDGE_SUNKEN};
			TRect ElRect = ARect;
			HBRUSH  Brush, SaveBrush ;
			int H;
			int InterP = 2;
			int PWid = 2;

			Brush = GetSysColorBrush(COLOR_BTNFACE);
			if (Flat)
			{
				FillRect(Canvas->Handle, &ElRect, Brush);
				InflateRect(&ElRect, -1, -1);
			} else
			{
				DrawEdge(Canvas->Handle, &ElRect, PressedFlags[Pressed], BF_RECT | BF_MIDDLE);
				InflateRect(&ElRect, -2, -2);
			}

			int W = ElRect.Right - ElRect.Left; //+ Ord(not Active and Flat);
			if (W < 12)
				InterP = 1;
			if (W < 8)
				PWid = 1;
			W = ElRect.Left + W / 2 - PWid / 2 + Pressed; //- Ord(not Active and Flat);
			H = ElRect.Top + (ElRect.Bottom - ElRect.Top) / 2 - PWid / 2 + Pressed;


			if (!Enabled)
			{
				W++; H++;
				Brush = GetSysColorBrush(COLOR_BTNHILIGHT);
				SaveBrush = SelectObject(Canvas->Handle, Brush);
				PatBlt(Canvas->Handle, W, H, PWid, PWid, PATCOPY);
				PatBlt(Canvas->Handle, W - InterP - PWid, H, PWid, PWid, PATCOPY);
				PatBlt(Canvas->Handle, W + InterP + PWid, H, PWid, PWid, PATCOPY);
				W--; H--;
				SelectObject(Canvas->Handle, SaveBrush);
				Brush = GetSysColorBrush(COLOR_BTNSHADOW);
			} else
				Brush = GetSysColorBrush(COLOR_BTNTEXT);

			SaveBrush = SelectObject(Canvas->Handle, Brush);
			PatBlt(Canvas->Handle, W, H, PWid, PWid, PATCOPY);
			PatBlt(Canvas->Handle, W - InterP - PWid, H, PWid, PWid, PATCOPY);
			PatBlt(Canvas->Handle, W + InterP + PWid, H, PWid, PWid, PATCOPY);
			SelectObject(Canvas->Handle, SaveBrush);
			return;
		}
		//-----

		TRect            R = ARect;
		TBitmap   *TmpBmp  = CreateBmp24(R.Width(), R.Height());
 /*		TCacheInfo EmptyCI = MakeCacheInfo(TmpBmp);
		TBitmap   *DefBmp  = GetResBmp("BTN_ELLIPSIS");

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetActiveEditColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,R.Width(), R.Height()));

		TRect r2;
		if (Pressed)
			r2 = TRect(2 * 18, 0, 3 * 18 , 17);
		else if (Active)
			r2 = TRect(18, 0, 2 * 18 , 17);
		else
			r2 = TRect(0, 0, 18, 17 );

		EmptyCI.X = 0;

		if (R.Left < 0)
			R.Left = 0;

		if (R.Top < 0)
			R.Top = 0;

		if (Pressed)
			R.Left = R.Left-1;

		CopyByMask(Rect(R.Width() - 18 ,((R.Height() -  17)<0)? 0 : (R.Height() -  17),R.Width(), R.Height()), r2, TmpBmp, DefBmp, EmptyCI, True);
		*/

		int CurrentState = 0;
		if (Pressed)
			CurrentState = 2;
		else if (Active)
			CurrentState = 1;
		else
			CurrentState = 0;

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetActiveEditColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,R.Width(), R.Height()));
		DrawAlphaImgList(acResImgList, TmpBmp, (R.Width() - 16)/2, (((R.Height() -  16)<0) ? 0 : (R.Height() -  16))/2, iBTN_ELLIPSIS, 0, Graphics::clNone, CurrentState, 3, false);

	//	EmptyCI.Ready = true;
		if (!Enabled)
			BlendColorRect(TmpBmp, Rect(0 ,0 ,R.Width(), R.Height()), 50, DefaultManager->GetActiveEditColor());

		BitBlt(Canvas->Handle, R.Left, R.Top, R.Width(), R.Height(), TmpBmp->Canvas->Handle, R.Left, R.Top, SRCCOPY);
		delete TmpBmp;
	}

	//---------------------------------------------------------------------------
	 void __fastcall DrawPlusMinusButton(Graphics::TCanvas* Canvas, const Types::TRect &ARect, bool Enabled, bool Active, bool Flat, bool Pressed, bool Plus)
	{
		int iButtonSkinIndex;
		TRect R = ARect;
		const int PressedFlags[2] = {EDGE_RAISED, EDGE_SUNKEN};
		TRect ElRect = ARect;
		HBRUSH  Brush, SaveBrush ;
		int H;
		int PWid = 0;

		if (((!DefaultManager)||(!DefaultManager->Active))||(!DefaultManager->IsValidSkinIndex(iButtonSkinIndex = DefaultManager->GetSkinIndex(s_Button))))
		{
			Brush = GetSysColorBrush(COLOR_BTNFACE);
			if (Flat)
			{
				FillRect(Canvas->Handle, &ElRect, Brush);
				InflateRect(&ElRect, -1, -1);
			} else
			{
				DrawEdge(Canvas->Handle, &ElRect, PressedFlags[Pressed], BF_RECT | BF_MIDDLE);
				InflateRect(&ElRect, -2, -2);
				FillRect(Canvas->Handle, &ElRect, Brush);
			}
		}
		else
		{
			TBitmap   *TmpBmp  = CreateBmp24(R.Width(), R.Height());
			int Mode;

			if (Pressed)
				Mode = 2;
			else if (Active)
				Mode = 1;
			else
				Mode = 0;
			if (!Enabled)
				Mode = 0;

			TmpBmp->Canvas->Brush->Color = DefaultManager->GetActiveEditColor();
			TmpBmp->Canvas->FillRect(TRect(0,0,R.Width(), R.Height()));

			PaintItem(iButtonSkinIndex, s_Button, MakeCacheInfo(TmpBmp),
				true, Mode, TRect(0,0,R.Width(), R.Height()), Point(0, 0), TmpBmp, DefaultManager); // PaintSmallItem

			BitBlt(Canvas->Handle, R.Left, R.Top, R.Width(), R.Height(), TmpBmp->Canvas->Handle, R.Left, R.Top, SRCCOPY);
			delete TmpBmp;
		}

		int MinWH = ElRect.Right - ElRect.Left;
		if (ElRect.Bottom - ElRect.Top < MinWH)
			MinWH = ElRect.Bottom - ElRect.Top;
		PWid = MinWH * 4 / 7;
		if (PWid == 0)
			PWid = 1;
		int PHet = PWid / 3;
		if (PHet == 0)
			PHet = 1;
		if (Flat)
			PWid--;
		if (PWid % 2 != MinWH % 2)
			PWid++;
		if (Plus && (PWid % 2 != PHet % 2))
			if (MinWH < 12)
				PWid++;
			else
				PWid--;
		int PlusInd = PWid / 2 - PHet / 2;

		int W = ElRect.Left + (ElRect.Right - ElRect.Left - PWid) / 2;
		W += Pressed;
		H = ElRect.Top + (ElRect.Bottom - ElRect.Top - PHet) / 2 + Pressed;

		if ((!Enabled)&&((!DefaultManager)||(!DefaultManager->Active)))
		{
			W++; H++;
			Brush = GetSysColorBrush(COLOR_BTNHILIGHT);
			SaveBrush = SelectObject(Canvas->Handle, Brush);
			PatBlt(Canvas->Handle, W, H, PWid, PHet, PATCOPY);
			if (Plus)
				PatBlt(Canvas->Handle, W + PlusInd, H - PlusInd, PHet, PWid, PATCOPY);
			W--; H--;
			SelectObject(Canvas->Handle, SaveBrush);
			Brush = GetSysColorBrush(COLOR_BTNSHADOW);
		} else
			Brush = GetSysColorBrush(COLOR_BTNTEXT);

		if ((DefaultManager)&&(DefaultManager->Active))
			Brush = CreateSolidBrush(DefaultManager->GetGlobalFontColor());

		SaveBrush = SelectObject(Canvas->Handle, Brush);
		PatBlt(Canvas->Handle, W, H, PWid, PHet, PATCOPY);
		if (Plus)
			PatBlt(Canvas->Handle, W + PlusInd, H - PlusInd, PHet, PWid, PATCOPY);
		SelectObject(Canvas->Handle, SaveBrush);

		if ((DefaultManager)&&(DefaultManager->Active))
			DeleteObject(Brush);

		if ((!Enabled)&&((DefaultManager)&&(DefaultManager->Active)))
		{
			TBitmap *TmpBmp  = CreateBmp24(R.Width(), R.Height());
			BitBlt(TmpBmp->Canvas->Handle, 0, 0, TmpBmp->Width, TmpBmp->Height, Canvas->Handle, 0, 0, SRCCOPY);
			BlendColorRect(TmpBmp, Rect(0 ,0 ,R.Width(), R.Height()), 50, DefaultManager->GetActiveEditColor());
			BitBlt(Canvas->Handle, R.Left, R.Top, R.Width(), R.Height(), TmpBmp->Canvas->Handle, R.Left, R.Top, SRCCOPY);
			delete TmpBmp;
		}
	}

	//---------------------------------------------------------------------------
	 void __fastcall DrawUpDownButton(Graphics::TCanvas* Canvas, const Types::TRect &R, bool Enabled, bool Flat, bool Active, bool Down, bool DownDirection)
	{
		if ((!DefaultManager)||(!DefaultManager->Active))
		{
			const int DownFlags[2]    = {0, DFCS_PUSHED };
			const int FlatFlags[2]    = {0, DFCS_FLAT};
			const int EnabledFlags[2] = {DFCS_INACTIVE, 0};
			const int IsDownFlags[2]  = {DFCS_SCROLLUP, DFCS_SCROLLDOWN};

			int Flags = DownFlags[Down] | FlatFlags[Flat] | EnabledFlags[Enabled];
			DrawFrameControl(Canvas->Handle, (tagRECT*)&R, DFC_SCROLL, Flags | IsDownFlags[DownDirection]);
			return;
		}

		TBitmap   *TmpBmp  = CreateBmp24(R.Width(), R.Height());
		int Mode;

		if (Down)
			Mode = 2;
		else if (Active)
			Mode = 0; //= 1;
		else
			Mode = 0;
		if (!Enabled)
			Mode = 0;

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetActiveEditColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,R.Width(), R.Height()));

		int sSkinIndex,sMaskIndex,sBGIndex,sBGHotIndex,sArrowMask, sLimPosition, XOffset, YOffset;
		String	sSkinSection;
		TCacheInfo CI;
		CI.Ready = false;

		if (DownDirection)
		{
			sSkinIndex   = DefaultManager->ConstData.IndexScrollBottom;
			sMaskIndex   = DefaultManager->ConstData.MaskScrollBottom;
			sBGIndex     = DefaultManager->ConstData.IndexBGScrollBottom;
			sBGHotIndex  = DefaultManager->ConstData.IndexBGHotScrollBottom;
			sSkinSection = s_ScrollBtnBottom;
			sArrowMask = DefaultManager->ConstData.MaskArrowBottom;
		}
		else
		{
			sSkinIndex   = DefaultManager->ConstData.IndexScrollTop;
			sMaskIndex   = DefaultManager->ConstData.MaskScrollTop;
			sBGIndex     = DefaultManager->ConstData.IndexBGScrollTop;
			sBGHotIndex  = DefaultManager->ConstData.IndexBGHotScrollTop;
			sSkinSection = s_ScrollBtnTop;
			sArrowMask   = DefaultManager->ConstData.MaskArrowTop;
		}
		CI = MakeCacheInfo(TmpBmp);
		PaintItemBGFast(sSkinIndex, sBGIndex, sBGHotIndex, sSkinSection, CI, Mode, R, Point(0,0)/*Point(Left + XOffset, Top + YOffset)*/, TmpBmp, DefaultManager);

		CI.X = 0;
		CI.Y = 0;

		if (DefaultManager->IsValidImgIndex(sMaskIndex))
			DrawSkinRect(TmpBmp, R, True, CI, DefaultManager->ma[sMaskIndex], Mode, true, DefaultManager);

		CI.Bmp = TmpBmp;

		if (sArrowMask > -1)
		{
			TPoint p;
			if (DefaultManager->ma[sArrowMask].Bmp == NULL)
			{
				p.x = (TmpBmp->Width - WidthOf(DefaultManager->ma[sArrowMask].R) / DefaultManager->ma[sArrowMask].ImageCount) / 2;
				p.y = (TmpBmp->Height - HeightOf(DefaultManager->ma[sArrowMask].R) / (1 + DefaultManager->ma[sArrowMask].MaskType)) / 2;
			}
			else if (DefaultManager->ma[sArrowMask].Bmp->Height / 2 < TmpBmp->Height)
			{
				p.x = (TmpBmp->Width - DefaultManager->ma[sArrowMask].Bmp->Width / 3) / 2;
				p.y = (TmpBmp->Height - DefaultManager->ma[sArrowMask].Bmp->Height / 2) / 2;
			}
			if ((p.x > -1) && (p.y > -1))
				DrawSkinGlyph(TmpBmp, p, Mode, 1, DefaultManager->ma[sArrowMask],MakeCacheInfo(TmpBmp));
		}

		if (!Enabled)
			BlendColorRect(TmpBmp, Rect(0 ,0 ,R.Width(), R.Height()), 50, DefaultManager->GetActiveEditColor());

		BitBlt(Canvas->Handle, R.Left, R.Top, R.Width(), R.Height(), TmpBmp->Canvas->Handle, R.Left, R.Top, SRCCOPY);
		delete TmpBmp;
	}

	//---------------------------------------------------------------------------

	 void __fastcall DrawCheck(HDC DC, const Types::TRect &R, Stdctrls::TCheckBoxState AState, bool AEnabled, bool AFlat, bool ADown, bool AActive)
	{
		if ((!DefaultManager)||(!DefaultManager->Active))
		{
			int oldRgn   = 0;
			TRect DrawRect = R;
			int DrawState;
			HRGN Rgn, SaveRgn;

			if ((DrawRect.Right - DrawRect.Left) > (DrawRect.Bottom - DrawRect.Top))
			{
				DrawRect.Left  = DrawRect.Left + ((DrawRect.Right - DrawRect.Left) - (DrawRect.Bottom - DrawRect.Top)) / 2;
				DrawRect.Right = DrawRect.Left + (DrawRect.Bottom - DrawRect.Top);
			} else if ((DrawRect.Right - DrawRect.Left) < (DrawRect.Bottom - DrawRect.Top))
			{
				DrawRect.Top    = DrawRect.Top + ((DrawRect.Bottom - DrawRect.Top) - (DrawRect.Right - DrawRect.Left)) / 2;
				DrawRect.Bottom = DrawRect.Top + (DrawRect.Right - DrawRect.Left);
			}

			 switch (AState) {
				 case cbChecked:
						DrawState = DFCS_BUTTONCHECK | DFCS_CHECKED;
				 break;
				 case cbUnchecked:
						DrawState = DFCS_BUTTONCHECK;
				 break;
				 default:
						DrawState = DFCS_BUTTON3STATE | DFCS_CHECKED; // cbGrayed
			 }

			if (!AEnabled)
				DrawState = DrawState | DFCS_INACTIVE;
			if (ADown)
				DrawState = DrawState | DFCS_PUSHED;

			if (AFlat)
			{
					//{ Remember current clipping region }
				SaveRgn = CreateRectRgn(0, 0, 0, 0);
				oldRgn = GetClipRgn(DC, SaveRgn);
				 //	{ Clip 3d-style checkbox to prevent flicker }
				Rgn = CreateRectRgn(DrawRect.Left + 1, DrawRect.Top + 1, DrawRect.Right - 1, DrawRect.Bottom - 1);
				SelectClipRgn(DC, Rgn);
				DeleteObject(Rgn);
			}
			if (AFlat)
				InflateRect(&DrawRect, 1, 1);

			DrawFrameControl(DC, &DrawRect, DFC_BUTTON, DrawState);

			if (AFlat)
			{
				if (oldRgn == 0)
					SelectClipRgn(DC, 0);
				else
					SelectClipRgn(DC, SaveRgn);
				DeleteObject(SaveRgn);
				 //	{ Draw flat rectangle in-place of clipped 3d checkbox above }
				InflateRect(&DrawRect, -1, -1);
				if (AActive)
					FrameRect(DC, &DrawRect, GetSysColorBrush(COLOR_BTNFACE));
				else
					FrameRect(DC, &DrawRect, GetSysColorBrush(COLOR_BTNSHADOW));
			}
			return;
		}

		int glIndex;
		switch (AState) {
			case cbChecked:
				glIndex = DefaultManager->GetMaskIndex(DefaultManager->ConstData.IndexGLobalInfo, s_GlobalInfo, s_CheckBoxChecked);
			break;
			case cbUnchecked:
				glIndex = DefaultManager->GetMaskIndex(DefaultManager->ConstData.IndexGLobalInfo, s_GlobalInfo, s_CheckBoxUnChecked);
			break;
			default:
				glIndex = DefaultManager->GetMaskIndex(DefaultManager->ConstData.IndexGLobalInfo, s_GlobalInfo, s_CheckBoxGrayed);
		}

		TRect R2 = R;
		R2.bottom += (DefaultManager->ma[glIndex].Bmp)?(DefaultManager->ma[glIndex].Bmp->Height / 2):(HeightOf(DefaultManager->ma[glIndex].R) / (DefaultManager->ma[glIndex].MaskType + 1)) - R2.Height();
		if (DefaultManager->ma[glIndex].ImageCount == 0)
			DefaultManager->ma[glIndex].ImageCount = 1;
		R2.right  += (DefaultManager->ma[glIndex].Bmp)?(DefaultManager->ma[glIndex].Bmp->Width / 3):(WidthOf(DefaultManager->ma[glIndex].R) / DefaultManager->ma[glIndex].ImageCount) - R2.Width();
		Toolctrlseh::DefaultCheckBoxWidth  = R2.Bottom;
		Toolctrlseh::DefaultCheckBoxHeight = R2.Right;


		TBitmap   *TmpBmp  = CreateBmp24(R2.Width(), R2.Height());
		int Mode;

		if (ADown)
			Mode = 2;
		else if (AActive)
			Mode = 1;
		else
			Mode = 0;
		if (!AEnabled)
			Mode = 0;

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetGlobalColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,R2.Width(), R2.Height()));

		DrawSkinGlyph(TmpBmp, Point(R2.Top,R2.Left), Mode, 1, DefaultManager->ma[glIndex],MakeCacheInfo(TmpBmp));

		if (!AEnabled)
			BlendColorRect(TmpBmp, Rect(0 ,0 ,R2.Width(), R2.Height()), 50, DefaultManager->GetGlobalColor());

		BitBlt(DC, R2.Left, R2.Top, R2.Width(), R2.Height(), TmpBmp->Canvas->Handle, R2.Left, R2.Top, SRCCOPY);
		delete TmpBmp;
	}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSkinedSizeGripEh : public Toolctrlseh::TSizeGripEh
{
	inline void __fastcall RotateBmp90(TBitmap *SrcBitmap,TBitmap *DestBitmap)
	{	// поворачиваем на 90
		// вращаем попиксельно
		for (int x=0;x<SrcBitmap->Width;x++)
			for(int y=0;y<SrcBitmap->Height;y++)
				DestBitmap->Canvas->Pixels[y][SrcBitmap->Width-1-x] = SrcBitmap->Canvas->Pixels[x][y];
	}

	inline void __fastcall RotateBmp180(TBitmap *SrcBitmap,TBitmap *DestBitmap)
	{	// поворачиваем на 180
		// вращаем попиксельно
		for (int x=0;x<SrcBitmap->Width;x++)
			for(int y=0;y<SrcBitmap->Height;y++)
				DestBitmap->Canvas->Pixels[x][SrcBitmap->Height-1-y] = SrcBitmap->Canvas->Pixels[x][y];
	}

	virtual void __fastcall Paint(void)
	{
		if ((!DefaultManager)||(!DefaultManager->Active))
		{
			int i=1, xi, yi;
			int XArray[2];
			int YArray[2];
			int xIdx, yIdx;

			if (Position == Toolctrlseh::sgpBottomRight)
			{
				xi = 1; yi = 1;
				xIdx = 0; yIdx = 1;
				XArray[0] = 0; YArray[0] = Width;
				XArray[1] = Width; YArray[1] = 0;
			} else if (Position == Toolctrlseh::sgpBottomLeft)
			{
				xi = -1; yi = 1;
				xIdx = 1; yIdx = 0;
				XArray[0] = 0; YArray[0] = 1;
				XArray[1] = Width - 1; YArray[1] = Width;
			} else if (Position == Toolctrlseh::sgpTopLeft)
			{
				xi = -1; yi = -1;
				xIdx = 0; yIdx = 1;
				XArray[0] = Width - 1; YArray[0] = -1;
				XArray[1] = -1; YArray[1] = Width - 1;
			} else //  Position = sgpTopRight
			{
				xi = 1; yi = -1;
				xIdx = 1; yIdx = 0;
				XArray[0] = Width; YArray[0] = Width - 1;
				XArray[1] = 0; YArray[1] = -1;
			}

			while (i < Width)
			{
				TPoint tmp[2] = {Point(XArray[0], YArray[0]), Point(XArray[1], YArray[1])};
				Canvas->Pen->Color = clBtnHighlight;
				Canvas->Polyline(tmp,2/*[Point(XArray[0], YArray[0]), Point(XArray[1], YArray[1])]*/);
				i++; XArray[xIdx] += xi; YArray[yIdx] += yi;
				Canvas->Pen->Color = clBtnShadow;
				tmp[0] = Point(XArray[0], YArray[0]); tmp[1] = Point(XArray[1], YArray[1]);
				Canvas->Polyline(tmp,2/*[Point(XArray[0], YArray[0]), Point(XArray[1], YArray[1])]*/);
				i++; XArray[xIdx] += xi; YArray[yIdx] += yi;
				tmp[0] = Point(XArray[0], YArray[0]); tmp[1] = Point(XArray[1], YArray[1]);
				Canvas->Polyline(tmp,2/*[Point(XArray[0], YArray[0]), Point(XArray[1], YArray[1])]*/);
				i++; XArray[xIdx] +=xi; YArray[yIdx] += yi;
				Canvas->Pen->Color = clBtnFace;
				tmp[0] = Point(XArray[0], YArray[0]); tmp[1] = Point(XArray[1], YArray[1]);
				Canvas->Polyline(tmp,2/*[Point(XArray[0], YArray[0]), Point(XArray[1], YArray[1])]*/);
				i++; XArray[xIdx] += xi; YArray[yIdx] += yi;
			}
			return;
		}

		TBitmap   *TmpBmp  = CreateBmp24(Width, Height);

		TmpBmp->Canvas->Brush->Color = DefaultManager->GetGlobalColor();
		TmpBmp->Canvas->FillRect(TRect(0,0,Width, Height));

		int i = DefaultManager->GetMaskIndex(DefaultManager->ConstData.IndexGLobalInfo, s_GlobalInfo, s_GripImage);

		int glIndex = i;
		int grHeight = (DefaultManager->ma[glIndex].Bmp)?(DefaultManager->ma[glIndex].Bmp->Height / 2):(HeightOf(DefaultManager->ma[glIndex].R) / (DefaultManager->ma[glIndex].MaskType + 1));
		if (DefaultManager->ma[glIndex].ImageCount == 0)
			DefaultManager->ma[glIndex].ImageCount = 1;
		int grWidth = (DefaultManager->ma[glIndex].Bmp)?(DefaultManager->ma[glIndex].Bmp->Width / 3):(WidthOf(DefaultManager->ma[glIndex].R) / DefaultManager->ma[glIndex].ImageCount);

		if (Width - grWidth < 0)
			grWidth = Width;
		if (Height - grHeight < 0)
			grHeight = Height;

		if (DefaultManager->IsValidImgIndex(i))
			 DrawSkinGlyph(TmpBmp,	Point(Width - grWidth ,	Height - grHeight) , 0, 1, DefaultManager->ma[i],MakeCacheInfo(TmpBmp));

		TBitmap   *TmpBmp2  = CreateBmp24(Width, Height);
		if (Position == Toolctrlseh::sgpBottomRight)
		{
			TmpBmp2->Assign(TmpBmp); // nop
		} else if (Position == Toolctrlseh::sgpBottomLeft)
		{	// отразить по горизантали
			RotateBmp90(TmpBmp,TmpBmp2);
			TmpBmp->Assign(TmpBmp2);
			RotateBmp90(TmpBmp,TmpBmp2);
			TmpBmp->Assign(TmpBmp2);
			RotateBmp90(TmpBmp,TmpBmp2);
		} else if (Position == Toolctrlseh::sgpTopLeft)
		{
			RotateBmp180(TmpBmp,TmpBmp2);
			TmpBmp->Assign(TmpBmp2);
			RotateBmp90(TmpBmp,TmpBmp2);
		} else //  Position = sgpTopRight
		{
			RotateBmp180(TmpBmp,TmpBmp2); // отразить по вертикали
		}

		Canvas->Draw(0,0,TmpBmp2);
		delete TmpBmp,TmpBmp2;
	};
	__fastcall TSkinedSizeGripEh(HWND ParentWindow): Toolctrlseh::TSizeGripEh(ParentWindow) {};

};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
} // end acEhLibPainter
//--
#ifdef ACEHLIBPAINTER_FlatImproving
// сейчас это не нужно
#pragma alias	"@Dbctrlseh@TCustomDBEditEh@UpdateEditButtonControlsState$qqrv"="@TMyCustomDBEditEh@UpdateEditButtonControlsState$qqrv"
class PACKAGE TMyCustomDBEditEh : public TCustomDBEditEh
{
	virtual void __fastcall UpdateEditButtonControlsState(void)
	{
		if (FEditButtonControlList.Length == 0)
			return;
		if (!Enabled)
				FEditButtonControlList[0].EditButtonControl->Enabled = ButtonEnabled();
			else
				FEditButtonControlList[0].EditButtonControl->Enabled = EditButton->Enabled;

		FEditButtonControlList[0].EditButtonControl->Active = FBorderActive;
		FEditButtonControlList[0].ButtonLine->Pen->Color = Color;
		FEditButtonControlList[0].EditButtonControl->Flat = false;

		for (int i = 1; i <= FEditButtonControlList.Length - 1; i++)
		{
			if (!Enabled)
				FEditButtonControlList[i].EditButtonControl->Enabled = ButtonEnabled();
			else
				FEditButtonControlList[i].EditButtonControl->Enabled = EditButtons->Items[i-1]->Enabled;

			FEditButtonControlList[i].EditButtonControl->Active = FBorderActive;
			FEditButtonControlList[i].EditButtonControl->Flat = false;
			FEditButtonControlList[i].EditButtonControl->Tag = -98;
			FEditButtonControlList[i].ButtonLine->Pen->Color = Color;
		}
	};
		virtual void __fastcall PaintWindow(HDC DC){};

	inline __fastcall TMyCustomDBEditEh(HWND ParentWindow) : TCustomDBEditEh(ParentWindow) { };
};
#endif

#pragma alias	"@Toolctrlseh@TPopupListboxEh@CreateWnd$qqrv"="@aTPopupListboxEh@CreateWnd$qqrv"
class PACKAGE aTPopupListboxEh : public TPopupListboxEh
{
public:
	virtual void __fastcall CreateWnd(void)
	{
		Stdctrls::TCustomListBox::CreateWnd();
		::SetParent(Handle, 0);
		CallWindowProc((FARPROC)DefWndProc, Handle, WM_SETFOCUS, 0, 0);

		TsSkinProvider *SProv = GetSkinProvider(this);
		if (!SProv)
			return;
		int i = SProv->Adapter->IndexOf(this);
		if (i != -1)
				SProv->Adapter->RemoveItem(i);
		SProv->Adapter->AddNewItem(this);
	};
	__fastcall virtual aTPopupListboxEh(Classes::TComponent* Owner) : TPopupListboxEh(Owner) {};
};

#pragma alias	"@Dbctrlseh@TCustomDBComboBoxEh@InternalUpdatePostData$qqrv"="@TMyTInplaceFilterEditEh@aInternalUpdatePostData$qqrv"
class PACKAGE TMyTInplaceFilterEditEh : public TDBComboBoxEh
{
public:
	virtual void __fastcall aInternalUpdatePostData(void)
	{
		if (DataIndepended() && !FKeyBased)
			FDataLink->SetText(EditText);
		else
			FDataLink->SetText(VarToStr(Value));

		TsSkinProvider *SProv = GetSkinProvider(this);
		if (!SProv)
			return;
		int i = SProv->Adapter->IndexOf(this);
		if (i == -1)
			SProv->Adapter->AddNewItem(this);
	};
	virtual void __fastcall PaintWindow(HDC DC)	{};
	__fastcall virtual TMyTInplaceFilterEditEh(Classes::TComponent* Owner) : TDBComboBoxEh(Owner) {};
};
void acEhLibPainter_start()
{
	TDBGridEhStyle* NewGridDefaultStyle = new acEhLibPainter::TMyDBGridEhStyle();
	delete SetDBGridEhDefaultStyle(NewGridDefaultStyle);
}

// дл€ "перехвата" используюетс€ недокументиированна€ возможность линкера "подмен€ть" функции (#pragma alias )
// на Delphi вместо этого можно использовать либо способ перехвата из библиотеки FastCode
// либо не перехватывать ничего, а сразу править исходника EhLib

// SkinCheckBox  - —кинирование CheckBox - внутри грида выгл€дит не красиво - лучше его выключить (SkinCheckBox)
// #pragma alias "@Toolctrlseh@DrawCheck$qqruirx11Types@TRect23Stdctrls@TCheckBoxStateoooo"="@acEhLibPainter@DrawCheck$qqrpvrx11Types@TRect23Stdctrls@TCheckBoxStateoooo"

// SkinDropDownButton
#pragma alias "@Toolctrlseh@DrawDropDownButton$qqruirx11Types@TRectoooo"="@acEhLibPainter@DrawDropDownButton$qqrpvrx11Types@TRectoooo"

// SkinEllipsisButton
#pragma alias "@Toolctrlseh@DrawEllipsisButton$qqrp16Graphics@TCanvasrx11Types@TRectoooo"="@acEhLibPainter@DrawEllipsisButton$qqrp16Graphics@TCanvasrx11Types@TRectoooo"

// SkinPlusMinusButton
#pragma alias "@Toolctrlseh@DrawPlusMinusButton$qqrp16Graphics@TCanvasrx11Types@TRectooooo"="@acEhLibPainter@DrawPlusMinusButton$qqrp16Graphics@TCanvasrx11Types@TRectooooo"

// SkinUpDownButton
#pragma alias "@Toolctrlseh@DrawUpDownButton$qqrp16Graphics@TCanvasrx11Types@TRectooooo"="@acEhLibPainter@DrawUpDownButton$qqrp16Graphics@TCanvasrx11Types@TRectooooo"

// SkinSizeGripEh
#pragma alias "@Toolctrlseh@TSizeGripEh@Paint$qqrv"="@acEhLibPainter@TSkinedSizeGripEh@Paint$qqrv"

#pragma startup acEhLibPainter_start
