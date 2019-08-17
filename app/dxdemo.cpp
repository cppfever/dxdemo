#include "dxdemo.hpp"


int main(int argc, char **argv)
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{    
    try
    {
        dxdemo::EventLoop loop;
        dxdemo::DX9Window window("Main ok Window", 300, 400, 800, 600);

        return loop.Run();
    }
    catch(const dxdemo::Exception& e)
    {
        e.Print();
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

    return 777;
}
