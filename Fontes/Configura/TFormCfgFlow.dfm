object formCfgFlow: TformCfgFlow
  Left = 307
  Top = 120
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'Par'#226'metros do fluxo'
  ClientHeight = 295
  ClientWidth = 436
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbCarga: TGroupBox
    Left = 6
    Top = 28
    Width = 170
    Height = 90
    Caption = 'Modelagem de carga (%)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lblPotCon: TLabel
      Left = 6
      Top = 21
      Width = 92
      Height = 13
      Caption = 'Pot'#234'ncia constante'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblCorConst: TLabel
      Left = 6
      Top = 43
      Width = 90
      Height = 13
      Caption = 'Corrente constante'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblImpConst: TLabel
      Left = 6
      Top = 65
      Width = 105
      Height = 13
      Caption = 'Imped'#226'ncia constante'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object edtImpCon: TEdit
      Left = 117
      Top = 63
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object edtCorCon: TEdit
      Left = 117
      Top = 41
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object edtPotCon: TEdit
      Left = 117
      Top = 19
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object gbPrecisao: TGroupBox
    Left = 6
    Top = 121
    Width = 171
    Height = 60
    Caption = 'Precis'#227'o'
    Color = clBtnFace
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 1
    object lblIterMax: TLabel
      Left = 8
      Top = 17
      Width = 106
      Height = 13
      Caption = 'N'#250'm.m'#225'ximo itera'#231#245'es'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblToler: TLabel
      Left = 6
      Top = 35
      Width = 106
      Height = 13
      Caption = 'Toler'#226'ncia tens'#227'o (pu)'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object edtIterMax: TEdit
      Left = 116
      Top = 11
      Width = 40
      Height = 21
      BiDiMode = bdLeftToRight
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 0
    end
    object edtToler: TEdit
      Left = 116
      Top = 33
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
  end
  object gbFaixaI: TGroupBox
    Left = 6
    Top = 185
    Width = 255
    Height = 102
    Caption = 'Faixas de corrente (% cor.adm.cabo)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label9: TLabel
      Left = 9
      Top = 36
      Width = 35
      Height = 13
      Caption = 'M'#237'nima'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 9
      Top = 58
      Width = 36
      Height = 13
      Caption = 'M'#225'xima'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object edtI2: TEdit
      Left = 50
      Top = 54
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnExit = ActionExibeLblExecute
    end
    object edtI1: TEdit
      Left = 50
      Top = 32
      Width = 40
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnExit = ActionExibeLblExecute
    end
    object gbGrafI: TGroupBox
      Left = 107
      Top = 17
      Width = 142
      Height = 73
      Caption = 'Representa'#231#227'o gr'#225'fica'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object lblI1: TLabel
        Left = 7
        Top = 18
        Width = 70
        Height = 13
        Caption = 'abaixo de 0,94'
      end
      object lblI3: TLabel
        Left = 7
        Top = 55
        Width = 67
        Height = 13
        Caption = 'acima de 1,04'
      end
      object lblI2: TLabel
        Left = 7
        Top = 37
        Width = 81
        Height = 13
        Caption = 'entre 0,99 e 0,88'
      end
      object panCorI1: TPanel
        Left = 108
        Top = 19
        Width = 21
        Height = 12
        TabOrder = 0
      end
      object panCorI2: TPanel
        Left = 108
        Top = 37
        Width = 21
        Height = 12
        TabOrder = 1
      end
      object panCorI3: TPanel
        Left = 108
        Top = 55
        Width = 21
        Height = 12
        TabOrder = 2
      end
    end
  end
  object gbFaixaV: TGroupBox
    Left = 183
    Top = 28
    Width = 247
    Height = 153
    Caption = 'Faixas de tens'#227'o (pu)'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object gbFaixaA: TGroupBox
      Left = 5
      Top = 16
      Width = 91
      Height = 62
      Caption = 'Faixa A'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Label7: TLabel
        Left = 6
        Top = 18
        Width = 35
        Height = 13
        Caption = 'M'#237'nima'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 6
        Top = 39
        Width = 36
        Height = 13
        Caption = 'M'#225'xima'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edtV1: TEdit
        Left = 45
        Top = 14
        Width = 40
        Height = 21
        TabOrder = 0
        OnExit = ActionExibeLblExecute
      end
      object edtV2: TEdit
        Left = 45
        Top = 35
        Width = 40
        Height = 21
        TabOrder = 1
        OnExit = ActionExibeLblExecute
      end
    end
    object gbFaixaB: TGroupBox
      Left = 6
      Top = 83
      Width = 90
      Height = 62
      Caption = 'Faixa B'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object Label2: TLabel
        Left = 5
        Top = 18
        Width = 35
        Height = 13
        Caption = 'M'#237'nima'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 5
        Top = 39
        Width = 36
        Height = 13
        Caption = 'M'#225'xima'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edtV3: TEdit
        Left = 44
        Top = 14
        Width = 40
        Height = 21
        TabOrder = 0
        OnExit = ActionExibeLblExecute
      end
      object edtV4: TEdit
        Left = 44
        Top = 35
        Width = 40
        Height = 21
        TabOrder = 1
        OnExit = ActionExibeLblExecute
      end
    end
    object gbGrafV: TGroupBox
      Left = 100
      Top = 17
      Width = 141
      Height = 129
      Caption = 'Representa'#231#227'o gr'#225'fica'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object lblV1: TLabel
        Left = 7
        Top = 18
        Width = 70
        Height = 13
        Caption = 'abaixo de 0,94'
      end
      object lblV2: TLabel
        Left = 7
        Top = 40
        Width = 81
        Height = 13
        Caption = 'entre 0,94 e 0,97'
      end
      object lblV5: TLabel
        Left = 7
        Top = 104
        Width = 67
        Height = 13
        Caption = 'acima de 1,04'
      end
      object lblV3: TLabel
        Left = 7
        Top = 61
        Width = 81
        Height = 13
        Caption = 'entre 0,97 e 1,02'
      end
      object lblV4: TLabel
        Left = 7
        Top = 83
        Width = 81
        Height = 13
        Caption = 'entre 1,02 e 1,04'
      end
      object panCorV1: TPanel
        Left = 108
        Top = 18
        Width = 21
        Height = 12
        TabOrder = 0
      end
      object panCorV2: TPanel
        Left = 108
        Top = 40
        Width = 21
        Height = 12
        TabOrder = 1
      end
      object panCorV3: TPanel
        Left = 108
        Top = 61
        Width = 21
        Height = 12
        TabOrder = 2
      end
      object panCorV4: TPanel
        Left = 108
        Top = 83
        Width = 21
        Height = 12
        TabOrder = 3
      end
      object panCorV5: TPanel
        Left = 108
        Top = 104
        Width = 21
        Height = 12
        TabOrder = 4
      end
    end
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 436
    Height = 25
    ButtonWidth = 71
    Caption = 'ToolBar1'
    EdgeBorders = [ebBottom]
    Flat = True
    Images = ImageList
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 4
    object butDefault: TToolButton
      Left = 0
      Top = 0
      Action = ActionDefault
    end
    object butInclui: TToolButton
      Left = 71
      Top = 0
      Action = ActionConfirma
      AutoSize = True
    end
    object butSair: TToolButton
      Left = 146
      Top = 0
      Hint = 'Sair'
      Action = ActionCancela
      AutoSize = True
    end
  end
  object ActionList1: TActionList
    Left = 639
    Top = 27
    object ActionExibeLbl: TAction
      OnExecute = ActionExibeLblExecute
    end
  end
  object ImageList: TImageList
    Left = 280
    Top = 8
    Bitmap = {
      494C010103000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      0000000000000031CE000031CE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000084000000840000008400000084000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      00000031CE000031CE000031CE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000840000000000000000000000840000008400000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000031
      CE000031CE000031CE0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000840000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000000000000031CE000031
      CE000031CE0000000000000000000031CE000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000008400000084
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE000000000000000000000000000031CE000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000084000000840000008400000000000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE0000000000000000000000000000000000000000000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000008400000084000000840000008400000084000000
      00000000000000000000000000000000000000000000000000000031CE000031
      CE00000000000000000000000000000000000031CE000031CE000031CE000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000084
      0000008400000084000000000000000000000000000000000000000000000000
      0000000000000000840000008400000084000000000000008400000084000000
      84000000000000000000000000000000000000000000000000000031CE000031
      CE000000000000000000000000000031CE000031CE000031CE00000000000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      0000000084000000840000008400000000000000000000000000000084000000
      8400000084000000000000000000000000000000000000000000000000000031
      CE000031CE000031CE000031CE000031CE000031CE0000000000000000000031
      CE000031CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000008400000084000000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      8400000084000000840000000000000000000000000000000000000000000000
      00000031CE000031CE000031CE000031CE000000000000000000000000000000
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
      000000000000000000000000FFFFFF00FFFFFFFFFFFF0000FFFFFFFFFFFF0000
      FE7FFFFFFFFF0000FC3FE7F3F9FF0000F81FE3E3F1FF0000F19FF1C7E3FF0000
      E38FF88FC6070000E7CFFC1FCE070000FFC7FE3FCF870000FFE7FC1FCF070000
      FFE3F88FCE270000FFF3F1C7E0670000FFF3E3E3F0FF0000FFFFE7F3FFFF0000
      FFFFFFFFFFFF0000FFFFFFFFFFFF000000000000000000000000000000000000
      000000000000}
  end
  object ActionList2: TActionList
    Images = ImageList
    Left = 248
    Top = 8
    object ActionConfirma: TAction
      Caption = 'Confirmar'
      Hint = 'Confirmar sele'#231#227'o de SEs'
      ImageIndex = 0
      OnExecute = ActionConfirmaExecute
    end
    object ActionCancela: TAction
      Caption = 'Cancelar'
      Hint = 'Cancelar sele'#231#227'o de SEs'
      ImageIndex = 1
      OnExecute = ActionCancelaExecute
    end
    object ActionDefault: TAction
      Caption = 'Redefinir'
      Hint = 'Redefine valores padr'#227'o'
      ImageIndex = 2
      OnExecute = ActionDefaultExecute
    end
  end
end
