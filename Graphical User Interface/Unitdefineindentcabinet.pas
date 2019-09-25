unit Unitdefineindentcabinet;
  // ������������ �������� �������� �� ������������� ����������
  // ��������� ������ �������.
interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls,
  Vcl.Imaging.pngimage;

type
  TFormcabinetindent = class(TForm)
    Image1: TImage;
    RadioGroup1: TRadioGroup;
    GroupBox1: TGroupBox;
    ButtonNext: TButton;
    PanelH_only: TPanel;
    Label1: TLabel;
    Edithonly: TEdit;
    Labelunion: TLabel;
    procedure ButtonNextClick(Sender: TObject);
    procedure RadioGroup1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Formcabinetindent: TFormcabinetindent;

implementation

{$R *.dfm}

uses UnitVariables;

procedure TFormcabinetindent.ButtonNextClick(Sender: TObject);
var
   bOk : Boolean;
   r : Real;
   s : String;
   i : Integer;
begin
    case RadioGroup1.ItemIndex of
      0 : begin
             // ��� �������.
              Close;
          end;
      1 : begin
             // � ��������.
             bOk:=true;
             s:=Trim(Edithonly.Text);
              if (FormatSettings.DecimalSeparator='.') then
              begin
                  for i:=1 to length(s) do
                  begin
                     if (s[i]=',') then s[i]:='.';
                  end;
                  Edithonly.Text:=Trim(s);
               end;
               if (FormatSettings.DecimalSeparator=',') then
               begin
                  for i:=1 to length(s) do
                  begin
                     if (s[i]='.') then s[i]:=',';
                  end;
                  Edithonly.Text:=Trim(s);
               end;
                if bOk then r:=FormVariables.my_real_convert(s,bOk);
                if (bOk) then
                begin
                   Edithonly.Color:=clwhite;
                   Close;
                end
                else
                begin
                   Edithonly.Color:=clred;
                end;
          end;
   end;

end;

// ����� ���� ������.
procedure TFormcabinetindent.RadioGroup1Click(Sender: TObject);
begin
   case RadioGroup1.ItemIndex of
      0 : begin
             // ��� �������.
             PanelH_only.Visible:=false;
          end;
      1 : begin
             // � ��������.
             PanelH_only.Visible:=true;
          end;
      2 : begin
             // ����������
             PanelH_only.Visible:=false;
             RadioGroup1.ItemIndex:=0;
             ShowMessage('� �������� ����������.');
          end;
      3 : begin
             // ����������
             PanelH_only.Visible:=false;
             RadioGroup1.ItemIndex:=0;
             ShowMessage('� �������� ����������.');
          end;
   end;
end;

end.
