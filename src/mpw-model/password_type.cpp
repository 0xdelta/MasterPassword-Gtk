//
// Created by lukas on 19.03.16.
//

#include "password_type.h"

PasswordType::PasswordType(const std::string &name, MPSiteType mpSiteType) :
        name(name), mpSiteType(mpSiteType) { }

const std::string &PasswordType::getName() const {
    return name;
}

MPSiteType PasswordType::getMpSiteType() const {
    return mpSiteType;
}