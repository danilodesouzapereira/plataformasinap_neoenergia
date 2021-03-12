object FormRelChave: TFormRelChave
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Capacidade Disruptiva das Chaves'
  ClientHeight = 256
  ClientWidth = 492
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
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 492
    Height = 22
    ButtonHeight = 19
    ButtonWidth = 120
    EdgeBorders = [ebBottom]
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Action = ActionExporta
      AutoSize = True
    end
    object ToolButton3: TToolButton
      Left = 119
      Top = 0
      Width = 8
      Caption = 'ToolButton3'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object ToolButton2: TToolButton
      Left = 127
      Top = 0
      Action = ActionClose
      AutoSize = True
    end
  end
  object LView: TListView
    Left = 0
    Top = 22
    Width = 492
    Height = 234
    Align = alClient
    BorderStyle = bsNone
    Columns = <
      item
        Caption = 'Chave'
        Width = 120
      end
      item
        Alignment = taRightJustify
        Caption = 'Cap.Disruptiva(A)'
        Width = 100
      end
      item
        Caption = 'Defeito'
      end
      item
        Alignment = taRightJustify
        Caption = 'I defeito (A)'
        Width = 100
      end
      item
        Caption = 'Diagn'#243'stico'
        Width = 100
      end>
    GridLines = True
    ReadOnly = True
    RowSelect = True
    TabOrder = 1
    ViewStyle = vsReport
    OnColumnClick = LViewColumnClick
    OnCompare = LViewCompare
    ExplicitLeft = 192
    ExplicitTop = 80
    ExplicitWidth = 250
    ExplicitHeight = 150
  end
  object ActionList: TActionList
    Left = 32
    Top = 72
    object ActionClose: TAction
      Caption = 'Fechar'
      OnExecute = ActionCloseExecute
    end
    object ActionExporta: TAction
      Caption = 'Relat'#243'rio (Arquivo txt)'
      OnExecute = ActionExportaExecute
    end
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo text (*.txt)|*.txt'
    Left = 88
    Top = 72
  end
end
