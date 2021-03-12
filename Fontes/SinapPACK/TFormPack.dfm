object FormPack: TFormPack
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Aguarde !'
  ClientHeight = 214
  ClientWidth = 304
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RichEdit: TRichEdit
    Left = 0
    Top = 0
    Width = 304
    Height = 214
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Zoom = 100
  end
  object Timer: TTimer
    Interval = 5000
    OnTimer = TimerTimer
    Left = 144
    Top = 112
  end
end
