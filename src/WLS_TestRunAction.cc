// Make this appear first!
#include "G4Timer.hh"

#include "WLS_TestRunAction.hh"
#include "WLS_TestAnalysis.hh"
#include "WLS_TestRunActionMessenger.hh"

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
    
    fMessenger = new WLS_TestRunActionMessenger(this);
    fileName = "";
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
    
    if(fileName==""){fileName = "output/WLS_Test";}
    analysisManager->OpenFile(fileName);
    
    // index 0
    analysisManager->CreateH1("Photons", "Bins: 1=Primaries, 2=OpTPB Photons, 3=TPB Photons inside, 4=TPB Photons Outside, 5=OpWLS Photons, 6=fiber +z end, 7=fiber -z end", 10, 0., 10.);
    
    // index 1
    analysisManager->CreateH1("wlsSpectrum", "Wavelength of photons produced in green fibers", 100, 300., 800.);
    
    // index 2
    analysisManager->CreateH1("wlsEnergy", "Energy of photons produced in green fibers", 100, 1., 4.);
    
    // index 3
    analysisManager->CreateH1("cosThetaPosz", "Cosine of the exit angle for +z end of fiber", 100, -1.001, 1.001);
    
    // index 4
    analysisManager->CreateH1("cosThetaNegz", "Cosine of the exit angle for -z end of fiber", 100, -1.001, 1.001);
   
    // index 5
    analysisManager->CreateH1("PhotonFate", "Bins: 0=Undefined, 1=X-Plate, 2=Y-Plate, 3=Z-Plate, 4=Cell Absorption, 5=TPB Absorption, 8=Trapped, 9=Not Trapped", 10, 0., 10.);

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
