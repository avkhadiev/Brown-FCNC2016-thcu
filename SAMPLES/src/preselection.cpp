/*
 * preselection.cpp
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#include "interface/preselection.h"
#include <cmath>

void preselection::analyze(size_t childid /* this info can be used for printouts */){

    /*
     * This skeleton analyser runs directly on the Delphes output.
     * It can be used to create histograms directly or a skim.
     * If a skim is created, a new input configuration will be written automatically
     * and stored in the output directory together with the ntuples.
     * The skim can contain delphes objects again or can be flat. This is up
     * to the user.
     * Examples for both are given here.
     *
     * The same skeleton can be used to read the skim. Please refer to the comments
     * marked with "==SKIM=="
     *
     * These parts are commented, since the code is supposed to work as an example without
     * modifications on Delphes output directly.
     */



    /*
     * Define the branches that are to be considered for the analysis
     * This branch handler (notice the "d")
     * is used to run directly in Delphes output.
     * For skimmed ntuples, see below
     */
    d_ana::dBranchHandler<Electron>  elecs(tree(),"Electron");
    d_ana::dBranchHandler<Muon>      muons(tree(),"MuonTight");
    d_ana::dBranchHandler<Jet>       jets( tree(),"JetPUPPI");
    

    /* ==SKIM==
     *
     * If a skim of the Delphes outout was created in a way indicated
     * further below, use the tBranchHandler (please notive the "t")
     * to access vectors of objects...
     *
     */
    // d_ana::tBranchHandler<std::vector<Electron> > electrons(tree(),"Electrons");

    /*==SKIM==
     *
     * Or an object directly
     *
     */
    //d_ana::tBranchHandler<MissingET> met(tree(),"MET");



    /*
     * Always use this function to add a new histogram (can also be 2D)!
     * Histograms created this way are automatically added to the output file
     */
    //TH1* histo=addPlot(new TH1D("histoname1","histotitle1",100,0,100),"p_{T} [GeV]","N_{e}");


    /*
     * Helper variables 
     */
    Int_t    nElecs=0;
    Int_t    nMuons=0;
    Int_t     nTaus=0;
    Int_t     nJets=0;
    Double_t elecPt=0;
    Double_t muonPt=0;
    Double_t  tauPt=0;
    Double_t elecEta=0;
    Double_t muonEta=0;
    Double_t tauEta=0;


    size_t nevents=tree()->entries();
    if(isTestMode())
        nevents/=100;
    
    Int_t ee4jcounter   = 0;
    Int_t eegte5jcounter = 0;
    Int_t em4jcounter = 0;
    Int_t emgte5jcounter = 0;
    Int_t mm4jcounter = 0;
    Int_t mmgte5jcounter = 0;
    Int_t lllcounter = 0;
    Int_t lltcounter = 0;

    for(size_t eventno=0;eventno<nevents;eventno++){
        /*
         * The following two lines report the status and set the event link
         * Do not remove!
         */
        reportStatus(eventno,nevents);
        tree()->setEntry(eventno);

        nElecs=0;
        nMuons=0;
        nTaus =0; 
        nJets =0;
        Double_t nBJets=0; //needs to be reset each loop

        /*
         * Perform pre-selection
         * Must have lepton Pts > 24
         */

        for(size_t i=0;i<elecs.size();i++){
            elecPt=elecs.at(i)->PT;
            elecEta=elecs.at(i)->Eta;
            if(elecPt < 25 || std::abs(elecEta) > 2.4) {
                continue;
            }
            nElecs++;
        }

        for(size_t i=0;i<muons.size();i++){
            muonPt=muons.at(i)->PT;
            muonEta=muons.at(i)->Eta;
            if(muonPt < 25 || std::abs(muonEta) > 2.5) {
                continue;
            }
            nMuons++;
        }

        for(size_t i=0;i<jets.size();i++){
            bool isTau = (jets.at(i)->TauTag>>2) & 0x1;
            bool isBJet = (jets.at(i)->BTag>>2) & 0x1;
            tauPt=jets.at(i)->PT;
            tauEta=jets.at(i)->Eta;
            if(!isTau) {
                if (tauPt > 25 && std::abs(tauEta) < 2.5){
                   nBJets = ((isBJet) ? nBJets + 1 : nBJets);
                   nJets++;
                   continue;
                } 
                continue;
            }
            if (tauPt < 24 || tauEta > 2.5) continue; 
            nTaus++;
        } 
        

        // PRESEL: Must have at least 1 lepton and 2 B jets
        if(nElecs + nMuons + nTaus < 1) continue;
        if (nBJets < 2) continue;

        // Output channel counts
        if (nElecs == 2 && nJets == 4) ee4jcounter++;
        if (nElecs == 2 && nJets >= 5) eegte5jcounter++;
        if (nElecs == 1 && nMuons == 1 && nJets == 4) em4jcounter++;
        if (nElecs == 1 && nMuons == 1 && nJets >= 5) emgte5jcounter++;
        if (nMuons == 2 && nJets == 4) mm4jcounter++;
        if (nMuons == 2 && nJets >= 5) mmgte5jcounter++;
        if ((nMuons + nElecs + nTaus) == 3) lllcounter++;
        if ((nMuons + nElecs + nTaus) == 2 && nTaus == 1) lltcounter++; 

    }                                        
    TH1 *h = addPlot(new TH1I("count_histo", "count consistency check", 8, 0, 8), "Category", "Events");

    h->GetXaxis()->SetBinLabel(1, "ee4j"        );          
    h->GetXaxis()->SetBinLabel(2, "ee#geq5j"    );
    h->GetXaxis()->SetBinLabel(3, "#mu#mu4j"    );
    h->GetXaxis()->SetBinLabel(4, "#mu#mu#geq5j");
    h->GetXaxis()->SetBinLabel(5, "e#mu4j"      );
    h->GetXaxis()->SetBinLabel(6, "e#mu#geq5j"  );
    h->GetXaxis()->SetBinLabel(7, "3l"          );
    h->GetXaxis()->SetBinLabel(8, "2l1#tau"     );

    h->Fill("ee4j"        ,  ee4jcounter     );              
    h->Fill("ee#geq5j"    ,  eegte5jcounter );       
    h->Fill("#mu#mu4j"    ,  mm4jcounter   );      
    h->Fill("#mu#mu#geq5j",  mmgte5jcounter);     
    h->Fill("e#mu4j"      ,  em4jcounter    );       
    h->Fill("e#mu#geq5j"  ,  emgte5jcounter );      
    h->Fill("3l"          ,  lllcounter        );  
    h->Fill("2l1#tau"     ,  lltcounter   );

    std::cout << "Number of ee4j: " << ee4jcounter << std::endl;
    std::cout << "Number of ee>=5j: " << eegte5jcounter << std::endl;
    std::cout << "Number of emu4j: " << em4jcounter << std::endl;
    std::cout << "Number of emu>=5j: " << emgte5jcounter << std::endl;
    std::cout << "Number of mumu4j: " << mm4jcounter << std::endl;
    std::cout << "Number of mumu>=5j: " << mmgte5jcounter << std::endl; 
    std::cout << "Number of 3l: " << lllcounter << std::endl; 
    std::cout << "Number of llt: " << lltcounter << std::endl; 
    /*
     * Must be called in the end, takes care of thread-safe writeout and
     * call-back to the parent process
     */
    processEndFunction();
}
