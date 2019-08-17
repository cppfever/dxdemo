#include <vgui/config.hpp>
#include <vgui/config-check.hpp>
#include <vgui/3dparty.hpp>
#include <vgui/Exception.hpp>
#include <vgui/def.hpp>

//NanoVG include's must be here, otherwise multiple definitions error will occur
#include <nanovg.h>
#include <nanovg_gl.h>

namespace vgui
{

//https://eb2.co/blog/2014/04/c--14-and-sdl2-managing-resources/
template<typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
{
    auto r = c(std::forward<Arguments>(args)...);

    if(!r)
        throw ExceptionInfo << ("vgui::make_resource error.");

    return std::shared_ptr<std::decay_t<decltype(*r)>>(r, d);
}


VGContext_p make_NanoVG2()
{
    return vgui::make_resource(nvgCreateGL2, nvgDeleteGL2,
                               NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

#ifdef _WIN32

std::wstring Utf8ToWstring(const char* str)
{
    size_t length = std::strlen(str);

    if(length != 0)
    {
        int num_chars = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, length, NULL, 0);
        std::wstring wstrTo;
        if(num_chars)
        {
            wstrTo.resize(num_chars);
            if(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, length, &wstrTo[0], num_chars))
            {
                return wstrTo;
            }
        }
    }

    return std::wstring();
}

#endif//_WIN32

}//namespace vgui

