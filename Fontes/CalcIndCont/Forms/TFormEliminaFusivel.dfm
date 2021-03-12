object FormEliminaFusivel: TFormEliminaFusivel
  Left = 0
  Top = 0
  Caption = 'FormEliminaFusivel'
  ClientHeight = 240
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 447
    Height = 240
    ActivePage = TabSheet2
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Button2: TButton
        Left = 8
        Top = 8
        Width = 121
        Height = 25
        Caption = 'Mostrar'
        TabOrder = 0
        OnClick = Button2Click
      end
      object Button1: TButton
        Left = 8
        Top = 39
        Width = 121
        Height = 25
        Caption = 'Eliminar Fusiveis'
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button3: TButton
        Left = 8
        Top = 70
        Width = 121
        Height = 25
        Caption = 'Inserir chaves NA'
        TabOrder = 2
        OnClick = Button3Click
      end
      object butEliminaNADuplic: TButton
        Left = 8
        Top = 101
        Width = 121
        Height = 25
        Caption = 'Eliminar NA Duplicadas'
        TabOrder = 3
        OnClick = butEliminaNADuplicClick
      end
      object Button4: TButton
        Left = 8
        Top = 132
        Width = 121
        Height = 25
        Action = ActionAvaliaDefeito
        TabOrder = 4
      end
      object rchRes: TRichEdit
        Left = 144
        Top = 8
        Width = 289
        Height = 149
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Zoom = 100
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'TabSheet2'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object tvChvProt: TTreeView
        Left = 219
        Top = 0
        Width = 220
        Height = 212
        Align = alRight
        Indent = 19
        TabOrder = 0
        OnClick = ActionMostraRamoArvoreExecute
      end
      object Button5: TButton
        Left = 24
        Top = 16
        Width = 75
        Height = 25
        Action = ActionArvoreProt
        TabOrder = 1
      end
      object Button6: TButton
        Left = 24
        Top = 47
        Width = 75
        Height = 25
        Action = ActionImprimeResultados
        TabOrder = 2
      end
    end
  end
  object ActionList1: TActionList
    Left = 184
    Top = 40
    object ActionAvaliaDefeito: TAction
      Caption = 'Avaliar defeito'
      OnExecute = ActionAvaliaDefeitoExecute
    end
    object ActionArvoreProt: TAction
      Caption = 'ArvoreProt'
      OnExecute = ActionArvoreProtExecute
    end
    object ActionMostraRamoArvore: TAction
      Caption = 'ActionMostraRamoArvore'
      OnExecute = ActionMostraRamoArvoreExecute
    end
    object ActionImprimeResultados: TAction
      Caption = 'Imprime'
      OnExecute = ActionImprimeResultadosExecute
    end
  end
end
