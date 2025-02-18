// $Id: StTpcdEdxCorrection.h,v 1.17 2021/05/10 16:54:45 fisyak Exp $
#ifndef STAR_StTpcdEdxCorrection
#define STAR_StTpcdEdxCorrection
//
#include "TObject.h"
#include "TF1.h"
#include "Stiostream.h"
#include "StDetectorDbMaker/St_tpcCorrectionC.h"
#include "StDetectorDbMaker/St_tpcGasC.h"
//#include "StDetectorDbMaker/St_trigDetSumsC.h"
#include "StTrackPidTraits.h"
#include "TString.h"
//class St_trigDetSums;
//class trigDetSums_st;
//________________________________________________________________________________
struct dE_t {
 public:
  Float_t dE;
  Float_t dx;
  Float_t dEdx;
  Float_t dEdxL;
  Float_t ddEdxL;
  Float_t dEdxN;
  void    Print(Option_t *option="") const {
    cout << Form("dE = %5.2f keV, dx = %f5.2f cm, dE/dx = %5.2f keV/cm, dEdxL = %f, ddEdxL = %f",1e6*dE,dx,1e6*dEdx,dEdxL,ddEdxL) << endl;
  }
};
//________________________________________________________________________________
struct dEdxCorrection_t {
  dEdxCorrection_t(const Char_t *name = "",const  Char_t *title = "", TChair *chair=0, Int_t n=0) : 
    Name(name), Title(title), Chair(chair), nrows(n), dE(0) {}
  const Char_t *Name;
  const Char_t *Title;
  TChair       *Chair;
  Int_t   nrows;
  Float_t dE;
};
//________________________________________________________________________________
class dEdxY2_t;
class StTpcdEdxCorrection : public TObject {
 public:
  enum ESector : int {kTpcOuter = 0, kTpcInner = 1, kiTpc = 2};
  enum EOptions : int {
      kUncorrected           =  0,//U   				           
      kAdcCorrection         =  1,//R  					     
      kEdge                  =  2,//E   correction near edge of chamber	     
      kAdcCorrectionMDF      =  3,//RMDF  					     
      kAdcCorrection3MDF     =  4,//RMDF 3D  					     
      kTpcdCharge            =  5,//D  					     
      kTpcrCharge            =  6,//D  					     
      kTpcCurrentCorrection  =  7,//      					     
      kTpcSecRowB            =  8,//S  					     
      kTpcSecRowC            =  9,//S  					     
      kTpcRowQ               = 10,//   	 					       	   
      kTpcAccumulatedQ       = 11,//   	 					       	   
      ktpcPressure           = 12,//P  					     
      ktpcTime               = 13,//t  					     
      kDrift                 = 14,//O  					     
      kMultiplicity          = 15,//M  					     
      kGatingGrid            = 16,//G  					     
      kzCorrectionC          = 17,//Z  					     
      kzCorrection           = 18,//Z  					     
      ktpcMethaneIn          = 19,//m  					     
      ktpcGasTemperature     = 20,//T  					     
      ktpcWaterOut           = 21,//W   				7       	   
      kSpaceCharge           = 22,//C   space charge near the wire	       	   
      kPhiDirection          = 23,//p   correction wrt local interception angle  
      kTanL                  = 24,//p   correction wrt local tan(lambda)  
      kdXCorrection          = 25,//X  					     
      kTpcEffectivedX        = 26,//X   Effective pad row height
      kTpcPadTBins           = 27,//d  					     
      kTpcZDC                = 28,//   					     
      kTpcPadMDF             = 29, 
      kAdcI                  = 30,
      knPad                  = 31, 
      knTbk                  = 32,
      kdZdY                  = 33, 
      kdXdY                  = 34,
      kTpcLast               = 35,//                                             
      kTpcNoAnodeVGainC      = 36,//   					     
      kTpcLengthCorrection   = 37,//                                             
      kTpcLengthCorrectionMDF= 38,//   					   
      kTpcLengthCorrectionMD2= 39,//   					   
      kTpcdEdxCor            = 40,//   					   
      kTpcAllCorrections     = 41 //                                             
  };
  StTpcdEdxCorrection(Int_t Option=0, Int_t debug=0);
  ~StTpcdEdxCorrection();
  Int_t dEdxCorrection(dEdxY2_t &dEdx, Bool_t doIT=kTRUE); 
  Int_t dEdxTrackCorrection(Int_t type, dst_dedx_st &dedx);
  Int_t dEdxTrackCorrection(EOptions k, Int_t type, dst_dedx_st &dedx);
  void SettpcGas               (St_tpcGas          *m = 0) {m_tpcGas = m;}
  
