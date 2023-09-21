
#include "../Detector/DRsegmentation/include/GridDRcalo.h"

// DD4Hep
#include "DD4hep/Detector.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <string>


int main() {
    dd4hep::Detector* m_dd4hepgeo = nullptr;
    dd4hep::DDSegmentation::GridDRcalo* pSeg = nullptr;
    dd4hep::DDSegmentation::DRparamBase* pParamBase = nullptr;
    // dd4hep::DDSegmentation::DRparamBarrel* pParamBarrel = nullptr;
    // dd4hep::DDSegmentation::DRparamEndcap* pParamEndcap = nullptr;

    std::string filename = "/u/user/swkim/KEY4HEP/dual-readout/install/share/compact/DRcalo.xml";
    m_dd4hepgeo = &(dd4hep::Detector::getInstance());
    m_dd4hepgeo->fromCompact(filename);
    m_dd4hepgeo->volumeManager();
    m_dd4hepgeo->apply("DD4hepVolumeManager", 0, 0);

    std::string m_readoutName = "DRcaloSiPMreadout";
    pSeg = dynamic_cast<dd4hep::DDSegmentation::GridDRcalo*>(m_dd4hepgeo->readout(m_readoutName).segmentation().segmentation());

    int numPhi = 0;
    double theta_angle = (1.5 * M_PI) / 180. ; // 1.5 deg in rad
    double phi_angle = (M_PI) / 180. ; // 1.0 deg in rad
    // pParamBarrel = pSeg->paramBarrel();
    // pParamEndcap = pSeg->paramEndcap();

    for (int noEta = 0; noEta < 92; noEta++) {
        std::cout << "Eta number : " << noEta << std::endl;
        pParamBase = pSeg->setParamBase(noEta);
        pParamBase->SetIsRHS(true); 
        auto towerPos = pParamBase->GetTowerPos(numPhi);
        auto waferPos = pParamBase->GetSipmLayerPos(numPhi);
        auto towerFaceNormalVector = towerPos - waferPos; // Normal vector of tower face (inward)
        auto towerFaceNormalUnitVector = towerFaceNormalVector.Unit();
        double tan_theta = ( std::sqrt( (towerFaceNormalUnitVector.z() * towerFaceNormalUnitVector.z()) + (towerFaceNormalUnitVector.y() * towerFaceNormalUnitVector.y()) ) ) / std::abs(towerFaceNormalUnitVector.x());
        double towerH = pParamBase->GetTowerH();
        double beamTheta = ( (M_PI/2.) - pParamBase->GetThetaOfCenter() - theta_angle);
        double beamPhi   = phi_angle;
        std::cout << "tower center position  X : " << towerPos.x() << " Y : " << towerPos.y() << " Z : " << towerPos.z() << std::endl;
        // std::cout << "wafer pos  X : " << waferPos.x() << " Y : " << waferPos.y() << " Z : " << waferPos.z() << std::endl;
        // std::cout << "tower H : " << towerH << std::endl;
        std::cout << "tower inner R : " << pParamBase->GetCurrentInnerR() << std::endl;
        std::cout << "tower theta : " << pParamBase->GetThetaOfCenter() << std::endl;
        double x_beamSpotSize = 0.5 * std::sin(pParamBase->GetThetaOfCenter());
        double z_beamSpotSize = 0.5 * std::cos(pParamBase->GetThetaOfCenter());
        double y_beamSpotSize = 0.5;
        if ( pParamBase->GetTl1() <= 1. ) y_beamSpotSize = pParamBase->GetTl1() * 0.4;
        if ( pParamBase->GetBl1() <= 1. ) y_beamSpotSize = pParamBase->GetBl1() * 0.4;
        // std::cout << "tower innerX : " << pParamBase->GetInnerX() << std::endl;
        std::cout << "tower dTheta : " << pParamBase->GetDeltaTheta() << std::endl;

        auto beamStartingPoint = towerPos + ( (towerH/2.) * towerFaceNormalUnitVector ) + (180. * towerFaceNormalUnitVector);
        double z_translate = -1. * pParamBase->GetCurrentInnerR() * std::tan(theta_angle) * std::cos(tan_theta);
        double x_translate = pParamBase->GetCurrentInnerR() * std::tan(theta_angle) * std::sin(tan_theta);
        double y_translate = -1. * (180.) * std::tan(phi_angle);
        // std::cout << "beam pos  X : " << beamStartingPoint.x() << " Y : " << beamStartingPoint.y() << " Z : " << beamStartingPoint.z() << std::endl;
        // std::cout << "beam Theta : " << beamTheta << std::endl;
        // std::cout << "beam Phi   : " << beamPhi << std::endl;
        // std::cout << "trans X : " << x_translate << " Y : " << y_translate << " Z : " << z_translate << std::endl;
        double beamStartingPoint_x = beamStartingPoint.x() + x_translate; 
        double beamStartingPoint_y = beamStartingPoint.y() + y_translate; 
        double beamStartingPoint_z = beamStartingPoint.z() + z_translate;

        std::cout << "Tower face normal vector X : " << towerFaceNormalUnitVector.x() << " Y : " << towerFaceNormalUnitVector.y() << " Z : " << towerFaceNormalUnitVector.z() << std::endl; 
        std::cout << "Tower face  Tl1 : " << pParamBase->GetTl1() << " Bl1 : " << pParamBase->GetBl1() << " H1 : " << pParamBase->GetH1() << std::endl;
        std::cout << "Beam starting point X : " << beamStartingPoint_x << " Y : " << beamStartingPoint_y << " Z : " << beamStartingPoint_z << std::endl;
        std::cout << "beam Theta : " << beamTheta << std::endl;
        std::cout << "beam Phi   : " << beamPhi << std::endl;
        std::cout << "Beam spot X : " << beamStartingPoint_x - x_beamSpotSize << " ~ " << beamStartingPoint_x + x_beamSpotSize << std::endl;
        std::cout << "Beam spot Y : " << beamStartingPoint_y - y_beamSpotSize << " ~ " << beamStartingPoint_y + y_beamSpotSize << std::endl;
        std::cout << "Beam spot Z : " << beamStartingPoint_z - z_beamSpotSize << " ~ " << beamStartingPoint_z + z_beamSpotSize << std::endl << std::endl;
    }
}