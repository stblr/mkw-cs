#pragma once

#include "ObjectBase.hxx"

namespace Object {

class ObjectKcl : public ObjectBase {
    virtual void vf_b4();
    virtual void vf_b8();
    virtual void vf_bc();
};

} // namespace Object
