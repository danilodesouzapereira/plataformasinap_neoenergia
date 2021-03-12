object FormCorTensao: TFormCorTensao
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  ClientHeight = 143
  ClientWidth = 184
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
  object lblV1: TLabel
    Left = 15
    Top = 13
    Width = 72
    Height = 13
    Caption = 'abaixo de 0,92'
  end
  object lblV2: TLabel
    Left = 15
    Top = 39
    Width = 85
    Height = 13
    Caption = 'entre 0,92 e 0,97'
  end
  object lblV3: TLabel
    Left = 15
    Top = 65
    Width = 85
    Height = 13
    Caption = 'entre 0,97 e 1,02'
  end
  object lblV5: TLabel
    Left = 15
    Top = 117
    Width = 67
    Height = 13
    Caption = 'acima de 1,08'
  end
  object lblV4: TLabel
    Left = 15
    Top = 91
    Width = 85
    Height = 13
    Caption = 'entre 1,02 e 1,08'
  end
  object PanCorV1: TPanel
    Left = 120
    Top = 10
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 0
    OnClick = PanCorV1Click
  end
  object PanCorV2: TPanel
    Left = 120
    Top = 36
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 1
    OnClick = PanCorV2Click
  end
  object PanCorV3: TPanel
    Left = 120
    Top = 62
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 2
    OnClick = PanCorV3Click
  end
  object PanCorV4: TPanel
    Left = 120
    Top = 88
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 3
    OnClick = PanCorV4Click
  end
  object PanCorV5: TPanel
    Left = 120
    Top = 114
    Width = 50
    Height = 20
    ParentBackground = False
    TabOrder = 4
    OnClick = PanCorV5Click
  end
  object ColorDialog: TColorDialog
    Left = 80
    Top = 54
  end
end
