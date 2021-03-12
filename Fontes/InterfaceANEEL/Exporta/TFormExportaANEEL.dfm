object FormExportaANEEL: TFormExportaANEEL
  Left = 560
  Top = 172
  BorderIcons = [biSystemMenu]
  Caption = 'Exporta'#231#227'o para Arquivo ANEEL'
  ClientHeight = 302
  ClientWidth = 470
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
    Width = 470
    Height = 25
    ButtonHeight = 19
    ButtonWidth = 120
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
      Left = 119
      Top = 0
      Action = ActionCancela
      AutoSize = True
    end
  end
  object GBoxBaseDado: TGroupBox
    Left = 0
    Top = 25
    Width = 470
    Height = 47
    Align = alTop
    Caption = 'Informa'#231#245'es adicionais'
    TabOrder = 1
    object PanelData: TPanel
      Left = 2
      Top = 15
      Width = 466
      Height = 25
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object LabelData: TLabel
        Left = 110
        Top = 4
        Width = 192
        Height = 16
        Caption = 'Data de refer'#234'ncia dos dados'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object DateTimePicker: TDateTimePicker
        Left = 3
        Top = 1
        Width = 100
        Height = 21
        Date = 40697.416249247680000000
        Format = 'dd/MM/yyyy'
        Time = 40697.416249247680000000
        DateFormat = dfLong
        TabOrder = 0
      end
    end
  end
  object RadioGroupCampoAcoplamento: TRadioGroup
    Left = 0
    Top = 184
    Width = 470
    Height = 68
    Align = alTop
    Caption = 'Formato do campo "c'#243'digo do ponto acoplamento"'
    ItemIndex = 2
    Items.Strings = (
      'Sem informa'#231#227'o de coordenadas'
      'Coordenadas utm, concatenadas sem separador'
      'Coordenadas utm com numerador sequencial (padr'#227'o Sinap) ')
    TabOrder = 2
  end
  object RadioGroupModelo: TRadioGroup
    Left = 0
    Top = 72
    Width = 470
    Height = 56
    Align = alTop
    Caption = 'Modelagem da rede'
    ItemIndex = 1
    Items.Strings = (
      'Conforme especifica'#231#227'o ANEEL (sem cabo de neutro)'
      'Conforme cadastro dos equipamentos no GIS')
    TabOrder = 3
  end
  object RadioGroupTrafoDY: TRadioGroup
    Left = 0
    Top = 128
    Width = 470
    Height = 56
    Align = alTop
    Caption = 'Transformadores trif'#225'sicos Delta/Estrela'
    ItemIndex = 1
    Items.Strings = (
      'Utilizar liga'#231#227'o definida no transformador'
      'Impor liga'#231#227'o padr'#227'o:  Dy1')
    TabOrder = 4
  end
  object ActionList: TActionList
    Left = 408
    Top = 40
    object ActionExporta: TAction
      Caption = 'Gravar Base de Dados'
      OnExecute = ActionExportaExecute
    end
    object ActionCancela: TAction
      Caption = 'Cancelar'
      OnExecute = ActionCancelaExecute
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'mdb'
    Left = 328
    Top = 40
  end
end
