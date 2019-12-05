#ifndef HEPMC_G4_INTERFACE_H
#define HEPMC_G4_INTERFACE_H

#include "G4VPrimaryGenerator.hh"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

/// A base class for primary generation via HepMC object.
/// This class is derived from G4VPrimaryGenerator.

class HepMCG4Interface : public G4VPrimaryGenerator {
protected:
  // Note that the life of HepMC event object must be handled by users.
  // In the default implementation, a current HepMC event will be
  // deleted at GeneratePrimaryVertex() in the next event.
  HepMC3::GenEvent* hepmcEvent; // (care for single event case only)

  // We  have to take care for the position of primaries because
  // primary vertices outside the world voulme give rise to G4Execption.
  // If the default implementation is not adequate, an alternative
  // can be implemented in your own class.
  virtual G4bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;

  // service method for conversion from HepMC::GenEvent to G4Event
  void HepMC2G4(const HepMC3::GenEvent* hepmcevt, G4Event* g4event);

  // Implement this method in his/her own concrete class.
  // An empty event will be created in default.
  virtual HepMC3::GenEvent* GenerateHepMCEvent();

public:
  HepMCG4Interface();
  virtual ~HepMCG4Interface();

  HepMC3::GenEvent* GetHepMCGenEvent() const;

  // The default behavior is that a single HepMC event generated by
  // GenerateHepMCEvent() will be converted to G4Event through HepMC2G4().
  virtual void GeneratePrimaryVertex(G4Event* anEvent);
};

inline HepMC3::GenEvent* HepMCG4Interface::GetHepMCGenEvent() const { return hepmcEvent; }

#endif