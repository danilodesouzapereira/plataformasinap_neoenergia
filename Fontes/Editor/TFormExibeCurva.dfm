object FormExibeCurva: TFormExibeCurva
  Left = 526
  Top = 203
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Editor de Curva'
  ClientHeight = 205
  ClientWidth = 307
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
  object Chart: TChart
    Left = 0
    Top = 41
    Width = 307
    Height = 103
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
      '')
    BottomAxis.AxisValuesFormat = '#,##0'
    BottomAxis.ExactDateTime = False
    BottomAxis.Increment = 1.000000000000000000
    BottomAxis.LabelsSeparation = 1
    BottomAxis.LabelStyle = talValue
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.AxisValuesFormat = '#,##0.#0'
    LeftAxis.ExactDateTime = False
    LeftAxis.Increment = 0.100000000000000000
    LeftAxis.LabelsSeparation = 1
    LeftAxis.LabelStyle = talValue
    LeftAxis.Maximum = 697.500000000000000000
    LeftAxis.Minimum = 1.200000000000045000
    RightAxis.Visible = False
    TopAxis.Visible = False
    View3D = False
    View3DOptions.Elevation = 315
    View3DOptions.Orthogonal = False
    View3DOptions.Perspective = 0
    View3DOptions.Rotation = 360
    View3DWalls = False
    Align = alClient
    BevelOuter = bvNone
    Enabled = False
    TabOrder = 0
    OnClick = ChartClick
    ExplicitTop = 42
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TFastLineSeries
      Legend.Visible = False
      Marks.Frame.Style = psDot
      Marks.Frame.SmallDots = True
      Marks.Frame.Visible = False
      Marks.Style = smsLegend
      Marks.Clip = True
      ShowInLegend = False
      LinePen.Color = clRed
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TFastLineSeries
      Marks.Style = smsValue
      SeriesColor = clRed
      LinePen.Color = clGreen
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series3: TFastLineSeries
      Marks.Style = smsValue
      SeriesColor = clBlue
      LinePen.Color = clBlue
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series4: TFastLineSeries
      Marks.Style = smsValue
      SeriesColor = 33023
      LinePen.Color = 33023
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series5: TFastLineSeries
      Marks.Style = smsValue
      SeriesColor = 16777088
      LinePen.Color = 16777088
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series6: TFastLineSeries
      Marks.Style = smsValue
      SeriesColor = 16744703
      LinePen.Color = 16744703
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object RGroupSerie: TRadioGroup
    Left = 0
    Top = 144
    Width = 307
    Height = 61
    Align = alBottom
    Columns = 2
    Items.Strings = (
      'Curva 1'
      'Curva 2'
      'Curva 3'
      'Curva 4'
      'Curva 5'
      'Curva 6')
    TabOrder = 1
    OnClick = ChartClick
  end
  object gbTipo: TGroupBox
    Left = 0
    Top = 0
    Width = 307
    Height = 41
    Align = alTop
    Caption = 'Tipo'
    TabOrder = 2
    object CBoxTipoCurva: TComboBox
      Left = 2
      Top = 15
      Width = 303
      Height = 24
      Margins.Top = 0
      Align = alClient
      Style = csDropDownList
      TabOrder = 0
      OnChange = CBoxTipoCurvaChange
      Items.Strings = (
        'Valor Total'
        'Valor por Fase')
    end
  end
end
