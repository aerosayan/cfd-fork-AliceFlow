unit Unitamgmanager;
// 26 ������� 2016.
// ������ ������ ������������ ��� �����������
// ������������ � ���������� �������.
// �������� ������� ��������� ����������� � ������� �����.
// � ����� ����������� ��������� ��������� � ������� ����� ���
// ������� ����� ������ ������.
// ��� ��������� ������������� ������ ��� ��������� ���� ����� v.0.14
// ����� �� ���������� �������� �� �����������.
// ����� � ����� ������������� ��������� �� ��� ������ ����� �� ����� ����
// � �������� �� ����� ����. �� ����� ������ �������������� ���� ������������������
// �� ��� ��������� ����� �� ����� ������ �� ����� ������ ������� �� ��� ����
// ���������.
// ��������� �������� � ���� ��� � ������ ��� ���������� ����� ������������� �����
// ��������� �������� ����� ����� ������� � ������� ��� �������� �������������������
// ������� ������ ����.

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls, Vcl.AppEvnts;

type
  TForm_amg_manager = class(TForm)
    Panel1: TPanel;
    Label1: TLabel;
    ComboBoxmaximumreducedlevels: TComboBox;
    Label2: TLabel;
    ComboBoxinterpolation: TComboBox;
    Label3: TLabel;
    ComboBoxnFinnest: TComboBox;
    Label4: TLabel;
    Label5: TLabel;
    ComboBoxnumberpresmothers: TComboBox;
    ComboBoxnumberpostsweeps: TComboBox;
    Label6: TLabel;
    ComboBoxmemorysize: TComboBox;
    Buttondefault: TButton;
    Label7: TLabel;
    Label8: TLabel;
    Editthreshold: TEdit;
    Labelmagic: TLabel;
    EditmagicT: TEdit;
    Label9: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    Label13: TLabel;
    Label14: TLabel;
    Label15: TLabel;
    Label16: TLabel;
    EditthresholdSpeed: TEdit;
    EditthresholdPressure: TEdit;
    ComboBoxmaximumreducedlevelsSpeed: TComboBox;
    ComboBoxmaximumreducedlevelsPressure: TComboBox;
    ComboBoxnFinnestSpeed: TComboBox;
    ComboBoxnFinnestPressure: TComboBox;
    ComboBoxnumberpresmothersSpeed: TComboBox;
    ComboBoxnumberpresmothersPressure: TComboBox;
    ComboBoxnumberpostsweepsSpeed: TComboBox;
    ComboBoxnumberpostsweepsPressure: TComboBox;
    ComboBoxmemorysizeSpeed: TComboBox;
    ComboBoxmemorysizePressure: TComboBox;
    ApplicationEvents1: TApplicationEvents;
    ComboBoxsmoothertypeTemperature: TComboBox;
    ComboBoxsmoothertypeSpeed: TComboBox;
    ComboBoxsmoothertypePressure: TComboBox;
    Label17: TLabel;
    ComboBoxcoarseningTemp: TComboBox;
    ComboBoxcoarseningSpeed: TComboBox;
    ComboBoxcoarseningPressure: TComboBox;
    Label18: TLabel;
    ComboBoxStabilizationTemp: TComboBox;
    ComboBoxStabilizationSpeed: TComboBox;
    ComboBoxStabilizationPressure: TComboBox;
    Label19: TLabel;
    EditmagicSpeed: TEdit;
    EditmagicPressure: TEdit;
    Label20: TLabel;
    Edit_truncation_T: TEdit;
    Edit_truncation_Speed: TEdit;
    Edit_truncation_Pressure: TEdit;
    CheckBoxtruncationT: TCheckBox;
    CheckBoxtruncationSpeed: TCheckBox;
    CheckBoxtruncationPressure: TCheckBox;
    ComboBoxInterpolationSpeed: TComboBox;
    ComboBoxinterpolationPressure: TComboBox;
    Label21: TLabel;
    CheckBoxprintlogTemperature: TCheckBox;
    CheckBoxprintlogSpeed: TCheckBox;
    CheckBoxprintlogPressure: TCheckBox;
    Label26: TLabel;
    EditthresholdStress: TEdit;
    LabelStress: TLabel;
    ComboBoxmaximumreducedlevelsStress: TComboBox;
    ComboBoxnFinnestStress: TComboBox;
    ComboBoxnumberpresmoothersStress: TComboBox;
    ComboBoxnumberpostsweepsStress: TComboBox;
    ComboBoxmemorysizeStress: TComboBox;
    ComboBoxinterpollationStress: TComboBox;
    CheckBoxtruncationStress: TCheckBox;
    Edittruncation_Stress: TEdit;
    EditmagicStress: TEdit;
    ComboBoxsmoothertypeStress: TComboBox;
    ComboBoxcoarseningStress: TComboBox;
    ComboBoxstabilizationStress: TComboBox;
    CheckBoxprintlogStress: TCheckBox;
    GroupBox1: TGroupBox;
    ComboBoxCFalgorithmandDataStruct_Temperature: TComboBox;
    ComboBoxCFalgorithmandDataStruct_Speed: TComboBox;
    ComboBoxCFalgorithmandDataStruct_Pressure: TComboBox;
    ComboBoxCFalgorithmandDataStruct_Stress: TComboBox;
    Label22: TLabel;
    CheckBoxTemperatureMatrixPortrait: TCheckBox;
    CheckBoxSpeedMatrixPortrait: TCheckBox;
    CheckBoxPressureMatrixPortrait: TCheckBox;
    CheckBoxStressMatrixPortrait: TCheckBox;
    ComboBoxSort: TComboBox;
    CheckBoxDiagonalDominant: TCheckBox;
    CheckBoxStrongTranspose: TCheckBox;
    procedure ButtondefaultClick(Sender: TObject);
    procedure ApplicationEvents1Message(var Msg: tagMSG; var Handled: Boolean);
    procedure CheckBoxtruncationTClick(Sender: TObject);
    procedure CheckBoxtruncationSpeedClick(Sender: TObject);
    procedure CheckBoxtruncationPressureClick(Sender: TObject);
    procedure Edittruncation_StressClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form_amg_manager: TForm_amg_manager;

