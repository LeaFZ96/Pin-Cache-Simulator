#include "L2Cache.h"
#include "LLCache.h"

#ifndef L1Cache_included
#define L1Cache_included

class L2Cache;

class L1Cache {
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
    L2Cache *child;
    vector<vector<ll>> data;
    replacement_policy policy;
    unordered_map<ull, long long>
        last_use;  // Will be useful only if the policy is LRU

    L1Cache(int num_sets, int associativity, int line_size, int type, int id,
            replacement_policy policy);
    ~L1Cache();

    void set_child(L2Cache *child);

    void find_in_cache(ull addr, int category, ull pc);

    void invalidate(ull addr);
};

#endif