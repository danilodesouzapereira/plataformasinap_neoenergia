object FormEdtReguladorMT: TFormEdtReguladorMT
  Left = 800
  Top = 147
  BorderIcons = [biSystemMenu]
  Caption = 'Edi'#231#227'o de Regulador de Tens'#227'o MT'
  ClientHeight = 410
  ClientWidth = 375
  Color = clBtnFace
  Constraints.MinHeight = 346
  Constraints.MinWidth = 328
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 375
    Height = 26
    AutoSize = True
    ButtonWidth = 71
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList1
    List = True
    ShowCaptions = True
    TabOrder = 0
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
  object PageControl: TPageControl
    Left = 0
    Top = 26
    Width = 375
    Height = 384
    ActivePage = psheetReferencia
    Align = alClient
    TabOrder = 1
    object psheetDados: TTabSheet
      Caption = 'Dados B'#225'sicos'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GBoxRegu: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 361
        Height = 132
        Align = alTop
        Caption = 'Dados b'#225'sicos'
        TabOrder = 0
        object Panel1: TPanel
          Left = 2
          Top = 15
          Width = 357
          Height = 58
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Panel4: TPanel
            Left = 0
            Top = 27
            Width = 357
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label1: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 94
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'C'#243'digo'
              Layout = tlCenter
            end
            object EditCodigo: TEdit
              AlignWithMargins = True
              Left = 103
              Top = 3
              Width = 251
              Height = 21
              Align = alClient
              MaxLength = 20
              TabOrder = 0
            end
          end
          object Panel5: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Label8: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 94
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Rede'
              Layout = tlCenter
            end
            object CBoxRede: TComboBox
              AlignWithMargins = True
              Left = 103
              Top = 3
              Width = 251
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
            end
          end
        end
        object PanelPotencia: TPanel
          Left = 2
          Top = 73
          Width = 357
          Height = 57
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object EditValor: TEdit
            AlignWithMargins = True
            Left = 103
            Top = 30
            Width = 251
            Height = 21
            Margins.Bottom = 6
            Align = alClient
            MaxLength = 20
            TabOrder = 1
          end
          object Panel2: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label2: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 94
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Grandeza'
              Layout = tlCenter
              ExplicitTop = 2
            end
            object CBoxTipoPotCorr: TComboBox
              AlignWithMargins = True
              Left = 103
              Top = 3
              Width = 251
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxTipoPotCorrChange
              Items.Strings = (
                'Pot'#234'ncia Nominal (unid. monof'#225'sica) [kVA]'
                'Pot'#234'ncia Passante [kVA]'
                'Corrente Nominal [A]')
            end
          end
          object Panel8: TPanel
            AlignWithMargins = True
            Left = 3
            Top = 30
            Width = 94
            Height = 24
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 2
            object Label5: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 88
              Height = 18
              Align = alLeft
              AutoSize = False
              Caption = 'Valor'
              Layout = tlCenter
              ExplicitLeft = 6
              ExplicitHeight = 15
            end
          end
        end
      end
      object PanelFluxo: TPanel
        Left = 0
        Top = 242
        Width = 367
        Height = 80
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 1
        object GBoxBarEntrada: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 231
          Height = 74
          Align = alClient
          Caption = 'Sentido Direto'
          Color = clBtnFace
          ParentColor = False
          TabOrder = 0
          object PanelLabels: TPanel
            Left = 2
            Top = 15
            Width = 87
            Height = 57
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 0
            object Label3: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 33
              Width = 81
              Height = 21
              Margins.Top = 0
              Align = alClient
              AutoSize = False
              Caption = 'Barra Jusante'
              Layout = tlCenter
              ExplicitTop = 8
              ExplicitWidth = 91
              ExplicitHeight = 50
            end
            object Label10: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 0
              Width = 81
              Height = 30
              Margins.Top = 0
              Align = alTop
              AutoSize = False
              Caption = 'Barra Montante'
              Layout = tlCenter
              ExplicitTop = -3
              ExplicitWidth = 49
            end
          end
          object PanelBarras: TPanel
            Left = 89
            Top = 15
            Width = 140
            Height = 57
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            object CBoxBarEntrada: TComboBox
              AlignWithMargins = True
              Left = 3
              Top = 6
              Width = 134
              Height = 21
              Margins.Top = 6
              Align = alTop
              Style = csDropDownList
              BiDiMode = bdLeftToRight
              ParentBiDiMode = False
              TabOrder = 0
              OnChange = CBoxBarEntradaChange
              OnEnter = CBoxBarEnter
            end
            object CBoxBarSaida: TComboBox
              AlignWithMargins = True
              Left = 3
              Top = 33
              Width = 134
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 1
              OnChange = CBoxBarSaidaChange
              OnEnter = CBoxBarEnter
            end
          end
        end
        object GBoxEstado: TGroupBox
          AlignWithMargins = True
          Left = 240
          Top = 3
          Width = 124
          Height = 74
          Align = alRight
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = 'Estado'
          Color = clBtnFace
          ParentColor = False
          TabOrder = 1
          object Panel7: TPanel
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 114
            Height = 51
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 0
            object checkBypass: TCheckBox
              AlignWithMargins = True
              Left = 3
              Top = 0
              Width = 108
              Height = 48
              Margins.Top = 0
              Align = alClient
              Caption = 'By Pass'
              TabOrder = 0
            end
          end
        end
      end
      object GBoxUnidades: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 193
        Width = 361
        Height = 46
        Align = alTop
        Caption = 'Configura'#231#227'o das unidades monof'#225'sicas'
        Color = clBtnFace
        ParentColor = False
        TabOrder = 2
        object CBoxConfiguracao: TComboBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 351
          Height = 23
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
          OnChange = CBoxConfiguracaoChange
          Items.Strings = (
            '1 unidade:  Fases AB'
            '1 unidade:  Fases BC'
            '1 unidade:  Fases CA'
            '1 unidade:  Fases AN'
            '1 unidade:  Fases BN'
            '1 unidade:  Fases CN'
            '1 unidade:  Fases ABN'
            '1 unidade:  Fases BCN'
            '1 unidade:  Fases CAN'
            '2 unidades: Delta aberto (CA, AB)'
            '2 unidades: Delta aberto (AB, BC)'
            '2 unidades: Delta aberto (BC, CA)'
            '3 unidades: Delta fechado'
            '3 unidades: Estrela')
        end
      end
      object GBoxTipoOperacao: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 141
        Width = 361
        Height = 46
        Align = alTop
        Caption = 'Tipo de Opera'#231#227'o'
        Color = clBtnFace
        ParentColor = False
        TabOrder = 3
        object ComboBoxTipoAjuste: TComboBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 351
          Height = 23
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
          OnChange = ActionSelTipoAjusteExecute
          Items.Strings = (
            'Tap fixo'
            'Autom'#225'tico bidirecional'
            'Autom'#225'tico barra fixa'
            'Autom'#225'tico unidirecional (by-pass no sentido inverso)'
            'Autom'#225'tico tap cont'#237'nuo (NR1/NR3)')
        end
      end
    end
    object psheetAjustes: TTabSheet
      Caption = 'Ajustes'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GBoxTipoAjuste: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 247
        Width = 361
        Height = 98
        Align = alTop
        Caption = 'Ajuste de Tap'
        TabOrder = 0
        object Panel9: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 15
          Width = 351
          Height = 27
          Margins.Top = 0
          Margins.Bottom = 0
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object lbRegMaxima: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 158
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Faixa de varia'#231#227'o de tens'#227'o (%)'
            Layout = tlCenter
          end
          object CBoxFaixaTensao: TComboBox
            Left = 164
            Top = 0
            Width = 187
            Height = 21
            Align = alClient
            Style = csDropDownList
            ItemIndex = 4
            TabOrder = 0
            Text = '-10/10'
            OnChange = CBoxFaixaTensaoChange
            Items.Strings = (
              '-5/5'
              '-6,25/6,25'
              '-7,5/7,5'
              '-8,75/8,75'
              '-10/10')
          end
        end
        object Panel10: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 42
          Width = 351
          Height = 27
          Margins.Top = 0
          Margins.Bottom = 0
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object lbDegrau: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 158
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Faixa de passos do comutador'
            Layout = tlCenter
            ExplicitLeft = 0
          end
          object CBoxFaixaPassos: TComboBox
            Left = 164
            Top = 0
            Width = 187
            Height = 21
            Align = alClient
            Style = csDropDownList
            ItemIndex = 4
            TabOrder = 0
            Text = '-16/16'
            OnChange = CBoxFaixaPassosChange
            Items.Strings = (
              '-8/8'
              '-10/10'
              '-12/12'
              '-14/14'
              '-16/16')
          end
        end
        object Panel11: TPanel
          Left = 2
          Top = 69
          Width = 357
          Height = 26
          Margins.Top = 0
          Margins.Bottom = 0
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label9: TLabel
            AlignWithMargins = True
            Left = 6
            Top = 3
            Width = 155
            Height = 20
            Margins.Left = 6
            Align = alLeft
            AutoSize = False
            Caption = 'Rela'#231#227'o de Tap'
            Layout = tlCenter
          end
          object EditRTP: TEdit
            AlignWithMargins = True
            Left = 167
            Top = 3
            Width = 187
            Height = 20
            Align = alClient
            TabOrder = 0
            OnKeyPress = EditRTPKeyPress
            ExplicitHeight = 21
          end
        end
      end
      object gboxTapFixo: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 155
        Width = 361
        Height = 86
        Align = alTop
        Caption = 'Tap Fixo'
        TabOrder = 1
        object PanelTapFix: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 351
          Height = 60
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object PanelTap: TPanel
            Left = 0
            Top = 0
            Width = 351
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object PanelPasso: TPanel
              Left = 0
              Top = 0
              Width = 161
              Height = 27
              Align = alLeft
              BevelOuter = bvNone
              TabOrder = 0
              object Label6: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 29
                Height = 13
                Align = alLeft
                Caption = 'Passo'
                Layout = tlCenter
              end
              object EditFixoPasso: TEdit
                AlignWithMargins = True
                Left = 38
                Top = 3
                Width = 120
                Height = 21
                Align = alClient
                ReadOnly = True
                TabOrder = 0
              end
            end
            object PanelV: TPanel
              Left = 161
              Top = 0
              Width = 190
              Height = 27
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 1
              object Label7: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 24
                Height = 13
                Align = alLeft
                Caption = 'V (%)'
                Layout = tlCenter
              end
              object EditFixoTensao: TEdit
                AlignWithMargins = True
                Left = 33
                Top = 3
                Width = 154
                Height = 21
                Align = alClient
                ReadOnly = True
                TabOrder = 0
              end
            end
          end
          object TrackBarPassoFixo: TTrackBar
            Left = 0
            Top = 27
            Width = 351
            Height = 38
            Align = alTop
            Max = 16
            Min = -16
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnChange = ActionSelPassoFixoExecute
          end
        end
      end
      object gboxBMdir: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 361
        Height = 70
        Align = alTop
        Caption = 'Banda Morta - Sentido Direto'
        TabOrder = 2
        Visible = False
        object pnlBMDireto: TPanel
          Left = 2
          Top = 15
          Width = 357
          Height = 53
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object lblBandaMorta: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 29
            Width = 130
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Insensibilidade'
            Layout = tlCenter
            ExplicitHeight = 20
          end
          object cboxUnidadeBM: TComboBox
            AlignWithMargins = True
            Left = 316
            Top = 29
            Width = 38
            Height = 21
            Align = alRight
            Style = csDropDownList
            ItemIndex = 0
            TabOrder = 1
            Text = 'V'
            OnChange = cboxUnidadeBMChange
            Items.Strings = (
              'V'
              '%')
          end
          object edtInsDireto: TEdit
            AlignWithMargins = True
            Left = 139
            Top = 29
            Width = 171
            Height = 21
            Align = alClient
            TabOrder = 0
            OnExit = edtInsDiretoExit
            OnKeyPress = edtInsDiretoKeyPress
          end
          object PanelLimitesDir: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 26
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object Label12: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 130
              Height = 20
              Align = alLeft
              AutoSize = False
              Caption = 'Limites'
              Layout = tlCenter
            end
            object EditBMdireto: TEdit
              AlignWithMargins = True
              Left = 139
              Top = 3
              Width = 170
              Height = 20
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
              ExplicitHeight = 21
            end
            object Panel20: TPanel
              Left = 312
              Top = 0
              Width = 45
              Height = 26
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 1
            end
          end
        end
      end
      object gboxBMinv: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 79
        Width = 361
        Height = 70
        Align = alTop
        Caption = 'Banda Morta - Sentido Inverso'
        TabOrder = 3
        object pnlBMInverso: TPanel
          Left = 2
          Top = 15
          Width = 357
          Height = 53
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object lblBandaMortaInv: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 29
            Width = 130
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Insensibilidade'
            Layout = tlCenter
            ExplicitHeight = 20
          end
          object cboxUnidadeBMInv: TComboBox
            AlignWithMargins = True
            Left = 316
            Top = 29
            Width = 38
            Height = 21
            Align = alRight
            Style = csDropDownList
            ItemIndex = 0
            TabOrder = 0
            Text = 'V'
            OnChange = cboxUnidadeBMInvChange
            Items.Strings = (
              'V'
              '%')
          end
          object edtInsInverso: TEdit
            AlignWithMargins = True
            Left = 139
            Top = 29
            Width = 171
            Height = 21
            Align = alClient
            TabOrder = 1
            OnExit = edtInsInversoExit
            OnKeyPress = edtInsInversoKeyPress
          end
          object PanelLimitesInv: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 26
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object Label11: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 130
              Height = 20
              Align = alLeft
              AutoSize = False
              Caption = 'Limites'
              Layout = tlCenter
            end
            object EditBMinverso: TEdit
              AlignWithMargins = True
              Left = 139
              Top = 3
              Width = 170
              Height = 20
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
              ExplicitHeight = 21
            end
            object Panel18: TPanel
              Left = 312
              Top = 0
              Width = 45
              Height = 26
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 1
            end
          end
        end
      end
    end
    object psheetReferencia: TTabSheet
      Caption = 'Refer'#234'ncia'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBoxVrefInv: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 79
        Width = 361
        Height = 76
        Align = alTop
        Caption = 'Tens'#227'o de Refer'#234'ncia - Sentido Inverso'
        TabOrder = 1
        object Panel21: TPanel
          Left = 2
          Top = 15
          Width = 357
          Height = 55
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Panel25: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelBarraDireto: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 100
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object CBoxBarraInverso: TComboBox
              AlignWithMargins = True
              Left = 109
              Top = 3
              Width = 245
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxBarraInversoChange
              OnEnter = CBoxBarEnter
            end
          end
          object Panel26: TPanel
            Left = 0
            Top = 27
            Width = 357
            Height = 28
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Panel3: TPanel
              Left = 267
              Top = 0
              Width = 90
              Height = 28
              Align = alRight
              BevelOuter = bvNone
              TabOrder = 0
              object Label15: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 28
                Height = 13
                Align = alLeft
                Alignment = taCenter
                Caption = 'V (pu)'
                Layout = tlCenter
              end
              object EditVinv: TEdit
                AlignWithMargins = True
                Left = 37
                Top = 3
                Width = 50
                Height = 22
                Align = alClient
                TabOrder = 0
                OnKeyPress = EditVinvKeyPress
                ExplicitHeight = 21
              end
            end
            object PanelNVinv: TPanel
              Left = 0
              Top = 0
              Width = 267
              Height = 28
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 1
              object LabelBarraInverso: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 100
                Height = 22
                Align = alLeft
                AutoSize = False
                Caption = 'N'#237'vel de Tens'#227'o (V)'
                Layout = tlCenter
              end
              object EditNTinv: TEdit
                AlignWithMargins = True
                Left = 109
                Top = 3
                Width = 155
                Height = 22
                Align = alClient
                Enabled = False
                ReadOnly = True
                TabOrder = 0
                ExplicitHeight = 21
              end
            end
          end
        end
      end
      object GroupBoxVrefDir: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 361
        Height = 70
        Align = alTop
        Caption = 'Tens'#227'o de Refer'#234'ncia - Sentido Direto'
        TabOrder = 0
        object Panel22: TPanel
          Left = 2
          Top = 15
          Width = 357
          Height = 26
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Panel23: TPanel
            Left = 0
            Top = 0
            Width = 357
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label13: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 100
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Barra'
              Layout = tlCenter
            end
            object CBoxBarraDireto: TComboBox
              AlignWithMargins = True
              Left = 109
              Top = 3
              Width = 245
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxBarraDiretoChange
              OnEnter = CBoxBarEnter
            end
          end
        end
        object Panel6: TPanel
          Left = 2
          Top = 41
          Width = 357
          Height = 27
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Panel24: TPanel
            Left = 267
            Top = 0
            Width = 90
            Height = 27
            Align = alRight
            BevelOuter = bvNone
            TabOrder = 0
            object Label14: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 28
              Height = 13
              Align = alLeft
              Alignment = taCenter
              Caption = 'V (pu)'
              Layout = tlCenter
            end
            object EditVdir: TEdit
              AlignWithMargins = True
              Left = 37
              Top = 3
              Width = 50
              Height = 21
              Align = alClient
              TabOrder = 0
              OnKeyPress = EditVdirKeyPress
            end
          end
          object PanelNVdir: TPanel
            Left = 0
            Top = 0
            Width = 267
            Height = 27
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            object Label4: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 100
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'N'#237'vel de Tens'#227'o (V)'
              Layout = tlCenter
              ExplicitLeft = 5
              ExplicitTop = 44
            end
            object EditNTdir: TEdit
              AlignWithMargins = True
              Left = 109
              Top = 3
              Width = 155
              Height = 21
              Align = alClient
              Enabled = False
              ReadOnly = True
              TabOrder = 0
            end
          end
        end
      end
      object RadioGroupBarraRef: TRadioGroup
        AlignWithMargins = True
        Left = 3
        Top = 161
        Width = 361
        Height = 41
        Align = alTop
        BiDiMode = bdLeftToRight
        Caption = 'Op'#231#245'es para Barra de Refer'#234'ncia'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          'Utilizar barras do regulador'
          'Selecionar barras de refer'#234'ncia')
        ParentBiDiMode = False
        TabOrder = 2
        OnClick = RadioGroupBarraRefClick
      end
    end
  end
  object ActionList1: TActionList
    Images = ImageList1
    Left = 336
    Top = 8
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
    object ActionSelPassoFixo: TAction
      OnExecute = ActionSelPassoFixoExecute
    end
    object ActionSelTipoAjuste: TAction
      OnExecute = ActionSelTipoAjusteExecute
    end
    object ActionBarrasDefault: TAction
      Caption = 'Barras Padr'#227'o'
      OnExecute = ActionBarrasDefaultExecute
    end
  end
  object ImageList1: TImageList
    Left = 304
    Top = 8
    Bitmap = {
      494C010105000700140110001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
end
