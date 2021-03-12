object FormEdtCurvaMed: TFormEdtCurvaMed
  Left = 493
  Top = 152
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Editor de Curva de Suprimento'
  ClientHeight = 325
  ClientWidth = 329
  Color = clBtnFace
  Constraints.MinWidth = 216
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 109
    Width = 329
    Height = 216
    ActivePage = TabSheetPonto
    Align = alClient
    TabOrder = 1
    OnChange = PageControlChange
    OnChanging = PageControlChanging
    object TabSheetPonto: TTabSheet
      Caption = 'Edi'#231#227'o dos Pontos'
      object GroupBox3: TGroupBox
        Left = 0
        Top = 0
        Width = 321
        Height = 188
        Align = alClient
        Caption = 'Pontos da curva'
        TabOrder = 0
        object SGrid: TStringGrid
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 311
          Height = 165
          Align = alClient
          ColCount = 3
          DefaultColWidth = 65
          DefaultRowHeight = 16
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
          TabOrder = 0
        end
      end
    end
    object TabSheetChart: TTabSheet
      Caption = 'Visualiza'#231#227'o Gr'#225'fica'
      ImageIndex = 1
      object chart: TChart
        Left = 0
        Top = 0
        Width = 321
        Height = 188
        Legend.Alignment = laBottom
        Legend.ColorWidth = 10
        Legend.Symbol.Width = 10
        Legend.TextStyle = ltsRightPercent
        Legend.TopPos = 0
        Legend.Visible = False
        MarginBottom = 5
        MarginLeft = 5
        MarginTop = 1
        Title.Text.Strings = (
          'Pot'#234'ncia Ativa')
        BottomAxis.AxisValuesFormat = '#,##0'
        BottomAxis.ExactDateTime = False
        BottomAxis.Increment = 1.000000000000000000
        BottomAxis.LabelsSeparation = 1
        BottomAxis.LabelStyle = talValue
        LeftAxis.AxisValuesFormat = '#,##0.#0'
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 0.100000000000000000
        LeftAxis.LabelsSeparation = 1
        LeftAxis.LabelStyle = talValue
        Panning.InsideBounds = True
        Panning.MouseWheel = pmwNone
        RightAxis.Visible = False
        TopAxis.Visible = False
        View3D = False
        View3DOptions.Elevation = 315
        View3DOptions.Orthogonal = False
        View3DOptions.Perspective = 0
        View3DOptions.Rotation = 360
        View3DWalls = False
        Zoom.Animated = True
        Zoom.UpLeftZooms = True
        ZoomWheel = pmwNormal
        Align = alClient
        BevelOuter = bvNone
        Enabled = False
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          29
          15
          29)
        ColorPaletteIndex = 13
        object Series1: TFastLineSeries
          Marks.Style = smsValue
          LinePen.Color = clGreen
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 329
    Height = 50
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object GroupBox5: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 233
      Height = 44
      Align = alClient
      Caption = 'C'#243'digo'
      TabOrder = 0
      object EditCodigo: TEdit
        AlignWithMargins = True
        Left = 5
        Top = 18
        Width = 223
        Height = 21
        Align = alClient
        TabOrder = 0
      end
    end
    object GBoxTolerancia: TGroupBox
      AlignWithMargins = True
      Left = 242
      Top = 3
      Width = 84
      Height = 44
      Align = alRight
      Caption = 'Toler'#226'ncia (%)'
      Constraints.MinWidth = 84
      TabOrder = 1
      object EditTolerancia: TEdit
        AlignWithMargins = True
        Left = 5
        Top = 18
        Width = 74
        Height = 21
        Align = alClient
        TabOrder = 0
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 50
    Width = 329
    Height = 59
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object GroupBox2: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 323
      Height = 53
      Align = alClient
      Caption = 'Intervalo de tempo da curva'
      TabOrder = 0
      object CBoxNumPto: TComboBox
        AlignWithMargins = True
        Left = 5
        Top = 18
        Width = 313
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
      end
    end
  end
end
