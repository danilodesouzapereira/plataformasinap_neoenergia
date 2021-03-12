object FormConfirmaMedicao: TFormConfirmaMedicao
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Arquivo de Medi'#231#227'o'
  ClientHeight = 371
  ClientWidth = 463
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 463
    Height = 41
    Align = alTop
    Caption = 'Arquivo de medi'#231#227'o selecionado'
    TabOrder = 0
    object CBoxArquivo: TComboBox
      Left = 8
      Top = 14
      Width = 272
      Height = 21
      Style = csDropDownList
      TabOrder = 0
      OnChange = CBoxArquivoChange
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 309
    Width = 463
    Height = 62
    Align = alBottom
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 32
      Width = 266
      Height = 13
      Caption = 'O arquivo cont'#233'm medi'#231#245'es no formato Sinap ?'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ButSim: TButton
      Left = 293
      Top = 25
      Width = 75
      Height = 30
      Caption = 'Sim'
      TabOrder = 0
      OnClick = ButSimClick
    end
    object ButNao: TButton
      Left = 376
      Top = 25
      Width = 75
      Height = 30
      Caption = 'N'#227'o'
      TabOrder = 1
      OnClick = ButNaoClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 41
    Width = 463
    Height = 268
    Align = alClient
    Caption = 'Cont'#233'udo do arquivo de medi'#231#227'o'
    TabOrder = 2
    ExplicitTop = 96
    ExplicitHeight = 213
    object RichEdit: TRichEdit
      Left = 2
      Top = 15
      Width = 459
      Height = 251
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PlainText = True
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
      Zoom = 100
      ExplicitHeight = 196
    end
  end
end
