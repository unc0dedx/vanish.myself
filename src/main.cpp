#include <iostream>
#include "storage.h"
#include "job_queue.h"

#include <windows.h>
#include "sciter-x.h"
#include "sciter-x-window.hpp"
#include "sciter-x-host-callback.h"

#include "resources.cpp"

namespace sciter {
    namespace application {
        // dirty little hook to bypass the full winmain setup
        HINSTANCE hinstance() {
            return GetModuleHandle(nullptr);
        }
    }
}

class MainWindow : public sciter::window {
public:
    MainWindow() : window(SW_TITLEBAR | SW_RESIZEABLE | SW_CONTROLS | SW_MAIN | SW_ENABLE_DEBUG) {}
};

int main(int argc, char* argv[]) {
    std::cout << "Starting vanishing.persona core..." << std::endl;

    // setup encrypted local storage.
    // relying on Windows DPAPI so we don't have to juggle raw keys ourselves
    vanish::storage::EncryptedStore store("default_password");
    std::cout << "DPAPI Encrypted JSON Storage initialized." << std::endl;

    // spin up the job queue. 
    // using 2 workers and a 1s rate limit. that's usually enough to keep most apis happy without getting banned
    vanish::core::JobQueue queue(2, 1000);
    std::cout << "Thread-safe Rate-Limited Job queue initialized." << std::endl;

    // init the sciter console for debugging
    sciter::debug_output_console console;
    
    // unpack our embedded html/css payload
    sciter::archive::instance().open(resources, sizeof(resources));

    // build the main window and load the app interface
    MainWindow* pwin = new MainWindow();
    pwin->load(WSTR("this://app/index.htm"));
    
    pwin->expand();
    
    // standard win32 message pump to keep the ui thread alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    queue.stop();
    return 0;
}
