object FormZoom: TFormZoom
  Left = 538
  Top = 233
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Zoom'
  ClientHeight = 149
  ClientWidth = 159
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = ActionZoomChangedExecute
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image: TImage
    Left = 0
    Top = 0
    Width = 159
    Height = 149
    Align = alClient
    OnDblClick = ImageDblClick
    OnMouseDown = ImageMouseDown
    OnMouseMove = ImageMouseMove
    OnMouseUp = ImageMouseUp
    ExplicitWidth = 157
    ExplicitHeight = 139
  end
  object ActionList: TActionList
    Left = 8
    Top = 8
    object ActionZoomChanged: TAction
      OnExecute = ActionZoomChangedExecute
    end
  end
end
