object FormMT_AIS: TFormMT_AIS
  Left = 445
  Top = 191
  BorderIcons = [biSystemMenu]
  Caption = #193'rea de Influ'#234'ncia de Subesta'#231#227'o'
  ClientHeight = 380
  ClientWidth = 288
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 288
    Height = 26
    AutoSize = True
    ButtonWidth = 60
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object butFecha: TToolButton
      Left = 0
      Top = 0
      Action = ActionFecha
      AutoSize = True
    end
    object ToolButton2: TToolButton
      Left = 64
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 6
      Style = tbsSeparator
    end
    object ToolButton1: TToolButton
      Left = 72
      Top = 0
      Action = ActionHelp
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 206
    Width = 288
    Height = 174
    Align = alClient
    Caption = 'Subesta'#231#245'es'
    TabOrder = 1
    object ToolBarAIS: TToolBar
      Left = 2
      Top = 15
      Width = 284
      Height = 27
      ButtonHeight = 19
      Caption = 'ToolBarAIS'
      EdgeBorders = [ebTop, ebBottom]
      TabOrder = 0
      object ButSelAll: TBitBtn
        Left = 0
        Top = 0
        Width = 17
        Height = 19
        Action = ActionSelAll
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
      object ButSelNone: TBitBtn
        Left = 17
        Top = 0
        Width = 17
        Height = 19
        Action = ActionSelNone
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
    object LViewAis: TListView
      Left = 2
      Top = 42
      Width = 284
      Height = 130
      Align = alClient
      BorderStyle = bsNone
      Checkboxes = True
      Columns = <
        item
          AutoSize = True
          Caption = 'Subesta'#231#227'o'
        end
        item
          Alignment = taRightJustify
          Caption = 'Trafos'
          Width = 42
        end
        item
          Alignment = taRightJustify
          Caption = 'Snom(MVA)'
          Width = 70
        end
        item
          Alignment = taRightJustify
          Caption = 'Sfirme(MVA)'
          Width = 70
        end
        item
          Caption = 'Cor'
          Width = 28
        end>
      GridLines = True
      ReadOnly = True
      RowSelect = True
      SortType = stText
      TabOrder = 1
      ViewStyle = vsReport
      OnClick = LViewAisClick
      OnCustomDrawSubItem = LViewAisCustomDrawSubItem
      OnDblClick = LViewAisDblClick
    end
  end
  object RGroupCarga: TRadioGroup
    Left = 0
    Top = 26
    Width = 288
    Height = 52
    Align = alTop
    Caption = 'Abrang'#234'ncia'
    ItemIndex = 1
    Items.Strings = (
      'Cargas que podem ser atendidas pela subesta'#231#227'o'
      'Cargas atualmente atendidas pela subesta'#231#227'o')
    TabOrder = 2
    OnClick = ActionOptionChangeExecute
  end
  object PanelOpcao: TPanel
    Left = 0
    Top = 78
    Width = 288
    Height = 128
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    object RGroupCapacidade: TRadioGroup
      Left = 0
      Top = 0
      Width = 288
      Height = 40
      Align = alTop
      Caption = 'Capacidade das Subesta'#231#245'es'
      Columns = 2
      ItemIndex = 0
      Items.Strings = (
        'Pot'#234'ncia nominal'
        'Pot'#234'ncia firme')
      TabOrder = 0
      OnClick = ActionOptionChangeExecute
    end
    object GBoxCargtoMaximo: TGroupBox
      Left = 0
      Top = 40
      Width = 288
      Height = 41
      Align = alTop
      Caption = 'Carregamento M'#225'ximo (%)'
      TabOrder = 1
      object CBoxCargto: TComboBox
        Left = 19
        Top = 14
        Width = 50
        Height = 21
        TabOrder = 0
        Text = '100'
        OnChange = ActionOptionChangeExecute
        Items.Strings = (
          '70'
          '80'
          '90'
          '100'
          '110'
          '120'
          '130')
      end
    end
    object GBoxPatamar: TGroupBox
      Left = 0
      Top = 81
      Width = 288
      Height = 45
      Align = alTop
      Caption = 'Patamar'
      TabOrder = 2
      object CBoxPatamar: TComboBox
        Left = 7
        Top = 15
        Width = 149
        Height = 21
        Style = csDropDownList
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = ActionOptionChangeExecute
      end
      object BitBtn1: TBitBtn
        Left = 177
        Top = 15
        Width = 56
        Height = 21
        Action = ActionAnimacao
        Caption = 'Iniciar'
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          3333333333FFFFF3333333333000003333333333F77777FFF333333009999900
          3333333777777777FF33330998FFF899033333777333F3777FF33099FFFCFFF9
          903337773337333777F3309FFFFFFFCF9033377333F3337377FF098FF0FFFFFF
          890377F3373F3333377F09FFFF0FFFFFF90377F3F373FFFFF77F09FCFFF90000
          F90377F733377777377F09FFFFFFFFFFF90377F333333333377F098FFFFFFFFF
          890377FF3F33333F3773309FCFFFFFCF9033377F7333F37377F33099FFFCFFF9
          90333777FF37F3377733330998FCF899033333777FF7FF777333333009999900
          3333333777777777333333333000003333333333377777333333}
        NumGlyphs = 2
        TabOrder = 1
      end
    end
  end
  object ImageList: TImageList
    Left = 56
    Top = 280
    Bitmap = {
      494C010106000900180010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF0000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000FFFFFF000000000000000000FFFFFF000000
      00000000000000000000FFFFFF000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF0000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000BFBFBF00BFBFBF00FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000BFBFBF00BFBFBF00000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF0000000000000000007F7F7F00000000007F7F7F00000000000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF00000000000000
      FF000000FF000000FF007F7F7F00000000007F7F7F0000000000000000000000
      00000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000007F7F7F00000000007F7F7F0000000000000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000800000000000000080000000FF00000000000000
      0000000000000000FF000000FF000000000031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C0031319C0031319C0031319C003131
      9C0031319C0031319C0031319C0031319C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000FF000000FF00000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      0000000000000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      840000008400000000000000000000000000000000000000FF000000FF000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF00000000000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      840000008400000084000000000000000000000000000000FF000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF000000
      FF000000FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      00000000840000008400000000000000000000000000000000000000FF000000
      FF000000FF00000000007F7F7F00000000007F7F7F0000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      000000000000000000000000FFFFFF00FFFFFFFF000000000000F83F00000000
      0000E00F000000000000C0070000000000008003000000000000800300000000
      0000000100000000000000010000000000000001000000000000000100000000
      0000000100000000000080030000000000008003000000000000C00700000000
      FFFFE00F00000000FFFFF83F00000000FFFFFFFFFFFFFFFFFFFFFFFFF83FFFFF
      FE7FFFFFE00FFFFFFC3FE7F3CC47FFFFF81FE3E38463FFFFF19FF1C7A073FFFF
      E38FF88F31F9FFFFE7CFFC1F38F90000FFC7FE3F3C790000FFE7FC1F3C390000
      FFE3F88F3C19FFFFFFF3F1C79C0BFFFFFFF3E3E38C43FFFFFFFFE7F3C467FFFF
      FFFFFFFFE00FFFFFFFFFFFFFF83FFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 24
    Top = 280
    object ActionFecha: TAction
      Caption = 'Fechar'
      ImageIndex = 1
      OnExecute = ActionFechaExecute
    end
    object ActionSelAll: TAction
      Caption = #254
      OnExecute = ActionSelAllExecute
    end
    object ActionSelNone: TAction
      Caption = 'o'
      OnExecute = ActionSelNoneExecute
    end
    object ActionAnimacao: TAction
      Caption = 'Iniciar'
      OnExecute = ActionAnimacaoExecute
    end
    object ActionOptionChange: TAction
      OnExecute = ActionOptionChangeExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      ImageIndex = 5
      OnExecute = ActionHelpExecute
    end
  end
  object ColorDialog: TColorDialog
    Left = 88
    Top = 280
  end
  object Timer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = TimerTimer
    Left = 120
    Top = 280
  end
end