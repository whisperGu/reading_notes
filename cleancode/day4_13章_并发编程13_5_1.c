13. 并发编程
对象是过程的抽象，线程是调度的抽象。

真正要做到整洁的并发，这非常难，一章搞不定的，这里简单介绍并发的困难，以及一些对付这些难点的建议而已。

并发场景：
1. 巨大数据集，需要处理完毕后给出解决方案，这时采用并发，在一台计算机处理每个数据集。
2. 用户访问多个网站，无需等待Io，同时访问多个网站
3. 响应用户访问，无需一个个用户响应，完全可以并发处理多个请求

误解：
1. 并发不一定总改善性能，只在那种多线程和处理器分摊等待时间时管用
2. 正确的并发是极为复杂的。
3. 并发缺陷很难重现，所以编写并发程序，往往和单线程设计极为不同。

并发防御原则：
1. 单一权责 SRP 
并发代码与其他代码分离，它应该有自己的开发，修改，调优的生命周期。
2. 限制数据作用域
对共享对象限制临界区。
3. 使用数据副本，尽可能以只读方式对待。从多线程拿到副本，在单线程中合并这些结果。
4. 线程间尽可能互斥。

常见的并发模型:

基本概念：
限定资源：并发环境中有固定尺寸或数量的资源
互斥:每一时刻仅有一个线程可以访问
线程饥饿：一个或一组线程在很长时间内无法运行
石锁：两个或多个线程互相等待资源，无法种植
活锁：线程和任务不是block的状态， 表现为线程会持续尝试起步，改变自身状态，，但可能很长时间都无法如愿启动。

多进程并发和多线程并发：
那么进程和线程有什么区别呢？
进程是系统进行资源分配和调度的一个独立单位。线程是cpu调度的基本单位。线程不会有系统资源，在运行时，只是暂用
一些计数器，寄存器和栈。
一个线程只属于一个进程，一个进程可以有多个线程。线程同步简答，进程同步需要用IPC（管道，SOCKET等等）

生产者-消费者模型：
多个生产者对多个消费者，生产出的产品放在缓存队列中，消费者拿到产品进行工作。

生产者-》缓冲区-》消费者

算法思想：
多个生产者和多个生产者的问题。生产者不断的向仓库放入产品，消费者不断的从仓库取出产品，仓库的容量是有限的。
因此，当仓库处于满状态时，生产者必须等待消费者取出 1 个或多个产品后才能继续生产；
同理，当仓库处于空状态时，消费者必须等待生产者将产品放入仓库后才能消费（取出）产品。

使用数组模拟仓库，需要记录下一次生产和消费在数组中的位置。 
用生产和消费在数组中的位置判断仓库是否为空或者为满： 
假设仓库容量为 N：


(produce_position+1)%N ==  consume_position 满 
因为初始位置都是 0，当两者相差一个位置时，定义满状态。（最多存储N-1个）
consume_position == produce_position 空


当仓库满时，阻塞生产者；当一个消费行为后，仓库非满，唤醒生产者； 
当仓库空时，阻塞消费者；当一个生产行为后，仓库非空，唤醒消费者； 
因此需要引入，仓库非满条件变量和仓库非空条件变量。

由于生产和消费行为都会修改数据，因此两者操作必须互斥，需引入生产消费互斥锁。 
当我们要生产（或消费）一定数量的产品时，需要计数判断是否已经完成工作；多个生产者进行生产时，
都会对生产的计数变量进行修改，因此需引入生产计数互斥锁和消费计数互斥锁，保证同时只有一个生产（或消费）进程对计数变量进行修改。

代码：
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <thread>
using namespace std;
const int kProduceItems = 10;
const int kRepositorySize = 4;
template<class T>
class Repository {
public:
    T items_buff[kRepositorySize];
    mutex mtx; // 生产者消费者互斥量
    mutex produce_mutex; // 生产计数互斥量
    mutex consume_mutex; // 消费计数互斥量
    size_t produce_item_count;
    size_t consume_item_count;
    size_t produce_position; // 下一个生产的位置
    size_t consume_position; // 下一个消费的位置
    condition_variable repo_not_full; // 仓库不满条件变量
    condition_variable repo_not_empty; // 仓库不空条件变量
    Repository() {
        produce_item_count = 0;
        consume_item_count = 0;
        produce_position = 0;
        consume_position = 0;
    };
    void Init() {
        fill_n(items_buff, sizeof(items_buff)/sizeof(items_buff[0]), 0);
        produce_item_count = 0;
        consume_item_count = 0;
        produce_position = 0;
        consume_position = 0;
    }
};
template<class T> 
class Factory {
private:
    Repository<T> repo;

