#include "constants.h"
#include "L1Cache.h"
#include "L2Cache.h"

L2Cache::L2Cache(int num_sets, int associativity, int line_size, int type, int id,
                 replacement_policy policy) {
    this->lineMask = ((uint64_t)line_size) - 1;
    this->setShift = log2(line_size);
    this->setMask = (((uint64_t)pow(2, 12) / associativity) - 1) << setShift;
    this->tagMask = ~(this->setMask | this->lineMask);

    this->associativity = associativity;
    this->num_sets = num_sets;
    this->type = type;
    this->id = id;
    this->clock = 1;
    this->policy = policy;
    this->data.resize(num_sets, vector<ll>(associativity, INVALID));
}

void L2Cache::set_parent(L1Cache* parentData, L1Cache* parentInstruction) {
    this->parentData = parentData;
    this->parentInstruction = parentInstruction;
}

void L2Cache::find_in_cache(ull addr, int category, ull pc) {
    uint64_t set = (addr & setMask) >> setShift;
    uint64_t tag = addr & tagMask;

    this->clock++;
    ull idx = set;
    for (int j = 0; j < associativity; ++j) {
        if (this->data[idx][j] == tag) {  // Hit
            switch (this->policy) {
                case BELADY:

                    break;
                case LRU:
                    this->last_use[tag] = this->clock;
                    // cout << "L2 Hit " << addr << endl;
                    break;
            }
            return;
        }
    }
    // Recording  a Miss
    cout << pc << " " << this->id << " " << addr << " " << category << endl;
    // this->child->find_in_cache(addr);

    // If cache has space left
    for (int j = 0; j < this->associativity; ++j) {
        if (this->data[idx][j] == INVALID) {
            this->data[idx][j] = tag;
            switch (this->policy) {
                case BELADY:

                    break;
                case LRU:
                    this->last_use[tag] = this->clock;
                    // cout << "L2 Miss " << addr << endl;
                    break;
            }
            return;
        }
    }

    // Need to invoke replacement policy
    int evict_way;
    switch (this->policy) {
        case BELADY:

            break;
        case LRU:
            long long min_use = LLONG_MAX;
            for (int j = 0; j < associativity; ++j) {
                if (this->last_use[this->data[idx][j]] < min_use) {
                    min_use = this->last_use[this->data[idx][j]];
                    evict_way = j;
                }
            }
            this->last_use.erase(this->data[idx][evict_way]);
            this->last_use[tag] = clock;
            // cout << "L2 Miss " << addr << " Replacing " <<
            // this->data[idx][evict_way] << endl;
            break;
    }
    if (category == INSTRUCTION) {
        this->parentInstruction->invalidate(this->data[idx][evict_way]);
    } else {
        this->parentData->invalidate(this->data[idx][evict_way]);
    }

    this->data[idx][evict_way] = tag;
}

L2Cache::~L2Cache() {}