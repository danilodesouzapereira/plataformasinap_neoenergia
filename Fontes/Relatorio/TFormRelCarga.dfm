object FormRelCarga: TFormRelCarga
  Left = 51
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Relat'#243'rio de Demanda Nominal das Redes'
  ClientHeight = 465
  ClientWidth = 934
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 934
    Height = 24
    AutoSize = True
    ButtonWidth = 111
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object PanelTipoRel: TPanel
      Left = 0
      Top = 0
      Width = 233
      Height = 22
      BevelOuter = bvNone
      TabOrder = 1
      object Label2: TLabel
        Left = 7
        Top = 5
        Width = 81
        Height = 13
        Caption = 'Tipo de Relat'#243'rio'
      end
      object CBoxRelatorio: TComboBox
        Left = 94
        Top = -3
        Width = 128
        Height = 21
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 0
        Text = 'Demanda Pr'#243'pria'
        OnChange = CBoxRelatorioChange
        Items.Strings = (
          'Demanda Pr'#243'pria'
          'Demanda Agregada')
      end
    end
    object ToolButton3: TToolButton
      Left = 233
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object PanelUnidade: TPanel
      Left = 241
      Top = 0
      Width = 198
      Height = 22
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 0
      object Label1: TLabel
        Left = 7
        Top = 5
        Width = 91
        Height = 13
        Caption = 'Selecionar unidade'
      end
      object CBoxUnidade: TComboBox
        Left = 104
        Top = 1
        Width = 75
        Height = 21
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 0
        Text = 'S (kVA)'
        OnChange = CBoxUnidadeChange
        Items.Strings = (
          'S (kVA)'
          'P (kW)'
          'Q (kvAr)')
      end
    end
    object ToolButton2: TToolButton
      Left = 439
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object ToolButton1: TToolButton
      Left = 447
      Top = 0
      Action = ActionExporta
      AutoSize = True
    end
    object ToolButton4: TToolButton
      Left = 562
      Top = 0
      Width = 8
      Caption = 'ToolButton4'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object ToolButton5: TToolButton
      Left = 570
      Top = 0
      Action = ActionHelp
      AutoSize = True
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 24
    Width = 934
    Height = 441
    ActivePage = TabSheetGrafico
    Align = alClient
    TabHeight = 30
    TabOrder = 1
    OnChange = PageControlChange
    object TabSheetPlanilha: TTabSheet
      Caption = 'Planilha de Demanda de Todas as Redes'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LView: TListView
        Left = 0
        Top = 0
        Width = 926
        Height = 401
        Align = alClient
        Columns = <
          item
            Caption = 'Tipo de Rede'
            Width = 100
          end
          item
            Caption = 'Rede'
            Width = 100
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.01'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.02'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.03'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.04'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.05'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.06'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.07'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.08'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.09'
            Width = 78
          end
          item
            Caption = 'Pat.10'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.11'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.12'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.13'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.14'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.15'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.16'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.17'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.18'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.19'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.20'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.21'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.22'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.23'
            Width = 78
          end
          item
            Alignment = taRightJustify
            Caption = 'Pat.24'
            Width = 78
          end>
        GridLines = True
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object TabSheetGrafico: TTabSheet
      Caption = 'Gr'#225'fico de Demanda por Rede'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GBoxHieraquia: TGroupBox
        Left = 0
        Top = 0
        Width = 209
        Height = 401
        Align = alLeft
        Caption = 'Hierarquia das Redes'
        TabOrder = 0
        object TView: TTreeView
          Left = 2
          Top = 36
          Width = 205
          Height = 363
          Align = alClient
          BorderStyle = bsNone
          HideSelection = False
          Indent = 19
          ReadOnly = True
          RowSelect = True
          TabOrder = 0
          OnChange = TViewChange
        end
        object CBoxClasses: TComboBox
          Left = 2
          Top = 15
          Width = 205
          Height = 21
          Align = alTop
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 1
          Text = 'Todas as Classes'
          OnChange = CBoxClassesChange
          Items.Strings = (
            'Todas as Classes')
        end
      end
      object GBoxDemPropria: TGroupBox
        Left = 209
        Top = 0
        Width = 261
        Height = 401
        Align = alLeft
        Caption = 'Demanda por Patamar'
        TabOrder = 1
        object SGridDemPropria: TStringGrid
          Left = 2
          Top = 15
          Width = 257
          Height = 384
          Align = alClient
          ColCount = 4
          DefaultColWidth = 50
          DefaultRowHeight = 14
          RowCount = 25
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goTabs, goRowSelect]
          TabOrder = 0
          ExplicitLeft = 0
          ExplicitTop = 14
          ColWidths = (
            79
            50
            50
            50)
        end
      end
      object GBoxGrafico: TGroupBox
        Left = 470
        Top = 0
        Width = 456
        Height = 401
        Align = alClient
        Caption = 'Gr'#225'fico de Demanda '
        TabOrder = 2
        object chart: TChart
          Left = 2
          Top = 15
          Width = 452
          Height = 384
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
          BottomAxis.AxisValuesFormat = '#,##0'
          BottomAxis.ExactDateTime = False
          BottomAxis.Increment = 1.000000000000000000
          BottomAxis.LabelsSeparation = 1
          BottomAxis.LabelStyle = talValue
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
          LeftAxis.LabelsSeparation = 1
          LeftAxis.LabelStyle = talValue
          LeftAxis.MaximumRound = True
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          RightAxis.Title.Caption = 'Patamar (h)'
          RightAxis.Title.Position = tpEnd
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
          object Series1: TFastLineSeries
            Marks.Frame.Style = psDot
            Marks.Frame.SmallDots = True
            Marks.Frame.Visible = False
            Marks.Style = smsLegend
            Marks.Clip = True
            Title = 'S[kVA]'
            LinePen.Color = clRed
            LinePen.Width = 2
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Y'
            YValues.Order = loNone
          end
          object Series2: TFastLineSeries
            Marks.Style = smsValue
            SeriesColor = clBlue
            Title = 'P[kW]'
            LinePen.Color = clBlue
            LinePen.Width = 2
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Y'
            YValues.Order = loNone
          end
          object Series3: TFastLineSeries
            Marks.Style = smsValue
            SeriesColor = clRed
            Title = 'Q[kvar]'
            LinePen.Color = clGreen
            LinePen.Width = 2
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Y'
            YValues.Order = loNone
          end
        end
      end
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 24
    Top = 96
    object ActionExporta: TAction
      Caption = 'Exportar Relat'#243'rio'
      ImageIndex = 0
      OnExecute = ActionExportaExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      Hint = 'Ajuda'
      ImageIndex = 1
      OnExecute = ActionHelpExecute
    end
  end
  object ImageList: TImageList
    Left = 64
    Top = 96
    Bitmap = {
      494C0101020004007C0010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF00000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000BFBF
      BF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      000000000000000000000000FFFFFF00FFDFFFFF00000000FFCFF83F00000000
      FFC7E00F000000000003C0070000000000018003000000000000800300000000
      000100010000000000030001000000000007000100000000000F000100000000
      001F000100000000007F80030000000000FF80030000000001FFC00700000000
      03FFE00F00000000FFFFF83F0000000000000000000000000000000000000000
      000000000000}
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.txt'
    Filter = 'Arquivo texto (*.txt)|*.txt|Arquivo Excel (*.xls)|*.xls'
    Left = 104
    Top = 96
  end
end
