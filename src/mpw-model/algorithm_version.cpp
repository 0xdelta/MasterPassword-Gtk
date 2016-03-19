//
// Created by lukas on 19.03.16.
//

#include "algorithm_version.h"

AlgorithmVersion::AlgorithmVersion(int id, const std::string &displayName, MPAlgorithmVersion mpAlgorithmVersion) : id(id), displayName(displayName), mpAlgorithmVersion(mpAlgorithmVersion) { }

int AlgorithmVersion::getId() const {
    return id;
}

const std::string &AlgorithmVersion::getDisplayName() const {
    return displayName;
}

MPAlgorithmVersion AlgorithmVersion::getMpAlgorithmVersion() const {
    return mpAlgorithmVersion;
}