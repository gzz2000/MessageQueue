#include <iostream>
#include <string>
#include <thread>
#include "messagequeue.hpp"

messagequeue<std::string> q;

void a() {
    std::string s;
    while(true) {
        s = q.pop();
        std::cout << s << std::endl;
        if(s == "end") break;
    }
}

int main() {
    std::thread t(a);
    q.push("test");
    q.setTimeout("hello", 500);
    q.clearTimeout(q.setTimeout("world", 600));
    q.setTimeout("end", 1000);
    t.join();
    return 0;
}
