#define TTbarSelector_cxx

#include <math.h>
#include "TList.h"
#include "TParameter.h"
#include "TLorentzVector.h"

#include "TTbarSelector.h"
#include "Global.h"
#include "Obj.cxx"

#include <iostream>
#include "TH1.h"

#include <TH2.h>
#include <TStyle.h>

using namespace std;

//=============================================================================
// Begin function
//=============================================================================
void TTbarSelector::Begin(TTree * /*tree*/)
{ TString option = GetOption(); }


//=============================================================================
// SlaveBegin function
//=============================================================================
void TTbarSelector::SlaveBegin(TTree * /*tree*/)
{
   	TString option = GetOption();

	// Initialize our histograms
	h_ttbar_jet = new ZbPlots("TTbar_jet");

	std::vector<TH1*> tmp = h_ttbar_jet->returnHisto();	
	for (size_t i = 0; i < tmp.size(); ++i) GetOutputList()->Add(tmp[i]);
}

//=============================================================================
// Process function
//=============================================================================
Bool_t TTbarSelector::Process(Long64_t entry)
{
   	fReader.SetLocalEntry(entry);
	GetEntry(entry);

	++TotalEvent;
	if (TotalEvent % 10000 == 0)
		std::cout << "Entry #" << TotalEvent << std::endl;

	float genW = 1.;
	if (*genWeight < 0) genW = -1;
	float evtW = 1.;
	evtW *= genW;

	//===== Get the Electrons =====
	std::vector<LepObj> elecs;
	std::vector<LepObj> elecs_jetOverlap;
	for (unsigned int i = 0; i < *nElectron; ++i)
	{
		LepObj elec(Electron_pt[i], Electron_eta[i], Electron_phi[i],
			Electron_mass[i], 0);

		// check for jet overlap removal
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
		int elecID = Electron_cutBased[i];
		if (elecID < 2) continue;

		elecs.push_back(elec);
	}

	//====== Get the Muons ======
	std::vector<LepObj> muons;
	std::vector<LepObj> muons_jetOverlap;

	for (unsigned int i = 0; i < *nMuon; ++i)
	{
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

		// skip over the muon if our cuts aren't met
		if (muon.m_lvec.Pt() < CUTS.Get<float>("lep_pt1") || 
		fabs(muon.m_lvec.Eta()) > CUTS.Get<float>("lep_eta")) continue ; 

		if (Muon_looseId[i] <= 0) continue;
		if (muon.m_iso > CUTS.Get<float>("muon_iso")) continue;
		muons.push_back(muon);
	}

	//====== Get the Secondary Vertex ======
	std::vector<TLorentzVector> lvec_SVs;
	for (unsigned i = 0; i < *nSV; ++i)
	{
		TLorentzVector tmp;
		tmp.SetPtEtaPhiM(SV_pt[i], SV_eta[i], SV_phi[i], SV_mass[i]);
		lvec_SVs.push_back(tmp);	
	}

	//===== Get the Jets ======
	std::vector<JetObj> jets;
	
	for (unsigned int i = 0; i < *nJet; ++i)
	{
		int jetFlav = Jet_hadronFlavour[i];
		JetObj jet(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i], jetFlav,
			Jet_btagDeepB[i], Jet_btagDeepFlavB[i]);
		jet.SetSV(lvec_SVs);

		// If our jet doesn't meet the cuts, skip it
		if (jet.m_lvec.Pt() < CUTS.Get<float>("jet_pt") ||
		fabs(jet.m_lvec.Eta()) > CUTS.Get<float>("jet_eta")) continue;
	
		// If our jet is a lepton, skip it
		if (jet.IsLepton(elecs_jetOverlap)) continue;
		if (jet.IsLepton(muons_jetOverlap)) continue;

		jets.push_back(jet);
	}

	//====== Do the analysis ======
	if (jets.size() >= 1)
	{
		h_ttbar_jet->FillMet(*MET_pt, *PuppiMET_pt, 1.);
	}

  	return kTRUE;
}

void TTbarSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void TTbarSelector::Terminate()
{
   // Get the file for output
   TFile *f = new TFile("results/out.root", "RECREATE");

   // Take the elements we have from processing and print them out.
   for (auto obj : *GetOutputList())
   	obj->Write();

   // close the file
   f->Close(); delete f;
}
