object FormRelCurto: TFormRelCurto
  Left = 289
  Top = 189
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Relat'#243'rio de Curto-Circuito'
  ClientHeight = 245
  ClientWidth = 616
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 616
    Height = 29
    Caption = 'ToolBar'
    EdgeBorders = [ebTop, ebBottom]
    Images = ImageList
    TabOrder = 0
    object CBoxDefeito: TComboBox
      Left = 0
      Top = 0
      Width = 145
      Height = 22
      Style = csDropDownList
      TabOrder = 0
      OnChange = ActionAtualizaExecute
    end
    object ToolButton2: TToolButton
      Left = 145
      Top = 0
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object CBoxUnidTensao: TComboBox
      Left = 153
      Top = 0
      Width = 86
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 1
      Text = 'Tens'#227'o: pu'
      OnChange = ActionAtualizaExecute
      Items.Strings = (
        'Tens'#227'o: pu'
        'Tens'#227'o: kV')
    end
    object ToolButton1: TToolButton
      Left = 239
      Top = 0
      Width = 8
      Caption = 'ToolButton1'
      Style = tbsSeparator
    end
    object CBoxUnidCorrente: TComboBox
      Left = 247
      Top = 0
      Width = 86
      Height = 21
      Style = csDropDownList
      ItemIndex = 1
      TabOrder = 2
      Text = 'Corrente: kA'
      OnChange = ActionAtualizaExecute
      Items.Strings = (
        'Corrente: A'
        'Corrente: kA')
    end
  end
  object PageControl: TPageControl
    Left = 0
    Top = 29
    Width = 616
    Height = 216
    ActivePage = TabSheetCorrente
    Align = alClient
    TabOrder = 1
    OnChange = PageControlChange
    object TabSheetPtoDefeito: TTabSheet
      Caption = 'Ponto de Defeito'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewDefIcc: TListView
        Left = 0
        Top = 41
        Width = 608
        Height = 38
        Align = alTop
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Ia'
            Width = 75
          end
          item
            Caption = 'Ib'
            Width = 75
          end
          item
            Caption = 'Ic'
            Width = 75
          end
          item
            Caption = 'I0'
            Width = 75
          end
          item
            Caption = 'I1'
            Width = 75
          end
          item
            Caption = 'I2'
            Width = 75
          end>
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
      end
      object LViewDefVcc: TListView
        Left = 0
        Top = 0
        Width = 608
        Height = 41
        Align = alTop
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Va'
            Width = 75
          end
          item
            Caption = 'Vb'
            Width = 75
          end
          item
            Caption = 'Vc'
            Width = 75
          end
          item
            Caption = 'V0'
            Width = 75
          end
          item
            Caption = 'V1'
            Width = 75
          end
          item
            Caption = 'V2'
            Width = 75
          end>
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 1
        ViewStyle = vsReport
        OnChange = LViewChange
      end
    end
    object TabSheetContribuicao: TTabSheet
      Caption = 'Contribui'#231#245'es'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewSupIcc: TListView
        Left = 0
        Top = 0
        Width = 608
        Height = 188
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Ia'
            Width = 75
          end
          item
            Caption = 'Ib'
            Width = 75
          end
          item
            Caption = 'Ic'
            Width = 75
          end
          item
            Caption = 'I0'
            Width = 75
          end
          item
            Caption = 'I1'
            Width = 75
          end
          item
            Caption = 'I2'
            Width = 75
          end>
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
      end
    end
    object TabSheetTensao: TTabSheet
      Caption = 'Tens'#245'es'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewBarVcc: TListView
        Left = 0
        Top = 0
        Width = 608
        Height = 188
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Va'
            Width = 75
          end
          item
            Caption = 'Vb'
            Width = 75
          end
          item
            Caption = 'Vc'
            Width = 75
          end
          item
            Caption = 'V0'
            Width = 75
          end
          item
            Caption = 'V1'
            Width = 75
          end
          item
            Caption = 'V2'
            Width = 75
          end>
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
      end
    end
    object TabSheetCorrente: TTabSheet
      Caption = 'Correntes'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LViewLigIcc: TListView
        Left = 0
        Top = 0
        Width = 608
        Height = 188
        Align = alClient
        Columns = <
          item
            Caption = 'Eqpto'
            Width = 65
          end
          item
            Caption = 'C'#243'digo'
            Width = 80
          end
          item
            Caption = 'Ia'
            Width = 75
          end
          item
            Caption = 'Ib'
            Width = 75
          end
          item
            Caption = 'Ic'
            Width = 75
          end
          item
            Caption = 'I0'
            Width = 75
          end
          item
            Caption = 'I1'
            Width = 75
          end
          item
            Caption = 'I2'
            Width = 75
          end>
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnChange = LViewChange
      end
    end
  end
  object ImageList: TImageList
    Left = 448
  end
  object ActionList: TActionList
    Images = ImageList
    Left = 392
    object ActionAtualiza: TAction
      OnExecute = ActionAtualizaExecute
    end
  end
end
