object Form1: TForm1
  Left = 0
  Top = 0
  Caption = '2048 Game'
  ClientHeight = 535
  ClientWidth = 662
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 32
    Top = 94
    Width = 401
    Height = 401
    AutoSize = True
  end
  object Label1: TLabel
    Left = 48
    Top = 32
    Width = 207
    Height = 43
    Caption = '2048 Game'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clTeal
    Font.Height = -36
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 432
    Top = 24
    Width = 60
    Height = 23
    Caption = 'Score:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 528
    Top = 24
    Width = 111
    Height = 23
    Caption = 'High Score:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 472
    Top = 454
    Width = 145
    Height = 41
    AutoSize = False
    Caption = #39'w'#39' - UP; '#39's'#39' - DOWN; '#39'a'#39' - LEFT; '#39'd'#39' - RIGHT'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    WordWrap = True
  end
  object RadioGroup1: TRadioGroup
    Left = 454
    Top = 168
    Width = 185
    Height = 105
    Caption = 'Choose target number to win:'
    ItemIndex = 0
    Items.Strings = (
      '128'
      '256'
      '512'
      '1024'
      '2048')
    TabOrder = 0
  end
  object Button1: TButton
    Left = 488
    Top = 360
    Width = 97
    Height = 25
    Caption = 'Start New Game'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 488
    Top = 408
    Width = 97
    Height = 25
    Caption = 'Reset High Score'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 20
    OnTimer = Timer1Timer
    Left = 280
    Top = 32
  end
end
