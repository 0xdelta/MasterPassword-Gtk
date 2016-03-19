//
// Created by lukas on 19.03.16.
//

#ifndef MPW_GTK_ALGORITHM_VERSION_H
#define MPW_GTK_ALGORITHM_VERSION_H

#include <string>
#include "mpw-algorithm.h"

class AlgorithmVersion {
private:
    std::string displayName;
    MPAlgorithmVersion mpAlgorithmVersion;

public:
    AlgorithmVersion(const std::string &displayName, MPAlgorithmVersion mpAlgorithmVersion);

    const std::string &getDisplayName() const;
    MPAlgorithmVersion getMpAlgorithmVersion() const;
};

static AlgorithmVersion algorithmVersions[] = {
        {"V1", MPAlgorithmVersion1},
        {"V2", MPAlgorithmVersion2},
        {"V3", MPAlgorithmVersion3},
};
static AlgorithmVersion defaultAlgorithmVersion = algorithmVersions[2]; // V3


#endif //MPW_GTK_ALGORITHM_VERSION_H
