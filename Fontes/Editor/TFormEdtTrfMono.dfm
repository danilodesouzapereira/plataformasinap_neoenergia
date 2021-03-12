object FormEdtTrfMono: TFormEdtTrfMono
  Left = 267
  Top = 398
  Caption = 'Editor: Transformador Monof'#225'sico'
  ClientHeight = 268
  ClientWidth = 384
  Color = clBtnFace
  Constraints.MinWidth = 377
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
    Width = 384
    Height = 242
    ActivePage = TabSheetTrafo
    Align = alClient
    Constraints.MinHeight = 131
    TabOrder = 0
    object TabSheetTrafo: TTabSheet
      Caption = 'Transformador'
      object gboxCodigo: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 370
        Height = 104
        Align = alTop
        Caption = 'Identifica'#231#227'o'
        TabOrder = 0
        object Panel10: TPanel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 201
          Height = 81
          Align = alClient
          AutoSize = True
          BevelOuter = bvNone
          TabOrder = 0
          object Panel9: TPanel
            Left = 0
            Top = 54
            Width = 201
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label6: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Tipo'
              Layout = tlCenter
            end
            object CBoxModelo: TComboBox
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 156
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxModeloChange
              Items.Strings = (
                'Monof'#225'sico (4 terminais)'
                'Monof'#225'sico (5 terminais)')
            end
          end
          object Panel14: TPanel
            Left = 0
            Top = 0
            Width = 201
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object Label1: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'C'#243'digo'
              Layout = tlCenter
              ExplicitLeft = 6
              ExplicitTop = 14
              ExplicitHeight = 13
            end
            object EditCodigo: TEdit
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 156
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel15: TPanel
            Left = 0
            Top = 27
            Width = 201
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object Label4: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 33
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Rede'
              Layout = tlCenter
            end
            object CBoxRede: TComboBox
              AlignWithMargins = True
              Left = 42
              Top = 3
              Width = 156
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
        end
        object Panel18: TPanel
          Left = 209
          Top = 15
          Width = 159
          Height = 87
          Align = alRight
          BevelOuter = bvNone
          TabOrder = 1
          object GroupBox3: TGroupBox
            AlignWithMargins = True
            Left = 3
            Top = 0
            Width = 153
            Height = 65
            Margins.Top = 0
            Align = alTop
            Caption = 'Fases'
            TabOrder = 0
            object Panel16: TPanel
              Left = 2
              Top = 9
              Width = 149
              Height = 27
              Align = alBottom
              BevelOuter = bvNone
              TabOrder = 0
              object Label8: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 54
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Prim'#225'rio'
                Layout = tlCenter
              end
              object CBoxFasePri: TComboBox
                AlignWithMargins = True
                Left = 63
                Top = 3
                Width = 83
                Height = 21
                Align = alClient
                Style = csDropDownList
                TabOrder = 0
                OnChange = CBoxFasePriChange
                Items.Strings = (
                  'A'
                  'B'
                  'C'
                  'AN'
                  'BN'
                  'CN'
                  'AB'
                  'BC'
                  'CA')
              end
            end
            object Panel17: TPanel
              Left = 2
              Top = 36
              Width = 149
              Height = 27
              Align = alBottom
              BevelOuter = bvNone
              TabOrder = 1
              object Label11: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 54
                Height = 21
                Align = alLeft
                AutoSize = False
                Caption = 'Secund'#225'rio'
                Layout = tlCenter
                ExplicitLeft = 8
                ExplicitTop = 14
                ExplicitHeight = 13
              end
              object CBoxFaseSec: TComboBox
                AlignWithMargins = True
                Left = 63
                Top = 3
                Width = 83
                Height = 21
                Align = alClient
                Style = csDropDownList
                TabOrder = 0
                OnChange = CBoxFaseSecChange
                Items.Strings = (
                  'A'
                  'B'
                  'C'
                  'AN'
                  'BN'
                  'CN'
                  'AB'
                  'BC'
                  'CA')
              end
            end
          end
        end
      end
      object gboxParmEletric: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 113
        Width = 370
        Height = 98
        Align = alClient
        BiDiMode = bdLeftToRight
        Caption = 'Par'#226'metros el'#233'tricos'
        Constraints.MinHeight = 66
        ParentBiDiMode = False
        TabOrder = 1
        object Panel20: TPanel
          Left = 209
          Top = 15
          Width = 159
          Height = 81
          Align = alRight
          BevelOuter = bvNone
          TabOrder = 0
          object GroupBox1: TGroupBox
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 153
            Height = 75
            Align = alClient
            Caption = 'Zcurto (Sbase=Snom)'
            TabOrder = 0
            object Panel25: TPanel
              Left = 2
              Top = 15
              Width = 149
              Height = 27
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 0
              object Label9: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 24
                Height = 21
                Align = alLeft
                Caption = 'r (pu)'
                Layout = tlCenter
                ExplicitHeight = 13
              end
              object EditR: TEdit
                AlignWithMargins = True
                Left = 33
                Top = 3
                Width = 113
                Height = 21
                Align = alClient
                TabOrder = 0
              end
            end
            object Panel24: TPanel
              Left = 2
              Top = 42
              Width = 149
              Height = 27
              Align = alTop
              BevelOuter = bvNone
              TabOrder = 1
              object Label14: TLabel
                AlignWithMargins = True
                Left = 3
                Top = 3
                Width = 26
                Height = 21
                Align = alLeft
                Caption = 'x (pu)'
                Layout = tlCenter
                ExplicitHeight = 13
              end
              object EditX: TEdit
                AlignWithMargins = True
                Left = 35
                Top = 3
                Width = 111
                Height = 21
                Align = alClient
                TabOrder = 0
              end
            end
          end
        end
        object Panel19: TPanel
          Left = 2
          Top = 15
          Width = 207
          Height = 81
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object Panel21: TPanel
            Left = 0
            Top = 0
            Width = 207
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object LabelSnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 66
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Snom(kVA)'
              Layout = tlCenter
            end
            object EditSnom: TEdit
              AlignWithMargins = True
              Left = 75
              Top = 3
              Width = 129
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel22: TPanel
            Left = 0
            Top = 27
            Width = 207
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
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
              ExplicitLeft = 6
              ExplicitTop = 14
              ExplicitHeight = 13
            end
            object EditPerdaFe: TEdit
              AlignWithMargins = True
              Left = 75
              Top = 3
              Width = 129
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel23: TPanel
            Left = 0
            Top = 54
            Width = 207
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object CheckAutoTrafo: TCheckBox
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 84
              Height = 21
              Align = alLeft
              Alignment = taLeftJustify
              BiDiMode = bdLeftToRight
              Caption = 'Auto-trafo'
              ParentBiDiMode = False
              TabOrder = 0
            end
          end
        end
      end
    end
    object TabSheetPriSec: TTabSheet
      Caption = 'Prim'#225'rio/Secund'#225'rio'
      ImageIndex = 1
      object gboxPrimario: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 182
        Height = 208
        Align = alLeft
        Caption = 'Prim'#225'rio'
        Constraints.MinHeight = 176
        TabOrder = 0
        object GBoxZatPri: TGroupBox
          AlignWithMargins = True
          Left = 5
          Top = 126
          Width = 172
          Height = 44
          Align = alTop
          Caption = 'Aterramento (ohm)'
          TabOrder = 0
          object LabelSecRterra: TLabel
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 8
            Height = 21
            Align = alLeft
            Caption = 'R'
            Layout = tlCenter
            ExplicitHeight = 13
          end
          object LabelSecXterra: TLabel
            AlignWithMargins = True
            Left = 85
            Top = 18
            Width = 7
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'X'
            Layout = tlCenter
            ExplicitHeight = 13
          end
          object EditRatPri: TEdit
            AlignWithMargins = True
            Left = 19
            Top = 18
            Width = 60
            Height = 21
            Align = alLeft
            TabOrder = 0
          end
          object EditXatPri: TEdit
            AlignWithMargins = True
            Left = 98
            Top = 18
            Width = 60
            Height = 21
            Align = alLeft
            TabOrder = 1
          end
        end
        object Panel2: TPanel
          Left = 2
          Top = 15
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label7: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Barra'
            Layout = tlCenter
          end
          object CBoxBarPri: TComboBox
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 120
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
            OnChange = CBoxBarPriChange
          end
        end
        object Panel3: TPanel
          Left = 2
          Top = 69
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object LabelTapPri: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Tap(kV)'
            Layout = tlCenter
          end
          object EditTapPri: TEdit
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 102
            Height = 21
            Align = alClient
            TabOrder = 0
          end
          object Panel8: TPanel
            Left = 160
            Top = 0
            Width = 18
            Height = 27
            Align = alRight
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 1
            object UpDownPri: TUpDown
              Left = 0
              Top = 3
              Width = 18
              Height = 21
              Min = -5
              TabOrder = 0
              OnClick = UpDownPriClick
            end
          end
        end
        object Panel4: TPanel
          Left = 2
          Top = 42
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object LabelPriVnom: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Vnom(kV)'
            Layout = tlCenter
            ExplicitLeft = 11
            ExplicitTop = 14
            ExplicitHeight = 13
          end
          object EditVnomPri: TEdit
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 120
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
        object Panel5: TPanel
          Left = 2
          Top = 96
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 4
          object LabelNumTap1: TLabel
            AlignWithMargins = True
            Left = 36
            Top = 3
            Width = 139
            Height = 21
            Align = alClient
            Caption = '0)'
            Layout = tlCenter
            ExplicitWidth = 9
            ExplicitHeight = 13
          end
          object LabelTap1: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 27
            Height = 21
            Align = alLeft
            Alignment = taRightJustify
            Caption = '(tap ='
            Layout = tlCenter
            ExplicitHeight = 13
          end
        end
      end
      object GroupBox2: TGroupBox
        AlignWithMargins = True
        Left = 191
        Top = 3
        Width = 182
        Height = 208
        Align = alClient
        Caption = 'Secund'#225'rio'
        Constraints.MinWidth = 182
        TabOrder = 1
        object GBoxZatSec: TGroupBox
          AlignWithMargins = True
          Left = 5
          Top = 126
          Width = 172
          Height = 44
          Align = alTop
          Caption = 'Aterramento (ohm)'
          Constraints.MinWidth = 149
          TabOrder = 0
          object Label2: TLabel
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 8
            Height = 21
            Align = alLeft
            Caption = 'R'
            Layout = tlCenter
            ExplicitHeight = 13
          end
          object Label3: TLabel
            AlignWithMargins = True
            Left = 85
            Top = 18
            Width = 7
            Height = 21
            Align = alLeft
            Caption = 'X'
            Layout = tlCenter
            ExplicitHeight = 13
          end
          object EditRatSec: TEdit
            AlignWithMargins = True
            Left = 19
            Top = 18
            Width = 60
            Height = 21
            Align = alLeft
            TabOrder = 0
          end
          object EditXatSec: TEdit
            AlignWithMargins = True
            Left = 98
            Top = 18
            Width = 60
            Height = 21
            Align = alLeft
            TabOrder = 1
          end
        end
        object Panel6: TPanel
          Left = 2
          Top = 96
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object LabelNumTap2: TLabel
            Left = 27
            Top = 0
            Width = 151
            Height = 27
            Align = alClient
            AutoSize = False
            Caption = '0)'
            Layout = tlCenter
            ExplicitLeft = 80
            ExplicitWidth = 94
          end
          object LabelTap2: TLabel
            Left = 0
            Top = 0
            Width = 27
            Height = 27
            Align = alLeft
            Alignment = taRightJustify
            Caption = '(tap ='
            Layout = tlCenter
            ExplicitHeight = 13
          end
        end
        object Panel11: TPanel
          Left = 2
          Top = 15
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object Label10: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Barra'
            Layout = tlCenter
          end
          object CBoxBarSec: TComboBox
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 120
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
            OnChange = CBoxBarSecChange
          end
        end
        object Panel12: TPanel
          Left = 2
          Top = 69
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object LabelTapSec: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Tap(kV)'
            Layout = tlCenter
          end
          object EditTapSec: TEdit
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 103
            Height = 21
            Align = alClient
            TabOrder = 0
          end
          object Panel7: TPanel
            Left = 161
            Top = 0
            Width = 17
            Height = 27
            Align = alRight
            AutoSize = True
            BevelOuter = bvNone
            TabOrder = 1
            object UpDownSec: TUpDown
              Left = 0
              Top = 3
              Width = 17
              Height = 21
              Min = -5
              TabOrder = 0
              OnClick = UpDownSecClick
            end
          end
        end
        object Panel13: TPanel
          Left = 2
          Top = 42
          Width = 178
          Height = 27
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 4
          object LabelSecVnom: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 46
            Height = 21
            Align = alLeft
            AutoSize = False
            Caption = 'Vnom(kV)'
            Layout = tlCenter
            ExplicitLeft = 11
            ExplicitTop = 39
            ExplicitHeight = 13
          end
          object EditVnomSec: TEdit
            AlignWithMargins = True
            Left = 55
            Top = 3
            Width = 120
            Height = 21
            Align = alClient
            TabOrder = 0
          end
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Ajuste Autom'#225'tico'
      ImageIndex = 3
      object gboxTap: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 370
        Height = 47
        Align = alTop
        Caption = 'Ajuste autom'#225'tico (LTC)'
        Enabled = False
        TabOrder = 0
        object CBoxLtcTipo: TComboBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 360
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
      object Panel1: TPanel
        Left = 0
        Top = 53
        Width = 376
        Height = 53
        Align = alTop
        AutoSize = True
        BevelOuter = bvNone
        TabOrder = 1
        object GBoxLtcBar: TGroupBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 216
          Height = 47
          Align = alClient
          Caption = 'Barra de tens'#227'o controlada'
          Constraints.MinWidth = 143
          TabOrder = 0
          object CBoxLtcBar: TComboBox
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 206
            Height = 21
            Align = alClient
            Style = csDropDownList
            TabOrder = 0
          end
        end
        object GBoxLtcVpu: TGroupBox
          AlignWithMargins = True
          Left = 225
          Top = 3
          Width = 148
          Height = 47
          Align = alRight
          Caption = 'Tens'#227'o controlada (pu)'
          TabOrder = 1
          object EditLtcVpu: TEdit
            AlignWithMargins = True
            Left = 5
            Top = 18
            Width = 138
            Height = 24
            Align = alClient
            TabOrder = 0
            ExplicitHeight = 21
          end
        end
      end
    end
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 384
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
    object ToolButton1: TToolButton
      Left = 210
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object ButImagem: TToolButton
      Left = 218
      Top = 0
      Action = ActionSelFoto
      AutoSize = True
    end
  end
  object ImageList1: TImageList
    Left = 328
    Top = 8
    Bitmap = {
      494C0101050007004C0010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
  object ActionList1: TActionList
    Images = ImageList1
    Left = 384
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
    object ActionSelFoto: TAction
      Caption = 'Imagens'
      ImageIndex = 4
      OnExecute = ActionSelFotoExecute
    end
  end
end
