inherited FormExplorer: TFormExplorer
  Left = 206
  Top = 225
  BorderIcons = [biSystemMenu]
  Caption = 'Explorer'
  ClientHeight = 416
  ClientWidth = 392
  OldCreateOrder = True
  Position = poDesigned
  OnClose = FormClose
  OnShow = FormShow
  ExplicitWidth = 408
  ExplicitHeight = 455
  PixelsPerInch = 96
  TextHeight = 13
  inherited PanelToolBar: TPanel
    Width = 392
    ExplicitWidth = 392
    inherited PanelLeft: TPanel
      inherited ToolBarLeft: TToolBar
        inherited butAlign: TToolButton
          ExplicitWidth = 48
        end
        inherited butEdt: TToolButton
          ExplicitWidth = 27
        end
      end
    end
    inherited PanelRight: TPanel
      Width = 287
      ExplicitWidth = 287
      object ToolBarRight: TToolBar
        Left = 0
        Top = 0
        Width = 287
        Height = 22
        AutoSize = True
        ButtonWidth = 79
        Images = ImageList
        List = True
        ShowCaptions = True
        TabOrder = 0
        object ButExplorar: TToolButton
          Left = 0
          Top = 0
          Caption = 'Explorar...'
          DropdownMenu = PopupMenuExplorer
          ImageIndex = 9
        end
        object ButAjuda: TToolButton
          Left = 79
          Top = 0
          Action = ActionHelp
        end
      end
    end
  end
  inherited ActionList: TActionList
    object ActionRedeChanged: TAction
      OnExecute = ActionRedeChangedExecute
    end
    object ActionRedeSel: TAction
    end
    object ActionRedeSelAll: TAction
    end
    object ActionRedeSelNone: TAction
    end
    object ActionSelEqpto: TAction
      OnExecute = ActionSelEqptoExecute
    end
    object ActionExibeRedePorHierarquia: TAction
      Caption = 'Redes ordenadas por hierarquia'
      ImageIndex = 9
      OnExecute = ActionExibeRedePorHierarquiaExecute
    end
    object ActionExibeRedePorTipo: TAction
      Caption = 'Redes ordenadas por tipo'
      ImageIndex = 8
      OnExecute = ActionExibeRedePorTipoExecute
    end
    object ActionExibeChavePorTipo: TAction
      Caption = 'Chaves ordenadas por tipo'
      OnExecute = ActionExibeChavePorTipoExecute
    end
    object ActionExibeCargaPorTipo: TAction
      Caption = 'Cargas ordenadas por tipo'
      OnExecute = ActionExibeCargaPorTipoExecute
    end
    object ActionExibeTrechoPorCargto: TAction
      Caption = 'Trechos ordenados por carregamento'
      OnExecute = ActionExibeTrechoPorCargtoExecute
    end
    object ActionExibeCapPorPotencia: TAction
      Caption = 'ActionExibeCapPorPotencia'
      OnExecute = ActionExibeCapPorPotenciaExecute
    end
    object ActionExibeReguladorPorTipoOp: TAction
      OnExecute = ActionExibeReguladorPorTipoOpExecute
    end
    object ActionHelp: TAction
      Caption = 'Ajuda'
      Hint = 'Ajuda'
      ImageIndex = 10
      OnExecute = ActionHelpExecute
    end
  end
  inherited ImageList: TImageList
    Bitmap = {
      494C01010B000E00200010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000003000000001002000000000000030
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF0000000000FF000000FF000000FF00000000000000FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF000000000000000000FFFFFF00FFFFFF00FFFF
      FF0000000000FF000000FF000000FF00000000000000FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF00000000000000000000000000000000000000
      000000000000FFFFFF00808080000000FF0080808000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF0000000000FF00000000000000FFFFFF00FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF00000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF0000000000FF00000000000000FFFFFF00FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF000000000000000000000000000000000000FF
      FF00FFFFFF0000FFFF000000FF000000FF000000FF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF0000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF000000
      00000000FF000000FF000000FF0000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF00808080000000FF0080808000FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00000000007F7F7F0000FFFF007F7F7F0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000FFFFFF00000000007F7F7F0000FFFF007F7F7F0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF000000FF008080800000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000007F7F7F0000FFFF007F7F7F0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000007F7F7F0000FFFF007F7F7F0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF0000000000FFFF0000FFFF0000FFFF
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00FFFFFF0000000000FFFF0000FFFF0000FFFF
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00808080008080800000FFFF00FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFF0000FFFF0000FFFF
      000000000000FFFF000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF000000FF000000FF00FFFFFF0000FFFF00808080000000FF000000FF0000FF
      FF00FFFFFF0000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF00FFFF
      FF000000FF000000FF0080808000FFFFFF00808080000000FF000000FF00FFFF
      FF0000FFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00000000000000000000000000FFFF
      0000FFFF0000FFFF000000000000FFFF00000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF000000FF000000FF000000FF000000FF000000FF00FFFFFF0000FF
      FF00FFFFFF0000FFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000FFFF00000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF000000FF000000FF000000FF00FFFFFF0000FFFF00FFFF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF00000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF0000000000000000000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      FF000000FF000000FF0000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      FF000000FF000000FF0000000000FFFFFF000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      FF000000FF000000FF0000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      000000000000000000000000000000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000000000000FFFF000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF00000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000007F7F
      7F00000000007F7F7F0000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007F7F7F000000
      00000000FF000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFF0000FFFF
      0000FFFF0000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FF00
      0000FF000000FFFFFF00FF000000FF0000000000000000000000000000000000
      00000000000000000000FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F000000000000000000000000007F7F7F00000000000000
      FF00000000007F7F7F0000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      00000000000000000000000000000000000000000000FFFF0000FFFF0000FFFF
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007F7F7F00000000000000FF000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000FFFF0000FFFF0000FFFF00000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000FF000000FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF0000000000000000000000FF00000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F007F7F7F007F7F7F007F7F7F007F7F7F00000000000000
      00000000000000000000000000000000000000000000FFFF00000000000000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000FFFF000000000000FFFF000000000000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF00000000007F7F7F000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      000000000000000000000000000000000000000000000000000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00FF000000FF000000FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000FF000000FF000000FF000000FF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF007F7F7F00FFFFFF00FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000000
      000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF0000000000000000007F7F
      7F00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000FFFFFF0000FF
      FF000000000000FFFF00FFFFFF0000000000FFFFFF0000FFFF0000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F00000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000000000FFFFFF0000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FF000000FF000000000000000000
      000000000000000000000000000000FFFF00FFFFFF000000000000000000FFFF
      FF00FFFFFF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000007F7F7F00000000000000
      000000000000000000000000000000000000FF000000FF0000000000FF000000
      FF000000000000000000000000000000000000000000000000000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007F7F7F000000000000000000000000007F7F7F000000
      000000000000000000007F7F7F000000000000000000000000000000FF000000
      FF0000000000000000000000000000000000FFFFFF000000FF000000FF00FFFF
      FF000000FF000000FF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000000000000000000000000000
      0000000000007F7F7F0000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00007F7F7F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF0000000000FF00
      0000FF000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF000000
      FF000000FF00FFFFFF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000300000000100010000000000800100000000000000000000
      000000000000000000000000FFFFFF0080008000FFFF000080008000F83F0000
      C000C000E00F0000E000E000C0070000F000F00080030000F800F80080030000
      FC00FC0000010000FC00040000010000FE00060000010000FF80018000010000
      83C001C000010000838000008003000083E0C0608003000083E0C060C0070000
      83E0F060E00F0000FF86F006F83F0000FFFFFFFFE00F8000FFFFFFFFE00F8000
      FFF81FFFE00FC000F820041FF00FE000F000000FF00FF000F000000FF80FF800
      E0000007F007FC0080000001F007FE0000000000F007FF0080000001F00FFF80
      FC00003FF01F8380FE3FFC7FF81F83E0FFFFFFFFFE1F83E0FFFFFFFFFF1F83E0
      FFFFFFFFFF1F8384FFFFFFFFFFBFFFFEFFFFFC7FFFFFFDFFFFE3F83FC600F8FF
      FFC3F01F8200F8FFFB83F01F0000F87FF907F01F0000F81FF80FF01F0000F80F
      F01FF01F8000F00FF03FF01F8000E00FE01FF83F8000E00FE00FFEC38000E00F
      C07FFEB9C800F01FC1FFFF7D3C00F00F87FFFF3D0E00F00F9FFFFC99CE00F007
      FFFFF9C32600F007FFFFF3FF2600F00700000000000000000000000000000000
      000000000000}
  end
  object dlgSelColor: TColorDialog
    Left = 111
    Top = 33
  end
  object PopupMenuExplorer: TPopupMenu
    Left = 144
    Top = 32
    object MenuItemRedePorTipo: TMenuItem
      Action = ActionExibeRedePorTipo
    end
    object MenuItemRedePorHierarquia: TMenuItem
      Action = ActionExibeRedePorHierarquia
    end
    object MenuItemCapPorPotencia: TMenuItem
      Action = ActionExibeCapPorPotencia
      Caption = 'Capacitores ordenados por pot'#234'ncia'
    end
    object MenuItemCargaPorTipo: TMenuItem
      Action = ActionExibeCargaPorTipo
      Caption = 'Cargas ordenadas por classe'
    end
    object MenuItemChavePorTipo: TMenuItem
      Action = ActionExibeChavePorTipo
    end
    object Reguladoresportipodeoperao1: TMenuItem
      Action = ActionExibeReguladorPorTipoOp
      Caption = 'Reguladores por tipo de opera'#231#227'o'
    end
    object MenuItemTrechoPorCargto: TMenuItem
      Action = ActionExibeTrechoPorCargto
    end
  end
end