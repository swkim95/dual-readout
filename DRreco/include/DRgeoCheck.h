#ifndef DRgeoCheck_h
#define DRgeoCheck_h 1

#include "k4FWCore/DataHandle.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"
// #include "GaudiKernel/IAlgManager.h"

#include "GridDRcalo.h"
#include "k4Interface/IGeoSvc.h"

class IGeoSvc;

class DRgeoCheck : public GaudiAlgorithm {
public:
    DRgeoCheck(const std::string& name, ISvcLocator* svcLoc);
    virtual ~DRgeoCheck() {};

    StatusCode initialize();
    StatusCode execute();
    StatusCode finalize();

private:

    ServiceHandle<IGeoSvc> m_geoSvc;
    dd4hep::DDSegmentation::GridDRcalo* pSeg;
    dd4hep::DDSegmentation::DRparamBase* pParamBase;

    Gaudi::Property<std::string> m_calibPath{this, "calibPath", "share/calib.csv", "relative path to calibration csv file"};
    Gaudi::Property<std::string> m_readoutName{this, "readoutName", "DRcaloSiPMreadout", "readout name of DRcalo"};

};

#endif
