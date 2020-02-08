读者写者模型
读者写者问题之写者优先
问题要求：

读者-写者问题的读写操作限制(仅读者优先或写者优先)：

写-写互斥，即不能有两个写者同时进行写操作。
读-写互斥，即不能同时有一个线程在读，而另一个线程在写。
读-读允许，即可以有一个或多个读者在读。


写者优先的附加限制：如果一个读者申请进行读操作时已有另一写者在等待访问共享资源，
则该读者必须等到没有写者处于等待状态后才能开始读操作。

写者优先实现思路：

写者优先与读者优先类似。不同之处在于一旦一个写者到来，它应该尽快对文件进行写操作，
如果有一个写者在等待，则新到来的读者不允许进行读操作。为此应当添加一个整型变量write_count，
用于记录正在等待的写者的数目，当write_count=0时，才可以释放等待的读者线程队列。

为了对全局变量write_count实现互斥，必须增加一个互斥对象mutex2。

为了实现写者优先，应当添加一个临界区对象read，当有写者在写文件或等待时，读者必须阻塞在read上。同样，有读者读时，写者必须等待。
于是，必须有一个互斥对象RW_mutex来实现这个互斥。

有写者在写时，写者必须等待。

读者线程要对全局变量read_count实现操作上的互斥，必须有一个互斥对象命名为mutex1。

实现代码：
代码中信号量解析：

设置五个信号量，分别是RWMutex, mutex1, mutex2, mutex3, wrt，两个全局整型变量writeCount, readCount
信号量mutex1在写者的进入区和退出区中使用，使得每次只有一个写者对其相应进入区或推出区进行操作，
主要原因是进入区和退出区存在对变量writeCount的修改，每个写者其进入区中writeCount加1，
退出区中writeCount减1。信号量RWMutex则是读者和写者两个之间的互斥信号量，保证每次只读或者只写。
写者优先中，写者的操作应该优先于读者，则信号量一直被占用着，直到没有写者的时候才会释放，
即当writeCount等于1的时候，申请信号量RWMutex，其余的写者无需再次申请，但是写者是不能同时进行写操作的，
则需要设置一个信号量wrt来保证每次只有一个写者进行写操作，当写者的数量writeCount等于0的时候，
则证明此时没有没有读者了，释放信号量RWMutex。信号量mutex2防止一次多个读者修改readCount。
当readCount为1的时候，为阻止写者进行写操作，申请信号量wrt，则写者就无法进行写操作了。
信号量mutex3的主要用处就是避免写者同时与多个读者进行竞争，读者中信号量RWMutex比mutex3先释放，则一旦有写者，写者可马上获得资源。

实现代码：
/*
* 	写者优先
*/

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <sys/types.h>
# include <pthread.h>
# include <semaphore.h>
# include <string.h>
# include <unistd.h>

//semaphores
sem_t RWMutex, mutex1, mutex2, mutex3, wrt;
int writeCount, readCount;


struct data {
	int id;
	int opTime;
	int lastTime;
};

//读者
void* Reader(void* param) {
	int id = ((struct data*)param)->id;
	int lastTime = ((struct data*)param)->lastTime;
	int opTime = ((struct data*)param)->opTime;

	sleep(opTime);
	printf("Thread %d: waiting to read\n", id);	

	sem_wait(&mutex3);
	sem_wait(&RWMutex);
	sem_wait(&mutex2);
	readCount++;
	if(readCount == 1)
		sem_wait(&wrt);
	sem_post(&mutex2);
	sem_post(&RWMutex);
	sem_post(&mutex3);

	printf("Thread %d: start reading\n", id);
	/* reading is performed */ 
	sleep(lastTime);
	printf("Thread %d: end reading\n", id);

	sem_wait(&mutex2);
	readCount--;
	if(readCount == 0)
		sem_post(&wrt);
	sem_post(&mutex2);

	pthread_exit(0);
}

