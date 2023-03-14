#include "MainFrame.h"
#include "../include/components.h"
#include "../include/riscv.h"
#include "../inlcude/functions.h"
#include <iomanip>
#include <fstream>
#include <string>
#include "wx/wx.h"

using namespace std;

enum IDs {
	BUTTON_ID1 = 10,
	BUTTON_ID2 = 20,
	BUTTON_ID3 = 30,
	BUTTON_ID4 = 40,
	BUTTON_ID5 = 50,
	BUTTON_ID6 = 60,
	BUTTON_ID7 = 70,
};

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
	wxButton* button1 = new wxButton(panel, BUTTON_ID1, "Editor", wxPoint(600, 20), wxSize(100, 40));
	wxButton* button2 = new wxButton(panel, BUTTON_ID2, "Simulator", wxPoint(800, 20), wxSize(100, 40));
	
	textField = new wxTextCtrl(panel, wxID_ANY, "Enter your code here", wxPoint(20, 80), wxSize(1400, 700), wxTE_MULTILINE);
	
	outField = new wxTextCtrl(panel, wxID_ANY, "Machine code", wxPoint(20, 200), wxSize(350, 600), wxTE_MULTILINE);
	//cslField = new wxTextCtrl(panel, wxID_ANY, "console output", wxPoint(20, 660), wxSize(950, 150), wxTE_MULTILINE);
	regField = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(1050, 100), wxSize(400, 700));
	outField->Hide();
	//cslField->Hide();
	regField->Hide();
	
	button3 = new wxButton(panel, BUTTON_ID3, "Run", wxPoint(300, 100), wxSize(60, 30));
	button4 = new wxButton(panel, BUTTON_ID4, "Step", wxPoint(400, 100), wxSize(60, 30));
	button5 = new wxButton(panel, BUTTON_ID5, "Prev", wxPoint(500, 100), wxSize(60, 30));
	button6 = new wxButton(panel, BUTTON_ID6, "Reset", wxPoint(600, 100), wxSize(60, 30));
    //button7 = new wxButton(panel, BUTTON_ID7, "Dump", wxPoint(700, 100), wxSize(80, 40));
	
	regButton = new wxButton(panel, wxID_ANY, "Registers", wxPoint(1100, 50), wxSize(80, 40));
	memButton = new wxButton(panel, wxID_ANY, "Memory", wxPoint(600, 150), wxSize(80, 40));

	addButton = new wxButton(panel, wxID_ANY, "Go", wxPoint(720, 750), wxSize(50, 30));

	txtx0 = new wxStaticText(panel, wxID_ANY, "zero", wxPoint(1000, 100));
	txtx1 = new wxStaticText(panel, wxID_ANY, "ra (x1)", wxPoint(1000, 140));
	txtx2 = new wxStaticText(panel, wxID_ANY, "sp (x2)", wxPoint(1000, 180));
	txtx3 = new wxStaticText(panel, wxID_ANY, "gp (x3)", wxPoint(1000, 220));
	txtx4 = new wxStaticText(panel, wxID_ANY, "tp (x4)", wxPoint(1000, 260));
	txtx5 = new wxStaticText(panel, wxID_ANY, "t0 (x5)", wxPoint(1000, 300));
	txtx6 = new wxStaticText(panel, wxID_ANY, "t1 (x6)", wxPoint(1000, 340));
	txtx7 = new wxStaticText(panel, wxID_ANY, "t2 (x7)", wxPoint(1000, 380));
	txtx8 = new wxStaticText(panel, wxID_ANY, "s0 (x8)", wxPoint(1000, 420));
	txtx9 = new wxStaticText(panel, wxID_ANY, "s1 (x9)", wxPoint(1000, 460));
	txtx10 = new wxStaticText(panel, wxID_ANY, "a0 (x10)", wxPoint(1000, 500));
	txtx11 = new wxStaticText(panel, wxID_ANY, "a1 (x11)", wxPoint(1000, 540));
	txtx12 = new wxStaticText(panel, wxID_ANY, "a2 (x12)", wxPoint(1000, 580));
	txtx13 = new wxStaticText(panel, wxID_ANY, "a3 (x13)", wxPoint(1000, 620));
	txtx14 = new wxStaticText(panel, wxID_ANY, "a4 (x14)", wxPoint(1000, 660));
	txtx15 = new wxStaticText(panel, wxID_ANY, "a5 (x15)", wxPoint(1000, 700));
	txtx16 = new wxStaticText(panel, wxID_ANY, "a6 (x16)", wxPoint(1000, 740));
	txtx17 = new wxStaticText(panel, wxID_ANY, "a7 (x17)", wxPoint(1250, 100));
	txtx18 = new wxStaticText(panel, wxID_ANY, "s2 (x18)", wxPoint(1250, 140));
	txtx19 = new wxStaticText(panel, wxID_ANY, "s3 (x19)", wxPoint(1250, 180));
	txtx20 = new wxStaticText(panel, wxID_ANY, "s4 (x20)", wxPoint(1250, 220));
	txtx21 = new wxStaticText(panel, wxID_ANY, "s5 (x21)", wxPoint(1250, 260));
	txtx22 = new wxStaticText(panel, wxID_ANY, "s6 (x22)", wxPoint(1250, 300));
	txtx23 = new wxStaticText(panel, wxID_ANY, "s7 (x23)", wxPoint(1250, 340));
	txtx24 = new wxStaticText(panel, wxID_ANY, "s8 (x24)", wxPoint(1250, 380));
	txtx25 = new wxStaticText(panel, wxID_ANY, "s9 (x25)", wxPoint(1250, 420));
	txtx26 = new wxStaticText(panel, wxID_ANY, "s10 (x26)", wxPoint(1250, 460));
	txtx27 = new wxStaticText(panel, wxID_ANY, "s11 (x27)", wxPoint(1250, 500));
	txtx28 = new wxStaticText(panel, wxID_ANY, "t3 (x28)", wxPoint(1250, 540));
	txtx29 = new wxStaticText(panel, wxID_ANY, "t4 (x29)", wxPoint(1250, 580));
	txtx30 = new wxStaticText(panel, wxID_ANY, "t5 (x30)", wxPoint(1250, 620));
	txtx31 = new wxStaticText(panel, wxID_ANY, "t6 (x31)", wxPoint(1250, 660));
	regChoice = new wxStaticText(panel, wxID_ANY, "Display Setting", wxPoint(1250, 720));

	
	string outRegs[32];
	for (int i = 0; i < 32; i++) {
		outRegs[i] = "0";
	}
	outRegs[2] = "100000";

	wxTextCtrl* temp1;
	//for (int i = 0; i < 32; i++) {
	//	temp1 = new wxTextCtrl(panel, wxID_ANY, outRegs[i], wxPoint(1050, 100), wxSize(100, -1), wxTE_MULTILINE);
	//	TextField.push_back(temp1);
	//}
	textFieldx0 = new wxTextCtrl(panel, wxID_ANY, outRegs[0], wxPoint(1050, 100), wxSize(100, -1));
	textFieldx1 = new wxTextCtrl(panel, wxID_ANY, outRegs[1], wxPoint(1050, 140), wxSize(100, -1));
	textFieldx2 = new wxTextCtrl(panel, wxID_ANY, outRegs[2], wxPoint(1050, 180), wxSize(100, -1));
	textFieldx3 = new wxTextCtrl(panel, wxID_ANY, outRegs[3], wxPoint(1050, 220), wxSize(100, -1));
	textFieldx4 = new wxTextCtrl(panel, wxID_ANY, outRegs[4], wxPoint(1050, 260), wxSize(100, -1));
	textFieldx5 = new wxTextCtrl(panel, wxID_ANY, outRegs[5], wxPoint(1050, 300), wxSize(100, -1));
	textFieldx6 = new wxTextCtrl(panel, wxID_ANY, outRegs[6], wxPoint(1050, 340), wxSize(100, -1));
	textFieldx7 = new wxTextCtrl(panel, wxID_ANY, outRegs[7], wxPoint(1050, 380), wxSize(100, -1));
	textFieldx8 = new wxTextCtrl(panel, wxID_ANY, outRegs[8], wxPoint(1050, 420), wxSize(100, -1));
	textFieldx9 = new wxTextCtrl(panel, wxID_ANY, outRegs[9], wxPoint(1050, 460), wxSize(100, -1));
	textFieldx10 = new wxTextCtrl(panel, wxID_ANY, outRegs[10], wxPoint(1050, 500), wxSize(100, -1));
	textFieldx11 = new wxTextCtrl(panel, wxID_ANY, outRegs[11], wxPoint(1050, 540), wxSize(100, -1));
	textFieldx12 = new wxTextCtrl(panel, wxID_ANY, outRegs[12], wxPoint(1050, 580), wxSize(100, -1));
	textFieldx13 = new wxTextCtrl(panel, wxID_ANY, outRegs[13], wxPoint(1050, 620), wxSize(100, -1));
	textFieldx14 = new wxTextCtrl(panel, wxID_ANY, outRegs[14], wxPoint(1050, 660), wxSize(100, -1));
	textFieldx15 = new wxTextCtrl(panel, wxID_ANY, outRegs[15], wxPoint(1050, 700), wxSize(100, -1));
	textFieldx16 = new wxTextCtrl(panel, wxID_ANY, outRegs[16], wxPoint(1050, 740), wxSize(100, -1));
	textFieldx17 = new wxTextCtrl(panel, wxID_ANY, outRegs[17], wxPoint(1300, 100), wxSize(100, -1));
	textFieldx18 = new wxTextCtrl(panel, wxID_ANY, outRegs[18], wxPoint(1300, 140), wxSize(100, -1));
	textFieldx19 = new wxTextCtrl(panel, wxID_ANY, outRegs[19], wxPoint(1300, 180), wxSize(100, -1));
	textFieldx20 = new wxTextCtrl(panel, wxID_ANY, outRegs[20], wxPoint(1300, 220), wxSize(100, -1));
	textFieldx21 = new wxTextCtrl(panel, wxID_ANY, outRegs[21], wxPoint(1300, 260), wxSize(100, -1));
	textFieldx22 = new wxTextCtrl(panel, wxID_ANY, outRegs[22], wxPoint(1300, 300), wxSize(100, -1));
	textFieldx23 = new wxTextCtrl(panel, wxID_ANY, outRegs[23], wxPoint(1300, 340), wxSize(100, -1));
	textFieldx24 = new wxTextCtrl(panel, wxID_ANY, outRegs[24], wxPoint(1300, 380), wxSize(100, -1));
	textFieldx25 = new wxTextCtrl(panel, wxID_ANY, outRegs[25], wxPoint(1300, 420), wxSize(100, -1));
	textFieldx26 = new wxTextCtrl(panel, wxID_ANY, outRegs[26], wxPoint(1300, 460), wxSize(100, -1));
	textFieldx27 = new wxTextCtrl(panel, wxID_ANY, outRegs[27], wxPoint(1300, 500), wxSize(100, -1));
	textFieldx28 = new wxTextCtrl(panel, wxID_ANY, outRegs[28], wxPoint(1300, 540), wxSize(100, -1));
	textFieldx29 = new wxTextCtrl(panel, wxID_ANY, outRegs[29], wxPoint(1300, 580), wxSize(100, -1));
	textFieldx30 = new wxTextCtrl(panel, wxID_ANY, outRegs[30], wxPoint(1300, 620), wxSize(100, -1));
	textFieldx31 = new wxTextCtrl(panel, wxID_ANY, outRegs[31], wxPoint(1300, 660), wxSize(100, -1));

	txtAddress1 = new wxStaticText(panel, wxID_ANY, "Address", wxPoint(450, 200));
	txt0 = new wxStaticText(panel, wxID_ANY, "+0", wxPoint(590, 200));
	txt1 = new wxStaticText(panel, wxID_ANY, "+1", wxPoint(700, 200));
	txt2 = new wxStaticText(panel, wxID_ANY, "+2", wxPoint(800, 200));
	txt3 = new wxStaticText(panel, wxID_ANY, "+3", wxPoint(900, 200));
	txtAddress2 = new wxStaticText(panel, wxID_ANY, "Address:", wxPoint(400, 750));

	addField = new wxTextCtrl(panel, wxID_ANY, finalOut, wxPoint(380, 220), wxSize(150,500), wxTE_MULTILINE);
	mem0 = new wxTextCtrl(panel, wxID_ANY, finalValue[0], wxPoint(540, 220), wxSize(400, 500), wxTE_MULTILINE);
	//mem1 = new wxTextCtrl(panel, wxID_ANY, finalValue[1], wxPoint(650, 220), wxSize(100, 500), wxTE_MULTILINE);
	//mem2 = new wxTextCtrl(panel, wxID_ANY, finalValue[2], wxPoint(760, 220), wxSize(100, 500), wxTE_MULTILINE);
	//mem3 = new wxTextCtrl(panel, wxID_ANY, finalValue[3], wxPoint(870, 220), wxSize(100, 500), wxTE_MULTILINE);
	addField1 = new wxTextCtrl(panel, wxID_ANY, " ", wxPoint(450, 750), wxSize(250, -1));

	button3->Hide();
	button4->Hide();
	button5->Hide();
	button6->Hide();
	//button7->Hide();
	regButton->Hide();
	memButton->Hide();
	txtAddress1->Hide();
	txtAddress2->Hide();
	addButton->Hide();
	txt0->Hide();
	txt1->Hide();
	txt2->Hide();
	txt3->Hide();
	txtx0->Hide();
	txtx1->Hide();
	txtx2->Hide();
	txtx3->Hide();
	txtx4->Hide();
	txtx5->Hide();
	txtx6->Hide();
	txtx7->Hide();
	txtx8->Hide();
	txtx9->Hide();
	txtx10->Hide();
	txtx11->Hide();
	txtx12->Hide();
	txtx13->Hide();
	txtx14->Hide();
	txtx15->Hide();
	txtx16->Hide();
	txtx17->Hide();
	txtx18->Hide();
	txtx19->Hide();
	txtx20->Hide();
	txtx21->Hide();
	txtx22->Hide();
	txtx23->Hide();
	txtx24->Hide();
	txtx25->Hide();
	txtx26->Hide();
	txtx27->Hide();
	txtx28->Hide();
	txtx29->Hide();
	txtx30->Hide();
	txtx31->Hide();
	regChoice->Hide();
	//for (int i = 0; i < 32; i++) {
	//	TextField[i]->Hide();
	//}
	textFieldx0->Hide();
	textFieldx1->Hide();
	textFieldx2->Hide();
	textFieldx3->Hide();
	textFieldx4->Hide();
	textFieldx5->Hide();
	textFieldx6->Hide();
	textFieldx7->Hide();
	textFieldx8->Hide();
	textFieldx9->Hide();
	textFieldx10->Hide();
	textFieldx11->Hide();
	textFieldx12->Hide();
	textFieldx13->Hide();
	textFieldx14->Hide();
	textFieldx15->Hide();
	textFieldx16->Hide();
	textFieldx17->Hide();
	textFieldx18->Hide();
	textFieldx19->Hide();
	textFieldx20->Hide();
	textFieldx21->Hide();
	textFieldx22->Hide();
	textFieldx23->Hide();
	textFieldx24->Hide();
	textFieldx25->Hide();
	textFieldx26->Hide();
	textFieldx27->Hide();
	textFieldx28->Hide();
	textFieldx29->Hide();
	textFieldx30->Hide();
	textFieldx31->Hide();
	addField->Hide();
	addField1->Hide();
	mem0->Hide();
	//mem1->Hide();
	//mem2->Hide();
	//mem3->Hide();

	button1->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked1, this);
	button2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked2, this);
	regButton->Bind(wxEVT_BUTTON, &MainFrame::OnRegClicked, this);
	memButton->Bind(wxEVT_BUTTON, &MainFrame::OnMemClicked, this);
	textField->Bind(wxEVT_TEXT, &MainFrame::onIpChanged, this);
	button3->Bind(wxEVT_BUTTON, &MainFrame::runFunc, this);
	CreateStatusBar();
	
}

