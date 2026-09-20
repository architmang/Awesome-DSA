#include<bits/stdc++.h>
using namespace std;
// volatile version: data race
// atomic version: clean
template<typename T, int Size>
class spsc_queue{
    public:
        spsc_queue()
        {
            head.store(0);
            tail.store(0);
        }
        bool write(const T* item)
        {
            // Only the shared state needs to be atomic. Locals never do
            // local: head_, tail_
            // snapshot, whole function reasons about one fixed state
            // an atomic load is a real memory access
            // and the compiler is not permitted to keep it in a register across statements
            int head_ = head.load(memory_order_acquire);
            int tail_ = tail.load(memory_order_acquire);
            
            if(head_ - tail_ == Size)
                return false;
            
            int index = head_ % Size;
            buffer[index] = *item;
            head.store(head_+1, memory_order_release);
            // head pe insert, then incremented
            return true;
        }
        const T* peek_nxt_pkt() const
        {
            // local: head_, tail_
            int head_ = head.load(memory_order_acquire);
            int tail_ = tail.load(memory_order_acquire);

            if(head_ - tail_ == 0)
                return nullptr;

            int index = tail_ % Size;
            return &buffer[index];
        }
        void release_pkt()
        {
            int tail_ = tail.load(memory_order_acquire);
            tail.store(tail_ + 1, memory_order_release);
        }
        bool empty() const
        {
            return head.load(memory_order_acquire) == tail.load(memory_order_acquire);
        }
        int size()
        {
            return head.load(memory_order_acquire) - tail.load(memory_order_acquire);
        }
    private:
        alignas(64) atomic<int> head, tail;
        alignas(64) T buffer[Size];
};

static spsc_queue<int, 64> global_q;
static int global_N = 1e5;
static long long global_sum = 0;

void produce()
{
    for(int i=0; i<global_N; i++)
    {
        while(!global_q.write(&i)) {
            // wait coz full
            // write returned as false
        }
    }
}

void consume()
{
    int seen = 0;
    while(seen < global_N)
    {
        while(global_q.peek_nxt_pkt() == nullptr)
        {
            // wait coz empty
            // nullptr returned
        }
        global_sum += *(global_q.peek_nxt_pkt());
        global_q.release_pkt();
        seen += 1;
    }
}

int main()
{
    // 1. empty queue
    {
        spsc_queue<int,4> q;
        assert(q.peek_nxt_pkt() == nullptr);
        cout << "1. empty queue OK\n";
    }

    // 2. write -> peek -> release
    {
        spsc_queue<int,4> q;
        int v = 7;
        assert(q.write(&v));
        assert(*q.peek_nxt_pkt() == 7);
        assert(*q.peek_nxt_pkt() == 7);          // peek does NOT consume
        q.release_pkt();
        assert(q.peek_nxt_pkt() == nullptr);     // release does
        cout << "2. write/peek/release OK\n";
    }

    // 3. queue full
    {
        spsc_queue<int,4> q;
        for(int i = 0; i < 4; i++) assert(q.write(&i));
        int extra = 99;
        assert(!q.write(&extra));                // full, rejected
        q.release_pkt();
        assert(q.write(&extra));                 // one slot freed
        cout << "3. full boundary OK\n";
    }

    // 4. FIFO order
    {
        spsc_queue<int,4> q;
        for(int i = 0; i < 4; i++) q.write(&i);
        for(int i = 0; i < 4; i++){
            assert(*q.peek_nxt_pkt() == i);
            q.release_pkt();
        }
        cout << "4. FIFO order OK\n";
    }

    // 5. wraparound: 1000 items through a 4-slot ring
    {
        spsc_queue<int,4> q;
        for(int i = 0; i < 1000; i++){
            assert(q.write(&i));
            assert(*q.peek_nxt_pkt() == i);
            q.release_pkt();
        }
        cout << "5. wraparound OK\n";
    }

    // 6. two threads
    {
        thread producer(produce);
        thread consumer(consume);
        producer.join();
        consumer.join();
        assert(global_sum == 4999950000);
        cout << "6. two threads OK\n";
    }

    cout << "all tests passed\n";
    return 0;
}