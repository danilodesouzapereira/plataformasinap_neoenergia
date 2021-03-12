object FormClient: TFormClient
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Cliente'
  ClientHeight = 462
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 256
    Width = 792
    Height = 9
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 8
    ExplicitTop = 89
    ExplicitWidth = 415
  end
  object GBoxTX: TGroupBox
    Left = 0
    Top = 118
    Width = 792
    Height = 138
    Align = alTop
    Caption = 'Pacotes Enviados'
    TabOrder = 0
    object RichEditTx: TRichEdit
      Left = 2
      Top = 15
      Width = 788
      Height = 121
      Align = alClient
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
    Top = 265
    Width = 792
    Height = 178
    Align = alClient
    Caption = 'Pacotes Recebidos'
    TabOrder = 1
    object RichEditRx: TRichEdit
      Left = 2
      Top = 15
      Width = 788
      Height = 161
      Align = alClient
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
    Top = 443
    Width = 792
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 22
    Width = 792
    Height = 96
    Align = alTop
    Caption = 'Solicita'#231#227'o de Registro'
    TabOrder = 3
    object Label1: TLabel
      Left = 13
      Top = 24
      Width = 100
      Height = 13
      Caption = 'Tipo de Equipamento'
    end
    object Label2: TLabel
      Left = 13
      Top = 46
      Width = 113
      Height = 13
      Caption = 'C'#243'digo do Equipamento'
    end
    object Label3: TLabel
      Left = 13
      Top = 70
      Width = 87
      Height = 13
      Caption = 'Hor'#225'rio dos dados'
    end
    object CBoxEqptoTipo: TComboBox
      Left = 130
      Top = 21
      Width = 156
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 0
      Text = '01 - Barra'
      Items.Strings = (
        '01 - Barra'
        '02 - Trecho'
        '03 - Chave'
        '04 - Transformador'
        '05 - Regulador de Tens'#227'o'
        '06 - Capacitor S'#233'rie'
        '07 - Capacitor'
        '08 - Gerador'
        '09 - Suprimento'
        '10 - Carga'
        '11 - Filtro'
        '12 - Reator')
    end
    object EditEqptoCodigo: TEdit
      Left = 129
      Top = 44
      Width = 157
      Height = 21
      TabOrder = 1
    end
    object Button1: TButton
      Left = 292
      Top = 21
      Width = 90
      Height = 33
      Action = ActionSolicitaRegistro
      TabOrder = 2
    end
    object DateTimePicker: TDateTimePicker
      Left = 130
      Top = 67
      Width = 71
      Height = 21
      Date = 41572.766978252320000000
      Time = 41572.766978252320000000
      Kind = dtkTime
      TabOrder = 3
    end
    object CheckBoxTempoReal: TCheckBox
      Left = 218
      Top = 69
      Width = 91
      Height = 17
      Caption = 'Tempo real'
      TabOrder = 4
      OnClick = CheckBoxTempoRealClick
    end
    object Button2: TButton
      Left = 491
      Top = 21
      Width = 90
      Height = 33
      Action = ActionChaveAbre
      TabOrder = 5
    end
    object Button3: TButton
      Left = 491
      Top = 60
      Width = 90
      Height = 33
      Action = ActionChaveFecha
      TabOrder = 6
    end
    object Button4: TButton
      Left = 391
      Top = 21
      Width = 90
      Height = 33
      Action = ActionSimulaFalta
      TabOrder = 7
    end
    object Button5: TButton
      Left = 391
      Top = 60
      Width = 90
      Height = 33
      Action = ActionEliminaFalta
      TabOrder = 8
    end
    object Button6: TButton
      Left = 588
      Top = 21
      Width = 90
      Height = 33
      Action = ActionTrafoDecTap
      TabOrder = 9
    end
    object Button7: TButton
      Left = 588
      Top = 60
      Width = 90
      Height = 33
      Action = ActionTrafoIncTap
      TabOrder = 10
    end
    object Button8: TButton
      Left = 688
      Top = 21
      Width = 90
      Height = 33
      Action = ActionCapacitorOff
      TabOrder = 11
    end
    object Button9: TButton
      Left = 688
      Top = 60
      Width = 90
      Height = 33
      Action = ActionCapacitorOn
      TabOrder = 12
    end
  end
  object PanelToolBar: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 22
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 4
    object ToolBar: TToolBar
      Left = 0
      Top = 0
      Width = 469
      Height = 22
      Align = alLeft
      ButtonWidth = 169
      EdgeBorders = [ebBottom]
      Images = ImageList
      List = True
      ShowCaptions = True
      TabOrder = 0
      Wrapable = False
      object ButStart: TToolButton
        Left = 0
        Top = 0
        Action = ActionStart
        AutoSize = True
      end
      object ButStop: TToolButton
        Left = 106
        Top = 0
        Action = ActionStop
        AutoSize = True
      end
      object ButSupervisaoAuto: TToolButton
        Left = 222
        Top = 0
        Action = ActionSupervisaoAuto
        AutoSize = True
      end
      object ToolButton1: TToolButton
        Left = 375
        Top = 0
        Width = 8
        Caption = 'ToolButton1'
        ImageIndex = 0
        Style = tbsSeparator
      end
      object ButClearLog: TToolButton
        Left = 383
        Top = 0
        Action = ActionClearLog
        AutoSize = True
      end
    end
    object Panel: TPanel
      Left = 469
      Top = 0
      Width = 323
      Height = 22
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object label: TLabel
        Left = 8
        Top = 4
        Width = 62
        Height = 13
        Caption = 'Selecionar IP'
      end
      object CBoxIP: TComboBox
        Left = 78
        Top = 1
        Width = 133
        Height = 21
        TabOrder = 0
      end
    end
  end
  object ActionList: TActionList
    Left = 24
    Top = 160
    object ActionStart: TAction
      Caption = 'Iniciar Comunica'#231#227'o'
      OnExecute = ActionStartExecute
    end
    object ActionStop: TAction
      Caption = 'Finalizar Comunica'#231#227'o'
      OnExecute = ActionStopExecute
    end
    object ActionClearLog: TAction
      Caption = 'Limpar Log'
      OnExecute = ActionClearLogExecute
    end
    object ActionChaveAbre: TAction
      Caption = 'Abrir Chave'
      OnExecute = ActionChaveAbreExecute
    end
    object ActionChaveFecha: TAction
      Caption = 'Fechar Chave'
      OnExecute = ActionChaveFechaExecute
    end
    object ActionEliminaFalta: TAction
      Caption = 'Eliminar Falta'
      OnExecute = ActionEliminaFaltaExecute
    end
    object ActionSimulaFalta: TAction
      Caption = 'Simular Falta'
      OnExecute = ActionSimulaFaltaExecute
    end
    object ActionSolicitaRegistro: TAction
      Caption = 'Solicitar Registro'
      OnExecute = ActionSolicitaRegistroExecute
    end
    object ActionTrafoDecTap: TAction
      Caption = 'Trafo Tap--'
      Hint = 'Aumentar tens'#227'o no secund'#225'rio'
      OnExecute = ActionTrafoDecTapExecute
    end
    object ActionTrafoIncTap: TAction
      Caption = 'Trafo Tap++'
      Hint = 'Diminuir tens'#227'o no secund'#225'rio'
      OnExecute = ActionTrafoIncTapExecute
    end
    object ActionSupervisaoAuto: TAction
      Caption = 'Ativar Supervis'#227'o Autom'#225'tica'
      OnExecute = ActionSupervisaoAutoExecute
    end
    object ActionCapacitorOff: TAction
      Caption = 'Capacitor OFF'
      OnExecute = ActionCapacitorOffExecute
    end
    object ActionCapacitorOn: TAction
      Caption = 'Capacitor ON'
      OnExecute = ActionCapacitorOnExecute
    end
  end
  object ImageList: TImageList
    Left = 80
    Top = 160
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 128
    Top = 160
  end
end
