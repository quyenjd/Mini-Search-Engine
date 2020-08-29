#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Base.h"
#include "Query.h"

namespace Operations
{
    void opAND (queryData& q, const baseData& bd);
    void opOR (queryData& q, const baseData& bd);
    void opInclude (queryData& q, const baseData& bd);
    void opExclude (queryData& q, const baseData& bd);
    void opTitle (queryData& q, const baseData& bd);
    void opFileType (queryData& q, const baseData& bd);
    void opPrize (queryData& q, const baseData& bd);
    void opHashtag (queryData& q, const baseData& bd);
    void opExactMatch (queryData& q, const baseData& bd);
    void opWildcard (queryData& q, const baseData& bd);
    void opRange (queryData& q, const baseData& bd);
    void opSynonym (queryData& q, const baseData& bd);
}

#endif // SE_OPERATIONS_H