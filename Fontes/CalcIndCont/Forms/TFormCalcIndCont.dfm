inherited FormCalcIndCont: TFormCalcIndCont
  Left = 549
  Top = 158
  Caption = 'Indicadores de continuidade para Redes Radiais'
  ClientHeight = 425
  ClientWidth = 735
  Menu = MainMenu1
  Position = poOwnerFormCenter
  OnClose = FormClose
  ExplicitWidth = 751
  ExplicitHeight = 464
  PixelsPerInch = 96
  TextHeight = 13
  inherited PanelToolBar: TPanel
    Width = 735
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    ExplicitWidth = 735
    inherited PanelLeft: TPanel
      Width = 337
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      ExplicitWidth = 337
      inherited ToolBarLeft: TToolBar
        Width = 337
        ExplicitWidth = 337
        inherited butAlign: TToolButton
          ImageIndex = 3
          ExplicitWidth = 48
        end
        inherited butEdt: TToolButton
          Enabled = False
          ExplicitWidth = 27
        end
        object ToolButton2: TToolButton
          Left = 97
          Top = 0
          Action = ActionCalcIndDeriv
        end
        object ToolButton1: TToolButton
          Left = 120
          Top = 0
          Hint = 'Atualizar Par'#226'metros de Rede'
          Action = ActionReiniciar
          ImageIndex = 21
        end
        object ToolButton14: TToolButton
          Left = 143
          Top = 0
          Action = ActionMostraParamAux
          Visible = False
        end
        object ToolButton15: TToolButton
          Left = 166
          Top = 0
          Action = ActionSalvar
          Visible = False
        end
        object ToolButton19: TToolButton
          Left = 189
          Top = 0
          Action = ActionGravaParamMdb
          Visible = False
        end
        object ToolButton24: TToolButton
          Left = 212
          Top = 0
          Action = ActionLeArqMdb
          Visible = False
        end
        object ToolButton28: TToolButton
          Left = 235
          Top = 0
          Action = ActionExecutaMes
          Visible = False
        end
        object ToolButton12: TToolButton
          Left = 258
          Top = 0
          Action = ActionImprimeParamSimula
          Visible = False
        end
      end
    end
    inherited PanelRight: TPanel
      Left = 337
      Width = 398
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      ExplicitLeft = 337
      ExplicitWidth = 398
    end
  end
  object PageControl2: TPageControl [1]
    Left = 0
    Top = 25
    Width = 735
    Height = 381
    ActivePage = TabSheet7
    Align = alClient
    TabOrder = 1
    object TabSheet7: TTabSheet
      Caption = 'Dados de Entrada'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PageControl3: TPageControl
        Left = 0
        Top = 0
        Width = 727
        Height = 353
        ActivePage = TabSheet8
        Align = alClient
        TabOrder = 0
        object TabSheet8: TTabSheet
          Caption = 'Gerais'
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object ToolBar2: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Wrapable = False
            object ToolButton25: TToolButton
              Left = 0
              Top = 0
              Action = ActionLeArqMdb
            end
            object ToolButton26: TToolButton
              Left = 23
              Top = 0
              Action = ActionGravaParamMdb
            end
            object ToolButton9: TToolButton
              Left = 46
              Top = 0
              Hint = 'Carregar arquivo de par'#226'metros gerais'
              Action = ActionArqParam
              Visible = False
            end
            object ToolButton5: TToolButton
              Left = 69
              Top = 0
              Hint = 'Atualizar par'#226'metros modificados'
              Action = ActionConfirmaParamGerais
            end
            object ToolButton17: TToolButton
              Left = 92
              Top = 0
              Hint = 'Restaurar par'#226'metros anteriores'
              Action = ActionRejeitaParamGerais
              Visible = False
            end
            object ToolButton18: TToolButton
              Left = 115
              Top = 0
              Width = 8
              Caption = 'ToolButton18'
              ImageIndex = 12
              Style = tbsSeparator
              Visible = False
            end
            object ToolButton27: TToolButton
              Left = 123
              Top = 0
              Action = ActionEditaParamJusProt
              Visible = False
            end
            object ToolButton6: TToolButton
              Left = 146
              Top = 0
              Action = ActionMostraVIP
              Visible = False
            end
            object ToolButton23: TToolButton
              Left = 169
              Top = 0
              Action = ActionVoltaParamMdb
            end
          end
          object PageControl7: TPageControl
            Left = 0
            Top = 28
            Width = 719
            Height = 297
            ActivePage = TabSheet23
            Align = alClient
            TabOrder = 1
            object TabSheet23: TTabSheet
              Caption = 'Padr'#227'o'
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object GroupBox1: TGroupBox
                Left = 0
                Top = 0
                Width = 711
                Height = 269
                Align = alClient
                Caption = 'Dados gerais padr'#245'es'
                TabOrder = 0
                object Label1: TLabel
                  Left = 231
                  Top = 25
                  Width = 63
                  Height = 13
                  Caption = 'Reparo(min):'
                end
                object Label2: TLabel
                  Left = 14
                  Top = 82
                  Width = 107
                  Height = 13
                  Caption = 'Manobra manual(min):'
                end
                object Label3: TLabel
                  Left = 14
                  Top = 25
                  Width = 104
                  Height = 13
                  Caption = 'Taxa(falhas/km/ano):'
                end
                object Label4: TLabel
                  Left = 231
                  Top = 54
                  Width = 95
                  Height = 13
                  Caption = 'Manobra auto(min):'
                end
                object Label5: TLabel
                  Left = 231
                  Top = 81
                  Width = 82
                  Height = 13
                  Caption = 'Sobrecarga (pu):'
                end
                object Label6: TLabel
                  Left = 14
                  Top = 111
                  Width = 114
                  Height = 13
                  Caption = 'Falhas tempor'#225'rias(%):'
                end
                object Label7: TLabel
                  Left = 14
                  Top = 54
                  Width = 82
                  Height = 13
                  Caption = 'Localiza'#231#227'o(min):'
                end
                object Label10: TLabel
                  Left = 231
                  Top = 106
                  Width = 99
                  Height = 26
                  Caption = 'Redu'#231#227'o de tempo de preparo - IF (%):'
                  WordWrap = True
                end
                object Label11: TLabel
                  Left = 231
                  Top = 139
                  Width = 79
                  Height = 26
                  Caption = 'Indisponibilidade padr'#227'o:'
                  WordWrap = True
                end
                object cbRecurso: TCheckBox
                  Left = 231
                  Top = 169
                  Width = 97
                  Height = 33
                  Caption = 'Recurso NA'
                  Checked = True
                  State = cbChecked
                  TabOrder = 0
                  Visible = False
                end
                object cbRetorno: TCheckBox
                  Left = 231
                  Top = 192
                  Width = 97
                  Height = 30
                  BiDiMode = bdLeftToRight
                  Caption = 'Avalia Retorno'
                  Checked = True
                  ParentBiDiMode = False
                  State = cbChecked
                  TabOrder = 1
                  Visible = False
                end
                object edtLambda: TRichEdit
                  Left = 133
                  Top = 24
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 2
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtTLoc: TRichEdit
                  Left = 133
                  Top = 51
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 3
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtTMan: TRichEdit
                  Left = 133
                  Top = 79
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 4
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtPercDefTrans: TRichEdit
                  Left = 133
                  Top = 106
                  Width = 42
                  Height = 22
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 5
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtTRep: TRichEdit
                  Left = 341
                  Top = 22
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 6
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtTManAuto: TRichEdit
                  Left = 341
                  Top = 51
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 7
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtPuLim: TRichEdit
                  Left = 341
                  Top = 79
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 8
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object rgZonaReliga: TRadioGroup
                  Left = 14
                  Top = 153
                  Width = 185
                  Height = 66
                  Caption = 'Zona de Atua'#231#227'o dos Religadores'
                  ItemIndex = 1
                  Items.Strings = (
                    'M'#237'nima (Primeiro Bloco de Carga)'
                    'M'#225'xima')
                  TabOrder = 9
                end
                object edtMes: TEdit
                  Left = 400
                  Top = 79
                  Width = 121
                  Height = 21
                  TabOrder = 10
                  Text = '-1'
                  Visible = False
                end
                object edtRedTprepIF: TRichEdit
                  Left = 341
                  Top = 108
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  Lines.Strings = (
                    '')
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 11
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
                object edtIndispPadrao: TRichEdit
                  Left = 341
                  Top = 137
                  Width = 42
                  Height = 21
                  Alignment = taCenter
                  Font.Charset = ANSI_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Tahoma'
                  Font.Style = []
                  Lines.Strings = (
                    '')
                  ParentFont = False
                  ParentShowHint = False
                  PlainText = True
                  ShowHint = True
                  TabOrder = 12
                  WantReturns = False
                  WordWrap = False
                  Zoom = 100
                end
              end
            end
            object TabSheet24: TTabSheet
              Caption = 'Por Circuito'
              ImageIndex = 1
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object GroupBox2: TGroupBox
                Left = 0
                Top = 0
                Width = 711
                Height = 269
                Align = alClient
                Caption = 'Par'#226'metros por circuito'
                TabOrder = 0
                object sgridParamRede: TStringGrid
                  Left = 2
                  Top = 43
                  Width = 707
                  Height = 224
                  Align = alClient
                  DefaultColWidth = 76
                  DefaultRowHeight = 18
                  RowCount = 2
                  Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
                  TabOrder = 0
                end
                object ToolBar4: TToolBar
                  Left = 2
                  Top = 15
                  Width = 707
                  Height = 28
                  Hint = 'Mostrar VIPs'
                  EdgeBorders = [ebTop, ebBottom]
                  Images = ImageList
                  List = True
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                  Visible = False
                  Wrapable = False
                  object ToolButton22: TToolButton
                    Left = 0
                    Top = 0
                    Action = ActionVoltaParamMdb
                  end
                end
              end
            end
            object TabSheet16: TTabSheet
              Caption = 'Por Bloco'
              ImageIndex = 6
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object sgridParamBloco: TStringGrid
                Left = 0
                Top = 0
                Width = 711
                Height = 269
                Align = alClient
                ColCount = 4
                DefaultColWidth = 76
                DefaultRowHeight = 18
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving, goFixedColClick]
                TabOrder = 0
                OnClick = sgridParamBlocoClick
              end
            end
            object TabSheet25: TTabSheet
              Caption = 'Avan'#231'ados'
              ImageIndex = 2
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object Splitter7: TSplitter
                Left = 177
                Top = 0
                Height = 269
                ExplicitHeight = 301
              end
              object GroupBox6: TGroupBox
                Left = 0
                Top = 0
                Width = 177
                Height = 269
                Align = alLeft
                Caption = 'Distribui'#231#227'o falhas/patamar'
                TabOrder = 0
                object sgridFalhasPat: TStringGrid
                  Left = 2
                  Top = 15
                  Width = 173
                  Height = 252
                  Align = alClient
                  ColCount = 2
                  DefaultColWidth = 50
                  DefaultRowHeight = 18
                  RowCount = 2
                  Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
                  TabOrder = 0
                end
              end
              object GroupBox9: TGroupBox
                Left = 180
                Top = 0
                Width = 531
                Height = 269
                Align = alClient
                Caption = 'Sele'#231#227'o de equipamentos de prote'#231#227'o'
                TabOrder = 1
                object chkLstTipoProt: TCheckListBox
                  Left = 2
                  Top = 15
                  Width = 527
                  Height = 252
                  Align = alClient
                  ItemHeight = 13
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                end
              end
            end
            object TabSheet3: TTabSheet
              Caption = 'Ocorr'#234'ncias'
              ImageIndex = 3
              TabVisible = False
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object Label8: TLabel
                Left = 3
                Top = 19
                Width = 29
                Height = 13
                Caption = 'In'#237'cio:'
              end
              object Label9: TLabel
                Left = 3
                Top = 54
                Width = 42
                Height = 13
                Caption = 'T'#233'rmino:'
              end
              object Button3: TButton
                Left = 16
                Top = 86
                Width = 138
                Height = 25
                Caption = 'Carrega!'
                TabOrder = 0
                OnClick = Button3Click
              end
              object DateTimePickerInicio: TDateTimePicker
                Left = 51
                Top = 16
                Width = 114
                Height = 21
                Date = 40909.000000000000000000
                Time = 40909.000000000000000000
                TabOrder = 1
              end
              object DateTimePickerTermino: TDateTimePicker
                Left = 51
                Top = 51
                Width = 114
                Height = 21
                Date = 42055.999988425930000000
                Time = 42055.999988425930000000
                TabOrder = 2
              end
            end
            object TabSheet11: TTabSheet
              Caption = 'Reiniciar par'#226'metros'
              ImageIndex = 4
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object GroupBox11: TGroupBox
                Left = 0
                Top = 28
                Width = 711
                Height = 81
                Align = alTop
                Caption = 'Reinicia valores hist'#243'ricos'
                TabOrder = 0
                object chkBoxReiniciaParam: TCheckListBox
                  Left = 2
                  Top = 15
                  Width = 707
                  Height = 64
                  Align = alClient
                  BorderStyle = bsNone
                  ItemHeight = 13
                  Items.Strings = (
                    'Equipamento'
                    'Circuito'
                    'Geral'
                    'Hor'#225'ria')
                  TabOrder = 0
                end
              end
              object ToolBar8: TToolBar
                Left = 0
                Top = 0
                Width = 711
                Height = 28
                EdgeBorders = [ebTop, ebBottom]
                Images = ImageList
                List = True
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
                Wrapable = False
                object ToolButton30: TToolButton
                  Left = 0
                  Top = 0
                  Action = ActionVoltaParamOriginais
                end
              end
            end
            object TabSheet13: TTabSheet
              Caption = 'Arquivo de Par'#226'metros'
              ImageIndex = 5
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object lblPathParam: TLabel
                Left = 0
                Top = 0
                Width = 3
                Height = 13
                Align = alTop
                Transparent = False
              end
            end
          end
        end
        object TabSheet14: TTabSheet
          Caption = 'Chaves Autom'#225'ticas'
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter3: TSplitter
            Left = 157
            Top = 0
            Height = 325
            ExplicitHeight = 346
          end
          object GroupBox3: TGroupBox
            Left = 0
            Top = 0
            Width = 157
            Height = 325
            Align = alLeft
            Caption = 'Chaves n'#227'o autom'#225'ticas'
            TabOrder = 0
            object lbChaves: TListBox
              Left = 2
              Top = 15
              Width = 153
              Height = 308
              Align = alClient
              ItemHeight = 13
              MultiSelect = True
              TabOrder = 0
              OnClick = lbChavesClick
            end
          end
          object GroupBox4: TGroupBox
            Left = 160
            Top = 0
            Width = 83
            Height = 325
            Align = alLeft
            TabOrder = 1
            object Button1: TButton
              Left = 4
              Top = 124
              Width = 75
              Height = 25
              Action = ActionSelChvAuto
              TabOrder = 0
            end
            object Button2: TButton
              Left = 4
              Top = 155
              Width = 75
              Height = 25
              Action = ActionRemChvAuto
              TabOrder = 1
            end
          end
          object GroupBox5: TGroupBox
            Left = 243
            Top = 0
            Width = 476
            Height = 325
            Align = alClient
            Caption = 'Chaves autom'#225'ticas'
            TabOrder = 2
            object lbChavesAuto: TListBox
              Left = 2
              Top = 15
              Width = 472
              Height = 308
              Align = alClient
              ItemHeight = 13
              MultiSelect = True
              TabOrder = 0
              OnClick = lbChavesAutoClick
            end
          end
        end
        object TabSheet4: TTabSheet
          Caption = 'Ajuste dos par'#226'metros'
          ImageIndex = 3
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object GroupBox10: TGroupBox
            Left = 0
            Top = 0
            Width = 719
            Height = 325
            Align = alClient
            Caption = 'Ajusta de par'#226'metros'
            TabOrder = 0
            object sgridAjuste: TStringGrid
              Left = 2
              Top = 43
              Width = 715
              Height = 280
              Align = alClient
              Ctl3D = False
              DefaultColWidth = 80
              DefaultRowHeight = 17
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing, goTabs, goThumbTracking]
              ParentCtl3D = False
              ScrollBars = ssVertical
              TabOrder = 0
              OnKeyPress = sgridAjusteKeyPress
              RowHeights = (
                17
                17
                17
                17
                17)
            end
            object ToolBar13: TToolBar
              Left = 2
              Top = 15
              Width = 715
              Height = 28
              EdgeBorders = [ebTop, ebBottom]
              Images = ImageList
              List = True
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
              Wrapable = False
              object ToolButton33: TToolButton
                Left = 0
                Top = 0
                Action = ActionAjusteParam
              end
              object ToolButton21: TToolButton
                Left = 23
                Top = 0
                Width = 8
                Caption = 'ToolButton21'
                ImageIndex = 22
                Style = tbsSeparator
                Visible = False
              end
              object btnAdicionaConjunto: TToolButton
                Left = 31
                Top = 0
                Action = ActionInsereConjunto
              end
              object btnDeletaConjunto: TToolButton
                Left = 54
                Top = 0
                Action = ActionDeletaConjunto
              end
              object ToolButton34: TToolButton
                Left = 77
                Top = 0
                Width = 8
                Caption = 'ToolButton34'
                ImageIndex = 2
                Style = tbsSeparator
              end
              object chkReiniciaParam: TCheckBox
                Left = 85
                Top = 0
                Width = 124
                Height = 22
                Caption = 'Reiniciar par'#226'metros'
                TabOrder = 0
                Visible = False
              end
            end
          end
        end
        object TabSheet19: TTabSheet
          Caption = 'Par'#226'metros Resultantes'
          ImageIndex = 4
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter1: TSplitter
            Left = 0
            Top = 166
            Width = 719
            Height = 3
            Cursor = crVSplit
            Align = alTop
            ExplicitTop = 159
            ExplicitWidth = 723
          end
          object ToolBar10: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
            Wrapable = False
            object ToolButton20: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaRelatVerifAlimBloco
            end
            object butReiniciaParHist: TToolButton
              Left = 23
              Top = 0
              Hint = 'Reiniciar par'#226'metros'
              Action = ActionVoltaParamOriginais
              Enabled = False
            end
          end
          object GroupBox16: TGroupBox
            Left = 0
            Top = 28
            Width = 719
            Height = 138
            Align = alTop
            Caption = 'Par'#226'metros por circuito'
            TabOrder = 1
            object lvParamRede: TListView
              Left = 2
              Top = 15
              Width = 715
              Height = 121
              Align = alClient
              Columns = <
                item
                  Caption = 'Rede'
                  Width = 60
                end
                item
                  Caption = 'Taxa (f/km/ano)'
                  Width = 75
                end
                item
                  Caption = 'Reparo(min)'
                  Width = 75
                end
                item
                  Caption = 'Falhas(f/ano)'
                  Width = 75
                end
                item
                  Caption = 'Comprimento(km)'
                  Width = 75
                end
                item
                  Caption = 'Dura'#231#227'o(min)'
                  Width = 75
                end>
              MultiSelect = True
              ReadOnly = True
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
              ViewStyle = vsReport
              OnClick = ActionMostraBlocoParamExecute
              OnColumnClick = lvParamRedeColumnClick
              OnCompare = lvParamRedeCompare
            end
          end
          object GroupBox17: TGroupBox
            Left = 0
            Top = 169
            Width = 719
            Height = 156
            Align = alClient
            Caption = 'Par'#226'metros por bloco'
            TabOrder = 2
            object lvParamBloco: TListView
              Left = 2
              Top = 15
              Width = 715
              Height = 139
              Align = alClient
              Columns = <
                item
                  Caption = 'Bloco'
                end
                item
                  Caption = 'Equipamento'
                  Width = 60
                end
                item
                  Caption = 'Rede'
                  Width = 75
                end
                item
                  Caption = 'Taxa (f/km/ano)'
                  Width = 75
                end
                item
                  Caption = 'Reparo(min)'
                  Width = 75
                end
                item
                  Caption = 'Comprimento(km)'
                  Width = 75
                end>
              MultiSelect = True
              ReadOnly = True
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
              ViewStyle = vsReport
              OnClick = ActionMostraBlocoParamExecute
              OnColumnClick = lvParamBlocoColumnClick
              OnCompare = lvParamBlocoCompare
            end
          end
        end
        object TabSheet18: TTabSheet
          Caption = 'Identificadores de falta'
          ImageIndex = 5
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter4: TSplitter
            Left = 0
            Top = 202
            Width = 719
            Height = 3
            Cursor = crVSplit
            Align = alTop
            ExplicitTop = 196
            ExplicitWidth = 723
          end
          object GroupBox15: TGroupBox
            Left = 0
            Top = 25
            Width = 719
            Height = 177
            Align = alTop
            TabOrder = 0
            object Splitter2: TSplitter
              Left = 265
              Top = 15
              Width = 2
              Height = 160
            end
            object GroupBox12: TGroupBox
              Left = 2
              Top = 15
              Width = 263
              Height = 160
              Align = alLeft
              Caption = 'Chaves sem IF'
              TabOrder = 0
              object lbChavesSemIF: TListBox
                Left = 2
                Top = 15
                Width = 259
                Height = 143
                Align = alClient
                ItemHeight = 13
                MultiSelect = True
                TabOrder = 0
                OnClick = lbChavesClick
              end
            end
            object GroupBox13: TGroupBox
              Left = 267
              Top = 15
              Width = 83
              Height = 160
              Align = alLeft
              TabOrder = 1
              object Button4: TButton
                Left = 4
                Top = 68
                Width = 75
                Height = 25
                Action = ActionSelChaveIF
                TabOrder = 0
              end
              object Button5: TButton
                Left = 4
                Top = 99
                Width = 75
                Height = 25
                Action = ActionRemChaveIF
                TabOrder = 1
              end
            end
            object GroupBox14: TGroupBox
              Left = 350
              Top = 15
              Width = 367
              Height = 160
              Align = alClient
              Caption = 'Chaves com IF'
              TabOrder = 2
              object lbChavesComIF: TListBox
                Left = 2
                Top = 15
                Width = 363
                Height = 143
                Align = alClient
                ItemHeight = 13
                MultiSelect = True
                TabOrder = 0
                OnClick = lbChavesAutoClick
              end
            end
          end
          object GroupBox18: TGroupBox
            Left = 0
            Top = 205
            Width = 719
            Height = 120
            Align = alClient
            TabOrder = 1
            object GroupBox19: TGroupBox
              Left = 2
              Top = 15
              Width = 715
              Height = 103
              Align = alClient
              Caption = 'Prepara'#231#227'o de indicadores de falta'
              TabOrder = 0
              object tvIF: TTreeView
                Left = 2
                Top = 15
                Width = 711
                Height = 86
                Align = alClient
                Indent = 19
                TabOrder = 0
                OnClick = tvIFClick
              end
            end
          end
          object ToolBar9: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 25
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            Wrapable = False
            object btnLeIF: TToolButton
              Left = 0
              Top = 0
              ImageIndex = 11
              OnClick = ActionLeIFExecute
            end
          end
        end
      end
    end
    object TabSheet10: TTabSheet
      Caption = 'Prepara'#231#227'o de dados'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PageControl4: TPageControl
        Left = 0
        Top = 0
        Width = 727
        Height = 353
        ActivePage = TabSheet12
        Align = alClient
        TabOrder = 0
        object TabSheet12: TTabSheet
          Caption = 'Chaves'
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter6: TSplitter
            Left = 0
            Top = 147
            Width = 719
            Height = 3
            Cursor = crVSplit
            Align = alBottom
            ExplicitTop = 175
            ExplicitWidth = 723
          end
          object GroupBox7: TGroupBox
            Left = 0
            Top = 150
            Width = 719
            Height = 175
            Align = alBottom
            Caption = 'Chaves NA de transfer'#234'ncia'
            TabOrder = 0
            object PageControl5: TPageControl
              Left = 2
              Top = 15
              Width = 715
              Height = 158
              ActivePage = TabSheet17
              Align = alClient
              TabOrder = 0
              object TabSheet6: TTabSheet
                Caption = 'Folga de transfer'#234'ncia de chaves NA'
                ExplicitLeft = 0
                ExplicitTop = 0
                ExplicitWidth = 0
                ExplicitHeight = 0
                object tvChavesNA: TTreeView
                  Left = 0
                  Top = 28
                  Width = 707
                  Height = 102
                  Align = alClient
                  Indent = 19
                  ReadOnly = True
                  TabOrder = 0
                  OnClick = tvChavesNAClick
                end
                object ToolBar12: TToolBar
                  Left = 0
                  Top = 0
                  Width = 707
                  Height = 28
                  EdgeBorders = [ebTop, ebBottom]
                  Images = ImageList
                  List = True
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 1
                  Wrapable = False
                  object ToolButton10: TToolButton
                    Left = 0
                    Top = 0
                    Action = ActionImprimeRelNA
                  end
                end
              end
              object TabSheet17: TTabSheet
                Caption = 'Circuitos de Socorro'
                ImageIndex = 1
                ExplicitLeft = 0
                ExplicitTop = 0
                ExplicitWidth = 0
                ExplicitHeight = 0
                object ToolBar11: TToolBar
                  Left = 0
                  Top = 0
                  Width = 707
                  Height = 28
                  EdgeBorders = [ebTop, ebBottom]
                  Images = ImageList
                  List = True
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                  Wrapable = False
                  object ToolButton31: TToolButton
                    Left = 0
                    Top = 0
                    Action = ActionImprimeCircuitosSocorro
                  end
                end
                object tvCircuitosSocorro: TTreeView
                  Left = 0
                  Top = 28
                  Width = 707
                  Height = 102
                  Align = alClient
                  Indent = 19
                  ReadOnly = True
                  TabOrder = 1
                  OnClick = tvChavesNAClick
                end
              end
            end
          end
          object GroupBox8: TGroupBox
            Left = 0
            Top = 0
            Width = 719
            Height = 147
            Align = alClient
            Caption = 'Chaves de prote'#231#227'o na rede'
            TabOrder = 1
            object tvChavesProt: TTreeView
              Left = 2
              Top = 15
              Width = 715
              Height = 130
              Align = alClient
              Indent = 19
              MultiSelect = True
              MultiSelectStyle = [msControlSelect, msShiftSelect]
              ReadOnly = True
              TabOrder = 0
              OnClick = tvChavesProtClick
              OnMouseUp = tvChavesProtMouseUp
            end
          end
        end
      end
    end
    object TabSheet15: TTabSheet
      Caption = 'Resultados'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PageControl1: TPageControl
        Left = 0
        Top = 0
        Width = 727
        Height = 353
        ActivePage = TabSheet1
        Align = alClient
        TabOrder = 0
        object TabSheet1: TTabSheet
          Caption = 'Gerais'
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter13: TSplitter
            Left = 0
            Top = 241
            Width = 719
            Height = 6
            Cursor = crVSplit
            Align = alTop
            ExplicitTop = 255
            ExplicitWidth = 723
          end
          object lvResultRede: TListView
            Left = 0
            Top = 28
            Width = 719
            Height = 213
            Align = alTop
            Columns = <
              item
                Caption = 'Rede'
              end
              item
                Caption = 'Bloco'
              end
              item
                Caption = 'CLIh (deslig.h/ano)'
              end
              item
                Caption = 'CLI  (deslig/ano)'
              end
              item
                Caption = 'Consumidores'
              end
              item
                Caption = 'DEC (deslig.h/ano.cons.)'
              end
              item
                Caption = 'FEC (deslig/ano.cons.)'
              end
              item
                Caption = 'END (MWh/ano)'
              end
              item
                Caption = 'DECBT (deslig.h/ano.cons.)'
              end
              item
                Caption = 'FECBT (deslh/ano.cons.)'
              end
              item
                Caption = 'ENDBT (MWh/ano)'
              end
              item
                Caption = 'FECPISCA(pisca/ano.cons.)'
              end
              item
                Caption = 'Consumidores VIP'
              end
              item
                Caption = 'FECPISCAVIP(pisca/ano.cons.)'
              end>
            ReadOnly = True
            TabOrder = 0
            ViewStyle = vsReport
            OnColumnClick = lvResultRedeColumnClick
            OnCompare = lvResultRedeCompare
          end
          object lvResult: TListView
            Left = 0
            Top = 274
            Width = 719
            Height = 51
            Align = alClient
            Columns = <
              item
                Caption = 'Rede'
              end
              item
                Caption = 'Bloco'
              end
              item
                Caption = 'CLIh  (deslig.h/ano)'
              end
              item
                Caption = 'CLI  (deslig/ano)'
              end
              item
                Caption = 'Consumidores'
              end
              item
                Caption = 'DEC (deslig.h/ano.cons.)'
              end
              item
                Caption = 'FEC (deslig/ano.cons.)'
              end
              item
                Caption = 'END (MWh/ano)'
              end
              item
                Caption = 'ConsumidoresVIP'
              end
              item
                Caption = 'FECPISCA(pisca/ano.cons.)'
              end>
            ReadOnly = True
            TabOrder = 1
            ViewStyle = vsReport
            OnClick = lvResultClick
            OnColumnClick = lvResultColumnClick
            OnCompare = lvResultCompare
          end
          object ToolBar5: TToolBar
            Left = 0
            Top = 247
            Width = 719
            Height = 27
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
            Wrapable = False
            object ToolButton7: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaResultadosGeraisRes
            end
            object ToolButton11: TToolButton
              Left = 23
              Top = 0
              Action = ActionViewDEC
            end
          end
          object ToolBar6: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
            Wrapable = False
            object ToolButton8: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaResultadosGeraisResRede
            end
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'Defeitos'
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object lvResultDefeito: TListView
            Left = 0
            Top = 28
            Width = 719
            Height = 297
            Action = ActionMostraResBloco
            Align = alClient
            Columns = <
              item
                Caption = 'Rede'
                Width = 70
              end
              item
                Caption = 'Chave Prot.'
              end
              item
                Caption = 'Bloco'
                Width = 40
              end
              item
                Caption = 'Comprimento(km)'
              end
              item
                Caption = 'Falhas(x1000)'
                Width = 40
              end
              item
                Caption = 'Cli (deslig./ano)'
                Width = 70
              end
              item
                Caption = 'Clih (deslig.h/ano)'
                Width = 70
              end
              item
                Caption = 'ManobrasAuto'
                Width = 80
              end
              item
                Caption = 'ManobrasManuais'
                Width = 80
              end
              item
                Caption = 'TReparo(min)'
              end
              item
                Caption = 'Ptpre'
              end
              item
                Caption = 'Ptdlo'
              end
              item
                Caption = 'Ptexe'
              end
              item
                Caption = 'FatIFDlo'
              end
              item
                Caption = 'FatIFPre'
              end>
            ReadOnly = True
            TabOrder = 0
            ViewStyle = vsReport
            OnColumnClick = lvResultDefeitoColumnClick
            OnCompare = lvResultDefeitoCompare
            OnDblClick = lvResultDefeitoDblClick
          end
          object ToolBar3: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Wrapable = False
            object ToolButton4: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaRelatorio
            end
            object ToolButton13: TToolButton
              Left = 23
              Top = 0
              Action = ActionViewCLI
            end
            object ToolButton3: TToolButton
              Left = 46
              Top = 0
              Action = ActionGrafGanhosNA
            end
          end
        end
        object TabSheet5: TTabSheet
          Caption = 'Compensa'#231#245'es'
          ImageIndex = 3
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object lvComp: TListView
            Left = 0
            Top = 28
            Width = 719
            Height = 297
            Align = alClient
            Columns = <
              item
                Caption = 'Rede'
              end
              item
                Caption = 'Compensa'#231#227'o(R$/Ano)'
                Width = 130
              end
              item
                Caption = 'Deslig.h/Trafo Vip'
                Width = 130
              end
              item
                Caption = 'Deslig/Trafo Vip'
                Width = 130
              end>
            ReadOnly = True
            TabOrder = 0
            ViewStyle = vsReport
            OnColumnClick = lvCompColumnClick
            OnCompare = lvCompCompare
          end
          object ToolBar7: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Wrapable = False
            object ToolButton29: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaRelatorioCompensacoes
            end
          end
        end
        object TabSheet9: TTabSheet
          Caption = 'Individuais'
          ImageIndex = 3
          TabVisible = False
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object lvIndicIndiv: TListView
            Left = 0
            Top = 28
            Width = 719
            Height = 297
            Align = alClient
            Columns = <
              item
                Caption = 'Rede'
              end
              item
                Caption = 'Carga'
              end
              item
                Caption = 'VIP'
              end
              item
                Caption = 'MT'
              end
              item
                Caption = 'Rural'
              end
              item
                Caption = 'DIC  (deslig.h/ano)'
              end
              item
                Caption = 'FIC  (deslig/ano)'
              end
              item
                Caption = 'DICBT  (deslig.h/ano)'
              end
              item
                Caption = 'FICBT  (deslig/ano)'
              end
              item
                Caption = 'Consumidores'
              end
              item
                Caption = 'Compensa'#231#227'oTotal'
              end
              item
                Caption = 'dicPu01'
              end
              item
                Caption = 'dicPu02'
              end
              item
                Caption = 'dicPu03'
              end
              item
                Caption = 'dicPu04'
              end
              item
                Caption = 'dicPu05'
              end
              item
                Caption = 'dicPu06'
              end
              item
                Caption = 'dicPu07'
              end
              item
                Caption = 'dicPu08'
              end
              item
                Caption = 'dicPu09'
              end
              item
                Caption = 'dicPu10'
              end
              item
                Caption = 'dicPu11'
              end
              item
                Caption = 'dicPu12'
              end
              item
                Caption = 'ficPu01'
              end
              item
                Caption = 'ficPu02'
              end
              item
                Caption = 'ficPu03'
              end
              item
                Caption = 'ficPu04'
              end
              item
                Caption = 'ficPu05'
              end
              item
                Caption = 'ficPu06'
              end
              item
                Caption = 'ficPu07'
              end
              item
                Caption = 'ficPu08'
              end
              item
                Caption = 'ficPu09'
              end
              item
                Caption = 'ficPu10'
              end
              item
                Caption = 'ficPu11'
              end
              item
                Caption = 'ficPu12'
              end>
            ReadOnly = True
            TabOrder = 0
            ViewStyle = vsReport
            OnColumnClick = lvIndicIndivColumnClick
            OnCompare = lvIndicIndivCompare
          end
          object ToolBar1: TToolBar
            Left = 0
            Top = 0
            Width = 719
            Height = 28
            EdgeBorders = [ebTop, ebBottom]
            Images = ImageList
            List = True
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            Wrapable = False
            object ToolButton16: TToolButton
              Left = 0
              Top = 0
              Action = ActionExportaResIndicIndividuais
            end
          end
        end
      end
    end
  end
  object StatusBarCount: TStatusBar [2]
    Left = 0
    Top = 406
    Width = 735
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  inherited ActionList: TActionList
    Left = 456
    Top = 256
    object ActionSelEqpto: TAction
      Caption = 'ActionSelEqpto'
      OnExecute = ActionSelEqptoExecute
    end
    object ActionAvaliaVis: TAction
      Caption = 'Avalia'
    end
    object ActionDefListas: TAction
      Caption = 'Listas'
      Hint = 'C'#225'lculo de Indicadores de Continuidade'
      ImageIndex = 2
      OnExecute = ActionDefListasExecute
    end
    object ActionFormParam: TAction
      Caption = 'Par'#226'metros'
      ImageIndex = 4
    end
    object ActionConfirmaParamGerais: TAction
      Caption = 'Confirma'
      ImageIndex = 7
      OnExecute = ActionConfirmaParamGeraisExecute
    end
    object ActionSelChvAuto: TAction
      Caption = '--->'
      OnExecute = ActionSelChvAutoExecute
    end
    object ActionRemChvAuto: TAction
      Caption = '<---'
      OnExecute = ActionRemChvAutoExecute
    end
    object ActionMostraLbChaves: TAction
      Caption = 'ActionMostraLbChaves'
      OnExecute = ActionMostraLbChavesExecute
    end
    object ActionMostraResBloco: TAction
      Caption = 'ActionMostraResBloco'
      OnExecute = ActionMostraResBlocoExecute
    end
    object ActionReiniciar: TAction
      Caption = 'ActionReiniciar'
      Hint = 'Reiniciar topologia'
      ImageIndex = 24
      OnExecute = ActionReiniciarExecute
    end
    object ActionExportaRelatorio: TAction
      Caption = 'ActionExportaRelatorio'
      ImageIndex = 10
      OnExecute = ActionExportaRelatorioExecute
    end
    object ActionExportaDetalhaDefeito: TAction
      Caption = 'ActionExportaDetalhaDefeito'
      ImageIndex = 10
    end
    object ActionMostraDetalheDefeito: TAction
      Caption = 'ActionMostraDetalheDefeito'
    end
    object ActionExportaResultadosGeraisResRede: TAction
      Caption = 'ActionExportaResultadosGeraisResRede'
      ImageIndex = 10
      OnExecute = ActionExportaResultadosGeraisResRedeExecute
    end
    object ActionExportaResultadosGeraisRes: TAction
      Caption = 'ActionExportaResultadosGeraisRes'
      ImageIndex = 10
      OnExecute = ActionExportaResultadosGeraisResExecute
    end
    object ActionArqParam: TAction
      Caption = 'ActionArqParam'
      ImageIndex = 11
      OnExecute = ActionArqParamExecute
    end
    object ActionAjusteParam: TAction
      Caption = 'Ajuste'
      Hint = 'Ajuste dos par'#226'metros para valores verificados'
      ImageIndex = 12
      OnExecute = ActionAjusteParamExecute
    end
    object ActionCalcIndDeriv: TAction
      Caption = 'ActionCalcIndDeriv'
      Hint = 'C'#225'lculo de compensa'#231#245'es regulat'#243'rias'
      ImageIndex = 13
      OnExecute = ActionCalcIndDerivExecute
    end
    object ActionClusterChavesNA: TAction
      Caption = 'ActionClusterChavesNA'
      ImageIndex = 14
    end
    object ActionDefLisBLocosDiv: TAction
      Caption = 'ActionDefLisBLocosDiv'
      Hint = 'Defini'#231#227'o de blocos divis'#237'veis'
      ImageIndex = 2
    end
    object ActionLisBlocosCand: TAction
      Caption = 'ActionLisBlocosCand'
    end
    object ActionRejeitaParamGerais: TAction
      Caption = 'ActionRejeitaParamGerais'
      ImageIndex = 16
      OnExecute = ActionRejeitaParamGeraisExecute
    end
    object ActionArqCircuitoVerificado: TAction
      Caption = 'ActionArqCircuitoVerificado'
      ImageIndex = 11
    end
    object ActionArqOcorrVerificado: TAction
      Caption = 'ActionArqOcorrVerificado'
      ImageIndex = 11
    end
    object ActionMostraBlocoParam: TAction
      Caption = 'ActionMostraBlocoParam'
      OnExecute = ActionMostraBlocoParamExecute
    end
    object ActionAtualizaParamMedios: TAction
      Caption = 'ActionAtualizaParamMedios'
      Hint = 'Atualiza par'#226'metros m'#233'dios'
      ImageIndex = 7
    end
    object ActionExportaRelatVerifAlimBloco: TAction
      Caption = 'ActionExportaRelatVerifAlimBloco'
      ImageIndex = 10
      OnExecute = ActionExportaRelatVerifAlimBlocoExecute
    end
    object ActionExportaRelatOcorrBloco: TAction
      Caption = 'ActionExportaRelatOcorrBloco'
      ImageIndex = 10
    end
    object ActionExportaRelatOcorrAlim: TAction
      Caption = 'ActionExportaRelatOcorrAlim'
      ImageIndex = 10
    end
    object ActionImprime: TAction
      Caption = 'ActionImprime'
    end
    object ActionEditaParamJusProt: TAction
      Caption = 'ActionEditaParamJusProt'
      Hint = 'Edi'#231#227'o par'#226'metros por chave prote'#231#227'o'
      ImageIndex = 18
      OnExecute = ActionEditaParamJusProtExecute
    end
    object ActionImprimeRelNA: TAction
      Caption = 'ActionImprimeRelNA'
      ImageIndex = 10
      OnExecute = ActionImprimeRelNAExecute
    end
    object ActionViewDEC: TAction
      Caption = 'ActionViewDEC'
      ImageIndex = 19
      OnExecute = ActionViewDECExecute
    end
    object ActionMostraVIP: TAction
      Caption = 'ActionMostraVIP'
      ImageIndex = 17
      OnExecute = ActionMostraVIPExecute
    end
    object ActionViewCLI: TAction
      Caption = 'ActionViewCLI'
      ImageIndex = 19
      OnExecute = ActionViewCLIExecute
    end
    object ActionMostraParamAux: TAction
      Caption = 'ActionMostraParamAux'
      OnExecute = ActionMostraParamAuxExecute
    end
    object ActionSalvar: TAction
      Caption = 'Salvar'
      Hint = 'Salvar par'#226'metros'
      ImageIndex = 20
      OnExecute = ActionSalvarExecute
    end
    object ActionExportaResIndicIndividuais: TAction
      Caption = 'ActionExportaResIndicIndividuais'
      ImageIndex = 10
      OnExecute = ActionExportaResIndicIndividuaisExecute
    end
    object ActionVoltaParamOriginais: TAction
      Caption = 'ActionVoltaParamOriginais'
      ImageIndex = 21
      OnExecute = ActionVoltaParamOriginaisExecute
    end
    object ActionVoltaParamMdb: TAction
      Caption = 'ActionCarregaParametrosMdb'
      Hint = 'Recarrega par'#226'metros'
      ImageIndex = 21
      OnExecute = ActionVoltaParamMdbExecute
    end
    object ActionGravaParamMdb: TAction
      Caption = 'ActionGravaParamMdb'
      Hint = 'Grava de arquivo (paramConf.mdb)'
      ImageIndex = 20
      OnExecute = ActionGravaParamMdbExecute
    end
    object ActionLeArqMdb: TAction
      Caption = 'ActionLeArqMdb'
      Hint = 'Leitura de arquivo (paramConf.mdb)'
      ImageIndex = 11
      OnExecute = ActionLeArqMdbExecute
    end
    object ActionExecutaMes: TAction
      Caption = 'ActionExecutaMes'
      ImageIndex = 22
      OnExecute = ActionExecutaMesExecute
    end
    object ActionExportaRelatorioCompensacoes: TAction
      Caption = 'ActionExportaRelatorioCompensacoes'
      ImageIndex = 10
      OnExecute = ActionExportaRelatorioCompensacoesExecute
    end
    object ActionGrafGanhosNA: TAction
      Caption = 'ActionGrafGanhosNA'
      ImageIndex = 23
      OnExecute = ActionGrafGanhosNAExecute
    end
    object ActionLeTxtIndObserv: TAction
      Caption = 'ActionLeTxtIndObserv'
      ImageIndex = 11
      OnExecute = ActionLeTxtIndObservExecute
    end
    object ActionImprimeParamSimula: TAction
      Caption = 'ActionImprimeParamSimula'
      Hint = 'Impress'#227'o de par'#226'metros de simula'#231#227'o'
      ImageIndex = 10
      OnExecute = ActionImprimeParamSimulaExecute
    end
    object ActionTeste: TAction
      Caption = 'ActionTeste'
    end
    object ActionInsereConjunto: TAction
      ImageIndex = 0
      OnExecute = ActionInsereConjuntoExecute
    end
    object ActionDeletaConjunto: TAction
      ImageIndex = 16
      OnExecute = ActionDeletaConjuntoExecute
    end
    object ActionSelChaveIF: TAction
      Caption = '--->'
      OnExecute = ActionSelChaveIFExecute
    end
    object ActionRemChaveIF: TAction
      Caption = '<--'
      OnExecute = ActionRemChaveIFExecute
    end
    object ActionPreparaIndicacaoFalta: TAction
      Caption = 'ActionPreparaIndicacaoFalta'
      OnExecute = ActionPreparaIndicacaoFaltaExecute
    end
    object ActionLeIF: TAction
      Caption = 'ActionLeIF'
      OnExecute = ActionLeIFExecute
    end
    object ActionImprimeCircuitosSocorro: TAction
      Caption = 'ActionImprimeCircuitosSocorro'
      ImageIndex = 10
      OnExecute = ActionImprimeCircuitosSocorroExecute
    end
    object ActionNAParalelas: TAction
      Caption = 'ActionNAParalelas'
      ImageIndex = 15
      OnExecute = ActionNAParalelasExecute
    end
  end
  inherited ImageList: TImageList
    Left = 80
    Top = 448
    Bitmap = {
      494C010119002803640410001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000007000000001002000000000000070
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF00000000000000000000000000
      000000000000000000000000000000000000FF00000000000000000000000000
      0000000000000000000091919100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      000000000000000000000000000000000000FF00000000000000000000000000
      0000000000009191910091919100919191000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF0000000000
      000000000000000000000000000000000000FF00000000000000000000000000
      0000000000000000000091919100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      000000000000000000000000000000000000FF00000000000000000000000000
      0000000000000000000091919100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF000000FF000000FF00000087878700878787008787
      8700878787008787870091919100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF00000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF00000000000000000000000000
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
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000007F7F7F0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F700F700000000000000000000000000008484000084
      8400000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600000000000084840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF0000000000BFBF
      BF00000000000000000000000000000000007F7F7F0000000000000000000000
      FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF0000000000FF00
      0000FF000000FF00000000000000000000000000000000000000008484000084
      8400000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600000000000084840000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6550000C65500000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF00BFBFBF0000000000BFBFBF00BFBFBF0000000000BFBF
      BF00000000000000000000000000000000007F7F7F00000000007F7F7F000000
      FF000000FF000000FF007F7F7F0000FFFF0000FFFF0000FFFF007F7F7F00FF00
      0000FF000000FF000000F30CF300000000000000000000000000008484000084
      8400000000000000000000000000000000000000000000000000C6C6C600C6C6
      C600000000000084840000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000A5653900BD550000C655
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000000000000000000000000000000000000000FF00
      0000FF000000FF00000000000000000000000000000000000000008484000084
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000084840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6590000C659
      0000C6550000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000000000000000000000000000000000000000FF00
      0000FF000000FF00000000000000000000000000000000000000008484000084
      8400008484000084840000848400008484000084840000848400008484000084
      840000848400008484000000000000000000000000000000000000000000CE65
      0000C6610000C6550000C6610000C6550000CE6500000000000000000000C659
      0000BD550800C65500000000000000000000000000000000000000000000BFBF
      BF000000000000000000BFBFBF00BFBFBF00BFBFBF000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF00000000000000000000000000000000000000008484000084
      8400000000000000000000000000000000000000000000000000000000000000
      000000848400008484000000000000000000000000000000000000000000CE65
      0000C66D2100D6690000C6590000CE610000CE61000000000000000000000000
      0000CE650000C65500000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000008484000000000000000000000000000000000000000000C65D
      0000CE610000DE6D0000B54D0000000000000000000000000000000000000000
      0000C6550000BD5910000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FF40FF000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000008484000000000000000000000000000000000000000000BD69
      3100CE610000BD550000DE710000BD5500000000000000000000000000000000
      0000BD5D1800BD5500000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000008484000000000000000000000000000000000000000000C655
      0000CE61000000000000C6712100B54D0000BD55000000000000000000000000
      0000BD550000B54D00000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000008484000000000000000000000000000000000000000000CE65
      0000BD5508000000000000000000C66D2100B54D0000CE650000C67D4A00BD5D
      1800B54D0000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000FFFF000000000000FFFF000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6865A00D66D0000B54D0000B54D
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008484000000
      0000C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C60000000000C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000007F7F7F007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      00000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000000000FFFF0000FFFF0000FFFF000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F000000FF000000FF000000FF007F7F7F0000FFFF0000FFFF0000FFFF007F7F
      7F00FF000000FF000000FF0000007F7F7F000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F000000000000000000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F
      7F00FF000000FF000000FF0000007F7F7F000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      0000000000000000000000000000A7A7A7000000000000000000000000000000
      000000000000000000007C7C7C00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      00007F7F7F00000000007F7F7F00000000007F7F7F00000000007F7F7F000000
      00007F7F7F00000000007F7F7F0000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00D0D0D000000000000000000000000000000000000000
      0000000000000000000000000000B2B2B2000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF007C7C7C000000000000000000D9D9D9000000FF00BFBF
      BF00F0F0F00000000000000000004D4D4D007F7F7F007F7F7F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00000000000000000000000000BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00000000000000000000FFFF000000
      00000000000000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000007F7F7F00000000000000
      FF000000FF0000000000000000000000000000000000FF000000000000000000
      000000000000000000000000FF000000FF00000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000C7C7C7009A9A9A004D4D
      4D00000000000000000000000000C7C7C700000000007F7F7F00000000000000
      00000000000000000000000000000000000000FFFF0000000000000000000000
      000000FFFF0000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF00000000000000000000000000FF000000000000000000
      0000000000000000FF000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00D0D0D00000000000000000000000
      00000000000000000000BDBDBD00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      00000000000000FFFF000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F007F7F7F000000FF007F7F7F007F7F7F00FF0000007F7F7F007F7F
      7F000000FF007F7F7F007F7F7F007F7F7F00000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000D9D9D9000000000000000000000000000000
      00008C8C8C00F0F0F00000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F000000FF007F7F7F007F7F7F007F7F7F0000FFFF007F7F7F0000FF
      FF0000FFFF007F7F7F007F7F7F007F7F7F00000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000000000000000FF000000000000000000
      00000000FF00000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF000000000000000000BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000007F7F7F00000000000000
      00000000000000000000000000000000FF0000000000FF000000000000000000
      FF0000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF00000000004D4D4D0000000000BDBDBD0000000000BFBF
      BF00F0F0F00000000000000000007C7C7C00000000007F7F7F00000000000000
      FF000000000000000000000000000000FF000000000000000000000000000000
      000000000000FF0000000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00A7A7A7000000000000000000000000000000
      0000000000000000000000000000D0D0D0000000000000000000000000000000
      00000000FF00000000000000FF00000000000000000000000000000000000000
      0000FF00000000000000FF00000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F00FF0000007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000000000000000BFBF
      BF00000000000000000000000000000000007C7C7C0000000000000000000000
      00000000000000000000A7A7A700000000007F7F7F007F7F7F00000000007F7F
      7F0000000000000000007F7F7F000000FF007F7F7F007F7F7F007F7F7F007F7F
      7F007F7F7F00000000007F7F7F007F7F7F0000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000FFFF000000000000FFFF000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      FF00000000000000FF000000000000000000000000000000000000000000FF00
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000007F7F7F00000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      0000000000000000000000000000FF000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000000000FF00000000000000000000000000000000000000
      0000FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000007F7F7F007F7F7F00000000007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      000000FFFF00BFBFBF0000FFFF00BFBFBF0000FFFF00BFBFBF0000FFFF00BFBF
      BF0000FFFF000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF7671
      F4FFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFBAB8
      F9FF7671F4FF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000000000000000FFFF
      FF000000000000FFFF00BFBFBF0000FFFF00BFBFBF0000FFFF00BFBFBF0000FF
      FF00BFBFBF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000241CEDFF322AEEFFD6D4FCFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFADAAF8FF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000000000000000000000000000000FF
      FF00FFFFFF000000000000FFFF00BFBFBF0000FFFF00BFBFBF0000FFFF00BFBF
      BF0000FFFF00BFBFBF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      000000000000000000000000000000000000241CEDFF9F9BF7FFFFFFFFFFD6D4
      FCFF5B55F2FF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF6963
      F3FFF1F1FEFFFFFFFFFF8480F5FF241CEDFF0000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000000000000000FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF0000000000000000000000000000000000241CEDFFFFFFFFFFFFFFFFFF3F38
      EFFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF5B55F2FFFFFFFFFFD6D4FCFF241CEDFF0000000000000000000000000000
      FF00000000000000000000000000FFFFFF0000000000BFBFBF00000000000000
      0000FFFFFF0000000000FFFFFF000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF0000000000000000000000000000000000241CEDFFFFFFFFFFC8C6FAFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFFFFFFFFFFFFFFFFFF241CEDFF0000000000000000000000000000
      FF000000FF000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000FFFF0000000000FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF0000000000000000000000000000000000241CEDFFFFFFFFFFF1F1FEFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF3F38EFFFFFFFFFFFF1F1FEFF241CEDFF000000000000FF000000FF000000
      FF000000FF000000FF0000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000000000000000000000000000241CEDFFC8C6FAFFFFFFFFFFADAA
      F8FF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF322A
      EEFFBAB8F9FFFFFFFFFFADAAF8FF241CEDFF000000000000FF000000FF000000
      FF000000FF000000FF000000FF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF0000000000000000000000000000FFFF00FFFFFF000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000000000FFFF000000000000FFFF00000000000000
      000000000000000000000000000000000000241CEDFF4D47F0FFF1F1FEFFFFFF
      FFFFE4E3FDFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFC8C6FAFFF1F1
      FEFFFFFFFFFFF1F1FEFF322AEEFF241CEDFF000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF0000000000BFBFBF00FFFFFF000000
      0000FFFFFF0000000000000000000000000000000000FFFFFF0000FFFF007F7F
      7F00000000000000000000000000000000007F7F7F00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF4D47F0FFC8C6
      FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFBAB8F9FF3F38EFFF241CEDFF241CEDFF000000000000FF000000FF000000
      FF000000FF000000FF000000FF0000000000FFFFFF00FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF000000000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241CEDFF241C
      EDFF241CEDFF241CEDFF241CEDFF241CEDFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000007F7F7F00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF000000000000FFFF000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
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
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007F7F
      7F00000000007F7F7F0000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF0000000000000000000000000000000000000000000000000000000000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      00000000FF000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF000000FF000000FF000000FF00BFBF
      BF00000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      FF00000000007F7F7F0000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F00000000000000FF000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF0000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      000000000000000000000000FF0000000000000000000000FF00000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF000000000000000000000000000000000000000000FFFF00000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF00000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF0000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000BFBFBF0000000000BFBFBF0000000000BFBFBF0000000000BFBF
      BF0000000000000000000000000000000000000000000000000000FFFF000000
      000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF0000000000000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF000000000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F0000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000000000FFFFFF0000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000FFFF000000000000FFFF000000000000BFBF
      BF0000000000000000000000000000000000FF000000FF000000000000000000
      000000000000000000000000000000FFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF0000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000000000000000000000000000BFBF
      BF0000000000000000000000000000000000FF000000FF0000000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F0000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF000000000000000000000000000000000000000000000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF000000FF000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000000000000000000000BFBF
      BF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF00424D3E000000000000003E000000
      2800000040000000700000000100010000000000800300000000000000000000
      000000000000000000000000FFFFFF00FFF7000000000000FFEF000000000000
      FFDF000000000000FFBF0000000000007F7D000000000000BF78000000000000
      DF7D000000000000EF7D000000000000F001000000000000FF7F000000000000
      FFBF000000000000FFDF000000000000FFEF000000000000FFDF000000000000
      FFBF000000000000FF7F000000000000FFFFFFFFC007FFFFC001FFFFC0070001
      8001FFFFC00722238001FF9FC00700018001FF8FC007A2238001FFC7C007A3E3
      8001E063C007BFE38001E073C007BFFF8001E1F3C007FFFE8001E0F3C00707E0
      8001E473C00777EE8001E607C007718E8001FF0FC00776EE8001FFFFC0070760
      8001FFFFC007FFFFFFFFFFFFC007FFFFFFFFFFFFFFFFF111FFFFFFFFFFFFFFFF
      FFFFFFFFFFFF0000E7F3FFFFFFFFD111E3E3EEBFFFFF9111F1C7E6BFFFFFD111
      F88FD6BF07E00000FC1FDA8777EED111FE3FBABB718E91F1FC1F7CBB76EED1F1
      F88F7C8707600000F1C7FFFFFFFFDFF1E3E3FEFFFFFF9FF1E7F3FEFFFFFFDFFF
      FFFFFFFFFFFF0000FFFFFFFFFFFFFFFFDDDDC001F111FFFFD555C000FFFFFFFF
      0000C0000000FF83DFBFC000DEDBDF0187BCC0009F57CF83DBBBC001DFDB07FF
      0000C00300000383DDB7C000DFDF01019EAFC0008EFB0383DF1FC000D1F507FF
      0000C0010000CF83DFB7C007CBE1DF019F82C0079DF7FF83DFB7C007DFFDFFFF
      0000C0070000FFFFFFFFC007FFFFFFFFE00F0000FFFFFFFFE00F0000FC00E007
      E00F0000FC00C003F00F0000FC00C001F00F0000FC00C000F80F0000EC00C000
      F0070000E4008007F0070000E0000007F00700000000000FF00F00000001001F
      F01F00000003001FF81F000000070018FE1F0000000F007CFF1F0000E3FF005A
      FF1F0000E7FF81C7FFBF0000EFFF81FFFDFFFFFFFFFFFFFFF8FFFFFFFFFFFFFF
      F8FFFFF81FFFFE7FF87FF820041FFC3FF81FF000000FF81FF80FF000000FF19F
      F00FE0000007E38FE00F80000001E7CFE00F00000000FFC7E00F80000001FFE7
      F01FFC00003FFFE3F00FFE3FFC7FFFF3F00FFFFFFFFFFFF3F007FFFFFFFFFFFF
      F007FFFFFFFFFFFFF007FFFFFFFFFFFFFFFFFC7FC007FFFFFFE3F83FC007C600
      FFC3F01FC0078200FB83F01FC0070000F907F01FC0070000F80FF01FC0070000
      F01FF01FC0078000F03FF01FC0078000E01FF83FC0078000E00FFEC3C0078000
      C07FFEB9C007C800C1FFFF7DC0073C0087FFFF3DC0070E009FFFFC99C007CE00
      FFFFF9C3C0072600FFFFF3FFC007260000000000000000000000000000000000
      000000000000}
  end
  inherited PopupMenu: TPopupMenu
    Left = 344
    Top = 456
    inherited mnuAlignLeft: TMenuItem
      ImageIndex = 5
    end
    inherited mnuAlignRight: TMenuItem
      ImageIndex = 6
    end
    inherited mnuAlignDown: TMenuItem
      ImageIndex = 4
    end
    inherited mnuAlignUp: TMenuItem
      ImageIndex = 8
    end
    inherited mnuAlignNode: TMenuItem
      ImageIndex = 3
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '*.txt'
    Filter = 'Todos arquivos|*.*'
    Left = 272
    Top = 456
  end
  object MainMenu1: TMainMenu
    Left = 152
    Top = 448
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo Texto (*.txt)|*.txt|Arquivo Excel (*.csv)|*.csv'
    Left = 200
    Top = 448
  end
end
