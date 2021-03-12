object FormExportaODSS: TFormExportaODSS
  Left = 560
  Top = 172
  BorderIcons = [biSystemMenu]
  Caption = 'Exporta'#231#227'o de Arquivo OpenDSS'
  ClientHeight = 255
  ClientWidth = 467
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
    Width = 467
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
  object RadioGroupModelo: TRadioGroup
    Left = 0
    Top = 25
    Width = 467
    Height = 56
    Align = alTop
    Caption = 'Modelagem da rede'
    ItemIndex = 1
    Items.Strings = (
      'Conforme especifica'#231#227'o ANEEL (sem cabo de neutro)'
      'Conforme cadastro dos equipamentos no GIS')
    TabOrder = 1
  end
  object RadioGroupTrafoDY: TRadioGroup
    Left = 0
    Top = 81
    Width = 467
    Height = 56
    Align = alTop
    Caption = 'Transformadores trif'#225'sicos Delta/Estrela'
    ItemIndex = 1
    Items.Strings = (
      'Utilizar liga'#231#227'o definida no transformador'
      'Impor liga'#231#227'o padr'#227'o:  Dy1')
    TabOrder = 2
  end
  object RadioGroupCargaEqv: TRadioGroup
    Left = 0
    Top = 137
    Width = 467
    Height = 56
    Align = alTop
    Caption = 'Cargas de uma mesma barra'
    ItemIndex = 0
    Items.Strings = (
      'Manter as cargas originais'
      'Substituir por uma carga equivalente')
    TabOrder = 3
  end
  object GBoxAviso: TGroupBox
    Left = 0
    Top = 193
    Width = 467
    Height = 62
    Align = alClient
    Caption = 'Aviso'
    Enabled = False
    TabOrder = 4
    object RichEdit1: TRichEdit
      Left = 2
      Top = 15
      Width = 463
      Height = 45
      Align = alClient
      BorderStyle = bsNone
      Color = clMoneyGreen
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Constraints.MinHeight = 45
      Lines.Strings = (
        
          'Redes com problemas n'#227'o ser'#227'o inclu'#237'das no arquivo gerado para o' +
          ' OpenDSS.'
        'O diagn'#243'stico '#233' feito atrav'#233's do c'#225'lculo de fluxo de pot'#234'ncia.')
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
      Zoom = 100
    end
  end
  object ActionList: TActionList
    Left = 400
    Top = 40
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
    DefaultExt = 'dss'
    Left = 320
    Top = 40
  end
end
