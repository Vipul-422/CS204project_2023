#include "GUI.h"
#include "components.h"
#include "riscv.h"
#include "functions.h"
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "wx/wx.h"

using namespace std;

wxFont font1(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
wxColour myColor(0, 0, 255);
string lines[200];
int pc = 0;
int countLine = 0;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	int memAdd[32];
	memAdd[0] = reqAdd;
	char outAdd[13][10];
	outAdd[0][0] = '0';
	outAdd[0][1] = 'x';
	sprintf(&outAdd[0][2], "%X", memAdd[0]);
	string finalOut;
	finalOut.append(outAdd[0]);
	finalOut.append("\n");
	for (int i = 1; i < 13; i++) {
		memAdd[i] = memAdd[i - 1] + 4;
		outAdd[i][0] = '0';
		sprintf(&outAdd[i][2], "%X", memAdd[i]);
		outAdd[i][1] = 'x';
		finalOut.append(outAdd[i]);
		finalOut.append("\n");
	}
	finalOut.pop_back();

	string value[4][13];
	int temp;
	string finalValue[4];
	for (int i = 0; i < 4; i++) {
		temp = reqAdd + i;
		for (int j = 0; j < 13; j++) {
			value[i][j] = "0";
			temp += 4;
			finalValue[i].append(value[i][j]);
			finalValue[i].append("\n");
		}
	}

	wxPanel* panel = new wxPanel(this);

	



	buttons[0] = new wxButton(panel, wxID_ANY, "Editor", wxPoint(600, 20), wxSize(100, 60));
	buttons[1] = new wxButton(panel, wxID_ANY, "Simulator", wxPoint(800, 20), wxSize(100, 60));
	buttons[2] = new wxButton(panel, wxID_ANY, "Run", wxPoint(700, 750), wxSize(60, 30));
	buttons[3] = new wxButton(panel, wxID_ANY, "Step", wxPoint(800,750), wxSize(60, 30));
	buttons[4] = new wxButton(panel, wxID_ANY, "Output", wxPoint(900, 750), wxSize(60, 30));;
	buttons[5] = new wxButton(panel, wxID_ANY, "Hide", wxPoint(1000, 750), wxSize(60, 30));
	buttons[6] = new wxButton(panel, wxID_ANY, "Registers", wxPoint(1100, 50), wxSize(100, 40));
	buttons[7] = new wxButton(panel, wxID_ANY, "Memory", wxPoint(800, 150), wxSize(80, 40));

	txtField[0] = new wxTextCtrl(panel, wxID_ANY, "Enter your code here", wxPoint(20, 100), wxSize(1400, 700), wxTE_MULTILINE);

	for (int i = 0; i < 32; i++) {
		if (i < 16) {
			txt[i] = new wxStaticText(panel, wxID_ANY, wxString::Format("x%d", i), wxPoint(1000, 100 + i * 40));
		}
		else {
			txt[i] = new wxStaticText(panel, wxID_ANY, wxString::Format("x%d", i), wxPoint(1250, 100 + (i - 16) * 40));
		}

	}
	txt[40] = new wxStaticText(panel, wxID_ANY, "PC", wxPoint(100, 60));
	txt[41] = new wxStaticText(panel, wxID_ANY, "Machine Code", wxPoint(400, 60));

	string outRegs[32];
	for (int i = 0; i < 32; i++) {
		outRegs[i] = "0";
	}

	for (int i = 0; i < 32; i++) {
		if (i < 16) {
			txtField[i + 10] = new wxTextCtrl(panel, wxID_ANY, outRegs[i], wxPoint(1050, 100 + i * 40), wxSize(100, -1));
		}
		else {
			txtField[i + 10] = new wxTextCtrl(panel, wxID_ANY, outRegs[i], wxPoint(1300, 100 + (i - 16) * 40), wxSize(100, -1));
		}
	}
	
	/*txt[41] = new wxStaticText(panel, wxID_ANY, "Address", wxPoint(450, 200));
	txt[42] = new wxStaticText(panel, wxID_ANY, "+0", wxPoint(590, 200));
	txt[43] = new wxStaticText(panel, wxID_ANY, "+1", wxPoint(700, 200));
	txt[44] = new wxStaticText(panel, wxID_ANY, "+2", wxPoint(800, 200));
	txt[45] = new wxStaticText(panel, wxID_ANY, "+3", wxPoint(900, 200));
	txt[46] = new wxStaticText(panel, wxID_ANY, "Address:", wxPoint(400, 750));
	*/
	//txtField[2] = new wxTextCtrl(panel, wxID_ANY, finalOut, wxPoint(380, 220), wxSize(150, 500), wxTE_MULTILINE);
	txtField[2] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(50, 85), wxSize(200, 750), wxTE_MULTILINE);
	txtField[3] = new wxTextCtrl(panel, wxID_ANY, finalValue[0], wxPoint(700, 220), wxSize(250, 500), wxTE_MULTILINE);
	txtField[4] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(300, 85), wxSize(300, 750), wxTE_MULTILINE);
	
	for (int i = 43; i < 80; i++) {
		txtField[i] = new wxTextCtrl(panel, wxID_ANY," ", wxPoint(50, 85 + (i - 43) * 20), wxSize(80, 20));
	}

	for (int i = 80; i < 117; i++) {
		txtField[i] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(135, 85 + (i - 80) * 20), wxSize(80, 20));
	}

	for (int i = 120; i < 157; i++) {
		txtField[i] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(300, 85 + (i - 120) * 20), wxSize(130, 20));
	}

	for (int i = 157; i < 194; i++) {
		txtField[i] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(440, 85 + (i - 157) * 20), wxSize(130, 20));
	}

	txtField[5] = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(300, 85), wxSize(300, 750), wxTE_MULTILINE);

	txtField[3]->SetFont(font1);
	buttons[0]->SetFont(font1);
	buttons[1]->SetFont(font1);

	for (int i = 2; i < 8; i++) {
		buttons[i]->Hide();
		buttons[i]->SetFont(font1);
	}
	for (int i = 0; i < 32; i++) {
		txt[i]->Hide();
		txt[i]->SetFont(font1);
	}
	for (int i = 40; i < 42; i++) {
		txt[i]->Hide();
		txt[i]->SetFont(font1);
	}
	for (int i = 2; i < 6; i++) {
		txtField[i]->Hide();
	}
	for (int i = 10; i < 42; i++) {
		txtField[i]->Hide();
	}

	for (int i = 43; i < 80; i++) {
		txtField[i]->Hide();
	}
	for (int i = 80; i < 117; i++) {
		txtField[i]->Hide();
	}
	for (int i = 120; i < 157; i++) {
		txtField[i]->Hide();
	}
	for (int i = 157; i < 194; i++) {
		txtField[i]->Hide();
	}
	
	buttons[0]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked1, this);
	buttons[1]->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked2, this);
	buttons[4]->Bind(wxEVT_BUTTON, &MainFrame::outputFile, this);
	buttons[5]->Bind(wxEVT_BUTTON, &MainFrame::hideFile, this);
	buttons[6]->Bind(wxEVT_BUTTON, &MainFrame::OnRegClicked, this);
	buttons[7]->Bind(wxEVT_BUTTON, &MainFrame::OnMemClicked, this);
	txtField[0]->Bind(wxEVT_TEXT, &MainFrame::onIpChanged, this);
	buttons[2]->Bind(wxEVT_BUTTON, &MainFrame::runFunc, this);
	buttons[3]->Bind(wxEVT_BUTTON, &MainFrame::stepClicked, this);
}

