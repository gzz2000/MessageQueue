# MessageQueue
A fast and lightweight message queue for inter-thread communication in C++11, with easy-to-use support for timeout events.

## Example
See [example.cpp](example.cpp). You can compile and run the example by the following commands.

``` bash
cd <project directory>
make
./example
```

and the output will be

``` text
test
hello
end
```

## How to use this
This is a header-only library. You can just copy [messagequeue.hpp](messagequeue.hpp) to your project and #include it in your source files.

To create a message queue:

``` c++
messagequeue<Type> q;
```

To implement an event loop:

``` c++
Type msg;
while(1) {
    msg = q.pop();     // will block until one is available
    process_message(msg);
}
```

To send a message to a queue: (which is probably used in another worker thread, and that thread will be unblocked)

``` c++
q.push(msg);
```

To set a timeout message that will be automatically pushed into the queue after 500 milliseconds:

``` c++
auto timer = q.setTimeout(msg, 500);

// probably cancel the timer before it alarms
q.clearTimeout(timer);
```

## License
MIT License
