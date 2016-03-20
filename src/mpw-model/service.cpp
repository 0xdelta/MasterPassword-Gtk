//
// Created by lukas on 13.03.16.
//

#include "service.h"
#include "password_type.h"
#include "algorithm_version.h"

Service::Service(std::string _name) :
        name(_name), type(defaultPasswordType.getMpSiteType()),
        algorithmVersion(defaultAlgorithmVersion.getMpAlgorithmVersion()), counter(1) { }

Service::Service(std::string _name, MPSiteType _siteType, MPAlgorithmVersion _algorithmVersion, int _counter) :
        name(_name), type(_siteType), algorithmVersion(_algorithmVersion),
        counter(_counter) { }

bool Service::operator==(const Service &other) {
    return name == other.name;
}
