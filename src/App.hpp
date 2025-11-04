#ifndef APP_HPP
#define APP_HPP

class App {
public:
    App(int argc, char* argv[]);

    void run();
private:
    void init();
    void mainLoop();
    void cleanup();
};

#endif