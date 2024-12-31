#define SOKOL_IMPL
#define SOKOL_GLCORE // using opengl
#define FONTSTASH_IMPLEMENTATION
#define SOKOL_IMGUI_NO_SOKOL_APP 
#include <cstdio>
#include <cstdlib>
#include "fontstash/fontstash.h"
#include "sokol/sokol_gfx.h"
#include "sokol/util/sokol_gl.h"
#include "sokol/util/sokol_fontstash.h"
#include "sokol/sokol_fetch.h"


//#include <imgui.h>
//#include "sokol/util/sokol_imgui.h"

#include <print>
#include <string>
#include <cmath>
#include <GLFW/glfw3.h>

void basicLogger(const char* tag, uint32_t log_level, uint32_t log_item_id, const char* message, uint32_t line, const char* filename, void* data) {
    std::string level;

    switch (log_level) {
        case 0:
            level = "panic";
            break;
        case 1:
            level = "error";
            break;
        case 2:
            level = "warning";
            break;
        case 3:
            level = "info";
            break;
        default:
            level = "unknown";
    }

    std::println("{} ({}): {} @ {}:{} ({}) {}",
        level,
        tag,
        message,
        filename,
        line,
        log_item_id,
        data
    );
}

FONScontext* fons;
float dpi = 96.0f; // TODO: Make this actually find the dpi, glfw has some functions for it
int font;
uint8_t font_data[256 * 1024];

const char* fileutil_get_path(const char* filename, char* buf, size_t buf_size) {
    snprintf(buf, buf_size, "%s", filename);
    return buf;
}

void onFontLoad(const sfetch_response_t* response) {
    if (response->fetched) {
        std::println("fetch successful");
        font = fonsAddFontMem(fons, "sans", (unsigned char*)response->data.ptr, (int)response->data.size, false);
    } else {
        std::println("{}", static_cast<int>(response->error_code));
    }
}

