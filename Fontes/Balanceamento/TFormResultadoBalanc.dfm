object FormResultadoBalanc: TFormResultadoBalanc
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Resultado do Balanceamento de Cargas'
  ClientHeight = 506
  ClientWidth = 944
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 944
    Height = 28
    ButtonWidth = 147
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    Wrapable = False
    object butEdt: TToolButton
      Left = 0
      Top = 0
      Action = ActionConfirmar
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ToolButton5: TToolButton
      Left = 78
      Top = 0
      Action = ActionCancelar
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 151
      Top = 0
      Action = ActionRelCargas
      AutoSize = True
    end
    object ToolButton1: TToolButton
      Left = 302
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 8
      Style = tbsSeparator
    end
    object Panel: TPanel
      Left = 310
      Top = 0
      Width = 235
      Height = 22
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 0
      object Label1: TLabel
        Left = 6
        Top = 5
        Width = 117
        Height = 13
        Caption = 'Patamar cargto. m'#225'ximo'
      end
      object CBoxPatamar: TComboBox
        Left = 129
        Top = 0
        Width = 102
        Height = 21
        Style = csDropDownList
        Enabled = False
        TabOrder = 0
      end
    end
  end
  object PanelAntes: TPanel
    Left = 0
    Top = 28
    Width = 473
    Height = 478
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    object PanelPQa: TPanel
      Left = 0
      Top = 0
      Width = 473
      Height = 292
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object GroupBox2: TGroupBox
        Left = 0
        Top = 0
        Width = 473
        Height = 70
        Align = alTop
        Caption = 'An'#225'lise de carregamento m'#225'ximo (original)'
        TabOrder = 0
        object LViewPQantes: TListView
          Left = 2
          Top = 15
          Width = 469
          Height = 53
          Align = alClient
          Columns = <
            item
              AutoSize = True
              Caption = 'Rede'
            end
            item
              Caption = 'ET'
            end
            item
              Alignment = taRightJustify
              Caption = 'Sa [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Sb [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Sc [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Deseq. M'#225'x[%]'
              Width = 90
            end>
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
      object PanelPQantes: TPanel
        Left = 0
        Top = 70
        Width = 473
        Height = 222
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object chartPantes: TChart
          Left = 0
          Top = 0
          Width = 241
          Height = 222
          Legend.Alignment = laBottom
          Legend.CheckBoxesStyle = cbsRadio
          Legend.TextStyle = ltsLeftPercent
          Legend.VertSpacing = 1
          PrintProportional = False
          Title.Text.Strings = (
            'Pot'#234'ncia Ativa (original)')
          DepthAxis.Automatic = False
          DepthAxis.AutomaticMaximum = False
          DepthAxis.AutomaticMinimum = False
          DepthAxis.Maximum = -0.500000000000000000
          DepthAxis.Minimum = -0.500000000000000000
          DepthTopAxis.Automatic = False
          DepthTopAxis.AutomaticMaximum = False
          DepthTopAxis.AutomaticMinimum = False
          DepthTopAxis.Maximum = -0.500000000000000000
          DepthTopAxis.Minimum = -0.500000000000000000
          LeftAxis.Automatic = False
          LeftAxis.AutomaticMaximum = False
          LeftAxis.AutomaticMinimum = False
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          View3D = False
          View3DOptions.Elevation = 315
          View3DOptions.HorizOffset = 20
          View3DOptions.Orthogonal = False
          View3DOptions.Perspective = 0
          View3DOptions.Rotation = 360
          View3DOptions.VertOffset = -2
          Align = alClient
          TabOrder = 0
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            13
            20
            13
            17)
          ColorPaletteIndex = 13
          object SeriesCarregPantes: TPieSeries
            Marks.Visible = False
            Title = 'SeriesCarregAntes'
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
            OtherSlice.Style = poBelowPercent
            Data = {
              06030000000000000000005940FFFF0000FF0600000046617365204100000000
              0000594000000020FF060000004661736520420000000000005940FF000000FF
              06000000466173652043}
          end
        end
        object chartQantes: TChart
          Left = 241
          Top = 0
          Width = 232
          Height = 222
          Legend.Alignment = laBottom
          Legend.CheckBoxesStyle = cbsRadio
          Legend.TextStyle = ltsLeftPercent
          Legend.VertSpacing = 1
          PrintProportional = False
          Title.Text.Strings = (
            'Pot'#234'ncia Reativa (original)')
          DepthAxis.Automatic = False
          DepthAxis.AutomaticMaximum = False
          DepthAxis.AutomaticMinimum = False
          DepthAxis.Maximum = -0.500000000000000000
          DepthAxis.Minimum = -0.500000000000000000
          DepthTopAxis.Automatic = False
          DepthTopAxis.AutomaticMaximum = False
          DepthTopAxis.AutomaticMinimum = False
          DepthTopAxis.Maximum = -0.500000000000000000
          DepthTopAxis.Minimum = -0.500000000000000000
          LeftAxis.Automatic = False
          LeftAxis.AutomaticMaximum = False
          LeftAxis.AutomaticMinimum = False
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          View3D = False
          View3DOptions.Elevation = 315
          View3DOptions.HorizOffset = 20
          View3DOptions.Orthogonal = False
          View3DOptions.Perspective = 0
          View3DOptions.Rotation = 360
          View3DOptions.VertOffset = -2
          Align = alRight
          TabOrder = 1
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            13
            20
            13
            17)
          ColorPaletteIndex = 13
          object SeriesCarregQantes: TPieSeries
            Marks.Visible = False
            Title = 'SeriesCarregDepois'
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
            OtherSlice.Style = poBelowPercent
            Data = {
              06030000000000000000005940FFFF0000FF0600000046617365204100000000
              0000594000000020FF060000004661736520420000000000005940FF000000FF
              06000000466173652043}
          end
        end
      end
    end
    object PanelSant: TPanel
      Left = 0
      Top = 292
      Width = 473
      Height = 186
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object chartS3Fantes: TChart
        Left = 0
        Top = 0
        Width = 473
        Height = 186
        LeftWall.Color = clWhite
        Legend.CheckBoxes = True
        Title.Text.Strings = (
          'Carregamento (original)'
          '')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = 'Per'#237'odo [24h]'
        Chart3DPercent = 10
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 0.500000000000045100
        DepthAxis.Minimum = -0.499999999999955500
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 0.500000000000045100
        DepthTopAxis.Minimum = -0.499999999999955500
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 0.100000000000000000
        LeftAxis.Maximum = 1224.350000000000000000
        LeftAxis.Minimum = 236.999999999999900000
        LeftAxis.Title.Caption = 'Pot'#234'ncia [kVA]'
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        View3D = False
        View3DOptions.HorizOffset = -4
        View3DOptions.VertOffset = -8
        View3DOptions.Zoom = 102
        Align = alClient
        Color = 14737632
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          12
          15
          12)
        ColorPaletteIndex = 13
        object SeriesAa: TLineSeries
          SeriesColor = clNavy
          Title = 'Fase A'
          ValueFormat = '#.##0,###'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = clNavy
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesBa: TLineSeries
          SeriesColor = 33023
          Title = 'Fase B'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = 4227327
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesCa: TLineSeries
          SeriesColor = clRed
          Title = 'Fase C'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = clRed
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesABCa: TLineSeries
          Active = False
          SeriesColor = clGreen
          Title = 'Fase ABC'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = clGreen
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object PanelDepois: TPanel
    Left = 473
    Top = 28
    Width = 471
    Height = 478
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object PanelPQd: TPanel
      Left = 0
      Top = 0
      Width = 471
      Height = 292
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object PanelPQdepois: TPanel
        Left = 0
        Top = 70
        Width = 471
        Height = 222
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object chartPdepois: TChart
          Left = 0
          Top = 0
          Width = 239
          Height = 222
          Legend.Alignment = laBottom
          Legend.CheckBoxesStyle = cbsRadio
          Legend.TextStyle = ltsLeftPercent
          Legend.VertSpacing = 1
          PrintProportional = False
          Title.Text.Strings = (
            'Pot'#234'ncia Ativa (balanceada)')
          DepthAxis.Automatic = False
          DepthAxis.AutomaticMaximum = False
          DepthAxis.AutomaticMinimum = False
          DepthAxis.Maximum = -0.500000000000000000
          DepthAxis.Minimum = -0.500000000000000000
          DepthTopAxis.Automatic = False
          DepthTopAxis.AutomaticMaximum = False
          DepthTopAxis.AutomaticMinimum = False
          DepthTopAxis.Maximum = -0.500000000000000000
          DepthTopAxis.Minimum = -0.500000000000000000
          LeftAxis.Automatic = False
          LeftAxis.AutomaticMaximum = False
          LeftAxis.AutomaticMinimum = False
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          View3D = False
          View3DOptions.Elevation = 315
          View3DOptions.HorizOffset = 20
          View3DOptions.Orthogonal = False
          View3DOptions.Perspective = 0
          View3DOptions.Rotation = 360
          View3DOptions.VertOffset = -2
          Align = alClient
          TabOrder = 0
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            13
            20
            13
            17)
          ColorPaletteIndex = 13
          object SeriesCarregPdepois: TPieSeries
            Marks.Visible = False
            Title = 'SeriesCarregAntes'
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
            OtherSlice.Style = poBelowPercent
            Data = {
              06030000000000000000005940FFFF0000FF0600000046617365204100000000
              0000594000000020FF060000004661736520420000000000005940FF000000FF
              06000000466173652043}
          end
        end
        object chartQdepois: TChart
          Left = 239
          Top = 0
          Width = 232
          Height = 222
          Legend.Alignment = laBottom
          Legend.CheckBoxesStyle = cbsRadio
          Legend.TextStyle = ltsLeftPercent
          Legend.VertSpacing = 1
          PrintProportional = False
          Title.Text.Strings = (
            'Pot'#234'ncia Reativa (balanceada)')
          DepthAxis.Automatic = False
          DepthAxis.AutomaticMaximum = False
          DepthAxis.AutomaticMinimum = False
          DepthAxis.Maximum = -0.500000000000000000
          DepthAxis.Minimum = -0.500000000000000000
          DepthTopAxis.Automatic = False
          DepthTopAxis.AutomaticMaximum = False
          DepthTopAxis.AutomaticMinimum = False
          DepthTopAxis.Maximum = -0.500000000000000000
          DepthTopAxis.Minimum = -0.500000000000000000
          LeftAxis.Automatic = False
          LeftAxis.AutomaticMaximum = False
          LeftAxis.AutomaticMinimum = False
          RightAxis.Automatic = False
          RightAxis.AutomaticMaximum = False
          RightAxis.AutomaticMinimum = False
          View3D = False
          View3DOptions.Elevation = 315
          View3DOptions.HorizOffset = 20
          View3DOptions.Orthogonal = False
          View3DOptions.Perspective = 0
          View3DOptions.Rotation = 360
          View3DOptions.VertOffset = -2
          Align = alRight
          TabOrder = 1
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            13
            20
            13
            17)
          ColorPaletteIndex = 13
          object SeriesCarregQdepois: TPieSeries
            Marks.Visible = False
            Title = 'SeriesCarregDepois'
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
            OtherSlice.Style = poBelowPercent
            Data = {
              06030000000000000000005940FFFF0000FF0600000046617365204100000000
              0000594000000020FF060000004661736520420000000000005940FF000000FF
              06000000466173652043}
          end
        end
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 471
        Height = 70
        Align = alTop
        Caption = 'An'#225'lise de carregamento m'#225'ximo (balanceado)'
        TabOrder = 1
        object LViewPQdepois: TListView
          Left = 2
          Top = 15
          Width = 467
          Height = 53
          Align = alClient
          Columns = <
            item
              AutoSize = True
              Caption = 'Rede'
            end
            item
              Caption = 'ET'
            end
            item
              Alignment = taRightJustify
              Caption = 'Sa [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Sb [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Sc [kVA]'
              Width = 85
            end
            item
              Alignment = taRightJustify
              Caption = 'Deseq. M'#225'x[%]'
              Width = 90
            end>
          TabOrder = 0
          ViewStyle = vsReport
        end
      end
    end
    object PanelSdep: TPanel
      Left = 0
      Top = 292
      Width = 471
      Height = 186
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object chartS3Fdepois: TChart
        Left = 0
        Top = 0
        Width = 471
        Height = 186
        LeftWall.Color = clWhite
        Legend.CheckBoxes = True
        Title.Text.Strings = (
          'carregamento (balanceado)'
          '')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Maximum = 24.000000000000000000
        BottomAxis.Title.Caption = 'Per'#237'odo [24h]'
        Chart3DPercent = 10
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 0.500000000000045100
        DepthAxis.Minimum = -0.499999999999955500
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 0.500000000000045100
        DepthTopAxis.Minimum = -0.499999999999955500
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 0.100000000000000000
        LeftAxis.Maximum = 890.581250000000000000
        LeftAxis.Minimum = 377.500000000000000000
        LeftAxis.Title.Caption = 'Pot'#234'ncia [kVA]'
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        View3D = False
        View3DOptions.HorizOffset = -4
        View3DOptions.VertOffset = -8
        View3DOptions.Zoom = 102
        Align = alClient
        Color = 14540253
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        PrintMargins = (
          15
          12
          15
          12)
        ColorPaletteIndex = 13
        object SeriesAd: TLineSeries
          SeriesColor = clNavy
          Title = 'Fase A'
          ValueFormat = '#.##0,###'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = clNavy
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesBd: TLineSeries
          SeriesColor = 4227327
          Title = 'Fase B'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = 4227327
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesCd: TLineSeries
          SeriesColor = clRed
          Title = 'Fase C'
          Brush.BackColor = clDefault
          Dark3D = False
          LinePen.Color = clRed
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object SeriesABCd: TLineSeries
          Active = False
          SeriesColor = clGreen
          Title = 'Fase ABC'
          Brush.BackColor = clDefault
          ClickableLine = False
          Dark3D = False
          LinePen.Color = clGreen
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object PopupMenu: TPopupMenu
    Images = ImageList
    Left = 152
    Top = 64
    object mnuAlignLeft: TMenuItem
      Action = ActionAlignLeft
    end
    object mnuAlignRight: TMenuItem
      Action = ActionAlignRight
    end
    object mnuAlignDown: TMenuItem
      Action = ActionAlignDown
    end
    object mnuAlignUp: TMenuItem
      Action = ActionAlignUp
    end
    object mnuAlignNode: TMenuItem
      Action = ActionAlignNone
    end
  end
  object ImageList: TImageList
    Left = 88
    Top = 64
    Bitmap = {
      494C01010C000E00180010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000004000000001002000000000000040
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000D5D4D500000000000000000000000000000000000000000007060800ACAC
      AD00000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      00000000000000000000000000000000000000000000C1C1C100000000000000
      0000C1C1C10000000000A5A5A60000000000000000004241430000000000C4C3
      C4000000000000000000ACACAD00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000000FFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      00000000000000000000000000000000000018181A0000000000070608000000
      00000F0E100018181A00000000002A292A002A292A000000000018181A001211
      130000000000070608000000000018181A000000000000000000000000000000
      00000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF000000
      000000000000000000000000000000000000FFFF00000000000000000000FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF0000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      00000000000000000000000000000000000018181A00CACACA00CACACA00CDCD
      CD00CACACA0018181A00000000002A292A002A292A000000000018181A00CACA
      CA00CACACA00CACACA00CACACA0018181A000000000000000000000000000000
      FF000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000000000000000000000000000000000FFFF000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF000000000000000000000000000000000000000000FFFF
      FF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      00000000000000000000000000000000000018181A00FEFEFE00000000000000
      0000E1E1E10018181A00000000002A292A002A292A000000000018181A00E1E1
      E1000000000000000000FEFEFE0018181A00000000007F7F7F000000FF000000
      FF000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF007F7F7F000000000000000000FFFF00000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      000000000000000000000000000000000000000000002C2B2D00000000000000
      00002C2B2D0000000000000000002A292A002A292A0000000000000000002C2B
      2D0000000000000000002C2B2D000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF0000FF
      FF0000FFFF00000000000000000000000000FFFF000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF000000000000000000000000000000000000000000FFFF
      FF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      000000000000000000000000000000000000000000002C2B2D00000000000000
      00002C2B2D0000000000000000002A292A002A292A0000000000000000002C2B
      2D0000000000000000002C2B2D000000000000000000000000000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF00000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      00000084000000000000000000000000000000000000DDDDDD001B1A1C001B1A
      1C000000000000000000000000002A292A002A292A0000000000000000000000
      00001B1A1C001B1A1C00D5D4D5000000000000000000000000000000FF000000
      FF000000FF000000FF000000000000FF000000000000FF000000FF000000FF00
      0000FF000000000000000000000000000000FFFF000000000000FFFFFF0000FF
      FF0000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      00000084000000000000000000000000000000000000000000001B1A1C001B1A
      1C0000000000FEFEFE00000000002A292A002A292A0000000000000000000000
      00001B1A1C001B1A1C000000000000000000000000007F7F7F000000FF000000
      FF000000FF000000000000FF000000FF000000FF000000000000FF000000FF00
      0000FF0000007F7F7F000000000000000000000000000000000000000000FFFF
      FF0000FFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      00000084000000840000000000000000000000000000000000001B1A1C001B1A
      1C000000000000000000000000002A292A002A292A0000000000000000000000
      00001B1A1C001B1A1C0000000000000000000000000000000000000000000000
      FF000000000000FF000000FF000000FF000000FF000000FF000000000000FF00
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000084000000840000000000000000000000000000E1E1E1002C2B2D000000
      00000000000000000000E1E1E1002A292A002A292A00E1E1E100000000000000
      0000000000002C2B2D00D5D4D500000000000000000000000000000000000000
      000000FF000000FF000000FF000000FF000000FF000000FF000000FF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      000000000000E1E1E10000000000000000000000000000000000C6C6C6000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FF000000FF000000FF000000FF000000FF0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000FF000000FF000000FF000000FF000000FF00
      000000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000002C2B2D002C2B2D0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000000000FFFF000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007F7F
      7F00000000007F7F7F0000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      00000000FF000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      FF00000000007F7F7F0000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F00000000000000FF000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF0000000000000000000000FF00000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000
      00000000000000000000000000000000000000000000FFFF00000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF000000000000FFFF000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF00000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF0000000000000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000FFFFFF0000FF
      FF000000000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F00000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000000000FFFFFF0000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000000000000000
      000000000000000000000000000000FFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000000000000000000000FF000000FF0000000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F000000000000000000000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF000000FF000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000400000000100010000000000000200000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFF800FFFFF00FF83FF800
      FE7F842110102000FC3F0240E00F0000F81F0240C0070000F19F324C80030000
      E38FB66D80030000E7CFB66D80030000FFC78E7180030000FFE7CA7380030000
      FFE3CE73C007E000FFF38001E00FF800FFF3F81F3018F800FFFFFE7FF83FF801
      FFFFFE7FFFFFF803FFFFFFFFFFFFF807FFFFFFFFE00FFFFFFFFFFFFFE00FFFFF
      FFF81FFFE00FFFFFF820041FF00FE7F3F000000FF00FE3E3F000000FF80FF1C7
      E0000007F007F88F80000001F007FC1F00000000F007FE3F80000001F00FFC1F
      FC00003FF01FF88FFE3FFC7FF81FF1C7FFFFFFFFFE1FE3E3FFFFFFFFFF1FE7F3
      FFFFFFFFFF1FFFFFFFFFFFFFFFBFFFFFFFFFFC7FFFFFFDFFFFE3F83FC600F8FF
      FFC3F01F8200F8FFFB83F01F0000F87FF907F01F0000F81FF80FF01F0000F80F
      F01FF01F8000F00FF03FF01F8000E00FE01FF83F8000E00FE00FFEC38000E00F
      C07FFEB9C800F01FC1FFFF7D3C00F00F87FFFF3D0E00F00F9FFFFC99CE00F007
      FFFFF9C32600F007FFFFF3FF2600F00700000000000000000000000000000000
      000000000000}
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 120
    Top = 64
    object ActionAlignDown: TAction
      Caption = 'Alinhar em baixo'
      Hint = 'Fixar janela em baixo'
      ImageIndex = 3
    end
    object ActionAlignLeft: TAction
      Caption = 'Alinhar '#224' esquerda'
      Hint = 'Fixar janela '#224' esquerda'
      ImageIndex = 4
    end
    object ActionAlignNone: TAction
      Caption = 'Cancelar alinhamento'
      Hint = 'Tornar janela '#39'flutuante'#39
      ImageIndex = 2
    end
    object ActionAlignRight: TAction
      Caption = 'Alinhar '#224' direita'
      Hint = 'Fixar janela '#224' direita'
      ImageIndex = 5
    end
    object ActionAlignUp: TAction
      Caption = 'Alinhar em cima'
      Hint = 'Fixar janela em cima'
      ImageIndex = 6
    end
    object ActionEdt: TAction
      Caption = 'ActionEdt'
      Hint = 'Localizar equipamentos'
      ImageIndex = 0
    end
    object ActionMouseOff: TAction
      Caption = 'ActionMouseOff'
    end
    object ActionMouseOn: TAction
      Caption = 'ActionMouseOn'
    end
    object ActionSelEqpto: TAction
      Caption = 'ActionSelEqpto'
    end
    object ActionCancelar: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar e sair'
      ImageIndex = 7
      OnExecute = ActionCancelarExecute
    end
    object ActionConfirmar: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar e sair'
      ImageIndex = 8
      OnExecute = ActionConfirmarExecute
    end
    object ActionBalancear: TAction
      Caption = 'Balancear rede'
      Hint = 'Balancear rede'
      ImageIndex = 9
    end
    object ActionRelCargas: TAction
      Caption = 'Gerar relat'#243'rio de cargas'
      Hint = 'Gerar relat'#243'rio de balanceamento de cargas'
      ImageIndex = 11
      OnExecute = ActionRelCargasExecute
    end
  end
end
