#include <vkhr/window.hh>

#include <stdexcept>

namespace vkhr {
    Window::Window(const int width, const int height, const std::string& title,
                   const Image& icon, const bool fullscreen, const bool vsync)
                  : width { width }, height { height }, title { title },
                    vsync { vsync } { // fullscreen setting is set later
        if (!glfwInit()) {
            throw std::runtime_error { "Failed to initialize GLFW 3!" };
        }

        glfwWindowHint(GLFW_RESIZABLE,  GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_VISIBLE,    GLFW_FALSE);

        GLFWmonitor* primary_monitor { glfwGetPrimaryMonitor() };
        const GLFWvidmode* primary_vid_mode { glfwGetVideoMode(primary_monitor) };

        monitor_width  = primary_vid_mode->width;
        monitor_height = primary_vid_mode->height;
        monitor_refresh_rate = primary_vid_mode->refreshRate;

        monitor = primary_monitor;

        // This is needed to create a "windowed" fullscreen window.
        glfwWindowHint(GLFW_RED_BITS,     primary_vid_mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS,   primary_vid_mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS,    primary_vid_mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, monitor_refresh_rate);

        handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!handle) {
            throw std::runtime_error { "Couldn't open the required GLFW window!" };
        }

        set_icon(icon);

        const int monitor_center_x { monitor_width  / 2 - width  / 2 },
                  monitor_center_y { monitor_height / 2 - height / 2 };

        window_x = monitor_center_x;
        window_y = monitor_center_y;

        int monitor_width_in_mm, monitor_height_in_mm;
        glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(),
                                   &monitor_width_in_mm,
                                   &monitor_height_in_mm);

        const float inch_to_mm = 25.4;

        horizontal_dpi = monitor_width  / monitor_width_in_mm  / inch_to_mm;
        vertical_dpi   = monitor_height / monitor_height_in_mm / inch_to_mm;

        glfwSetWindowPos(handle, monitor_center_x, monitor_center_y);
        if (fullscreen) toggle_fullscreen();
        glfwShowWindow(handle);

        float elapsed {  get_current_time()  };
        last_frame_time = fps_update = elapsed;
    }

    Window::~Window() noexcept {
        glfwDestroyWindow(handle);
        handle = nullptr;
    }

    bool Window::is_open() const {
        return !glfwWindowShouldClose(handle);
    }

    void Window::close() {
        glfwSetWindowShouldClose(handle, GLFW_TRUE);
    }

    bool Window::is_fullscreen() const {
        return fullscreen;
    }

    void Window::toggle_fullscreen() {
        if (!fullscreen) {
            glfwSetWindowMonitor(handle, monitor, 0, 0,
                                 monitor_width, monitor_height,
                                 monitor_refresh_rate);
            fullscreen = true;
        } else {
            glfwSetWindowMonitor(handle, nullptr, window_x, window_y,
                                 width, height, monitor_refresh_rate);
            fullscreen = false;
        }
    }

    bool Window::request_vsync() const {
        return vsync;
    }

    int Window::get_width() const {
        if (fullscreen) {
            return monitor_width;
        } else {
            return width;
        }
    }

    float Window::get_aspect_ratio() const {
        return get_width() / static_cast<float>(get_height());
    }

    int Window::get_height() const {
        if (fullscreen) {
            return monitor_height;
        } else {
            return height;
        }
    }

    int Window::get_refresh_rate() const {
        return monitor_refresh_rate;
    }

    const char** Window::get_surface_extensions(unsigned* c) {
        return glfwGetRequiredInstanceExtensions(c);
    }

    VkSurfaceKHR Window::create_surface(VkInstance instance) {
        VkSurfaceKHR surface;

        if (glfwCreateWindowSurface(instance, handle, nullptr, &surface)) {
            throw std::runtime_error { "Failed to create Vulkan surface!" };
        }

        return surface;
    }

    void Window::resize(const int width, const int height) {
        this->width  = width;
        this->height = height;
        glfwSetWindowMonitor(handle, nullptr, window_x, window_y,
                             width, height, monitor_refresh_rate);
        fullscreen = false;
    }

    GLFWwindow* Window::get_handle() {
        return handle;
    }

    void Window::set_icon(const Image& icon) {
        auto corrected_icon = icon;
        corrected_icon.resize(48, 48); // Default size.
        GLFWimage new_icon { corrected_icon.get_width(),
                             corrected_icon.get_height(),
                             corrected_icon.get_data() };
        glfwSetWindowIcon(handle, 1, &new_icon);

    }

    void Window::change_title(const std::string& title) {
        this->title = title; // Copied by GLFW...
        glfwSetWindowTitle(handle, title.c_str());
    }

    void Window::append_string(const std::string& text) {
        append = text;
    }

    void Window::poll_events() {
        float elapsed_time = get_current_time();
        frame_time = elapsed_time - last_frame_time;
        last_frame_time = elapsed_time;

        // i.e. one second has gone, check FPS.
        if (elapsed_time - fps_update >= 1.0) {
            const auto fps = std::to_string(frames);
            const auto fps_text = " @ " + fps + " FPS";

            auto final_title = title + fps_text;
            if (append.size() > 0)
                final_title.append(" – " + append);

            glfwSetWindowTitle(handle, final_title.c_str());

            fps_update = elapsed_time;
            frames = 0;
        }

        glfwPollEvents();
        ++frames;
    }

    float Window::get_vertical_dpi() const {
        return vertical_dpi;
    }

    float Window::get_horizontal_dpi() const {
        return horizontal_dpi;
    }

    void Window::set_time(const float time) {
        glfwSetTime(time);
    }

    float Window::get_current_time() const {
        return static_cast<float>(glfwGetTime());
    }

    float Window::delta_time() const {
        return frame_time;
    }
}