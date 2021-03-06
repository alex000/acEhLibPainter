﻿acEhLibPainter
========================

Цели:
     Этот модуль предназначен для скинирования компонентов EhLib с использованием AlphaControls

   На самом деле - большая часть кода здесь - это переведенные оригинальные методы прорисовки EhLib
   их потребовалось перевести на с++ чтобы могла работать прорисовка с выключенными скинами.

   Для "перехвата" прорисовки компонентов EhLib используется недокументированная
   возможность линкера "подменять" функции (#pragma alias ).
   На Delphi вместо этого можно использовать либо способ перехвата из библиотеки FastCode
   либо не перехватывать ничего, а сразу править исходника EhLib

   К сожалению, для перехвата нужных функций прорисовки  - придется подправить ToolCtrlsEh.pas
   тк они не вынесены в интерфейс  - получить указатель на них или unmangled-имя невозможно
   придется  - перенести некоторые функции из раздела implementation в раздел interface

Применение:
   - Во первых нужно пропатчить EhLib - diff и инструкции см. в папке _eh_patch
   - Во вторых добавить в SkinManager->ThirdPartyList как Edit следующие классы:
    * TDBEditEh
    * TDBNumberEditEh
    * TDBDateTimeEditEh
    * TDBLookupComboboxEh
    * TDBComboBoxEh
    * TInplaceFilterEditEh
    * TPopupListboxEh
   - Далее достаточно подключить этот модуль к проекту

Известыне недостатки/проблемы:
   - TDBLookupGridEh не скинится
     там 2 проблемы - во первых он создается динамически,
     и во вторых у него "своя" обработка скроллов (см. TDBLookupGridEh.UpdateScrollBar())
   - Прит смене скина кнопки Eh controls не реагируют на смену скина :(
   - Eh controls скинятся DefaultManager`ом, а не тем которым скинится форма
   - Кнопки со стилем ebsGlyphEh не скинятся (а это вообще надо ?)
   - Не выключайте BitmapCache (Toolctrlseh::UseButtonsBitmapCache) - иначе будет поглючивать
   - Скинирование CheckBox - внутри грида выглядит не красиво - лучше его выключить
     чтобы включить - см. SkinCheckBox в этом модуле
   - Не скинируется сетка грида (видно на темных скинах)
   - Нужно скинировать DBGridEhInplaceSearchColor / DBGridEhInplaceSearchTextColor

Автор: alex000 @ [http://www.alphaskins.com/forum/](http://www.alphaskins.com/forum/)