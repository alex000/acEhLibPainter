Для применения патча скопируйте содержимое этого каталога, и запусите ExecPatch.bat

Главное исрпавление в файле  ToolCtrlsEh.pas
Нужно перенести - пренести некоторые функции из раздела implementation в раздел interface:
procedure DrawCheck(DC: HDC; R: TRect; AState: TCheckBoxState; AEnabled, AFlat, ADown, AActive: Boolean);  
procedure DrawDropDownButton(DC: HDC; ARect: TRect; Enabled, Flat, Active, Down: Boolean);                 
procedure DrawUpDownButton(DC: HDC; ARect: TRect; Enabled, Flat, Active, Down, DownDirection: Boolean);    
procedure DrawEllipsisButton(DC: HDC; ARect: TRect; Enabled, Active, Flat, Pressed: Boolean);              
procedure DrawPlusMinusButton(DC: HDC; ARect: TRect; Enabled, Active, Flat, Pressed, Plus: Boolean);       

К сожалению, полное скинировиние при Flat = true невозможно :(
Чтобы не заморачиватся, я пропатчил DBGridEh.pas понаставив везде Flat := false;