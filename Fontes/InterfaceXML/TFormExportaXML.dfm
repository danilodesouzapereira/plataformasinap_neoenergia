object FormExportaXML: TFormExportaXML
  Left = 560
  Top = 172
  BorderIcons = [biSystemMenu]
  Caption = 'Exporta'#231#227'o para Arquivo XML'
  ClientHeight = 197
  ClientWidth = 318
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 318
    Height = 25
    ButtonHeight = 19
    ButtonWidth = 108
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
      Left = 107
      Top = 0
      Action = ActionCancela
      AutoSize = True
    end
  end
  object GroupBox1: TGroupBox
    Left = 32
    Top = 44
    Width = 249
    Height = 121
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
  end
  object ActionList1: TActionList
    Left = 184
    Top = 64
    object ActionExporta: TAction
      Caption = 'Gravar Arquivo XML'
      OnExecute = ActionExportaExecute
    end
    object ActionCancela: TAction
      Caption = 'Cancelar'
      OnExecute = ActionCancelaExecute
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'xml'
    Left = 152
    Top = 64
  end
end
