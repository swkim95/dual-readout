#ifndef SimG4DRcaloActionInitialization_h
#define SimG4DRcaloActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "detectorSegmentations/GridDRcalo_k4geo.h"

namespace drc {
class SimG4DRcaloActionInitialization : public G4VUserActionInitialization {
public:
  SimG4DRcaloActionInitialization();
  virtual ~SimG4DRcaloActionInitialization();

  virtual void Build() const final;

  void setSegmentation(dd4hep::DDSegmentation::GridDRcalo_k4geo* seg) { pSeg = seg; }
  void setThreshold(const double thres) { m_thres = thres; }
  void setBirksConstant(const std::string scintName, const double birks);

private:
  dd4hep::DDSegmentation::GridDRcalo_k4geo* pSeg;
  std::string m_scintName;
  double m_birks;
  double m_thres;
};
}

#endif
