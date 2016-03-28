//
// Created by lukas on 22.03.16.
//

#ifndef MPW_GTK_ALGORITHM_VERSION_H
#define MPW_GTK_ALGORITHM_VERSION_H

#include <string>
#include "mpw-algorithm.h"

/**
 * This header holds everything that has to do with
 * the different algorithm versions.
 */

struct AlgorithmVersion {
    std::string displayName;
    MPAlgorithmVersion mpAlgorithmVersion;
};

static AlgorithmVersion algorithmVersions[] = {
        {"V1", MPAlgorithmVersion1},
        {"V2", MPAlgorithmVersion2},
        {"V3", MPAlgorithmVersion3},
};
static AlgorithmVersion defaultAlgorithmVersion = algorithmVersions[2]; // V3

#endif //MPW_GTK_ALGORITHM_VERSION_H
