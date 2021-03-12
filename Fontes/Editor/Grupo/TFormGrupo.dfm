object FormGrupo: TFormGrupo
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Inser'#231#227'o de M'#243'dulos Construtivos'
  ClientHeight = 236
  ClientWidth = 266
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object TViewGrupo: TTreeView
    Left = 0
    Top = 0
    Width = 266
    Height = 236
    Align = alClient
    BorderStyle = bsNone
    Color = clBtnFace
    HideSelection = False
    Indent = 19
    ReadOnly = True
    RowSelect = True
    SortType = stText
    TabOrder = 0
    OnClick = ActionExibeModuloExecute
    OnDblClick = ActionExibeModuloExecute
  end
  object DirectoryListBox: TDirectoryListBox
    Left = 104
    Top = 8
    Width = 145
    Height = 85
    TabOrder = 1
    Visible = False
  end
  object FileListBox: TFileListBox
    Left = 104
    Top = 99
    Width = 145
    Height = 97
    ItemHeight = 13
    TabOrder = 2
    Visible = False
  end
  object ActionList: TActionList
    Left = 72
    Top = 40
    object ActionExibeModulo: TAction
      Caption = 'Exibe M'#243'dulo Construtivo'
      OnExecute = ActionExibeModuloExecute
    end
    object ActionOnDone: TAction
      OnExecute = ActionOnDoneExecute
    end
  end
end
