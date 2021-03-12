object FormEditaCurva: TFormEditaCurva
  Left = 526
  Top = 203
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Editor de Curva'
  ClientHeight = 177
  ClientWidth = 300
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 294
    Height = 41
    Align = alTop
    Caption = 'Intervalo de tempo da curva'
    TabOrder = 0
    object CBoxNumPto: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 18
      Width = 284
      Height = 21
      Align = alClient
      Style = csDropDownList
      TabOrder = 0
      OnChange = CBoxNumPtoChange
      Items.Strings = (
        '24 horas (1 ponto)'
        '12 horas (2 pontos)'
        '  8 horas (3 pontos)'
        '  6 horas (4 pontos)'
        '  4 horas (6 pontos)'
        '  3 horas (8 pontos)'
        '  2 horas (12 pontos)'
        '  1 hora (24 pontos)'
        '15 minutos (96 pontos)'
        '10 minutos (144 pontos)'
        '  5 minutos (288 pontos)'
        'Vari'#225'vel por per'#237'odo')
      ExplicitLeft = 7
      ExplicitTop = 17
    end
  end
  object GroupBox3: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 50
    Width = 294
    Height = 124
    Align = alClient
    Caption = 'Pontos da curva'
    TabOrder = 1
    object SGrid: TStringGrid
      Left = 2
      Top = 15
      Width = 290
      Height = 107
      Align = alClient
      ColCount = 4
      DefaultColWidth = 66
      DefaultRowHeight = 16
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs, goAlwaysShowEditor]
      TabOrder = 0
      ColWidths = (
        76
        66
        66
        66)
    end
  end
end
