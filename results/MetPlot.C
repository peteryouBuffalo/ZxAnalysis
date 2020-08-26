#include <string>
#include <TFile.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>

void MetPlot(std::string file)
{
	//==============================================================
	// MET Stacked Histogram
	//==============================================================
	
	// Get the file
	TFile *f = new TFile(file.c_str(), "UPDATE");

	// Get the proper data from histograms
	TH1D* h_ttbar = (TH1D*)f->Get("TTbar_jet_met");
	h_ttbar->SetFillColor(kYellow);	

	TH1D* h_zee_lJet = (TH1D*)f->Get("Zee_ljet_met");
	h_zee_lJet->SetFillColor(kGreen);
	TH1D* h_zmm_lJet = (TH1D*)f->Get("Zmm_ljet_met");
	h_zmm_lJet->SetFillColor(kGreen+1);	

	TH1D* h_zee_bJet = (TH1D*)f->Get("Zee_bjet_met");
	h_zee_bJet->SetFillColor(kRed);
	TH1D* h_zmm_bJet = (TH1D*)f->Get("Zmm_bjet_met");
	h_zmm_bJet->SetFillColor(kRed+1);

	TH1D* h_zee_cJet = (TH1D*)f->Get("Zee_cjet_met");
	h_zee_cJet->SetFillColor(kBlue);
	TH1D* h_zmm_cJet = (TH1D*)f->Get("Zmm_cjet_met");
	h_zmm_cJet->SetFillColor(kBlue+1);

	// Let's now combine these into a proper THStack
	THStack *hstack = new THStack("MET_Stack", "");

	TLegend *l = new TLegend(0.76, 0.95-0.8*7/20., 1.0, 0.95);
	l->SetFillStyle(1001);
	l->SetFillColor(kWhite);
	l->SetLineColor(kWhite);

	hstack->Add(h_ttbar); l->AddEntry(h_ttbar, "Ttbar", "f");
	hstack->Add(h_zee_lJet); l->AddEntry(h_zee_lJet, "Z+l (elec)", "f");
	hstack->Add(h_zmm_lJet); l->AddEntry(h_zmm_lJet, "Z+l (muon)", "f");
	hstack->Add(h_zee_bJet); l->AddEntry(h_zee_bJet, "Z+b (elec)", "f");
	hstack->Add(h_zmm_bJet); l->AddEntry(h_zmm_bJet, "Z+b (muon)", "f");
	hstack->Add(h_zee_cJet); l->AddEntry(h_zee_cJet, "Z+c (elec)", "f");
	hstack->Add(h_zmm_cJet); l->AddEntry(h_zmm_cJet, "Z+c (muon)", "f");

	hstack->Write(); f->Close(); delete f;	

	hstack->Draw();	
}
