//
// Created by lukas on 13.03.16.
//

#ifndef MPW_GTK_MPW_SERVICE_H
#define MPW_GTK_MPW_SERVICE_H

#include <string>
#include <bits/stl_function.h>
#include <mpw-algorithm.h>

class mpw_service {
private:
    std::string name;
    MPSiteType type;
    MPAlgorithmVersion algorithmVersion;
    int counter;

public:
    //mpw_service(mpw_service &service);
    mpw_service(std::string name);
    mpw_service(std::string name, MPSiteType siteType, MPAlgorithmVersion algorithmVersion, int counter);

    bool operator==(const mpw_service &other);

    const std::string &getName() const {
        return name;
    }

    MPSiteType getType() const {
        return type;
    }

    MPAlgorithmVersion getAlgorithmVersion() const {
        return algorithmVersion;
    }

    int getCounter() const {
        return counter;
    }
};


#endif //MPW_GTK_MPW_SERVICE_H
