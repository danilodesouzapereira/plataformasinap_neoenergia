object FormSobreSinapsis: TFormSobreSinapsis
  Left = 239
  Top = 162
  BorderIcons = [biSystemMenu]
  Caption = 'Sobre a Sinapsis'
  ClientHeight = 520
  ClientWidth = 834
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poOwnerFormCenter
  ShowHint = True
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 834
    Height = 25
    ButtonHeight = 19
    ButtonWidth = 45
    EdgeBorders = [ebTop, ebBottom]
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    object butFecha: TToolButton
      Left = 0
      Top = 0
      Caption = 'Fechar'
      ImageIndex = 0
      OnClick = butFechaClick
    end
  end
  object WebBrowser: TWebBrowser
    Left = 0
    Top = 25
    Width = 834
    Height = 495
    Align = alClient
    TabOrder = 1
    ExplicitLeft = 400
    ExplicitTop = 272
    ExplicitWidth = 300
    ExplicitHeight = 150
    ControlData = {
      4C00000032560000293300000000000000000000000000000000000000000000
      000000004C000000000000000000000001000000E0D057007335CF11AE690800
      2B2E12620A000000000000004C0000000114020000000000C000000000000046
      8000000000000000000000000000000000000000000000000000000000000000
      00000000000000000100000000000000000000000000000000000000}
  end
end
