#include "include/ImGUI/imgui.h"
#include "include/ImGUI/backends/imgui_impl_glfw.h"
#include "include/ImGUI/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>
#include <ctime>
#include <curl/curl.h>
#include "Client.h"
#include <fstream>

#include "JSONHelper.h"
namespace fs = std::filesystem;
#include <filesystem>
time_t fresh_time = time(NULL);
time_t last_time = fresh_time;
#define GL_SILENCE_DEPRECATION
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1290
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

auto selected_json_path = std::string();
std::ifstream f;
json edit_file = nullptr;

void render_edit_window(bool &show_edit_window);

void displayFilesDirectory(const std::string& dirPath, bool &show_edit_window) {
    // Loop through the files in the directory
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {

            std::string fileName = entry.path().filename().string();

            ImGui::Text("%s", fileName.c_str());
            ImGui::SameLine(0, 30);  // 10 is the spacing between buttons

            if (ImGui::Button("Edit")) {
                std::cout << "Edit button clicked for: " << fileName << std::endl;
                selected_json_path = entry.path();
                show_edit_window = !show_edit_window;
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                std::cout << "Delete button clicked for: " << fileName << std::endl;
            }

            ImGui::Spacing();

        }
    }
}

// Main code
int main(int, char**)
{

    std::shared_ptr<Client> client = std::make_shared<Client>();

    if (client->connect()) {
        client->connected = true;
    }
    std::string dirPath = fs::current_path().parent_path().string() + "/avtomobili";
    // TODO: Generate the directory if it does not exist!
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    int serverCheckDelay = 5;
    bool show_edit_window = false;

    JSONHelper jsonHelper;


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        time(&fresh_time);
        time_t calculated_time = fresh_time - last_time;

        if (calculated_time > serverCheckDelay) {
            last_time = fresh_time;
            if (client->GetServerStatus()) {
                std::cout << "Server online" << std::endl;
            }
        }
        // Create a window called "My First Tool", with a menu bar.
        if (show_edit_window) {
            render_edit_window(show_edit_window);
        }
        ImGui::Begin("ImGui Bot", nullptr, ImGuiWindowFlags_MenuBar);
        ImGui::BeginMenuBar();
        if (ImGui::BeginMenu("Datoteka")) {
            if (ImGui::MenuItem("Uvozi datoteko")) {

            }
            if (ImGui::MenuItem("Ustvari nov vnos")) {

            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();

        displayFilesDirectory(dirPath, show_edit_window);
        // Footer separator
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Footer section
        float footer_height = 30.0f;  // Adjust height as needed
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footer_height - ImGui::GetStyle().WindowPadding.y);
        ImGui::BeginChild("Footer", ImVec2(0, footer_height), true, ImGuiWindowFlags_NoScrollbar);

        // Footer content
        ImGui::TextColored(client->getConnectionColor(client->connection_status),client->GetStatus(client->connection_status).c_str());
        ImGui::SameLine(ImGui::GetWindowWidth() - 100);

        ImGui::EndChild();
        ImGui::End();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void render_edit_window(bool &show_edit_window) {
    if (!f.is_open()) {  // Check if the file stream is uninitialized (not open)
        f.open(selected_json_path);

        if (!f) {  // Check if the file was successfully opened
            std::cerr << "Failed to open file: " << selected_json_path << std::endl;
        }
        edit_file = json::parse(f);


    }
    ImGui::Begin("Urejanje vnosa", nullptr, ImGuiWindowFlags_MenuBar);
    ImGui::Spacing();

    size_t index = 0;
    for (const auto& entry : edit_file[0].items()) {

    }

    ImGui::Spacing();
    ImGui::Separator();


    // Footer separator
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Footer section
    float footer_height = 30.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footer_height - ImGui::GetStyle().WindowPadding.y);
    ImGui::BeginChild("Footer", ImVec2(0, footer_height), true, ImGuiWindowFlags_NoScrollbar);

    // Footer content
    ImGui::Text(show_edit_window ? "Edit window turned on" : "Edit window turned off");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    // Close button
    if (ImGui::Button("Zapri"))
        show_edit_window = !show_edit_window;
        f.close();
    ImGui::EndChild();
    ImGui::End();
}
