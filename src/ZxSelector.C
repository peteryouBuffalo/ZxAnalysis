#define ZxSelector_cxx

#include <math.h>
#include "TList.h"
#include "TParameter.h"
#include "TLorentzVector.h"

#include "ZxSelector.h"
#include "Global.h"
#include "Obj.cxx"

#include <iostream>
#include "TH1.h"

using namespace std;
float PI = M_PI;

//=============================================================================
// Begin function
//=============================================================================
void ZxSelector::Begin(TTree * /*tree*/)
{ TString option = GetOption(); }

//=============================================================================
// SlaveBegin function
//=============================================================================
void ZxSelector::SlaveBegin(TTree * /*tree*/)
{
	TString option = GetOption();
	
	// Initialize our histograms
	h_evt = new TH1D("Nevt", "", 3, -1.5, 1.5);
	h_zee_jet = new ZbPlots("Zee_jet");
	h_zee_bjet = new ZbPlots("Zee_bjet");
	h_zee_cjet = new ZbPlots("Zee_cjet");
	h_zee_ljet = new ZbPlots("Zee_ljet");

	h_zmm_jet = new ZbPlots("Zmm_jet");
	h_zmm_bjet = new ZbPlots("Zmm_bjet");
	h_zmm_cjet = new ZbPlots("Zmm_cjet");
	h_zmm_ljet = new ZbPlots("Zmm_ljet");

	unsigned nBins = 9;
	float bins[10] = {20, 30, 50, 70, 100, 140, 200, 300, 600, 1000};
	h_eff_b = new EffPlots("b", nBins, bins);
	h_eff_c = new EffPlots("c", nBins, bins);
	h_eff_l = new EffPlots("l", nBins, bins);

	h_dR_je = new TH1D("h_dR_je", "", 500, 0, 5);
	h_dR_jm = new TH1D("h_dR_jm", "", 500, 0, 5);

	h_Zee_ZmassFull = new TH1D("Zee_ZmassFull", "", 300, 0, 300);
	h_Zmm_ZmassFull = new TH1D("Zmm_ZmassFull", "", 300, 0, 300);

	h_nMuon = new TH1D("h_nMuon", "", 10, -0.5, 9.5);
	h_nElec = new TH1D("h_nElec", "", 10, -0.5, 9.5);
	h_nJet = new TH1D("h_nJet", "", 10, -0.5, 9.5);

	h_elec_cutflow = new TH1D("elec_cutflow", "", 10, -0.5, 9.5);
	h_muon_cutflow = new TH1D("muon_cutflow", "", 10, -0.5, 9.5);
	h_Jet_cutflow = new TH1D("Jet_cutflow", "", 10, -0.5, 9.5);
	h_zee_cutflow = new TH1D("zee_cutflow", "", 10, -0.5, 9.5);
	h_zmm_cutflow = new TH1D("zmm_cutflow", "", 10, -0.5, 9.5);

	// Add histograms to fOutput so they can be saved in Processor::SlaveTerminate
	GetOutputList()->Add(h_evt);

	std::vector<TH1*> tmp = h_zee_jet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zmm_jet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zee_ljet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zmm_ljet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zee_bjet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zmm_bjet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zee_cjet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_zmm_cjet->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	tmp = h_eff_b->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);
	tmp = h_eff_c->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);
	tmp = h_eff_l->returnHisto();
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);

	GetOutputList()->Add(h_dR_je); GetOutputList()->Add(h_dR_jm);
	GetOutputList()->Add(h_elec_cutflow); GetOutputList()->Add(h_muon_cutflow);
	GetOutputList()->Add(h_Jet_cutflow);
	GetOutputList()->Add(h_zee_cutflow); GetOutputList()->Add(h_zmm_cutflow);

	const Int_t nx = 3, nx1 = 4, yx = 5, nzee = 7, nzmm = 7;
	const char *elec_cut[nx] = {"all", "kine", "ID"};
	const char *muon_cut[nx1] = {"all", "kine", "loose muon ID", "iso"};
	const char *jet_cut[yx] = {"all", "kine", "elec overlap removal",
							  "muon overlap removal", "loose jet ID"};
	const char *zee_cut[nzee] = { "all event (not cut", "trigger", "pass electron cuts",
								"pass Z mass cuts`", "pass jet cuts", "pass b-jet",
								"pass 2 b-jets"};
	const char *zmm_cut[nzmm] = {"all event (not cut)","trigger","pass muon  cuts",
	 							 "pass Z mass cuts", "pass jet cuts", "pass b-jet", 
								 "pass 2 b-jets"};

	for (int i=1;i<=nx;i++) h_elec_cutflow->GetXaxis()->SetBinLabel(i+1.5,elec_cut[i-1]);
	for (int i=1;i<=nx1;i++) h_muon_cutflow->GetXaxis()->SetBinLabel(i+1.5,muon_cut[i-1]);
	for (int i=1;i<=yx;i++) h_Jet_cutflow->GetXaxis()->SetBinLabel(i+1.5,jet_cut[i-1]);
	for (int i=1;i<=nzee;i++) h_zee_cutflow->GetXaxis()->SetBinLabel(i+1.5,zee_cut[i-1]);
	for (int i=1;i<=nzmm;i++) h_zmm_cutflow->GetXaxis()->SetBinLabel(i+1.5,zmm_cut[i-1]);
}


