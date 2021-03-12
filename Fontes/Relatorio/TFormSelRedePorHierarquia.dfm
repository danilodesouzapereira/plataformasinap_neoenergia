object FormSelRedePorHierarquia: TFormSelRedePorHierarquia
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Sele'#231#227'o de Redes por Hierarquia'
  ClientHeight = 446
  ClientWidth = 270
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
    Width = 270
    Height = 360
    Align = alClient
    BorderStyle = bsNone
    Color = clSkyBlue
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
    RowSelect = True
    SortType = stText
    TabOrder = 0
    OnChange = TViewChange
    OnDblClick = TViewDblClick
    OnMouseDown = TViewMouseDown
    ExplicitTop = -6
  end
  object RadioGroupCriterio: TRadioGroup
    Left = 0
    Top = 360
    Width = 270
    Height = 86
    Align = alBottom
    Caption = 'Crit'#233'rio de sele'#231#227'o de redes'
    ItemIndex = 0
    Items.Strings = (
      'Apenas selecionada'
      'Selecionada e redes a jusante'
      'Selecionada e redes a montante')
    TabOrder = 1
    OnClick = RadioGroupCriterioClick
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
