object FormResidualSST: TFormResidualSST
  Left = 0
  Top = 0
  Caption = 'Residual'
  ClientHeight = 464
  ClientWidth = 723
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Chart1: TChart
    Left = 0
    Top = 0
    Width = 721
    Height = 465
    Title.Text.Strings = (
      'Residual')
    LeftAxis.Automatic = False
    LeftAxis.AutomaticMaximum = False
    LeftAxis.AutomaticMinimum = False
    LeftAxis.Logarithmic = True
    LeftAxis.Maximum = 1.000000000000000000
    LeftAxis.Minimum = 0.000000000000010000
    View3D = False
    TabOrder = 0
    DefaultCanvas = 'TGDIPlusCanvas'
    ColorPaletteIndex = 13
    object Series1: TFastLineSeries
      Title = 'X-Vel'
      LinePen.Color = 10708548
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series2: TFastLineSeries
      Title = 'Y-Vel'
      LinePen.Color = 3513587
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series3: TFastLineSeries
      Title = 'Z-Vel'
      LinePen.Color = 1330417
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series4: TFastLineSeries
      Title = 'continity'
      LinePen.Color = 11048782
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series5: TFastLineSeries
      Title = 'ke'
      LinePen.Color = 7028779
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
    object Series6: TFastLineSeries
      Title = 'omega'
      LinePen.Color = 6519581
      XValues.Name = 'X'
      XValues.Order = loAscending
      YValues.Name = 'Y'
      YValues.Order = loNone
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 648
    Top = 360
  end
end
