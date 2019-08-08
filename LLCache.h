#ifndef LLCache_included
#define LLCache_included

class L1Cache;
class L2Cache;

class LLCache {
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
    
    L2Cache *parent;

    vector<vector<ll> > data;
    replacement_policy policy;
    unordered_map<ull, long long>
        last_use;  // Will be useful only if the policy is LRU

    LLCache(int num_sets, int associativity, int line_size, int type, int id,
            replacement_policy policy);
    ~LLCache();

    void find_in_cache(ull addr, int category, ull pc);

    void set_parent(L2Cache *parent);

};

#endif