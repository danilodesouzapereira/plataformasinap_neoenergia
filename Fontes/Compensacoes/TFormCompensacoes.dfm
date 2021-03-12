inherited FormCompensacoes: TFormCompensacoes
  BorderStyle = bsToolWindow
  Caption = 'Estimativa de Compensa'#231#245'es'
  ClientHeight = 437
  ClientWidth = 404
  ExplicitWidth = 410
  ExplicitHeight = 466
  PixelsPerInch = 96
  TextHeight = 13
  inherited PanelToolBar: TPanel
    Width = 404
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
      Width = 299
    end
  end
  object PageControl: TPageControl [1]
    Left = 0
    Top = 25
    Width = 404
    Height = 412
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 1
    object TabSheetDados: TTabSheet
      Caption = 'Dados'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 407
      ExplicitHeight = 357
      object Panel1: TPanel
        Left = 0
        Top = 347
        Width = 396
        Height = 37
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        ExplicitTop = 320
        ExplicitWidth = 407
      end
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 396
        Height = 347
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        ExplicitWidth = 407
        ExplicitHeight = 320
        object GroupBox1: TGroupBox
          Left = 0
          Top = 0
          Width = 396
          Height = 289
          Align = alTop
          Caption = 'SUBESTA'#199#195'O'
          TabOrder = 0
          ExplicitWidth = 407
          DesignSize = (
            396
            289)
          object ComboBoxSubestacoes: TComboBox
            Left = 8
            Top = 19
            Width = 376
            Height = 21
            Anchors = [akLeft, akTop, akRight, akBottom]
            TabOrder = 0
            Text = 'Selecione uma subesta'#231#227'o'
            OnChange = ComboBoxSubestacoesChange
          end
          object GroupBox17: TGroupBox
            Left = 2
            Top = 44
            Width = 392
            Height = 243
            Align = alBottom
            Caption = 'LIMITES'
            TabOrder = 1
            ExplicitWidth = 403
            object PageControlLimites: TPageControl
              Left = 2
              Top = 15
              Width = 388
              Height = 226
              ActivePage = TabSheetMT
              Align = alClient
              Style = tsFlatButtons
              TabOrder = 0
              ExplicitWidth = 399
              object TabSheetMT: TTabSheet
                Caption = 'MT'
                ExplicitLeft = 0
                ExplicitTop = 0
                ExplicitWidth = 391
                ExplicitHeight = 0
                object PageControlMt: TPageControl
                  Left = 0
                  Top = 0
                  Width = 380
                  Height = 195
                  ActivePage = TabSheetMtUrb
                  Align = alClient
                  Style = tsFlatButtons
                  TabOrder = 0
                  ExplicitWidth = 391
                  object TabSheetMtUrb: TTabSheet
                    Caption = 'Urbano'
                    ExplicitLeft = 0
                    ExplicitTop = 0
                    ExplicitWidth = 383
                    ExplicitHeight = 0
                    object GroupBoxMtUrbDMIC: TGroupBox
                      Left = 0
                      Top = 109
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DMIC'
                      TabOrder = 0
                      ExplicitWidth = 383
                      object Label39: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtUrbDMIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                    end
                    object GroupBoxMTUrbDIC: TGroupBox
                      Left = 0
                      Top = 54
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DIC'
                      TabOrder = 1
                      ExplicitWidth = 383
                      object Label40: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label41: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label42: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtUrbAnuDIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditMtUrbTriDIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditMtUrbMenDIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                    object GroupBoxMtUrbFIC: TGroupBox
                      Left = 0
                      Top = -1
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'FIC'
                      TabOrder = 2
                      ExplicitWidth = 383
                      object Label28: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label29: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label30: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtUrbAnuFIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditMtUrbTriFIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditMtUrbMenFIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                  end
                  object TabSheetMtRur: TTabSheet
                    Caption = 'Rural'
                    ImageIndex = 1
                    ExplicitLeft = 0
                    ExplicitTop = 0
                    ExplicitWidth = 383
                    ExplicitHeight = 0
                    object GroupBoxMtRurDMIC: TGroupBox
                      Left = 0
                      Top = 109
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DMIC'
                      TabOrder = 0
                      ExplicitWidth = 383
                      object Label31: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtRurDMIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                    end
                    object GroupBoxMtRurDIC: TGroupBox
                      Left = 0
                      Top = 54
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DIC'
                      TabOrder = 1
                      ExplicitWidth = 383
                      object Label32: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label34: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label35: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtRurAnuDIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditMtRurTriDIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditMtRurMenDIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                    object GroupBoxMtRurFIC: TGroupBox
                      Left = 0
                      Top = -1
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'FIC'
                      TabOrder = 2
                      ExplicitWidth = 383
                      object Label36: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label37: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label38: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditMtRurAnuFIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditMtRurTriFIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditMtRurMenFIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                  end
                end
              end
              object TabSheetBT: TTabSheet
                Caption = 'BT'
                ImageIndex = 1
                ExplicitLeft = 0
                ExplicitTop = 0
                ExplicitWidth = 391
                ExplicitHeight = 0
                object PageControlBt: TPageControl
                  Left = 0
                  Top = 0
                  Width = 380
                  Height = 195
                  ActivePage = TabSheetBtUrb
                  Align = alClient
                  Style = tsFlatButtons
                  TabOrder = 0
                  ExplicitWidth = 391
                  object TabSheetBtUrb: TTabSheet
                    Caption = 'Urbano'
                    ExplicitLeft = 0
                    ExplicitTop = 0
                    ExplicitWidth = 383
                    ExplicitHeight = 0
                    object GroupBoxBtUrbDMIC: TGroupBox
                      Left = 0
                      Top = 109
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DMIC'
                      TabOrder = 0
                      ExplicitWidth = 383
                      object Label46: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtUrbDMIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                    end
                    object GroupBoxBtUrbDIC: TGroupBox
                      Left = 0
                      Top = 54
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DIC'
                      TabOrder = 1
                      ExplicitWidth = 383
                      object Label47: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label48: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label49: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtUrbAnuDIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditBtUrbMenDIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditBtUrbTriDIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                    object GroupBoxBtUrbFIC: TGroupBox
                      Left = 0
                      Top = -1
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'FIC'
                      TabOrder = 2
                      ExplicitWidth = 383
                      object Label50: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label51: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label52: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtUrbAnuFIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditBtUrbTriFIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditBtUrbMenFIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                  end
                  object TabSheetBtRur: TTabSheet
                    Caption = 'Rural'
                    ImageIndex = 1
                    ExplicitLeft = 0
                    ExplicitTop = 0
                    ExplicitWidth = 383
                    ExplicitHeight = 0
                    object GroupBoxBtRurDMIC: TGroupBox
                      Left = 0
                      Top = 109
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DMIC'
                      TabOrder = 0
                      ExplicitWidth = 383
                      object Label33: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtRurDMIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                    end
                    object GroupBoxBtRurDIC: TGroupBox
                      Left = 0
                      Top = 54
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'DIC'
                      TabOrder = 1
                      ExplicitWidth = 383
                      object Label25: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label26: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label27: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtRurAnuDIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditBtRurTriDIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditBtRurMenDIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                    object GroupBoxBtRurFIC: TGroupBox
                      Left = 0
                      Top = -1
                      Width = 372
                      Height = 55
                      Align = alBottom
                      Caption = 'FIC'
                      TabOrder = 2
                      ExplicitWidth = 383
                      object Label53: TLabel
                        Left = 6
                        Top = 23
                        Width = 31
                        Height = 13
                        Caption = 'Anual:'
                      end
                      object Label54: TLabel
                        Left = 117
                        Top = 23
                        Width = 51
                        Height = 13
                        Caption = 'Trimestral:'
                      end
                      object Label55: TLabel
                        Left = 241
                        Top = 23
                        Width = 37
                        Height = 13
                        Caption = 'Mensal:'
                      end
                      object EditBtRurAnuFIC: TEdit
                        Left = 51
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 0
                      end
                      object EditBtRurTriFIC: TEdit
                        Left = 170
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 1
                      end
                      object EditBtRurMenFIC: TEdit
                        Left = 295
                        Top = 20
                        Width = 41
                        Height = 21
                        ReadOnly = True
                        TabOrder = 2
                      end
                    end
                  end
                end
              end
            end
          end
        end
      end
    end
    object TabSheetResultados: TTabSheet
      Caption = 'Compensa'#231#245'es'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 407
      ExplicitHeight = 357
      object GroupBox4: TGroupBox
        Left = 0
        Top = 193
        Width = 396
        Height = 72
        Align = alTop
        Caption = 'TRIMESTRAIS'
        TabOrder = 0
        ExplicitWidth = 407
        object Label65: TLabel
          Left = 12
          Top = 19
          Width = 64
          Height = 13
          Caption = '1o Trimestre:'
        end
        object Label66: TLabel
          Left = 12
          Top = 46
          Width = 64
          Height = 13
          Caption = '2o Trimestre:'
        end
        object Label68: TLabel
          Left = 199
          Top = 19
          Width = 64
          Height = 13
          Caption = '3o Trimestre:'
        end
        object Label71: TLabel
          Left = 199
          Top = 46
          Width = 64
          Height = 13
          Caption = '4o Trimestre:'
        end
        object EditComp2Tri: TEdit
          Left = 91
          Top = 43
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 0
        end
        object EditComp3Tri: TEdit
          Left = 282
          Top = 16
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 1
        end
        object EditComp1Tri: TEdit
          Left = 91
          Top = 16
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 2
        end
        object EditComp4Tri: TEdit
          Left = 282
          Top = 43
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 3
        end
      end
      object GroupBox5: TGroupBox
        Left = 0
        Top = 0
        Width = 396
        Height = 193
        Align = alTop
        Caption = 'MENSAIS'
        TabOrder = 1
        ExplicitWidth = 407
        object Label77: TLabel
          Left = 12
          Top = 22
          Width = 63
          Height = 13
          Caption = 'Janeiro [R$]:'
        end
        object Label78: TLabel
          Left = 12
          Top = 52
          Width = 74
          Height = 13
          Caption = 'Fevereiro [R$]:'
        end
        object Label79: TLabel
          Left = 12
          Top = 79
          Width = 57
          Height = 13
          Caption = 'Mar'#231'o [R$]:'
        end
        object Label80: TLabel
          Left = 12
          Top = 106
          Width = 49
          Height = 13
          Caption = 'Abril [R$]:'
        end
        object Label81: TLabel
          Left = 199
          Top = 22
          Width = 53
          Height = 13
          Caption = 'Julho [R$]:'
        end
        object Label82: TLabel
          Left = 199
          Top = 106
          Width = 68
          Height = 13
          Caption = 'Outubro [R$]:'
        end
        object Label83: TLabel
          Left = 12
          Top = 133
          Width = 50
          Height = 13
          Caption = 'Maio [R$]:'
        end
        object Label84: TLabel
          Left = 199
          Top = 52
          Width = 62
          Height = 13
          Caption = 'Agosto [R$]:'
        end
        object Label85: TLabel
          Left = 199
          Top = 133
          Width = 77
          Height = 13
          Caption = 'Novembro [R$]:'
        end
        object Label86: TLabel
          Left = 199
          Top = 161
          Width = 76
          Height = 13
          Caption = 'Dezembro [R$]:'
        end
        object Label87: TLabel
          Left = 199
          Top = 79
          Width = 74
          Height = 13
          Caption = 'Setembro [R$]:'
        end
        object Label88: TLabel
          Left = 12
          Top = 161
          Width = 57
          Height = 13
          Caption = 'Junho [R$]:'
        end
        object EditCompJan: TEdit
          Left = 91
          Top = 19
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 0
        end
        object EditCompFev: TEdit
          Left = 91
          Top = 49
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 1
        end
        object EditCompMar: TEdit
          Left = 91
          Top = 76
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 2
        end
        object EditCompAbr: TEdit
          Left = 91
          Top = 103
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 3
        end
        object EditCompJul: TEdit
          Left = 282
          Top = 19
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 4
        end
        object EditCompOut: TEdit
          Left = 282
          Top = 103
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 5
        end
        object EditCompNov: TEdit
          Left = 282
          Top = 130
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 6
        end
        object EditCompAgo: TEdit
          Left = 282
          Top = 49
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 7
        end
        object EditCompMai: TEdit
          Left = 91
          Top = 130
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 8
        end
        object EditCompJun: TEdit
          Left = 91
          Top = 158
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 9
        end
        object EditCompSet: TEdit
          Left = 282
          Top = 76
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 10
        end
        object EditCompDez: TEdit
          Left = 282
          Top = 158
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 11
        end
      end
      object GroupBox6: TGroupBox
        Left = 0
        Top = 312
        Width = 396
        Height = 47
        Align = alTop
        Caption = 'TOTAL'
        TabOrder = 2
        ExplicitWidth = 407
        object Label89: TLabel
          Left = 12
          Top = 22
          Width = 28
          Height = 13
          Caption = 'Total:'
        end
        object EditCompTotal: TEdit
          Left = 91
          Top = 19
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 0
        end
      end
      object GroupBox8: TGroupBox
        Left = 0
        Top = 265
        Width = 396
        Height = 47
        Align = alTop
        Caption = 'ANUAL'
        TabOrder = 3
        ExplicitWidth = 407
        object Label60: TLabel
          Left = 12
          Top = 22
          Width = 44
          Height = 13
          Caption = 'Subtotal:'
        end
        object EditCompAnu: TEdit
          Left = 91
          Top = 19
          Width = 70
          Height = 21
          ReadOnly = True
          TabOrder = 0
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      ImageIndex = 2
      object GroupBox2: TGroupBox
        Left = 0
        Top = 0
        Width = 396
        Height = 384
        Align = alClient
        Caption = 'CARGA'
        TabOrder = 0
        DesignSize = (
          396
          384)
        object ComboBoxCargas: TComboBox
          Left = 2
          Top = 15
          Width = 392
          Height = 21
          Align = alTop
          TabOrder = 0
          Text = 'Selecione uma carga'
          OnChange = ComboBoxCargasChange
        end
        object GroupBox19: TGroupBox
          Left = 2
          Top = 172
          Width = 392
          Height = 210
          Align = alClient
          Caption = 'Valores Estimados'
          TabOrder = 1
          object PageControlValApurados: TPageControl
            Left = 2
            Top = 15
            Width = 388
            Height = 193
            ActivePage = TabSheetDIC
            Align = alClient
            Style = tsFlatButtons
            TabOrder = 0
            object TabSheetDIC: TTabSheet
              Caption = 'DIC'
              object Label2: TLabel
                Left = 2
                Top = 33
                Width = 25
                Height = 13
                Caption = 'Abril:'
              end
              object Label1: TLabel
                Left = 2
                Top = 6
                Width = 39
                Height = 13
                Caption = 'Janeiro:'
              end
              object Label7: TLabel
                Left = 2
                Top = 60
                Width = 29
                Height = 13
                Caption = 'Julho:'
              end
              object Label10: TLabel
                Left = 2
                Top = 87
                Width = 44
                Height = 13
                Caption = 'Outubro:'
              end
              object Label11: TLabel
                Left = 116
                Top = 87
                Width = 53
                Height = 13
                Caption = 'Novembro:'
              end
              object Label8: TLabel
                Left = 116
                Top = 60
                Width = 38
                Height = 13
                Caption = 'Agosto:'
              end
              object Label5: TLabel
                Left = 116
                Top = 33
                Width = 26
                Height = 13
                Caption = 'Maio:'
              end
              object Label3: TLabel
                Left = 116
                Top = 6
                Width = 50
                Height = 13
                Caption = 'Fevereiro:'
              end
              object Label4: TLabel
                Left = 240
                Top = 6
                Width = 33
                Height = 13
                Caption = 'Mar'#231'o:'
              end
              object Label6: TLabel
                Left = 240
                Top = 33
                Width = 33
                Height = 13
                Caption = 'Junho:'
              end
              object Label9: TLabel
                Left = 240
                Top = 60
                Width = 50
                Height = 13
                Caption = 'Setembro:'
              end
              object Label12: TLabel
                Left = 240
                Top = 87
                Width = 52
                Height = 13
                Caption = 'Dezembro:'
              end
              object EditDICJan: TEdit
                Left = 47
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 0
                Text = '20'
              end
              object UpDown13: TUpDown
                Left = 88
                Top = 3
                Width = 15
                Height = 21
                Position = 10
                TabOrder = 1
              end
              object UpDown16: TUpDown
                Left = 88
                Top = 30
                Width = 16
                Height = 21
                Associate = EditDICAbr
                Position = 20
                TabOrder = 2
              end
              object EditDICAbr: TEdit
                Left = 47
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 3
                Text = '20'
              end
              object EditDICJul: TEdit
                Left = 47
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 4
                Text = '20'
              end
              object UpDown19: TUpDown
                Left = 88
                Top = 57
                Width = 16
                Height = 21
                Associate = EditDICJul
                Position = 20
                TabOrder = 5
              end
              object UpDown22: TUpDown
                Left = 88
                Top = 84
                Width = 16
                Height = 21
                Associate = EditDICOut
                Position = 20
                TabOrder = 6
              end
              object EditDICOut: TEdit
                Left = 47
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 7
                Text = '20'
              end
              object EditDICFev: TEdit
                Left = 169
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 8
                Text = '20'
              end
              object EditDICMai: TEdit
                Left = 169
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 9
                Text = '20'
              end
              object EditDICAgo: TEdit
                Left = 169
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 10
                Text = '20'
              end
              object EditDICNov: TEdit
                Left = 169
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 11
                Text = '20'
              end
              object UpDown23: TUpDown
                Left = 210
                Top = 84
                Width = 16
                Height = 21
                Associate = EditDICNov
                Position = 20
                TabOrder = 12
              end
              object UpDown20: TUpDown
                Left = 210
                Top = 57
                Width = 16
                Height = 21
                Associate = EditDICAgo
                Position = 20
                TabOrder = 13
              end
              object UpDown17: TUpDown
                Left = 210
                Top = 30
                Width = 16
                Height = 21
                Associate = EditDICMai
                Position = 20
                TabOrder = 14
              end
              object UpDown14: TUpDown
                Left = 210
                Top = 3
                Width = 16
                Height = 21
                Associate = EditDICFev
                Position = 20
                TabOrder = 15
              end
              object EditDICDez: TEdit
                Left = 294
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 16
                Text = '20'
              end
              object EditDICSet: TEdit
                Left = 294
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 17
                Text = '20'
              end
              object EditDICJun: TEdit
                Left = 294
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 18
                Text = '20'
              end
              object EditDICMar: TEdit
                Left = 294
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 19
                Text = '20'
              end
              object UpDown15: TUpDown
                Left = 335
                Top = 3
                Width = 16
                Height = 21
                Associate = EditDICMar
                Position = 20
                TabOrder = 20
              end
              object UpDown18: TUpDown
                Left = 335
                Top = 30
                Width = 16
                Height = 21
                Associate = EditDICJun
                Position = 20
                TabOrder = 21
              end
              object UpDown21: TUpDown
                Left = 335
                Top = 57
                Width = 16
                Height = 21
                Associate = EditDICSet
                Position = 20
                TabOrder = 22
              end
              object UpDown24: TUpDown
                Left = 335
                Top = 84
                Width = 16
                Height = 21
                Associate = EditDICDez
                Position = 20
                TabOrder = 23
              end
            end
            object TabSheetFIC: TTabSheet
              Caption = 'FIC'
              ImageIndex = 1
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 391
              ExplicitHeight = 114
              object Label14: TLabel
                Left = 2
                Top = 33
                Width = 25
                Height = 13
                Caption = 'Abril:'
              end
              object Label13: TLabel
                Left = 2
                Top = 6
                Width = 39
                Height = 13
                Caption = 'Janeiro:'
              end
              object Label19: TLabel
                Left = 2
                Top = 60
                Width = 29
                Height = 13
                Caption = 'Julho:'
              end
              object Label22: TLabel
                Left = 2
                Top = 87
                Width = 44
                Height = 13
                Caption = 'Outubro:'
              end
              object Label23: TLabel
                Left = 116
                Top = 87
                Width = 53
                Height = 13
                Caption = 'Novembro:'
              end
              object Label20: TLabel
                Left = 116
                Top = 60
                Width = 38
                Height = 13
                Caption = 'Agosto:'
              end
              object Label17: TLabel
                Left = 116
                Top = 33
                Width = 26
                Height = 13
                Caption = 'Maio:'
              end
              object Label15: TLabel
                Left = 116
                Top = 6
                Width = 50
                Height = 13
                Caption = 'Fevereiro:'
              end
              object Label16: TLabel
                Left = 240
                Top = 6
                Width = 33
                Height = 13
                Caption = 'Mar'#231'o:'
              end
              object Label18: TLabel
                Left = 240
                Top = 33
                Width = 33
                Height = 13
                Caption = 'Junho:'
              end
              object Label21: TLabel
                Left = 240
                Top = 60
                Width = 50
                Height = 13
                Caption = 'Setembro:'
              end
              object Label24: TLabel
                Left = 240
                Top = 87
                Width = 52
                Height = 13
                Caption = 'Dezembro:'
              end
              object EditFICJan: TEdit
                Left = 47
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 0
                Text = '10'
              end
              object EditFICAbr: TEdit
                Left = 47
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 1
                Text = '10'
              end
              object UpDown1: TUpDown
                Left = 88
                Top = 3
                Width = 15
                Height = 21
                Associate = EditFICJan
                Position = 10
                TabOrder = 2
              end
              object UpDown4: TUpDown
                Left = 88
                Top = 30
                Width = 16
                Height = 21
                Associate = EditFICAbr
                Position = 10
                TabOrder = 3
              end
              object EditFICJul: TEdit
                Left = 47
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 4
                Text = '10'
              end
              object EditFICOut: TEdit
                Left = 47
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 5
                Text = '10'
              end
              object UpDown7: TUpDown
                Left = 88
                Top = 57
                Width = 15
                Height = 21
                Associate = EditFICJul
                Position = 10
                TabOrder = 6
              end
              object UpDown10: TUpDown
                Left = 88
                Top = 84
                Width = 16
                Height = 21
                Associate = EditFICOut
                Position = 10
                TabOrder = 7
              end
              object EditFICFev: TEdit
                Left = 169
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 8
                Text = '10'
              end
              object EditFICMai: TEdit
                Left = 169
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 9
                Text = '10'
              end
              object EditFICAgo: TEdit
                Left = 169
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 10
                Text = '10'
              end
              object EditFICNov: TEdit
                Left = 169
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 11
                Text = '10'
              end
              object UpDown11: TUpDown
                Left = 210
                Top = 84
                Width = 16
                Height = 21
                Associate = EditFICNov
                Position = 10
                TabOrder = 12
              end
              object UpDown8: TUpDown
                Left = 210
                Top = 57
                Width = 16
                Height = 21
                Associate = EditFICAgo
                Position = 10
                TabOrder = 13
              end
              object UpDown5: TUpDown
                Left = 210
                Top = 30
                Width = 16
                Height = 21
                Associate = EditFICMai
                Position = 10
                TabOrder = 14
              end
              object UpDown2: TUpDown
                Left = 210
                Top = 3
                Width = 16
                Height = 21
                Associate = EditFICFev
                Position = 10
                TabOrder = 15
              end
              object EditFICMar: TEdit
                Left = 294
                Top = 3
                Width = 41
                Height = 21
                TabOrder = 16
                Text = '10'
              end
              object EditFICJun: TEdit
                Left = 294
                Top = 30
                Width = 41
                Height = 21
                TabOrder = 17
                Text = '10'
              end
              object EditFICSet: TEdit
                Left = 294
                Top = 57
                Width = 41
                Height = 21
                TabOrder = 18
                Text = '10'
              end
              object EditFICDez: TEdit
                Left = 294
                Top = 84
                Width = 41
                Height = 21
                TabOrder = 19
                Text = '10'
              end
              object UpDown12: TUpDown
                Left = 335
                Top = 84
                Width = 16
                Height = 21
                Associate = EditFICDez
                Position = 10
                TabOrder = 20
              end
              object UpDown9: TUpDown
                Left = 335
                Top = 57
                Width = 16
                Height = 21
                Associate = EditFICSet
                Position = 10
                TabOrder = 21
              end
              object UpDown6: TUpDown
                Left = 335
                Top = 30
                Width = 16
                Height = 21
                Associate = EditFICJun
                Position = 10
                TabOrder = 22
              end
              object UpDown3: TUpDown
                Left = 335
                Top = 3
                Width = 16
                Height = 21
                Associate = EditFICMar
                Position = 10
                TabOrder = 23
              end
            end
          end
        end
        object GroupBox3: TGroupBox
          Left = 2
          Top = 36
          Width = 392
          Height = 81
          Align = alTop
          Caption = 'Consumidores'
          TabOrder = 2
          object Label43: TLabel
            Left = 6
            Top = 23
            Width = 57
            Height = 13
            Caption = 'Residencial:'
          end
          object Label44: TLabel
            Left = 124
            Top = 23
            Width = 50
            Height = 13
            Caption = 'Comercial:'
          end
          object Label45: TLabel
            Left = 251
            Top = 23
            Width = 49
            Height = 13
            Caption = 'Industrial:'
          end
          object Label57: TLabel
            Left = 6
            Top = 50
            Width = 29
            Height = 13
            Caption = 'Rural:'
          end
          object Label58: TLabel
            Left = 124
            Top = 50
            Width = 37
            Height = 13
            Caption = 'Outros:'
          end
          object Label59: TLabel
            Left = 251
            Top = 50
            Width = 17
            Height = 13
            Caption = 'A4:'
          end
          object EditConsRes: TEdit
            Left = 69
            Top = 20
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 0
          end
          object EditConsCom: TEdit
            Left = 180
            Top = 20
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 1
          end
          object EditConsInd: TEdit
            Left = 305
            Top = 20
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 2
          end
          object EditConsRur: TEdit
            Left = 69
            Top = 47
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 3
          end
          object EditConsOut: TEdit
            Left = 180
            Top = 47
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 4
          end
          object EditConsA4: TEdit
            Left = 305
            Top = 47
            Width = 41
            Height = 21
            ReadOnly = True
            TabOrder = 5
          end
        end
        object GroupBox7: TGroupBox
          Left = 2
          Top = 117
          Width = 392
          Height = 55
          Align = alTop
          Caption = 'Energia [kWh/m'#234's]'
          TabOrder = 3
          object Label56: TLabel
            Left = 6
            Top = 23
            Width = 28
            Height = 13
            Caption = 'Total:'
          end
          object EditEnergia: TEdit
            Left = 68
            Top = 20
            Width = 80
            Height = 21
            ReadOnly = True
            TabOrder = 0
          end
        end
        object ButtonCalcula: TButton
          Left = 10
          Top = 336
          Width = 372
          Height = 25
          Anchors = [akLeft, akTop, akRight, akBottom]
          Caption = 'Calcula'
          TabOrder = 4
          OnClick = ButtonCalculaClick
        end
      end
    end
  end
  inherited ActionList: TActionList
    Left = 177
    Top = 8
  end
  inherited ImageList: TImageList
    Left = 209
    Top = 9
    Bitmap = {
      494C010107000900300010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
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
      000000FFFF000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000FFFF00000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFF0000FFFF0000FFFF0000FFFF0000FFFF0000FFFF00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00000000000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0000FFFF00FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000000000FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      000000000000FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00000000000000000000000000000000000000
      00000000000000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF000000000000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF0000FFFF000000
      0000FFFF0000000000000000000000000000000000000000000000000000FFFF
      00000000000000FFFF00FFFFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF00000000000000
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
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFE00F0000FFFFFFFFE00F0000
      FFF81FFFE00F0000F820041FF00F0000F000000FF00F0000F000000FF80F0000
      E0000007F007000080000001F007000000000000F007000080000001F00F0000
      FC00003FF01F0000FE3FFC7FF81F0000FFFFFFFFFE1F0000FFFFFFFFFF1F0000
      FFFFFFFFFF1F0000FFFFFFFFFFBF0000FFFFFC7FFFFFFDFFFFE3F83FC600F8FF
      FFC3F01F8200F8FFFB83F01F0000F87FF907F01F0000F81FF80FF01F0000F80F
      F01FF01F8000F00FF03FF01F8000E00FE01FF83F8000E00FE00FFEC38000E00F
      C07FFEB9C800F01FC1FFFF7D3C00F00F87FFFF3D0E00F00F9FFFFC99CE00F007
      FFFFF9C32600F007FFFFF3FF2600F00700000000000000000000000000000000
      000000000000}
  end
  inherited PopupMenu: TPopupMenu
    Left = 245
    Top = 11
  end
end
