#include "constants.h"
#include "L1Cache.h"
#include "L2Cache.h"

L1Cache::L1Cache(int num_sets, int associativity, int line_size, int type,
                 int id, replacement_policy policy) {
    this->lineMask = ((uint64_t)line_size) - 1;
    this->setShift = log2(line_size);
    this->setMask = (((uint64_t)pow(2, 9) / associativity) - 1) << setShift;
    this->tagMask = ~(this->setMask | this->lineMask);

    this->associativity = associativity;
    this->num_sets = num_sets;
    this->type = type;
    this->id = id;
    this->clock = 1;
    this->policy = policy;
    this->data.resize(num_sets, vector<ll>(associativity, INVALID));
}

void L1Cache::set_child(L2Cache *child) { this->child = child; }

void L1Cache::find_in_cache(ull addr, int category, ull pc) {
    uint64_t set = (addr & setMask) >> setShift;
    uint64_t tag = addr & tagMask;

    this->clock++;
    ull idx = set;
    for (int j = 0; j < this->associativity; ++j) {
        if (this->data[idx][j] == tag) {  // Hit
            switch (this->policy) {
                case BELADY:

                    break;
                case LRU:
                    this->last_use[tag] = this->clock;
                    // cout << "L1 Hit " << addr << endl;
                    break;
            }
            return;
        }
    }
    // Miss
    this->child->find_in_cache(addr, category, pc);

    // If cache has space left
    for (int j = 0; j < this->associativity; ++j) {
        if (this->data[idx][j] == INVALID) {
            this->data[idx][j] = tag;
            switch (this->policy) {
                case BELADY:

                    break;
                case LRU:
                    this->last_use[tag] = this->clock;
                    // cout << "L1 Miss " << addr << endl;
                    break;
            }
            return;
        }
    }

    // Need to invoke replacement policy

    switch (this->policy) {
        case BELADY:

            break;
        case LRU:
            int min_way;
            long long min_use = LLONG_MAX;
            for (int j = 0; j < associativity; ++j) {
                if (this->last_use[this->data[idx][j]] < min_use) {
                    min_use = this->last_use[this->data[idx][j]];
                    min_way = j;
                }
            }
            this->last_use.erase(this->data[idx][min_way]);
            this->last_use[tag] = clock;
            // cout << "L1 Miss " << addr << " Replacing " <<
            // this->data[idx][min_way]
            // << endl;
            this->data[idx][min_way] = tag;
            break;
    }
}

void L1Cache::invalidate(ull addr) {
    ull idx = (addr & this->setMask) >> this->setShift;
    uint64_t tag = addr & tagMask;
    // cout << "L1 invalidate " << addr << endl;
    for (int j = 0; j < this->associativity; ++j) {
        if (this->data[idx][j] == tag) {
            last_use.erase(tag);
            data[idx][j] = INVALID;
            return;
        }
    }
}

L1Cache::~L1Cache() {}