//
// Created by lukas on 13.03.16.
//

#include "mpw_service.h"

mpw_service::mpw_service(std::string _name) :
        name(_name),
        type(MPSiteTypeGeneratedLong),
        algorithmVersion(MPAlgorithmVersionCurrent),
        counter(1) { }

mpw_service::mpw_service(std::string _name, MPSiteType _siteType, MPAlgorithmVersion _algorithmVersion, int _counter) :
        name(_name),
        type(_siteType),
        algorithmVersion(_algorithmVersion),
        counter(_counter) { }
