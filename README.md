acEhLibPainter
========================

����:
		 ���� ������ ������������ ��� ������������ ����������� EhLib � �������������� AlphaControls

		 �� ����� ���� - ������� ����� ���� ����� - ��� ������������ ������������ ������ ���������� EhLib
	 �� ������������� ��������� �� �++ ����� ����� �������� ���������� � ������������ �������.

	 ��� "���������" ���������� ����������� EhLib ������������ �������������������
	 ����������� ������� "���������" ������� (#pragma alias ).
	 �� Delphi ������ ����� ����� ������������ ���� ������ ��������� �� ���������� FastCode
	 ���� �� ������������� ������, � ����� ������� ��������� EhLib

	 � ���������, ��� ��������� ������ ������� ����������  - �������� ���������� ToolCtrlsEh.pas
	 �� ��� �� �������� � ���������  - �������� ��������� �� ��� ��� unmangled-��� ����������
	 ��������  - ��������� ��������� ������� �� ������� implementation � ������ interface

����������:
		 - �� ������ ����� ���������� EhLib - diff � ���������� ��. � ����� _eh_patch
		 - �� ������ �������� � SkinManager->ThirdPartyList ��� Edit ��������� ������:
				TDBEditEh
				TDBNumberEditEh
				TDBDateTimeEditEh
				TDBLookupComboboxEh
				TDBComboBoxEh
				TInplaceFilterEditEh
				TPopupListboxEh
		 - ����� ���������� ���������� ���� ������ � �������

��������� ����������/��������:
	 - TDBLookupGridEh �� ��������
		 ��� 2 �������� - �� ������ �� ��������� �����������,
		 � �� ������ � ���� "����" ��������� �������� (��. TDBLookupGridEh.UpdateScrollBar())
	 - ���� ����� ����� ������ Eh controls �� ��������� �� ����� ����� :(
	 - Eh controls �������� DefaultManager`��, � �� ��� ������� �������� �����
	 - ������ �� ������ ebsGlyphEh �� �������� (� ��� ������ ���� ?)
	 - �� ���������� BitmapCache (Toolctrlseh::UseButtonsBitmapCache) - ����� ����� �����������
	 - ������������ CheckBox - ������ ����� �������� �� ������� - ����� ��� ���������
		 ����� �������� - ��. SkinCheckBox � ���� ������
	 - �� ����������� ����� ����� (����� �� ������ ������)
	 - ����� ����������� DBGridEhInplaceSearchColor / DBGridEhInplaceSearchTextColor

�����: alex000 @ [http://www.alphaskins.com/forum/](http://www.alphaskins.com/forum/)