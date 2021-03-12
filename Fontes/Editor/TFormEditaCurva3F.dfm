object FormEditaCurva3F: TFormEditaCurva3F
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'FormEditaCurva3F'
  ClientHeight = 233
  ClientWidth = 378
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object PanelCombos: TPanel
    Left = 0
    Top = 0
    Width = 378
    Height = 50
    Align = alTop
    AutoSize = True
    TabOrder = 0
    object gbPatamares: TGroupBox
      AlignWithMargins = True
      Left = 158
      Top = 4
      Width = 216
      Height = 42
      Align = alRight
      Caption = 'N'#250'mero de Patamares'
      TabOrder = 0
      object CBoxNumPto: TComboBox
        AlignWithMargins = True
        Left = 5
        Top = 15
        Width = 206
        Height = 21
        Margins.Top = 0
        Align = alClient
        Style = csDropDownList
        TabOrder = 0
        OnChange = CBoxNumPtoChange
        Items.Strings = (
          '24 horas (1 ponto)'
          '12 horas (2 pontos)'
          ' 8 horas (3 pontos)'
          ' 6 horas (4 pontos)'
          ' 4 horas (6 pontos)'
          ' 3 horas (8 pontos)'
          ' 2 horas (12 pontos)'
          ' 1 hora (24 pontos)'
          '15 minutos (96 pontos)'
          '10 minutos (144 pontos)'
          ' 5 minutos (288 pontos)'
          'Vari'#225'vel por per'#237'odo')
        ExplicitLeft = 7
        ExplicitTop = 18
      end
    end
    object gbGrandeza: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 148
      Height = 42
      Align = alClient
      Caption = 'Tipo'
      TabOrder = 1
      object CBoxTipoSup: TComboBox
        AlignWithMargins = True
        Left = 5
        Top = 15
        Width = 138
        Height = 21
        Margins.Top = 0
        Align = alClient
        Style = csDropDownList
        TabOrder = 0
        OnChange = CBoxTipoSupChange
        Items.Strings = (
          'P/V'
          'P/Q'
          'V/Teta')
      end
    end
  end
  object gbPontos: TGroupBox
    AlignWithMargins = True
    Left = 3
    Top = 53
    Width = 372
    Height = 177
    Align = alClient
    Caption = 'Pontos da Curva'
    TabOrder = 1
    object SGrid: TStringGrid
      Left = 2
      Top = 15
      Width = 368
      Height = 141
      Align = alClient
      ColCount = 7
      DefaultColWidth = 53
      DefaultRowHeight = 16
      RowCount = 3
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
      TabOrder = 0
      RowHeights = (
        16
        16
        16)
    end
    object sbUnidades: TStatusBar
      Left = 2
      Top = 156
      Width = 368
      Height = 19
      Panels = <>
      SimplePanel = True
      SimpleText = 'P: kW, Q: kvar, V: pu, Teta: graus'
    end
  end
end