implementation

{$R *.dfm}

// ������ ����� �������������.
procedure TForm_amg_manager.ApplicationEvents1Message(var Msg: tagMSG;
  var Handled: Boolean);
begin
     if (msg.message=WM_SYSCOMMAND)and(msg.wParam=SC_MINIMIZE)
 then
  msg.message:=0;
end;

 // ���������� ��������� �������� �� ���������.
procedure TForm_amg_manager.ButtondefaultClick(Sender: TObject);
begin

     // 1 - QUICK SORT
     // 2 - HEAP SORT
     // 3 - TIM SORT (��� ��������)
     // 4 - in development sort
     ComboBoxSort.ItemIndex:=0; // 0 - COUNTING SORT

     CheckBoxTemperatureMatrixPortrait.Checked:=false;
     CheckBoxSpeedMatrixPortrait.Checked:=false;
     CheckBoxPressureMatrixPortrait.Checked:=false;
     CheckBoxStressMatrixPortrait.Checked:=false;

    // 0 - AVL Tree, 1 - SPLAY Tree, 2 - Binary Heap.
    // 3 - Treap.
    ComboBoxCFalgorithmandDataStruct_Temperature.ItemIndex:=2;
    ComboBoxCFalgorithmandDataStruct_Speed.ItemIndex:=2;
    ComboBoxCFalgorithmandDataStruct_Pressure.ItemIndex:=2;
    ComboBoxCFalgorithmandDataStruct_Stress.ItemIndex:=2;

    // ������ ����:
    CheckBoxprintlogTemperature.Checked:=true;
    CheckBoxprintlogSpeed.Checked:=true;
    CheckBoxprintlogPressure.Checked:=true;
    CheckBoxprintlogStress.Checked:=true;

    CheckBoxStrongTranspose.Checked:=true;

    // ��� ������������� ������� �� ����������� ���� ��������� ������ ���������
    // �� ���������. �������� ������� ��������� ����������� ������.
    // ����� �� ���� � ���� ������ ��������� �������� ������.


    // ���������� ��������� �� ���������.
    ComboBoxmaximumreducedlevels.ItemIndex:=0; // ������� � ����� ������ ����������� ������ ������������ (������������).
    ComboBoxmaximumreducedlevelsSpeed.ItemIndex:=0;
    ComboBoxmaximumreducedlevelsPressure.ItemIndex:=0;
    ComboBoxmaximumreducedlevelsStress.ItemIndex:=0;

    ComboBoxinterpolation.ItemIndex:=3; // ����� ��������� �������������.
    ComboBoxinterpolationSpeed.ItemIndex:=3;
    ComboBoxinterpolationPressure.ItemIndex:=3;
    ComboBoxinterpollationStress.ItemIndex:=3;

    ComboBoxnFinnest.ItemIndex:=1; // nFinnest
    ComboBoxnFinnestSpeed.ItemIndex:=1;
    ComboBoxnFinnestPressure.ItemIndex:=1;
    ComboBoxnFinnestStress.ItemIndex:=1;

    ComboBoxnumberpresmothers.ItemIndex:=1; // nu1
    ComboBoxnumberpresmothersSpeed.ItemIndex:=1;
    ComboBoxnumberpresmothersPressure.ItemIndex:=1;
    ComboBoxnumberpresmoothersStress.ItemIndex:=1;

    ComboBoxnumberpostsweeps.ItemIndex:=2; //nu2
    ComboBoxnumberpostsweepsSpeed.ItemIndex:=2;
    ComboBoxnumberpostsweepsPressure.ItemIndex:=2;
    ComboBoxnumberpostsweepsStress.ItemIndex:=2;

    ComboBoxmemorysize.ItemIndex:=5;  // memory number A size
    ComboBoxmemorysizeSpeed.ItemIndex:=5;
    ComboBoxmemorysizePressure.ItemIndex:=5;
    ComboBoxmemorysizeStress.ItemIndex:=5;


   // �� ��������� �� �� ���������� ILU2 ������������.
   // 0 - standart ������������ �����, 1 - ILU0, 1 - ILU2.
   ComboBoxsmoothertypeTemperature.ItemIndex:=0; // 0 - Standart.
   ComboBoxsmoothertypeSpeed.ItemIndex:=0; // 0 - Standart.
   ComboBoxsmoothertypePressure.ItemIndex:=0; // 0 - Standart.
   ComboBoxsmoothertypeStress.ItemIndex:=0; // 0 - Standart.

   if (FormatSettings.DecimalSeparator=',') then
   begin
      Editthreshold.Text:='0,24';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       Editthreshold.Text:='0.24';
   end;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      EditmagicT.Text:='0,4';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditmagicT.Text:='0.4';
   end;

   if (FormatSettings.DecimalSeparator=',') then
   begin
      EditmagicSpeed.Text:='0,4';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditmagicSpeed.Text:='0.4';
   end;

   if (FormatSettings.DecimalSeparator=',') then
   begin
      EditmagicPressure.Text:='0,4';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditmagicPressure.Text:='0.4';
   end;

   if (FormatSettings.DecimalSeparator=',') then
   begin
      EditmagicStress.Text:='0,4';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditmagicStress.Text:='0.4';
   end;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      EditthresholdSpeed.Text:='0,24';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditthresholdSpeed.Text:='0.24';
   end;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      EditthresholdPressure.Text:='0,24';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditthresholdPressure.Text:='0.24';
   end;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      EditthresholdStress.Text:='0,24';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       EditthresholdStress.Text:='0.24';
   end;

   // AmgSplitting standart vs RS2. 2.01.2017
   // ST option - 8.01.2017.
   // neg con only begin 19.01.2017
   ComboBoxcoarseningTemp.ItemIndex:=2; // standart coarsening  all con
   ComboBoxcoarseningSpeed.ItemIndex:=2; // standart coarsening all con
   ComboBoxcoarseningPressure.ItemIndex:=2; // standart coarsening all con
   ComboBoxcoarseningStress.ItemIndex:=2; // standart coarsening all con

   // Stabilization BiCGStab + amg (�����).
   // 8.01.2017
   ComboBoxStabilizationTemp.ItemIndex:=0; // none
   ComboBoxStabilizationSpeed.ItemIndex:=0; // none
   ComboBoxStabilizationPressure.ItemIndex:=0; // none
   ComboBoxStabilizationStress.ItemIndex:=0; // none

    if (FormatSettings.DecimalSeparator=',') then
   begin
      Edit_truncation_T.Text:='0,2';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       Edit_truncation_T.Text:='0.2';
   end;
   Edit_truncation_T.Visible:=false;
   CheckBoxtruncationT.Checked:=true;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      Edit_truncation_Speed.Text:='0,2';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       Edit_truncation_Speed.Text:='0.2';
   end;
   Edit_truncation_Speed.Visible:=false;
   CheckBoxtruncationSpeed.Checked:=true;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      Edit_truncation_Pressure.Text:='0,2';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       Edit_truncation_Pressure.Text:='0.2';
   end;
   Edit_truncation_Pressure.Visible:=false;
   CheckBoxtruncationPressure.Checked:=true;

    if (FormatSettings.DecimalSeparator=',') then
   begin
      Edittruncation_Stress.Text:='0,2';
   end
    else  if (FormatSettings.DecimalSeparator='.') then
   begin
       Edittruncation_Stress.Text:='0.2';
   end;
   Edittruncation_Stress.Visible:=false;
   CheckBoxtruncationStress.Checked:=true;

