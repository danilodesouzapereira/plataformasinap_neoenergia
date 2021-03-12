object FormLog1: TFormLog1
  Left = 426
  Top = 290
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Relat'#243'rio de Log'
  ClientHeight = 230
  ClientWidth = 436
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object RichEdit: TRichEdit
    Left = 0
    Top = 23
    Width = 436
    Height = 207
    Align = alClient
    BorderStyle = bsNone
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    PlainText = True
    ScrollBars = ssBoth
    TabOrder = 0
    Zoom = 100
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 436
    Height = 23
    AutoSize = True
    ButtonHeight = 19
    ButtonWidth = 98
    EdgeBorders = [ebBottom]
    Flat = False
    List = True
    ShowCaptions = True
    TabOrder = 1
    Wrapable = False
    object ButSalvar: TToolButton
      Left = 0
      Top = 0
      AutoSize = True
      Caption = 'Salvar em Arquivo'
      ImageIndex = 1
      OnClick = ButSalvarClick
    end
    object ButFecha: TToolButton
      Left = 102
      Top = 0
      AutoSize = True
      Caption = 'Fechar'
      ImageIndex = 0
      OnClick = ButFechaClick
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.txt'
    Filter = 'Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*'
    Left = 56
    Top = 40
  end
end
