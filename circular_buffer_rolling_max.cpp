#include<bits/stdc++.h>
using namespace std;
class Circular_buffer{
    public:
        Circular_buffer(int capacity_){
            capacity = capacity_;
            head = 0;
            size = 0;
            buffer.resize(capacity);
            // every element in maxq is bigger than everything to its right
            // front is oldest
        }
        bool isempty()
        {
            if(size == 0) return true;
            return false;
        }
        bool isfull()
        {
            if(size == capacity) return true;
            return false;
        }
        int getsize(){
            return size;
        }
        int getcapacity()
        {
            return capacity;
        }
        void push(int val)
        {
            if(isfull())
                throw runtime_error("pushing on a full queue buffer \n");
            int tail = (head + size)%capacity;
            buffer[tail] = val;
            size++;

            // maxq has the maximum of queue
            // now val comes
            // 2 3 5
            while(!maxq.empty() && val > maxq.back())
            {
                maxq.pop_back();
            }
            maxq.push_back(val);
            // 5 3 2
        }
        int peek()
        {
            if(isempty())
                throw runtime_error("peeking on an empty queue buffer \n");
            int value = buffer[head];
            return value;
        }
        int pop()
        {
            if(isempty())
                throw runtime_error("popping on an empty queue buffer \n");
            int value = buffer[head];
            head = (head+1) % capacity;
            size--;

            // if the element leaving was the current max, drop it
            // else dont worry as it was already discarded
            if(!maxq.empty() && value == maxq.front())
                maxq.pop_front();

            return value;
        }
        int getmax()
        {
            if(isempty())
                throw runtime_error("get max on an empty queue buffer \n");
            return maxq.front();
        }
        // Callers index 0..size-1 and never learn where anything physically sits
        int at(int i)
        {
            // i is wrt number of elements entered
            if(i<0 || i>=size)
                throw runtime_error("invalid index \n");
            int index = (head + i)%capacity;
            return buffer[index];
        }
        void resize(int newCapacity)
        {
            if(newCapacity <size)
                throw runtime_error("capacity cant be smaller than current size \n");
            
            vector<int> fresh(newCapacity);
            for(int i=0; i<size; i++)
                fresh[i] = at(i);
            
            // reset the private data
            buffer = fresh;
            capacity = newCapacity;
            head = 0;
        }
    private:
        int head; //index of oldest element
        int size;
        int capacity;
        vector<int> buffer;
        deque<int> maxq;
};
int main(){ 
    Circular_buffer b(3);
b.push(5); b.push(5); b.push(2);
cout << b.getmax() << endl;  // 5
b.pop();      // one 5 leaves
cout << b.getmax() << endl;  // 5
    return 0;
}