object FormCreateProcess: TFormCreateProcess
  Left = 401
  Top = 227
  BorderIcons = []
  Caption = 'Aguarde: '
  ClientHeight = 167
  ClientWidth = 326
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pbar: TProgressBar
    Left = 8
    Top = 94
    Width = 310
    Height = 19
    Smooth = True
    TabOrder = 0
  end
  object ButCancela: TButton
    Left = 111
    Top = 128
    Width = 107
    Height = 31
    Caption = 'Cancelar'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = ButCancelaClick
  end
  object GBoxCodigo: TGroupBox
    Left = 8
    Top = 19
    Width = 310
    Height = 49
    Caption = 'Estudo Sinap'
    TabOrder = 2
    object EditCodigo: TEdit
      Left = 7
      Top = 19
      Width = 287
      Height = 21
      ReadOnly = True
      TabOrder = 0
    end
  end
  object timer: TTimer
    Enabled = False
    OnTimer = timerTimer
    Left = 48
    Top = 128
  end
end
