//
// Created by lukas on 13.03.16.
//

#ifndef MPW_GTK_MPW_SERVICE_H
#define MPW_GTK_MPW_SERVICE_H

#include <string>
#include <bits/stl_function.h>
#include <mpw-algorithm.h>

/**
 * This class encapsulates the information, that are needed, to
 * generate a password for a site.
 */
class Service {
private:
    std::string name;
    MPSiteType type;
    MPAlgorithmVersion algorithmVersion;
    int counter;

public:
    /**
     * Creates a new instance with default values.
     * The defaults can be found in password_type.h
     */
    Service(std::string name);
    /**
     * Creates a new instance with the given values.
     */
    Service(std::string name, MPSiteType siteType, MPAlgorithmVersion algorithmVersion, int counter);

    /**
     * Checks if the names equal.
     */
    bool operator==(const Service &other);

    /* Getters & Setters */

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

    void setType(MPSiteType type) {
        Service::type = type;
    }

    void setAlgorithmVersion(MPAlgorithmVersion algorithmVersion) {
        Service::algorithmVersion = algorithmVersion;
    }

    void setCounter(int counter) {
        Service::counter = counter;
    }
};


#endif //MPW_GTK_MPW_SERVICE_H
