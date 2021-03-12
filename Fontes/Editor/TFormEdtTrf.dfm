object FormEdtTrf: TFormEdtTrf
  Left = 267
  Top = 398
  BorderIcons = [biSystemMenu]
  Caption = 'Edi'#231#227'o de Transformador'
  ClientHeight = 291
  ClientWidth = 372
  Color = clBtnFace
  Constraints.MinHeight = 330
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgCtrl: TPageControl
    Left = 0
    Top = 26
    Width = 372
    Height = 265
    ActivePage = TabSheetSecundario
    Align = alClient
    TabOrder = 0
    OnChange = pgCtrlChange
    object TabSheetTrafo: TTabSheet
      Caption = 'Trafo'
      object PanelTrafoLeft: TPanel
        Left = 0
        Top = 55
        Width = 200
        Height = 182
        Align = alClient
        BevelOuter = bvNone
        Constraints.MinWidth = 200
        TabOrder = 0
        object PanelLtc: TPanel
          Left = 0
          Top = 54
          Width = 200
          Height = 36
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label3: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 88
            Height = 30
            Align = alLeft
            AutoSize = False
            Caption = 'LTC: Ajuste autom'#225'tico'
            Layout = tlCenter
            WordWrap = True
          end
          object CBoxLtcTipo: TComboBox
            AlignWithMargins = True
            Left = 97
            Top = 3
            Width = 100
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
            OnChange = CBoxLtcTipoChange
            Items.Strings = (
              'N'#227'o Utilizado'
              'Tap Prim'#225'rio'
              'Tap Secund'#225'rio')
          end
        end
        object PanelPerdaFerro: TPanel
          Left = 0
          Top = 27
          Width = 200
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label5: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 88
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Perda ferro (%)'
            Layout = tlCenter
          end
          object edtPerdaFe: TEdit
            AlignWithMargins = True
            Left = 97
            Top = 3
            Width = 100
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object PanelSnom: TPanel
          Left = 0
          Top = 0
          Width = 200
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object LabelSnom: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 88
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Snom (MVA)'
            Layout = tlCenter
          end
          object edtSnom: TEdit
            AlignWithMargins = True
            Left = 97
            Top = 3
            Width = 100
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object GBoxLTC: TGroupBox
          Left = 0
          Top = 90
          Width = 200
          Height = 87
          Align = alTop
          Caption = 'Configura'#231#227'o do LTC autom'#225'tico'
          TabOrder = 3
          object PanelLtcBarra: TPanel
            Left = 2
            Top = 15
            Width = 196
            Height = 33
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label8: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 86
              Height = 27
              Align = alLeft
              AutoSize = False
              Caption = 'Barra controalda'
              Layout = tlCenter
              WordWrap = True
            end
            object CBoxLtcBar: TComboBox
              AlignWithMargins = True
              Left = 95
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxLtcBarChange
            end
          end
          object PanelLtcTensao: TPanel
            Left = 2
            Top = 48
            Width = 196
            Height = 39
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Label6: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 86
              Height = 33
              Align = alLeft
              AutoSize = False
              Caption = 'Tens'#227'o controlada (pu)'
              Layout = tlCenter
              WordWrap = True
            end
            object EditLtcVpu: TEdit
              AlignWithMargins = True
              Left = 95
              Top = 6
              Width = 98
              Height = 21
              Margins.Top = 6
              Margins.Bottom = 6
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
        end
      end
      object PanelTrafoRight: TPanel
        Left = 200
        Top = 55
        Width = 164
        Height = 182
        Align = alRight
        BevelOuter = bvNone
        Constraints.MinWidth = 164
        TabOrder = 1
        object PanelZ0Z1: TPanel
          Left = 0
          Top = 0
          Width = 164
          Height = 138
          Align = alTop
          AutoSize = True
          BevelOuter = bvNone
          TabOrder = 0
          object PanelR0: TPanel
            Left = 0
            Top = 0
            Width = 164
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 0
            object Label9: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 54
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'r0 (pu)'
              Layout = tlCenter
              ExplicitHeight = 24
            end
            object EditR0: TEdit
              AlignWithMargins = True
              Left = 63
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
          object PanelR1: TPanel
            Left = 0
            Top = 54
            Width = 164
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 1
            object Label13: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 54
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'r1 (pu)'
              Layout = tlCenter
            end
            object EditR1: TEdit
              AlignWithMargins = True
              Left = 63
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
          object PanelSbase: TPanel
            Left = 0
            Top = 111
            Width = 164
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object Label20: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 54
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Sbase'
              Layout = tlCenter
            end
            object CBoxSbase: TComboBox
              AlignWithMargins = True
              Left = 63
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Style = csDropDownList
              ItemIndex = 0
              TabOrder = 0
              Text = 'Pot.Nominal'
              OnChange = CBoxSbaseChange
              Items.Strings = (
                'Pot.Nominal'
                '100 MVA')
            end
          end
          object PanelX0: TPanel
            Left = 0
            Top = 27
            Width = 164
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 3
            object Label14: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 54
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'x0 (pu)'
              Layout = tlCenter
              ExplicitHeight = 15
            end
            object EditX0: TEdit
              AlignWithMargins = True
              Left = 63
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
          object PanelX1: TPanel
            Left = 0
            Top = 81
            Width = 164
            Height = 30
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 4
            object Label15: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 54
              Height = 24
              Align = alLeft
              AutoSize = False
              Caption = 'x1 (pu)'
              Layout = tlCenter
              ExplicitHeight = 30
            end
            object EditX1: TEdit
              AlignWithMargins = True
              Left = 63
              Top = 3
              Width = 98
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
        end
        object PanelAutoTrafo: TPanel
          Left = 0
          Top = 138
          Width = 164
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object checkAutoTrafo: TCheckBox
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 158
            Height = 21
            Align = alClient
            Caption = 'Auto-transfomador'
            TabOrder = 0
            OnClick = checkAutoTrafoClick
          end
        end
      end
      object PanelTrafoTop: TPanel
        Left = 0
        Top = 0
        Width = 364
        Height = 55
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
        object PanelCodigo: TPanel
          Left = 0
          Top = 27
          Width = 364
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Label1: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 88
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'C'#243'digo'
            Layout = tlCenter
          end
          object EditCodigo: TEdit
            AlignWithMargins = True
            Left = 97
            Top = 3
            Width = 264
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object PanelRede: TPanel
          Left = 0
          Top = 0
          Width = 364
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label4: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 88
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Rede'
            Layout = tlCenter
          end
          object CBoxRede: TComboBox
            AlignWithMargins = True
            Left = 97
            Top = 3
            Width = 264
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
            OnChange = CBoxRedeChange
            Items.Strings = (
              'n'#227'o utilizada'
              'Disjuntor'
              'Religadora'
              'Seccionadora')
          end
        end
      end
    end
    object TabSheetPrimario: TTabSheet
      Caption = 'Prim'#225'rio'
      ImageIndex = 1
      object PanelPrimarioRight: TPanel
        AlignWithMargins = True
        Left = 196
        Top = 3
        Width = 165
        Height = 231
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object gboxPriZterra: TGroupBox
          Left = 0
          Top = 0
          Width = 165
          Height = 126
          Align = alTop
          Caption = 'Imped'#226'ncia de aterramento'
          Constraints.MinWidth = 145
          TabOrder = 0
          object Panel3: TPanel
            Left = 2
            Top = 42
            Width = 161
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 0
            object LabelPriRterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 34
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'R(ohm)'
              Layout = tlCenter
              ExplicitLeft = 5
              ExplicitTop = 22
              ExplicitHeight = 13
            end
            object EditPriRat: TEdit
              AlignWithMargins = True
              Left = 43
              Top = 3
              Width = 115
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
          object Panel4: TPanel
            Left = 2
            Top = 15
            Width = 161
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 1
            object LabelPriXterra: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'X(ohm)'
              Layout = tlCenter
              ExplicitHeight = 30
            end
            object EditPriXat: TEdit
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 116
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
        end
      end
      object PanelPrimarioLeft: TPanel
        Left = 0
        Top = 0
        Width = 193
        Height = 237
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object gboxPrimario: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 187
          Height = 127
          Align = alTop
          Caption = 'Prim'#225'rio'
          Constraints.MinWidth = 187
          TabOrder = 0
          object PanelPriBarra: TPanel
            Left = 2
            Top = 15
            Width = 183
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 0
            object Label7: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 57
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object CBoxPriBar: TComboBox
              AlignWithMargins = True
              Left = 66
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxPriBarChange
            end
          end
          object PanelPriLigacao: TPanel
            Left = 2
            Top = 42
            Width = 183
            Height = 27
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 1
            object LabelPriLiga: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 57
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object CBoxPriLiga: TComboBox
              AlignWithMargins = True
              Left = 66
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxPriLigaChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela')
            end
          end
          object PanelPriTensao: TPanel
            Left = 2
            Top = 69
            Width = 183
            Height = 54
            Margins.Bottom = 0
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 2
            object Panel1: TPanel
              Left = 0
              Top = 0
              Width = 183
              Height = 27
              Margins.Left = 0
              Margins.Right = 0
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 0
              object LabelPriVnom: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 57
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Vnom(kV)'
                Layout = tlCenter
              end
              object EditPriVnom: TEdit
                AlignWithMargins = True
                Left = 66
                Top = 3
                Width = 114
                Height = 21
                Align = alClient
                Constraints.MaxHeight = 21
                Constraints.MinHeight = 21
                TabOrder = 0
              end
            end
            object Panel2: TPanel
              Left = 0
              Top = 27
              Width = 183
              Height = 27
              Margins.Left = 0
              Margins.Right = 0
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              object LabelPriTap: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 57
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Tap(kV)'
                Layout = tlCenter
              end
              object EditPriVtap: TEdit
                AlignWithMargins = True
                Left = 66
                Top = 3
                Width = 114
                Height = 21
                Align = alClient
                Constraints.MaxHeight = 21
                TabOrder = 0
              end
            end
          end
        end
      end
    end
    object TabSheetSecundario: TTabSheet
      Caption = 'Secund'#225'rio'
      ImageIndex = 2
      object PanelSecLeft: TPanel
        Left = 0
        Top = 0
        Width = 193
        Height = 237
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object gboxSecundario: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 187
          Height = 127
          Align = alTop
          Caption = 'Secund'#225'rio'
          Constraints.MinWidth = 187
          TabOrder = 0
          object PanelSecBarra: TPanel
            Left = 2
            Top = 15
            Width = 183
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label11: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 57
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object CBoxSecBar: TComboBox
              AlignWithMargins = True
              Left = 66
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxSecBarChange
            end
          end
          object PanelSecLigacao: TPanel
            Left = 2
            Top = 42
            Width = 183
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelSecLiga: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 57
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object CBoxSecLiga: TComboBox
              AlignWithMargins = True
              Left = 66
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxSecLigaChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela')
            end
          end
          object PanelSecTensao: TPanel
            Left = 2
            Top = 69
            Width = 183
            Height = 54
            Margins.Top = 0
            Margins.Bottom = 0
            Align = alTop
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 2
            object Panel5: TPanel
              Left = 0
              Top = 27
              Width = 183
              Height = 27
              Margins.Left = 0
              Margins.Top = 0
              Margins.Right = 0
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 0
              object LabelSecTap: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 57
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Tap(kV)'
                Layout = tlCenter
              end
              object EditSecVtap: TEdit
                AlignWithMargins = True
                Left = 66
                Top = 3
                Width = 114
                Height = 21
                Align = alClient
                Constraints.MaxHeight = 21
                TabOrder = 0
              end
            end
            object Panel6: TPanel
              Left = 0
              Top = 0
              Width = 183
              Height = 27
              Margins.Left = 0
              Margins.Top = 0
              Margins.Right = 0
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              object LabelSecVnom: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 57
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Vnom(kV)'
                Layout = tlCenter
                ExplicitLeft = 0
                ExplicitHeight = 19
              end
              object EditSecVnom: TEdit
                AlignWithMargins = True
                Left = 66
                Top = 3
                Width = 114
                Height = 21
                Align = alClient
                Constraints.MaxHeight = 21
                TabOrder = 0
              end
            end
          end
        end
      end
      object PanelSecRight: TPanel
        Left = 193
        Top = 0
        Width = 171
        Height = 237
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 1
        object gboxSecZterra: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 165
          Height = 69
          Align = alTop
          Caption = 'Imped'#226'ncia de aterramento'
          Constraints.MinWidth = 165
          TabOrder = 0
          object Panel7: TPanel
            Left = 2
            Top = 41
            Width = 161
            Height = 28
            Margins.Left = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label21: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 34
              Height = 22
              Align = alLeft
              AutoSize = False
              Caption = 'R(ohm)'
              Layout = tlCenter
            end
            object EditSecRat: TEdit
              AlignWithMargins = True
              Left = 43
              Top = 3
              Width = 115
              Height = 21
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
            end
          end
          object Panel8: TPanel
            Left = 2
            Top = 15
            Width = 161
            Height = 26
            Margins.Left = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Label22: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 20
              Align = alLeft
              AutoSize = False
              Caption = 'X(ohm)'
              Layout = tlCenter
              ExplicitLeft = 9
              ExplicitTop = 28
              ExplicitHeight = 13
            end
            object EditSecXat: TEdit
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 116
              Height = 20
              Align = alClient
              Constraints.MaxHeight = 21
              TabOrder = 0
              ExplicitHeight = 21
            end
          end
        end
        object gboxSecDefasagem: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 78
          Width = 165
          Height = 52
          Align = alTop
          Caption = 'Defasagem prim'#225'rio/secund'#225'rio'
          Constraints.MinWidth = 165
          TabOrder = 1
          object CBoxSecDefasagem: TComboBox
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 155
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
            OnChange = CBoxSecDefasagemChange
          end
        end
      end
    end
    object TabSheetAtributo: TTabSheet
      Caption = 'Atributos edit'#225'veis'
      ImageIndex = 3
      object CLBoxAtributo: TCheckListBox
        Left = 0
        Top = 0
        Width = 364
        Height = 237
        OnClickCheck = CLBoxAtributoClickCheck
        Align = alClient
        BorderStyle = bsNone
        Color = clMoneyGreen
        ItemHeight = 13
        TabOrder = 0
      end
    end
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 372
    Height = 26
    AutoSize = True
    ButtonWidth = 71
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList
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
  object ImageList: TImageList
    Left = 240
    Bitmap = {
      494C010105000700640010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      000000000000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F007F7F
      7F007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F000000FF007F7F
      7F0000FF00007F7F7F007F7F7F00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F00BFBFBF00BFBFBF00000000000000
      00000000000000000000BFBFBF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00BFBFBF00BFBF
      BF00BFBFBF00BFBFBF00BFBFBF007F7F7F00BFBFBF00BFBFBF00000000000000
      0000000000007F7F7F007F7F7F00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      00000000000000000000BFBFBF007F7F7F00BFBFBF00BFBFBF00000000007F7F
      7F00000000007F7F7F00BFBFBF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00BFBFBF00BFBF
      BF00BFBFBF00BFBFBF00BFBFBF007F7F7F00BFBFBF00BFBFBF0000000000BFBF
      BF0000000000BFBFBF00BFBFBF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F007F7F7F007F7F
      7F007F7F7F007F7F7F007F7F7F007F7F7F00BFBFBF00BFBFBF00000000000000
      000000000000BFBFBF00BFBFBF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000BFBFBF00BFBFBF00BFBF
      BF00000000000000000000000000BFBFBF00BFBFBF00BFBFBF00000000000000
      00000000000000000000BFBFBF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000BFBFBF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000BFBFBF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BFBF
      BF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000BFBFBF00BFBFBF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000BFBFBF000000
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
      FF000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF0000000000000000007F7F7F00000000007F7F7F00000000000000FF000000
      FF000000FF0000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF007F7F7F00000000007F7F7F0000000000000000000000
      00000000FF000000FF00000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000007F7F7F00000000007F7F7F0000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF00000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF00000000000000000000000000BFBFBF00BFBFBF00FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000BFBFBF00BFBFBF00000000000000000000000000000000000000
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
      000000000000000000000000FFFFFF00FC070000000000008003000000000000
      0007000000000000003C00000000000000180000000000000000000000000000
      0000000000000000000000000000000000000000000000000018000000000000
      803C000000000000E3FF00000000000087FF0000000000000FFF000000000000
      8FFF000000000000DFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFF83F0000
      FE7FFFFFE00F0000FC3FE7F3CC470000F81FE3E384630000F19FF1C7A0730000
      E38FF88F31F90000E7CFFC1F38F90000FFC7FE3F3C790000FFE7FC1F3C390000
      FFE3F88F3C190000FFF3F1C79C0B0000FFF3E3E38C430000FFFFE7F3C4670000
      FFFFFFFFE00FFFFFFFFFFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 272
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
    object ActionSelFoto: TAction
      Caption = 'Imagens'
      ImageIndex = 4
      Visible = False
      OnExecute = ActionSelFotoExecute
    end
    object ActionSelPadrao: TAction
      Caption = 'Selecionar Padr'#227'o'
      ImageIndex = 3
      OnExecute = ActionSelPadraoExecute
    end
  end
end
