object FormMostraDefeito: TFormMostraDefeito
  Left = 0
  Top = 0
  Caption = 'Tratamento da falha'
  ClientHeight = 291
  ClientWidth = 225
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClick = ActionMostraBlocoDefeitoExecute
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter5: TSplitter
    Left = 0
    Top = 141
    Width = 225
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 132
  end
  object Splitter1: TSplitter
    Left = 0
    Top = 253
    Width = 225
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 261
  end
  object tvVisDefeito: TTreeView
    Left = 0
    Top = 256
    Width = 225
    Height = 35
    Align = alBottom
    Indent = 19
    TabOrder = 0
    Visible = False
    OnClick = ActionMostraDefeitoVisExecute
  end
  object tvDefeito: TTreeView
    Left = 0
    Top = 0
    Width = 225
    Height = 141
    Align = alClient
    HideSelection = False
    Indent = 19
    TabOrder = 1
    OnClick = ActionMostraDefeitoExecute
  end
  object tvDefeito1: TTreeView
    Left = 0
    Top = 144
    Width = 225
    Height = 109
    Align = alBottom
    Indent = 19
    TabOrder = 2
    Visible = False
    OnClick = ActionMostraDefeito1Execute
  end
  object ActionList1: TActionList
    Left = 152
    Top = 240
    object ActionMostraDefeito: TAction
      Caption = 'ActionMostraDefeito'
      OnExecute = ActionMostraDefeitoExecute
    end
    object ActionMostraDefeitoVis: TAction
      Caption = 'ActionMostraDefeitoVis'
      OnExecute = ActionMostraDefeitoVisExecute
    end
    object ActionMostraBlocoDefeito: TAction
      Caption = 'ActionMostraBlocoDefeito'
      OnExecute = ActionMostraBlocoDefeitoExecute
    end
    object ActionMostraDefeito1: TAction
      Caption = 'ActionMostraDefeito1'
      OnExecute = ActionMostraDefeito1Execute
    end
  end
end