void MainFrame::onIpChanged(wxCommandEvent& evt) {
	wxString newText = textField->GetValue();
	evt.Skip();
	ofstream fout("student.mc");
	fout << newText;
	fout.close();
}

void MainFrame::runFunc(wxCommandEvent& evt) {
	string main = "g++ main.cpp riscv.cpp functions.cpp components.cpp -o result.exe";
	//string main = "g++ k.cpp -o result.out";
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
	textFieldx0->SetValue(outRegs[0]);
	textFieldx1->SetValue(outRegs[1]);
	textFieldx2->SetValue(outRegs[2]);
	textFieldx3->SetValue(outRegs[3]);
	textFieldx4->SetValue(outRegs[4]);
	textFieldx5->SetValue(outRegs[5]);
	textFieldx6->SetValue(outRegs[6]);
	textFieldx7->SetValue(outRegs[7]);
	textFieldx8->SetValue(outRegs[8]);
	textFieldx9->SetValue(outRegs[9]);
	textFieldx10->SetValue(outRegs[10]);
	textFieldx11->SetValue(outRegs[11]);
	textFieldx12->SetValue(outRegs[12]);
	textFieldx13->SetValue(outRegs[13]);
	textFieldx14->SetValue(outRegs[14]);
	textFieldx15->SetValue(outRegs[15]);
	textFieldx16->SetValue(outRegs[16]);
	textFieldx17->SetValue(outRegs[17]);
	textFieldx18->SetValue(outRegs[18]);
	textFieldx19->SetValue(outRegs[19]);
	textFieldx20->SetValue(outRegs[20]);
	textFieldx21->SetValue(outRegs[21]);
	textFieldx22->SetValue(outRegs[22]);
	textFieldx23->SetValue(outRegs[23]);
	textFieldx24->SetValue(outRegs[24]);
	textFieldx25->SetValue(outRegs[25]);
	textFieldx26->SetValue(outRegs[26]);
	textFieldx27->SetValue(outRegs[27]);
	textFieldx28->SetValue(outRegs[28]);
	textFieldx29->SetValue(outRegs[29]);
	textFieldx30->SetValue(outRegs[30]);
	textFieldx31->SetValue(outRegs[31]);
}


