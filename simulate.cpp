#include "constants.h"
#include "LLCache.h"
#include "L2Cache.h"
#include "L1Cache.h"


int NUM_CORES = 1;
int main(int argc, char const *argv[]) {
    /******************************
                Simulation
        ******************************/
    int associativity_l1 = 8;
    int associativity_l2 = 8;
    int associativity_llc = 20;
    int line_size_l1 = 64;
    int line_size_l2 = 64;
    int line_size_llc = 64;
    int num_sets_l1 = 64;
    int num_sets_l2 = 512;
    int num_sets_llc = (int)pow(2, 10);

    unsigned int tid;
    ull block_addr;
    ull inst;
    int category;

    L1Cache l1data =
        L1Cache(num_sets_l1, associativity_l1, line_size_l1, 1, 0, LRU);
    L1Cache l1instruction = L1Cache(num_sets_l1, associativity_l1, line_size_l1,
                                    INSTRUCTION, 0, LRU);

    L2Cache l2unified = L2Cache(num_sets_l2, associativity_l2, line_size_l2, 1, 0, LRU);

    LLCache llcache = LLCache(num_sets_llc, associativity_llc, line_size_llc, 1, 0, LRU);

    l1data.set_child(&l2unified);
    l1instruction.set_child(&l2unified);
    l2unified.set_parent(&l1data, &l1instruction);
    l2unified.set_child(&llcache);
    llcache.set_parent(&l2unified);

    // L2Cache* l2 = new L2Cache(2,3,LRU);
    // l1->set_child(l2);
    // l2->set_parent(l1);

    ifstream infile;
    infile.open("pinatrace.out", ifstream::in);
    assert(infile.is_open());

    char rw;

    while (!infile.eof()) {
        infile >> hex >> inst;

        if (!inst) break;

        infile.ignore(256, ':');

        infile >> rw;
        assert(rw == 'R' || rw == 'W');
        if (rw == 'R') {
            category = LOAD;
        } else {
            category = STORE;
        }

        infile >> hex >> block_addr;

        l1data.find_in_cache(block_addr, category, inst);
    }
    infile.close();

    /*
        while(input_file >> tid >> block_addr >> category){
        cout << block_addr << endl;
                tid = tid%NUM_CORES;
                if(category==INSTRUCTION){
                        l1instruction[tid]->find_in_cache(block_addr, category,
   tid);
                }
                else{
                        l1data[tid]->find_in_cache(block_addr, category, tid);
                }
   }
   */

    return 0;
}