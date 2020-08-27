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

	// Let's create the ROC curves.
	Double_t cuts[22];
	Double_t lEff[22], bEff[22], cEff[22], tEff[22];
	for (int i = 0; i < 22; ++i)
	{
		// Calculate the i'th cut
		cuts[i] = i*5;

		// Determine efficiency for ttbar at the i'th cut
		Double_t tot = 0., sel = 0.;
		for (int b = 0; b < h_ttbar->GetSize(); ++b)
		{
			if (h_ttbar->GetBinCenter(b) < cuts[i])
				sel += h_ttbar->GetBinContent(b);
			tot += h_ttbar->GetBinContent(b);		
		}
		tEff[i] = sel/tot;

		// Determine efficiency for Z+l at the i'th cut
		tot = 0.; sel = 0.;
		for (int b = 0; b < h_zee_lJet->GetSize(); ++b)
		{
			if (h_zee_lJet->GetBinCenter(b) < cuts[i])
				sel += h_zee_lJet->GetBinContent(b);
			tot += h_zee_lJet->GetBinContent(b);
		}
		for (int b = 0; b < h_zmm_lJet->GetSize(); ++b)
		{
			if (h_zmm_lJet->GetBinCenter(b) < cuts[i])
				sel += h_zmm_lJet->GetBinContent(b);
			tot += h_zmm_lJet->GetBinContent(b);
		}
		lEff[i] = sel/tot;

		// Determine efficiency for Z+b at the i'th cut
		tot = 0.; sel = 0.;
		for (int b = 0; b < h_zee_bJet->GetSize(); ++b)
		{
			if (h_zee_bJet->GetBinCenter(b) < cuts[i])
				sel += h_zee_bJet->GetBinContent(b);
			tot += h_zee_bJet->GetBinContent(b);
		}
		for (int b = 0; b < h_zmm_bJet->GetSize(); ++b)
		{
			if (h_zmm_bJet->GetBinCenter(b) < cuts[i])
				sel += h_zmm_bJet->GetBinContent(b);
			tot += h_zmm_bJet->GetBinContent(b);
		}
		bEff[i] = sel/tot;

		// Determine efficiency for Z+c at the i'th cut
		tot = 0.; sel = 0.;
		for (int b = 0; b < h_zee_cJet->GetSize(); ++b)
		{
			if (h_zee_cJet->GetBinCenter(b) < cuts[i])
				sel += h_zee_cJet->GetBinContent(b);
			tot += h_zee_cJet->GetBinContent(b);
		}
		for (int b = 0; b < h_zmm_cJet->GetSize(); ++b)
		{
			if (h_zmm_cJet->GetBinCenter(b) < cuts[i])
				sel += h_zmm_cJet->GetBinContent(b);
			tot += h_zmm_cJet->GetBinContent(b);
		}
		cEff[i] = sel/tot;
	}

	// Now let's put our efficiencies into a plot.
	TGraph* gr_lJet = new TGraph(22, tEff, lEff);
	gr_lJet->SetLineColor(kGreen);

	TGraph* gr_bJet = new TGraph(22, tEff, bEff);
	gr_bJet->SetLineColor(kRed);

	TGraph* gr_cJet = new TGraph(22, tEff, cEff);
	gr_cJet->SetLineColor(kBlue);

	for (int j = 0; j < 22; ++j)
	{
		Double_t y = gr_lJet->GetY()[j] + 0.01;
		TLatex *lt = new TLatex(gr_lJet->GetX()[j], y, Form("%.1f", cuts[j]));
		lt->SetTextSize(0.02);
		gr_lJet->GetListOfFunctions()->Add(lt);
	}

	TMultiGraph *mg = new TMultiGraph("ROC_Curve", "");
	mg->GetXaxis()->SetTitle("ttbar Selection Eff.");
	mg->GetYaxis()->SetTitle("Z+j Selection Eff.");

	TLegend *leg = new TLegend(0.68, 0.72, 0.98, 0.92);
	mg->Add(gr_lJet, "c*"); leg->AddEntry(gr_lJet, "Z+l", "f");
	mg->Add(gr_bJet, "c*"); leg->AddEntry(gr_bJet, "Z+b", "f");
	mg->Add(gr_cJet, "c*"); leg->AddEntry(gr_cJet, "Z+c", "f");
	
	TCanvas *cs = new TCanvas("ROC_curve", "", 10, 10, 700, 900);
	mg->Draw("ac*"); leg->Draw(); cs->SetGrid();
	gPad->Update(); gPad->Modified();

	// Let's now combine these into a proper THStack
	std::cout << "Stacking the histograms..." << std::endl;
	THStack hstack("MET_Stack", "");

	TLegend *l = new TLegend(0.76, 0.95-0.8*7/20., 1.0, 0.95);
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

	c->Write(); cs->Write(); f2->Close(); delete f2;
	f->Close(); delete f;
	std::cout << "[END PROGRAM]" << std::endl;
}