//写者
void* Writer(void* param) {
	int id = ((struct data*)param)->id;
	int lastTime = ((struct data*)param)->lastTime;
	int opTime = ((struct data*)param)->opTime;

	sleep(opTime);
	printf("Thread %d: waiting to write\n", id);
	
	sem_wait(&mutex1);
	writeCount++;
	if(writeCount == 1){
		sem_wait(&RWMutex);
	}
	sem_post(&mutex1);
	
	sem_wait(&wrt);
	printf("Thread %d: start writing\n", id);
	/* writing is performed */
	sleep(lastTime);
	printf("Thread %d: end writing\n", id);
	sem_post(&wrt);

	sem_wait(&mutex1);
	writeCount--;
	if(writeCount == 0) {
		sem_post(&RWMutex);
	}
	sem_post(&mutex1);
	
	pthread_exit(0);
}

int main() {
	//pthread
	pthread_t tid; // the thread identifier

	pthread_attr_t attr; //set of thread attributes

	/* get the default attributes */
	pthread_attr_init(&attr);

	//initial the semaphores
    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);
    sem_init(&mutex3, 0, 1);
    sem_init(&wrt, 0, 1);
    sem_init(&RWMutex, 0, 1);

    readCount = writeCount = 0;

	int id = 0;
	while(scanf("%d", &id) != EOF) {

		char role;		//producer or consumer
		int opTime;		//operating time
		int lastTime;	//run time

		scanf("%c%d%d", &role, &opTime, &lastTime);
		struct data* d = (struct data*)malloc(sizeof(struct data));

		d->id = id;
		d->opTime = opTime;
		d->lastTime = lastTime;

		if(role == 'R') {
			printf("Create the %d thread: Reader\n", id);
			pthread_create(&tid, &attr, Reader, d);

		}
		else if(role == 'W') {
			printf("Create the %d thread: Writer\n", id);
			pthread_create(&tid, &attr, Writer, d);
		}
	}

	sem_destroy(&mutex1);
	sem_destroy(&mutex2);
	sem_destroy(&mutex3);
	sem_destroy(&RWMutex);
	sem_destroy(&wrt);

	return 0;
}

哲学家进餐问题描述

假设有五位哲学家围坐在一张圆形餐桌旁，做以下两件事情之一：吃饭，或者思考。吃东西的时候，
他们就停止思考，思考的时候也停止吃东西。餐桌中间有一大碗意大利面，每两个哲学家之间有一只餐叉。
因为用一只餐叉很难吃到意大利面，所以假设哲学家必须用两只餐叉吃东西。他们只能使用自己左右手边的那两只餐叉。
哲学家就餐问题有时也用米饭和筷子而不是意大利面和餐叉来描述，因为很明显，吃米饭必须用两根筷子。

三个求解策略

首先肯定是使用线程来模拟哲学家的，一个线程是一个哲学家，然后要求线程在进行资源竞争时不死锁。

下面给出几种求解策略

1. 利用锁机制，每次保证一个哲学家在吃，哲学家想吃的时候，先测试一下锁，如果能获得锁，
则测试一下自己的筷子以及右边的筷子能不能拿，如果能拿则拿起筷子吃，否则阻塞，哲学家思考时测试一下能不能获得互斥锁，如果能则该哲学家放下自己的筷子和右边的筷子。

2. 封装信号量，定义类Semaphore代表信号量，并封装down和up方法，当信号量执行down方法时信号量减一，
如果值变为0则阻塞线程，调用up方法使信号量加一，然后定义信号量metux=4
来保证同时只有四个哲学家拿筷子，信号量筷子chopsticks []={1,1,1,1,1,…,1}，
首先metux.down(),保证最多进4个哲学家，然后down自己的筷子，down右边的筷子,
如果线程未阻塞，则可以吃饭，吃完后，up右筷子，up自己的筷子，最后metux.up()。

3. 利用封装好的信号量，使用奇偶法。防止哲学家饿死的情况，制定如下规则：

规则： 奇数号的哲学家先拿起右边的筷子再拿起左边的筷子。

            偶数号哲学家先拿起左边的筷子，再拿起右边的筷子。

            如果哲学家抢到一只筷子，在抢占另一只筷子时失败，则要放弃已经抢占到的资源。

            左右两边都抢到筷子的哲学家，吃完放后释放资源。

实现时只需在求解策略二的基础上加个奇偶判断就可以了


C++11的thread线程类: C++11引入的跨平台的线程类

C++11 mutex互斥锁:mutex：独占的互斥量

condition_variable条件变量: 条件变量是C++11提供的另外一种用于等待的同步机制，
它能阻塞一个或多个线程，直到收到另外一个线程发出的通知或者超时，才会唤醒当前阻塞的线程