void MainFrame::onIpChanged(wxCommandEvent& evt) {
	wxString newText = txtField[0]->GetValue();
	evt.Skip();
	ofstream fout("student.mc");
	fout << newText;
	fout.close();
	
}

void MainFrame::runFunc(wxCommandEvent& evt) {
	string main = "g++ main.cpp riscv.cpp functions.cpp components.cpp -o result.exe";
	system(main.c_str());

	main = "result.exe";
	system(main.c_str());

	extern Memory mem;

	string outRegs[32];
	ifstream fin;
	fin.open("Registers.txt");
	for (int i = 0; i < 32; i++) {
		getline(fin, outRegs[i]);
	}

	for (int i = 10; i < 42; i++) {
		txtField[i]->SetValue(outRegs[i - 10]);
	}

}


void MainFrame::OnButtonClicked1(wxCommandEvent& evt) {
	txtField[0]->Show();
	txtField[2]->Hide();
	txtField[3]->Hide();
	txtField[4]->Hide();
	for (int i = 2; i < 8; i++) {
		buttons[i]->Hide();
	}
	for (int i = 0; i < 32; i++) {
		txt[i]->Hide();
	}
	for (int i = 40; i < 42; i++) {
		txt[i]->Hide();
	}
	for (int i = 2; i < 5; i++) {
		txtField[i]->Hide();
	}
	for (int i = 10; i < 42; i++) {
		txtField[i]->Hide();
	}
	for (int i = 43; i < 80; i++) {
		txtField[i]->Hide();
	}
	for (int i = 80; i < 117; i++) {
		txtField[i]->Hide();
	}
	for (int i = 120; i < 157; i++) {
		txtField[i]->Hide();
	}
	for (int i = 157; i < 194; i++) {
		txtField[i]->Hide();
	}
}

