#include "LLCache.h"

#ifndef L2Cache_included
#define L2Cache_included

class L1Cache;
class LLCache;

class L2Cache {
   public:
    uint64_t setMask;
    uint64_t tagMask;
    uint64_t lineMask;
    uint32_t setShift;

    int associativity;
    int num_sets;
    int type;
    int id;
    ull clock;

    L1Cache *parentData;
    L1Cache *parentInstruction;
    LLCache *child;

    vector<vector<ll>> data;
    replacement_policy policy;
    unordered_map<ull, long long>
        last_use;  // Will be useful only if the policy is LRU

    L2Cache(int num_sets, int associativity, int line_size, int type, int id,
            replacement_policy policy);
    ~L2Cache();

    void set_child(LLCache *child);

    void set_parent(L1Cache *parentData, L1Cache *parentInstruction);

    void find_in_cache(ull addr, int category, ull pc);

    void invalidate(ull addr);
};

#endif