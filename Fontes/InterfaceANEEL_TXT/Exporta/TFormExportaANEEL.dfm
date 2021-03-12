object FormExportaANEEL: TFormExportaANEEL
  Left = 560
  Top = 172
  BorderIcons = [biSystemMenu]
  Caption = 'Exporta'#231#227'o para Arquivo ANEEL'
  ClientHeight = 160
  ClientWidth = 315
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
    Width = 315
    Height = 25
    ButtonHeight = 19
    ButtonWidth = 122
    EdgeBorders = [ebTop, ebBottom]
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    ExplicitWidth = 390
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
  object GBoxBaseDado: TGroupBox
    Left = 0
    Top = 25
    Width = 315
    Height = 47
    Align = alTop
    Caption = 'Informa'#231#245'es adicionais'
    TabOrder = 1
    ExplicitLeft = -129
    ExplicitTop = 28
    ExplicitWidth = 519
    object PanelData: TPanel
      Left = 2
      Top = 15
      Width = 311
      Height = 25
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitWidth = 515
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
  object ActionList: TActionList
    Left = 184
    Top = 72
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
    Left = 120
    Top = 72
  end
end
