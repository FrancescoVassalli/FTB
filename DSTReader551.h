//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun 26 12:53:58 2018 by ROOT version 6.10/02
// from TTree T/Prototype4DSTReader
// found on file: beam_00000551-0000_DSTReader.root
//////////////////////////////////////////////////////////

#ifndef DSTReader551_h
#define DSTReader551_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TObject.h"

class DSTReader551 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxTOWER_RAW_LG_HCALIN = 16;
   static constexpr Int_t kMaxTOWER_RAW_LG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_RAW_HG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_CALIB_LG_HCALIN = 16;
   static constexpr Int_t kMaxTOWER_CALIB_LG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_CALIB_HG_HCALOUT = 16;
   static constexpr Int_t kMaxTOWER_RAW_CEMC = 64;
   static constexpr Int_t kMaxTOWER_CALIB_CEMC = 64;
   static constexpr Int_t kMaxTOWER_RAW_HODO_VERTICAL = 8;
   static constexpr Int_t kMaxTOWER_RAW_HODO_HORIZONTAL = 8;
   static constexpr Int_t kMaxTOWER_CALIB_HODO_VERTICAL = 8;
   static constexpr Int_t kMaxTOWER_CALIB_HODO_HORIZONTAL = 8;
   static constexpr Int_t kMaxTOWER_RAW_C1 = 1;
   static constexpr Int_t kMaxTOWER_CALIB_C1 = 1;
   static constexpr Int_t kMaxTOWER_RAW_C2 = 3;
   static constexpr Int_t kMaxTOWER_CALIB_C2 = 3;
   static constexpr Int_t kMaxTOWER_RAW_PbGL = 1;
   static constexpr Int_t kMaxTOWER_CALIB_PbGL = 1;
   static constexpr Int_t kMaxTOWER_RAW_TRIGGER_VETO = 4;
   static constexpr Int_t kMaxTOWER_CALIB_TRIGGER_VETO = 4;
   static constexpr Int_t kMaxTOWER_RAW_TILE_MAPPER = 16;
   static constexpr Int_t kMaxTOWER_CALIB_TILE_MAPPER = 16;
   static constexpr Int_t kMaxTOWER_RAW_SC3 = 1;
   static constexpr Int_t kMaxTOWER_CALIB_SC3 = 1;
   static constexpr Int_t kMaxTOWER_RAW_FINGER_COUNTER = 1;
   static constexpr Int_t kMaxTOWER_CALIB_FINGER_COUNTER = 1;

   // Declaration of leaf types
   Double_t        beam_MTNRG_GeV;
   Double_t        beam_2CH_mm;
   Double_t        beam_2CV_mm;
   Double_t        EMCAL_A0_HighGain;
   Double_t        EMCAL_GR0_BiasOffset_Tower21;
   Double_t        EMCAL_T0_Tower21;
   Double_t        EMCAL_Is_HighEta;
   Int_t           n_TOWER_RAW_LG_HCALIN;
   Int_t           TOWER_RAW_LG_HCALIN_;
   UInt_t          TOWER_RAW_LG_HCALIN_fUniqueID[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   UInt_t          TOWER_RAW_LG_HCALIN_fBits[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   UInt_t          TOWER_RAW_LG_HCALIN_towerid[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Double_t        TOWER_RAW_LG_HCALIN_energy[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Float_t         TOWER_RAW_LG_HCALIN_time[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Float_t         TOWER_RAW_LG_HCALIN_signal_samples[kMaxTOWER_RAW_LG_HCALIN][31];   //[TOWER_RAW_LG_HCALIN_]
   Int_t           TOWER_RAW_LG_HCALIN_HBD_channel[kMaxTOWER_RAW_LG_HCALIN];   //[TOWER_RAW_LG_HCALIN_]
   Int_t           n_TOWER_RAW_LG_HCALOUT;
   Int_t           TOWER_RAW_LG_HCALOUT_;
   UInt_t          TOWER_RAW_LG_HCALOUT_fUniqueID[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   UInt_t          TOWER_RAW_LG_HCALOUT_fBits[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   UInt_t          TOWER_RAW_LG_HCALOUT_towerid[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Double_t        TOWER_RAW_LG_HCALOUT_energy[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Float_t         TOWER_RAW_LG_HCALOUT_time[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Float_t         TOWER_RAW_LG_HCALOUT_signal_samples[kMaxTOWER_RAW_LG_HCALOUT][31];   //[TOWER_RAW_LG_HCALOUT_]
   Int_t           TOWER_RAW_LG_HCALOUT_HBD_channel[kMaxTOWER_RAW_LG_HCALOUT];   //[TOWER_RAW_LG_HCALOUT_]
   Int_t           n_TOWER_RAW_HG_HCALOUT;
   Int_t           TOWER_RAW_HG_HCALOUT_;
   UInt_t          TOWER_RAW_HG_HCALOUT_fUniqueID[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   UInt_t          TOWER_RAW_HG_HCALOUT_fBits[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   UInt_t          TOWER_RAW_HG_HCALOUT_towerid[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Double_t        TOWER_RAW_HG_HCALOUT_energy[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Float_t         TOWER_RAW_HG_HCALOUT_time[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Float_t         TOWER_RAW_HG_HCALOUT_signal_samples[kMaxTOWER_RAW_HG_HCALOUT][31];   //[TOWER_RAW_HG_HCALOUT_]
   Int_t           TOWER_RAW_HG_HCALOUT_HBD_channel[kMaxTOWER_RAW_HG_HCALOUT];   //[TOWER_RAW_HG_HCALOUT_]
   Int_t           n_TOWER_CALIB_LG_HCALIN;
   Int_t           TOWER_CALIB_LG_HCALIN_;
   UInt_t          TOWER_CALIB_LG_HCALIN_fUniqueID[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   UInt_t          TOWER_CALIB_LG_HCALIN_fBits[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   UInt_t          TOWER_CALIB_LG_HCALIN_towerid[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Double_t        TOWER_CALIB_LG_HCALIN_energy[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Float_t         TOWER_CALIB_LG_HCALIN_time[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Float_t         TOWER_CALIB_LG_HCALIN_signal_samples[kMaxTOWER_CALIB_LG_HCALIN][31];   //[TOWER_CALIB_LG_HCALIN_]
   Int_t           TOWER_CALIB_LG_HCALIN_HBD_channel[kMaxTOWER_CALIB_LG_HCALIN];   //[TOWER_CALIB_LG_HCALIN_]
   Int_t           n_TOWER_CALIB_LG_HCALOUT;
   Int_t           TOWER_CALIB_LG_HCALOUT_;
   UInt_t          TOWER_CALIB_LG_HCALOUT_fUniqueID[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   UInt_t          TOWER_CALIB_LG_HCALOUT_fBits[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   UInt_t          TOWER_CALIB_LG_HCALOUT_towerid[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Double_t        TOWER_CALIB_LG_HCALOUT_energy[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Float_t         TOWER_CALIB_LG_HCALOUT_time[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Float_t         TOWER_CALIB_LG_HCALOUT_signal_samples[kMaxTOWER_CALIB_LG_HCALOUT][31];   //[TOWER_CALIB_LG_HCALOUT_]
   Int_t           TOWER_CALIB_LG_HCALOUT_HBD_channel[kMaxTOWER_CALIB_LG_HCALOUT];   //[TOWER_CALIB_LG_HCALOUT_]
   Int_t           n_TOWER_CALIB_HG_HCALOUT;
   Int_t           TOWER_CALIB_HG_HCALOUT_;
   UInt_t          TOWER_CALIB_HG_HCALOUT_fUniqueID[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   UInt_t          TOWER_CALIB_HG_HCALOUT_fBits[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   UInt_t          TOWER_CALIB_HG_HCALOUT_towerid[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Double_t        TOWER_CALIB_HG_HCALOUT_energy[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Float_t         TOWER_CALIB_HG_HCALOUT_time[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Float_t         TOWER_CALIB_HG_HCALOUT_signal_samples[kMaxTOWER_CALIB_HG_HCALOUT][31];   //[TOWER_CALIB_HG_HCALOUT_]
   Int_t           TOWER_CALIB_HG_HCALOUT_HBD_channel[kMaxTOWER_CALIB_HG_HCALOUT];   //[TOWER_CALIB_HG_HCALOUT_]
   Int_t           n_TOWER_RAW_CEMC;
   Int_t           TOWER_RAW_CEMC_;
   UInt_t          TOWER_RAW_CEMC_fUniqueID[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   UInt_t          TOWER_RAW_CEMC_fBits[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   UInt_t          TOWER_RAW_CEMC_towerid[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Double_t        TOWER_RAW_CEMC_energy[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Float_t         TOWER_RAW_CEMC_time[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Float_t         TOWER_RAW_CEMC_signal_samples[kMaxTOWER_RAW_CEMC][31];   //[TOWER_RAW_CEMC_]
   Int_t           TOWER_RAW_CEMC_HBD_channel[kMaxTOWER_RAW_CEMC];   //[TOWER_RAW_CEMC_]
   Int_t           n_TOWER_CALIB_CEMC;
   Int_t           TOWER_CALIB_CEMC_;
   UInt_t          TOWER_CALIB_CEMC_fUniqueID[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   UInt_t          TOWER_CALIB_CEMC_fBits[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   UInt_t          TOWER_CALIB_CEMC_towerid[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Double_t        TOWER_CALIB_CEMC_energy[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Float_t         TOWER_CALIB_CEMC_time[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Float_t         TOWER_CALIB_CEMC_signal_samples[kMaxTOWER_CALIB_CEMC][31];   //[TOWER_CALIB_CEMC_]
   Int_t           TOWER_CALIB_CEMC_HBD_channel[kMaxTOWER_CALIB_CEMC];   //[TOWER_CALIB_CEMC_]
   Int_t           n_TOWER_RAW_HODO_VERTICAL;
   Int_t           TOWER_RAW_HODO_VERTICAL_;
   UInt_t          TOWER_RAW_HODO_VERTICAL_fUniqueID[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   UInt_t          TOWER_RAW_HODO_VERTICAL_fBits[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   UInt_t          TOWER_RAW_HODO_VERTICAL_towerid[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Double_t        TOWER_RAW_HODO_VERTICAL_energy[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Float_t         TOWER_RAW_HODO_VERTICAL_time[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Float_t         TOWER_RAW_HODO_VERTICAL_signal_samples[kMaxTOWER_RAW_HODO_VERTICAL][31];   //[TOWER_RAW_HODO_VERTICAL_]
   Int_t           TOWER_RAW_HODO_VERTICAL_HBD_channel[kMaxTOWER_RAW_HODO_VERTICAL];   //[TOWER_RAW_HODO_VERTICAL_]
   Int_t           n_TOWER_RAW_HODO_HORIZONTAL;
   Int_t           TOWER_RAW_HODO_HORIZONTAL_;
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_fUniqueID[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_fBits[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   UInt_t          TOWER_RAW_HODO_HORIZONTAL_towerid[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Double_t        TOWER_RAW_HODO_HORIZONTAL_energy[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Float_t         TOWER_RAW_HODO_HORIZONTAL_time[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Float_t         TOWER_RAW_HODO_HORIZONTAL_signal_samples[kMaxTOWER_RAW_HODO_HORIZONTAL][31];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Int_t           TOWER_RAW_HODO_HORIZONTAL_HBD_channel[kMaxTOWER_RAW_HODO_HORIZONTAL];   //[TOWER_RAW_HODO_HORIZONTAL_]
   Int_t           n_TOWER_CALIB_HODO_VERTICAL;
   Int_t           TOWER_CALIB_HODO_VERTICAL_;
   UInt_t          TOWER_CALIB_HODO_VERTICAL_fUniqueID[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   UInt_t          TOWER_CALIB_HODO_VERTICAL_fBits[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   UInt_t          TOWER_CALIB_HODO_VERTICAL_towerid[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Double_t        TOWER_CALIB_HODO_VERTICAL_energy[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Float_t         TOWER_CALIB_HODO_VERTICAL_time[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Float_t         TOWER_CALIB_HODO_VERTICAL_signal_samples[kMaxTOWER_CALIB_HODO_VERTICAL][31];   //[TOWER_CALIB_HODO_VERTICAL_]
   Int_t           TOWER_CALIB_HODO_VERTICAL_HBD_channel[kMaxTOWER_CALIB_HODO_VERTICAL];   //[TOWER_CALIB_HODO_VERTICAL_]
   Int_t           n_TOWER_CALIB_HODO_HORIZONTAL;
   Int_t           TOWER_CALIB_HODO_HORIZONTAL_;
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_fUniqueID[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_fBits[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   UInt_t          TOWER_CALIB_HODO_HORIZONTAL_towerid[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Double_t        TOWER_CALIB_HODO_HORIZONTAL_energy[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Float_t         TOWER_CALIB_HODO_HORIZONTAL_time[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Float_t         TOWER_CALIB_HODO_HORIZONTAL_signal_samples[kMaxTOWER_CALIB_HODO_HORIZONTAL][31];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Int_t           TOWER_CALIB_HODO_HORIZONTAL_HBD_channel[kMaxTOWER_CALIB_HODO_HORIZONTAL];   //[TOWER_CALIB_HODO_HORIZONTAL_]
   Int_t           n_TOWER_RAW_C1;
   Int_t           TOWER_RAW_C1_;
   UInt_t          TOWER_RAW_C1_fUniqueID[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   UInt_t          TOWER_RAW_C1_fBits[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   UInt_t          TOWER_RAW_C1_towerid[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Double_t        TOWER_RAW_C1_energy[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Float_t         TOWER_RAW_C1_time[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Float_t         TOWER_RAW_C1_signal_samples[kMaxTOWER_RAW_C1][31];   //[TOWER_RAW_C1_]
   Int_t           TOWER_RAW_C1_HBD_channel[kMaxTOWER_RAW_C1];   //[TOWER_RAW_C1_]
   Int_t           n_TOWER_CALIB_C1;
   Int_t           TOWER_CALIB_C1_;
   UInt_t          TOWER_CALIB_C1_fUniqueID[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   UInt_t          TOWER_CALIB_C1_fBits[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   UInt_t          TOWER_CALIB_C1_towerid[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Double_t        TOWER_CALIB_C1_energy[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Float_t         TOWER_CALIB_C1_time[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Float_t         TOWER_CALIB_C1_signal_samples[kMaxTOWER_CALIB_C1][31];   //[TOWER_CALIB_C1_]
   Int_t           TOWER_CALIB_C1_HBD_channel[kMaxTOWER_CALIB_C1];   //[TOWER_CALIB_C1_]
   Int_t           n_TOWER_RAW_C2;
   Int_t           TOWER_RAW_C2_;
   UInt_t          TOWER_RAW_C2_fUniqueID[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   UInt_t          TOWER_RAW_C2_fBits[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   UInt_t          TOWER_RAW_C2_towerid[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Double_t        TOWER_RAW_C2_energy[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Float_t         TOWER_RAW_C2_time[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Float_t         TOWER_RAW_C2_signal_samples[kMaxTOWER_RAW_C2][31];   //[TOWER_RAW_C2_]
   Int_t           TOWER_RAW_C2_HBD_channel[kMaxTOWER_RAW_C2];   //[TOWER_RAW_C2_]
   Int_t           n_TOWER_CALIB_C2;
   Int_t           TOWER_CALIB_C2_;
   UInt_t          TOWER_CALIB_C2_fUniqueID[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   UInt_t          TOWER_CALIB_C2_fBits[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   UInt_t          TOWER_CALIB_C2_towerid[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Double_t        TOWER_CALIB_C2_energy[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Float_t         TOWER_CALIB_C2_time[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Float_t         TOWER_CALIB_C2_signal_samples[kMaxTOWER_CALIB_C2][31];   //[TOWER_CALIB_C2_]
   Int_t           TOWER_CALIB_C2_HBD_channel[kMaxTOWER_CALIB_C2];   //[TOWER_CALIB_C2_]
   Int_t           n_TOWER_RAW_PbGL;
   Int_t           TOWER_RAW_PbGL_;
   UInt_t          TOWER_RAW_PbGL_fUniqueID[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   UInt_t          TOWER_RAW_PbGL_fBits[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   UInt_t          TOWER_RAW_PbGL_towerid[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Double_t        TOWER_RAW_PbGL_energy[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Float_t         TOWER_RAW_PbGL_time[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Float_t         TOWER_RAW_PbGL_signal_samples[kMaxTOWER_RAW_PbGL][31];   //[TOWER_RAW_PbGL_]
   Int_t           TOWER_RAW_PbGL_HBD_channel[kMaxTOWER_RAW_PbGL];   //[TOWER_RAW_PbGL_]
   Int_t           n_TOWER_CALIB_PbGL;
   Int_t           TOWER_CALIB_PbGL_;
   UInt_t          TOWER_CALIB_PbGL_fUniqueID[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   UInt_t          TOWER_CALIB_PbGL_fBits[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   UInt_t          TOWER_CALIB_PbGL_towerid[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Double_t        TOWER_CALIB_PbGL_energy[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Float_t         TOWER_CALIB_PbGL_time[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Float_t         TOWER_CALIB_PbGL_signal_samples[kMaxTOWER_CALIB_PbGL][31];   //[TOWER_CALIB_PbGL_]
   Int_t           TOWER_CALIB_PbGL_HBD_channel[kMaxTOWER_CALIB_PbGL];   //[TOWER_CALIB_PbGL_]
   Int_t           n_TOWER_RAW_TRIGGER_VETO;
   Int_t           TOWER_RAW_TRIGGER_VETO_;
   UInt_t          TOWER_RAW_TRIGGER_VETO_fUniqueID[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   UInt_t          TOWER_RAW_TRIGGER_VETO_fBits[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   UInt_t          TOWER_RAW_TRIGGER_VETO_towerid[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Double_t        TOWER_RAW_TRIGGER_VETO_energy[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Float_t         TOWER_RAW_TRIGGER_VETO_time[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Float_t         TOWER_RAW_TRIGGER_VETO_signal_samples[kMaxTOWER_RAW_TRIGGER_VETO][31];   //[TOWER_RAW_TRIGGER_VETO_]
   Int_t           TOWER_RAW_TRIGGER_VETO_HBD_channel[kMaxTOWER_RAW_TRIGGER_VETO];   //[TOWER_RAW_TRIGGER_VETO_]
   Int_t           n_TOWER_CALIB_TRIGGER_VETO;
   Int_t           TOWER_CALIB_TRIGGER_VETO_;
   UInt_t          TOWER_CALIB_TRIGGER_VETO_fUniqueID[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   UInt_t          TOWER_CALIB_TRIGGER_VETO_fBits[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   UInt_t          TOWER_CALIB_TRIGGER_VETO_towerid[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Double_t        TOWER_CALIB_TRIGGER_VETO_energy[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Float_t         TOWER_CALIB_TRIGGER_VETO_time[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Float_t         TOWER_CALIB_TRIGGER_VETO_signal_samples[kMaxTOWER_CALIB_TRIGGER_VETO][31];   //[TOWER_CALIB_TRIGGER_VETO_]
   Int_t           TOWER_CALIB_TRIGGER_VETO_HBD_channel[kMaxTOWER_CALIB_TRIGGER_VETO];   //[TOWER_CALIB_TRIGGER_VETO_]
   Int_t           n_TOWER_RAW_TILE_MAPPER;
   Int_t           TOWER_RAW_TILE_MAPPER_;
   UInt_t          TOWER_RAW_TILE_MAPPER_fUniqueID[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   UInt_t          TOWER_RAW_TILE_MAPPER_fBits[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   UInt_t          TOWER_RAW_TILE_MAPPER_towerid[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Double_t        TOWER_RAW_TILE_MAPPER_energy[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Float_t         TOWER_RAW_TILE_MAPPER_time[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Float_t         TOWER_RAW_TILE_MAPPER_signal_samples[kMaxTOWER_RAW_TILE_MAPPER][31];   //[TOWER_RAW_TILE_MAPPER_]
   Int_t           TOWER_RAW_TILE_MAPPER_HBD_channel[kMaxTOWER_RAW_TILE_MAPPER];   //[TOWER_RAW_TILE_MAPPER_]
   Int_t           n_TOWER_CALIB_TILE_MAPPER;
   Int_t           TOWER_CALIB_TILE_MAPPER_;
   UInt_t          TOWER_CALIB_TILE_MAPPER_fUniqueID[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   UInt_t          TOWER_CALIB_TILE_MAPPER_fBits[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   UInt_t          TOWER_CALIB_TILE_MAPPER_towerid[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Double_t        TOWER_CALIB_TILE_MAPPER_energy[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Float_t         TOWER_CALIB_TILE_MAPPER_time[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Float_t         TOWER_CALIB_TILE_MAPPER_signal_samples[kMaxTOWER_CALIB_TILE_MAPPER][31];   //[TOWER_CALIB_TILE_MAPPER_]
   Int_t           TOWER_CALIB_TILE_MAPPER_HBD_channel[kMaxTOWER_CALIB_TILE_MAPPER];   //[TOWER_CALIB_TILE_MAPPER_]
   Int_t           n_TOWER_RAW_SC3;
   Int_t           TOWER_RAW_SC3_;
   UInt_t          TOWER_RAW_SC3_fUniqueID[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   UInt_t          TOWER_RAW_SC3_fBits[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   UInt_t          TOWER_RAW_SC3_towerid[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Double_t        TOWER_RAW_SC3_energy[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Float_t         TOWER_RAW_SC3_time[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Float_t         TOWER_RAW_SC3_signal_samples[kMaxTOWER_RAW_SC3][31];   //[TOWER_RAW_SC3_]
   Int_t           TOWER_RAW_SC3_HBD_channel[kMaxTOWER_RAW_SC3];   //[TOWER_RAW_SC3_]
   Int_t           n_TOWER_CALIB_SC3;
   Int_t           TOWER_CALIB_SC3_;
   UInt_t          TOWER_CALIB_SC3_fUniqueID[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   UInt_t          TOWER_CALIB_SC3_fBits[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   UInt_t          TOWER_CALIB_SC3_towerid[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Double_t        TOWER_CALIB_SC3_energy[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Float_t         TOWER_CALIB_SC3_time[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Float_t         TOWER_CALIB_SC3_signal_samples[kMaxTOWER_CALIB_SC3][31];   //[TOWER_CALIB_SC3_]
   Int_t           TOWER_CALIB_SC3_HBD_channel[kMaxTOWER_CALIB_SC3];   //[TOWER_CALIB_SC3_]
   Int_t           n_TOWER_RAW_FINGER_COUNTER;
   Int_t           TOWER_RAW_FINGER_COUNTER_;
   UInt_t          TOWER_RAW_FINGER_COUNTER_fUniqueID[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   UInt_t          TOWER_RAW_FINGER_COUNTER_fBits[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   UInt_t          TOWER_RAW_FINGER_COUNTER_towerid[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Double_t        TOWER_RAW_FINGER_COUNTER_energy[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Float_t         TOWER_RAW_FINGER_COUNTER_time[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Float_t         TOWER_RAW_FINGER_COUNTER_signal_samples[kMaxTOWER_RAW_FINGER_COUNTER][31];   //[TOWER_RAW_FINGER_COUNTER_]
   Int_t           TOWER_RAW_FINGER_COUNTER_HBD_channel[kMaxTOWER_RAW_FINGER_COUNTER];   //[TOWER_RAW_FINGER_COUNTER_]
   Int_t           n_TOWER_CALIB_FINGER_COUNTER;
   Int_t           TOWER_CALIB_FINGER_COUNTER_;
   UInt_t          TOWER_CALIB_FINGER_COUNTER_fUniqueID[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   UInt_t          TOWER_CALIB_FINGER_COUNTER_fBits[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   UInt_t          TOWER_CALIB_FINGER_COUNTER_towerid[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Double_t        TOWER_CALIB_FINGER_COUNTER_energy[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Float_t         TOWER_CALIB_FINGER_COUNTER_time[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]
   Float_t         TOWER_CALIB_FINGER_COUNTER_signal_samples[kMaxTOWER_CALIB_FINGER_COUNTER][31];   //[TOWER_CALIB_FINGER_COUNTER_]
   Int_t           TOWER_CALIB_FINGER_COUNTER_HBD_channel[kMaxTOWER_CALIB_FINGER_COUNTER];   //[TOWER_CALIB_FINGER_COUNTER_]

   // List of branches
   TBranch        *b_beam_MTNRG_GeV;   //!
   TBranch        *b_beam_2CH_mm;   //!
   TBranch        *b_beam_2CV_mm;   //!
   TBranch        *b_EMCAL_A0_HighGain;   //!
   TBranch        *b_EMCAL_GR0_BiasOffset_Tower21;   //!
   TBranch        *b_EMCAL_T0_Tower21;   //!
   TBranch        *b_EMCAL_Is_HighEta;   //!
   TBranch        *b_n_TOWER_RAW_LG_HCALIN;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_fBits;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_towerid;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_energy;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_time;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_signal_samples;   //!
   TBranch        *b_TOWER_RAW_LG_HCALIN_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_LG_HCALOUT;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_time;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_RAW_LG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HG_HCALOUT;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_time;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_LG_HCALIN;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_fBits;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_towerid;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_energy;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_time;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALIN_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_LG_HCALOUT;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_time;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_LG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HG_HCALOUT;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_fBits;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_towerid;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_energy;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_time;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HG_HCALOUT_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_CEMC;   //!
   TBranch        *b_TOWER_RAW_CEMC_;   //!
   TBranch        *b_TOWER_RAW_CEMC_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_CEMC_fBits;   //!
   TBranch        *b_TOWER_RAW_CEMC_towerid;   //!
   TBranch        *b_TOWER_RAW_CEMC_energy;   //!
   TBranch        *b_TOWER_RAW_CEMC_time;   //!
   TBranch        *b_TOWER_RAW_CEMC_signal_samples;   //!
   TBranch        *b_TOWER_RAW_CEMC_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_CEMC;   //!
   TBranch        *b_TOWER_CALIB_CEMC_;   //!
   TBranch        *b_TOWER_CALIB_CEMC_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_CEMC_fBits;   //!
   TBranch        *b_TOWER_CALIB_CEMC_towerid;   //!
   TBranch        *b_TOWER_CALIB_CEMC_energy;   //!
   TBranch        *b_TOWER_CALIB_CEMC_time;   //!
   TBranch        *b_TOWER_CALIB_CEMC_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_CEMC_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HODO_VERTICAL;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_fBits;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_towerid;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_energy;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_time;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HODO_VERTICAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_HODO_HORIZONTAL;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_fBits;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_towerid;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_energy;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_time;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_HODO_HORIZONTAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HODO_VERTICAL;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_fBits;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_towerid;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_energy;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_time;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HODO_VERTICAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_HODO_HORIZONTAL;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_fBits;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_towerid;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_energy;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_time;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_HODO_HORIZONTAL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_C1;   //!
   TBranch        *b_TOWER_RAW_C1_;   //!
   TBranch        *b_TOWER_RAW_C1_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_C1_fBits;   //!
   TBranch        *b_TOWER_RAW_C1_towerid;   //!
   TBranch        *b_TOWER_RAW_C1_energy;   //!
   TBranch        *b_TOWER_RAW_C1_time;   //!
   TBranch        *b_TOWER_RAW_C1_signal_samples;   //!
   TBranch        *b_TOWER_RAW_C1_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_C1;   //!
   TBranch        *b_TOWER_CALIB_C1_;   //!
   TBranch        *b_TOWER_CALIB_C1_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_C1_fBits;   //!
   TBranch        *b_TOWER_CALIB_C1_towerid;   //!
   TBranch        *b_TOWER_CALIB_C1_energy;   //!
   TBranch        *b_TOWER_CALIB_C1_time;   //!
   TBranch        *b_TOWER_CALIB_C1_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_C1_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_C2;   //!
   TBranch        *b_TOWER_RAW_C2_;   //!
   TBranch        *b_TOWER_RAW_C2_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_C2_fBits;   //!
   TBranch        *b_TOWER_RAW_C2_towerid;   //!
   TBranch        *b_TOWER_RAW_C2_energy;   //!
   TBranch        *b_TOWER_RAW_C2_time;   //!
   TBranch        *b_TOWER_RAW_C2_signal_samples;   //!
   TBranch        *b_TOWER_RAW_C2_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_C2;   //!
   TBranch        *b_TOWER_CALIB_C2_;   //!
   TBranch        *b_TOWER_CALIB_C2_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_C2_fBits;   //!
   TBranch        *b_TOWER_CALIB_C2_towerid;   //!
   TBranch        *b_TOWER_CALIB_C2_energy;   //!
   TBranch        *b_TOWER_CALIB_C2_time;   //!
   TBranch        *b_TOWER_CALIB_C2_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_C2_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_PbGL;   //!
   TBranch        *b_TOWER_RAW_PbGL_;   //!
   TBranch        *b_TOWER_RAW_PbGL_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_PbGL_fBits;   //!
   TBranch        *b_TOWER_RAW_PbGL_towerid;   //!
   TBranch        *b_TOWER_RAW_PbGL_energy;   //!
   TBranch        *b_TOWER_RAW_PbGL_time;   //!
   TBranch        *b_TOWER_RAW_PbGL_signal_samples;   //!
   TBranch        *b_TOWER_RAW_PbGL_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_PbGL;   //!
   TBranch        *b_TOWER_CALIB_PbGL_;   //!
   TBranch        *b_TOWER_CALIB_PbGL_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_PbGL_fBits;   //!
   TBranch        *b_TOWER_CALIB_PbGL_towerid;   //!
   TBranch        *b_TOWER_CALIB_PbGL_energy;   //!
   TBranch        *b_TOWER_CALIB_PbGL_time;   //!
   TBranch        *b_TOWER_CALIB_PbGL_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_PbGL_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_TRIGGER_VETO;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_fBits;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_towerid;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_energy;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_time;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_signal_samples;   //!
   TBranch        *b_TOWER_RAW_TRIGGER_VETO_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_TRIGGER_VETO;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_fBits;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_towerid;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_energy;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_time;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_TRIGGER_VETO_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_TILE_MAPPER;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_fBits;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_towerid;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_energy;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_time;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_signal_samples;   //!
   TBranch        *b_TOWER_RAW_TILE_MAPPER_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_TILE_MAPPER;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_fBits;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_towerid;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_energy;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_time;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_TILE_MAPPER_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_SC3;   //!
   TBranch        *b_TOWER_RAW_SC3_;   //!
   TBranch        *b_TOWER_RAW_SC3_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_SC3_fBits;   //!
   TBranch        *b_TOWER_RAW_SC3_towerid;   //!
   TBranch        *b_TOWER_RAW_SC3_energy;   //!
   TBranch        *b_TOWER_RAW_SC3_time;   //!
   TBranch        *b_TOWER_RAW_SC3_signal_samples;   //!
   TBranch        *b_TOWER_RAW_SC3_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_SC3;   //!
   TBranch        *b_TOWER_CALIB_SC3_;   //!
   TBranch        *b_TOWER_CALIB_SC3_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_SC3_fBits;   //!
   TBranch        *b_TOWER_CALIB_SC3_towerid;   //!
   TBranch        *b_TOWER_CALIB_SC3_energy;   //!
   TBranch        *b_TOWER_CALIB_SC3_time;   //!
   TBranch        *b_TOWER_CALIB_SC3_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_SC3_HBD_channel;   //!
   TBranch        *b_n_TOWER_RAW_FINGER_COUNTER;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_fUniqueID;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_fBits;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_towerid;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_energy;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_time;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_signal_samples;   //!
   TBranch        *b_TOWER_RAW_FINGER_COUNTER_HBD_channel;   //!
   TBranch        *b_n_TOWER_CALIB_FINGER_COUNTER;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_fUniqueID;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_fBits;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_towerid;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_energy;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_time;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_signal_samples;   //!
   TBranch        *b_TOWER_CALIB_FINGER_COUNTER_HBD_channel;   //!

   DSTReader551(TTree *tree=0);
   virtual ~DSTReader551();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DSTReader551_cxx
DSTReader551::DSTReader551(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("beam_00000551-0000_DSTReader.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("beam_00000551-0000_DSTReader.root");
      }
      f->GetObject("T",tree);

   }
   Init(tree);
}

DSTReader551::~DSTReader551()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DSTReader551::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DSTReader551::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DSTReader551::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("beam_MTNRG_GeV", &beam_MTNRG_GeV, &b_beam_MTNRG_GeV);
   fChain->SetBranchAddress("beam_2CH_mm", &beam_2CH_mm, &b_beam_2CH_mm);
   fChain->SetBranchAddress("beam_2CV_mm", &beam_2CV_mm, &b_beam_2CV_mm);
   fChain->SetBranchAddress("EMCAL_A0_HighGain", &EMCAL_A0_HighGain, &b_EMCAL_A0_HighGain);
   fChain->SetBranchAddress("EMCAL_GR0_BiasOffset_Tower21", &EMCAL_GR0_BiasOffset_Tower21, &b_EMCAL_GR0_BiasOffset_Tower21);
   fChain->SetBranchAddress("EMCAL_T0_Tower21", &EMCAL_T0_Tower21, &b_EMCAL_T0_Tower21);
   fChain->SetBranchAddress("EMCAL_Is_HighEta", &EMCAL_Is_HighEta, &b_EMCAL_Is_HighEta);
   fChain->SetBranchAddress("n_TOWER_RAW_LG_HCALIN", &n_TOWER_RAW_LG_HCALIN, &b_n_TOWER_RAW_LG_HCALIN);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN", &TOWER_RAW_LG_HCALIN_, &b_TOWER_RAW_LG_HCALIN_);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.fUniqueID", TOWER_RAW_LG_HCALIN_fUniqueID, &b_TOWER_RAW_LG_HCALIN_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.fBits", TOWER_RAW_LG_HCALIN_fBits, &b_TOWER_RAW_LG_HCALIN_fBits);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.towerid", TOWER_RAW_LG_HCALIN_towerid, &b_TOWER_RAW_LG_HCALIN_towerid);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.energy", TOWER_RAW_LG_HCALIN_energy, &b_TOWER_RAW_LG_HCALIN_energy);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.time", TOWER_RAW_LG_HCALIN_time, &b_TOWER_RAW_LG_HCALIN_time);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.signal_samples[31]", TOWER_RAW_LG_HCALIN_signal_samples, &b_TOWER_RAW_LG_HCALIN_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALIN.HBD_channel", TOWER_RAW_LG_HCALIN_HBD_channel, &b_TOWER_RAW_LG_HCALIN_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_LG_HCALOUT", &n_TOWER_RAW_LG_HCALOUT, &b_n_TOWER_RAW_LG_HCALOUT);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT", &TOWER_RAW_LG_HCALOUT_, &b_TOWER_RAW_LG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.fUniqueID", TOWER_RAW_LG_HCALOUT_fUniqueID, &b_TOWER_RAW_LG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.fBits", TOWER_RAW_LG_HCALOUT_fBits, &b_TOWER_RAW_LG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.towerid", TOWER_RAW_LG_HCALOUT_towerid, &b_TOWER_RAW_LG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.energy", TOWER_RAW_LG_HCALOUT_energy, &b_TOWER_RAW_LG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.time", TOWER_RAW_LG_HCALOUT_time, &b_TOWER_RAW_LG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.signal_samples[31]", TOWER_RAW_LG_HCALOUT_signal_samples, &b_TOWER_RAW_LG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_LG_HCALOUT.HBD_channel", TOWER_RAW_LG_HCALOUT_HBD_channel, &b_TOWER_RAW_LG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HG_HCALOUT", &n_TOWER_RAW_HG_HCALOUT, &b_n_TOWER_RAW_HG_HCALOUT);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT", &TOWER_RAW_HG_HCALOUT_, &b_TOWER_RAW_HG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.fUniqueID", TOWER_RAW_HG_HCALOUT_fUniqueID, &b_TOWER_RAW_HG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.fBits", TOWER_RAW_HG_HCALOUT_fBits, &b_TOWER_RAW_HG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.towerid", TOWER_RAW_HG_HCALOUT_towerid, &b_TOWER_RAW_HG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.energy", TOWER_RAW_HG_HCALOUT_energy, &b_TOWER_RAW_HG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.time", TOWER_RAW_HG_HCALOUT_time, &b_TOWER_RAW_HG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.signal_samples[31]", TOWER_RAW_HG_HCALOUT_signal_samples, &b_TOWER_RAW_HG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HG_HCALOUT.HBD_channel", TOWER_RAW_HG_HCALOUT_HBD_channel, &b_TOWER_RAW_HG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_LG_HCALIN", &n_TOWER_CALIB_LG_HCALIN, &b_n_TOWER_CALIB_LG_HCALIN);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN", &TOWER_CALIB_LG_HCALIN_, &b_TOWER_CALIB_LG_HCALIN_);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.fUniqueID", TOWER_CALIB_LG_HCALIN_fUniqueID, &b_TOWER_CALIB_LG_HCALIN_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.fBits", TOWER_CALIB_LG_HCALIN_fBits, &b_TOWER_CALIB_LG_HCALIN_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.towerid", TOWER_CALIB_LG_HCALIN_towerid, &b_TOWER_CALIB_LG_HCALIN_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.energy", TOWER_CALIB_LG_HCALIN_energy, &b_TOWER_CALIB_LG_HCALIN_energy);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.time", TOWER_CALIB_LG_HCALIN_time, &b_TOWER_CALIB_LG_HCALIN_time);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.signal_samples[31]", TOWER_CALIB_LG_HCALIN_signal_samples, &b_TOWER_CALIB_LG_HCALIN_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALIN.HBD_channel", TOWER_CALIB_LG_HCALIN_HBD_channel, &b_TOWER_CALIB_LG_HCALIN_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_LG_HCALOUT", &n_TOWER_CALIB_LG_HCALOUT, &b_n_TOWER_CALIB_LG_HCALOUT);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT", &TOWER_CALIB_LG_HCALOUT_, &b_TOWER_CALIB_LG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.fUniqueID", TOWER_CALIB_LG_HCALOUT_fUniqueID, &b_TOWER_CALIB_LG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.fBits", TOWER_CALIB_LG_HCALOUT_fBits, &b_TOWER_CALIB_LG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.towerid", TOWER_CALIB_LG_HCALOUT_towerid, &b_TOWER_CALIB_LG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.energy", TOWER_CALIB_LG_HCALOUT_energy, &b_TOWER_CALIB_LG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.time", TOWER_CALIB_LG_HCALOUT_time, &b_TOWER_CALIB_LG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.signal_samples[31]", TOWER_CALIB_LG_HCALOUT_signal_samples, &b_TOWER_CALIB_LG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_LG_HCALOUT.HBD_channel", TOWER_CALIB_LG_HCALOUT_HBD_channel, &b_TOWER_CALIB_LG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HG_HCALOUT", &n_TOWER_CALIB_HG_HCALOUT, &b_n_TOWER_CALIB_HG_HCALOUT);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT", &TOWER_CALIB_HG_HCALOUT_, &b_TOWER_CALIB_HG_HCALOUT_);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.fUniqueID", TOWER_CALIB_HG_HCALOUT_fUniqueID, &b_TOWER_CALIB_HG_HCALOUT_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.fBits", TOWER_CALIB_HG_HCALOUT_fBits, &b_TOWER_CALIB_HG_HCALOUT_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.towerid", TOWER_CALIB_HG_HCALOUT_towerid, &b_TOWER_CALIB_HG_HCALOUT_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.energy", TOWER_CALIB_HG_HCALOUT_energy, &b_TOWER_CALIB_HG_HCALOUT_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.time", TOWER_CALIB_HG_HCALOUT_time, &b_TOWER_CALIB_HG_HCALOUT_time);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.signal_samples[31]", TOWER_CALIB_HG_HCALOUT_signal_samples, &b_TOWER_CALIB_HG_HCALOUT_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HG_HCALOUT.HBD_channel", TOWER_CALIB_HG_HCALOUT_HBD_channel, &b_TOWER_CALIB_HG_HCALOUT_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_CEMC", &n_TOWER_RAW_CEMC, &b_n_TOWER_RAW_CEMC);
   fChain->SetBranchAddress("TOWER_RAW_CEMC", &TOWER_RAW_CEMC_, &b_TOWER_RAW_CEMC_);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.fUniqueID", TOWER_RAW_CEMC_fUniqueID, &b_TOWER_RAW_CEMC_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.fBits", TOWER_RAW_CEMC_fBits, &b_TOWER_RAW_CEMC_fBits);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.towerid", TOWER_RAW_CEMC_towerid, &b_TOWER_RAW_CEMC_towerid);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.energy", TOWER_RAW_CEMC_energy, &b_TOWER_RAW_CEMC_energy);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.time", TOWER_RAW_CEMC_time, &b_TOWER_RAW_CEMC_time);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.signal_samples[31]", TOWER_RAW_CEMC_signal_samples, &b_TOWER_RAW_CEMC_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_CEMC.HBD_channel", TOWER_RAW_CEMC_HBD_channel, &b_TOWER_RAW_CEMC_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_CEMC", &n_TOWER_CALIB_CEMC, &b_n_TOWER_CALIB_CEMC);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC", &TOWER_CALIB_CEMC_, &b_TOWER_CALIB_CEMC_);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.fUniqueID", TOWER_CALIB_CEMC_fUniqueID, &b_TOWER_CALIB_CEMC_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.fBits", TOWER_CALIB_CEMC_fBits, &b_TOWER_CALIB_CEMC_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.towerid", TOWER_CALIB_CEMC_towerid, &b_TOWER_CALIB_CEMC_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.energy", TOWER_CALIB_CEMC_energy, &b_TOWER_CALIB_CEMC_energy);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.time", TOWER_CALIB_CEMC_time, &b_TOWER_CALIB_CEMC_time);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.signal_samples[31]", TOWER_CALIB_CEMC_signal_samples, &b_TOWER_CALIB_CEMC_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_CEMC.HBD_channel", TOWER_CALIB_CEMC_HBD_channel, &b_TOWER_CALIB_CEMC_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HODO_VERTICAL", &n_TOWER_RAW_HODO_VERTICAL, &b_n_TOWER_RAW_HODO_VERTICAL);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL", &TOWER_RAW_HODO_VERTICAL_, &b_TOWER_RAW_HODO_VERTICAL_);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.fUniqueID", TOWER_RAW_HODO_VERTICAL_fUniqueID, &b_TOWER_RAW_HODO_VERTICAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.fBits", TOWER_RAW_HODO_VERTICAL_fBits, &b_TOWER_RAW_HODO_VERTICAL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.towerid", TOWER_RAW_HODO_VERTICAL_towerid, &b_TOWER_RAW_HODO_VERTICAL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.energy", TOWER_RAW_HODO_VERTICAL_energy, &b_TOWER_RAW_HODO_VERTICAL_energy);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.time", TOWER_RAW_HODO_VERTICAL_time, &b_TOWER_RAW_HODO_VERTICAL_time);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.signal_samples[31]", TOWER_RAW_HODO_VERTICAL_signal_samples, &b_TOWER_RAW_HODO_VERTICAL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HODO_VERTICAL.HBD_channel", TOWER_RAW_HODO_VERTICAL_HBD_channel, &b_TOWER_RAW_HODO_VERTICAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_HODO_HORIZONTAL", &n_TOWER_RAW_HODO_HORIZONTAL, &b_n_TOWER_RAW_HODO_HORIZONTAL);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL", &TOWER_RAW_HODO_HORIZONTAL_, &b_TOWER_RAW_HODO_HORIZONTAL_);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.fUniqueID", TOWER_RAW_HODO_HORIZONTAL_fUniqueID, &b_TOWER_RAW_HODO_HORIZONTAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.fBits", TOWER_RAW_HODO_HORIZONTAL_fBits, &b_TOWER_RAW_HODO_HORIZONTAL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.towerid", TOWER_RAW_HODO_HORIZONTAL_towerid, &b_TOWER_RAW_HODO_HORIZONTAL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.energy", TOWER_RAW_HODO_HORIZONTAL_energy, &b_TOWER_RAW_HODO_HORIZONTAL_energy);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.time", TOWER_RAW_HODO_HORIZONTAL_time, &b_TOWER_RAW_HODO_HORIZONTAL_time);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.signal_samples[31]", TOWER_RAW_HODO_HORIZONTAL_signal_samples, &b_TOWER_RAW_HODO_HORIZONTAL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_HODO_HORIZONTAL.HBD_channel", TOWER_RAW_HODO_HORIZONTAL_HBD_channel, &b_TOWER_RAW_HODO_HORIZONTAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HODO_VERTICAL", &n_TOWER_CALIB_HODO_VERTICAL, &b_n_TOWER_CALIB_HODO_VERTICAL);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL", &TOWER_CALIB_HODO_VERTICAL_, &b_TOWER_CALIB_HODO_VERTICAL_);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.fUniqueID", TOWER_CALIB_HODO_VERTICAL_fUniqueID, &b_TOWER_CALIB_HODO_VERTICAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.fBits", TOWER_CALIB_HODO_VERTICAL_fBits, &b_TOWER_CALIB_HODO_VERTICAL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.towerid", TOWER_CALIB_HODO_VERTICAL_towerid, &b_TOWER_CALIB_HODO_VERTICAL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.energy", TOWER_CALIB_HODO_VERTICAL_energy, &b_TOWER_CALIB_HODO_VERTICAL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.time", TOWER_CALIB_HODO_VERTICAL_time, &b_TOWER_CALIB_HODO_VERTICAL_time);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.signal_samples[31]", TOWER_CALIB_HODO_VERTICAL_signal_samples, &b_TOWER_CALIB_HODO_VERTICAL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_VERTICAL.HBD_channel", TOWER_CALIB_HODO_VERTICAL_HBD_channel, &b_TOWER_CALIB_HODO_VERTICAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_HODO_HORIZONTAL", &n_TOWER_CALIB_HODO_HORIZONTAL, &b_n_TOWER_CALIB_HODO_HORIZONTAL);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL", &TOWER_CALIB_HODO_HORIZONTAL_, &b_TOWER_CALIB_HODO_HORIZONTAL_);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.fUniqueID", TOWER_CALIB_HODO_HORIZONTAL_fUniqueID, &b_TOWER_CALIB_HODO_HORIZONTAL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.fBits", TOWER_CALIB_HODO_HORIZONTAL_fBits, &b_TOWER_CALIB_HODO_HORIZONTAL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.towerid", TOWER_CALIB_HODO_HORIZONTAL_towerid, &b_TOWER_CALIB_HODO_HORIZONTAL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.energy", TOWER_CALIB_HODO_HORIZONTAL_energy, &b_TOWER_CALIB_HODO_HORIZONTAL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.time", TOWER_CALIB_HODO_HORIZONTAL_time, &b_TOWER_CALIB_HODO_HORIZONTAL_time);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.signal_samples[31]", TOWER_CALIB_HODO_HORIZONTAL_signal_samples, &b_TOWER_CALIB_HODO_HORIZONTAL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_HODO_HORIZONTAL.HBD_channel", TOWER_CALIB_HODO_HORIZONTAL_HBD_channel, &b_TOWER_CALIB_HODO_HORIZONTAL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_C1", &n_TOWER_RAW_C1, &b_n_TOWER_RAW_C1);
   fChain->SetBranchAddress("TOWER_RAW_C1", &TOWER_RAW_C1_, &b_TOWER_RAW_C1_);
   fChain->SetBranchAddress("TOWER_RAW_C1.fUniqueID", TOWER_RAW_C1_fUniqueID, &b_TOWER_RAW_C1_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_C1.fBits", TOWER_RAW_C1_fBits, &b_TOWER_RAW_C1_fBits);
   fChain->SetBranchAddress("TOWER_RAW_C1.towerid", TOWER_RAW_C1_towerid, &b_TOWER_RAW_C1_towerid);
   fChain->SetBranchAddress("TOWER_RAW_C1.energy", TOWER_RAW_C1_energy, &b_TOWER_RAW_C1_energy);
   fChain->SetBranchAddress("TOWER_RAW_C1.time", TOWER_RAW_C1_time, &b_TOWER_RAW_C1_time);
   fChain->SetBranchAddress("TOWER_RAW_C1.signal_samples[31]", TOWER_RAW_C1_signal_samples, &b_TOWER_RAW_C1_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_C1.HBD_channel", TOWER_RAW_C1_HBD_channel, &b_TOWER_RAW_C1_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_C1", &n_TOWER_CALIB_C1, &b_n_TOWER_CALIB_C1);
   fChain->SetBranchAddress("TOWER_CALIB_C1", &TOWER_CALIB_C1_, &b_TOWER_CALIB_C1_);
   fChain->SetBranchAddress("TOWER_CALIB_C1.fUniqueID", TOWER_CALIB_C1_fUniqueID, &b_TOWER_CALIB_C1_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_C1.fBits", TOWER_CALIB_C1_fBits, &b_TOWER_CALIB_C1_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_C1.towerid", TOWER_CALIB_C1_towerid, &b_TOWER_CALIB_C1_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_C1.energy", TOWER_CALIB_C1_energy, &b_TOWER_CALIB_C1_energy);
   fChain->SetBranchAddress("TOWER_CALIB_C1.time", TOWER_CALIB_C1_time, &b_TOWER_CALIB_C1_time);
   fChain->SetBranchAddress("TOWER_CALIB_C1.signal_samples[31]", TOWER_CALIB_C1_signal_samples, &b_TOWER_CALIB_C1_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_C1.HBD_channel", TOWER_CALIB_C1_HBD_channel, &b_TOWER_CALIB_C1_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_C2", &n_TOWER_RAW_C2, &b_n_TOWER_RAW_C2);
   fChain->SetBranchAddress("TOWER_RAW_C2", &TOWER_RAW_C2_, &b_TOWER_RAW_C2_);
   fChain->SetBranchAddress("TOWER_RAW_C2.fUniqueID", TOWER_RAW_C2_fUniqueID, &b_TOWER_RAW_C2_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_C2.fBits", TOWER_RAW_C2_fBits, &b_TOWER_RAW_C2_fBits);
   fChain->SetBranchAddress("TOWER_RAW_C2.towerid", TOWER_RAW_C2_towerid, &b_TOWER_RAW_C2_towerid);
   fChain->SetBranchAddress("TOWER_RAW_C2.energy", TOWER_RAW_C2_energy, &b_TOWER_RAW_C2_energy);
   fChain->SetBranchAddress("TOWER_RAW_C2.time", TOWER_RAW_C2_time, &b_TOWER_RAW_C2_time);
   fChain->SetBranchAddress("TOWER_RAW_C2.signal_samples[31]", TOWER_RAW_C2_signal_samples, &b_TOWER_RAW_C2_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_C2.HBD_channel", TOWER_RAW_C2_HBD_channel, &b_TOWER_RAW_C2_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_C2", &n_TOWER_CALIB_C2, &b_n_TOWER_CALIB_C2);
   fChain->SetBranchAddress("TOWER_CALIB_C2", &TOWER_CALIB_C2_, &b_TOWER_CALIB_C2_);
   fChain->SetBranchAddress("TOWER_CALIB_C2.fUniqueID", TOWER_CALIB_C2_fUniqueID, &b_TOWER_CALIB_C2_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_C2.fBits", TOWER_CALIB_C2_fBits, &b_TOWER_CALIB_C2_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_C2.towerid", TOWER_CALIB_C2_towerid, &b_TOWER_CALIB_C2_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_C2.energy", TOWER_CALIB_C2_energy, &b_TOWER_CALIB_C2_energy);
   fChain->SetBranchAddress("TOWER_CALIB_C2.time", TOWER_CALIB_C2_time, &b_TOWER_CALIB_C2_time);
   fChain->SetBranchAddress("TOWER_CALIB_C2.signal_samples[31]", TOWER_CALIB_C2_signal_samples, &b_TOWER_CALIB_C2_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_C2.HBD_channel", TOWER_CALIB_C2_HBD_channel, &b_TOWER_CALIB_C2_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_PbGL", &n_TOWER_RAW_PbGL, &b_n_TOWER_RAW_PbGL);
   fChain->SetBranchAddress("TOWER_RAW_PbGL", &TOWER_RAW_PbGL_, &b_TOWER_RAW_PbGL_);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.fUniqueID", TOWER_RAW_PbGL_fUniqueID, &b_TOWER_RAW_PbGL_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.fBits", TOWER_RAW_PbGL_fBits, &b_TOWER_RAW_PbGL_fBits);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.towerid", TOWER_RAW_PbGL_towerid, &b_TOWER_RAW_PbGL_towerid);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.energy", TOWER_RAW_PbGL_energy, &b_TOWER_RAW_PbGL_energy);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.time", TOWER_RAW_PbGL_time, &b_TOWER_RAW_PbGL_time);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.signal_samples[31]", TOWER_RAW_PbGL_signal_samples, &b_TOWER_RAW_PbGL_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_PbGL.HBD_channel", TOWER_RAW_PbGL_HBD_channel, &b_TOWER_RAW_PbGL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_PbGL", &n_TOWER_CALIB_PbGL, &b_n_TOWER_CALIB_PbGL);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL", &TOWER_CALIB_PbGL_, &b_TOWER_CALIB_PbGL_);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.fUniqueID", TOWER_CALIB_PbGL_fUniqueID, &b_TOWER_CALIB_PbGL_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.fBits", TOWER_CALIB_PbGL_fBits, &b_TOWER_CALIB_PbGL_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.towerid", TOWER_CALIB_PbGL_towerid, &b_TOWER_CALIB_PbGL_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.energy", TOWER_CALIB_PbGL_energy, &b_TOWER_CALIB_PbGL_energy);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.time", TOWER_CALIB_PbGL_time, &b_TOWER_CALIB_PbGL_time);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.signal_samples[31]", TOWER_CALIB_PbGL_signal_samples, &b_TOWER_CALIB_PbGL_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_PbGL.HBD_channel", TOWER_CALIB_PbGL_HBD_channel, &b_TOWER_CALIB_PbGL_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_TRIGGER_VETO", &n_TOWER_RAW_TRIGGER_VETO, &b_n_TOWER_RAW_TRIGGER_VETO);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO", &TOWER_RAW_TRIGGER_VETO_, &b_TOWER_RAW_TRIGGER_VETO_);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.fUniqueID", TOWER_RAW_TRIGGER_VETO_fUniqueID, &b_TOWER_RAW_TRIGGER_VETO_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.fBits", TOWER_RAW_TRIGGER_VETO_fBits, &b_TOWER_RAW_TRIGGER_VETO_fBits);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.towerid", TOWER_RAW_TRIGGER_VETO_towerid, &b_TOWER_RAW_TRIGGER_VETO_towerid);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.energy", TOWER_RAW_TRIGGER_VETO_energy, &b_TOWER_RAW_TRIGGER_VETO_energy);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.time", TOWER_RAW_TRIGGER_VETO_time, &b_TOWER_RAW_TRIGGER_VETO_time);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.signal_samples[31]", TOWER_RAW_TRIGGER_VETO_signal_samples, &b_TOWER_RAW_TRIGGER_VETO_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_TRIGGER_VETO.HBD_channel", TOWER_RAW_TRIGGER_VETO_HBD_channel, &b_TOWER_RAW_TRIGGER_VETO_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_TRIGGER_VETO", &n_TOWER_CALIB_TRIGGER_VETO, &b_n_TOWER_CALIB_TRIGGER_VETO);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO", &TOWER_CALIB_TRIGGER_VETO_, &b_TOWER_CALIB_TRIGGER_VETO_);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.fUniqueID", TOWER_CALIB_TRIGGER_VETO_fUniqueID, &b_TOWER_CALIB_TRIGGER_VETO_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.fBits", TOWER_CALIB_TRIGGER_VETO_fBits, &b_TOWER_CALIB_TRIGGER_VETO_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.towerid", TOWER_CALIB_TRIGGER_VETO_towerid, &b_TOWER_CALIB_TRIGGER_VETO_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.energy", TOWER_CALIB_TRIGGER_VETO_energy, &b_TOWER_CALIB_TRIGGER_VETO_energy);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.time", TOWER_CALIB_TRIGGER_VETO_time, &b_TOWER_CALIB_TRIGGER_VETO_time);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.signal_samples[31]", TOWER_CALIB_TRIGGER_VETO_signal_samples, &b_TOWER_CALIB_TRIGGER_VETO_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_TRIGGER_VETO.HBD_channel", TOWER_CALIB_TRIGGER_VETO_HBD_channel, &b_TOWER_CALIB_TRIGGER_VETO_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_TILE_MAPPER", &n_TOWER_RAW_TILE_MAPPER, &b_n_TOWER_RAW_TILE_MAPPER);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER", &TOWER_RAW_TILE_MAPPER_, &b_TOWER_RAW_TILE_MAPPER_);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.fUniqueID", TOWER_RAW_TILE_MAPPER_fUniqueID, &b_TOWER_RAW_TILE_MAPPER_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.fBits", TOWER_RAW_TILE_MAPPER_fBits, &b_TOWER_RAW_TILE_MAPPER_fBits);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.towerid", TOWER_RAW_TILE_MAPPER_towerid, &b_TOWER_RAW_TILE_MAPPER_towerid);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.energy", TOWER_RAW_TILE_MAPPER_energy, &b_TOWER_RAW_TILE_MAPPER_energy);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.time", TOWER_RAW_TILE_MAPPER_time, &b_TOWER_RAW_TILE_MAPPER_time);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.signal_samples[31]", TOWER_RAW_TILE_MAPPER_signal_samples, &b_TOWER_RAW_TILE_MAPPER_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_TILE_MAPPER.HBD_channel", TOWER_RAW_TILE_MAPPER_HBD_channel, &b_TOWER_RAW_TILE_MAPPER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_TILE_MAPPER", &n_TOWER_CALIB_TILE_MAPPER, &b_n_TOWER_CALIB_TILE_MAPPER);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER", &TOWER_CALIB_TILE_MAPPER_, &b_TOWER_CALIB_TILE_MAPPER_);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.fUniqueID", TOWER_CALIB_TILE_MAPPER_fUniqueID, &b_TOWER_CALIB_TILE_MAPPER_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.fBits", TOWER_CALIB_TILE_MAPPER_fBits, &b_TOWER_CALIB_TILE_MAPPER_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.towerid", TOWER_CALIB_TILE_MAPPER_towerid, &b_TOWER_CALIB_TILE_MAPPER_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.energy", TOWER_CALIB_TILE_MAPPER_energy, &b_TOWER_CALIB_TILE_MAPPER_energy);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.time", TOWER_CALIB_TILE_MAPPER_time, &b_TOWER_CALIB_TILE_MAPPER_time);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.signal_samples[31]", TOWER_CALIB_TILE_MAPPER_signal_samples, &b_TOWER_CALIB_TILE_MAPPER_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_TILE_MAPPER.HBD_channel", TOWER_CALIB_TILE_MAPPER_HBD_channel, &b_TOWER_CALIB_TILE_MAPPER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_SC3", &n_TOWER_RAW_SC3, &b_n_TOWER_RAW_SC3);
   fChain->SetBranchAddress("TOWER_RAW_SC3", &TOWER_RAW_SC3_, &b_TOWER_RAW_SC3_);
   fChain->SetBranchAddress("TOWER_RAW_SC3.fUniqueID", TOWER_RAW_SC3_fUniqueID, &b_TOWER_RAW_SC3_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_SC3.fBits", TOWER_RAW_SC3_fBits, &b_TOWER_RAW_SC3_fBits);
   fChain->SetBranchAddress("TOWER_RAW_SC3.towerid", TOWER_RAW_SC3_towerid, &b_TOWER_RAW_SC3_towerid);
   fChain->SetBranchAddress("TOWER_RAW_SC3.energy", TOWER_RAW_SC3_energy, &b_TOWER_RAW_SC3_energy);
   fChain->SetBranchAddress("TOWER_RAW_SC3.time", TOWER_RAW_SC3_time, &b_TOWER_RAW_SC3_time);
   fChain->SetBranchAddress("TOWER_RAW_SC3.signal_samples[31]", TOWER_RAW_SC3_signal_samples, &b_TOWER_RAW_SC3_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_SC3.HBD_channel", TOWER_RAW_SC3_HBD_channel, &b_TOWER_RAW_SC3_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_SC3", &n_TOWER_CALIB_SC3, &b_n_TOWER_CALIB_SC3);
   fChain->SetBranchAddress("TOWER_CALIB_SC3", &TOWER_CALIB_SC3_, &b_TOWER_CALIB_SC3_);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.fUniqueID", TOWER_CALIB_SC3_fUniqueID, &b_TOWER_CALIB_SC3_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.fBits", TOWER_CALIB_SC3_fBits, &b_TOWER_CALIB_SC3_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.towerid", TOWER_CALIB_SC3_towerid, &b_TOWER_CALIB_SC3_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.energy", TOWER_CALIB_SC3_energy, &b_TOWER_CALIB_SC3_energy);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.time", TOWER_CALIB_SC3_time, &b_TOWER_CALIB_SC3_time);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.signal_samples[31]", TOWER_CALIB_SC3_signal_samples, &b_TOWER_CALIB_SC3_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_SC3.HBD_channel", TOWER_CALIB_SC3_HBD_channel, &b_TOWER_CALIB_SC3_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_RAW_FINGER_COUNTER", &n_TOWER_RAW_FINGER_COUNTER, &b_n_TOWER_RAW_FINGER_COUNTER);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER", &TOWER_RAW_FINGER_COUNTER_, &b_TOWER_RAW_FINGER_COUNTER_);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.fUniqueID", TOWER_RAW_FINGER_COUNTER_fUniqueID, &b_TOWER_RAW_FINGER_COUNTER_fUniqueID);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.fBits", TOWER_RAW_FINGER_COUNTER_fBits, &b_TOWER_RAW_FINGER_COUNTER_fBits);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.towerid", TOWER_RAW_FINGER_COUNTER_towerid, &b_TOWER_RAW_FINGER_COUNTER_towerid);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.energy", TOWER_RAW_FINGER_COUNTER_energy, &b_TOWER_RAW_FINGER_COUNTER_energy);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.time", TOWER_RAW_FINGER_COUNTER_time, &b_TOWER_RAW_FINGER_COUNTER_time);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.signal_samples[31]", TOWER_RAW_FINGER_COUNTER_signal_samples, &b_TOWER_RAW_FINGER_COUNTER_signal_samples);
   fChain->SetBranchAddress("TOWER_RAW_FINGER_COUNTER.HBD_channel", TOWER_RAW_FINGER_COUNTER_HBD_channel, &b_TOWER_RAW_FINGER_COUNTER_HBD_channel);
   fChain->SetBranchAddress("n_TOWER_CALIB_FINGER_COUNTER", &n_TOWER_CALIB_FINGER_COUNTER, &b_n_TOWER_CALIB_FINGER_COUNTER);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER", &TOWER_CALIB_FINGER_COUNTER_, &b_TOWER_CALIB_FINGER_COUNTER_);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.fUniqueID", TOWER_CALIB_FINGER_COUNTER_fUniqueID, &b_TOWER_CALIB_FINGER_COUNTER_fUniqueID);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.fBits", TOWER_CALIB_FINGER_COUNTER_fBits, &b_TOWER_CALIB_FINGER_COUNTER_fBits);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.towerid", TOWER_CALIB_FINGER_COUNTER_towerid, &b_TOWER_CALIB_FINGER_COUNTER_towerid);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.energy", TOWER_CALIB_FINGER_COUNTER_energy, &b_TOWER_CALIB_FINGER_COUNTER_energy);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.time", TOWER_CALIB_FINGER_COUNTER_time, &b_TOWER_CALIB_FINGER_COUNTER_time);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.signal_samples[31]", TOWER_CALIB_FINGER_COUNTER_signal_samples, &b_TOWER_CALIB_FINGER_COUNTER_signal_samples);
   fChain->SetBranchAddress("TOWER_CALIB_FINGER_COUNTER.HBD_channel", TOWER_CALIB_FINGER_COUNTER_HBD_channel, &b_TOWER_CALIB_FINGER_COUNTER_HBD_channel);
   Notify();
}

Bool_t DSTReader551::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DSTReader551::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DSTReader551::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DSTReader551_cxx