  void SetDebug(Int_t m=0) {m_Debug = m;}
  void SetMask (Long_t m=0) {m_Mask = m;}
  void SetSimulation(Bool_t k = kTRUE)     {m_IsSimulation = k;}
  void ReSetCorrections();
  St_tpcGas         *tpcGas()              {return m_tpcGas;}
  //  St_trigDetSums    *trigDetSums()         {return m_trigDetSums;}
  const dEdxCorrection_t &CorrectionStatus(Int_t k = 0) {return  *&m_Corrections[k];}
  St_tpcCorrectionC *Correction(Int_t k = 0) { return dynamic_cast<St_tpcCorrectionC *>(m_Corrections[k].Chair);}
  St_tpcCorrectionC *drift()               {return Correction(kDrift);}
  St_tpcCorrectionC *Multiplicity()        {return Correction(kMultiplicity);}
  St_tpcCorrectionC *AdcCorrection()       {return Correction(kAdcCorrection);}
  St_tpcCorrectionC *zCorrection()         {return Correction(kzCorrection);}
  St_tpcCorrectionC *dXCorrection()        {return Correction(kdXCorrection);}
  St_tpcCorrectionC *TpcdEdxCor()          {return Correction(kTpcdEdxCor);}
  St_tpcCorrectionC *TpcLengthCorrection() {return Correction(kTpcLengthCorrection);}
  St_tpcCorrectionC *tpcPressure()         {return Correction(ktpcPressure);}
  St_tpcCorrectionC *tpcMethaneIn()        {return Correction(ktpcMethaneIn);}
  St_tpcCorrectionC *tpcGasTemperature()   {return Correction(ktpcGasTemperature);}
  St_tpcCorrectionC *tpcWaterOut()         {return Correction(ktpcWaterOut);}
  St_tpcCorrectionC *TpcPadTBins()         {return Correction(kTpcPadTBins);}
  Int_t  Debug()                           {return m_Debug;}
  Long_t Mask()                            {return m_Mask;}
  Float_t           Adc2GeV()              {return mAdc2GeV;}
  void Print(Option_t *opt = "") const;
  Bool_t            IsFixedTarget()        {return m_isFixedTarget;}
  Bool_t            IsSimulation()         {return m_IsSimulation;}
 private:
  Long_t               m_Mask;                  //!
  St_tpcGas           *m_tpcGas;                //!
  dEdxY2_t            *mdEdx;
  Float_t              mAdc2GeV;                //! Outer/Inner conversion factors from ADC -> GeV
  dEdxCorrection_t     m_Corrections[kTpcAllCorrections];//!
  Int_t                m_Debug;                 //!
  Bool_t               m_isFixedTarget;
  Bool_t               m_IsSimulation;
  Double_t             m_TrigT0; //us
  Double_t             mTimeBinWidth; 
  Double_t             mInnerSectorzOffset;
  Double_t             mOuterSectorzOffset;
};
//________________________________________________________________________________
class dEdxY2_t {
 public:
  dEdxY2_t() {}
  virtual ~dEdxY2_t() {}
  /* U->R->S->P->O->Z->X
     U->R (TpcAdcCorrection) -> P (tpcPressure) ->
     S (TpcSecRowB/TpcSecRowC) ->  O (TpcDriftDistOxygen) ->  
     Z (TpcZCorrection) -> X(TpcdXCorrection) */
  Char_t   first[1];
  Int_t    sector;
  Int_t    row;
  Int_t    channel;
  Float_t  pad;
  Int_t    Npads; // cluster size in pads
  Int_t    Ntbks; // clustre size in time buckets
  Float_t  ZdriftDistance;     // drift distance
  Float_t  ZdriftDistanceO2;   // ZdriftDistance*ppmOxygenIn
  Float_t  ZdriftDistanceO2W;  // ZdriftDistance*ppmOxygenIn*ppmWaterOut
  Float_t  DeltaZ;             // distance to privious cluster
  Float_t  QRatio;             // Ratio to previous cluster Charge 
  Float_t  QRatioA;            // Ratio to Sum of all previous cluster Charge 
  Float_t  QSumA;              // Sum of all previous cluster Charge 
  Float_t  dxC;                // corrected dx which should be used with FitN
  Float_t  xyz[3];  // local
  Float_t  xyzD[3]; // local direction
  Float_t  edge;    // distance to sector edge
  Float_t  PhiR;    // relative phi
  Float_t  resXYZ[3]; // track SectorLocal residual wrt local track 
  Float_t  Prob; 
  Float_t  zdev; 
  Float_t  zP;      // the most probable value from Bichsel
  Float_t  zG;      // global z oh Hit
  Float_t  sigmaP;  // sigma from Bichsel
  Float_t  dCharge; // d_undershoot_Q/Q = ratio of modified - original charge normalized on original charge
  Float_t  rCharge; // d_rounding_Q/Q   = estimated rounding normalized on original charge
  Int_t    lSimulated;
  Float_t  Qcm;     // accumulated charge uC/cm
  Float_t  Crow;    // Current per row;
  Float_t  Zdc;     // ZDC rate from trigger
  Float_t  Weight;  // 1/.sigma^2 of TpcSecRow gas gain correction
  Float_t  adc;     //  adc count from cluster finder
  Float_t  TanL;
  Float_t  Voltage; // Anode Voltage
  Float_t  xpad;    // relative position in pad [-1.0,1.0]
  Float_t  yrow;    // relative position in row [-0.5,0.0] inner, and [0.0,0.5] outer
  Float_t  tpcTime;
  dE_t     C[StTpcdEdxCorrection::kTpcAllCorrections+1];
  dE_t     F;     //! 
  Float_t  AdcI;  // Log10(integrated ADC over time for a given socket0 or 0
  Float_t  dZdY;  // cluster projection on on Z
  Float_t  dXdY;  // cluster projection on Wire
  Char_t   last[1];
  void Reset() {memset(first, 0, last - first);}
}; 
#endif
