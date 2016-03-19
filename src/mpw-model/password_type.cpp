//
// Created by lukas on 19.03.16.
//

#include "password_type.h"

PasswordType::PasswordType(int id, const std::string &name, MPSiteType mpSiteType) :
        id(id), name(name), mpSiteType(mpSiteType) { }

int PasswordType::getId() const {
    return id;
}

const std::string &PasswordType::getName() const {
    return name;
}

MPSiteType PasswordType::getMpSiteType() const {
    return mpSiteType;
}