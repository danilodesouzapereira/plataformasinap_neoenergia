object FormEdtCar: TFormEdtCar
  Left = 500
  Top = 170
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu]
  Caption = 'Edi'#231#227'o de Carga'
  ClientHeight = 287
  ClientWidth = 284
  Color = clBtnFace
  Constraints.MinHeight = 326
  Constraints.MinWidth = 300
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 26
    Width = 284
    Height = 261
    ActivePage = TabSheetEdicao
    Align = alClient
    TabOrder = 0
    OnChange = PageControlChange
    OnChanging = PageControlChanging
    object TabSheetCarga: TTabSheet
      Caption = 'Carga'
      Constraints.MinHeight = 221
      object RadioGroupTipoCurva: TRadioGroup
        AlignWithMargins = True
        Left = 3
        Top = 138
        Width = 270
        Height = 40
        Align = alTop
        Caption = 'Demanda'
        Columns = 2
        Items.Strings = (
          'Curva t'#237'pica'
          'Curva pr'#243'pria')
        TabOrder = 0
        OnClick = RadioGroupTipoCurvaClick
      end
      object GBoxTipica: TGroupBox
        AlignWithMargins = True
        Left = 3
        Top = 184
        Width = 270
        Height = 44
        Align = alTop
        Caption = 'Demanda definida por curva t'#237'pica'
        TabOrder = 1
        object Label4: TLabel
          AlignWithMargins = True
          Left = 5
          Top = 18
          Width = 73
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Energia'
          Layout = tlCenter
        end
        object CBoxUnidEnergia: TComboBox
          AlignWithMargins = True
          Left = 187
          Top = 18
          Width = 78
          Height = 21
          Align = alRight
          Style = csDropDownList
          TabOrder = 0
          OnChange = CBoxUnidEnergiaChange
          Items.Strings = (
            'kWh/m'#234's'
            'MWh/m'#234's')
        end
        object EditEnergia: TEdit
          AlignWithMargins = True
          Left = 84
          Top = 18
          Width = 97
          Height = 21
          Align = alClient
          TabOrder = 1
        end
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 276
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 2
        object Label1: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 78
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'C'#243'digo'
          Layout = tlCenter
        end
        object EditCodigo: TEdit
          AlignWithMargins = True
          Left = 87
          Top = 3
          Width = 186
          Height = 21
          Align = alClient
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 27
        Width = 276
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 3
        object Label5: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 78
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Fases'
          Layout = tlCenter
        end
        object CBoxFase: TComboBox
          AlignWithMargins = True
          Left = 220
          Top = 3
          Width = 53
          Height = 21
          Align = alRight
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 0
          Text = 'ABC'
          OnChange = CBoxFaseChange
          Items.Strings = (
            'ABC'
            'ABCN')
        end
        object CBoxMBT: TComboBox
          AlignWithMargins = True
          Left = 87
          Top = 3
          Width = 127
          Height = 21
          Align = alClient
          Style = csDropDownList
          ItemIndex = 2
          TabOrder = 1
          Text = 'Trif'#225'sica'
          OnChange = CBoxMBTChange
          Items.Strings = (
            'Monof'#225'sica'
            'Bif'#225'sica'
            'Trif'#225'sica')
        end
      end
      object Panel3: TPanel
        Left = 0
        Top = 54
        Width = 276
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 4
        object Label6: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 78
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Modelo'
          Layout = tlCenter
        end
        object CBoxModCarga: TComboBox
          AlignWithMargins = True
          Left = 87
          Top = 3
          Width = 186
          Height = 21
          Align = alClient
          Style = csDropDownList
          ItemIndex = 2
          TabOrder = 0
          Text = 'Imped'#226'ncia constante'
          Items.Strings = (
            'Corrente constante'
            'Pot'#234'ncia constante'
            'Imped'#226'ncia constante')
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 81
        Width = 276
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 5
        object Label2: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 78
          Height = 21
          Align = alLeft
          AutoSize = False
          Caption = 'Classe'
          Layout = tlCenter
        end
        object CBoxClasse: TComboBox
          AlignWithMargins = True
          Left = 87
          Top = 3
          Width = 186
          Height = 21
          Align = alClient
          Style = csDropDownList
          TabOrder = 0
          OnChange = CBoxClasseChange
        end
      end
      object Panel5: TPanel
        Left = 0
        Top = 108
        Width = 276
        Height = 27
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 6
        object CheckBoxVIC: TCheckBox
          AlignWithMargins = True
          Left = 168
          Top = 3
          Width = 105
          Height = 21
          Hint = 'Clientes priorit'#225'rios'
          Align = alRight
          Alignment = taLeftJustify
          Caption = 'Cliente Priorit'#225'rio'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object CheckBoxGrupoA: TCheckBox
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 68
          Height = 21
          Hint = 'Clientes priorit'#225'rios'
          Align = alLeft
          Alignment = taLeftJustify
          Caption = 'Grupo A'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object TabSheetEdicao: TTabSheet
      Caption = 'Curva'
      ImageIndex = 2
    end
    object TabSheetGrafico: TTabSheet
      Caption = 'Gr'#225'fico'
      ImageIndex = 2
    end
    object TabSheetQtdCons: TTabSheet
      Caption = 'Consumidores'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 276
        Height = 233
        Align = alClient
        Caption = 'Quantidade de Consumidores por Classe'
        TabOrder = 0
        object Label9: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 122
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'Rural'
          Layout = tlCenter
        end
        object Label3: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 29
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'Residencial'
          Layout = tlCenter
        end
        object Label7: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 60
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'Comercial'
          Layout = tlCenter
        end
        object Label10: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 153
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'Outros'
          Layout = tlCenter
        end
        object Label11: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 184
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'A4'
          Layout = tlCenter
        end
        object Label8: TLabel
          AlignWithMargins = True
          Left = 11
          Top = 91
          Width = 78
          Height = 18
          AutoSize = False
          Caption = 'Industrial'
          Layout = tlCenter
        end
        object EditNrur: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 122
          Width = 185
          Height = 21
          TabOrder = 0
        end
        object EditNind: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 91
          Width = 185
          Height = 21
          TabOrder = 1
        end
        object EditNres: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 29
          Width = 185
          Height = 21
          TabOrder = 2
        end
        object EditNout: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 153
          Width = 185
          Height = 21
          TabOrder = 3
        end
        object EditNa4: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 184
          Width = 185
          Height = 21
          TabOrder = 4
        end
        object EditNcom: TEdit
          AlignWithMargins = True
          Left = 88
          Top = 60
          Width = 185
          Height = 21
          TabOrder = 5
        end
      end
    end
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 284
    Height = 26
    AutoSize = True
    ButtonWidth = 71
    Caption = 'ToolBar1'
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList1
    List = True
    ShowCaptions = True
    TabOrder = 1
    object butConfirma: TToolButton
      Left = 0
      Top = 0
      Action = ActionConfirma
      AutoSize = True
      ParentShowHint = False
      ShowHint = True
    end
    object butCancela: TToolButton
      Left = 75
      Top = 0
      Action = ActionCancela
      AutoSize = True
      ParentShowHint = False
      ShowHint = True
    end
    object ButImporta: TToolButton
      Left = 148
      Top = 0
      Action = ActionImporta
      AutoSize = True
      ParentShowHint = False
      ShowHint = True
    end
    object butRetira: TToolButton
      Left = 217
      Top = 0
      Action = ActionRetira
      AutoSize = True
      ParentShowHint = False
      ShowHint = True
    end
  end
  object ImageList1: TImageList
    Left = 208
    Top = 24
    Bitmap = {
      494C010104000900AC0010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF0000000000000000007F7F7F00000000007F7F7F00000000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF007F7F7F00000000007F7F7F0000000000000000000000
      00000000FF000000FF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000007F7F7F00000000007F7F7F0000000000000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF00000000000000000000000000FFFFFF0000000000BDBD
      BD00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF000000FF00000000007F7F7F00000000007F7F7F0000000000000000000000
      FF000000FF0000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFFFDFFFFFFFFFF83FFFCF
      FE7FFFFFE00FFFC7FC3FE7F3CC470003F81FE3E384630001F19FF1C7A0730000
      E38FF88F31F90001E7CFFC1F38F90003FFC7FE3F3C790007FFE7FC1F3C39000F
      FFE3F88F3C19001FFFF3F1C79C0B007FFFF3E3E38C4300FFFFFFE7F3C46701FF
      FFFFFFFFE00F03FFFFFFFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionList1: TActionList
    Images = ImageList1
    Left = 248
    Top = 24
    object ActionCancela: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar edi'#231#227'o da carga'
      ImageIndex = 1
      OnExecute = ActionCancelaExecute
    end
    object ActionConfirma: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar edi'#231#227'o da carga'
      ImageIndex = 0
      OnExecute = ActionConfirmaExecute
    end
    object ActionRetira: TAction
      Caption = 'Retirar'
      Hint = 'Retirar carga da rede'
      ImageIndex = 2
      OnExecute = ActionRetiraExecute
    end
    object ActionImporta: TAction
      Caption = 'Importar'
      Hint = 'Importar curva de carga de arquivo texto'
      ImageIndex = 3
      Visible = False
      OnExecute = ActionImportaExecute
    end
  end
end