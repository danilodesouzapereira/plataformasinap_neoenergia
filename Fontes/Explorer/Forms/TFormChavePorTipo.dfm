object FormChavePorTipo: TFormChavePorTipo
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Explorer: Chaves ordenadas por tipo'
  ClientHeight = 307
  ClientWidth = 380
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 200
    Top = 0
    Width = 4
    Height = 307
    ExplicitLeft = 208
  end
  object gboxRede: TGroupBox
    Left = 204
    Top = 0
    Width = 176
    Height = 307
    Align = alClient
    Caption = 'Chaves'
    TabOrder = 0
    object CLBoxChave: TCheckListBox
      Left = 2
      Top = 42
      Width = 172
      Height = 263
      OnClickCheck = ActionChaveSelExecute
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ItemHeight = 12
      ParentFont = False
      Sorted = True
      Style = lbOwnerDrawFixed
      TabOrder = 0
      OnDblClick = CLBoxChaveDblClick
    end
    object ToolBarRede: TToolBar
      Left = 2
      Top = 15
      Width = 172
      Height = 27
      ButtonHeight = 19
      Caption = 'ToolBarRede'
      EdgeBorders = [ebTop, ebBottom]
      TabOrder = 1
      object butSelAllCirc: TBitBtn
        Left = 0
        Top = 0
        Width = 17
        Height = 19
        Action = ActionChaveSelAll
        Caption = #254
        Font.Charset = SYMBOL_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Wingdings'
        Font.Style = [fsBold]
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 0
      end
      object butSelNoneCirc: TBitBtn
        Left = 17
        Top = 0
        Width = 17
        Height = 19
        Action = ActionChaveSelNone
        Caption = 'o'
        Font.Charset = SYMBOL_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Wingdings'
        Font.Style = [fsBold]
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 1
      end
    end
  end
  object GBoxOrdenacao: TGroupBox
    Left = 0
    Top = 0
    Width = 200
    Height = 307
    Align = alLeft
    Caption = 'Ordena'#231#227'o por tipo de chave'
    TabOrder = 1
    object TView: TTreeView
      Left = 2
      Top = 15
      Width = 196
      Height = 290
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      HideSelection = False
      Indent = 16
      ParentFont = False
      ReadOnly = True
      SortType = stText
      TabOrder = 0
      OnChange = TViewChange
      OnDblClick = TViewDblClick
      OnMouseDown = TViewMouseDown
    end
  end
  object ActionList: TActionList
    Left = 16
    Top = 32
    object ActionChaveSel: TAction
      Caption = 'ActionChaveSel'
      OnExecute = ActionChaveSelExecute
    end
    object ActionChaveSelAll: TAction
      OnExecute = ActionChaveSelAllExecute
    end
    object ActionChaveSelNone: TAction
      Caption = 'ActionRedeSelNone'
      OnExecute = ActionChaveSelNoneExecute
    end
  end
  object PopupMenu: TPopupMenu
    AutoPopup = False
    Left = 48
    Top = 32
    object MenuItemCor: TMenuItem
      Caption = 'Redefinir cor das redes do mesmo tipo'
    end
    object MenuItemVizinha: TMenuItem
      Caption = 'Exibir redes vizinhas da rede'
    end
  end
end
