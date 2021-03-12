object FormPerda: TFormPerda
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Perdas T'#233'cnicas'
  ClientHeight = 594
  ClientWidth = 1209
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
  object PanelLeft: TPanel
    Left = 0
    Top = 34
    Width = 247
    Height = 560
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
    object GBoxRede: TGroupBox
      Left = 0
      Top = 210
      Width = 247
      Height = 350
      Align = alClient
      Caption = 'Filtrar Redes nos Relat'#243'rios'
      TabOrder = 0
      object TView: TTreeView
        Left = 2
        Top = 15
        Width = 243
        Height = 297
        Align = alClient
        BorderStyle = bsNone
        DragMode = dmAutomatic
        HideSelection = False
        Indent = 19
        MultiSelect = True
        ReadOnly = True
        RowSelect = True
        SortType = stText
        TabOrder = 0
        OnChange = TViewChange
      end
      object PanelLocaliza: TPanel
        Left = 2
        Top = 312
        Width = 243
        Height = 36
        Align = alBottom
        TabOrder = 1
        object ButLocaliza: TBitBtn
          Left = 5
          Top = 8
          Width = 77
          Height = 23
          Action = ActionLocaliza
          Caption = 'Localizar'
          Glyph.Data = {
            36040000424D3604000000000000360000002800000010000000100000000100
            2000000000000004000000000000000000000000000000000000FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00000000000000
            0000000000000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF000000000000000000000000000000000000000000FF00FF0000000000FFFF
            FF00000000000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF0000000000FFFFFF00000000000000000000000000FF00FF0000000000FFFF
            FF00000000000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF0000000000FFFFFF00000000000000000000000000FF00FF00000000000000
            00000000000000000000000000000000000000000000FF00FF00000000000000
            00000000000000000000000000000000000000000000FF00FF00000000000000
            0000FFFFFF00000000000000000000000000000000000000000000000000FFFF
            FF000000000000000000000000000000000000000000FF00FF00000000000000
            0000FFFFFF00000000000000000000000000FF00FF000000000000000000FFFF
            FF000000000000000000000000000000000000000000FF00FF00000000000000
            0000FFFFFF00000000000000000000000000FF00FF000000000000000000FFFF
            FF000000000000000000000000000000000000000000FF00FF00FF00FF000000
            0000000000000000000000000000000000000000000000000000000000000000
            000000000000000000000000000000000000FF00FF00FF00FF00FF00FF00FF00
            FF0000000000FFFFFF00000000000000000000000000FF00FF0000000000FFFF
            FF00000000000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF000000000000000000000000000000000000000000FF00FF00000000000000
            0000000000000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00000000000000000000000000FF00FF00FF00FF00FF00FF000000
            00000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF0000000000FFFFFF0000000000FF00FF00FF00FF00FF00FF000000
            0000FFFFFF0000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00000000000000000000000000FF00FF00FF00FF00FF00FF000000
            00000000000000000000FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
            FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
          TabOrder = 0
        end
        object RichEditLocaliza: TRichEdit
          Left = 88
          Top = 9
          Width = 120
          Height = 20
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          PlainText = True
          TabOrder = 1
          WordWrap = False
          Zoom = 100
          OnKeyPress = RichEditLocalizaKeyPress
        end
      end
    end
    object GBoxRelatorio: TGroupBox
      Left = 0
      Top = 115
      Width = 247
      Height = 95
      Align = alTop
      Caption = 'Selecionar Relat'#243'rio'
      Color = clBtnFace
      ParentBackground = False
      ParentColor = False
      TabOrder = 1
      object CheckBoxBalancoTotal: TCheckBox
        Left = 6
        Top = 16
        Width = 187
        Height = 13
        Caption = 'Balan'#231'o Total de Energia e Perdas'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = CheckBoxBalancoTotalClick
      end
      object CBoxRelatorio: TComboBox
        Left = 3
        Top = 37
        Width = 234
        Height = 21
        Style = csDropDownList
        DropDownCount = 30
        TabOrder = 1
        OnChange = CBoxRelatorioChange
        Items.Strings = (
          'Equipamentos por SED'
          'Equipamentos por SDMT'
          'Equipamentos por SDBT'
          'Equipamentos por Rede'
          'Balan'#231'o de Energia por SED'
          'Balan'#231'o de Energia por SDMT'
          'Balan'#231'o de Energia por SDBT'
          'Balan'#231'o de Energia por Rede'
          'Perdas por SED'
          'Perdas por SDMT'
          'Perdas por SDBT'
          'Perdas por Rede'
          'Equipamentos Agregados por SED'
          'Equipamentos Agregados por SDMT'
          'Equipamentos Agregados por Rede'
          'Balan'#231'o Agregado de Energia por SED'
          'Balan'#231'o Agregado de Energia por SDMT'
          'Balan'#231'o Agregado de Energia por Rede'
          'Perdas Agregadas por SED'
          'Perdas Agregadas por SDMT'
          'Perdas Agregadas por Rede'
          'Perdas Agregadas por Segmento')
      end
      object ButHelp: TBitBtn
        Left = 22
        Top = 64
        Width = 203
        Height = 22
        Action = ActionHelp
        Caption = 'Ajuda '
        Glyph.Data = {
          36040000424D3604000000000000360000002800000010000000100000000100
          2000000000000004000000000000000000000000000000000000FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FFFFFF007F7F7F000000FF007F7F7F00FFFF
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF0000FFFF00FFFFFF0000FFFF000000FF000000FF000000FF0000FF
          FF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF007F7F7F000000FF007F7F7F00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FF00FF0000FF
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF00FF00FF00FF00FF00FFFFFF0000FF
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000FF007F7F7F0000FF
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00FF00FF0000FFFF00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FF00FF00FFFFFF0000FF
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00FF00FF0000FFFF00FFFF
          FF0000FFFF00FFFFFF007F7F7F007F7F7F0000FFFF00FFFFFF007F7F7F000000
          FF000000FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FF00FF00FFFFFF0000FF
          FF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF007F7F7F000000
          FF000000FF0000FFFF00FFFFFF0000FFFF00FFFFFF00FF00FF00FF00FF00FFFF
          FF0000FFFF00FFFFFF000000FF000000FF007F7F7F00FFFFFF007F7F7F000000
          FF000000FF00FFFFFF0000FFFF00FFFFFF00FF00FF00FF00FF00FF00FF0000FF
          FF00FFFFFF0000FFFF00FFFFFF000000FF000000FF000000FF000000FF000000
          FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FF00
          FF0000FFFF00FFFFFF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFF
          FF0000FFFF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
          FF00FFFFFF0000FFFF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00
          FF00FF00FF00FF00FF00FF00FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
          FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00FF00}
        TabOrder = 2
      end
    end
    object GBoxResumo: TGroupBox
      Left = 0
      Top = 0
      Width = 247
      Height = 115
      Align = alTop
      Caption = 'Balan'#231'o Total de Energia'
      TabOrder = 2
      object LViewResumo: TListView
        Left = 2
        Top = 15
        Width = 243
        Height = 98
        Align = alClient
        Color = clBtnShadow
        Columns = <
          item
            Width = 88
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 60
          end>
        Constraints.MaxWidth = 1154
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
  end
  object PanelClient: TPanel
    Left = 247
    Top = 34
    Width = 962
    Height = 560
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object Splitter: TSplitter
      Left = 0
      Top = 115
      Width = 962
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitTop = 106
    end
    object GBoxBalancoTotal: TGroupBox
      Left = 0
      Top = 0
      Width = 962
      Height = 115
      Align = alTop
      Caption = 'Balan'#231'o de Energia por Segmento'
      TabOrder = 0
      object LViewTotalBalanco: TListView
        Left = 2
        Top = 15
        Width = 958
        Height = 98
        Align = alClient
        Color = clMoneyGreen
        Columns = <
          item
            Caption = 'Segmento'
            Width = 75
          end
          item
            Caption = 'Seq.'
            Width = 75
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Externa(1)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Montante(2)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.GD(3)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Injetada(1+2+3)'
            Tag = 1
            Width = 95
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Perda'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Consumidor(4)'
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Jusante(5)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Externa(6)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Fornecida(4+5+6)'
            Tag = 1
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxAgregadoPerda: TGroupBox
      Left = 6
      Top = 331
      Width = 909
      Height = 79
      Caption = 'Perdas Agregadas da Rede e de suas Redes a Jusante'
      TabOrder = 1
      object LViewAgregadoPerda: TListView
        Left = 2
        Top = 15
        Width = 905
        Height = 62
        Align = alClient
        Color = clSkyBlue
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia Injetada'
            Tag = 1
            Width = 92
          end
          item
            Alignment = taRightJustify
            Caption = 'Perda Pr'#243'pria'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Cabo'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'RT+BC'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo(cu)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo(fe)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Ramal Liga'#231#227'o'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Medidor'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Outros'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Total'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = '% (PT*)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Tag = 1
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxRedeEqpto: TGroupBox
      Left = 6
      Top = 109
      Width = 905
      Height = 65
      Caption = 'Quantidade de Equipamentos por Rede'
      Color = clBtnFace
      Constraints.MinHeight = 65
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 2
      object LViewRedeEqpto: TListView
        Left = 2
        Top = 15
        Width = 901
        Height = 48
        Align = alClient
        BorderStyle = bsNone
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Rede'
            Tag = 1
            Width = 45
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cabo(km)'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Regulador'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Capacitor'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.A4'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Res.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Com.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Ind.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Rur.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Out.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Nulo'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia'
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxRedeBalanco: TGroupBox
      Left = 6
      Top = 409
      Width = 907
      Height = 81
      Caption = 'Balan'#231'o de Energia por Rede '
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 3
      object LViewRedeBalanco: TListView
        Left = 2
        Top = 15
        Width = 903
        Height = 64
        Align = alClient
        BorderStyle = bsNone
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Externa(1)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Montante(2)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.GD(3)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Injetada(1+2+3)'
            Tag = 1
            Width = 95
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Perda'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Consumidor(4)'
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Jusante(5)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Externa(6)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Fornecida(4+5+6)'
            Tag = 1
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxAgregadoBalanco: TGroupBox
      Left = 6
      Top = 564
      Width = 907
      Height = 80
      Caption = 'Balan'#231'o de Energia Agregado da Rede e de suas Redes a Jusante'
      Color = clBtnFace
      Constraints.MinHeight = 80
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 4
      object LViewAgregadoBalanco: TListView
        Left = 2
        Top = 15
        Width = 903
        Height = 63
        Align = alClient
        BorderStyle = bsNone
        Color = clSkyBlue
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Externa(1)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.Montante(2)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Inj.GD(3)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Injetada(1+2+3)'
            Tag = 1
            Width = 95
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Perda'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Consumidor(4)'
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Jusante(5)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'For.Externa(6)'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Fornecida(4+5+6)'
            Tag = 1
            Width = 105
          end
          item
            Alignment = taRightJustify
            Caption = '% (EI*)'
            Tag = 1
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxAgregadoEqpto: TGroupBox
      Left = 6
      Top = 180
      Width = 907
      Height = 64
      Caption = 
        'Quantidade de Equipamentos Agregados da Rede e de suas Redes a J' +
        'usante'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 5
      object LViewAgregadoEqpto: TListView
        Left = 2
        Top = 15
        Width = 903
        Height = 47
        Align = alClient
        BorderStyle = bsNone
        Color = clSkyBlue
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Rede'
            Tag = 1
            Width = 45
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cabo(km)'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Regulador'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Capacitor'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.A4'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Res.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Com.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Ind.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Rur.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Out.'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Cons.Nulo'
            Tag = 1
            Width = 65
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia'
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxRedePerda: TGroupBox
      Left = 6
      Top = 248
      Width = 907
      Height = 77
      Caption = 'Perdas por Rede '
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 6
      object LViewRedePerda: TListView
        Left = 2
        Top = 15
        Width = 903
        Height = 60
        Align = alClient
        BorderStyle = bsNone
        Columns = <
          item
            Caption = 'Tipo'
          end
          item
            Caption = 'C'#243'digo'
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia Injetada'
            Tag = 1
            Width = 92
          end
          item
            Alignment = taRightJustify
            Caption = 'Cabo'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'RT+BC'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo(cu)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Trafo(fe)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Ramal Liga'#231#227'o'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Medidor'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Outros'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'Total'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = '% (PT*)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Tag = 1
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
    object GBoxPerdaSegmento: TGroupBox
      Left = 6
      Top = 662
      Width = 909
      Height = 67
      Caption = 'Perdas por Segmento'
      TabOrder = 7
      object LViewPerdaSegmento: TListView
        Left = 2
        Top = 15
        Width = 905
        Height = 50
        Align = alClient
        Color = clActiveBorder
        Columns = <
          item
            Caption = 'Segmento'
            Width = 150
          end
          item
            Alignment = taRightJustify
            Caption = 'Energia Injetada'
            Tag = 1
            Width = 92
          end
          item
            Alignment = taRightJustify
            Caption = 'Perda '
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'IPTS (%)'
            Tag = 1
            Width = 90
          end
          item
            Alignment = taRightJustify
            Caption = 'PPT (%)'
            Tag = 1
            Width = 90
          end>
        GridLines = True
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnColumnClick = LViewColumnClick
        OnCompare = LViewCompare
      end
    end
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 1209
    Height = 34
    ButtonHeight = 30
    ButtonWidth = 109
    Caption = 'ToolBar'
    EdgeBorders = [ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 2
    object ButFecha: TToolButton
      Left = 0
      Top = 0
      Action = ActionFecha
      AutoSize = True
    end
    object ButUnidade: TToolButton
      Left = 64
      Top = 0
      AutoSize = True
      Caption = 'Definir Unidade'
      DropdownMenu = PopupMenuUnidade
      Style = tbsDropDown
    end
    object ButRelatorio: TToolButton
      Left = 169
      Top = 0
      Action = ActionSalvaRelatorio
      AutoSize = True
    end
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 40
    Top = 281
    object ActionMnuGrupos_RegiaoAdiciona: TAction
      Category = 'Menu Grupos'
      Caption = 'Adicionar Regi'#227'o'
      ImageIndex = 8
    end
    object ActionMnuGrupos_RegiaoRemove: TAction
      Category = 'Menu Grupos'
      Caption = 'Remover Regi'#227'o'
      ImageIndex = 9
    end
    object ActionMnuGrupos_RegiaoRenomeia: TAction
      Category = 'Menu Grupos'
      Caption = 'Renomear Regi'#227'o'
      ImageIndex = 10
    end
    object ActionMnuGrupos_LocalAdiciona: TAction
      Category = 'Menu Grupos'
      Caption = 'Adicionar Localidade'
      ImageIndex = 6
    end
    object ActionMnuGrupos_LocalRemove: TAction
      Category = 'Menu Grupos'
      Caption = 'Remover Localidade'
      ImageIndex = 7
    end
    object ActionMnuGrupos_LocalRenomeia: TAction
      Category = 'Menu Grupos'
      Caption = 'Renomear Localidade'
      ImageIndex = 10
    end
    object ActionLocaliza: TAction
      Caption = 'Localizar'
      ImageIndex = 1
      OnExecute = ActionLocalizaExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda sobre os relat'#243'rios'
      ImageIndex = 2
      OnExecute = ActionHelpExecute
    end
    object ActionAlteraUnidade: TAction
      OnExecute = ActionAlteraUnidadeExecute
    end
    object ActionFecha: TAction
      Caption = 'Fechar'
      ImageIndex = 3
      OnExecute = ActionFechaExecute
    end
    object ActionSalvaRelatorio: TAction
      Caption = 'Salvar Relat'#243'rio'
      ImageIndex = 4
      OnExecute = ActionSalvaRelatorioExecute
    end
  end
  object SaveDialog: TSaveDialog
    Filter = 
      'Arquivo texto (*.txt)|*.txt|Arquivo CSV (*.csv)|*.csv|Todos os a' +
      'rquivos|*.*'
    Title = 'Selecione arquivo'
    OnTypeChange = SaveDialogTypeChange
    Left = 40
    Top = 416
  end
  object ImageList: TImageList
    Left = 40
    Top = 352
    Bitmap = {
      494C010105006000A00010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000808080000000
      0000808080008080800000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C0C0C0000000
      0000C0C0C000C0C0C00000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C0C0C000C0C0
      C000C0C0C000C0C0C00000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000C0C0C00000000000FF000000FF000000FF00
      00000000FF00FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      00000000000000000000FFFFFF0000000000FFFFFF00000000000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000C0C0
      C000FFFFFF0000000000FFFFFF00000000000000000000000000808080000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF007F7F7F000000FF007F7F7F00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF0000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF007F7F7F000000FF007F7F7F00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF00000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      84000000840000008400000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      84000000840000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF007F7F7F0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF00000000000000000000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF007F7F7F007F7F7F0000FFFF00FFFFFF007F7F7F000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF000000000000000000000000000000000000000000FFFFFF00000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF007F7F7F000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF007F7F7F00FFFFFF007F7F7F000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      84000000840000000000000000000000000000000000FFFFFF0000000000BFBF
      BF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      84000000840000008400000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF000000000000000000000000000000000000000000FFFFFF000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF00FF00000000000000FF00000000000000
      FF00000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000230000000000000001000000000000
      00000000000000000023000000000000006300000000000000C3000000000000
      010700000000000003FF000000000000FFDFFFFFFFFFFFFFFFCFFFFFF83FFFFF
      FFC707C1E00FFFFF000307C1C007E7F3000107C18003E3E3000001018003F1C7
      000100010001F88F000302010001FC1F000702010001FE3F000F80030001FC1F
      001FC1070001F88F007FC1078003F1C700FFE38F8003E3E301FFE38FC007E7F3
      03FFE38FE00FFFFFFFFFFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object PopupMenuUnidade: TPopupMenu
    AutoHotkeys = maManual
    Left = 143
    Top = 289
    object MenuItem_kwh: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'kWh'
      RadioItem = True
    end
    object MenuItem_kwh_30dias: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'kWh/30dias'
      RadioItem = True
    end
    object MenuItem_kwh_diasmes: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'kWh/m'#234's'
      RadioItem = True
    end
    object N1: TMenuItem
      AutoCheck = True
      Caption = '-'
      RadioItem = True
    end
    object MenuItem_mwh: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'MWh'
      RadioItem = True
    end
    object MenuItem_mwh_30dias: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'MWh/30dias'
      RadioItem = True
    end
    object MenuItem_mwh_diasmes: TMenuItem
      Action = ActionAlteraUnidade
      AutoCheck = True
      Caption = 'MWh/m'#234's'
      Checked = True
      RadioItem = True
    end
  end
end
