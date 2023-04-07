#pragma once
#include <wx/wx.h>
#include <vector>

using namespace std;

class MainFrame : public wxFrame
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
	void outputFile(wxCommandEvent& evt);
	void hideFile(wxCommandEvent& evt);
	void stepClicked(wxCommandEvent& evt);
	wxButton* buttons[100];
	wxTextCtrl* txtField[200];
	wxStaticText* txt[100];
	
};


