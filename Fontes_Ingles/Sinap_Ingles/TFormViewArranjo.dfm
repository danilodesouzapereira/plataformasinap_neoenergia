object FormViewArranjo: TFormViewArranjo
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Arranjos da Rede'
  ClientHeight = 353
  ClientWidth = 150
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object CLBox: TCheckListBox
    Left = 0
    Top = 107
    Width = 150
    Height = 246
    OnClickCheck = CLBoxClickCheck
    Align = alClient
    BorderStyle = bsNone
    ItemHeight = 16
    Sorted = True
    Style = lbOwnerDrawFixed
    TabOrder = 0
    OnDblClick = CLBoxDblClick
    OnDrawItem = CLBoxDrawItem
    ExplicitTop = 97
    ExplicitHeight = 256
  end
  object GBoxAtributo: TGroupBox
    Left = 0
    Top = 0
    Width = 150
    Height = 107
    Align = alTop
    Caption = 'Exibir atributos dos trechos'
    TabOrder = 1
    object CheckBoxCodigo: TCheckBox
      Left = 7
      Top = 30
      Width = 130
      Height = 17
      Caption = 'C'#243'digo do arranjo'
      TabOrder = 0
      OnClick = CheckBoxClick
    end
    object CheckBoxZseq_ohm: TCheckBox
      Left = 7
      Top = 63
      Width = 122
      Height = 17
      Caption = 'Imped'#226'ncia (ohm)'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = CheckBoxClick
    end
    object CheckBoxSempre: TCheckBox
      Left = 7
      Top = 15
      Width = 122
      Height = 17
      Caption = 'Exibir sempre'
      TabOrder = 2
      OnClick = CheckBoxClick
    end
    object ToolBar1: TToolBar
      Left = 2
      Top = 82
      Width = 146
      Height = 23
      Align = alBottom
      ButtonHeight = 19
      ButtonWidth = 8
      Caption = 'ToolBar'
      EdgeBorders = [ebTop]
      ShowCaptions = True
      TabOrder = 3
      ExplicitTop = 72
      object butSelAll: TBitBtn
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
        ParentFont = False
        TabOrder = 0
        NumGlyphs = 2
      end
      object butSelNone: TBitBtn
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
        ParentFont = False
        TabOrder = 1
        NumGlyphs = 2
      end
    end
    object CheckBoxZseq_pu: TCheckBox
      Left = 7
      Top = 46
      Width = 122
      Height = 17
      Caption = 'Imped'#226'ncia (pu)'
      TabOrder = 4
      OnClick = CheckBoxClick
    end
  end
  object ColorDialog: TColorDialog
    Left = 8
    Top = 112
  end
  object ActionList: TActionList
    Left = 48
    Top = 112
    object ActionSelAll: TAction
      Caption = #254
      OnExecute = ActionSelAllExecute
    end
    object ActionSelNone: TAction
      Caption = 'o'
      OnExecute = ActionSelNoneExecute
    end
  end
end
