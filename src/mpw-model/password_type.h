//
// Created by lukas on 19.03.16.
//

#ifndef MPW_GTK_PASSWORD_TYPE_H
#define MPW_GTK_PASSWORD_TYPE_H

#include "mpw-types.h"
#include <string>

class PasswordType {
private:
    std::string name;
    MPSiteType mpSiteType;

public:
    PasswordType(const std::string &name, MPSiteType mpSiteType);

    const std::string &getName() const;
    MPSiteType getMpSiteType() const;
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

#endif //MPW_GTK_PASSWORD_TYPE_H
