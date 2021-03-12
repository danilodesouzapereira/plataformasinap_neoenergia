object FormViewFase: TFormViewFase
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Fases dos Trechos'
  ClientHeight = 353
  ClientWidth = 149
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
    Top = 97
    Width = 149
    Height = 256
    OnClickCheck = CLBoxClickCheck
    Align = alClient
    BorderStyle = bsNone
    Sorted = True
    Style = lbOwnerDrawFixed
    TabOrder = 0
    OnDblClick = CLBoxDblClick
    OnDrawItem = CLBoxDrawItem
  end
  object GBoxAtributo: TGroupBox
    Left = 0
    Top = 0
    Width = 149
    Height = 97
    Align = alTop
    Caption = 'Exibir atributos dos trechos'
    TabOrder = 1
    object CheckBoxCodigo: TCheckBox
      Left = 7
      Top = 46
      Width = 114
      Height = 17
      Caption = 'C'#243'digo do arranjo'
      TabOrder = 0
      OnClick = CheckBoxClick
    end
    object CheckBoxFase: TCheckBox
      Left = 7
      Top = 30
      Width = 114
      Height = 17
      Caption = 'Fases'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = CheckBoxClick
    end
    object CheckBoxSempre: TCheckBox
      Left = 7
      Top = 15
      Width = 114
      Height = 17
      Caption = 'Exibir sempre'
      TabOrder = 2
      OnClick = CheckBoxClick
    end
    object ToolBar1: TToolBar
      Left = 2
      Top = 72
      Width = 145
      Height = 23
      Align = alBottom
      ButtonHeight = 19
      ButtonWidth = 8
      Caption = 'ToolBar1'
      ShowCaptions = True
      TabOrder = 3
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
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 0
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
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 1
      end
    end
  end
  object ColorDialog: TColorDialog
    Left = 16
    Top = 104
  end
  object ActionList: TActionList
    Left = 64
    Top = 104
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
