object FormServer: TFormServer
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Servidor'
  ClientHeight = 291
  ClientWidth = 772
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
  object Splitter1: TSplitter
    Left = 0
    Top = 154
    Width = 772
    Height = 9
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 8
    ExplicitTop = 89
    ExplicitWidth = 415
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 772
    Height = 33
    ButtonWidth = 137
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ButStart: TToolButton
      Left = 0
      Top = 0
      Action = ActionServerStart
      AutoSize = True
    end
    object ButStop: TToolButton
      Left = 106
      Top = 0
      Action = ActionServerStop
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 222
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object ButClear: TToolButton
      Left = 230
      Top = 0
      Action = ActionClearLog
      AutoSize = True
    end
    object ButListaIP: TToolButton
      Left = 292
      Top = 0
      Action = ActionListaIP
      AutoSize = True
    end
    object ToolButton1: TToolButton
      Left = 347
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object ButSupervisaoAuto: TToolButton
      Left = 355
      Top = 0
      Action = ActionCfgSup
      AutoSize = True
    end
    object ButOpManual: TToolButton
      Left = 473
      Top = 0
      Action = ActionOpManual
      AutoSize = True
    end
    object ButOpAuto: TToolButton
      Left = 574
      Top = 0
      Action = ActionOpAuto
    end
  end
  object GBoxTX: TGroupBox
    Left = 0
    Top = 163
    Width = 772
    Height = 109
    Align = alClient
    Caption = 'Pacotes Enviados'
    TabOrder = 1
    object RichEditTx: TRichEdit
      Left = 2
      Top = 15
      Width = 768
      Height = 92
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      PlainText = True
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
      Zoom = 100
    end
  end
  object GBoxRx: TGroupBox
    Left = 0
    Top = 33
    Width = 772
    Height = 121
    Align = alTop
    Caption = 'Pacotes Recebidos'
    TabOrder = 2
    object RichEditRx: TRichEdit
      Left = 2
      Top = 15
      Width = 768
      Height = 104
      Align = alClient
      BorderStyle = bsNone
      Enabled = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      PlainText = True
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
      Zoom = 100
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 272
    Width = 772
    Height = 19
    Panels = <
      item
        Text = 'Port'
        Width = 300
      end
      item
        Width = 300
      end>
    SizeGrip = False
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 24
    Top = 56
    object ActionClearLog: TAction
      Caption = 'Limpar Log'
      OnExecute = ActionClearLogExecute
    end
    object ActionListaIP: TAction
      Caption = 'Listar IPs'
      OnExecute = ActionListaIPExecute
    end
    object ActionServerStart: TAction
      Caption = 'Iniciar Comunica'#231#227'o'
      OnExecute = ActionServerStartExecute
    end
    object ActionServerStop: TAction
      Caption = 'Finalizar Comunica'#231#227'o'
      OnExecute = ActionServerStopExecute
    end
  end
  object ImageList: TImageList
    Left = 80
    Top = 56
  end
  object ActionListSupervisao: TActionList
    Left = 296
    Top = 56
    object ActionCfgSup: TAction
      Caption = 'Configurar Supervis'#227'o'
      OnExecute = ActionCfgSupExecute
    end
    object ActionOpManual: TAction
      Caption = 'Supervis'#227'o Manual'
      OnExecute = ActionOpManualExecute
    end
    object ActionOpAuto: TAction
      Caption = 'Supervis'#227'o Autom'#225'tica'
      OnExecute = ActionOpAutoExecute
    end
    object ActionShow: TAction
      OnExecute = ActionShowExecute
    end
  end
end
