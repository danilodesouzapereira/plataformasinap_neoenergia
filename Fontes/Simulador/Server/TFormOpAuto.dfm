object FormOpAuto: TFormOpAuto
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Simula'#231#227'o Autom'#225'tica'
  ClientHeight = 480
  ClientWidth = 307
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 307
    Height = 27
    ButtonWidth = 106
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    Wrapable = False
    object ButFecha: TToolButton
      Left = 0
      Top = 0
      Action = ActionFecha
      AutoSize = True
    end
    object butStart: TToolButton
      Left = 44
      Top = 0
      Action = ActionStart
      AutoSize = True
    end
    object ToolButton1: TToolButton
      Left = 134
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object ButStop: TToolButton
      Left = 142
      Top = 0
      Action = ActionStop
      AutoSize = True
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 109
    Width = 307
    Height = 236
    Align = alTop
    Caption = 'Conting'#234'ncias: Ciclo de Religamento'
    TabOrder = 1
    object Label1: TLabel
      Left = 9
      Top = 21
      Width = 113
      Height = 13
      Caption = 'Rede em estado normal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clTeal
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 27
      Top = 212
      Width = 206
      Height = 13
      Caption = 'Per'#237'odo p/ retornar ao estado normal (seg)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 27
      Top = 77
      Width = 202
      Height = 13
      Caption = 'Abertura 1: per'#237'odo c/ chave aberta (seg)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 27
      Top = 96
      Width = 165
      Height = 13
      Caption = 'Fechamento 1: defeito permanece'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 27
      Top = 115
      Width = 202
      Height = 13
      Caption = 'Abertura 2: per'#237'odo c/ chave aberta (seg)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 27
      Top = 134
      Width = 165
      Height = 13
      Caption = 'Fechamento 2: defeito permanece'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 27
      Top = 153
      Width = 114
      Height = 13
      Caption = 'Abertura 3: fim do ciclo '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 9
      Top = 60
      Width = 39
      Height = 13
      Caption = 'Defeito:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 9
      Top = 193
      Width = 109
      Height = 13
      Caption = 'Rede em conting'#234'ncia:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 27
      Top = 40
      Width = 182
      Height = 13
      Caption = 'Per'#237'odo p/ ocorr'#234'ncia de defeito (seg)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clTeal
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object CBoxTempoRedeNormal: TComboBox
      Left = 239
      Top = 37
      Width = 45
      Height = 21
      Style = csDropDownList
      DropDownCount = 15
      ItemIndex = 5
      TabOrder = 0
      Text = '60'
      Items.Strings = (
        '10'
        '20'
        '30'
        '40'
        '50'
        '60'
        '120'
        '180'
        '240'
        '300')
    end
    object CBoxTempoAbertura2: TComboBox
      Left = 239
      Top = 112
      Width = 45
      Height = 21
      Style = csDropDownList
      DropDownCount = 15
      ItemIndex = 1
      TabOrder = 1
      Text = '20'
      Items.Strings = (
        '10'
        '20'
        '30'
        '40'
        '50'
        '60')
    end
    object CBoxTempoRedeContingencia: TComboBox
      Left = 239
      Top = 206
      Width = 45
      Height = 21
      Style = csDropDownList
      DropDownCount = 15
      ItemIndex = 5
      TabOrder = 2
      Text = '30'
      Items.Strings = (
        '5'
        '10'
        '15'
        '20'
        '25'
        '30'
        '45'
        '50'
        '60')
    end
    object CBoxTempoAbertura1: TComboBox
      Left = 239
      Top = 74
      Width = 45
      Height = 21
      Style = csDropDownList
      DropDownCount = 15
      ItemIndex = 1
      TabOrder = 3
      Text = '10'
      Items.Strings = (
        '5'
        '10'
        '15'
        '20'
        '25'
        '30'
        '45'
        '50'
        '60')
    end
    object CheckBoxLockout: TCheckBox
      Left = 27
      Top = 172
      Width = 214
      Height = 17
      Caption = 'Gerar indica'#231#227'o de lockout'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 4
    end
  end
  object PanelTop: TPanel
    Left = 0
    Top = 27
    Width = 307
    Height = 82
    Align = alTop
    TabOrder = 2
    object RadioGroupPatamar: TRadioGroup
      Left = 1
      Top = 1
      Width = 305
      Height = 80
      Align = alClient
      Caption = 'Altera'#231#227'o de patamares'
      ItemIndex = 2
      Items.Strings = (
        'Tempo real'
        'Intervalo de tempo configur'#225'vel (min)'
        'Ap'#243's simular defeito em todas chaves')
      TabOrder = 0
      OnClick = RadioGroupPatamarClick
    end
    object CBoxTempoPatamar: TComboBox
      Left = 239
      Top = 36
      Width = 45
      Height = 21
      BevelInner = bvNone
      Style = csDropDownList
      DropDownCount = 15
      ItemIndex = 2
      TabOrder = 1
      Text = '5'
      Items.Strings = (
        '1'
        '2'
        '5'
        '10'
        '15'
        '20'
        '30')
    end
  end
  object GBoxEvento: TGroupBox
    Left = 0
    Top = 345
    Width = 307
    Height = 135
    Align = alClient
    Caption = 'Eventos'
    TabOrder = 3
    object RichEdit: TRichEdit
      Left = 2
      Top = 15
      Width = 303
      Height = 78
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      HideScrollBars = False
      ParentFont = False
      ParentShowHint = False
      PlainText = True
      ShowHint = True
      TabOrder = 0
      WordWrap = False
      Zoom = 100
    end
    object PanelPatamar: TPanel
      Left = 2
      Top = 93
      Width = 303
      Height = 40
      Align = alBottom
      BevelOuter = bvLowered
      Color = clMoneyGreen
      ParentBackground = False
      TabOrder = 1
      object LabelPatamar: TLabel
        Left = 7
        Top = 6
        Width = 49
        Height = 13
        Caption = 'Patamar'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelChave: TLabel
        Left = 7
        Top = 23
        Width = 35
        Height = 13
        Caption = 'Chave'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LabelTimer: TLabel
        Left = 263
        Top = 23
        Width = 33
        Height = 13
        Alignment = taRightJustify
        Caption = 'Timer'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
  end
  object Timer: TTimer
    OnTimer = TimerTimer
    Left = 24
    Top = 368
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 80
    Top = 368
    object ActionFecha: TAction
      Caption = 'Fechar'
      OnExecute = ActionFechaExecute
    end
    object ActionStart: TAction
      Caption = 'Iniciar Simula'#231#227'o'
      OnExecute = ActionStartExecute
    end
    object ActionStop: TAction
      Caption = 'Parar Simula'#231#227'o'
      OnExecute = ActionStopExecute
    end
  end
  object ImageList: TImageList
    Left = 136
    Top = 368
  end
end
