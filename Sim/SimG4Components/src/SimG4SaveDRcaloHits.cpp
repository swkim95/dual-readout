#include "SimG4SaveDRcaloHits.h"

#include "GridDRcalo.h"

// Geant4
#include "G4Event.hh"

// DD4hep
#include "DDG4/Geant4Hits.h"

#include <stdexcept>
#include <functional>

SimG4SaveDRcaloHits::SimG4SaveDRcaloHits(const std::string filename) {
  m_geoSvc = GeoSvc::GetInstance();
  m_readoutNames = {"DRcaloSiPMreadout"};
  mFilename = filename;

  initialize();

  if (m_geoSvc==0) throw std::runtime_error("Attempt to save hits while GeoSvc is not initialized!");
}

SimG4SaveDRcaloHits::~SimG4SaveDRcaloHits() {
  finalize();
}

void SimG4SaveDRcaloHits::initialize() {
  // DD4hep readouts
  auto lcdd = m_geoSvc->lcdd();
  auto allReadouts = lcdd->readouts();
  for (auto& readoutName : m_readoutNames) {
    if (allReadouts.find(readoutName) == allReadouts.end()) {
      throw std::runtime_error("Readout " + readoutName + " not found! Please check tool configuration.");
    } else {
      std::cout << "Hits will be saved to EDM from the collection " << readoutName << std::endl;
    }
  }

  // EDM4hep/PODIO event store
  pStore = std::make_unique<podio::EventStore>();
  pWriter = std::make_unique<podio::ROOTWriter>(mFilename,pStore.get());

  *mSimCaloHits = pStore->create<edm4hep::SimCalorimeterHitCollection>("SimCalorimeterHits");
  pWriter->registerForWrite("SimCalorimeterHits");

  *mDRsimCaloHits = pStore->create<edm4hep::DRSimCalorimeterHitCollection>("DRSimCalorimeterHits");
  pWriter->registerForWrite("DRSimCalorimeterHits");

  return;
}

void SimG4SaveDRcaloHits::saveOutput(const G4Event* aEvent) const {
  G4HCofThisEvent* collections = aEvent->GetHCofThisEvent();
  G4VHitsCollection* collect;
  ddDRcalo::DRcaloSiPMHit* hit;

  std::map<int, DRsimInterface::DRsimTowerData> towerMap;

  if (collections != nullptr) {
    for (int iter_coll = 0; iter_coll < collections->GetNumberOfCollections(); iter_coll++) {
      collect = collections->GetHC(iter_coll);

      if (std::find(m_readoutNames.begin(), m_readoutNames.end(), collect->GetName()) != m_readoutNames.end()) {
        size_t n_hit = collect->GetSize();

        dd4hep::DDSegmentation::GridDRcalo* segmentation = dynamic_cast<dd4hep::DDSegmentation::GridDRcalo*>(m_geoSvc->lcdd()->readout(collect->GetName()).segmentation().segmentation());

        for (size_t iter_hit = 0; iter_hit < n_hit; iter_hit++) {
          hit = dynamic_cast<ddDRcalo::DRcaloSiPMHit*>(collect->GetHit(iter_hit));

          auto caloHit = mSimCaloHits->create();
          caloHit.setCellID( static_cast<unsigned long long>(hit->GetSiPMnum()) );

          auto globalPos = segmentation->position( hit->GetSiPMnum() );
          caloHit.setPosition( {globalPos.x(),globalPos.y(),globalPos.z()} );

          auto DRcaloHit = mDRsimCaloHits->create();
          DRcaloHit.setCount( hit->GetPhotonCount() );

          checkMetadata(hit);

          addStruct( hit->GetTimeStruct(), std::bind( &edm4hep::DRSimCalorimeterHit::addToTimeStruct, &DRcaloHit, std::placeholders::_1 ) );
          addStruct( hit->GetWavlenSpectrum(), std::bind( &edm4hep::DRSimCalorimeterHit::addToWavlenSpectrum, &DRcaloHit, std::placeholders::_1 ) );

          DRcaloHit.setEdm4hepSimCalorimeterHit( caloHit );
        }
      }
    }
  }

  return;
}

void SimG4SaveDRcaloHits::finalize() {
  pWriter->finish();
}

void SimG4SaveDRcaloHits::checkMetadata(const ddDRcalo::DRcaloSiPMHit* hit) const {
  auto& colMD = pStore->getCollectionMetaData( mDRsimCaloHits->getID() );
  if ( !colMD.getStringVal("Producer").empty() ) return;

  auto& timeStruct = hit->GetTimeStruct();
  std::vector<float> timeBins;
  std::for_each( timeStruct.begin(), timeStruct.end(), [&](const std::pair< std::pair<float,float>, int >& element) { timeBins.push_back(element.first.first); } );

  auto& wavlenSpectrum = hit->GetWavlenSpectrum();
  std::vector<float> waveBins;
  std::for_each( wavlenSpectrum.begin(), wavlenSpectrum.end(), [&](const std::pair< std::pair<float,float>, int >& element) { waveBins.push_back(element.first.first); } );

  colMD.setValues( "timeBins", timeBins );
  colMD.setValues( "waveBins", waveBins );
  colMD.setValue( "Producer", "SimG4SaveDRcaloHits" );
}

void SimG4SaveDRcaloHits::addStruct( const std::map< std::pair<float,float>, int >& structData, std::function<void(int)> addTo ) const {
  std::for_each( structData.begin(), structData.end(), [&](const std::pair< std::pair<float,float>, int >& element) { addTo(element.second); } );
}
