//
// Created by lukas on 19.03.16.
//

#include "algorithm_version.h"

AlgorithmVersion::AlgorithmVersion(const std::string &displayName, MPAlgorithmVersion mpAlgorithmVersion) :
        displayName(displayName), mpAlgorithmVersion(mpAlgorithmVersion) { }

const std::string &AlgorithmVersion::getDisplayName() const {
    return displayName;
}

MPAlgorithmVersion AlgorithmVersion::getMpAlgorithmVersion() const {
    return mpAlgorithmVersion;
}