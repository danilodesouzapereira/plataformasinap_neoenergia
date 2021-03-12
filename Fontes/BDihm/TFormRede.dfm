object FormRede: TFormRede
  Left = 0
  Top = 0
  BorderStyle = bsNone
  ClientHeight = 281
  ClientWidth = 223
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object TView: TTreeView
    Left = 0
    Top = 0
    Width = 223
    Height = 261
    Align = alClient
    AutoExpand = True
    BorderStyle = bsNone
    Color = clMoneyGreen
    HideSelection = False
    Indent = 19
    PopupMenu = PopupMenu
    ReadOnly = True
    TabOrder = 0
    OnAdvancedCustomDrawItem = TViewAdvancedCustomDrawItem
    OnChange = TViewChange
    OnChanging = TViewChanging
    OnDblClick = TViewDblClick
    OnExpanding = TViewExpanding
    OnMouseDown = TViewMouseDown
  end
  object PanelLog: TPanel
    Left = 0
    Top = 261
    Width = 223
    Height = 20
    Align = alBottom
    BevelOuter = bvLowered
    Caption = 'Aguarde: lendo a base de dados'
    Color = clBackground
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 1
    Visible = False
  end
  object PopupMenu: TPopupMenu
    OnPopup = PopupMenuPopup
    Left = 88
    Top = 16
    object IncluirRedes1: TMenuItem
      Action = ActionInclui
    end
    object Selecionar1: TMenuItem
      Action = ActionExclui
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object MenuItemAlteraDominio: TMenuItem
      Action = ActionAlteraDominio
    end
    object MenuItemDeleteDominio: TMenuItem
      Action = ActionDeleteDominio
    end
    object MenuItemDeleteRede: TMenuItem
      Action = ActionDeleteRede
    end
  end
  object ActionList1: TActionList
    Left = 32
    Top = 16
    object ActionExclui: TAction
      Caption = 'Excluir redes da sele'#231#227'o'
      OnExecute = ActionExcluiExecute
    end
    object ActionInclui: TAction
      Caption = 'Incluir redes na sele'#231#227'o'
      OnExecute = ActionIncluiExecute
    end
    object ActionAlteraDominio: TAction
      Caption = 'Renomear Dom'#237'nio'
      OnExecute = ActionAlteraDominioExecute
    end
    object ActionDeleteDominio: TAction
      Caption = 'Retirar Dom'#237'nio da base de dados'
      OnExecute = ActionDeleteDominioExecute
    end
    object ActionDeleteRede: TAction
      Caption = 'Retirar Rede da base de dados'
      OnExecute = ActionDeleteRedeExecute
    end
  end
end