end;

// truncation interpolation for Pressure click.
procedure TForm_amg_manager.CheckBoxtruncationPressureClick(Sender: TObject);
var
  s1 : String;
  i : Integer;
begin
   if (CheckBoxtruncationPressure.Checked) then
   begin
      Edit_truncation_Pressure.Visible:=false;
   end
   else
   begin
      Edit_truncation_Pressure.Visible:=true;
      if (FormatSettings.DecimalSeparator=',') then
      begin
         s1:=Edit_truncation_Pressure.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]=',') then s1[i]:='.';
         end;
         Edit_truncation_Pressure.Text:=s1;
      end;
      if (FormatSettings.DecimalSeparator='.') then
      begin
         s1:=Edit_truncation_Pressure.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]='.') then s1[i]:=',';
         end;
         Edit_truncation_Pressure.Text:=s1;
      end;
   end;
end;

// truncation interpolation for Speed click.
procedure TForm_amg_manager.CheckBoxtruncationSpeedClick(Sender: TObject);
var
  s1 : String;
  i : Integer;
begin
   if (CheckBoxtruncationSpeed.Checked) then
   begin
      Edit_truncation_Speed.Visible:=false;
   end
   else
   begin
      Edit_truncation_Speed.Visible:=true;
      if (FormatSettings.DecimalSeparator=',') then
      begin
         s1:=Edit_truncation_Speed.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]=',') then s1[i]:='.';
         end;
         Edit_truncation_Speed.Text:=s1;
      end;
      if (FormatSettings.DecimalSeparator='.') then
      begin
         s1:=Edit_truncation_Speed.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]='.') then s1[i]:=',';
         end;
         Edit_truncation_Speed.Text:=s1;
      end;
   end;
