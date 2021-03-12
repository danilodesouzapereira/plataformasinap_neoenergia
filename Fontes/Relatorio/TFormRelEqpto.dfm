object FormRelEqpto: TFormRelEqpto
  Left = 358
  Top = 173
  BorderIcons = [biSystemMenu]
  Caption = 'Relat'#243'rio dos Equipamentos'
  ClientHeight = 415
  ClientWidth = 893
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 893
    Height = 34
    AutoSize = True
    ButtonHeight = 30
    ButtonWidth = 195
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList
    List = True
    ParentShowHint = False
    ShowCaptions = True
    ShowHint = True
    TabOrder = 0
    Wrapable = False
    object ButRelatorio: TToolButton
      Left = 0
      Top = 0
      AutoSize = True
      Caption = 'Fluxo de Pot'#234'ncia: 00:00 '#224's 23:00'
      DropdownMenu = PopupMenuRelatorio
      Style = tbsDropDown
    end
    object ToolButton1: TToolButton
      Left = 194
      Top = 0
      Width = 11
      Caption = 'ToolButton1'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object butRelRede: TToolButton
      Left = 205
      Top = 0
      Action = ActionRelRede
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelBarra: TToolButton
      Left = 242
      Top = 0
      Action = ActionRelBarra
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelCapacitor: TToolButton
      Left = 278
      Top = 0
      Action = ActionRelCapacitor
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelChave: TToolButton
      Left = 334
      Top = 0
      Action = ActionRelChave
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelCarga: TToolButton
      Left = 376
      Top = 0
      Action = ActionRelCarga
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelGerador: TToolButton
      Left = 415
      Top = 0
      Action = ActionRelGerador
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelReator: TToolButton
      Left = 464
      Top = 0
      Action = ActionRelReator
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelRegulador: TToolButton
      Left = 507
      Top = 0
      Action = ActionRelRegulador
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelSuprimento: TToolButton
      Left = 567
      Top = 0
      Action = ActionRelSuprimento
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelTrafo: TToolButton
      Left = 631
      Top = 0
      Action = ActionRelTrafo
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelTrafo3E: TToolButton
      Left = 667
      Top = 0
      Action = ActionRelTrafo3E
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelTrecho: TToolButton
      Left = 716
      Top = 0
      Action = ActionRelTrecho
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object butRelPerfilCarga: TToolButton
      Left = 761
      Top = 0
      Action = ActionRelPerfilCarga
      AutoSize = True
      Grouped = True
      Style = tbsCheck
    end
    object ToolButton2: TToolButton
      Left = 826
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object ToolButton3: TToolButton
      Left = 834
      Top = 0
      Action = ActionHelp
      AutoSize = True
    end
  end
  object panelRel: TPanel
    Left = 0
    Top = 34
    Width = 893
    Height = 381
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
  end
  object ActionList1: TActionList
    Images = ImageList
    Left = 32
    Top = 80
    object ActionRelBarra: TAction
      Caption = 'Barra'
      Hint = 'Exibir relat'#243'rio de barras'
      OnExecute = ActionRelBarraExecute
    end
    object ActionRelCapacitor: TAction
      Caption = 'Capacitor'
      OnExecute = ActionRelCapacitorExecute
    end
    object ActionRelCarga: TAction
      Caption = 'Carga'
      Hint = 'Exibir relat'#243'rio de cargas'
      OnExecute = ActionRelCargaExecute
    end
    object ActionRelChave: TAction
      Caption = 'Chave'
      Hint = 'Exibir relat'#243'rio de chaves'
      OnExecute = ActionRelChaveExecute
    end
    object ActionRelEqpto: TAction
      Caption = 'Equipamento (atributos)'
      OnExecute = ActionRelEqptoExecute
    end
    object ActionRelFluxo: TAction
      Caption = 'Fluxo de Pot'#234'ncia'
      OnExecute = ActionRelFluxoExecute
    end
    object ActionRelFluxoBarra: TAction
      Caption = 'Barra'
      Hint = 'Exibir relat'#243'rio de fluxo de pot'#234'ncia'
      OnExecute = ActionRelFluxoBarraExecute
    end
    object ActionRelFluxoCapacitor: TAction
      Caption = 'Capacitor'
      Enabled = False
    end
    object ActionRelFluxoCarga: TAction
      Caption = 'Carga'
      Enabled = False
    end
    object ActionRelFluxoChave: TAction
      Caption = 'Chave'
      Enabled = False
    end
    object ActionRelFluxoGerador: TAction
      Caption = 'Gerador'
      Enabled = False
    end
    object ActionRelFluxoPerfilCarga: TAction
      Caption = 'Perfil Carga'
      Enabled = False
      Visible = False
    end
    object ActionRelFluxoReator: TAction
      Caption = 'Reator'
      Enabled = False
    end
    object ActionRelFluxoRede: TAction
      Caption = 'Rede'
      OnExecute = ActionRelFluxoRedeExecute
    end
    object ActionRelFluxoRegulador: TAction
      Caption = 'Regulador'
    end
    object ActionRelFluxoSuprimento: TAction
      Caption = 'Suprimeto'
      Enabled = False
    end
    object ActionRelFluxoTrafo: TAction
      Caption = 'Trafo'
      OnExecute = ActionRelFluxoTrafoExecute
    end
    object ActionRelFluxoTrafo3E: TAction
      Caption = 'Trafo3E'
      Enabled = False
    end
    object ActionRelFluxoTrecho: TAction
      Caption = 'Trecho'
      OnExecute = ActionRelFluxoTrechoExecute
    end
    object ActionRelGerador: TAction
      Caption = 'Gerador'
      Hint = 'Exibir relat'#243'rio de geradores'
      OnExecute = ActionRelGeradorExecute
    end
    object ActionRelPerfilCarga: TAction
      Caption = 'Perfil Carga'
      OnExecute = ActionRelPerfilCargaExecute
    end
    object ActionRelReator: TAction
      Caption = 'Reator'
      OnExecute = ActionRelReatorExecute
    end
    object ActionRelRede: TAction
      Caption = 'Rede'
      Hint = 'Exibir relat'#243'rio de redes'
      OnExecute = ActionRelRedeExecute
    end
    object ActionRelSuprimento: TAction
      Caption = 'Suprimento'
      Hint = 'Exibir relat'#243'rio de suprimentos'
      OnExecute = ActionRelSuprimentoExecute
    end
    object ActionRelTrafo: TAction
      Caption = 'Trafo'
      Hint = 'Exibir relat'#243'rio de transformadores'
      OnExecute = ActionRelTrafoExecute
    end
    object ActionRelTrafo3E: TAction
      Caption = 'Trafo3E'
      OnExecute = ActionRelTrafo3EExecute
    end
    object ActionRelTrecho: TAction
      Caption = 'Trecho'
      Hint = 'Exibir relat'#243'rio de trechos'
      OnExecute = ActionRelTrechoExecute
    end
    object ActionRelRegulador: TAction
      Caption = 'Regulador'
      Hint = 'Exibir relat'#243'rio de reguladores'
      OnExecute = ActionRelReguladorExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      ImageIndex = 1
      OnExecute = ActionHelpExecute
    end
  end
  object table: TADOTable
    Connection = Connection
    Left = 32
    Top = 176
  end
  object Connection: TADOConnection
    LoginPrompt = False
    Left = 32
    Top = 128
  end
  object query: TADOQuery
    Connection = Connection
    Parameters = <>
    Left = 32
    Top = 224
  end
  object PopupMenuRelatorio: TPopupMenu
    Left = 32
    Top = 32
    object EquipamentosAtributos1: TMenuItem
      Caption = 'Equipamentos: Atributos '
    end
    object FluxodePotncia1: TMenuItem
      Caption = 'Fluxo de Pot'#234'ncia'
      object Patamar01: TMenuItem
        Caption = 'Patamar 0'
      end
      object Patamar11: TMenuItem
        Caption = 'Patamar 1'
      end
    end
  end
  object ImageList: TImageList
    Left = 88
    Top = 80
    Bitmap = {
      494C010102000400480010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
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
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      FF000000000000000000000000000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF00000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF0000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000FF000000
      00000000000000000000000000000000000000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000FFFFFF000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000000000BFBF
      BF00FFFFFF0000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
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
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFDFFFFF00000000FFCFF83F00000000
      FFC7E00F000000000003C0070000000000018003000000000000800300000000
      000100010000000000030001000000000007000100000000000F000100000000
      001F000100000000007F80030000000000FF80030000000001FFC00700000000
      03FFE00F00000000FFFFF83F0000000000000000000000000000000000000000
      000000000000}
  end
end