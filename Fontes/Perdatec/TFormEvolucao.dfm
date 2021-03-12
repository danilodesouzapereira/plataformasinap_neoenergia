object FormEvolucao: TFormEvolucao
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Hist'#243'rioc de Perdas'
  ClientHeight = 630
  ClientWidth = 799
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDefault
  PixelsPerInch = 96
  TextHeight = 13
  object GBoxRedeEqpto: TGroupBox
    Left = 0
    Top = 0
    Width = 799
    Height = 150
    Align = alTop
    Caption = 'Quantidade de Equipamentos da Rede'
    TabOrder = 0
    object LViewEqpto: TListView
      Left = 2
      Top = 15
      Width = 795
      Height = 133
      Align = alClient
      BorderStyle = bsNone
      Columns = <
        item
          Caption = 'Data'
          Width = 70
        end
        item
          Caption = 'C'#243'digo Rede'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = 'Redes'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Trafo'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cabo(km)'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Regulador'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Capacitor'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Reator'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.A4'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.Res.'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.Com.'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.Ind.'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.Rur.'
          Width = 65
        end
        item
          Alignment = taRightJustify
          Caption = 'Cons.Out.'
          Width = 65
        end>
      GridLines = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object GBoxRedeBalanco: TGroupBox
    Left = 0
    Top = 150
    Width = 799
    Height = 150
    Align = alTop
    Caption = 'Balan'#231'o de Energia da Rede (kWh/m'#234's)'
    TabOrder = 1
    object LViewBalanco: TListView
      Left = 2
      Top = 15
      Width = 795
      Height = 133
      Align = alClient
      BorderStyle = bsNone
      Columns = <
        item
          Caption = 'Data'
          Width = 70
        end
        item
          Caption = 'C'#243'digo Rede'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(+) Comprada'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(+) Recebida'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(+) Gerada'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(-) Transferida'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(-) Consumida'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = '(-) Perda'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = 'Perda (%)'
          Width = 90
        end>
      GridLines = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
  object GBoxRedePerda: TGroupBox
    Left = 0
    Top = 300
    Width = 799
    Height = 150
    Align = alTop
    Caption = ' Perdas na Rede (kWh/m'#234's)'
    TabOrder = 2
    object LViewPerda: TListView
      Left = 2
      Top = 15
      Width = 795
      Height = 133
      Align = alClient
      BorderStyle = bsNone
      Columns = <
        item
          Caption = 'Data'
          Width = 70
        end
        item
          Caption = 'C'#243'digo Rede'
          Width = 90
        end
        item
          Alignment = taRightJustify
          Caption = 'Total'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Trafo(cu)'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Cabo'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'SubTotal#1'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Trafo(fe)'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Regulador'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Capacitor'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Reator'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Ramal Lig.'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Medidor'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'Outros'
          Width = 80
        end
        item
          Alignment = taRightJustify
          Caption = 'SubTotal#2'
          Width = 80
        end>
      GridLines = True
      ReadOnly = True
      RowSelect = True
      TabOrder = 0
      ViewStyle = vsReport
    end
  end
end
