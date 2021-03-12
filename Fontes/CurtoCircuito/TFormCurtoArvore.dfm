object FormCurtoArvore: TFormCurtoArvore
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Curto: Relat'#243'rio'
  ClientHeight = 290
  ClientWidth = 294
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object TView: TTreeView
    Left = 0
    Top = 25
    Width = 294
    Height = 265
    Align = alClient
    BorderStyle = bsNone
    HideSelection = False
    Indent = 19
    ReadOnly = True
    SortType = stText
    TabOrder = 0
    OnAdvancedCustomDrawItem = TViewAdvancedCustomDrawItem
    OnChange = TViewChange
    OnDblClick = TViewDblClick
  end
  object TViewHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 294
    Height = 25
    Sections = <
      item
        Alignment = taCenter
        ImageIndex = -1
        MinWidth = 140
        Text = 'Atributo'
        Width = 140
      end
      item
        Alignment = taCenter
        AutoSize = True
        ImageIndex = -1
        Text = 'Valor'
        Width = 154
      end>
    Style = hsFlat
    OnSectionResize = TViewHeaderSectionResize
  end
end