end;

// truncation interpolation for Temperature click.
procedure TForm_amg_manager.CheckBoxtruncationTClick(Sender: TObject);
var
  s1 : String;
  i : Integer;
begin
   if (CheckBoxtruncationT.Checked) then
   begin
      Edit_truncation_T.Visible:=false;
   end
   else
   begin
      Edit_truncation_T.Visible:=true;
      if (FormatSettings.DecimalSeparator=',') then
      begin
         s1:=Edit_truncation_T.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]=',') then s1[i]:='.';
         end;
         Edit_truncation_T.Text:=s1;
      end;
      if (FormatSettings.DecimalSeparator='.') then
      begin
         s1:=Edit_truncation_T.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]='.') then s1[i]:=',';
         end;
         Edit_truncation_T.Text:=s1;
      end;
   end;
end;

procedure TForm_amg_manager.Edittruncation_StressClick(Sender: TObject);
var
  s1 : String;
  i : Integer;
begin
    if (CheckBoxtruncationStress.Checked) then
   begin
      Edittruncation_Stress.Visible:=false;
   end
   else
   begin
      Edittruncation_Stress.Visible:=true;
      if (FormatSettings.DecimalSeparator=',') then
      begin
         s1:=Edittruncation_Stress.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]=',') then s1[i]:='.';
         end;
         Edittruncation_Stress.Text:=s1;
      end;
      if (FormatSettings.DecimalSeparator='.') then
      begin
         s1:=Edittruncation_Stress.Text;
         for i := 1 to length(s1) do
         begin
            if (s1[i]='.') then s1[i]:=',';
         end;
         Edittruncation_Stress.Text:=s1;
      end;
   end;
end;

end.