void MainFrame::OnButtonClicked1(wxCommandEvent& evt) {
	textField->Show();
	button3->Hide();
	button4->Hide();
	button5->Hide();
	button6->Hide();
	//button7->Hide();
	regButton->Hide();
	memButton->Hide();
	addButton->Hide();
	txtAddress1->Hide();
	txtAddress2->Hide();
	txt0->Hide();
	txt1->Hide();
	txt2->Hide();
	txt3->Hide();
	txtx0->Hide();
	txtx1->Hide();
	txtx2->Hide();
	txtx3->Hide();
	txtx4->Hide();
	txtx5->Hide();
	txtx6->Hide();
	txtx7->Hide();
	txtx8->Hide();
	txtx9->Hide();
	txtx10->Hide();
	txtx11->Hide();
	txtx12->Hide();
	txtx13->Hide();
	txtx14->Hide();
	txtx15->Hide();
	txtx16->Hide();
	txtx17->Hide();
	txtx18->Hide();
	txtx19->Hide();
	txtx20->Hide();
	txtx21->Hide();
	txtx22->Hide();
	txtx23->Hide();
	txtx24->Hide();
	txtx25->Hide();
	txtx26->Hide();
	txtx27->Hide();
	txtx28->Hide();
	txtx29->Hide();
	txtx30->Hide();
	txtx31->Hide();
	regChoice->Hide();
	textFieldx0->Hide();
	textFieldx1->Hide();
	textFieldx2->Hide();
	textFieldx3->Hide();
	textFieldx4->Hide();
	textFieldx5->Hide();
	textFieldx6->Hide();
	textFieldx7->Hide();
	textFieldx8->Hide();
	textFieldx9->Hide();
	textFieldx10->Hide();
	textFieldx11->Hide();
	textFieldx12->Hide();
	textFieldx13->Hide();
	textFieldx14->Hide();
	textFieldx15->Hide();
	textFieldx16->Hide();
	textFieldx17->Hide();
	textFieldx18->Hide();
	textFieldx19->Hide();
	textFieldx20->Hide();
	textFieldx21->Hide();
	textFieldx22->Hide();
	textFieldx23->Hide();
	textFieldx24->Hide();
	textFieldx25->Hide();
	textFieldx26->Hide();
	textFieldx27->Hide();
	textFieldx28->Hide();
	textFieldx29->Hide();
	textFieldx30->Hide();
	textFieldx31->Hide();
	outField->Hide();
	//cslField->Hide();
	regField->Hide();
	addField->Hide();
	addField1->Hide();
	mem0->Hide();
	//mem1->Hide();
	//mem2->Hide();
	//mem3->Hide();
}

