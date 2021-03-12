object FormCargaPorTipo: TFormCargaPorTipo
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'Explorer: Cargas ordenadas por classe de consumidor'
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
    Caption = 'Cargas'
    TabOrder = 0
    object CLBoxCarga: TCheckListBox
      Left = 2
      Top = 42
      Width = 172
      Height = 263
      OnClickCheck = ActionCargaSelExecute
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
      OnDblClick = CLBoxCargaDblClick
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
        Action = ActionCargaSelAll
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
        Action = ActionCargaSelNone
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
    Caption = 'Ordena'#231#227'o por classe de consumidor'
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
    object ActionCargaSel: TAction
      OnExecute = ActionCargaSelExecute
    end
    object ActionCargaSelAll: TAction
      OnExecute = ActionCargaSelAllExecute
    end
    object ActionCargaSelNone: TAction
      OnExecute = ActionCargaSelNoneExecute
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
