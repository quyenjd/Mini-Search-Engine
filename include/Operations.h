#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"

namespace Operations
{
    void opFilterByPos (queryData* q); // quyen
    
    void opAND (queryData* q, baseData* bd); // ?
    void opOR (queryData* q, baseData* bd); // ?
    void opInclude (queryData* q, baseData* bd); // an
    void opExclude (queryData* q, baseData* bd); // an
    void opTitle (queryData* q, baseData* bd); // quyen
    void opFileType (queryData* q, baseData* bd); // quyen
    void opWildcard (queryData* q, baseData* bd); // quyen
    void opRange (queryData* q, baseData* bd); // an
    void opSynonym (queryData* q, baseData* bd); // an
}

#endif // SE_OPERATIONS_H