int main() {
    constexpr int width = 1280;
    constexpr int height = 720;
    const std::string title = "SandboxGame";

    glfwInit();
    
    // OpenGL 4.1 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    sg_desc desc = {0};
    desc.environment = {
        .defaults = {
            .color_format = SG_PIXELFORMAT_RGBA8,
            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
            .sample_count = 1
        }
    };

    desc.logger.func = basicLogger;

    sg_setup(desc);

    if (sg_isvalid()) {
        std::println("SokolGfx: Setup was valid, continuing...");
    }

    sgl_desc_t gl_desc = {
        .logger = basicLogger
    };

    sgl_setup(gl_desc);

   // simgui_desc_t imgui_desc = {
   //     .logger = {
   //         .func = basicLogger
   //     }
   // };

   // simgui_setup(imgui_desc);

    // font stuff

    auto round_pow2 = [](float v) -> int {
        uint32_t vi = ((uint32_t) v) - 1;
        for (uint32_t i = 0; i < 5; i++) {
            vi |= (vi >> (1<<i));
        }
        return (int) (vi + 1);
    };

    const int dim = round_pow2(1024.0f * dpi);

    sfons_desc_t fons_desc = {
        .width = 1024,
        .height = 1024
    };

    fons = sfons_create(&fons_desc);
    font = FONS_INVALID;

    fonsSetErrorCallback(fons, [](void* uptr, int error, int val) {
        std::println("FONS error: {}, value: {}", error, val);
    }, nullptr);


    sfetch_desc_t fetch_desc = {
        .num_channels = 1,
        .num_lanes = 4,
        .logger = {
            .func = basicLogger
        }
    };

    sfetch_setup(fetch_desc);

    char pathbuf[2048];

    sfetch_request_t fetch_request = {
        .path = fileutil_get_path("droid.ttf", pathbuf, sizeof(pathbuf)),
        .callback = onFontLoad,
        .buffer = SFETCH_RANGE(font_data)
    };

    sfetch_send(fetch_request);

    sg_pass_action pass_action{};
    pass_action.colors[0] = {
        .load_action = SG_LOADACTION_CLEAR,
        .clear_value = {0.18f, 0.21f, 0.5f, 1.0f}
    };

    sg_pass pass = {
        .action = pass_action,
        .swapchain = {
            .width = width,
            .height = height, 
            .sample_count = 1,
            .color_format = SG_PIXELFORMAT_RGBA8,
            .depth_format = SG_PIXELFORMAT_DEPTH_STENCIL,
            .gl = {
                .framebuffer = 0
            }
        }
    };

    uint64_t frame = 0;

    while (!glfwWindowShouldClose(window)) {
        sfetch_dowork();
        //simgui_new_frame({
        //    width, 
        //    height, 
        //    1.f / 60.f, // TODO: Get actual frame time, rn im assuimg a constant 60fps
        //    dpi
       // });

       // ImGui::ShowDemoWindow();
        fonsClearState(fons);
        
        sgl_defaults();
        sgl_matrix_mode_projection();

        // Here is my understanding of glOrtho

        /*
        *
        * near and far are z cutoff (all geom needs to be inbetween them, or else it won't be rendered)
        * left and right, are minimum and maximum x 
        * bottom and top is minimum and maximum y
        */
        sgl_ortho(0.0f, 1280.f, 720.f, 0.f, -1.0f, 1.0f); 
        fonsSetFont(fons, font);
        fonsSetSize(fons, 24.f);

        constexpr float start_x = 375.f;
        float x = start_x;
        float y = 150.f;  

        float line_height;
        fonsVertMetrics(fons, nullptr, nullptr, &line_height);
        
        fonsSetColor(fons, sfons_rgba(255, 0, 0, 255));
        x = fonsDrawText(fons, x, y, "red", nullptr);

        fonsSetColor(fons, sfons_rgba(255, 165, 0, 255));
        x = fonsDrawText(fons, x, y, "orange", nullptr);

        fonsSetColor(fons, sfons_rgba(255, 255, 0, 255));
        x = fonsDrawText(fons, x, y, "yellow", nullptr);

        fonsSetColor(fons, sfons_rgba(0, 255, 0, 255));
        x = fonsDrawText(fons, x, y, "green", nullptr);

        fonsSetColor(fons, sfons_rgba(0, 0, 255, 255));
        x = fonsDrawText(fons, x, y, "blue", nullptr);

        fonsSetColor(fons, sfons_rgba(128, 0, 128, 255));
        x = fonsDrawText(fons, x, y, "purple", nullptr);

        fonsSetColor(fons, sfons_rgba(75, 0, 130, 255));
        x = fonsDrawText(fons, x, y, "indigo", nullptr);

        fonsSetColor(fons, sfons_rgba(238, 130, 238, 255));
        x = fonsDrawText(fons, x, y, "violet", nullptr);

        x = start_x; // reset to line start
        y += line_height;

        fonsSetColor(fons, sfons_rgba(255, 255, 255, 255));
        x = fonsDrawText(fons, x, y, "this should be white text on another line", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(31, 31, 31, 255));
        x = fonsDrawText(fons, x, y, "Line 1", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(63, 63, 63, 255));
        x = fonsDrawText(fons, x, y, "Line 2", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(95, 95, 95, 255));
        x = fonsDrawText(fons, x, y, "Line 3", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(127, 127, 127, 255));
        x = fonsDrawText(fons, x, y, "Line 4", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(159, 159, 159, 255));
        x = fonsDrawText(fons, x, y, "Line 5", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(191, 191, 191, 255));
        x = fonsDrawText(fons, x, y, "Line 6", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(223, 223, 223, 255));
        x = fonsDrawText(fons, x, y, "Line 7", nullptr);

        x = start_x; 
        y += line_height;

        fonsSetColor(fons, sfons_rgba(255, 255, 255, 255));
        x = fonsDrawText(fons, x, y, "Line 8", nullptr);

        sfons_flush(fons);

        sg_begin_pass(pass);

        sgl_begin_triangles();

        sgl_v2f_c3f(50.0f, 100.f, 1.0f, 0.0f, 0.0f);
        sgl_v2f_c3f(100.f, 0.f, 0.0f, 1.0f, 0.0f);
        sgl_v2f_c3f(0.f, 0.f, 0.0f, 0.0f, 1.0f);

        sgl_end();

        sgl_draw();

        //simgui_render();

        sg_end_pass();
        sg_commit();
        glfwSwapBuffers(window);
        glfwPollEvents();

        frame++;
    }

    sfetch_shutdown();
    sfons_destroy(fons);
    sgl_shutdown();
    //simgui_shutdown();
    sg_shutdown();
    glfwTerminate();

    return 0;

}