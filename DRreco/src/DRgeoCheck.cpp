#include "DRgeoCheck.h"

#include "GridDRcalo.h"

#include "DD4hep/DD4hepUnits.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <cmath>

DECLARE_COMPONENT(DRgeoCheck)


DRgeoCheck::DRgeoCheck(const std::string& aName, ISvcLocator* aSvcLoc) : GaudiAlgorithm(aName, aSvcLoc), m_geoSvc("GeoSvc", aName) {
    declareProperty("GeoSvc", m_geoSvc);

    pSeg = nullptr;
    pParamBase = nullptr;
}

StatusCode DRgeoCheck::initialize() {
    StatusCode sc = GaudiAlgorithm::initialize();

    if (sc.isFailure()) return sc;

    if (!m_geoSvc) {
        error() << "Unable to locate Geometry service." << endmsg;
        return StatusCode::FAILURE;
    }

    pSeg = dynamic_cast<dd4hep::DDSegmentation::GridDRcalo*>(m_geoSvc->lcdd()->readout(m_readoutName).segmentation().segmentation());

    info() << "DRgeoCheck initialized" << endmsg;

    return StatusCode::SUCCESS;
}

StatusCode DRgeoCheck::execute() {
    int numPhi = 0;
    double theta_angle = (1.5 * M_PI) / 180. ; // 1.5 deg in rad
    double phi_angle = (M_PI) / 180. ; // 1.0 deg in rad

    for(unsigned int numEta = 0; numEta < 92; numEta++) {
        pParamBase = pSeg->setParamBase(numEta);
        pParamBase->SetIsRHS(true);
        auto towerPos = pParamBase->GetTowerPos(numPhi);
        auto waferPos = pParamBase->GetSipmLayerPos(numPhi);
        auto towerFaceNormalVector = towerPos - waferPos; // Normal vector of tower face (inward)
        auto towerFaceNormalUnitVector = towerFaceNormalVector.Unit();
        double tan_theta = ( (towerFaceNormalUnitVector.z() * towerFaceNormalUnitVector.z()) + (towerFaceNormalUnitVector.y() * towerFaceNormalUnitVector.y()) ) / (towerFaceNormalUnitVector.x());
        double towerH = pParamBase->GetTowerH();

        auto beamStartingPoint = towerPos + ( (towerH/2.) * towerFaceNormalUnitVector ) + (1800. * CLHEP::millimeter/dd4hep::millimeter * towerFaceNormalUnitVector);
        double z_translate = -1. * pParamBase->GetCurrentInnerR() * std::tan(theta_angle) * std::cos(tan_theta);
        double x_translate = pParamBase->GetCurrentInnerR() * std::tan(theta_angle) * std::sin(tan_theta);
        double y_translate = -1. * (1800. * CLHEP::millimeter/dd4hep::millimeter) * std::tan(phi_angle);

        double beamStartingPoint_x = beamStartingPoint.x() + x_translate; 
        double beamStartingPoint_y = beamStartingPoint.y() + y_translate; 
        double beamStartingPoint_z = beamStartingPoint.z() + z_translate;

        info() << "Tower eta num : " << numEta << endmsg;
        info() << "Tower face normal vector X : " << beamStartingPoint.x() << " Y : " << beamStartingPoint.y() << " Z : " << beamStartingPoint.z() << endmsg; 
        info() << "Beam starting point X : " << beamStartingPoint_x << endmsg;
        info() << "Beam starting point Y : " << beamStartingPoint_y << endmsg;
        info() << "Beam starting point Z : " << beamStartingPoint_z << endmsg;
        info() << "Tower face  Tl1 : " << pParamBase->GetTl1() << " Bl1 : " << pParamBase->GetBl1() << " H1 : " << pParamBase->GetH1() << endmsg;
    }

    return StatusCode::SUCCESS;
}

StatusCode DRgeoCheck::finalize() {
    return GaudiAlgorithm::finalize();
}
