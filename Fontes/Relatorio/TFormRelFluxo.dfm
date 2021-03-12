object FormRelFluxo: TFormRelFluxo
  Left = 46
  Top = 51
  BorderIcons = [biSystemMenu]
  Caption = 'Relat'#243'rio de Fluxo de Pot'#234'ncia'
  ClientHeight = 595
  ClientWidth = 840
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SplitterFiltro: TSplitter
    Left = 193
    Top = 29
    Width = 4
    Height = 566
    ExplicitTop = 25
    ExplicitHeight = 532
  end
  object SplitterPlanilha: TSplitter
    Left = 553
    Top = 29
    Width = 4
    Height = 566
    ExplicitLeft = 562
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 840
    Height = 29
    ButtonHeight = 30
    ButtonWidth = 117
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ButRelBarra: TToolButton
      Left = 0
      Top = 0
      Action = ActionRelBarra
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ButRelCarga: TToolButton
      Left = 41
      Top = 0
      Action = ActionRelCarga
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ButRelDemanda: TToolButton
      Left = 85
      Top = 0
      Action = ActionRelDemanda
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ButRelTrafo: TToolButton
      Left = 142
      Top = 0
      Action = ActionRelTrafo
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ButRelTrecho: TToolButton
      Left = 232
      Top = 0
      Action = ActionRelTrecho
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ToolButton2: TToolButton
      Left = 282
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object ButDestacaEqpto: TToolButton
      Left = 290
      Top = 0
      AutoSize = True
      Caption = 'Destacar Eqptos.'
      DropdownMenu = PopupMenuDestaca
      Style = tbsDropDown
      OnClick = ActionDestacaEqptoCriticoExecute
    end
    object ToolButton1: TToolButton
      Left = 404
      Top = 0
      Action = ActionExporta
      AutoSize = True
    end
    object ToolButton5: TToolButton
      Left = 519
      Top = 0
      Action = ActionHelp
      AutoSize = True
    end
    object ToolButton3: TToolButton
      Left = 577
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 1
      Style = tbsSeparator
    end
  end
  object GBoxPlanilha: TGroupBox
    AlignWithMargins = True
    Left = 200
    Top = 32
    Width = 350
    Height = 560
    Align = alLeft
    Caption = 'N'#250'mero de Barras por N'#237'vel de Tens'#227'o'
    TabOrder = 1
    object LView: TListView
      Left = 2
      Top = 15
      Width = 346
      Height = 518
      Align = alClient
      BorderStyle = bsNone
      Columns = <
        item
          Caption = 'Patamar'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Adequada'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Prec'#225'ria'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cr'#237'tica'
          Width = 65
        end>
      GridLines = True
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
    object Panel1: TPanel
      Left = 2
      Top = 533
      Width = 346
      Height = 25
      Align = alBottom
      BorderStyle = bsSingle
      TabOrder = 1
      object CheckBoxPercentual: TCheckBox
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 142
        Height = 13
        Align = alClient
        Caption = ' Exibir valores porcentuais'
        TabOrder = 0
        OnClick = CheckBoxPercentualClick
      end
      object CheckBoxSemFluxo: TCheckBox
        AlignWithMargins = True
        Left = 152
        Top = 4
        Width = 186
        Height = 13
        Align = alRight
        Caption = 'Contabilizar equipamentos sem fluxo'
        TabOrder = 1
        OnClick = CheckBoxSemFluxoClick
      end
    end
  end
  object PanelFiltro: TPanel
    Left = 0
    Top = 29
    Width = 193
    Height = 566
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 2
    object Splitter1: TSplitter
      Left = 0
      Top = 388
      Width = 193
      Height = 4
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 397
    end
    object GBoxFiltroRede: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 66
      Width = 187
      Height = 189
      Align = alClient
      Caption = 'Selecionar redes para relat'#243'rio'
      TabOrder = 0
    end
    object GBoxFiltroClasse: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 395
      Width = 187
      Height = 168
      Align = alBottom
      Caption = 'Selecionar classe de consumidor'
      TabOrder = 1
    end
    object GBoxFiltroArranjo: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 261
      Width = 187
      Height = 124
      Align = alBottom
      Caption = 'Selecionar arranjo'
      TabOrder = 2
    end
    object RadioGroupModo: TRadioGroup
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 187
      Height = 57
      Align = alTop
      Caption = 'Sele'#231#227'o das redes'
      ItemIndex = 0
      Items.Strings = (
        'Redes ordenadas por tipo'
        'Redes ordenadas por hierarquia')
      TabOrder = 3
      OnClick = RadioGroupModoClick
    end
  end
  object PanelGrafico: TPanel
    Left = 557
    Top = 29
    Width = 283
    Height = 566
    Align = alClient
    ShowCaption = False
    TabOrder = 3
    object SplitterCharts: TSplitter
      Left = 1
      Top = 303
      Width = 281
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitWidth = 260
    end
    object GBoxGrafico24pat: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 275
      Height = 296
      Align = alTop
      Caption = 'Gr'#225'fico: 24 patamares'
      TabOrder = 0
      object Chart24pat: TChart
        Left = 2
        Top = 15
        Width = 271
        Height = 279
        Legend.Alignment = laBottom
        Legend.CheckBoxes = True
        Legend.ColorWidth = 10
        Legend.Symbol.Width = 10
        Legend.TextStyle = ltsRightPercent
        Legend.TopPos = 0
        MarginBottom = 5
        MarginLeft = 5
        MarginTop = 1
        Title.Text.Strings = (
          'Demanda <pr'#243'pria> da Rede <nome da rede>')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.AxisValuesFormat = '#,##0'
        BottomAxis.ExactDateTime = False
        BottomAxis.LabelsAngle = 90
        BottomAxis.LabelsSeparation = 1
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 1.000000000000000000
        BottomAxis.Minimum = 43013.000000000000000000
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 0.500000000000000000
        DepthAxis.Minimum = -0.500000000000000000
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 0.500000000000000000
        DepthTopAxis.Minimum = -0.500000000000000000
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.AxisValuesFormat = '#,##0.#0'
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 0.100000000000000000
        LeftAxis.LabelsSeparation = 1
        LeftAxis.LabelStyle = talValue
        LeftAxis.MaximumRound = True
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.Visible = False
        TopAxis.Visible = False
        View3D = False
        View3DOptions.Elevation = 315
        View3DOptions.Orthogonal = False
        View3DOptions.Perspective = 0
        View3DOptions.Rotation = 360
        View3DWalls = False
        Zoom.Pen.Mode = pmNotXor
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object SeriesAdequada: TFastLineSeries
          Legend.Text = 'Adequada'
          LegendTitle = 'Adequada'
          Marks.Frame.Style = psDot
          Marks.Frame.SmallDots = True
          Marks.Frame.Visible = False
          Marks.Style = smsLegend
          Marks.Clip = True
          SeriesColor = clLime
          Title = 'Adequada'
          LinePen.Color = clLime
          LinePen.Width = 2
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesPrecaria: TFastLineSeries
          Legend.Text = 'Prec'#225'ria'
          LegendTitle = 'Prec'#225'ria'
          Marks.Style = smsValue
          SeriesColor = clYellow
          Title = 'Prec'#225'ria'
          LinePen.Color = clYellow
          LinePen.Width = 2
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesCritica: TFastLineSeries
          Legend.Text = 'Cr'#237'tica'
          LegendTitle = 'Cr'#237'tica'
          Marks.Style = smsValue
          SeriesColor = clRed
          Title = 'Cr'#237'tica'
          LinePen.Color = clRed
          LinePen.Width = 2
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesSemFluxo: TFastLineSeries
          Legend.Text = 'Sem Fluxo'
          LegendTitle = 'Sem Fluxo'
          Marks.Style = smsValue
          SeriesColor = clGray
          Title = 'Sem Fluxo'
          LinePen.Color = clGray
          LinePen.Width = 2
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesTotal: TFastLineSeries
          Legend.Text = 'Total'
          LegendTitle = 'Total'
          Marks.Style = smsValue
          SeriesColor = clBlack
          Title = 'Total'
          LinePen.Width = 2
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object GBoxGrafico01pat: TGroupBox
      AlignWithMargins = True
      Left = 4
      Top = 309
      Width = 275
      Height = 253
      Align = alClient
      Caption = 'Gr'#225'fico: 1 patamar'
      TabOrder = 1
      object GBoxAnimacao: TGroupBox
        AlignWithMargins = True
        Left = 5
        Top = 206
        Width = 265
        Height = 45
        Margins.Top = 0
        Margins.Bottom = 0
        Align = alBottom
        Caption = 'Anima'#231#227'o: variar patamar'
        TabOrder = 0
        object ButTimer: TBitBtn
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 84
          Height = 22
          Action = ActionAnimacao
          Align = alLeft
          Caption = 'Anima'#231#227'o'
          Glyph.Data = {
            76010000424D7601000000000000760000002800000020000000100000000100
            04000000000000010000120B0000120B00001000000000000000000000000000
            800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
            FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
            3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
            3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
            903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
            890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
            F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
            890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
            90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
            3333333777777777333333333000003333333333377777333333}
          NumGlyphs = 2
          TabOrder = 0
        end
        object CBoxPatamar: TComboBox
          AlignWithMargins = True
          Left = 95
          Top = 18
          Width = 165
          Height = 21
          Align = alClient
          Style = csDropDownList
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = CBoxPatamarChange
        end
      end
      object Chart01pat: TChart
        Left = 2
        Top = 15
        Width = 271
        Height = 191
        AllowPanning = pmNone
        Legend.Alignment = laBottom
        Legend.CheckBoxesStyle = cbsRadio
        Title.Text.Strings = (
          'TChart')
        Title.Visible = False
        AxisBehind = False
        AxisVisible = False
        View3DOptions.Elevation = 315
        View3DOptions.Orthogonal = False
        View3DOptions.Perspective = 0
        View3DOptions.Rotation = 360
        Zoom.Allow = False
        Zoom.Pen.Mode = pmNotXor
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          28
          15
          28)
        ColorPaletteIndex = 13
        object PieSeriesComObra: TPieSeries
          Marks.Visible = False
          PercentFormat = '0.#%'
          Title = 'PieSeriesComObra'
          ValueFormat = '#,##0.#%'
          XValues.Order = loAscending
          YValues.Name = 'Pie'
          YValues.Order = loNone
          Frame.InnerBrush.BackColor = clRed
          Frame.InnerBrush.Gradient.EndColor = clGray
          Frame.InnerBrush.Gradient.MidColor = clWhite
          Frame.InnerBrush.Gradient.StartColor = 4210752
          Frame.InnerBrush.Gradient.Visible = True
          Frame.MiddleBrush.BackColor = clYellow
          Frame.MiddleBrush.Gradient.EndColor = 8553090
          Frame.MiddleBrush.Gradient.MidColor = clWhite
          Frame.MiddleBrush.Gradient.StartColor = clGray
          Frame.MiddleBrush.Gradient.Visible = True
          Frame.OuterBrush.BackColor = clGreen
          Frame.OuterBrush.Gradient.EndColor = 4210752
          Frame.OuterBrush.Gradient.MidColor = clWhite
          Frame.OuterBrush.Gradient.StartColor = clSilver
          Frame.OuterBrush.Gradient.Visible = True
          Frame.Width = 4
          OtherSlice.Legend.Visible = False
          Data = {
            0603000000000000000080404000FF0000FF0800000061646571756164610000
            000000804040FFFF0000FF0800000070726563E17269610000000000003740FF
            000000FF070000006372ED74696361}
        end
      end
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 16
    Top = 112
    object ActionDestacaEqptoAdequado: TAction
      Caption = 'Faixa adequada'
      OnExecute = ActionDestacaEqptoAdequadoExecute
    end
    object ActionDestacaEqptoCritico: TAction
      Caption = 'Faixa cr'#237'tica'
      OnExecute = ActionDestacaEqptoCriticoExecute
    end
    object ActionDestacaEqptoPrecario: TAction
      Caption = 'Faixa prec'#225'ria'
      OnExecute = ActionDestacaEqptoPrecarioExecute
    end
    object ActionExporta: TAction
      Caption = 'Exportar Relat'#243'rio'
      ImageIndex = 0
      OnExecute = ActionExportaExecute
    end
    object ActionRelBarra: TAction
      Caption = 'Barras'
      OnExecute = ActionRelBarraExecute
    end
    object ActionRelCarga: TAction
      Caption = 'Cargas'
      OnExecute = ActionRelCargaExecute
    end
    object ActionRelDemanda: TAction
      Caption = 'Demanda'
      OnExecute = ActionRelDemandaExecute
    end
    object ActionRelTrafo: TAction
      Caption = 'Transformadores'
      OnExecute = ActionRelTrafoExecute
    end
    object ActionRelTrecho: TAction
      Caption = 'Trechos'
      OnExecute = ActionRelTrechoExecute
    end
    object ActionSelLisArranjo: TAction
      OnExecute = ActionSelLisArranjoExecute
    end
    object ActionSelLisClasse: TAction
      OnExecute = ActionSelLisClasseExecute
    end
    object ActionSelLisRede: TAction
      OnExecute = ActionSelLisRedeExecute
    end
    object ActionDefineCor: TAction
      Caption = 'Definir Cor...'
      OnExecute = ActionDefineCorExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      Hint = 'Ajuda'
      ImageIndex = 1
      OnExecute = ActionHelpExecute
    end
    object ActionAnimacao: TAction
      Caption = 'Anima'#231#227'o'
      ImageIndex = 2
      OnExecute = ActionAnimacaoExecute
    end
  end
  object ImageList: TImageList
    Left = 80
    Top = 120
    Bitmap = {
      494C010103001801C40010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF00000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      FF000000FF00C0C0C000FFFFFF00FFFFFF00FFFFFF00C0C0C0000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00000000000000000000000000000000000000FF000000
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF00000000000000000000000000000000000000FF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000000000000000000000FF00C0C0C000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C0C0C0000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF0000000000000000000000FF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000000000000000000000FF00FFFFFF00FF00
      0000FFFFFF00FFFFFF00FFFFFF000000FF000000000000000000000000000000
      0000FFFFFF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF0000000000000000000000FF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000000000000000000000FF00C0C0C000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00C0C0C0000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF00000000000000000000000000000000000000FF00FFFF
      FF00FF000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000BFBF
      BF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00000000000000000000000000000000000000FF000000
      FF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      FF000000FF00C0C0C000FFFFFF00FF000000FFFFFF00C0C0C0000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFDFFFFFFFFF0000FFCFF83FF83F0000
      FFC7E00FE00F00000003C007C007000000018003800300000000800380030000
      000100010001000000030001000100000007000100010000000F000100010000
      001F000100010000007F80038003000000FF80038003000001FFC007C0070000
      03FFE00FE00F0000FFFFF83FF83F000000000000000000000000000000000000
      000000000000}
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo text (*.txt)|*.txt|Arquivo Excel (*.xls)|*.xls'
    Left = 80
    Top = 176
  end
  object PopupMenuDestaca: TPopupMenu
    Left = 80
    Top = 232
    object MenuItemFaixaAdequada: TMenuItem
      Action = ActionDestacaEqptoAdequado
    end
    object MenuItemFaixaCritica: TMenuItem
      Action = ActionDestacaEqptoCritico
    end
    object MenuItemFaixaPrecaria: TMenuItem
      Action = ActionDestacaEqptoPrecario
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object DefinirCor1: TMenuItem
      Action = ActionDefineCor
    end
  end
  object ColorDialog: TColorDialog
    Left = 16
    Top = 168
  end
  object Timer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerTimer
    Left = 16
    Top = 230
  end
end
