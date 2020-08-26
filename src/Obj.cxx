#ifndef Obj_cxx
#define Obj_cxx

#include "TLorentzVector.h"

//========================================================================
// Lepton Object class
//========================================================================
class LepObj
{
	public:

		LepObj(float pt, float eta, float phi, float mass, float iso)
			: m_iso(iso)
		{ m_lvec.SetPtEtaPhiM(pt, eta, phi, mass); }

		virtual ~LepObj() {};

		TLorentzVector m_lvec;
		float m_iso;
};

//========================================================================
// Jet Object class 
//========================================================================
class JetObj
{
	public:

		JetObj(float pt, float eta, float phi, float mass, unsigned flav,
		float deepCSV, float deepJet) : m_flav(flav), m_deepCSV(deepCSV),
		m_deepJet(deepJet)
		{ m_lvec.SetPtEtaPhiM(pt, eta, phi, mass); }

		virtual ~JetObj() {};

		bool IsLepton(std::vector<LepObj>& leps)
		{
			float minDr = 1000;
			for (std::vector<LepObj>::iterator it = leps.begin();
			it != leps.end(); ++it)
			{
				float dRtmp = m_lvec.DeltaR(it->m_lvec);
				if (dRtmp < minDr) minDr = dRtmp;
			}
			return minDr <= 0.4;
		};

		void SetSV(std::vector<TLorentzVector>& sv)
		{
			float maxPt = -1;
			m_svIdx = -1; m_mSV = -1;
			for (unsigned isv = 0; isv < sv.size(); ++isv)
			{
				float dRtmp = m_lvec.DeltaR(sv[isv]);
				if (dRtmp <= 0.4 && sv[isv].Pt() > maxPt) {
					m_svIdx = isv;
					m_mSV = sv[isv].M();
				}
			}
		};

		TLorentzVector m_lvec;
		unsigned m_flav;
		float m_deepCSV;
		float m_deepJet;
		unsigned m_svIdx;
		float m_mSV;
};

//========================================================================
// Z Boson Object class
//========================================================================
class ZObj
{
	public:

		ZObj(LepObj lep0, LepObj lep1) : m_lep0(lep0), m_lep1(lep1) 
		{
			m_lvec = m_lep0.m_lvec + m_lep1.m_lvec;
		};

		virtual ~ZObj() {};

		TLorentzVector m_lvec;
		LepObj m_lep0;
		LepObj m_lep1;

};

#endif