代码如下


#include<iostream>

#include<thread>

#include<mutex>

#include<windows.h>

#include<chrono>

#include<condition_variable>

#include<map>

#include<functional>

#include<string>

#include<sstream>

#include<vector>

#include<functional>

#include<ctime>

#include<cstdlib>

#define random(a,b) (rand()%(b-a+1)+a)

using namespace std;

unsigned int MaxN;

namespace Method {

mutex alock;

condition_variable t;

class Semaphore{//信号量的封装 down up操作

private:

    unsigned int m;

public:

    Semaphore(unsigned int x):m(x){}

    Semaphore(){}

    Semaphore(const Semaphore& t){m = t.m;}

    ~Semaphore(){}

    void down(){

      unique_lock<mutex> locker(alock);

       while(m==0){

           cout<<"(thread id="<<this_thread::get_id()<<" is wait)"<<endl;

           t.wait(locker);

       }

        m--;

    }

    void up(){

        alock.lock();

        m++;

        t.notify_all();

        alock.unlock();

    }

    bool operator == (const Semaphore &t) {

        return m == t.m;

    }

    bool operator !=(const Semaphore &t){

        return m!=t.m;

    }

    bool operator == (const unsigned int &t) {

        return m == t;

    }

    bool operator != (const unsigned int &t){

        return m!=t;

    }

    void operator = (const Semaphore &t){

        m = t.m;

    }

    void operator = (const unsigned int &t){

        m = t;

    }

    Semaphore operator + (const Semaphore &t){

        return Semaphore(m+t.m);

    }

    Semaphore operator + (const unsigned int &t){

        return Semaphore(m+t);

    }

};

int Int(thread::id id){

  stringstream ss;

  ss<<id;

  return std::stoi(ss.str());

}

Semaphore mutexs = 4;

Semaphore chopsticks[10000];

//求解策略部分的代码是重点

//求解策略一

auto functionA = []()->void{ //lambda表达式

        auto eating = []()->void{ //获取线程ID

 

               unique_lock<mutex> locker(alock);

               int current_id = Int(this_thread::get_id())-2;

               while(chopsticks[current_id]==0||chopsticks[(current_id+1)%MaxN]==0){ //是否能拿自己的筷子和右边的筷子，如果不能则阻塞

                   cout<<"(thread id="<<this_thread::get_id()<<" is wait)"<<endl;

                   t.wait(locker); //阻塞线程

               }

               chopsticks[current_id] = 0; //拿自己的筷子

               chopsticks[(current_id+1)%MaxN] = 0; //拿右边的筷子

               cout<<"Philosopher"<<current_id<<" are eating (thread id="<<this_thread::get_id()<<" is run)"<<endl;

 

          };

         auto thinking = []()->void{ //思考方法

               alock.lock(); //获得锁

               int current_id = Int(this_thread::get_id())-2; //当前线程

               chopsticks[current_id]=1; //放下自己的筷子

               chopsticks[(current_id+1)%MaxN]=1; //放下右边的筷子

               t.notify_all(); //唤醒所有线程

               cout<<"Philosopher"<<current_id<<" are thinking (thread id="<<this_thread::get_id()<<" is run)"<<endl;

               alock.unlock(); //释放锁

           };

       while(true){ //无限执行线程

           thinking();

           chrono::milliseconds s(1000); //睡眠1s

           this_thread::sleep_for(s);

           eating();

       }

   };

//求解策略二 

