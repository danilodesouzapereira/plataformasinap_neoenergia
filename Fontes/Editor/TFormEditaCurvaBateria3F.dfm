object FormEditaCurvaBateria3F: TFormEditaCurvaBateria3F
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'FormEditaCurvaBateria3F'
  ClientHeight = 233
  ClientWidth = 378
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelSelect: TPanel
    Left = 0
    Top = 0
    Width = 378
    Height = 50
    Align = alTop
    TabOrder = 0
    object gbTipo: TGroupBox
      Left = 1
      Top = 1
      Width = 160
      Height = 48
      Align = alClient
      Caption = 'Tipo'
      TabOrder = 0
      object CBoxTipoCarga: TComboBox
        Left = 2
        Top = 15
        Width = 156
        Height = 21
        Margins.Top = 0
        Align = alClient
        Style = csDropDownList
        TabOrder = 0
        OnChange = CBoxTipoCargaChange
        Items.Strings = (
          'Editar Carga Total'
          'Editar Carga por Fase')
      end
    end
    object gpPatamares: TGroupBox
      Left = 161
      Top = 1
      Width = 216
      Height = 48
      Align = alRight
      Caption = 'N'#250'mero de Patamares'
      TabOrder = 1
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
    object sbUnidades: TStatusBar
      Left = 2
      Top = 156
      Width = 368
      Height = 19
      Panels = <>
      SimplePanel = True
      SimpleText = 'P: kW, Q: kVAr'
    end
    object SGrid: TStringGrid
      Left = 2
      Top = 15
      Width = 368
      Height = 141
      Align = alClient
      ColCount = 7
      DefaultColWidth = 70
      DefaultRowHeight = 16
      RowCount = 3
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs, goAlwaysShowEditor]
      TabOrder = 1
    end
  end
end
