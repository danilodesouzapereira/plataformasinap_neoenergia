object FormMapa: TFormMapa
  Left = 0
  Top = 0
  Margins.Left = 0
  Margins.Top = 0
  Margins.Right = 0
  Margins.Bottom = 0
  HorzScrollBar.Style = ssFlat
  HorzScrollBar.Visible = False
  VertScrollBar.Style = ssFlat
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Mapa Google'
  ClientHeight = 385
  ClientWidth = 569
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object WebBrowser: TWebBrowser
    Left = -5
    Top = 0
    Width = 551
    Height = 352
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    ParentCustomHint = False
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    StyleElements = [seFont, seClient]
    ControlData = {
      4C000000F3380000612400000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E126209000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 488
    Top = 304
  end
end