 auto functionB = []()->void{ //lambda表达式

 

            while(true){ //无限执行线程

                mutexs.down(); //down(mutex)

                size_t id = Int(this_thread::get_id())-2; //线程ID

                chopsticks[id].down(); //down(chopsticks[id])

                chopsticks[(id+1)%MaxN].down(); //down(chopsticks[(id+1)%MaxN])

                alock.lock();  //锁一下，保证同时只有一个线程往屏幕里输出

                cout<<"Philosopher"<<id<<" are eating (thread id="<<this_thread::get_id()<<" is run)"<<endl;

                alock.unlock(); //释放锁

                chrono::milliseconds s(1000); //睡眠1s

                this_thread::sleep_for(s);

 

                chopsticks[(id+1)%MaxN].up(); //up(chopsticks[(id+1)%MaxN])

                chopsticks[id].up(); //up(chopsticks[id])

                alock.lock(); //锁一下，保证同时只有一个线程往屏幕里输出

                cout<<"Philosopher"<<id<<" are thinking (thread id="<<this_thread::get_id()<<" is run)"<<endl;

                alock.unlock(); //释放锁

                this_thread::sleep_for(s);

                mutexs.up();

 

            }

 

 

};

//求解策略三

auto functionC = []()->void{ //lambda表达式

 

            while(true){ //无限执行线程

                int id = Int(this_thread::get_id())-2; //获取当前线程

                if(id%2){

                    //奇数

                    chopsticks[(id+1)%MaxN].down(); //down(chopsticks[(id+1)%MaxN])

                    chopsticks[id].down(); //down(chopsticks[id])

                    alock.lock(); //锁一下，保证同时只有一个线程往屏幕里输出

                    cout<<"Philosopher"<<id<<" are eating (thread id="<<this_thread::get_id()<<" is run)"<<endl;

                    alock.unlock(); //释放锁

                    chrono::milliseconds s(1000); //睡眠1s

                    this_thread::sleep_for(s);

                    chopsticks[(id+1)%MaxN].up(); //up(chopsticks[(id+1)%MaxN])

                    chopsticks[id].up(); //up(chopsticks[id])

 

                }else{

                    //偶数

                    chopsticks[id].down(); //down(chopsticks[id])

                    chopsticks[(id+1)%MaxN].down(); //down(chopsticks[(id+1)%MaxN])

                    alock.lock(); //锁一下，保证同时只有一个线程往屏幕里输出

                    cout<<"Philosopher"<<id<<" are eating (thread id="<<this_thread::get_id()<<" is run)"<<endl;

                    alock.unlock(); //释放锁

                    chrono::milliseconds s(1000); //睡眠1s

                    this_thread::sleep_for(s);

                    chopsticks[id].up(); //up(chopsticks[id])

                    chopsticks[(id+1)%MaxN].up(); //up(chopsticks[(id+1)%MaxN])

                }

               alock.lock(); //锁一下，保证同时只有一个线程往屏幕

               cout<<"Philosopher"<<id<<" are thinking (thread id="<<this_thread::get_id()<<" is run)"<<endl;

               alock.unlock(); //释放锁

 

                   }

                };

 

}

using namespace Method;

//定义哲学家类

class Philosopher{

private:

    vector<function<void()> > fns;

 

    Philosopher(){}

    Philosopher(const Philosopher& t){fns = t.fns;}

    ~Philosopher(){}

public:

    static Philosopher *instance;

    static Philosopher *of(){

        if(instance) return instance;

        return (instance = new Philosopher());

    }

    void add(initializer_list<function<void()> >fs){

        for(auto iter = fs.begin();iter!=fs.end();iter++){

            fns.push_back(*iter);

        }

 

    }

    void add(function<void()> t){

        fns.push_back(t);

    }

    function<void() > get(size_t i){

        return fns[i];

    }

};

 Philosopher *Philosopher::instance = 0;

int main()

{

    Philosopher::of()->add({functionA,functionB,functionC});

    cout<<"input Philosopher number:"<<endl;

    cin>>MaxN;

    cout<<"select method"<<endl;

    cout<<"1.lock"<<endl;

    cout<<"2.semaphore use mutex = 4"<<endl;

    cout<<"3.semaphore use odd even method"<<endl;

    unsigned int num;

    cin>>num;

 

    vector<thread> phils;

    for(size_t i=0;i<MaxN;i++){

        chopsticks[i] = 1;

    }

    switch (num) {

    case 1:

        for(size_t i=0;i<MaxN;i++){

            phils.push_back(thread(Philosopher::of()->get(0)));

        }

        break;

    case 2:

        for(size_t i=0;i<MaxN;i++){

 

            phils.push_back(thread(Philosopher::of()->get(1)));

        }

        break;

    case 3:

        for(size_t i=0;i<MaxN;i++){

 

            phils.push_back(thread(Philosopher::of()->get(2)));

        }

        break;

    default:

        break;

    }

 

    for(auto iter = phils.begin();iter!=phils.end();iter++){

        iter->join();

    }

    return 0;

}


14 逐步改进

这一章主要是讲述一个命令行参数的一个程序，逐步改进代码。本章篇幅极大（190页到250页），工作量约为3天。



