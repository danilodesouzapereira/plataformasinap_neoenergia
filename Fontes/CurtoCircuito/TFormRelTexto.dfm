object FormRelTexto: TFormRelTexto
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Relat'#243'rio Detalhado de Curto Simult'#226'neo'
  ClientHeight = 454
  ClientWidth = 1088
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
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 1088
    Height = 20
    ButtonHeight = 19
    ButtonWidth = 100
    Caption = 'ToolBar'
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionClose
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 44
      Top = 0
      Action = ActionExporta
      AutoSize = True
    end
  end
  object RichEdit: TRichEdit
    Left = 0
    Top = 20
    Width = 1088
    Height = 434
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
    TabOrder = 1
    WordWrap = False
    Zoom = 100
    ExplicitTop = 29
    ExplicitHeight = 425
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo texto (*.txt)|*.txt'
    Title = 'Selecione arquivo'
    Left = 24
    Top = 128
  end
  object ActionList: TActionList
    Left = 88
    Top = 128
    object ActionClose: TAction
      Caption = 'Fechar'
      OnExecute = ActionCloseExecute
    end
    object ActionExporta: TAction
      Caption = 'Exportar Relat'#243'rio'
      OnExecute = ActionExportaExecute
    end
  end
end