void MainFrame::OnButtonClicked2(wxCommandEvent& evt) {

	ifstream file("student.mc");
	
	string line;
	while (getline(file, line)) {
		lines[countLine]=line;
		countLine++;
	}
	file.close();

	txtField[0]->Hide();
	txtField[2]->Show();
	txtField[3]->Show();
	txtField[4]->Show();
	for (int i = 2; i < 8; i++) {
		buttons[i]->Show();
	}
	for (int i = 0; i < 32; i++) {
		txt[i]->Show();
	}
	for (int i = 40; i < 42; i++) {
		txt[i]->Show();
	}
	for (int i = 2; i < 5; i++) {
		txtField[i]->Show();
	}
	for (int i = 10; i < 42; i++) {
		txtField[i]->Show();
	}
	for (int i = 43; i < 80; i++) {
		txtField[i]->Show();
	}
	for (int i = 80; i < 117; i++) {
		txtField[i]->Show();
	}
	for (int i = 120; i < 157; i++) {
		txtField[i]->Show();
		txtField[i]->SetValue(wxString::Format("%s", lines[i - 120]));
	}
	for (int i = 157; i < 194; i++) {
		txtField[i]->Show();
		txtField[i]->SetValue(wxString::Format("%s", lines[i - 120]));
	}
	for (int i = 0; i < countLine; i++) {
		txtField[i + 43]->SetValue(wxString::Format("%d", i));
	}
}

void MainFrame::OnRegClicked(wxCommandEvent& evt) {

	string outRegs[32];
	ifstream fin;
	fin.open("Registers.txt");
	for (int i = 0; i < 32; i++) {
		getline(fin, outRegs[i]);
	}
	fin.close();


	for (int i = 10; i < 42; i++) {
		txtField[i]->SetValue(outRegs[i - 10]);
	}

}

void MainFrame::OnMemClicked(wxCommandEvent& evt) {
	ifstream fin;
	string finalValue;
	fin.open("Memory.txt");
	string value[25000];
	string stmp;
	for (int i = 0; i < 25000; i++) {
		getline(fin, value[i]);
		finalValue.append(value[i]);
		finalValue.append("\n");
	}

	txtField[3]->SetValue(finalValue);

}

void MainFrame::stepClicked(wxCommandEvent& evt) {
	txtField[43+pc]->SetBackgroundColour(myColor);
	txtField[120+pc]->SetBackgroundColour(myColor);
	txtField[43 + pc]->Refresh();
	txtField[120 + pc]->Refresh();

	ofstream file("student.mc", ios::trunc);
	int i = 0;
	while (i <= pc) {
		file << lines[i];
		file << "\n";
		i++;
	}
	file.close();
	

	pc++;

	string main = "g++ main.cpp riscv.cpp functions.cpp components.cpp -o result.exe";
	system(main.c_str());

	main = "result.exe";
	system(main.c_str());
}

void MainFrame::outputFile(wxCommandEvent& evt) {
	txtField[5]->Show();
}

void MainFrame::hideFile(wxCommandEvent& evt) {
	txtField[5]->Hide();
}