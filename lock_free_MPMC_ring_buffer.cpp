#include<bits/stdc++.h>
using namespace std;
template<typename T, int Size>
class mpmc_queue{
    public:
        struct Cell{
            // data and sequence number
            atomic<int> seq;
            T data;
        };
        // delete the copy constructor
        // Disallow: mpmc_queue q2(q1); (Copy Construction)
        mpmc_queue(const mpmc_queue&) = delete;

        // Disallow: q2 = q1; (Copy Assignment)
        mpmc_queue& operator= (const mpmc_queue&) = delete;

        // Always keep the const and the references (&)
        // It explicitly tells the compiler to look at the 
        // original object directly without creating copies

        mpmc_queue()
        {
            // initialize sequence numbers of slots
            for(int i=0; i<Size; i++)
            {
                buffer[i].seq.store(i, memory_order_relaxed);
            }
            enqueue_pos.store(0, memory_order_relaxed);
            dequeue_pos.store(0, memory_order_relaxed);
        }
        bool push(const T& item)
        {
            // initialize a cell
            Cell* mini_cell;
            int pos = enqueue_pos.load(memory_order_relaxed);
            // The exact value of these global indices doesn't carry payload synchronization
            while(true)
            {
                mini_cell = &buffer[pos % Size];
                int seq_ = mini_cell->seq.load(memory_order_acquire);
                int diff = seq_ - pos;

                if(diff == 0)
                {
                    // perfect macth
                    // try to claim pos
                    if(enqueue_pos.compare_exchange_weak(pos, pos+1, memory_order_relaxed))
                        break;
                }
                else if(diff < 0)
                    return false;
                else{
                        pos = enqueue_pos.load(memory_order_relaxed);
                }
            }        
            mini_cell->data=item;
            mini_cell->seq.store(pos+1, memory_order_release);
            return true;
        }
        bool pop(T& item)
        {
            // initialize a cell
            Cell* mini_cell;
            int pos = dequeue_pos.load(memory_order_relaxed);
            // The exact value of these global indices doesn't carry payload synchronization
            while(true)
            {
                mini_cell = &buffer[pos % Size];
                int seq_ = mini_cell->seq.load(memory_order_acquire);
                int diff = seq_ - (pos+1);

                if(diff == 0)
                {
                    // perfect macth
                    // try to claim pos
                    if(dequeue_pos.compare_exchange_weak(pos, pos+1, memory_order_relaxed))
                    // If the CAS fails, we don't care about memory 
                    // ordering since we didn't acquire the slot.
                        break;
                }
                else if(diff < 0)
                    return false;
                else{
                        pos = dequeue_pos.load(memory_order_relaxed);
                }
            }        
            item = mini_cell->data;
            mini_cell->seq.store(pos+Size, memory_order_release);
            return true;
        }
    private:
        alignas(64) Cell buffer[Size];
        alignas(64) atomic<int> enqueue_pos{0};
        alignas(64) atomic<int> dequeue_pos{0};
};

mpmc_queue<int, 10> q;
atomic<int> seen_so_far;
atomic<int> seen[400];

void producers(int id)
{
    for(int i=0; i<100; i++)
    {
        int value = id * 100 + i; 
        // Generates 0-99, 100-199, 200-299, 300-399

        // q is full so wait
        while(!q.push(value))
            this_thread::yield();
    }
}

void consumers()
{
    int value;
    while(seen_so_far.load(memory_order_acquire) < 400)    
    {
        if(q.pop(value))
        {
            seen_so_far.fetch_add(1, memory_order_release);
            seen[value].fetch_add(1, memory_order_release);
        }
        else{
            this_thread::yield();
            // q is empty so wait
        }
    }
}

int main()
{
    // 1. empty queue
    { mpmc_queue<int,4> t; int out; assert(!t.pop(out)); }

    // 2. push then pop
    { mpmc_queue<int,4> t; int out = 0;
      assert(t.push(7));
      assert(t.pop(out) && out == 7);
      assert(!t.pop(out)); }

    // 3. queue full
    { mpmc_queue<int,4> t;
      for (int i = 0; i < 4; i++) assert(t.push(i));
      assert(!t.push(99));                  // full, rejected
      int out; t.pop(out);
      assert(t.push(99)); }                 // one slot freed

    // 4. FIFO order, single threaded
    { mpmc_queue<int,4> t;
      for (int i = 0; i < 4; i++) t.push(i);
      for (int i = 0; i < 4; i++){ int out; assert(t.pop(out) && out == i); } }

    // 5. wraparound: 1000 items through a 4-slot ring
    { mpmc_queue<int,4> t;
      for (int i = 0; i < 1000; i++){
          int out;
          assert(t.push(i));
          assert(t.pop(out) && out == i);
      } }

    // 6. 4 producers 4 consumers every item exactly once
    {
        // The object initialization in the constructor 
        // and test suite setups can use relaxed ordering safely before threads spawn.
        for(int i=0; i<400; i++)
            seen[i].store(0, memory_order_release);
        
        vector<thread> threads;
        for(int i=0; i<4; i++)
            threads.push_back(thread(producers, i));
        for(int i=0; i<4; i++)
            threads.push_back(thread(consumers));
        for(int i=0; i<8; i++)
            threads[i].join();
        
        int missing =0, duplicated =0;
        for(int i=0; i<400; i++)
        {
            int n = seen[i].load();
            if(n == 0) missing++;
            else if(n>1) duplicated++;
        }
        cout << "missing " << missing << endl;
        cout << "duplicated " << duplicated << endl;
        assert(!missing && !duplicated);

    }

    cout << "all tests passed\n";
    return 0;
}