void MainFrame::OnButtonClicked2(wxCommandEvent& evt) {
	button3->Show();
	button4->Show();
	button5->Show();
	button6->Show();
	//button7->Show();
	regButton->Show();
	memButton->Show();
	addButton->Show();
	txtAddress1->Show();
	txtAddress2->Show();
	txt0->Show();
	txt1->Show();
	txt2->Show();
	txt3->Show();
	txtx0->Show();
	txtx1->Show();
	txtx2->Show();
	txtx3->Show();
	txtx4->Show();
	txtx5->Show();
	txtx6->Show();
	txtx7->Show();
	txtx8->Show();
	txtx9->Show();
	txtx10->Show();
	txtx11->Show();
	txtx12->Show();
	txtx13->Show();
	txtx14->Show();
	txtx15->Show();
	txtx16->Show();
	txtx17->Show();
	txtx18->Show();
	txtx19->Show();
	txtx20->Show();
	txtx21->Show();
	txtx22->Show();
	txtx23->Show();
	txtx24->Show();
	txtx25->Show();
	txtx26->Show();
	txtx27->Show();
	txtx28->Show();
	txtx29->Show();
	txtx30->Show();
	txtx31->Show();
	regChoice->Show();
	textFieldx0->Show();
	textFieldx1->Show();
	textFieldx2->Show();
	textFieldx3->Show();
	textFieldx4->Show();
	textFieldx5->Show();
	textFieldx6->Show();
	textFieldx7->Show();
	textFieldx8->Show();
	textFieldx9->Show();
	textFieldx10->Show();
	textFieldx11->Show();
	textFieldx12->Show();
	textFieldx13->Show();
	textFieldx14->Show();
	textFieldx15->Show();
	textFieldx16->Show();
	textFieldx17->Show();
	textFieldx18->Show();
	textFieldx19->Show();
	textFieldx20->Show();
	textFieldx21->Show();
	textFieldx22->Show();
	textFieldx23->Show();
	textFieldx24->Show();
	textFieldx25->Show();
	textFieldx26->Show();
	textFieldx27->Show();
	textFieldx28->Show();
	textFieldx29->Show();
	textFieldx30->Show();
	textFieldx31->Show();
	outField->Show();
	//cslField->Show();
	textField->Hide();
	addField->Show();
	addField1->Show();
	mem0->Show();
	//mem1->Show();
	//mem2->Show();
	//mem3->Show();
}

