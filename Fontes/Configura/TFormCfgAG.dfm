object formCfgAG: TformCfgAG
  Left = 344
  Top = 154
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Configura'#231#227'o - Algoritmo Gen'#233'tico'
  ClientHeight = 451
  ClientWidth = 432
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 432
    Height = 25
    ButtonWidth = 71
    Caption = 'ToolBar1'
    EdgeBorders = [ebBottom]
    Flat = True
    Images = ImageList
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    object butDefault: TToolButton
      Left = 0
      Top = 0
      Action = ActionDefault
      Caption = 'Padr'#227'o'
      ImageIndex = 2
    end
    object butInclui: TToolButton
      Left = 71
      Top = 0
      Action = ActionConfirma
      AutoSize = True
    end
    object butSair: TToolButton
      Left = 146
      Top = 0
      Hint = 'Sair'
      Action = ActionCancela
      AutoSize = True
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 25
    Width = 432
    Height = 426
    Align = alClient
    TabOrder = 1
    object PageControl1: TPageControl
      Left = 0
      Top = 8
      Width = 433
      Height = 417
      ActivePage = TabSheet1
      TabIndex = 0
      TabOrder = 0
      object TabSheet1: TTabSheet
        Caption = 'B'#225'sico'
        object gboxGeral: TGroupBox
          Left = 8
          Top = 10
          Width = 193
          Height = 137
          Caption = 'Dados Gerais do AG'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label1: TLabel
            Left = 8
            Top = 24
            Width = 99
            Height = 13
            Caption = 'N'#250'mero de gera'#231#245'es'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label2: TLabel
            Left = 8
            Top = 48
            Width = 104
            Height = 13
            Caption = 'N'#250'mero de indiv'#237'duos'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label3: TLabel
            Left = 8
            Top = 72
            Width = 124
            Height = 13
            Caption = 'Probabilidade de Muta'#231#227'o'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label4: TLabel
            Left = 8
            Top = 96
            Width = 96
            Height = 13
            Caption = 'Prob de Cruzamento'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtnger: TEdit
            Left = 136
            Top = 16
            Width = 49
            Height = 21
            TabOrder = 0
            OnKeyPress = edtngerKeyPress
          end
          object edtnindiv: TEdit
            Left = 136
            Top = 40
            Width = 49
            Height = 21
            TabOrder = 1
            OnKeyPress = edtngerKeyPress
          end
          object edtprobMut: TEdit
            Left = 136
            Top = 64
            Width = 49
            Height = 21
            TabOrder = 2
            OnKeyPress = edtngerKeyPress
          end
          object edtProbCruz: TEdit
            Left = 136
            Top = 88
            Width = 49
            Height = 21
            TabOrder = 3
            OnKeyPress = edtngerKeyPress
          end
        end
        object gboxAvalia: TGroupBox
          Left = 216
          Top = 10
          Width = 201
          Height = 135
          Caption = 'Fun'#231#227'o objetivo'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label13: TLabel
            Left = 8
            Top = 24
            Width = 133
            Height = 13
            Caption = 'K Custo Total (Inv + Perdas)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label14: TLabel
            Left = 8
            Top = 72
            Width = 76
            Height = 13
            Caption = 'K Carregamento'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label15: TLabel
            Left = 8
            Top = 96
            Width = 46
            Height = 13
            Caption = 'K Tens'#227'o'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label9: TLabel
            Left = 8
            Top = 48
            Width = 70
            Height = 13
            Caption = 'K Investimento'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtkCusto: TEdit
            Left = 152
            Top = 16
            Width = 41
            Height = 21
            TabOrder = 0
            OnKeyPress = edtngerKeyPress
          end
          object edtkCar: TEdit
            Left = 152
            Top = 64
            Width = 41
            Height = 21
            TabOrder = 1
            OnKeyPress = edtngerKeyPress
          end
          object edtkDV: TEdit
            Left = 152
            Top = 88
            Width = 41
            Height = 21
            TabOrder = 2
            OnKeyPress = edtngerKeyPress
          end
          object edtkInvest: TEdit
            Left = 152
            Top = 40
            Width = 41
            Height = 21
            TabOrder = 3
            OnKeyPress = edtngerKeyPress
          end
        end
        object gboxGrafico: TGroupBox
          Left = 8
          Top = 156
          Width = 409
          Height = 137
          Caption = 'Op'#231#245'es para visualiza'#231#227'o gr'#225'fica da evolu'#231#227'o por quadrante'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object gboxPatamar: TGroupBox
            Left = 208
            Top = 88
            Width = 193
            Height = 41
            Caption = 'Patamar para o fluxo'
            TabOrder = 0
            object cboxPatamar: TComboBox
              Left = 8
              Top = 15
              Width = 177
              Height = 21
              ItemHeight = 13
              TabOrder = 0
            end
          end
          object rgroupTipoGraf: TRadioGroup
            Left = 208
            Top = 16
            Width = 193
            Height = 65
            Caption = 'Tipo de gr'#225'fico nesta janela'
            Items.Strings = (
              'N'#227'o exibir'
              'Topologia'
              'Fluxo de Pot'#234'ncia')
            TabOrder = 1
            OnClick = rgroupTipoGrafClick
          end
          object rgroupExibeIndiv: TRadioGroup
            Left = 8
            Top = 64
            Width = 193
            Height = 65
            Caption = 'Tipo de exibi'#231#227'o para gera'#231#227'o'
            Items.Strings = (
              'Exibir todos os indiv'#237'duos'
              'Exibir somente o melhor')
            TabOrder = 2
          end
          object cboxVisu: TComboBox
            Left = 8
            Top = 24
            Width = 193
            Height = 21
            ItemHeight = 13
            TabOrder = 3
            OnChange = cboxVisuChange
          end
        end
      end
      object TabSheet2: TTabSheet
        Caption = 'Avan'#231'ado'
        ImageIndex = 1
        object Label12: TLabel
          Left = 136
          Top = 336
          Width = 28
          Height = 13
          Caption = 'Gama'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object rgroupTipoFObj: TRadioGroup
          Left = 0
          Top = 288
          Width = 129
          Height = 97
          Caption = 'M'#250'ltiplos Objetivos'
          Items.Strings = (
            'Utilizar produto'
            'Utilizar e-fuzzy')
          TabOrder = 0
          OnClick = rgroupTipoFObjClick
        end
        object gboxCruzamento: TGroupBox
          Left = 0
          Top = 2
          Width = 185
          Height = 65
          Caption = 'Cruzamento Uniforme (Bit a Bit)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label5: TLabel
            Left = 8
            Top = 40
            Width = 106
            Height = 13
            Caption = 'Probabilidade de troca'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtProbCruzUnif: TEdit
            Left = 128
            Top = 33
            Width = 49
            Height = 21
            TabOrder = 0
            OnKeyPress = edtngerKeyPress
          end
          object ckboxCruzUnif: TCheckBox
            Left = 8
            Top = 16
            Width = 161
            Height = 17
            Caption = 'Utilizar cruzamento uniforme'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = ckboxCruzUnifClick
          end
        end
        object edtGama: TEdit
          Left = 168
          Top = 336
          Width = 41
          Height = 21
          TabOrder = 2
          OnKeyPress = edtngerKeyPress
        end
        object gboxSubPop: TGroupBox
          Left = 192
          Top = 2
          Width = 225
          Height = 65
          Caption = 'Dados da sub-popula'#231#227'o'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label6: TLabel
            Left = 8
            Top = 24
            Width = 153
            Height = 13
            Caption = 'Tamanho da sub-popula'#231#227'o (%):'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtnindivSub: TEdit
            Left = 168
            Top = 24
            Width = 49
            Height = 21
            TabOrder = 0
            OnKeyPress = edtngerKeyPress
          end
        end
        object gboxPondera: TGroupBox
          Left = 0
          Top = 72
          Width = 257
          Height = 73
          Caption = 'Pondera'#231#227'o para a composi'#231#227'o do fator K Tens'#227'o'
          TabOrder = 4
          object Label16: TLabel
            Left = 24
            Top = 32
            Width = 49
            Height = 13
            Caption = 'Diag Ruim'
          end
          object Label17: TLabel
            Left = 152
            Top = 32
            Width = 62
            Height = 13
            Caption = 'Diag Regular'
          end
          object lbTrack: TLabel
            Left = 106
            Top = 56
            Width = 20
            Height = 13
            Caption = '20%'
          end
          object TrackBarTensao: TTrackBar
            Left = 78
            Top = 24
            Width = 69
            Height = 33
            Max = 100
            Orientation = trHorizontal
            Frequency = 5
            Position = 0
            SelEnd = 0
            SelStart = 0
            TabOrder = 0
            TickMarks = tmBottomRight
            TickStyle = tsAuto
            OnChange = TrackBarTensaoChange
          end
        end
        object rgroupSelecao: TRadioGroup
          Left = 264
          Top = 72
          Width = 153
          Height = 73
          Caption = 'Sele'#231#227'o'
          Ctl3D = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Items.Strings = (
            'Proporcional (roleta)'
            'Torneio'
            'Ranking')
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 5
          OnClick = rgroupSelecaoClick
        end
        object gboxInterface: TGroupBox
          Left = 192
          Top = 152
          Width = 225
          Height = 65
          Caption = 'A'#231#227'o durante o processo evolutivo'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          object Label7: TLabel
            Left = 32
            Top = 24
            Width = 181
            Height = 26
            Caption = 'Permitir recondutoramento autom'#225'tico de trechos sobrecarregados'
            WordWrap = True
          end
          object ckboxRecondut: TCheckBox
            Left = 8
            Top = 24
            Width = 25
            Height = 25
            TabOrder = 0
          end
        end
        object gboxRanking: TGroupBox
          Left = 0
          Top = 152
          Width = 185
          Height = 65
          Caption = 'Dados do Ranking'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          object Label8: TLabel
            Left = 8
            Top = 27
            Width = 112
            Height = 13
            Caption = 'Indice de Uniformidade:'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edtIndUnif: TEdit
            Left = 128
            Top = 24
            Width = 49
            Height = 21
            TabOrder = 0
            OnKeyPress = edtngerKeyPress
          end
        end
        object gboxCritParada: TGroupBox
          Left = 0
          Top = 224
          Width = 417
          Height = 57
          Caption = 'Crit'#233'rio de Parada'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          object Label10: TLabel
            Left = 192
            Top = 16
            Width = 152
            Height = 26
            Alignment = taCenter
            BiDiMode = bdLeftToRight
            Caption = 'No. de gera'#231#245'es m'#225'ximo aceito sem melhores solu'#231#245'es:'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentBiDiMode = False
            ParentFont = False
            WordWrap = True
          end
          object ckboxCriterioParada: TCheckBox
            Left = 24
            Top = 16
            Width = 137
            Height = 33
            Caption = 'Utilizar crit'#233'rio de parada'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = ckboxCriterioParadaClick
          end
          object edtngerParada: TEdit
            Left = 352
            Top = 16
            Width = 49
            Height = 21
            TabOrder = 1
            OnKeyPress = edtngerKeyPress
          end
        end
      end
    end
  end
  object ActionList1: TActionList
    Images = ImageList
    Left = 248
    Top = 8
    object ActionCancela: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar sele'#231#227'o de SEs'
      ImageIndex = 1
      OnExecute = ActionCancelaExecute
    end
    object ActionConfirma: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar sele'#231#227'o de SEs'
      ImageIndex = 0
      OnExecute = ActionConfirmaExecute
    end
    object ActionDefault: TAction
      Caption = #39'Default'#39
      Hint = 'Redefine valores padr'#227'o'
      OnExecute = ActionDefaultExecute
    end
  end
  object ImageList: TImageList
    Left = 280
    Top = 8
    Bitmap = {
      494C010103000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000031CE000031CE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      00000031CE000031CE000031CE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000031
      CE000031CE000031CE0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000000000000031CE000031
      CE000031CE0000000000000000000031CE000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE000000000000000000000000000031CE000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE0000000000000000000000000000000000000000000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE00000000000000000000000000000000000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000000000000031CE000031
      CE000000000000000000000000000031CE000031CE000031CE00000000000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000031
      CE000031CE000031CE000031CE000031CE000031CE0000000000000000000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      00000031CE000031CE000031CE000031CE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFFFFF0000FFFFFFFFFFFF0000
      FE7FFFFFFFFF0000FC3FE7F3F9FF0000F81FE3E3F1FF0000F19FF1C7E3FF0000
      E38FF88FC6070000E7CFFC1FCE070000FFC7FE3FCF870000FFE7FC1FCF070000
      FFE3F88FCE270000FFF3F1C7E0670000FFF3E3E3F0FF0000FFFFE7F3FFFF0000
      FFFFFFFFFFFF0000FFFFFFFFFFFF000000000000000000000000000000000000
      000000000000}
  end
end
