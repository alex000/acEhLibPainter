��� ���������� ����� ���������� ���������� ����� ��������, � �������� ExecPatch.bat

������� ����������� � �����  ToolCtrlsEh.pas
����� ��������� - �������� ��������� ������� �� ������� implementation � ������ interface:
procedure DrawCheck(DC: HDC; R: TRect; AState: TCheckBoxState; AEnabled, AFlat, ADown, AActive: Boolean);  
procedure DrawDropDownButton(DC: HDC; ARect: TRect; Enabled, Flat, Active, Down: Boolean);                 
procedure DrawUpDownButton(DC: HDC; ARect: TRect; Enabled, Flat, Active, Down, DownDirection: Boolean);    
procedure DrawEllipsisButton(DC: HDC; ARect: TRect; Enabled, Active, Flat, Pressed: Boolean);              
procedure DrawPlusMinusButton(DC: HDC; ARect: TRect; Enabled, Active, Flat, Pressed, Plus: Boolean);       

� ���������, ������ ������������ ��� Flat = true ���������� :(
����� �� �������������, � ��������� DBGridEh.pas ���������� ����� Flat := false;