void MainFrame::OnRegClicked(wxCommandEvent& evt) {
	//regField->Hide();
	
	string outRegs[32];
	ifstream fin;
	fin.open("Registers.txt");
	for (int i = 0; i < 32; i++) {
		getline(fin, outRegs[i]);
	}
	fin.close();

	textFieldx0->SetValue(outRegs[0]);
	textFieldx1->SetValue(outRegs[1]);
	textFieldx2->SetValue(outRegs[2]);
	textFieldx3->SetValue(outRegs[3]);
	textFieldx4->SetValue(outRegs[4]);
	textFieldx5->SetValue(outRegs[5]);
	textFieldx6->SetValue(outRegs[6]);
	textFieldx7->SetValue(outRegs[7]);
	textFieldx8->SetValue(outRegs[8]);
	textFieldx9->SetValue(outRegs[9]);
	textFieldx10->SetValue(outRegs[10]);
	textFieldx11->SetValue(outRegs[11]);
	textFieldx12->SetValue(outRegs[12]);
	textFieldx13->SetValue(outRegs[13]);
	textFieldx14->SetValue(outRegs[14]);
	textFieldx15->SetValue(outRegs[15]);
	textFieldx16->SetValue(outRegs[16]);
	textFieldx17->SetValue(outRegs[17]);
	textFieldx18->SetValue(outRegs[18]);
	textFieldx19->SetValue(outRegs[19]);
	textFieldx20->SetValue(outRegs[20]);
	textFieldx21->SetValue(outRegs[21]);
	textFieldx22->SetValue(outRegs[22]);
	textFieldx23->SetValue(outRegs[23]);
	textFieldx24->SetValue(outRegs[24]);
	textFieldx25->SetValue(outRegs[25]);
	textFieldx26->SetValue(outRegs[26]);
	textFieldx27->SetValue(outRegs[27]);
	textFieldx28->SetValue(outRegs[28]);
	textFieldx29->SetValue(outRegs[29]);
	textFieldx30->SetValue(outRegs[30]);
	textFieldx31->SetValue(outRegs[31]);
	
	/*txtx0->Show();
	txtx1->Show();
	txtx2->Show();
	txtx3->Show();
	txtx4->Show();
	txtx5->Show();
	txtx6->Show();
	txtx7->Show();
	txtx8->Show();
	txtx9->Show();
	txtx10->Show();
	txtx11->Show();
	txtx12->Show();
	txtx13->Show();
	txtx14->Show();
	txtx15->Show();
	txtx16->Show();
	txtx17->Show();
	txtx18->Show();
	txtx19->Show();
	txtx20->Show();
	txtx21->Show();
	txtx22->Show();
	txtx23->Show();
	txtx24->Show();
	txtx25->Show();
	txtx26->Show();
	txtx27->Show();
	txtx28->Show();
	txtx29->Show();
	txtx30->Show();
	txtx31->Show();
	regChoice->Show();
	textFieldx0->Show();
	textFieldx1->Show();
	textFieldx2->Show();
	textFieldx3->Show();
	textFieldx4->Show();
	textFieldx5->Show();
	textFieldx6->Show();
	textFieldx7->Show();
	textFieldx8->Show();
	textFieldx9->Show();
	textFieldx10->Show();
	textFieldx11->Show();
	textFieldx12->Show();
	textFieldx13->Show();
	textFieldx14->Show();
	textFieldx15->Show();
	textFieldx16->Show();
	textFieldx17->Show();
	textFieldx18->Show();
	textFieldx19->Show();
	textFieldx20->Show();
	textFieldx21->Show();
	textFieldx22->Show();
	textFieldx23->Show();
	textFieldx24->Show();
	textFieldx25->Show();
	textFieldx26->Show();
	textFieldx27->Show();
	textFieldx28->Show();
	textFieldx29->Show();
	textFieldx30->Show();
	textFieldx31->Show();*/
}

