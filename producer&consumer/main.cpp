//
//  main.cpp
//  producer&consumer
//
//  Created by Michael Tan on 2019/11/28.
//  Copyright © 2019 MichaelTan9999. All rights reserved.
//

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable produce, consume;

queue<int> good;
int maxSize = 20;

void consumer(int id)
{
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(1250));
        unique_lock<mutex> lck(mtx);
        consume.wait(lck, [] {return good.size() != 0; });
        cout << "<- consumer " << id << ": ";
        good.pop();
        cout << good.size() << endl;
        produce.notify_all();
    }
}

void producer(int id)
{
    while (true)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        unique_lock<mutex> lck(mtx);
        produce.wait(lck, [] {return good.size() != maxSize; });
        cout << "-> producer " << id << ": ";
        good.push(id);
        cout << good.size() << endl;
    }
}

int main()
{
    thread consumers[4], producers[3];
    
    for (int i = 0; i < 4; ++i)
    {
        consumers[i] = thread(consumer, i + 1);
       
    }
    for (int i = 0; i < 3; i++)
    {
        producers[i] = thread(producer, i + 1);
    }
    
    
    for (int i = 0; i < 4; ++i)
    {
        consumers[i].join();
    }
    for (int i = 0; i < 3; i++)
    {
        producers[i].join();
    }
    
//    system("pause");
    return 0;
}

