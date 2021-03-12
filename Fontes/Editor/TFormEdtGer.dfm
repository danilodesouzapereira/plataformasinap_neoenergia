object FormEdtGer: TFormEdtGer
  Left = 470
  Top = 340
  BorderIcons = [biSystemMenu]
  Caption = 'Edi'#231#227'o de Gerador'
  ClientHeight = 262
  ClientWidth = 388
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 26
    Width = 388
    Height = 236
    ActivePage = TabSheetGerador
    Align = alClient
    TabOrder = 0
    OnChange = PageControlChange
    OnChanging = PageControlChanging
    object TabSheetGerador: TTabSheet
      Caption = 'Gerador'
      object gboxGeral: TGroupBox
        Left = 0
        Top = 0
        Width = 380
        Height = 208
        Align = alClient
        Caption = 'Dados gerais'
        Constraints.MinHeight = 208
        Constraints.MinWidth = 375
        TabOrder = 0
        object Panel1: TPanel
          Left = 2
          Top = 15
          Width = 185
          Height = 191
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 0
          object Panel3: TPanel
            Left = 0
            Top = 0
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Label1: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'C'#243'digo'
              Layout = tlCenter
            end
            object EditCodigo: TEdit
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel4: TPanel
            Left = 0
            Top = 27
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 1
            object LabelGerVnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Vnom (kV)'
              Layout = tlCenter
            end
            object edtVnom: TEdit
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel5: TPanel
            Left = 0
            Top = 54
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 2
            object LabelGerSnom: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Snom (MVA)'
              Layout = tlCenter
              ExplicitLeft = 6
              ExplicitTop = 64
              ExplicitHeight = 13
            end
            object edtSnom: TEdit
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              TabOrder = 0
            end
          end
          object Panel6: TPanel
            Left = 0
            Top = 81
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 3
            object Label6: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Liga'#231#227'o'
              Layout = tlCenter
            end
            object cboxLigacao: TComboBox
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = cboxLigacaoChange
              Items.Strings = (
                'Estrela aterrado'
                'Delta'
                'Estrela isolado'
                'Monof'#225'sico')
            end
          end
          object Panel7: TPanel
            Left = 0
            Top = 108
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 4
            object Label2: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Suprimento'
              Layout = tlCenter
            end
            object cboxTipoSup: TComboBox
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxTipoSupChange
              Items.Strings = (
                'P/V'
                'P/Q'
                'V/teta')
            end
          end
          object Panel8: TPanel
            Left = 0
            Top = 135
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 5
            object Label3: TLabel
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 59
              Height = 21
              Align = alLeft
              AutoSize = False
              Caption = 'Fonte'
              Layout = tlCenter
            end
            object CBoxFonte: TComboBox
              AlignWithMargins = True
              Left = 68
              Top = 3
              Width = 114
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxFonteChange
              Items.Strings = (
                'Diesel'
                'PCH'
                'Vapor'
                'E'#243'lica'
                'Fotovoltaica')
            end
          end
          object Panel9: TPanel
            Left = 0
            Top = 162
            Width = 185
            Height = 27
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 6
            object CheckBoxFatPotLim: TCheckBox
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 115
              Height = 21
              Align = alLeft
              Caption = 'Limitar Fat.Pot'#234'ncia'
              TabOrder = 0
              OnClick = CheckBoxFatPotLimClick
            end
            object EditFatPotLim: TEdit
              AlignWithMargins = True
              Left = 124
              Top = 3
              Width = 58
              Height = 21
              Align = alClient
              TabOrder = 1
            end
          end
        end
        object Panel2: TPanel
          Left = 187
          Top = 15
          Width = 191
          Height = 191
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 1
          object GBoxCurvaTipica: TGroupBox
            AlignWithMargins = True
            Left = 3
            Top = 57
            Width = 185
            Height = 48
            Align = alTop
            Caption = 'Curva t'#237'pica de gera'#231#227'o '
            TabOrder = 1
            Visible = False
            object CBoxCurvaTipica: TComboBox
              AlignWithMargins = True
              Left = 5
              Top = 18
              Width = 175
              Height = 21
              Align = alClient
              Style = csDropDownList
              TabOrder = 0
              OnChange = CBoxCurvaTipicaChange
            end
          end
          object RadioGroupTipoCurva: TRadioGroup
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 185
            Height = 48
            Align = alTop
            Caption = 'Curva de gera'#231#227'o'
            Columns = 2
            Items.Strings = (
              'Curva t'#237'pica'
              'Curva pr'#243'pria')
            TabOrder = 0
            Visible = False
            OnClick = RadioGroupTipoCurvaClick
          end
          object PanelFases: TPanel
            Left = 0
            Top = 108
            Width = 191
            Height = 83
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 2
            object GBoxFases: TGroupBox
              AlignWithMargins = True
              Left = 3
              Top = 3
              Width = 185
              Height = 50
              Align = alTop
              Caption = 'Fases'
              TabOrder = 0
              Visible = False
              object CBoxFases: TComboBox
                AlignWithMargins = True
                Left = 5
                Top = 18
                Width = 175
                Height = 21
                Align = alClient
                Style = csDropDownList
                TabOrder = 0
                Visible = False
                OnChange = CBoxFasesChange
                Items.Strings = (
                  'ANt'
                  'ABNt')
              end
            end
          end
        end
      end
    end
    object tsImpedancia: TTabSheet
      Caption = 'Imped'#226'ncia'
      ImageIndex = 3
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 380
        Height = 208
        Align = alClient
        Caption = 'Imped'#226'ncia (Sb = Snom)'
        TabOrder = 0
        object sgZeq: TStringGrid
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 370
          Height = 185
          Align = alClient
          ColCount = 3
          DefaultColWidth = 60
          DefaultRowHeight = 16
          RowCount = 3
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs, goAlwaysShowEditor]
          ScrollBars = ssNone
          TabOrder = 0
          ColWidths = (
            60
            53
            50)
          RowHeights = (
            16
            16
            16)
        end
      end
    end
    object TabSheetCurva: TTabSheet
      Caption = 'Curva'
      ImageIndex = 2
    end
    object TabSheetGrafico: TTabSheet
      Caption = 'Gr'#225'fico'
      ImageIndex = 4
    end
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 388
    Height = 26
    AutoSize = True
    ButtonWidth = 93
    Caption = 'ToolBar1'
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList1
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 1
    object butConfirma: TToolButton
      Left = 0
      Top = 0
      Hint = 'Confirmar edi'#231#227'o do gerador'
      AutoSize = True
      Caption = 'Confirmar'
      ImageIndex = 0
      OnClick = ActionConfirmaExecute
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
      Action = ActionImporta
    end
  end
  object ImageList1: TImageList
    Left = 304
    Top = 24
    Bitmap = {
      494C0101050020007C0010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      00000000000000000000000000000000000000000000000000000000FF000000
      FF0000000000000000007F7F7F00000000007F7F7F00000000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF007F7F7F00000000007F7F7F0000000000000000000000
      00000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000007F7F7F00000000007F7F7F0000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
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
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000080808000FFFFFF0000000000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00808080000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000080808000FFFFFF00808080008080
      8000000000008080800080808000000000008080800080808000000000008080
      8000808080008080800000000000808080000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000080808000FFFFFF0000000000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00808080000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000080808000FFFFFF00808080008080
      8000000000008080800080808000000000008080800080808000000000008080
      8000808080008080800000000000808080000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000080808000FFFFFF0000000000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00808080000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000080808000FFFFFF00808080008080
      8000000000008080800080808000000000008080800080808000000000008080
      8000808080008080800000000000808080000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      00000084000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000080808000FFFFFF0000000000FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF000000
      0000FFFFFF00FFFFFF00FFFFFF00808080000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      00000084000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000080808000FFFFFF00808080008080
      8000000000008080800080808000000000008080800080808000000000008080
      8000808080008080800000000000808080000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      00000084000000840000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000080808000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00808080000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000084000000840000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000084000000840000000000000000000000000000C0C0C000C0C0C000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000C0C0C000C0C0C0000000000080808000FFFFFF00FFFFFF008080
      8000808080008080800080808000808080008080800080808000808080008080
      800080808000FFFFFF00FFFFFF00808080000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008080800080808000808080008080
      8000808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080000000000000000000000000000000
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
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF00FFFF000000000000F83F000000000000
      E00F000000000000CC470000000000008463000000000000A073000000000000
      31F900000000000038F90000000000003C790000000000003C39000000000000
      3C190000000000009C0B0000000000008C43000000000000C467000000000000
      E00F000000000000F83F000000000000FFFFFFFF8000FFFFFFFF00000000FFFF
      FE7F00002490FFFFFC3F00000922E7F3F81F00002490E3E3F19F00000922F1C7
      E38F00002490F88FE7CF00000922FC1FFFC700002490FE3FFFE700000922FC1F
      FFE300000000F88FFFF300000000F1C7FFF300000000E3E3FFFF00000000E7F3
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionList1: TActionList
    Images = ImageList1
    Left = 360
    Top = 24
    object ActionCancela: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar edi'#231#227'o do gerador'
      ImageIndex = 3
      OnExecute = ActionCancelaExecute
    end
    object ActionConfirma: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar edi'#231#227'o do gerador'
      ImageIndex = 0
      OnExecute = ActionConfirmaExecute
    end
    object ActionRetira: TAction
      Caption = 'Retirar'
      Hint = 'Retirar gerador da rede'
      ImageIndex = 4
      OnExecute = ActionRetiraExecute
    end
    object ActionImporta: TAction
      Caption = 'Importa Curva'
      ImageIndex = 1
      OnExecute = ActionImportaExecute
    end
  end
  object OpenDialog: TOpenDialog
    Left = 252
    Top = 24
  end
end