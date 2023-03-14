#pragma once
#include <wx/wx.h>
#include <vector>

using namespace std;

class MainFrame  : public wxFrame
{
public:
	MainFrame(const wxString& title);
	int reqAdd = 4096;
private:
	void OnButtonClicked1(wxCommandEvent& evt);
	void OnButtonClicked2(wxCommandEvent& evt);
	void OnRegClicked(wxCommandEvent& evt);
	void OnMemClicked(wxCommandEvent& evt);
	void onIpChanged(wxCommandEvent& evt);
	void runFunc(wxCommandEvent& evt);
	wxTextCtrl* textField;
	wxButton* button3;
	wxButton* button4;
	wxButton* button5;
	wxButton* button6;
	//wxButton* button7;
	wxButton* regButton;
	wxButton* memButton;
	wxButton* addButton;
	wxStaticText* txtx0;
	wxStaticText* txtx1;
	wxStaticText* txtx2;
	wxStaticText* txtx3;
	wxStaticText* txtx4;
	wxStaticText* txtx5;
	wxStaticText* txtx6;
	wxStaticText* txtx7;
	wxStaticText* txtx8;
	wxStaticText* txtx9;
	wxStaticText* txtx10;
	wxStaticText* txtx11;
	wxStaticText* txtx12;
	wxStaticText* txtx13;
	wxStaticText* txtx14;
	wxStaticText* txtx15;
	wxStaticText* txtx16;
	wxStaticText* txtx17;
	wxStaticText* txtx18;
	wxStaticText* txtx19;
	wxStaticText* txtx20;
	wxStaticText* txtx21;
	wxStaticText* txtx22;
	wxStaticText* txtx23;
	wxStaticText* txtx24;
	wxStaticText* txtx25;
	wxStaticText* txtx26;
	wxStaticText* txtx27;
	wxStaticText* txtx28;
	wxStaticText* txtx29;
	wxStaticText* txtx30;
	wxStaticText* txtx31;
	wxStaticText* regChoice;
	wxStaticText* txtAddress1;
	wxStaticText* txtAddress2;
	wxStaticText* txt0;
	wxStaticText* txt1;
	wxStaticText* txt2;
	wxStaticText* txt3;
	vector<wxTextCtrl*> TextField;
	wxTextCtrl* textFieldx0;
	wxTextCtrl* textFieldx1;
	wxTextCtrl* textFieldx2;
	wxTextCtrl* textFieldx3;
	wxTextCtrl* textFieldx4;
	wxTextCtrl* textFieldx5;
	wxTextCtrl* textFieldx6;
	wxTextCtrl* textFieldx7;
	wxTextCtrl* textFieldx8;
	wxTextCtrl* textFieldx9;
	wxTextCtrl* textFieldx10;
	wxTextCtrl* textFieldx11;
	wxTextCtrl* textFieldx12;
	wxTextCtrl* textFieldx13;
	wxTextCtrl* textFieldx14;
	wxTextCtrl* textFieldx15;
	wxTextCtrl* textFieldx16;
	wxTextCtrl* textFieldx17;
	wxTextCtrl* textFieldx18;
	wxTextCtrl* textFieldx19;
	wxTextCtrl* textFieldx20;
	wxTextCtrl* textFieldx21;
	wxTextCtrl* textFieldx22;
	wxTextCtrl* textFieldx23;
	wxTextCtrl* textFieldx24;
	wxTextCtrl* textFieldx25;
	wxTextCtrl* textFieldx26;
	wxTextCtrl* textFieldx27;
	wxTextCtrl* textFieldx28;
	wxTextCtrl* textFieldx29;
	wxTextCtrl* textFieldx30;
	wxTextCtrl* textFieldx31;
	wxTextCtrl* outField;
	//wxTextCtrl* cslField;
	wxTextCtrl* regField;
	wxTextCtrl* addField;
	wxTextCtrl* addField1;
	wxTextCtrl* mem0;
	wxTextCtrl* mem1;
	wxTextCtrl* mem2;
	wxTextCtrl* mem3;


	wxString txtStr;

	//wxDECLARE_EVENT_TABLE();
};