void MainFrame::OnMemClicked(wxCommandEvent& evt) {
	ifstream fin;
	string finalValue;
	fin.open("Memory.txt");
	string value[25000];
	int temp, temp1;
	string stmp;
	for (int i = 0; i < 25000; i++) {
		getline(fin, value[i]);
		finalValue.append(value[i]);
		finalValue.append("\n");
	}

	//for (int i = 0; i < 13; i++) {
	//	finalValue.append(value[reqAdd + (i * 4)]);
	//	finalValue.append("\n");
	//}


	mem0->SetValue(finalValue);
	// string value[4][13];
	// int temp;
	// for (int i = 0; i < 4; i++) {
	// 	temp = reqAdd + i;
	// 	for (int j = 0; j < 13; j++) {
	// 		value[i][j] = ;
	// 		temp += 4;
	// 		finalValue[i].append(value[i][j]);
	// 		finalValue[i].append("\n");
	// 	}
	// }

	//string finalValue[4];
	////string svalue;
	//char* value[4][13];
	//int temp,temp1;
	//for (int i = 0; i < 4; i++) {
	//	temp = reqAdd + i;
	//	for (int j = 0; j < 13; j++) {
	//		temp1 = int(mem.mem[temp]);
	//		sprintf(value[i][j], "%X", temp1);
	//		temp += 4;
	//		finalValue[i].append(value[i][j]);
	//		finalValue[i].append(value[i][j]);
	//	}
	//}

	//ofstream fout("Memory0.txt");
	//for (int j = 0; j < 13; j++) {
	//	fout <<hex<< value[0][j] << '\n';
	//}
	//fout.close();
	//
	//ofstream fout1("Memory1.txt");
	//for (int j = 0; j < 13; j++) {
	//	fout1 <<hex<< value[1][j] << '\n';
	//}
	//fout1.close();
	//
	//ofstream fout2("Memory2.txt");
	//for (int j = 0; j < 13; j++) {
	//	fout2 <<hex<< value[2][j] << '\n';
	//}
	//fout2.close();
	//
	//ofstream fout3("Memory3.txt");
	//for (int j = 0; j < 13; j++) {
	//	fout3 <<hex<< value[3][j] << '\n';
	//}
	//fout3.close();
	//
	//ifstream fin;
	//fin.open("Memory0.txt");
	//string temp2;
	//for (int i = 0; i < 32; i++) {
	//	getline(fin, temp2);
	//	finalValue[0].append(temp2);
	//	finalValue[0].append("\n");
	//}
	//fin.close();
	//
	//fin.open("Memory1.txt");
	//for (int i = 0; i < 32; i++) {
	//	getline(fin, temp2);
	//	finalValue[1].append(temp2);
	//	finalValue[1].append("\n");
	//}
	//fin.close();
	//fin.open("Memory2.txt");
	//for (int i = 0; i < 32; i++) {
	//	getline(fin, temp2);
	//	finalValue[2].append(temp2);
	//	finalValue[2].append("\n");
	//}
	//fin.close();
	//fin.open("Memory3.txt");
	//for (int i = 0; i < 32; i++) {
	//	getline(fin, temp2);
	//	finalValue[3].append(temp2);
	//	finalValue[3].append("\n");
	//}
	//fin.close();
	

	//mem0->SetValue(finalValue[0]);
	//mem1->SetValue(finalValue[1]);
	//mem2->SetValue(finalValue[2]);
	//mem3->SetValue(finalValue[3]);

	/*regField->Show();
	txtx0->Hide();
	txtx1->Hide();
	txtx2->Hide();
	txtx3->Hide();
	txtx4->Hide();
	txtx5->Hide();
	txtx6->Hide();
	txtx7->Hide();
	txtx8->Hide();
	txtx9->Hide();
	txtx10->Hide();
	txtx11->Hide();
	txtx12->Hide();
	txtx13->Hide();
	txtx14->Hide();
	txtx15->Hide();
	txtx16->Hide();
	txtx17->Hide();
	txtx18->Hide();
	txtx19->Hide();
	txtx20->Hide();
	txtx21->Hide();
	txtx22->Hide();
	txtx23->Hide();
	txtx24->Hide();
	txtx25->Hide();
	txtx26->Hide();
	txtx27->Hide();
	txtx28->Hide();
	txtx29->Hide();
	txtx30->Hide();
	txtx31->Hide();
	regChoice->Hide();
	textFieldx0->Hide();
	textFieldx1->Hide();
	textFieldx2->Hide();
	textFieldx3->Hide();
	textFieldx4->Hide();
	textFieldx5->Hide();
	textFieldx6->Hide();
	textFieldx7->Hide();
	textFieldx8->Hide();
	textFieldx9->Hide();
	textFieldx10->Hide();
	textFieldx11->Hide();
	textFieldx12->Hide();
	textFieldx13->Hide();
	textFieldx14->Hide();
	textFieldx15->Hide();
	textFieldx16->Hide();
	textFieldx17->Hide();
	textFieldx18->Hide();
	textFieldx19->Hide();
	textFieldx20->Hide();
	textFieldx21->Hide();
	textFieldx22->Hide();
	textFieldx23->Hide();
	textFieldx24->Hide();
	textFieldx25->Hide();
	textFieldx26->Hide();
	textFieldx27->Hide();
	textFieldx28->Hide();
	textFieldx29->Hide();
	textFieldx30->Hide();
	textFieldx31->Hide();*/
}

