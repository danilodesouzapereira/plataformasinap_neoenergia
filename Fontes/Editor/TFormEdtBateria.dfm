object FormEdtBateria: TFormEdtBateria
  AlignWithMargins = True
  Left = 14
  Top = 0
  Caption = 'Bateria El'#233'trica'
  ClientHeight = 374
  ClientWidth = 510
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PanelToolBar: TPanel
    Left = 0
    Top = 0
    Width = 510
    Height = 27
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    object PanelLeft: TPanel
      Left = 0
      Top = 0
      Width = 510
      Height = 27
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object ToolBarLeft: TToolBar
        Left = 0
        Top = 0
        Width = 510
        Height = 26
        AutoSize = True
        ButtonWidth = 70
        EdgeBorders = [ebTop, ebBottom]
        Images = ImageList
        List = True
        ParentShowHint = False
        ShowCaptions = True
        ShowHint = True
        TabOrder = 0
        Transparent = False
        Wrapable = False
        object panelMouse: TPanel
          Left = 0
          Top = 0
          Width = 22
          Height = 22
          TabOrder = 0
          object ImageMouseOff: TImage
            Left = 4
            Top = 3
            Width = 15
            Height = 16
            Picture.Data = {
              07544269746D617036030000424D360300000000000036000000280000001000
              0000100000000100180000000000000300000000000000000000000000000000
              00000000FF0000FF0000FF0000FF0000FF0000FF0000000000000000000000FF
              0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              FF000000FFFFFFFFFFFFFFFFFF0000000000FF0000FF0000FF0000FF0000FF00
              00FF0000FF0000FF0000FF0000FF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              0000000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000FF0000FF0000FF0000FF00
              00FF0000FF0000FF0000FF0000FF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              0000000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              007F7F7F7F7F7F7F7F7F7F7F7F7F7F7F0000000000FF0000FF0000FF0000FF00
              00FF0000FF0000FF0000FF0000FF000000FFFFFFFFFFFF7F7F7FFFFFFFFFFFFF
              0000000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              00FFFFFFFFFFFF7F7F7FFFFFFFFFFFFF0000000000FF0000FF0000FF0000FF00
              00FF0000FF0000FF0000FF0000FF0000FF000000000000000000000000000000
              0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000000000FF0000FF0000000000000000007F7F7F0000FF00
              00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000000000FF000000
              0000FF0000FF0000FF0000007F7F7F0000FF0000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000FF0000000000FF0000FF0000FF0000FF0000FF00000000
              00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000007F7F7F
              0000FF0000FF0000FF0000FF0000000000FF0000FF0000FF0000FF0000FF0000
              FF0000FF7F7F7F0000000000FF0000007F7F7F0000FF0000FF0000007F7F7F00
              00FF0000FF0000FF0000FF0000FF0000FF7F7F7F0000000000FF0000FF0000FF
              0000000000000000007F7F7F0000FF0000FF0000FF0000FF0000FF0000FF7F7F
              7F0000000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF00
              00FF}
          end
          object ImageMouseOn: TImage
            Left = 3
            Top = 3
            Width = 16
            Height = 16
            Picture.Data = {
              07544269746D617036030000424D360300000000000036000000280000001000
              0000100000000100180000000000000300000000000000000000000000000000
              000000FF0000FF0000FF0000FF0000FF0000FF0000000000000000000000FF00
              00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
              00000000FFFFFFFFFFFFFFFFFF00000000FF0000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000FF0000FF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              00000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF000000
              00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000FF0000FF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              00000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF000000
              007F7F7F7F7F7F7F7F7F7F7F7F7F7F7F00000000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000FF0000FF00000000FFFFFFFFFFFF7F7F7FFFFFFFFFFFFF
              00000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF000000
              00FFFFFFFFFFFF7F7F7FFFFFFFFFFFFF00000000FF0000FF0000FF0000FF0000
              FF0000FF0000FF0000FF0000FF0000FF00000000000000000000000000000000
              00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
              0000FF0000FF0000000000FF0000FF000000000000000000007F7F7F00FF0000
              FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000000000FF00000000
              00FF0000FF0000FF000000007F7F7F00FF0000FF0000FF0000FF0000FF0000FF
              0000FF0000FF0000FF0000000000FF0000FF0000FF0000FF0000FF0000000000
              FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF000000007F7F7F
              00FF0000FF0000FF0000FF0000000000FF0000FF0000FF0000FF0000FF0000FF
              0000FF007F7F7F00000000FF000000007F7F7F00FF0000FF000000007F7F7F00
              FF0000FF0000FF0000FF0000FF0000FF007F7F7F00000000FF0000FF0000FF00
              0000000000000000007F7F7F00FF0000FF0000FF0000FF0000FF0000FF007F7F
              7F00000000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
              FF00}
          end
        end
        object butConfirma: TToolButton
          Left = 22
          Top = 0
          Action = ActionConfirma
          AutoSize = True
        end
        object ToolButton2: TToolButton
          Left = 96
          Top = 0
          Action = ActionCancela
          AutoSize = True
        end
      end
    end
  end
  object PageControlBateria: TPageControl
    Left = 0
    Top = 27
    Width = 510
    Height = 347
    ActivePage = TabSheetBateria
    Align = alClient
    MultiLine = True
    TabOrder = 1
    OnChange = PageControlBateriaChange
    OnChanging = PageControlBateriaChanging
    ExplicitHeight = 351
    object TabSheetBateria: TTabSheet
      AlignWithMargins = True
      Caption = 'Informa'#231#245'es Gerais'
      ExplicitHeight = 299
      object GBoxInfoArmazenamento: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 490
        Height = 294
        Align = alTop
        Caption = 'Informa'#231#245'es do Armazenamento de Energia'
        TabOrder = 0
        object Panel2: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 117
          Width = 480
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label1: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Fases'
            Layout = tlCenter
            ExplicitTop = -13
          end
          object CBoxMBT: TComboBox
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 187
            Height = 21
            Align = alClient
            Style = csDropDownList
            ItemIndex = 2
            TabOrder = 0
            Text = 'Trif'#225'sica'
            OnChange = CBoxMBTChange
            Items.Strings = (
              'Monof'#225'sica'
              'Bif'#225'sica'
              'Trif'#225'sica')
          end
          object CBoxFase: TComboBox
            AlignWithMargins = True
            Left = 424
            Top = 3
            Width = 53
            Height = 21
            Align = alRight
            Style = csDropDownList
            ItemIndex = 0
            TabOrder = 1
            Text = 'ABC'
            OnChange = CBoxFaseChange
            Items.Strings = (
              'ABC'
              'ABCN')
          end
        end
        object Panel1: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 51
          Width = 480
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label2: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Capacidade da Bateria (kWh)'
            Layout = tlCenter
          end
          object editCapkWh: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 246
            Height = 21
            Align = alClient
            NumbersOnly = True
            TabOrder = 0
          end
        end
        object Panel9: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 480
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label9: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'C'#243'digo da Bateria'
            Layout = tlCenter
          end
          object editCodigo: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 246
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel15: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 150
          Width = 480
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object Label15: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Modelo de Carga e Descarga'
            Layout = tlCenter
            ExplicitLeft = 11
            ExplicitTop = 6
          end
          object CBoxModCargaDescarga: TComboBox
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 246
            Height = 21
            Align = alClient
            Style = csDropDownList
            ItemIndex = 2
            TabOrder = 0
            Text = 'Imped'#226'ncia constante'
            Items.Strings = (
              'Corrente constante'
              'Pot'#234'ncia constante'
              'Imped'#226'ncia constante')
          end
        end
        object Panel18: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 84
          Width = 480
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 4
          object Label18: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Pot'#234'ncia Nominal (kW)'
            Layout = tlCenter
            ExplicitTop = 19
          end
          object editPotNomkW: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 246
            Height = 21
            Align = alClient
            NumbersOnly = True
            TabOrder = 0
          end
        end
        object RGroupModoOperacao: TRadioGroup
          AlignWithMargins = True
          Left = 5
          Top = 183
          Width = 480
          Height = 50
          Align = alTop
          Caption = 'Modo de Opera'#231#227'o'
          Columns = 2
          ItemIndex = 1
          Items.Strings = (
            'Curva Hor'#225'ria'
            'Autom'#225'tico')
          TabOrder = 5
          OnClick = ActionSelModoOperacaoExecute
        end
        object RGroupPeakShaving: TRadioGroup
          AlignWithMargins = True
          Left = 5
          Top = 239
          Width = 480
          Height = 50
          Align = alTop
          Caption = 'Executar Peak Shaving?'
          Columns = 2
          ItemIndex = 1
          Items.Strings = (
            'Sim'
            'N'#227'o')
          TabOrder = 6
          OnClick = ActionSelPeakShavingExecute
        end
      end
    end
    object TabSheetAvancado: TTabSheet
      Caption = 'Avan'#231'ado'
      ImageIndex = 4
      ExplicitHeight = 305
      object GroupBox1: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 496
        Height = 254
        Align = alTop
        Caption = 'Configura'#231#245'es Avan'#231'adas'
        TabOrder = 0
        object Panel21: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 84
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label21: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Reserva M'#237'nima (pu)'
            Layout = tlCenter
          end
          object editReserva: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel22: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 216
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label22: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Perdas Reativas por Inatividade (pu)'
            Layout = tlCenter
          end
          object editPerdasReativasInatividade: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel23: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 117
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object Label23: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Efici'#234'ncia de Carga (pu)'
            Layout = tlCenter
          end
          object editEficienciaCarga: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel24: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 150
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 4
          object Label24: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Efici'#234'ncia de Descarga (pu)'
            Layout = tlCenter
          end
          object editEficienciaDescarga: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel28: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label28: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Capacidade do Inversor (kVA)'
            Layout = tlCenter
          end
          object editCapkVA: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            NumbersOnly = True
            TabOrder = 0
          end
        end
        object Panel29: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 51
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 5
          object Label29: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Energia Armazenada Inicialmente (kWh)'
            Layout = tlCenter
          end
          object editEArmazIni: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            NumbersOnly = True
            TabOrder = 0
            OnChange = editEArmazIniChange
          end
        end
        object Panel3: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 183
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 6
          object Label3: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 222
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Perdas Ativas por Inatividade (pu)'
            Layout = tlCenter
          end
          object editPerdasAtivasInatividade: TEdit
            AlignWithMargins = True
            Left = 231
            Top = 3
            Width = 252
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
      end
    end
    object TabSheetModoOper: TTabSheet
      Caption = 'Modo de Opera'#231#227'o'
      ImageIndex = 3
      ExplicitHeight = 305
      object GBoxPeakShaving: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 71
        Width = 496
        Height = 106
        Align = alTop
        Caption = 'Peak-Shaving'
        TabOrder = 0
        object Panel6: TPanel
          Left = 2
          Top = 15
          Width = 492
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label6: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 230
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'C'#243'digo da Carga'
            Layout = tlCenter
          end
          object CBoxPeakShavingCarga: TComboBox
            AlignWithMargins = True
            Left = 239
            Top = 3
            Width = 250
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
        object Panel4: TPanel
          Left = 2
          Top = 42
          Width = 492
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label4: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 230
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'In'#237'cio de Descarga (hora)'
            Layout = tlCenter
          end
          object CBoxPeakShavingHoraIni: TComboBox
            AlignWithMargins = True
            Left = 239
            Top = 3
            Width = 250
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
        object Panel11: TPanel
          Left = 2
          Top = 69
          Width = 492
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label11: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 230
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Dura'#231#227'o de Descarga (hora)'
            Layout = tlCenter
          end
          object CBoxPeakShavingDuracao: TComboBox
            AlignWithMargins = True
            Left = 239
            Top = 3
            Width = 250
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
      end
      object GBoxCurvaCargaDescarga: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 496
        Height = 62
        Align = alTop
        Caption = 'Curva Hor'#225'ria de Carga e Descarga'
        TabOrder = 1
        object pnlCurvaPropria: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 486
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object butImportarCurva: TButton
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 134
            Height = 21
            Action = ActionImporta
            Align = alLeft
            Caption = 'Importar Curva'
            Images = ImageList
            TabOrder = 0
          end
        end
      end
    end
    object TabSheetCurvaCargaDescarga: TTabSheet
      AlignWithMargins = True
      Caption = 'Curva de Carga e Descarga'
      ImageIndex = 2
      ExplicitHeight = 299
    end
    object TabSheetGraficoCargaDescarga: TTabSheet
      Caption = 'Gr'#225'fico de Carga e Descarga'
      ImageIndex = 5
      ExplicitHeight = 305
    end
    object TabSheetCurvaEst: TTabSheet
      Caption = 'Curva de Estado de Carga da Bateria'
      ImageIndex = 1
      ExplicitHeight = 305
      object Chart: TChart
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 496
        Height = 295
        Legend.Alignment = laBottom
        Legend.ColorWidth = 10
        Legend.Symbol.Width = 10
        Legend.TextStyle = ltsRightPercent
        Legend.TopPos = 0
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
        LeftAxis.Title.Caption = 'Energia (kWh)'
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
        ExplicitHeight = 299
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
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 456
    Top = 80
    object ActionAlignDown: TAction
      Caption = 'Alinhar em baixo'
      Hint = 'Fixar janela em baixo'
      ImageIndex = 3
      OnExecute = ActionAlignDownExecute
    end
    object ActionAlignLeft: TAction
      Caption = 'Alinhar '#224' esquerda'
      Hint = 'Fixar janela '#224' esquerda'
      ImageIndex = 4
      OnExecute = ActionAlignLeftExecute
    end
    object ActionAlignNone: TAction
      Caption = 'Cancelar alinhamento'
      Hint = 'Tornar janela '#39'flutuante'#39
      ImageIndex = 2
      OnExecute = ActionAlignNoneExecute
    end
    object ActionAlignRight: TAction
      Caption = 'Alinhar '#224' direita'
      Hint = 'Fixar janela '#224' direita'
      ImageIndex = 5
      OnExecute = ActionAlignRightExecute
    end
    object ActionAlignUp: TAction
      Caption = 'Alinhar em cima'
      Hint = 'Fixar janela em cima'
      ImageIndex = 6
      OnExecute = ActionAlignUpExecute
    end
    object ActionEdt: TAction
      Caption = 'ActionEdt'
      Hint = 'Localizar equipamentos'
      ImageIndex = 0
      OnExecute = ActionEdtExecute
    end
    object ActionMouseOff: TAction
      Caption = 'ActionMouseOff'
      OnExecute = ActionMouseOffExecute
    end
    object ActionMouseOn: TAction
      Caption = 'ActionMouseOn'
      OnExecute = ActionMouseOnExecute
    end
    object ActionSelModoOperacao: TAction
      Caption = 'ActionSelModoOperacao'
      OnExecute = ActionSelModoOperacaoExecute
    end
    object ActionConfirma: TAction
      Caption = 'Confirma'
      ImageIndex = 9
      OnExecute = ActionConfirmaExecute
    end
    object ActionCancela: TAction
      Caption = 'Cancela'
      ImageIndex = 10
      OnExecute = ActionCancelaExecute
    end
    object ActionImporta: TAction
      Caption = 'ActionImporta'
      ImageIndex = 8
      OnExecute = ActionImportaExecute
    end
    object ActionSelPeakShaving: TAction
      Caption = 'ActionSelPeakShaving'
      OnExecute = ActionSelPeakShavingExecute
    end
  end
  object ImageList: TImageList
    Left = 248
    Top = 80
    Bitmap = {
      494C01010B006800C00010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000003000000001002000000000000030
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
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF00000000000000000000000000FFFFFF0000000000C0C0C000000000000000
      0000FFFFFF0000000000FFFFFF00000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF0000000000FFFFFF000000000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF0000000000C0C0C000FFFFFF000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000000000FFFF000000000000FFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000BFBF
      BF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      2800000040000000300000000100010000000000800100000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFFFFF0000FC00FFFFFFFF0000
      FC00FE7FFFFF0000FC00FC3FE7F30000FC00F81FE3E30000EC00F19FF1C70000
      E400E38FF88F0000E000E7CFFC1F00000000FFC7FE3F00000001FFE7FC1F0000
      0003FFE3F88F00000007FFF3F1C70000000FFFF3E3E30000E3FFFFFFE7F30000
      E7FFFFFFFFFF0000EFFFFFFFFFFF0000FFFFFFFFE00FFFDFFFFFFFFFE00FFFCF
      FFF81FFFE00FFFC7F820041FF00F0003F000000FF00F0001F000000FF80F0000
      E0000007F007000180000001F007000300000000F007000780000001F00F000F
      FC00003FF01F001FFE3FFC7FF81F007FFFFFFFFFFE1F00FFFFFFFFFFFF1F01FF
      FFFFFFFFFF1F03FFFFFFFFFFFFBFFFFFFFFFFC7FFFFFFDFFFFE3F83FC600F8FF
      FFC3F01F8200F8FFFB83F01F0000F87FF907F01F0000F81FF80FF01F0000F80F
      F01FF01F8000F00FF03FF01F8000E00FE01FF83F8000E00FE00FFEC38000E00F
      C07FFEB9C800F01FC1FFFF7D3C00F00F87FFFF3D0E00F00F9FFFFC99CE00F007
      FFFFF9C32600F007FFFFF3FF2600F00700000000000000000000000000000000
      000000000000}
  end
  object PopupMenu: TPopupMenu
    Images = ImageList
    Left = 328
    Top = 80
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
  object OpenDialog: TOpenDialog
    Left = 416
    Top = 80
  end
end
