object FormHelp: TFormHelp
  Left = 0
  Top = 0
  Caption = 'Ajuda do Sinap'
  ClientHeight = 299
  ClientWidth = 635
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
    Width = 635
    Height = 29
    ButtonWidth = 60
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ButClose: TToolButton
      Left = 0
      Top = 0
      Action = ActionClose
      AutoSize = True
    end
  end
  object GBoxHelp: TGroupBox
    Left = 161
    Top = 29
    Width = 474
    Height = 270
    Align = alClient
    Caption = '<item selecionado>'
    TabOrder = 2
    Visible = False
    object WebBrowser: TWebBrowser
      Left = 2
      Top = 15
      Width = 470
      Height = 253
      Align = alClient
      TabOrder = 0
      ExplicitLeft = -118
      ExplicitTop = 71
      ExplicitWidth = 446
      ControlData = {
        4C00000093300000261A00000100000001020000000000000000000000000000
        000000004C000000000000000000000001000000E0D057007335CF11AE690800
        2B2E126208000000000000004C0000000114020000000000C000000000000046
        8000000000000000000000000000000000000000000000000000000000000000
        00000000000000000100000000000000000000000000000000000000}
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 29
    Width = 161
    Height = 270
    ActivePage = TabSheetSinap
    Align = alLeft
    TabOrder = 3
    OnChange = PageControlChange
    object TabSheetSinap: TTabSheet
      Caption = 'Sinap'
      object LBoxSinap: TListBox
        Left = 0
        Top = 0
        Width = 153
        Height = 242
        Align = alClient
        BorderStyle = bsNone
        Color = clMoneyGreen
        ItemHeight = 13
        TabOrder = 0
        OnClick = LBoxSinapClick
      end
      object FileListBoxStk: TFileListBox
        Left = 20
        Top = 96
        Width = 115
        Height = 49
        ItemHeight = 13
        TabOrder = 1
        Visible = False
      end
    end
    object TabSheetStk: TTabSheet
      Caption = 'M'#243'dulos da Empresa'
      ImageIndex = 1
      object LBoxStk: TListBox
        Left = 0
        Top = 0
        Width = 153
        Height = 242
        Align = alClient
        BorderStyle = bsNone
        Color = clSkyBlue
        ItemHeight = 13
        TabOrder = 0
        OnClick = LBoxStkClick
      end
    end
  end
  object FileListBoxSinap: TFileListBox
    Left = 24
    Top = 80
    Width = 115
    Height = 49
    ItemHeight = 13
    TabOrder = 1
    Visible = False
  end
  object ImageList: TImageList
    Left = 232
    Top = 96
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 288
    Top = 88
    object ActionClose: TAction
      Caption = 'Fechar'
      OnExecute = ActionCloseExecute
    end
  end
end
