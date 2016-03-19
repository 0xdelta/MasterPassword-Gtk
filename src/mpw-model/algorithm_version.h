//
// Created by lukas on 19.03.16.
//

#ifndef MPW_GTK_ALGORITHM_VERSION_H
#define MPW_GTK_ALGORITHM_VERSION_H

#include <string>
#include "mpw-algorithm.h"

class AlgorithmVersion {
private:
    int id;
    std::string displayName;
    MPAlgorithmVersion mpAlgorithmVersion;

public:
    AlgorithmVersion(int id, const std::string &displayName, MPAlgorithmVersion mpAlgorithmVersion);

    int getId() const;
    const std::string &getDisplayName() const;
    MPAlgorithmVersion getMpAlgorithmVersion() const;
};

static AlgorithmVersion algorithmVersions[] = {
        {1, "V1", MPAlgorithmVersion1},
        {2, "V2", MPAlgorithmVersion2},
        {3, "V3", MPAlgorithmVersion3},
};
static AlgorithmVersion defaultAlgorithmVersion = algorithmVersions[2]; // V3


#endif //MPW_GTK_ALGORITHM_VERSION_H
