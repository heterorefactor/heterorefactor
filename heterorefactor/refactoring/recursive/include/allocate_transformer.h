#pragma once

#include "access_transformer.h"
#include "type_transformer.h"

class AllocateTransformer {
public:
    AllocateTransformer(SgProject* project);
    void collect_types(void);
    void set_type_transformer(TypeTransformer *type_trans);
    void set_access_transformer(AccessTransformer *access_trans);
    void transform(void);

protected:
    SgProject *m_project;
    TypeTransformer *m_type_trans;
    AccessTransformer *m_access_trans;

    SgType *malloc_type_from_call(SgNode *node);
    SgType *free_type_from_call(SgNode *node);

    std::map<SgNode *, SgType *> m_original_type;
};
