//
// Created by lukas on 19.03.16.
//

#ifndef MPW_GTK_PASSWORD_TYPE_H
#define MPW_GTK_PASSWORD_TYPE_H

#include "mpw-types.h"
#include <string>

class PasswordType {
private:
    int id;
    std::string name;
    MPSiteType mpSiteType;

public:
    PasswordType(int id, const std::string &name, MPSiteType mpSiteType);

    int getId() const;
    const std::string &getName() const;
    MPSiteType getMpSiteType() const;
};

static PasswordType passwordTypes[] = {
        {0x0, "Maximum", MPSiteTypeGeneratedMaximum},
        {0x1, "Long",    MPSiteTypeGeneratedLong},
        {0x2, "Medium",  MPSiteTypeGeneratedMedium},
        {0x3, "Basic",   MPSiteTypeGeneratedBasic},
        {0x4, "Short",   MPSiteTypeGeneratedShort},
        {0x5, "PIN",     MPSiteTypeGeneratedPIN},
        {0xE, "Name",    MPSiteTypeGeneratedName},
        {0xF, "Phrase",  MPSiteTypeGeneratedPhrase}
};
static PasswordType defaultPasswordType = passwordTypes[1]; // Long

#endif //MPW_GTK_PASSWORD_TYPE_H