    void ProduceItem(Repository<T> &repo, T item) {
        unique_lock<mutex> lock(repo.mtx);
        // +1 后判断，因为在初始时，两者位于同一位置（因此仓库中最大存在 kRepositorySize-1 个产品）
        while ((repo.produce_position+1) % kRepositorySize == repo.consume_position) { 
            cout << "Repository is full, waiting..." << endl;
            (repo.repo_not_full).wait(lock); // 阻塞时释放锁，被唤醒时获得锁
        }
        repo.items_buff[repo.produce_position++] = item;
        if (repo.produce_position == kRepositorySize)
            repo.produce_position = 0;
        (repo.repo_not_empty).notify_all(); // 唤醒所有因空阻塞的进程
        lock.unlock();
    }
    T ConsumeItem(Repository<T> &repo) {
        unique_lock<mutex> lock(repo.mtx);
        while (repo.consume_position == repo.produce_position) {
            cout << "Repository is empty, waiting ..." << endl;
            (repo.repo_not_empty).wait(lock);
        }
        T data = repo.items_buff[repo.consume_position++];
        if (repo.consume_position == kRepositorySize)
            repo.consume_position = 0;
        (repo.repo_not_full).notify_all();
        lock.unlock();
        return data;
    }
public:
    void Reset() {
        repo.Init();
    }
    void ProduceTask() {
        bool ready_to_exit = false;
        while (true) {
            sleep(1); // 如果不sleep ,运行太快，一个进程会完成所有生产
            unique_lock<mutex> lock(repo.produce_mutex);

            if (repo.produce_item_count < kProduceItems) {
                ++(repo.produce_item_count);
                T item = repo.produce_item_count;
                cout << "producer id: "<< this_thread::get_id() << " is producing " 
                     << item << "^th item..." << endl;
                ProduceItem(repo, item);
            } else {
                ready_to_exit = true;
            }

            lock.unlock();
            // sleep(1);
            if (ready_to_exit)
                break;
        }       
        cout << "Producer thread " << std::this_thread::get_id()
             << " is exiting..." << endl;
    }

    void ConsumeTask() {
        bool ready_to_exit =false;
        while (true) {
            sleep(1); // 如果不sleep ,运行太快，一个进程会消费所有产品
            unique_lock<mutex> lock(repo.consume_mutex);

            if (repo.consume_item_count < kProduceItems) {
                T item = ConsumeItem(repo);
                cout << "consumer id: " << this_thread::get_id() << " is consuming "
                     << item << "^th item" << endl;
                ++(repo.consume_item_count);
            } else {
                ready_to_exit = true;
            }

            lock.unlock();
            // sleep(1);
            if (ready_to_exit)
                break;
        }
        cout << "Consumer thread " << std::this_thread::get_id()
             << " is exiting..." << endl;
    }
};

int main() {
    cout << "Main thread id :" << this_thread::get_id() << endl;
    class Factory<int> myfactory;
    thread producer1(&Factory<int>::ProduceTask, &myfactory);
    thread producer2(&Factory<int>::ProduceTask, &myfactory);
    thread producer3(&Factory<int>::ProduceTask, &myfactory);

    thread consumer1(&Factory<int>::ConsumeTask, &myfactory);
    thread consumer2(&Factory<int>::ConsumeTask, &myfactory);
    thread consumer3(&Factory<int>::ConsumeTask, &myfactory);

    producer1.join();
    producer2.join();
    producer3.join();
    consumer1.join();
    consumer2.join();
    consumer3.join();
}

原文链接：https://blog.csdn.net/quzhongxin/article/details/47787543