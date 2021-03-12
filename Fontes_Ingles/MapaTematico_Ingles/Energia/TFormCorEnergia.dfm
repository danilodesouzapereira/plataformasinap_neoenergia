object FormCorEnergia: TFormCorEnergia
  Left = 0
  Top = 0
  BorderStyle = bsNone
  ClientHeight = 40
  ClientWidth = 219
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lblCor: TLabel
    Left = 45
    Top = 13
    Width = 54
    Height = 13
    Caption = 'Cor padr'#227'o'
  end
  object PanCor: TPanel
    Left = 120
    Top = 10
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 0
    OnClick = PanCorClick
  end
  object ColorDialog: TColorDialog
    Left = 184
    Top = 8
  end
end
