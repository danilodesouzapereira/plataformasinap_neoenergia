object FormEdtTrf3E: TFormEdtTrf3E
  Left = 262
  Top = 166
  BorderIcons = [biSystemMenu]
  Caption = 'Edi'#231#227'o de Transformador de 3 Enrolamentos'
  ClientHeight = 263
  ClientWidth = 367
  Color = clBtnFace
  Constraints.MinHeight = 302
  Constraints.MinWidth = 383
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgCtrl: TPageControl
    Left = 0
    Top = 26
    Width = 367
    Height = 237
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 0
    object tsTrafo: TTabSheet
      Caption = 'Dados gerais'
      object GroupBox4: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 111
        Width = 353
        Height = 95
        Align = alClient
        Caption = 'Imped'#226'ncia de Sequ'#234'ncia (pu)'
        TabOrder = 0
        object sgrid: TStringGrid
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 343
          Height = 72
          Align = alClient
          DefaultColWidth = 58
          DefaultRowHeight = 15
          RowCount = 4
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs, goAlwaysShowEditor]
          ScrollBars = ssNone
          TabOrder = 0
        end
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 359
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object Label2: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 66
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'C'#243'digo'
          Layout = tlCenter
        end
        object EditCodigo: TEdit
          AlignWithMargins = True
          Left = 75
          Top = 3
          Width = 281
          Height = 21
          Align = alClient
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 54
        Width = 359
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
        object Label20: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 66
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Sbase'
          Layout = tlCenter
        end
        object CBoxSbase: TComboBox
          AlignWithMargins = True
          Left = 75
          Top = 3
          Width = 281
          Height = 21
          Align = alClient
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 0
          Text = 'Pot.Nominal m'#225'xima'
          OnChange = CBoxSbaseChange
          Items.Strings = (
            'Pot.Nominal m'#225'xima'
            '100 MVA')
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 27
        Width = 359
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 3
        object Label4: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 66
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Rede'
          Layout = tlCenter
        end
        object cboxRede: TComboBox
          AlignWithMargins = True
          Left = 75
          Top = 3
          Width = 281
          Height = 21
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
          Items.Strings = (
            'n'#227'o utilizada'
            'Disjuntor'
            'Religadora'
            'Seccionadora')
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 81
        Width = 359
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 4
        object Label5: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 66
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Perda ferro(%)'
          Layout = tlCenter
          ExplicitLeft = 7
          ExplicitTop = 29
          ExplicitHeight = 13
        end
        object edtPerdaFe: TEdit
          AlignWithMargins = True
          Left = 75
          Top = 3
          Width = 281
          Height = 21
          Align = alClient
          TabOrder = 0
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Prim'#225'rio'
      ImageIndex = 1
      object Panel5: TPanel
        Left = 185
        Top = 0
        Width = 174
        Height = 209
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object gboxPriZterra: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 168
          Height = 154
          Align = alTop
          Caption = 'Imped'#226'ncia de aterramento'
          TabOrder = 0
          object Panel7: TPanel
            Left = 2
            Top = 42
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelPriRterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 44
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'R(ohm)'
              Layout = tlCenter
            end
            object edtPriRterra: TEdit
              AlignWithMargins = True
              Left = 53
              Top = 3
              Width = 108
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel8: TPanel
            Left = 2
            Top = 15
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelPriXterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 44
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'X(ohm)'
              Layout = tlCenter
            end
            object edtPriXterra: TEdit
              AlignWithMargins = True
              Left = 53
              Top = 3
              Width = 108
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
        end
      end
      object Panel6: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 209
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object gboxPrimario: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 179
          Height = 154
          Align = alTop
          Caption = 'Prim'#225'rio'
          TabOrder = 0
          object Panel9: TPanel
            Left = 2
            Top = 15
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label7: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object cboxPriBarra: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxPriBarraChange
            end
          end
          object Panel10: TPanel
            Left = 2
            Top = 42
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelPriLiga: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object cboxPriLiga: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxPriLigaChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela')
            end
          end
          object Panel11: TPanel
            Left = 2
            Top = 123
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object LabelPriTap: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Tap(KV)'
              Layout = tlCenter
            end
            object edtPriTap: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel12: TPanel
            Left = 2
            Top = 96
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object LabelPriVnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Vnom(KV)'
              Layout = tlCenter
            end
            object edtPriVnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
            end
          end
          object Panel13: TPanel
            Left = 2
            Top = 69
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 4
            object LabelPriSnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Snom(MVA)'
              Layout = tlCenter
              ExplicitLeft = 2
              ExplicitTop = 60
              ExplicitHeight = 13
            end
            object edtPriSnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Secund'#225'rio'
      ImageIndex = 2
      object Panel15: TPanel
        Left = 185
        Top = 0
        Width = 174
        Height = 209
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object gboxSecZterra: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 168
          Height = 73
          Align = alTop
          Caption = 'Imped'#226'ncia de aterramento'
          TabOrder = 0
          object Panel19: TPanel
            Left = 2
            Top = 42
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelSecRterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 46
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'R(ohm)'
              Layout = tlCenter
            end
            object edtSecRterra: TEdit
              AlignWithMargins = True
              Left = 55
              Top = 3
              Width = 106
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel20: TPanel
            Left = 2
            Top = 15
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelSecXterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 46
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'X(ohm)'
              Layout = tlCenter
            end
            object edtSecXterra: TEdit
              AlignWithMargins = True
              Left = 55
              Top = 3
              Width = 106
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
        end
        object GBoxSecDefasagem: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 82
          Width = 168
          Height = 44
          Align = alTop
          Caption = 'Defasagem prim'#225'rio/secund'#225'rio'
          TabOrder = 1
          object CBoxSecDefasagem: TComboBox
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 158
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
      end
      object Panel14: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 209
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object gBoxSecundario: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 179
          Height = 155
          Align = alTop
          Caption = 'Secund'#225'rio'
          TabOrder = 0
          object Panel21: TPanel
            Left = 2
            Top = 42
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelSecLiga: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object cboxSecLiga: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxSecLigaChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela')
            end
          end
          object Panel22: TPanel
            Left = 2
            Top = 96
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            ExplicitTop = 123
            object LabelSecVnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Vnom(KV)'
              Layout = tlCenter
            end
            object edtSecVnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
            end
          end
          object Panel23: TPanel
            Left = 2
            Top = 123
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            ExplicitLeft = 3
            ExplicitTop = 128
            object LabelSecTap: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Tap(KV)'
              Layout = tlCenter
            end
            object edtSecTap: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel24: TPanel
            Left = 2
            Top = 69
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object LabelSecSnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Snom(MVA)'
              Layout = tlCenter
              ExplicitLeft = 2
              ExplicitTop = 60
              ExplicitHeight = 13
            end
            object edtSecSnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel25: TPanel
            Left = 2
            Top = 15
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 4
            object Label9: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object cboxSecBarra: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxSecBarraChange
            end
          end
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Terci'#225'rio'
      ImageIndex = 3
      object Panel16: TPanel
        Left = 185
        Top = 0
        Width = 174
        Height = 209
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object GBoxTerDefasagem: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 83
          Width = 168
          Height = 46
          Align = alTop
          Caption = 'Defasagem prim'#225'rio/terci'#225'rio'
          TabOrder = 0
          object CBoxTerDefasagem: TComboBox
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 158
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
        object gboxTerZterra: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 168
          Height = 74
          Align = alTop
          Caption = 'Imped'#226'ncia de aterramento'
          TabOrder = 1
          object Panel26: TPanel
            Left = 2
            Top = 42
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelTerRterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 34
              Height = 21
              Align = alLeft
              Caption = 'R(ohm)'
              Layout = tlCenter
              ExplicitHeight = 13
            end
            object edtTerRterra: TEdit
              AlignWithMargins = True
              Left = 43
              Top = 3
              Width = 118
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel27: TPanel
            Left = 2
            Top = 15
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelTerXterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 21
              Align = alLeft
              Caption = 'X(ohm)'
              Layout = tlCenter
              ExplicitHeight = 13
            end
            object edtTerXterra: TEdit
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 119
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
        end
      end
      object Panel17: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 209
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object gBoxTerciario: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 179
          Height = 154
          Align = alTop
          Caption = 'Terci'#225'rio'
          TabOrder = 0
          object Panel28: TPanel
            Left = 2
            Top = 96
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            ExplicitTop = 69
            object LabelTerVnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Vnom(KV)'
              Layout = tlCenter
            end
            object edtTerVnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
            end
          end
          object Panel29: TPanel
            Left = 2
            Top = 123
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            ExplicitTop = 96
            object LabelTerTap: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Tap(KV)'
              Layout = tlCenter
            end
            object edtTerTap: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel30: TPanel
            Left = 2
            Top = 69
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            ExplicitLeft = 3
            ExplicitTop = 56
            object LabelTerSnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Snom(MVA)'
              Layout = tlCenter
              ExplicitTop = 0
            end
            object edtTerSnom: TEdit
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              TabOrder = 0
              ExplicitTop = -13
            end
          end
          object Panel31: TPanel
            Left = 2
            Top = 42
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object LabelTerLiga: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object cboxTerLiga: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxTerLigaChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela')
            end
          end
          object Panel32: TPanel
            Left = 2
            Top = 15
            Width = 175
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 4
            object Label13: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 56
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object cboxTerBarra: TComboBox
              AlignWithMargins = True
              Left = 65
              Top = 3
              Width = 107
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxTerBarraChange
            end
          end
        end
      end
    end
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 367
    Height = 26
    AutoSize = True
    ButtonWidth = 71
    Caption = 'ToolBar1'
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList1
    List = True
    ShowCaptions = True
    TabOrder = 1
    object butConfirma: TToolButton
      Left = 0
      Top = 0
      Action = ActionConfirma
      AutoSize = True
    end
    object butCancela: TToolButton
      Left = 75
      Top = 0
      Action = ActionCancela
      AutoSize = True
    end
    object butRetira: TToolButton
      Left = 148
      Top = 0
      Action = ActionRetira
      AutoSize = True
    end
  end
  object ImageList1: TImageList
    Left = 232
    Top = 65520
    Bitmap = {
      494C010105000700480010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
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
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000BFBFBF00BFBFBF00FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000BFBFBF00BFBFBF00000000000000000000000000000000000000
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
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF0000000000000000007F7F7F00000000007F7F7F00000000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF007F7F7F00000000007F7F7F0000000000000000000000
      00000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000007F7F7F00000000007F7F7F0000000000000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF000000FF00000000007F7F7F00000000007F7F7F0000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF00FFFF0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FFFF000000000000FFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFF83FFFFF
      FE7FFFFFE00FFFFFFC3FE7F3CC47FFFFF81FE3E38463FFFFF19FF1C7A073FFFF
      E38FF88F31F9FFFFE7CFFC1F38F90000FFC7FE3F3C790000FFE7FC1F3C390000
      FFE3F88F3C19FFFFFFF3F1C79C0BFFFFFFF3E3E38C43FFFFFFFFE7F3C467FFFF
      FFFFFFFFE00FFFFFFFFFFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionList1: TActionList
    Images = ImageList1
    Left = 288
    Top = 65520
    object ActionCancela: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar edi'#231#227'o da barra'
      ImageIndex = 1
      OnExecute = ActionCancelaExecute
    end
    object ActionConfirma: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar edi'#231#227'o da barra'
      ImageIndex = 0
      OnExecute = ActionConfirmaExecute
    end
    object ActionRetira: TAction
      Caption = 'Retirar'
      Hint = 'Retirar barra da rede'
      ImageIndex = 2
      OnExecute = ActionRetiraExecute
    end
  end
end
