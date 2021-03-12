object FormCelPlanner: TFormCelPlanner
  Left = 0
  Top = 0
  Caption = 'CelPlanner Exporter'
  ClientHeight = 287
  ClientWidth = 656
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 656
    Height = 22
    AutoSize = True
    ButtonWidth = 86
    Caption = 'ToolBar1'
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionGeraDados
      AutoSize = True
      ImageIndex = 1
    end
    object ToolButton3: TToolButton
      Left = 90
      Top = 0
      Action = ActionSalvaArquivo
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 177
      Top = 0
      Width = 10
      Caption = 'ToolButton2'
      ImageIndex = 1
      Style = tbsSeparator
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 22
    Width = 349
    Height = 265
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitHeight = 263
    object Panel2: TPanel
      Left = 0
      Top = 94
      Width = 161
      Height = 171
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitHeight = 169
      object gbSubestacao: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 45
        Width = 155
        Height = 36
        Align = alTop
        Caption = 'Substations'
        TabOrder = 0
        object checkSubestacao: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 145
          Height = 13
          Align = alTop
          Caption = 'Export Substations'
          TabOrder = 0
        end
      end
      object gbGeracao: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 87
        Width = 155
        Height = 36
        Align = alTop
        Caption = 'Generation'
        TabOrder = 1
        object checkGeradores: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 145
          Height = 13
          Align = alTop
          Caption = 'Export Generators'
          TabOrder = 0
        end
      end
      object gbCargas: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 155
        Height = 36
        Align = alTop
        Caption = 'Load'
        TabOrder = 2
        object checkCargas: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 145
          Height = 13
          Align = alTop
          Caption = 'Export Load'
          TabOrder = 0
        end
      end
      object gbCapacitores: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 129
        Width = 155
        Height = 36
        Align = alTop
        Caption = 'Capacitors'
        TabOrder = 3
        object checkCapacitores: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 145
          Height = 13
          Align = alTop
          Caption = 'Export Capacitors'
          TabOrder = 0
        end
      end
    end
    object Panel3: TPanel
      Left = 161
      Top = 94
      Width = 188
      Height = 171
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitHeight = 169
      object gbReguladores: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 129
        Width = 182
        Height = 36
        Align = alTop
        Caption = 'Voltage Regulators'
        TabOrder = 0
        object checkReguladores: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 172
          Height = 13
          Align = alTop
          Caption = 'Export Voltage Regulators'
          TabOrder = 0
        end
      end
      object gbChaves: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 182
        Height = 120
        Align = alTop
        Caption = 'Switches'
        TabOrder = 1
        object gbTipoChave: TGroupBox
          AlignWithMargins = True
          Left = 5
          Top = 48
          Width = 172
          Height = 67
          Align = alBottom
          Caption = 'Switch Types'
          TabOrder = 0
          object checkReligadores: TCheckBox
            AlignWithMargins = True
            Left = 5
            Top = 22
            Width = 162
            Height = 17
            Align = alBottom
            Caption = 'Add Reclosers'
            TabOrder = 0
          end
          object checkSeccionadoras: TCheckBox
            AlignWithMargins = True
            Left = 5
            Top = 45
            Width = 162
            Height = 17
            Align = alBottom
            Caption = 'Add Switchgears '
            TabOrder = 1
          end
        end
        object checkChaves: TCheckBox
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 172
          Height = 13
          Align = alTop
          Caption = 'Export Switches'
          TabOrder = 1
          ExplicitLeft = 7
        end
      end
    end
    object gbConfig: TGroupBox
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 343
      Height = 88
      Align = alTop
      Caption = 'Settings'
      TabOrder = 2
      object panelRedes: TPanel
        Left = 2
        Top = 15
        Width = 339
        Height = 26
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object Label1: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 70
          Height = 20
          Align = alLeft
          Alignment = taCenter
          Caption = 'Select Feeder:'
          Layout = tlCenter
          ExplicitHeight = 13
        end
        object ComboBox1: TComboBox
          AlignWithMargins = True
          Left = 79
          Top = 3
          Width = 257
          Height = 21
          Align = alClient
          Enabled = False
          TabOrder = 0
          Text = 'All'
          ExplicitLeft = 88
          ExplicitWidth = 248
        end
      end
      object radioCoordenadas: TRadioGroup
        AlignWithMargins = True
        Left = 5
        Top = 44
        Width = 333
        Height = 39
        Align = alTop
        Caption = 'Coordinate System'
        Columns = 3
        ItemIndex = 1
        Items.Strings = (
          'UTM'
          'LatLong GMS')
        TabOrder = 1
      end
    end
  end
  object RichEdit: TRichEdit
    Left = 349
    Top = 22
    Width = 307
    Height = 265
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    PlainText = True
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 2
    Zoom = 100
    ExplicitHeight = 263
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 368
    Top = 40
    object ActionGeraDados: TAction
      Caption = 'Create Data'
      ImageIndex = 3
      OnExecute = ActionGeraDadosExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      ImageIndex = 2
    end
    object ActionSalvaArquivo: TAction
      Caption = 'Save to File'
      ImageIndex = 0
      OnExecute = ActionSalvaArquivoExecute
    end
  end
  object ImageList: TImageList
    Left = 440
    Top = 48
    Bitmap = {
      494C010104000800940010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000808080000000
      0000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C0C0C0000000
      0000C0C0C000C0C0C0000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000000000000000
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C0C0C000C0C0
      C000C0C0C000C0C0C0000000000000000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFF00000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000FFFFFF008080
      8000808080008080800080808000808080008080800080808000808080008080
      80008080800000000000FFFF000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000FF0000000000000000000000
      0000000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF0000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000FF0000000000000000000000
      00000000FF000000FF00000000000000FF000000FF000000FF000000FF000000
      000000000000000000000000FF000000FF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF00000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000FF0000000000000000000000
      FF000000FF000000FF00000000000000FF000000FF00000000000000FF000000
      FF00000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000FF00000000000000FF000000
      FF000000FF000000FF00000000000000FF000000FF0000000000000000000000
      FF000000FF000000FF00000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000C0C0C00000000000FF000000FF000000FF00
      00000000FF00FF000000FF00000000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF00000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000FF000000FF000000FF008080
      8000000000000000FF00000000000000FF000000FF0000000000000000000000
      00000000FF0000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      FF000000FF000000FF000000000000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000FF000000FF000000FF000000
      0000000000000000FF00000000000000FF000000FF0000000000000000000000
      FF000000FF000000FF00000000000000000000000000FFFFFF00000000000000
      00000000000000000000FFFFFF0000000000FFFFFF00000000000000FF000000
      FF000000FF000000FF000000FF0000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF00000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000FF000000FF00000000000000
      0000000000000000FF00000000000000000000000000000000000000FF000000
      FF00000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF00FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000FF0000000000000000000000
      0000000000000000FF0000000000FF000000FF0000000000FF000000FF000000
      000000000000000000000000FF000000FF0000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000000000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF00000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      000000000000C0C0C000C0C0C000FF000000C0C0C00000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      FF000000FF000000FF000000000000000000FFFF000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008080800000000000FFFF0000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00000000000000000000000000C0C0C000C0C0C0008080
      0000808000000080000000800000FF000000FF00000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000C0C0
      C000FFFFFF0000000000FFFFFF00000000000000000000000000808080000000
      FF000000FF000000FF000000000000000000FFFF000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFF000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000008080000000800000008000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF00000000000000000000000000FFFF000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFF00000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000800000008000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF
      0000FFFF0000FFFF0000FFFF0000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000C0C0
      C000C0C0C0000080000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FF00FFFFFFFFFFFFFF000001F83FFFFF
      FF000001E00FFFFFFF000001C0077BFF00001FF18003721C00001DF180036249
      00001CF10001426300001C7100010A7700231C3100011A6300011C7100013BC9
      00001CF100017A1C00231DF18003F87F00631FF18003807F00C30001C0071FFF
      01070001E00F9FFF03FF0001F83FE3FF00000000000000000000000000000000
      000000000000}
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo NIX (*.nix)|*.nix'
    Left = 496
    Top = 40
  end
end
