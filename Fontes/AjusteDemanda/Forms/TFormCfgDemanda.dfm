object FormCfgDemanda: TFormCfgDemanda
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Configura'#231#227'o de Ajuste de Demanda'
  ClientHeight = 237
  ClientWidth = 499
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
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 499
    Height = 26
    ButtonWidth = 74
    EdgeBorders = [ebBottom]
    Images = ImageList1
    List = True
    ShowCaptions = True
    TabOrder = 0
    Wrapable = False
    object ButCancela: TToolButton
      Left = 0
      Top = 0
      Action = ActionCancelar
      AutoSize = True
    end
    object ButConfirma: TToolButton
      Left = 73
      Top = 0
      Action = ActionConfirmar
      AutoSize = True
    end
  end
  object GroupBox1: TGroupBox
    Left = 354
    Top = 26
    Width = 145
    Height = 211
    Align = alRight
    Caption = 'Patamares'
    TabOrder = 1
    object ToolBar2: TToolBar
      Left = 2
      Top = 15
      Width = 141
      Height = 27
      ButtonHeight = 19
      Caption = 'ToolBarRede'
      EdgeBorders = [ebTop, ebBottom]
      TabOrder = 0
      object ButSubestacaoSelAll: TBitBtn
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
      object ButSubestacaoSelNone: TBitBtn
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
    object CLBoxPatamares: TCheckListBox
      Left = 2
      Top = 42
      Width = 141
      Height = 167
      Align = alClient
      ItemHeight = 13
      Items.Strings = (
        '00:00 a 01:00'
        '01:00 a 02:00'
        '02:00 a 03:00'
        '03:00 a 04:00'
        '04:00 a 05:00'
        '05:00 a 06:00'
        '06:00 a 07:00'
        '07:00 a 08:00'
        '08:00 a 09:00'
        '09:00 a 10:00'
        '10:00 a 11:00'
        '11:00 a 12:00'
        '12:00 a 13:00'
        '13:00 a 14:00'
        '14:00 a 15:00'
        '15:00 a 16:00'
        '16:00 a 17:00'
        '17:00 a 18:00'
        '18:00 a 19:00'
        '19:00 a 20:00'
        '20:00 a 21:00'
        '21:00 a 22:00'
        '22:00 a 23:00'
        '23:00 a 00:00')
      TabOrder = 1
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 26
    Width = 354
    Height = 211
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object GBoxLimite: TGroupBox
      Left = 0
      Top = 111
      Width = 354
      Height = 50
      Align = alBottom
      Caption = 
        'Diferen'#231'a admiss'#237'vel entre demanda da medi'#231#227'o e demanda calculad' +
        'a'
      TabOrder = 0
      ExplicitLeft = -4
      ExplicitTop = 110
      object Label4: TLabel
        Left = 182
        Top = 23
        Width = 102
        Height = 13
        Caption = 'Depois do ajuste (%)'
      end
      object Label5: TLabel
        Left = 9
        Top = 23
        Width = 98
        Height = 13
        Caption = 'Antes do ajuste (%)'
      end
      object CBoxAjusteMax: TComboBox
        Left = 112
        Top = 20
        Width = 49
        Height = 21
        DropDownCount = 10
        ItemIndex = 4
        TabOrder = 0
        Text = '  50'
        Items.Strings = (
          '  10'
          '  20'
          '  30'
          '  40'
          '  50'
          '  60'
          '  70'
          '  80'
          '  90'
          '100')
      end
      object CBoxErroMax: TComboBox
        Left = 290
        Top = 20
        Width = 47
        Height = 21
        DropDownCount = 10
        TabOrder = 1
        Text = '  2'
        Items.Strings = (
          '  1'
          '  2'
          '  3'
          '  4'
          '  5'
          '  6'
          '  7'
          '  8'
          '  9'
          '10')
      end
    end
    object GBoxOpcao: TGroupBox
      Left = 0
      Top = 0
      Width = 354
      Height = 111
      Align = alClient
      Caption = 'Op'#231#245'es do ajuste'
      TabOrder = 1
      object CheckBoxConsA4: TCheckBox
        Left = 9
        Top = 31
        Width = 216
        Height = 22
        Caption = 'Incluir consumidores prim'#225'rios (grupo A) '
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 0
      end
      object CheckBoxRedeEquilibrada: TCheckBox
        Left = 9
        Top = 15
        Width = 164
        Height = 17
        Caption = 'Considerar rede equilibrada'
        TabOrder = 1
      end
      object CheckBoxLimite: TCheckBox
        Left = 9
        Top = 91
        Width = 248
        Height = 13
        Caption = 'Limitar diferen'#231'a entre medi'#231#227'o e demanda'
        TabOrder = 2
        OnClick = CheckBoxLimiteClick
      end
      object CheckBoxPerdaFe: TCheckBox
        Left = 9
        Top = 54
        Width = 248
        Height = 13
        Caption = 'Considerar perda no ferro para o fluxo'
        Checked = True
        State = cbChecked
        TabOrder = 3
        OnClick = CheckBoxLimiteClick
      end
      object CheckBoxTempo: TCheckBox
        Left = 9
        Top = 72
        Width = 248
        Height = 13
        Caption = 'Limitar tempo para ajustar a demanda'
        TabOrder = 4
        OnClick = CheckBoxTempoClick
      end
    end
    object GBoxTempo: TGroupBox
      Left = 0
      Top = 161
      Width = 354
      Height = 50
      Align = alBottom
      Caption = 'Tempo m'#225'ximo para execu'#231#227'o do ajuste de demanda'
      TabOrder = 2
      object Label1: TLabel
        Left = 73
        Top = 23
        Width = 37
        Height = 13
        Caption = 'minutos'
      end
      object CboxTempo: TComboBox
        Left = 9
        Top = 20
        Width = 49
        Height = 21
        DropDownCount = 10
        ItemIndex = 2
        TabOrder = 0
        Text = ' 4'
        Items.Strings = (
          ' 2'
          ' 3'
          ' 4'
          ' 5'
          ' 6'
          ' 7 '
          ' 8'
          ' 9'
          '10 ')
      end
    end
  end
  object ActionList1: TActionList
    Images = ImageList1
    Left = 246
    Top = 10
    object ActionCancelar: TAction
      Caption = 'Cancelar'
      ImageIndex = 1
      OnExecute = ActionCancelarExecute
    end
    object ActionConfirmar: TAction
      Caption = 'Confirmar'
      ImageIndex = 0
      OnExecute = ActionConfirmarExecute
    end
    object ActionSelAll: TAction
      Caption = #254
      OnExecute = ActionSelAllExecute
    end
    object ActionSelNone: TAction
      Caption = 'o'
      OnExecute = ActionSelNoneExecute
    end
  end
  object ImageList1: TImageList
    Left = 192
    Top = 8
    Bitmap = {
      494C010102000400700010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
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
      0000000000000000000000000000008400000084000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000840000008400000084000000840000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      8400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000000000000000000000000000000000000000000000000000
      0000000084000000840000000000000000000000000000000000000000000000
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
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFF00000000FFFFFFFF00000000
      FE7FFFFF00000000FC3FE7F300000000F81FE3E300000000F19FF1C700000000
      E38FF88F00000000E7CFFC1F00000000FFC7FE3F00000000FFE7FC1F00000000
      FFE3F88F00000000FFF3F1C700000000FFF3E3E300000000FFFFE7F300000000
      FFFFFFFF00000000FFFFFFFF0000000000000000000000000000000000000000
      000000000000}
  end
end