// Make this appear first!
#include "G4Timer.hh"

#include "WLS_TestRunAction.hh"
#include "WLS_TestAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestRunAction::WLS_TestRunAction()
 : G4UserRunAction(),
   timer(0)
{
    if (IsMaster()) {
        timer = new G4Timer();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

WLS_TestRunAction::~WLS_TestRunAction()
{
  delete timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4int runID = aRun->GetRunID();
    
    if (IsMaster()){
        G4cout << "### Run " << runID << " start." << G4endl;
        timer->Start();
    }
    
    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->OpenFile("Test");
    
    // index 0
    analysisManager->CreateH1("Photons", "Bins: 1=Primaries, 2=OpWLS Photons, 6=fiber +x end, 7=fiber -x end", 10, 0., 10.);
    
    analysisManager->CreateH1("wlsSpectrum", "Wavelength of photons produced in green fibers", 100, 300., 800.);
    
    analysisManager->CreateH1("wlsEnergy", "Energy of photons produced in green fibers", 100, 1., 4.);
   

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void WLS_TestRunAction::EndOfRunAction(const G4Run* aRun)
{

    //WLS_TestAnalysisManager::GetInstance()->EndOfRun();
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    // save histograms
    //
    analysisManager->Write();
    analysisManager->CloseFile();
    
    // complete cleanup
    //
    delete G4AnalysisManager::Instance();

    
    if (IsMaster()){
        timer->Stop();
        G4cout << "*****G4 RUN TIMER*****" << G4endl;
        G4cout << *timer << G4endl;
        
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
