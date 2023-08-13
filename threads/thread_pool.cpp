/** Basic thread pool example with predefined number of workers consuming vector's elements
 * and does some simple calculations */

#include <iostream>
#include <thread>
#include <vector>
#include <array>
#include <mutex>
#include <chrono>

using std::cout;
using std::endl;
using std::string;
using std::vector;

std::mutex mut;

class ThreadPool {
public:
    explicit ThreadPool(int size) : m_size(size) {}

    float run(float (*fptr)(float), float value, int index) {
        m_progress[index] = false;
        float result = fptr(value);
        m_progress[index] = true;
        return result;
    }

    bool start(const vector<float> &dataVector, float (*fptr)(float)) {
        if (!fptr)
            return false;

        if (dataVector.empty())
            return true;

        int data_index = -1;

        // 1. fill array of threads
        for (int i = 0; i < std::min(m_size, (int)dataVector.size()); i++) {
            data_index++;
            m_threads.emplace_back(&ThreadPool::run, this, fptr, dataVector[data_index], i);
            m_progress.push_back(false);
        }

        // 2. check which threads has finished to assign new task
        while (data_index < dataVector.size() - 1) {

            for (int i = 0; i < m_progress.size(); i++) {
                if (!m_progress[i])
                    continue;

                if (!m_threads[i].joinable())
                    continue;

                m_threads[i].join();
                data_index++;
                if (data_index >= dataVector.size())
                    break;

                m_threads[i] = std::thread(&ThreadPool::run, this, fptr, dataVector[data_index], i);
                m_progress[i] = false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Join any left running threads
        for (auto &t: m_threads) {
            if (t.joinable())
                t.join();
        }

        return true;
    }

protected:
    int m_size{0};
    std::vector<std::thread> m_threads;
    std::vector<bool> m_progress;
};


float process(float value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    float r = value + 0.2f;

    mut.lock();
    cout << r << endl;
    mut.unlock();
    return r;
}


int main() {

    vector<float> vf{0.0f, 0.1f, -1.2f, -0.9f, 2.7f, -1.02f};


    ThreadPool threadPool(3);
    threadPool.start(vf, process);


    return 0;
}