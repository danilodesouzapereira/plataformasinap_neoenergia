object formCfgPadrao: TformCfgPadrao
  Left = 189
  Top = 142
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Gr'#225'fico'
  ClientHeight = 458
  ClientWidth = 731
  Color = clBtnFace
  Constraints.MinHeight = 195
  Constraints.MinWidth = 135
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
  object toolBar: TToolBar
    Left = 0
    Top = 0
    Width = 731
    Height = 29
    ButtonHeight = 21
    Caption = 'toolBar'
    EdgeBorders = [ebTop, ebBottom]
    TabOrder = 0
    object cboxEqpto: TComboBox
      Left = 0
      Top = 2
      Width = 124
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = ActionSelecionaEqptoExecute
      Items.Strings = (
        'Redu'#231#227'o'
        'Barra'
        'Capacitor'
        'Carga'
        'Chave NA'
        'Chave NF'
        'Gerador'
        'Suprimento'
        'Trafo'
        'Trafo 3E'
        'Trecho'
        'Borda e Fundo'
        'Padr'#227'o'
        'Grid')
    end
  end
  object gboxSwing: TGroupBox
    Left = 69
    Top = 39
    Width = 120
    Height = 128
    Caption = 'Barras swing'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    object gboxSwingDim: TGroupBox
      Left = 8
      Top = 80
      Width = 66
      Height = 43
      Caption = 'Espessura'
      TabOrder = 0
      object cboxSwingDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '10')
      end
    end
    object gboxSwingCor: TGroupBox
      Left = 80
      Top = 80
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelSwingCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxSwingExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxSwingExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente barra'
          'C'#243'digo'
          'Tens'#227'o(pu)'
          'Tens'#227'o(kV)')
      end
    end
  end
  object gboxChvNA: TGroupBox
    Left = 199
    Top = 43
    Width = 120
    Height = 128
    Caption = 'Chaves NA'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Visible = False
    object gboxChvNADim: TGroupBox
      Left = 8
      Top = 78
      Width = 66
      Height = 43
      Caption = 'Espessura'
      TabOrder = 0
      object cboxChvNADim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '10')
      end
    end
    object gboxChvNACor: TGroupBox
      Left = 80
      Top = 78
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelChvNACor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxChvNAExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxChvNAExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente chave'
          'C'#243'digo'
          'Corrente(A)'
          'Fluxo(MVA)'
          'Cargto(%)')
      end
    end
  end
  object gboxTrf: TGroupBox
    Left = 328
    Top = 48
    Width = 120
    Height = 128
    Caption = 'Transformadores'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Visible = False
    object gboxTrfDim: TGroupBox
      Left = 8
      Top = 80
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxTrfDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxTrfCor: TGroupBox
      Left = 80
      Top = 80
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelTrfCor: TPanel
        Left = 8
        Top = 18
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxTrfExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxTrfExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente trafo'
          'C'#243'digo'
          'Fluxo(MVA)'
          'Fluxo(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieTrf: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxTrf3e: TGroupBox
    Left = 472
    Top = 56
    Width = 120
    Height = 128
    Caption = 'Transformadores - 3E'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Visible = False
    object gboxTrf3eDim: TGroupBox
      Left = 8
      Top = 80
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxTrf3eDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxTrf3eCor: TGroupBox
      Left = 80
      Top = 80
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelTrf3eCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxTrf3eExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxTrf3eExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente trafo'
          'C'#243'digo'
          'Fluxo(MVA)'
          'Fluxo(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieTrf3e: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxGer: TGroupBox
    Left = 597
    Top = 51
    Width = 120
    Height = 128
    Caption = 'Geradores'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Visible = False
    object gboxGerDim: TGroupBox
      Left = 8
      Top = 78
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxGerDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxGerCor: TGroupBox
      Left = 80
      Top = 78
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelGerCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxGerExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxGerExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'n'#227'o exibir'
          'somente suprimento'
          'C'#243'digo'
          'Fornecimento(MVA)'
          'Fornecimento(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieGer: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxCap: TGroupBox
    Left = 141
    Top = 314
    Width = 120
    Height = 128
    Caption = 'Capacitores'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    Visible = False
    object gboxCapDim: TGroupBox
      Left = 8
      Top = 78
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxCapDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxCapCor: TGroupBox
      Left = 80
      Top = 78
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelCapCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxCapExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxCapExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'n'#227'o exibir'
          'somente capacitor'
          'C'#243'digo'
          'Pot.Reativa(MVAr)')
      end
    end
  end
  object gboxCar: TGroupBox
    Left = 597
    Top = 203
    Width = 120
    Height = 128
    Caption = 'Cargas'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    Visible = False
    object gboxCarDim: TGroupBox
      Left = 8
      Top = 78
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxCarDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxCarCor: TGroupBox
      Left = 80
      Top = 78
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelCarCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxCarExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxCarExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'n'#227'o exibir'
          'somente carga'
          'C'#243'digo'
          'Demanda(MVA)'
          'Demanda(MW, MVAr)')
      end
    end
  end
  object gboxBorda: TGroupBox
    Left = 469
    Top = 199
    Width = 121
    Height = 111
    Caption = 'Cor de Borda e Fundo'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    Visible = False
    object panelBordaCor: TPanel
      Left = 8
      Top = 23
      Width = 105
      Height = 71
      TabOrder = 0
      OnClick = ActionAlteraCorExecute
      object panelFundoCor: TPanel
        Left = 20
        Top = 14
        Width = 66
        Height = 43
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
  end
  object gboxPad: TGroupBox
    Left = 277
    Top = 328
    Width = 121
    Height = 88
    Caption = 'Selecione padr'#227'o'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    Visible = False
    object cboxPad: TComboBox
      Left = 4
      Top = 21
      Width = 114
      Height = 21
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      Sorted = True
      TabOrder = 0
    end
    object butPadrao: TButton
      Left = 20
      Top = 53
      Width = 81
      Height = 22
      Action = ActionSelecionaPad
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
    end
  end
  object gboxChvNF: TGroupBox
    Left = 205
    Top = 179
    Width = 120
    Height = 128
    Caption = 'Chaves NF'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    Visible = False
    object gboxChvNFDim: TGroupBox
      Left = 8
      Top = 79
      Width = 66
      Height = 43
      Caption = 'Espessura'
      TabOrder = 0
      object cboxChvNFDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
    end
    object gboxChvNFCor: TGroupBox
      Left = 80
      Top = 79
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelChvNFCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxChvNFExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Formato'
      TabOrder = 2
      object cboxChvNFExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente chave'
          'C'#243'digo'
          'Corrente(A)'
          'Fluxo(MVA)'
          'Fluxo(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieChvNF: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxTre: TGroupBox
    Left = 5
    Top = 175
    Width = 120
    Height = 128
    Caption = 'Trechos'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    Visible = False
    object gboxTreDim: TGroupBox
      Left = 8
      Top = 77
      Width = 66
      Height = 43
      Caption = 'Espessura'
      TabOrder = 0
      object cboxTreDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5')
      end
    end
    object gboxTreCor: TGroupBox
      Left = 80
      Top = 77
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelTreCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxTreExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxTreExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente trecho'
          'C'#243'digo'
          'Corrente(A)'
          'Fluxo(MVA)'
          'Fluxo(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieTre: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxSup: TGroupBox
    Left = 333
    Top = 179
    Width = 120
    Height = 128
    Caption = 'Suprimento'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    Visible = False
    object gboxSupDim: TGroupBox
      Left = 8
      Top = 78
      Width = 66
      Height = 43
      Caption = 'Dimens'#227'o'
      TabOrder = 0
      object cboxSupDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '4'
          '5'
          '6'
          '7'
          '8'
          '9'
          '10')
      end
    end
    object gboxSupCor: TGroupBox
      Left = 80
      Top = 78
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelSupCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxSupExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 60
      Caption = 'Exibir'
      TabOrder = 2
      object cboxSupExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'n'#227'o exibir'
          'somente suprimento'
          'C'#243'digo'
          'Fornecimento(MVA)'
          'Fornecimento(MW, MVAr)'
          'Cargto(%)')
      end
      object checkPieSup: TCheckBox
        Left = 8
        Top = 40
        Width = 94
        Height = 17
        Action = ActionAlteraExibe
        Caption = 'Carregamento'
        TabOrder = 1
      end
    end
  end
  object gboxBar: TGroupBox
    Left = 4
    Top = 32
    Width = 120
    Height = 128
    Caption = 'Barras'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Visible = False
    object gboxBarDim: TGroupBox
      Left = 8
      Top = 62
      Width = 66
      Height = 43
      Caption = 'Espessura'
      TabOrder = 0
      object cboxBarDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '1'
          '2'
          '3'
          '4'
          '5'
          '10')
      end
    end
    object gboxBarCor: TGroupBox
      Left = 80
      Top = 62
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelBarCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxBarExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 43
      Caption = 'Exibir'
      TabOrder = 2
      object cboxBarExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'somente barra'
          'C'#243'digo'
          'Tens'#227'o(pu)'
          'Tens'#227'o(kV)')
      end
    end
  end
  object gboxReducao: TGroupBox
    Left = 12
    Top = 312
    Width = 120
    Height = 128
    Caption = 'Redes Reduzidas'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    Visible = False
    object gboxReducaoDim: TGroupBox
      Left = 8
      Top = 62
      Width = 66
      Height = 43
      Caption = 'Raio'
      TabOrder = 0
      object cboxReducaoDim: TComboBox
        Left = 8
        Top = 17
        Width = 49
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          '100'
          '200'
          '500'
          '1000')
      end
    end
    object gboxReducaoCor: TGroupBox
      Left = 80
      Top = 62
      Width = 33
      Height = 43
      Caption = 'Cor'
      TabOrder = 1
      object panelReducaoCor: TPanel
        Left = 8
        Top = 19
        Width = 17
        Height = 17
        TabOrder = 0
        OnClick = ActionAlteraCorExecute
      end
    end
    object gboxReducaoExibe: TGroupBox
      Left = 8
      Top = 16
      Width = 104
      Height = 43
      Caption = 'Exibir'
      TabOrder = 2
      object cboxReducaoExibe: TComboBox
        Left = 8
        Top = 16
        Width = 88
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionAlteraPadExecute
        Items.Strings = (
          'Esquem'#225'tico'
          'Equipamentos')
      end
    end
  end
  object gboxGrid: TGroupBox
    Left = 410
    Top = 325
    Width = 120
    Height = 128
    Caption = 'Grid'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
    object gboxGridDim: TGroupBox
      Left = 7
      Top = 34
      Width = 64
      Height = 40
      Caption = 'Dimens'#227'o'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object cboxGridDim: TComboBox
        Left = 6
        Top = 14
        Width = 51
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = ActionSelGridDimExecute
        Items.Strings = (
          '10'
          '25'
          '50'
          '100'
          '250'
          '500'
          '750'
          '1000'
          '1500'
          '2000'
          '5000')
      end
    end
    object gboxGridCor: TGroupBox
      Left = 76
      Top = 34
      Width = 38
      Height = 40
      Caption = 'Cor'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object panelGridCor: TPanel
        Left = 10
        Top = 16
        Width = 17
        Height = 17
        BevelOuter = bvLowered
        TabOrder = 0
        OnClick = ActionSelGridCorExecute
      end
    end
    object gboxAlign: TGroupBox
      Left = 7
      Top = 80
      Width = 108
      Height = 40
      Caption = 'Alinhamento eqptos.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object cboxGridAlign: TComboBox
        Left = 6
        Top = 14
        Width = 96
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 0
        TabOrder = 0
        Text = 'N'#227'o utilizado'
        OnChange = ActionSelGridAlignExecute
        Items.Strings = (
          'N'#227'o utilizado'
          'Centralizado'
          'Sup. esquerdo'
          'Sup. direito'
          'Inf. esquerdo'
          'Inf. direito')
      end
    end
    object checkGridExibir: TCheckBox
      Left = 9
      Top = 15
      Width = 82
      Height = 17
      Caption = 'Exibir linhas'
      Checked = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      State = cbChecked
      TabOrder = 3
      OnClick = ActionSelGridVisibleExecute
    end
  end
  object ActionList1: TActionList
    Left = 128
    Top = 2
    object ActionAlteraPad: TAction
      OnExecute = ActionAlteraPadExecute
    end
    object ActionAlteraCor: TAction
      OnExecute = ActionAlteraCorExecute
    end
    object ActionAlteraExibe: TAction
      OnExecute = ActionAlteraExibeExecute
    end
    object ActionSelecionaPad: TAction
      Caption = 'Aplicar'
      OnExecute = ActionSelecionaPadExecute
    end
    object ActionSelecionaEqpto: TAction
      OnExecute = ActionSelecionaEqptoExecute
    end
    object ActionSelGridAlign: TAction
      OnExecute = ActionSelGridAlignExecute
    end
    object ActionSelGridCor: TAction
      OnExecute = ActionSelGridCorExecute
    end
    object ActionSelGridDim: TAction
      OnExecute = ActionSelGridDimExecute
    end
    object ActionSelGridVisible: TAction
      OnExecute = ActionSelGridVisibleExecute
    end
  end
  object ColorDialog: TColorDialog
    Ctl3D = True
    Left = 160
    Top = 4
  end
end
