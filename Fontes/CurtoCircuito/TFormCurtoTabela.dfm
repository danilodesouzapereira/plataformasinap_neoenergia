object FormCurtoTabela: TFormCurtoTabela
  Left = 289
  Top = 189
  BorderIcons = [biSystemMenu, biMaximize]
  BorderStyle = bsNone
  Caption = 'Relat'#243'rio de Curto-Circuito'
  ClientHeight = 281
  ClientWidth = 685
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 31
    Width = 685
    Height = 250
    ActivePage = TabSheetPotencias
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabHeight = 30
    TabOrder = 0
    OnChange = PageControlChange
    object TabSheetPtoDefeito: TTabSheet
      Caption = 'Ponto de Defeito'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewDefIcc: TListView
        Left = 0
        Top = 48
        Width = 677
        Height = 48
        Align = alTop
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 86
          end
          item
            Caption = 'Ia'
            Width = 86
          end
          item
            Caption = 'Ib'
            Width = 86
          end
          item
            Caption = 'Ic'
            Width = 86
          end
          item
            Caption = 'I0'
            Width = 86
          end
          item
            Caption = 'I1'
            Width = 86
          end
          item
            Caption = 'I2'
            Width = 86
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnDblClick = LviewSelecionaZoom
      end
      object LViewDefVcc: TListView
        Left = 0
        Top = 0
        Width = 677
        Height = 48
        Align = alTop
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 86
          end
          item
            Caption = 'Va'
            Width = 86
          end
          item
            Caption = 'Vb'
            Width = 86
          end
          item
            Caption = 'Vc'
            Width = 86
          end
          item
            Caption = 'V0'
            Width = 86
          end
          item
            Caption = 'V1'
            Width = 86
          end
          item
            Caption = 'V2'
            Width = 86
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 1
        ViewStyle = vsReport
        OnChange = LViewChange
        OnDblClick = LviewSelecionaZoom
      end
    end
    object TabSheetContribuicao: TTabSheet
      Caption = 'Contribui'#231#245'es'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewSupIcc: TListView
        Left = 0
        Top = 0
        Width = 677
        Height = 210
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Ia'
            Width = 75
          end
          item
            Caption = 'Ib'
            Width = 75
          end
          item
            Caption = 'Ic'
            Width = 75
          end
          item
            Caption = 'I0'
            Width = 75
          end
          item
            Caption = 'I1'
            Width = 75
          end
          item
            Caption = 'I2'
            Width = 75
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
        OnDblClick = LviewSelecionaZoom
      end
    end
    object TabSheetTensao: TTabSheet
      Caption = 'Tens'#245'es'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewBarVcc: TListView
        Left = 0
        Top = 0
        Width = 677
        Height = 210
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Va'
            Width = 75
          end
          item
            Caption = 'Vb'
            Width = 75
          end
          item
            Caption = 'Vc'
            Width = 75
          end
          item
            Caption = 'V0'
            Width = 75
          end
          item
            Caption = 'V1'
            Width = 75
          end
          item
            Caption = 'V2'
            Width = 75
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
        OnDblClick = LviewSelecionaZoom
      end
    end
    object TabSheetCorrente: TTabSheet
      Caption = 'Correntes'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewLigIcc: TListView
        Left = 0
        Top = 0
        Width = 677
        Height = 210
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Barra ref.'
            Width = 60
          end
          item
            Caption = 'Ia'
            Width = 75
          end
          item
            Caption = 'Ib'
            Width = 75
          end
          item
            Caption = 'Ic'
            Width = 75
          end
          item
            Caption = 'I0'
            Width = 75
          end
          item
            Caption = 'I1'
            Width = 75
          end
          item
            Caption = 'I2'
            Width = 75
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
        OnDblClick = LviewSelecionaZoom
      end
    end
    object TabSheetPotencias: TTabSheet
      Caption = 'Pot'#234'ncias'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewLigScc: TListView
        Left = 0
        Top = 0
        Width = 677
        Height = 210
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Barra ref.'
            Width = 60
          end
          item
            Caption = 'P'
            Width = 75
          end
          item
            Caption = 'Q'
            Width = 75
          end
          item
            Caption = 'S'
            Width = 75
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
        OnDblClick = LviewSelecionaZoom
      end
    end
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 685
    Height = 31
    Align = alTop
    BevelInner = bvLowered
    TabOrder = 1
    object CBoxDefeito: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 5
      Width = 145
      Height = 21
      Align = alLeft
      Style = csDropDownList
      TabOrder = 0
      OnChange = CBoxChange
    end
    object CBoxUnidTensao: TComboBox
      AlignWithMargins = True
      Left = 156
      Top = 5
      Width = 86
      Height = 21
      Align = alLeft
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 1
      Text = 'Tens'#227'o: pu'
      OnChange = CBoxChange
      Items.Strings = (
        'Tens'#227'o: pu'
        'Tens'#227'o: kV')
    end
    object CBoxUnidCorrente: TComboBox
      AlignWithMargins = True
      Left = 248
      Top = 5
      Width = 86
      Height = 21
      Align = alLeft
      Style = csDropDownList
      ItemIndex = 1
      TabOrder = 2
      Text = 'Corrente: kA'
      OnChange = CBoxChange
      Items.Strings = (
        'Corrente: A'
        'Corrente: kA')
    end
    object ButRelatorio: TBitBtn
      Left = 424
      Top = 3
      Width = 129
      Height = 25
      Caption = 'Exportar Relat'#243'rio'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333303
        333333333333337FF3333333333333903333333333333377FF33333333333399
        03333FFFFFFFFF777FF3000000999999903377777777777777FF0FFFF0999999
        99037F3337777777777F0FFFF099999999907F3FF777777777770F00F0999999
        99037F773777777777730FFFF099999990337F3FF777777777330F00FFFFF099
        03337F773333377773330FFFFFFFF09033337F3FF3FFF77733330F00F0000003
        33337F773777777333330FFFF0FF033333337F3FF7F3733333330F08F0F03333
        33337F7737F7333333330FFFF003333333337FFFF77333333333000000333333
        3333777777333333333333333333333333333333333333333333}
      NumGlyphs = 2
      TabOrder = 3
      OnClick = ButRelatorioClick
    end
    object CBoxUnidPotencia: TComboBox
      AlignWithMargins = True
      Left = 340
      Top = 5
      Width = 86
      Height = 21
      Align = alLeft
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 4
      Text = 'Pot'#234'ncia: k*'
      OnChange = CBoxChange
      Items.Strings = (
        'Pot'#234'ncia: k*'
        'Pot'#234'ncia: M*')
    end
  end
  object SaveDialog: TSaveDialog
    Filter = 'Arquivo texto (*.txt)|*.txt|Arquivo Excel (*.xls)|*.xls'
    Title = 'Selecione arquivo'
    OnTypeChange = SaveDialogTypeChange
    Left = 24
    Top = 136
  end
end
