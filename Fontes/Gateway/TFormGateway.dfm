object FormGateway: TFormGateway
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Interface Gateway Sinapsis'
  ClientHeight = 275
  ClientWidth = 545
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 545
    Height = 29
    ButtonWidth = 140
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    ExplicitWidth = 455
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionClose
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 44
      Top = 0
      Action = ActionReadSel
      AutoSize = True
    end
    object ButRead: TToolButton
      Left = 168
      Top = 0
      Action = ActionReadAll
      AutoSize = True
    end
    object ButWrite: TToolButton
      Left = 259
      Top = 0
      Action = ActionWriteAll
      AutoSize = True
    end
    object ToolButton3: TToolButton
      Left = 365
      Top = 0
      Action = ActionClear
      AutoSize = True
    end
  end
  object GBoxRede: TGroupBox
    Left = 0
    Top = 29
    Width = 185
    Height = 246
    Align = alLeft
    Caption = 'Redes do Gateway'
    TabOrder = 1
    object TViewRede: TTreeView
      Left = 2
      Top = 15
      Width = 181
      Height = 229
      Align = alClient
      BorderStyle = bsNone
      Indent = 19
      MultiSelect = True
      ReadOnly = True
      TabOrder = 0
      OnDblClick = TViewRedeDblClick
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 216
    Top = 48
    object ActionClose: TAction
      Caption = 'Fechar'
      OnExecute = ActionCloseExecute
    end
    object ActionReadAll: TAction
      Caption = 'Ler Todas Redes'
      OnExecute = ActionReadAllExecute
    end
    object ActionReadSel: TAction
      Caption = 'Ler Redes Selecionadas'
      OnExecute = ActionReadSelExecute
    end
    object ActionWriteAll: TAction
      Caption = 'Salvar Todas Redes'
      OnExecute = ActionWriteAllExecute
    end
    object ActionClear: TAction
      Caption = 'Reiniciar Gateway'
      OnExecute = ActionClearExecute
    end
  end
  object ImageList: TImageList
    Left = 288
    Top = 48
  end
end
