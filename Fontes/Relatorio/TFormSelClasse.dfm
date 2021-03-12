object FormSelClasse: TFormSelClasse
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Sele'#231#227'o de Consumidor'
  ClientHeight = 244
  ClientWidth = 258
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object CLBoxClasse: TCheckListBox
    Left = 0
    Top = 27
    Width = 258
    Height = 217
    OnClickCheck = CLBoxClasseClickCheck
    Align = alClient
    BorderStyle = bsNone
    Color = clSkyBlue
    ItemHeight = 13
    TabOrder = 0
    ExplicitTop = 0
    ExplicitHeight = 244
  end
  object ToolBarRede: TToolBar
    Left = 0
    Top = 0
    Width = 258
    Height = 27
    ButtonHeight = 19
    Caption = 'ToolBarRede'
    EdgeBorders = [ebTop, ebBottom]
    TabOrder = 1
    ExplicitLeft = 2
    ExplicitTop = 15
    ExplicitWidth = 172
    object butSelAllCirc: TBitBtn
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
    object butSelNoneCirc: TBitBtn
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
  object ActionList: TActionList
    Left = 16
    Top = 32
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