//=============================================================================
// Process function
//=============================================================================
Bool_t ZxSelector::Process(Long64_t entry)
{
   
   fReader.SetLocalEntry(entry);
   GetEntry(entry);

	float genW = 1.;
	if (*genWeight < 0) genW = -1;
	if (*genWeight == 0) h_evt->Fill(0);
	if (*genWeight < 0) h_evt->Fill(-1);
	if (*genWeight > 0) h_evt->Fill(1);

	float evtW = 1.;
	evtW *= genW;

	//===== Get the Electrons =====
	std::vector<LepObj> elecs;
	std::vector<LepObj> elecs_jetOverlap;

	for (unsigned int i = 0; i < *nElectron; ++i)
	{
		h_elec_cutflow->Fill(1);
		LepObj elec(Electron_pt[i], Electron_eta[i], Electron_phi[i], Electron_mass[i],0);
		
		// Check for jet overlap removal
		if (elec.m_lvec.Pt() > CUTS.Get<float>("lep_jetOverlap_pt")  &&
		fabs(elec.m_lvec.Eta()) < CUTS.Get<float>("lep_jetOverlap_eta"))
		{
			int elecID = Electron_cutBased[i];
			if (elecID >= 2) elecs_jetOverlap.push_back(elec);
		}

		// Skip over the electron if our cuts aren't met
		if (elec.m_lvec.Pt() < CUTS.Get<float>("lep_pt1") ||
		fabs(elec.m_lvec.Eta()) > CUTS.Get<float>("lep_eta")) continue;
		float etaSC = elec.m_lvec.Eta() - Electron_deltaEtaSC[i];
		if (fabs(etaSC) < 1.566 && fabs(etaSC) > 1.442) continue;

		// If loose electron ID, skip over it
		h_elec_cutflow->Fill(2);
		int elecID = Electron_cutBased[i];
		if (elecID < 2) continue;

		// Add the electron to our list.
		h_elec_cutflow->Fill(3);
		elecs.push_back(elec);
	}

	//====== Get the Muons ======
	std::vector<LepObj> muons;
	std::vector<LepObj> muons_jetOverlap;

	for (unsigned int i = 0; i < *nMuon; ++i)
	{
		h_muon_cutflow->Fill(1);
		LepObj muon(Muon_pt[i], Muon_eta[i], Muon_phi[i], Muon_mass[i],
				Muon_pfRelIso04_all[i]);

		// check for jet overlap removal
		if (muon.m_lvec.Pt() > CUTS.Get<float>("lep_jetOverlap_pt") &&
		fabs(muon.m_lvec.Eta()) < CUTS.Get<float>("lep_jetOverlap_eta")) 
		{
			if (Muon_looseId[i] > 0 && Muon_pfRelIso04_all[i] <
				CUTS.Get<float>("muon_iso"))
			{ muons_jetOverlap.push_back(muon); }
		}

		// Skip over the muon if our cuts aren't met
		if (muon.m_lvec.Pt() < CUTS.Get<float>("lep_pt1") || 
		fabs(muon.m_lvec.Eta()) > CUTS.Get<float>("lep_eta")) continue ; 
		h_muon_cutflow->Fill(2);

		if (Muon_looseId[i] <= 0) continue;
		h_muon_cutflow->Fill(3);

		if (muon.m_iso > CUTS.Get<float>("muon_iso")) continue;
		h_muon_cutflow->Fill(4);
		muons.push_back(muon);
	}

	//===== Get the Secondary Vertex =====
	std::vector<TLorentzVector> lvec_SVs;
	for (unsigned i = 0; i < *nSV; ++i)
	{
		TLorentzVector tmp;
		tmp.SetPtEtaPhiM(SV_pt[i], SV_eta[i], SV_phi[i], SV_mass[i]);
		lvec_SVs.push_back(tmp);
	}

	//====== Get the Jets ======
	std::vector<JetObj> jets;
	std::vector<JetObj> jets_elec_removal;
	std::vector<JetObj> bjets;
	std::vector<JetObj> ljets;
	std::vector<JetObj> cjets;

	float btagM_cut = CUTS.Get<float>("jet_deepCSVM_2016");

	for (unsigned int i = 0; i < *nJet; ++i)
	{
		h_Jet_cutflow->Fill(1);
		int jetFlav = -1;
		jetFlav = Jet_hadronFlavour[i];

		JetObj jet(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i], jetFlav,
			Jet_btagDeepB[i], Jet_btagDeepFlavB[i]);
		jet.SetSV(lvec_SVs);

		// If our jet doesn't meet the cuts, skip it
		if (jet.m_lvec.Pt() < CUTS.Get<float>("jet_pt") ||
		fabs(jet.m_lvec.Eta()) > CUTS.Get<float>("jet_eta")) continue;
		h_Jet_cutflow->Fill(2);

		// If our jet is a lepton, skip it
		if (jet.IsLepton(elecs_jetOverlap)) continue;
		jets_elec_removal.push_back(jet);
		h_Jet_cutflow->Fill(3);

		if (jet.IsLepton(muons_jetOverlap)) continue;
		h_Jet_cutflow->Fill(4);

		if (Jet_jetId[i] <= 0) continue;
		h_Jet_cutflow->Fill(5);

		// Move the jets into the proper lists
		jets.push_back(jet);
		if (jetFlav == 5) bjets.push_back(jet);
		if (jetFlav == 4) cjets.push_back(jet);
		if (jetFlav <= 3) ljets.push_back(jet);
	}

	//====== Calculate SF for electron, muon, & b-tagging ======
	float elecSF_w(1.);
	float muonSF_w(1.);
	float btag_w(1.);

	//btag_w = CalBtagWeight(jets);
	//if (elecs.size() >= 2) elecSF_w = CalElecSF(elecs[0], elecs[1]);
	//if (muons.size() >= 2) muonSF_w = CalMuonSF_id_iso(muons[0], muons[1]);

	//===== Check the triggers =====
	bool elecTrig(false);
	bool muonTrig(false);

	if (*HLT_Ele27_WPTight_Gsf) elecTrig = true;
	if (*HLT_IsoMu24 || *HLT_IsoTkMu24) muonTrig = true;

	//===== Check the Zee + jets events =====
	h_zee_cutflow->Fill(1);		// all events (not cut)
	if (elecTrig)
	{
		h_zee_cutflow->Fill(2);	// trigger
		if (elecs.size() >= 2 && elecs[0].m_lvec.Pt() >= CUTS.Get<float>("lep_pt0"))
		{
			h_zee_cutflow->Fill(3);	//pass electron cut
			float zee_w = evtW*elecSF_w;
			float zeeb_w = zee_w*btag_w;
			ZObj Z(elecs[0], elecs[1]);

			h_Zee_ZmassFull->Fill(Z.m_lvec.M(), zee_w);
			
			if (Z.m_lvec.M() >= CUTS.Get<float>("ZMassL") &&
			Z.m_lvec.M() <= CUTS.Get<float>("ZMassH"))
			{
				h_zee_cutflow->Fill(4);
				h_zee_jet->FillNjet(jets.size(), zee_w);
				
				// if we have at least one jet
				if (jets.size() >= 1) {

					h_zee_cutflow->Fill(5);	// pass jet cuts
					h_zee_jet->Fill(Z, jets[0], zee_w);
					h_zee_jet->FillMet(*MET_pt, *PuppiMET_pt, zee_w);

					//isolation
					float deltaRelelep0 = Z.m_lep0.m_lvec.DeltaR(jets[0].m_lvec);
					float deltaRelelep1 = Z.m_lep1.m_lvec.DeltaR(jets[0].m_lvec);
					h_dR_je->Fill(std::min(deltaRelelep0, deltaRelelep1),zee_w);

					//tagging efficiency
					//Fill_btagEffi(jets,zee_w);
				}
				
				// if we have at least one l-tagged jet
				if (ljets.size() >= 1) {
				
					h_zee_ljet->Fill(Z, ljets[0], zeeb_w);
					h_zee_ljet->FillMet(*MET_pt, *PuppiMET_pt, zeeb_w);

				}	
				
				// if we have at least one b-tagged jet
				h_zee_bjet->FillNjet(bjets.size(), zeeb_w);
				if (bjets.size() >= 1) {

					h_zee_cutflow->Fill(6);
					h_zee_bjet->Fill(Z, bjets[0], zeeb_w);
					h_zee_bjet->FillMet(*MET_pt, *PuppiMET_pt, zeeb_w);

				}

				// if we have at least one c-tagged jet
				h_zee_cjet->FillNjet(cjets.size(), zeeb_w);
				if (cjets.size() >= 1) {

					h_zee_cjet->Fill(Z, cjets[0], zeeb_w);
					h_zee_cjet->FillMet(*MET_pt, *PuppiMET_pt, zeeb_w);

				}

			}//end-if-cut						
		}//end-if
	}//end-elec-trigger

	//===== Check the Zmm + jets events =====
	h_zmm_cutflow->Fill(1);	// all events (not cut)

	if (muonTrig)
	{
		h_zmm_cutflow->Fill(2);
		if (muons.size() >= 2 && muons[0].m_lvec.Pt() >= CUTS.Get<float>("lep_pt0"))
		{
			h_zmm_cutflow->Fill(3); // pass muon cuts

			float zmm_w = evtW*muonSF_w;
			float zmmb_w = zmm_w*btag_w;

			ZObj Z(muons[0], muons[1]);

			h_Zmm_ZmassFull->Fill(Z.m_lvec.M(), zmm_w);
			if (Z.m_lvec.M() >= CUTS.Get<float>("ZMassL") &&
				Z.m_lvec.M() <= CUTS.Get<float>("ZMassH"))
			{
				h_zmm_cutflow->Fill(4);	//pass Z mass cuts

				h_zmm_jet->FillNjet(jets.size(), zmm_w);
				
				// if we have at least one jet
				if (jets.size() >= 1)
				{
					h_zmm_cutflow->Fill(5);	// pass jet cuts

					float deltaRmuonlep0 = Z.m_lep0.m_lvec.DeltaR(jets[0].m_lvec);
					float deltaRmuonlep1 = Z.m_lep1.m_lvec.DeltaR(jets[0].m_lvec);

					h_zmm_jet->Fill(Z, jets[0], zmm_w);
					h_zmm_jet->FillMet(*MET_pt, *PuppiMET_pt, zmm_w);

					h_dR_jm->Fill(std::min(deltaRmuonlep0, deltaRmuonlep1), zmm_w);
					//Fill_btagEffi(jets,zmm_w);
				}//end-one-jet

				// if we have at least one l-jet
				if (ljets.size() >= 1)
				{
					h_zmm_ljet->Fill(Z, bjets[0], zmmb_w);
					h_zmm_ljet->FillMet(*MET_pt, *PuppiMET_pt, zmmb_w);
				}

				// if we have at least one b-jet
				if (bjets.size() >= 1)
				{
					h_zmm_cutflow->Fill(6);
					h_zmm_bjet->Fill(Z, bjets[0], zmmb_w);
					h_zmm_bjet->FillMet(*MET_pt, *PuppiMET_pt, zmmb_w);
				}

				// if we have at least one c-jet
				if (cjets.size() >= 1)
				{
					h_zmm_cjet->Fill(Z, cjets[0], zmmb_w);
					h_zmm_cjet->FillMet(*MET_pt, *PuppiMET_pt, zmmb_w);
				}

			}//end-z-cut
		}//end-muon-cut
	}//end-muon-trigger

	return kTRUE;
}

void ZxSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ZxSelector::Terminate()
{
	// Get the file for output
	TFile *f = new TFile("results/out.root", "RECREATE");

   	// Take the elements we have from processing and print them out.
	for (auto obj : *GetOutputList())
		obj->Write();

	// close the file
	f->Close(); delete f;
}
