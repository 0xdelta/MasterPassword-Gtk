//
// Created by lukas on 22.03.16.
//

#ifndef MPW_GTK_DATA_H
#define MPW_GTK_DATA_H

#include <string>
#include "mpw-types.h"
#include "mpw-algorithm.h"

struct PasswordType {
    std::string name;
    MPSiteType  mpSiteType;
};

static PasswordType passwordTypes[] = {
        {"Maximum", MPSiteTypeGeneratedMaximum},
        {"Long",    MPSiteTypeGeneratedLong},
        {"Medium",  MPSiteTypeGeneratedMedium},
        {"Basic",   MPSiteTypeGeneratedBasic},
        {"Short",   MPSiteTypeGeneratedShort},
        {"PIN",     MPSiteTypeGeneratedPIN},
        {"Name",    MPSiteTypeGeneratedName},
        {"Phrase",  MPSiteTypeGeneratedPhrase}
};
static PasswordType defaultPasswordType = passwordTypes[1]; // Long

#endif //MPW_GTK_DATA_H
