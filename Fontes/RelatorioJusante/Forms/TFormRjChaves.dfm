object FormRjChaves: TFormRjChaves
  Left = 0
  Top = 0
  Caption = 'FormRjChaves'
  ClientHeight = 201
  ClientWidth = 447
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object treeViewArvore: TTreeView
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 214
    Height = 195
    Align = alLeft
    Indent = 19
    TabOrder = 0
    OnClick = treeViewArvoreClick
  end
  object GroupBox1: TGroupBox
    AlignWithMargins = True
    Left = 223
    Top = 3
    Width = 221
    Height = 195
    Align = alClient
    Caption = 'Informa'#231#245'es'
    TabOrder = 1
    object vlistEditor: TValueListEditor
      Left = 2
      Top = 15
      Width = 217
      Height = 178
      Align = alClient
      TabOrder = 0
      ColWidths = (
        150
        61)
    end
  end
end
