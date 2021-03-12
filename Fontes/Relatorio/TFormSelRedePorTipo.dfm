object FormSelRedePorTipo: TFormSelRedePorTipo
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Sele'#231#227'o de Redes por Tipo'
  ClientHeight = 297
  ClientWidth = 258
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
  object TView: TTreeView
    Left = 0
    Top = 0
    Width = 258
    Height = 297
    Align = alClient
    BorderStyle = bsNone
    Color = clMoneyGreen
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    HideSelection = False
    Indent = 16
    MultiSelect = True
    ParentFont = False
    ReadOnly = True
    SortType = stText
    TabOrder = 0
    OnAdvancedCustomDrawItem = TViewAdvancedCustomDrawItem
    OnChange = TViewChange
    OnDblClick = TViewDblClick
    OnMouseDown = TViewMouseDown
  end
  object ActionList: TActionList
    Left = 16
    Top = 32
    object ActionExibeVizinha: TAction
      Caption = 'Exibir redes vizinhas da rede'
      OnExecute = ActionExibeVizinhaExecute
    end
  end
  object PopupMenu: TPopupMenu
    AutoPopup = False
    Left = 80
    Top = 32
    object MenuItemCor: TMenuItem
      Caption = 'Redefinir cor das redes do mesmo tipo'
    end
    object MenuItemVizinha: TMenuItem
      Action = ActionExibeVizinha
    end
  end
end
