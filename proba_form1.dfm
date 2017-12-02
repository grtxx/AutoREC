object Form1: TForm1
  Left = 268
  Top = 610
  AutoScroll = False
  Caption = 'Form1'
  ClientHeight = 70
  ClientWidth = 784
  Color = clGray
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Button2'
    TabOrder = 1
    OnClick = Button2Click
  end
  object StaticText1: TStaticText
    Left = 88
    Top = 8
    Width = 16
    Height = 17
    AutoSize = False
    BevelKind = bkFlat
    Color = clBlack
    ParentColor = False
    TabOrder = 2
  end
  object setupbutton: TButton
    Left = 88
    Top = 40
    Width = 75
    Height = 25
    Caption = 'Be'#225'll'#237't'#225's'
    TabOrder = 3
  end
  object Timer1: TTimer
    Interval = 10
    OnTimer = Timer1Timer
    Left = 344
    Top = 8
  end
end
