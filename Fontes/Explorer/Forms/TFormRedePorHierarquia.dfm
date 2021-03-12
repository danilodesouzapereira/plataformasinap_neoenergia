object FormRedePorHierarquia: TFormRedePorHierarquia
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Explorer: Redes ordenadas por hierarquia'
  ClientHeight = 418
  ClientWidth = 394
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 200
    Top = 0
    Width = 4
    Height = 418
    ExplicitLeft = 196
    ExplicitHeight = 307
  end
  object Panel: TPanel
    Left = 204
    Top = 0
    Width = 190
    Height = 418
    Align = alClient
    TabOrder = 0
    object Splitter2: TSplitter
      Left = 1
      Top = 222
      Width = 188
      Height = 4
      Cursor = crVSplit
      Align = alTop
      ExplicitTop = 177
      ExplicitWidth = 178
    end
    object Splitter3: TSplitter
      Left = 1
      Top = 97
      Width = 188
      Height = 5
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 6
      ExplicitTop = 25
      ExplicitWidth = 178
    end
    object GBoxMontante: TGroupBox
      Left = 1
      Top = 102
      Width = 188
      Height = 120
      Align = alTop
      Caption = 'Redes a Montante (Supridoras)'
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      TabOrder = 0
      object CLBoxMontante: TCheckListBox
        Left = 2
        Top = 39
        Width = 184
        Height = 79
        OnClickCheck = ActionSelRedeExecute
        Align = alClient
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ItemHeight = 12
        ParentFont = False
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnDblClick = CLBoxDblClick
        OnDrawItem = CLBoxDrawItem
      end
      object ToolBarMontante: TToolBar
        Left = 2
        Top = 15
        Width = 184
        Height = 24
        ButtonHeight = 19
        Caption = 'ToolBarMontante'
        EdgeBorders = [ebTop, ebBottom]
        TabOrder = 1
        object ButSelAllMontante: TBitBtn
          Left = 0
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelAllMontante
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
        object ButSelNoneMontante: TBitBtn
          Left = 23
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelNoneMontante
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
    object GBoxRede: TGroupBox
      Left = 1
      Top = 1
      Width = 188
      Height = 96
      Align = alTop
      Caption = 'Redes Selecionadas'
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      TabOrder = 1
      object CLBoxRede: TCheckListBox
        Left = 2
        Top = 39
        Width = 184
        Height = 55
        OnClickCheck = ActionSelRedeExecute
        Align = alClient
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ItemHeight = 12
        ParentFont = False
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnDblClick = CLBoxDblClick
        OnDrawItem = CLBoxDrawItem
      end
      object ToolBarSelecionada: TToolBar
        Left = 2
        Top = 15
        Width = 184
        Height = 24
        ButtonHeight = 19
        EdgeBorders = [ebTop, ebBottom]
        TabOrder = 1
        object ButSelAllSelecionada: TBitBtn
          Left = 0
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelAllSelecionada
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
        object ButSelNoneSelecionada: TBitBtn
          Left = 23
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelNoneSelecionada
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
    object GBoxJusante: TGroupBox
      Left = 1
      Top = 226
      Width = 188
      Height = 191
      Align = alClient
      Caption = 'Redes a Jusante (Supridas)'
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      TabOrder = 2
      object CLBoxJusante: TCheckListBox
        Left = 2
        Top = 39
        Width = 184
        Height = 150
        OnClickCheck = ActionSelRedeExecute
        Align = alClient
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ItemHeight = 12
        ParentFont = False
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnDblClick = CLBoxDblClick
        OnDrawItem = CLBoxDrawItem
      end
      object ToolBarJusante: TToolBar
        Left = 2
        Top = 15
        Width = 184
        Height = 24
        ButtonHeight = 19
        Caption = 'ToolBarRede'
        EdgeBorders = [ebTop, ebBottom]
        TabOrder = 1
        object ButSelAllJusante: TBitBtn
          Left = 0
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelAllJusante
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
        object ButSeNonelJusante: TBitBtn
          Left = 23
          Top = 0
          Width = 23
          Height = 19
          Action = ActionSelNoneJusante
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
  end
  object GBoxOrdenacao: TGroupBox
    Left = 0
    Top = 0
    Width = 200
    Height = 418
    Align = alLeft
    Caption = 'Ordena'#231#227'o por hierarquia'
    TabOrder = 1
    object TView: TTreeView
      Left = 2
      Top = 15
      Width = 196
      Height = 401
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      HideSelection = False
      Indent = 16
      MultiSelect = True
      MultiSelectStyle = [msControlSelect, msShiftSelect, msSiblingOnly]
      ParentFont = False
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      OnChange = TViewChange
      OnChanging = TViewChanging
      OnMouseDown = TViewMouseDown
    end
  end
  object dlgSelColor: TColorDialog
    Left = 31
    Top = 137
  end
  object ActionList: TActionList
    Left = 32
    Top = 16
    object ActionAlteraCor: TAction
      Caption = 'Redefinir cor das redes selecionadas'
      OnExecute = ActionAlteraCorExecute
    end
    object ActionSelAllJusante: TAction
      Caption = #254
      Hint = 'Exibir todas redes a jusante'
      OnExecute = ActionSelAllJusanteExecute
    end
    object ActionSelAllMontante: TAction
      Caption = #254
      Hint = 'Exibir todas redes a montante'
      OnExecute = ActionSelAllMontanteExecute
    end
    object ActionSelAllSelecionada: TAction
      Caption = #254
      Hint = 'Exibir todas redes selecionadas'
      OnExecute = ActionSelAllSelecionadaExecute
    end
    object ActionSelNoneJusante: TAction
      Caption = 'o'
      Hint = 'Esconder todas redes a jusante'
      OnExecute = ActionSelNoneJusanteExecute
    end
    object ActionSelNoneMontante: TAction
      Caption = 'o'
      Hint = 'Esconder todas redes a montante'
      OnExecute = ActionSelNoneMontanteExecute
    end
    object ActionSelNoneSelecionada: TAction
      Caption = 'o'
      Hint = 'Esconder todas redes a jusante'
      OnExecute = ActionSelNoneSelecionadaExecute
    end
    object ActionSelRede: TAction
      OnExecute = ActionSelRedeExecute
    end
    object ActionExcluiRedeJus: TAction
      Caption = 'Excluir redes a jusante'
      OnExecute = ActionExcluiRedeJusExecute
    end
    object ActionExcluiRedeMon: TAction
      Caption = 'Excluir redes a montante'
      OnExecute = ActionExcluiRedeMonExecute
    end
    object ActionExcluiRedeSel: TAction
      Caption = 'Excluir redes selecionadas'
      OnExecute = ActionExcluiRedeSelExecute
    end
    object ActionExcluiRedeSel_Jus: TAction
      Caption = 'Excluir rede selecionada e redes a jusante'
      OnExecute = ActionExcluiRedeSel_JusExecute
    end
  end
  object PopupMenu: TPopupMenu
    AutoPopup = False
    Left = 32
    Top = 64
    object Redefiniracor1: TMenuItem
      Action = ActionAlteraCor
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Excluirredeselecionada1: TMenuItem
      Action = ActionExcluiRedeSel
    end
    object Excluirredesamontante1: TMenuItem
      Action = ActionExcluiRedeMon
    end
    object Excluirredeselecionada2: TMenuItem
      Action = ActionExcluiRedeJus
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Excluirredeselecionadaeredesajusante1: TMenuItem
      Action = ActionExcluiRedeSel_Jus
    end
  end
end
