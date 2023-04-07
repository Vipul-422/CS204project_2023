#include "App.h"
#include"GUI.h"
#include<wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainframe = new MainFrame("C++ GUI");
	mainframe->SetSize(wxSize(1200, 800));
	mainframe->Show();
	return true;
}
