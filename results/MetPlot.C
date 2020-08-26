#include <string>

void MetPlot(std::string file)
{
	//==============================================================
	// MET Stacked Histogram
	//==============================================================
	
	// Get the file
	TFile *f = new TFile(file.c_str(), "UPDATE");

	// Get the proper data from histograms
	
	std::cout << "Getting the data from the ROOT file..." << std::endl;
	TH1D* h_ttbar = (TH1D*)f->Get("TTbar_jet_met");
	h_ttbar->SetFillColor(kYellow);	
	double L_ttbar = h_ttbar->GetEntries()/8.794e-08;

	TH1D* h_zee_lJet = (TH1D*)f->Get("Zee_ljet_met");
	h_zee_lJet->SetFillColor(kGreen);
	h_zee_lJet->Scale(L_ttbar / (h_zee_lJet->GetEntries()/(1.271e-06*0.427)));

	TH1D* h_zmm_lJet = (TH1D*)f->Get("Zmm_ljet_met");
	h_zmm_lJet->SetFillColor(kGreen+1);	
	h_zmm_lJet->Scale(L_ttbar / (h_zmm_lJet->GetEntries()/(1.271e-06*0.427)));

	TH1D* h_zee_bJet = (TH1D*)f->Get("Zee_bjet_met");
	h_zee_bJet->SetFillColor(kRed);
	h_zee_bJet->Scale(L_ttbar / (h_zee_bJet->GetEntries()/(1.271e-06*0.152)));

	TH1D* h_zmm_bJet = (TH1D*)f->Get("Zmm_bjet_met");
	h_zmm_bJet->SetFillColor(kRed+1);
	h_zmm_bJet->Scale(L_ttbar / (h_zmm_bJet->GetEntries()/(1.271e-06*0.152)));

	TH1D* h_zee_cJet = (TH1D*)f->Get("Zee_cjet_met");
	h_zee_cJet->SetFillColor(kBlue);
	h_zee_cJet->Scale(L_ttbar / (h_zee_cJet->GetEntries()/(1.271e-06*0.119)));

	TH1D* h_zmm_cJet = (TH1D*)f->Get("Zmm_cjet_met");
	h_zmm_cJet->SetFillColor(kBlue+1);
	h_zmm_cJet->Scale(L_ttbar / (h_zmm_cJet->GetEntries()/(1.271e-06*0.119)));

	// Let's now combine these into a proper THStack
	std::cout << "Stacking the histograms..." << std::endl;
	THStack hstack("MET_Stack", "");

	TLegend *l = new TLegend(0.76, 0.95-0.08*7/20., 1.0, 0.95);
	l->SetFillStyle(1001);
	l->SetFillColor(kWhite);
	l->SetLineColor(kWhite);

	std::cout << "Adding background (ttbar)..." << std::endl;
	hstack.Add(h_ttbar); l->AddEntry(h_ttbar, "Ttbar", "f");
	std::cout << "Adding Z+l-jets..." << std::endl;
	hstack.Add(h_zee_lJet); l->AddEntry(h_zee_lJet, "Z+l (elec)", "f");
	hstack.Add(h_zmm_lJet); l->AddEntry(h_zmm_lJet, "Z+l (muon)", "f");
	std::cout << "Adding Z+b-jets..." << std::endl;
	hstack.Add(h_zee_bJet); l->AddEntry(h_zee_bJet, "Z+b (elec)", "f");
	hstack.Add(h_zmm_bJet); l->AddEntry(h_zmm_bJet, "Z+b (muon)", "f");
	std::cout << "Adding Z+c-jets..." << std::endl;
	hstack.Add(h_zee_cJet); l->AddEntry(h_zee_cJet, "Z+c (elec)", "f");
	hstack.Add(h_zmm_cJet); l->AddEntry(h_zmm_cJet, "Z+c (muon)", "f");

	// Now, let's put this all into a final file
	std::cout << "Outputting to plots.root..." << std::endl;
	TFile *f2 = new TFile("plots.root", "RECREATE");
	TCanvas* c = new TCanvas("MET_Stack", "", 800, 600);
	c->SetLogy(true);
	hstack.Draw("hist"); l->Draw("same");

	c->Write(); f2->Close(); delete f2;
	f->Close(); delete f;
	std::cout << "[END PROGRAM]" << std::endl;
}

