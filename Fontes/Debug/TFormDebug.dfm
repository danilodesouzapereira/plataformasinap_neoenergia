object FormDebug: TFormDebug
  Left = 465
  Top = 251
  Caption = 'Debug'
  ClientHeight = 312
  ClientWidth = 553
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = ActionOrdenaPorCorteExecute
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 249
    Top = 0
    Height = 312
    ExplicitLeft = 201
    ExplicitHeight = 319
  end
  object gboxEqpto: TGroupBox
    Left = 252
    Top = 0
    Width = 301
    Height = 312
    Align = alClient
    Caption = '<Eqpto>'
    TabOrder = 0
    object sgrid: TStringGrid
      Left = 2
      Top = 15
      Width = 297
      Height = 295
      Align = alClient
      ColCount = 2
      DefaultRowHeight = 15
      FixedCols = 0
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goTabs]
      ParentFont = False
      TabOrder = 0
      ColWidths = (
        80
        107)
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 249
    Height = 312
    Align = alLeft
    Caption = 'Ordenar '
    TabOrder = 1
    object ToolBar1: TToolBar
      Left = 2
      Top = 15
      Width = 245
      Height = 25
      ButtonHeight = 19
      ButtonWidth = 70
      Caption = 'ToolBar'
      EdgeBorders = [ebTop, ebBottom]
      List = True
      ShowCaptions = True
      TabOrder = 0
      Wrapable = False
      object butOrdenaPorCorte: TToolButton
        Left = 0
        Top = 0
        Action = ActionOrdenaPorCorte
        AutoSize = True
        Grouped = True
        ParentShowHint = False
        ShowHint = True
        Style = tbsCheck
      end
      object butOrdenaPorRede: TToolButton
        Left = 41
        Top = 0
        Action = ActionOrdenaPorRede
        AutoSize = True
        Grouped = True
        Style = tbsCheck
      end
      object butOrdenaPorMRede: TToolButton
        Left = 83
        Top = 0
        Action = ActionOrdenaPorMRede
        AutoSize = True
        Grouped = True
        Style = tbsCheck
      end
      object ButZona: TToolButton
        Left = 152
        Top = 0
        Action = ActionZonas
        AutoSize = True
        Grouped = True
        Style = tbsCheck
      end
      object ButBlocos: TToolButton
        Left = 193
        Top = 0
        Action = ActionBlocos
        AutoSize = True
        Grouped = True
        PopupMenu = PopupMenuBlocos
        Style = tbsCheck
      end
    end
    object TView: TTreeView
      Left = 2
      Top = 40
      Width = 245
      Height = 242
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
      TabOrder = 1
      OnChange = TViewChange
      OnDblClick = TViewDblClick
      ExplicitLeft = 1
    end
    object Panel1: TPanel
      Left = 2
      Top = 282
      Width = 245
      Height = 28
      Align = alBottom
      TabOrder = 2
      object CheckBoxChaveAberta: TCheckBox
        Left = 8
        Top = 6
        Width = 200
        Height = 17
        Caption = 'Zonas: incluir chaves abertas '
        TabOrder = 0
        OnClick = ActionZonasExecute
      end
    end
  end
  object ActionList: TActionList
    Left = 16
    Top = 48
    object ActionBlocos: TAction
      Caption = 'Blocos'
      OnExecute = ActionBlocosExecute
    end
    object ActionOrdenaPorCorte: TAction
      Caption = 'Cortes'
      Hint = 'Exibir ordena'#231#227'o por Cortes'
      OnExecute = ActionOrdenaPorCorteExecute
    end
    object ActionOrdenaPorRede: TAction
      Caption = 'Redes'
      Hint = 'Exibir ordena'#231#227'o por Redes'
      OnExecute = ActionOrdenaPorRedeExecute
    end
    object ActionOrdenaPorMRede: TAction
      Caption = 'Meta Redes'
      Hint = 'Exibir ordena'#231#227'o por Meta Redes'
      OnExecute = ActionOrdenaPorMRedeExecute
    end
    object ActionZonas: TAction
      Caption = 'Zonas'
      OnExecute = ActionZonasExecute
    end
  end
  object PopupMenuBlocos: TPopupMenu
    Left = 184
    Top = 48
    object Eliminablocosnoenergizados1: TMenuItem
      Action = ActionEliminaBlocosNaoEnergizados
    end
    object Eliminaeloscomchaveaberta1: TMenuItem
      Action = ActionEliminaElosComChaveAberta
    end
    object Eliminaelosqueligamummesmobloco1: TMenuItem
      Action = ActionEliminaElosBlocosIguais
    end
  end
  object ActionListBlocos: TActionList
    Left = 184
    Top = 96
    object ActionEliminaBlocosNaoEnergizados: TAction
      Caption = 'Eliminar blocos n'#227'o energizados'
      OnExecute = ActionEliminaBlocosNaoEnergizadosExecute
    end
    object ActionEliminaElosBlocosIguais: TAction
      Caption = 'Eliminar elos que ligam um mesmo bloco'
      OnExecute = ActionEliminaElosBlocosIguaisExecute
    end
    object ActionEliminaElosComChaveAberta: TAction
      Caption = 'Eliminar elos com chave aberta'
      OnExecute = ActionEliminaElosComChaveAbertaExecute
    end
  end
end
