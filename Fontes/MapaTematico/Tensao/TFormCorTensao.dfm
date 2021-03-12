object FormCorTensao: TFormCorTensao
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsNone
  ClientHeight = 158
  ClientWidth = 184
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object tabControlNivelV: TTabControl
    Left = 0
    Top = 0
    Width = 184
    Height = 158
    Align = alClient
    TabOrder = 0
    Tabs.Strings = (
      'BT'
      'MT'
      'AT')
    TabIndex = 0
    OnChange = tabControlNivelVChange
    object pnlV1: TPanel
      Left = 4
      Top = 128
      Width = 176
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object lblV1: TLabel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 126
        Height = 20
        Align = alLeft
        AutoSize = False
        Caption = 'abaixo de 0,92'
        Layout = tlCenter
      end
      object PanCorV1: TPanel
        AlignWithMargins = True
        Left = 135
        Top = 3
        Width = 38
        Height = 20
        Align = alClient
        ParentBackground = False
        TabOrder = 0
        OnClick = PanCorV1Click
      end
    end
    object pnlV2: TPanel
      Left = 4
      Top = 102
      Width = 176
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object lblV2: TLabel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 126
        Height = 20
        Align = alLeft
        AutoSize = False
        Caption = 'entre 0,92 e 0,97'
        Layout = tlCenter
      end
      object PanCorV2: TPanel
        AlignWithMargins = True
        Left = 135
        Top = 3
        Width = 38
        Height = 20
        Align = alClient
        ParentBackground = False
        TabOrder = 0
        OnClick = PanCorV2Click
      end
    end
    object pnlV3: TPanel
      Left = 4
      Top = 76
      Width = 176
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 2
      object lblV3: TLabel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 126
        Height = 20
        Align = alLeft
        AutoSize = False
        Caption = 'entre 0,97 e 1,02'
        Layout = tlCenter
      end
      object PanCorV3: TPanel
        AlignWithMargins = True
        Left = 135
        Top = 3
        Width = 38
        Height = 20
        Align = alClient
        ParentBackground = False
        TabOrder = 0
        OnClick = PanCorV3Click
      end
    end
    object pnlV4: TPanel
      Left = 4
      Top = 50
      Width = 176
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 3
      object lblV4: TLabel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 126
        Height = 20
        Align = alLeft
        AutoSize = False
        Caption = 'entre 1,02 e 1,08'
        Layout = tlCenter
      end
      object PanCorV4: TPanel
        AlignWithMargins = True
        Left = 135
        Top = 3
        Width = 38
        Height = 20
        Align = alClient
        ParentBackground = False
        TabOrder = 0
        OnClick = PanCorV4Click
      end
    end
    object pnlV5: TPanel
      Left = 4
      Top = 24
      Width = 176
      Height = 26
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 4
      object lblV5: TLabel
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 126
        Height = 20
        Align = alLeft
        AutoSize = False
        Caption = 'acima de 1,08'
        Layout = tlCenter
      end
      object PanCorV5: TPanel
        AlignWithMargins = True
        Left = 135
        Top = 3
        Width = 38
        Height = 20
        Align = alClient
        ParentBackground = False
        TabOrder = 0
        OnClick = PanCorV5Click
      end
    end
  end
  object ColorDialog: TColorDialog
    Left = 104
    Top = 65534
  end
end
