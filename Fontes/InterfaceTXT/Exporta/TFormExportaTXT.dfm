object FormExportaTXT: TFormExportaTXT
  Left = 560
  Top = 172
  BorderIcons = [biSystemMenu]
  Caption = 'Exporta'#231#227'o para Arquivo TEXTO'
  ClientHeight = 160
  ClientWidth = 305
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 305
    Height = 25
    ButtonHeight = 19
    ButtonWidth = 122
    EdgeBorders = [ebTop, ebBottom]
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionExporta
      AutoSize = True
    end
    object ButCancela: TToolButton
      Left = 121
      Top = 0
      Action = ActionCancela
      AutoSize = True
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 25
    Width = 305
    Height = 135
    Align = alClient
    Caption = 'Dados Exportados'
    TabOrder = 1
    object CheckBoxArranjos: TCheckBox
      Left = 9
      Top = 21
      Width = 193
      Height = 17
      Caption = 'Cabos e Arranjos'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CheckBoxCurvasTipicas: TCheckBox
      Left = 9
      Top = 38
      Width = 169
      Height = 17
      Caption = 'Curvas T'#237'picas'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object CheckBoxTipos: TCheckBox
      Left = 9
      Top = 56
      Width = 198
      Height = 17
      Caption = 'Tipos de Redes e Chaves'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object CheckBoxRedes: TCheckBox
      Left = 9
      Top = 74
      Width = 193
      Height = 17
      Caption = 'Redes'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
    object CheckBoxReducoes: TCheckBox
      Left = 9
      Top = 92
      Width = 200
      Height = 19
      Caption = 'Redes equivalentes'
      TabOrder = 4
    end
    object CheckBoxCript: TCheckBox
      Left = 9
      Top = 109
      Width = 200
      Height = 19
      Caption = 'Criptografar arquivo texto'
      Enabled = False
      TabOrder = 5
    end
  end
  object ActionList: TActionList
    Left = 184
    Top = 64
    object ActionExporta: TAction
      Caption = 'Gravar Arquivo TEXTO'
      OnExecute = ActionExportaExecute
    end
    object ActionCancela: TAction
      Caption = 'Cancelar'
      OnExecute = ActionCancelaExecute
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'txt'
    Left = 152
    Top = 64
  end
end
