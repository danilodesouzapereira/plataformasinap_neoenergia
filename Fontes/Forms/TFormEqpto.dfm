object FormEqpto: TFormEqpto
  Left = 527
  Top = 156
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'FormEqpto'
  ClientHeight = 463
  ClientWidth = 328
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 328
    Height = 463
    ActivePage = TabSheetAtributo
    Align = alClient
    TabOrder = 0
    object TabSheetAtributo: TTabSheet
      Caption = 'Gr'#225'fico'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ToolBar1: TToolBar
        Left = 0
        Top = 0
        Width = 320
        Height = 25
        ButtonHeight = 19
        ButtonWidth = 62
        Caption = 'ToolBar1'
        EdgeBorders = [ebTop, ebBottom]
        List = True
        ShowCaptions = True
        TabOrder = 0
        object butSelAll: TBitBtn
          Left = 0
          Top = 0
          Width = 19
          Height = 19
          Action = ActionAtributoSelAll
          Caption = #254
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          NumGlyphs = 2
          ParentFont = False
          TabOrder = 0
        end
        object butSelNone: TBitBtn
          Left = 19
          Top = 0
          Width = 19
          Height = 19
          Action = ActionAtributoSelNone
          Caption = 'o'
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Wingdings'
          Font.Style = [fsBold]
          NumGlyphs = 2
          ParentFont = False
          TabOrder = 1
        end
        object ToolButton1: TToolButton
          Left = 38
          Top = 0
          Width = 8
          Caption = 'ToolButton1'
          Style = tbsSeparator
        end
        object ButPadronizar: TToolButton
          Left = 46
          Top = 0
          Action = ActionAtributoAllEqpto
          AutoSize = True
        end
      end
      object CLBoxAtributo: TCheckListBox
        Left = 0
        Top = 25
        Width = 320
        Height = 410
        OnClickCheck = ActionAtributoChangedExecute
        Align = alClient
        BorderStyle = bsNone
        ItemHeight = 13
        TabOrder = 1
      end
    end
    object TabSheetDado: TTabSheet
      Caption = 'Dados'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PanelTView: TPanel
        Left = 0
        Top = 0
        Width = 320
        Height = 435
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Splitter: TSplitter
          Left = 0
          Top = 258
          Width = 320
          Height = 4
          Cursor = crVSplit
          Align = alBottom
          Beveled = True
          Color = clBtnFace
          ParentColor = False
          ExplicitWidth = 221
        end
        object Bevel1: TBevel
          Left = 0
          Top = 0
          Width = 320
          Height = 2
          Align = alTop
          Shape = bsTopLine
          ExplicitWidth = 221
        end
        object TView: TTreeView
          Left = 0
          Top = 27
          Width = 320
          Height = 231
          Align = alClient
          BorderStyle = bsNone
          HideSelection = False
          Indent = 19
          MultiSelect = True
          ReadOnly = True
          TabOrder = 0
          OnAdvancedCustomDrawItem = TViewAdvancedCustomDrawItem
          OnClick = TViewClick
        end
        object PanelGrafico: TPanel
          Left = 0
          Top = 262
          Width = 320
          Height = 173
          Align = alBottom
          BevelOuter = bvNone
          BorderStyle = bsSingle
          TabOrder = 1
          object CLBoxSerie: TCheckListBox
            Left = 0
            Top = 140
            Width = 316
            Height = 29
            OnClickCheck = CLBoxSerieClickCheck
            Align = alBottom
            BorderStyle = bsNone
            Columns = 2
            ItemHeight = 13
            Items.Strings = (
              'Curva 1'
              'Curva 2'
              'Curva 3'
              'Curva 4')
            Style = lbOwnerDrawFixed
            TabOrder = 0
            OnDrawItem = CLBoxSerieDrawItem
          end
          object chart: TChart
            Left = 0
            Top = 0
            Width = 316
            Height = 140
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
            BottomAxis.LabelStyle = talText
            LeftAxis.AxisValuesFormat = '#,##0.#0'
            LeftAxis.ExactDateTime = False
            LeftAxis.Increment = 0.100000000000000000
            LeftAxis.LabelsSeparation = 1
            LeftAxis.LabelStyle = talValue
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
            TabOrder = 1
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
              SeriesColor = clRed
              LinePen.Color = clGreen
              XValues.Name = 'X'
              XValues.Order = loAscending
              YValues.Name = 'Y'
              YValues.Order = loNone
            end
            object Series4: TFastLineSeries
              Marks.Style = smsValue
              SeriesColor = clRed
              LinePen.Color = clGreen
              XValues.Name = 'X'
              XValues.Order = loAscending
              YValues.Name = 'Y'
              YValues.Order = loNone
            end
          end
        end
        object TViewHeader: THeaderControl
          Left = 0
          Top = 2
          Width = 320
          Height = 25
          Sections = <
            item
              Alignment = taCenter
              ImageIndex = -1
              MinWidth = 140
              Text = 'Atributo'
              Width = 180
            end
            item
              Alignment = taCenter
              AutoSize = True
              ImageIndex = -1
              Text = 'Valor'
              Width = 140
            end>
          Style = hsFlat
          OnSectionResize = TViewHeaderSectionResize
        end
      end
    end
    object TabSheetFoto: TTabSheet
      Caption = 'Imagens'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Image: TImage
        Left = 0
        Top = 27
        Width = 320
        Height = 408
        Align = alClient
        Proportional = True
        ExplicitWidth = 221
      end
      object ToolBar4: TToolBar
        Left = 0
        Top = 0
        Width = 320
        Height = 27
        ButtonWidth = 43
        Caption = 'ToolBar2'
        EdgeBorders = [ebTop, ebBottom]
        Font.Charset = ANSI_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = [fsBold]
        Images = ImageList1
        List = True
        ParentFont = False
        ParentShowHint = False
        ShowCaptions = True
        ShowHint = True
        TabOrder = 0
        object butFirst: TToolButton
          Left = 0
          Top = 0
          Action = ActionFotoFirst
          AutoSize = True
        end
        object butPrev: TToolButton
          Left = 25
          Top = 0
          Action = ActionFotoPrev
          AutoSize = True
        end
        object butNex: TToolButton
          Left = 50
          Top = 0
          Action = ActionFotoNext
          AutoSize = True
        end
        object butLast: TToolButton
          Left = 75
          Top = 0
          Action = ActionFotoLast
          AutoSize = True
        end
      end
    end
  end
  object ActionList: TActionList
    Left = 16
    Top = 96
    object ActionAtributoAllEqpto: TAction
      Caption = 'Padronizar'
      OnExecute = ActionAtributoAllEqptoExecute
    end
    object ActionAtributoChanged: TAction
      OnExecute = ActionAtributoChangedExecute
    end
    object ActionAtributoSelAll: TAction
      Caption = #254
      OnExecute = ActionAtributoSelAllExecute
    end
    object ActionAtributoSelNone: TAction
      Caption = 'o'
      OnExecute = ActionAtributoSelNoneExecute
    end
    object ActionFotoFirst: TAction
      Caption = '|<'
      OnExecute = ActionFotoFirstExecute
    end
    object ActionFotoLast: TAction
      Caption = '>|'
      OnExecute = ActionFotoLastExecute
    end
    object ActionFotoNext: TAction
      Caption = '>>'
      OnExecute = ActionFotoNextExecute
    end
    object ActionFotoPrev: TAction
      Caption = '<<'
      OnExecute = ActionFotoPrevExecute
    end
  end
  object ImageList1: TImageList
    ShareImages = True
    Left = 48
    Top = 96
  end
end
