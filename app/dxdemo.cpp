#include "dxdemo.hpp"


int main(int argc, char **argv)
{    
    try
    {
        dxdemo::EventLoop loop;
        dxdemo::DX9Window window("Main Window", 200, 200, 800, 600);
        window.SetPaintTimer(30);

        return loop.Run();
    }
    catch(const std::exception& e)
    {
        std::cout << "Some std::exception was caught." << std::endl;
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unknown Error in DxDemo application !" << std::endl;
    }

    return 111;
}
