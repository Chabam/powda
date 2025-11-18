#include <iostream>
#include <powda/window.hpp>

int main()
{
    powda::Window::init();
    powda::Window main_window{1280, 720, "powda"};
}
