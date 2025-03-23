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
#include "Car.h"
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
Car edit_car;
int selected_tab_edit = 0;

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
    if (ImGui::BeginTabBar("basic")) {
        // Tab: Splosne informacije
        if (ImGui::TabItemButton("Splosne informacije")) {
            selected_tab_edit = 0;
        }

        // Tab: Informacije
        if (ImGui::TabItemButton("Informacije")) {
           selected_tab_edit = 1;
        }

        // Tab: Slike
        if (ImGui::TabItemButton("Slike")) {
            selected_tab_edit = 2;
        }

        ImGui::EndTabBar();
    }
    if (selected_tab_edit == 0) {
        // Content for "Splosne informacije" tab
        ImGui::InputText("Znamka", edit_car.znamka_buffer, sizeof(edit_car.znamka_buffer));
        ImGui::InputText("Model", edit_car.model_buffer, sizeof(edit_car.model_buffer));
        ImGui::Spacing();

        if (ImGui::Combo("Izberi obliko", &edit_car.selected_karoserijska_oblika, edit_car.karoserijska_oblika, IM_ARRAYSIZE(edit_car.karoserijska_oblika))) {
            // Combo action for "Izberi obliko" dropdown
        }
        ImGui::InputText("Letnik prve registracije", edit_car.letnik_registracije_buffer, sizeof(edit_car.letnik_registracije_buffer));
        ImGui::InputText("Mesec prve registracije", edit_car.mesec_registracije_buffer, sizeof(edit_car.mesec_registracije_buffer));

        ImGui::Spacing();
        if (ImGui::Combo("Izberi gorivo", &edit_car.selected_gorivo, edit_car.gorivo, IM_ARRAYSIZE(edit_car.gorivo))) {
            // Combo action for "Izberi gorivo" dropdown
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::InputText("Znamka vozila", edit_car.znamka_vozila_buffer, sizeof(edit_car.znamka_vozila_buffer));
        ImGui::InputText("Model vozila", edit_car.model_vozila_buffer, sizeof(edit_car.model_vozila_buffer));
        ImGui::InputText("Izbira na avto net", edit_car.index_to_select, sizeof(edit_car.index_to_select));
    } else if (selected_tab_edit == 1) {
        ImGui::InputText("VIN stevilka", edit_car.vin_buffer, IM_ARRAYSIZE(edit_car.vin_buffer)); // ✅
        ImGui::Checkbox("Objavi VIN", &edit_car.objavi_vin); // ✅
        if (ImGui::Combo("Menjalnik", &edit_car.selected_menjalnik, edit_car.menjalnikOptions, IM_ARRAYSIZE(edit_car.menjalnikOptions))) {}
        ImGui::InputText("Leto registracije", edit_car.letoReg, IM_ARRAYSIZE(edit_car.letoReg));
        ImGui::InputText("Prevozeni km", edit_car.prevozenikm, IM_ARRAYSIZE(edit_car.prevozenikm));

        ImGui::InputText("Najem", edit_car.najem, IM_ARRAYSIZE(edit_car.najem)); // ✅
        ImGui::InputText("Cena", edit_car.cena, IM_ARRAYSIZE(edit_car.cena)); // ✅

        ImGui::Checkbox("Gotovinski popust", &edit_car.gotpopust);
        ImGui::Checkbox("Zadnja cena", &edit_car.zadcena);
        ImGui::Checkbox("Prevzem leasinga", &edit_car.prevzemleas);
        ImGui::Checkbox("Menjava", &edit_car.menjava);
        ImGui::Checkbox("Vozno", &edit_car.vozno);
        ImGui::Checkbox("Poškodovan", &edit_car.poskodovan);
        ImGui::Checkbox("Karambolirano", &edit_car.karambolirano);
        ImGui::Checkbox("Okvara", &edit_car.okvara);
        ImGui::Checkbox("Poplavljeno", &edit_car.poplavljeno);

        ImGui::Text("Zunanjost");
        if (ImGui::Combo("Barva", &edit_car.selected_barva, edit_car.barve, IM_ARRAYSIZE(edit_car.barve))) {}
        ImGui::Checkbox("Kovinska barva", &edit_car.kovinska);
        if (ImGui::Combo("Streha", &edit_car.selected_streha, edit_car.streha_options, IM_ARRAYSIZE(edit_car.streha_options))) {}
        ImGui::InputText("Stevilo vrat", edit_car.vrata, IM_ARRAYSIZE(edit_car.vrata));

        ImGui::Text("Notranjost");
        if (ImGui::Combo("Oblazinjenje", &edit_car.selected_oblazinjenje, edit_car.oblazinjenje_options, IM_ARRAYSIZE(edit_car.oblazinjenje_options))) {}
        ImGui::InputText("Barva oblazinjenja", edit_car.barvaoblazin, IM_ARRAYSIZE(edit_car.barvaoblazin));
        if (ImGui::Combo("Stevilo sedezev", &edit_car.selected_sedezev, edit_car.sedezev_options, IM_ARRAYSIZE(edit_car.sedezev_options))) {}

        ImGui::Text("Varnost");
        ImGui::Checkbox("ABS zavore", &edit_car.ABSzavore);
        ImGui::Checkbox("ESP", &edit_car.ESP);
        ImGui::Checkbox("ASR", &edit_car.ASR);
        ImGui::Checkbox("Nadzor tlaka", &edit_car.nadzortlaka);
        ImGui::Checkbox("Xenon luci", &edit_car.xenon);
        ImGui::Checkbox("LED spredaj", &edit_car.FLedFro);
        ImGui::Checkbox("Senzor za dez", &edit_car.senzordez);
        ImGui::Checkbox("Alarm", &edit_car.alarm);
        ImGui::Checkbox("Samodejno zaviranje", &edit_car.FSamZav);
        ImGui::Checkbox("Klimatska naprava", &edit_car.klima);
        ImGui::Checkbox("Avtomatska klima", &edit_car.aklima);
        ImGui::Checkbox("Webasto", &edit_car.webasto);
        ImGui::Checkbox("Tonirana stekla", &edit_car.tonirana);
        ImGui::Checkbox("El. stekla spredaj", &edit_car.estekla2);
        ImGui::Checkbox("El. ogledala", &edit_car.eogledala);
        ImGui::Checkbox("Gretje ogledal", &edit_car.gretjeogledal);
        ImGui::Checkbox("Servo volan", &edit_car.servo);
        ImGui::Checkbox("Usnjen volan", &edit_car.usnjenvolan);
        ImGui::Checkbox("Multifunkcijski volan", &edit_car.multivolan);
        ImGui::Checkbox("Tempomat", &edit_car.tempomat);

        ImGui::Text("Multimedija in povezljivost");
        ImGui::Checkbox("Avtoradio", &edit_car.avtoradio); // ✅
        ImGui::InputText("Avtoradio opis", edit_car.avtoradioopis, IM_ARRAYSIZE(edit_car.avtoradioopis)); // ✅
        ImGui::Checkbox("USB prikljucek", &edit_car.USB);
        ImGui::Checkbox("Racunalnik", &edit_car.racunalnik); // ✅
        ImGui::Checkbox("Komunikacijski paket", &edit_car.komunikacijskipaket);
        ImGui::Checkbox("Navigacija", &edit_car.navigacija); // ✅
        // fali še: Navigacija + touchscreen , avtotelefon,predpriprava za mobilni telefon
        /*
        CD izmenjevalnik / strežnik
        MP3 predvajalnik
        DVD predvajalnik
        trdi disk za shranjevanje podatkov
        USB priključek (iPod, HD, ...)
        TV sprejemnik / tuner
        Bluetooth vmesnik
        digitalni radio DAB
        */

        ImGui::Checkbox("Touch screen", &edit_car.FTouScre); // ✅
        ImGui::Checkbox("Android Auto", &edit_car.AndroidAuto); // ✅

        ImGui::Text("Dodatna oprema");
        ImGui::Checkbox("Deljiva zadnja klop (1/2)", &edit_car.deljivaklop12); // ✅
        ImGui::Checkbox("Deljiva zadnja klop (1/3)", &edit_car.deljivaklop13); // ✅
        // Fali še : deljiva zad.klop 1/3-1/3-1/3, ntegrirani otroški sedež, rolo prtljažnega prostora, navodila za uporabo v SLO jeziku,dvojno dno prtljažnika,strešne sani
        // pomoč pri speljevanju v klanec, pomoč pri parkiranju: kamera, pomoč pri parkiranju: prednji senzorji, pomoč pri parkiranju: zadnji senzorji, pomoč pri parkiranju: pogled 360 stopinj
        // vzvratna kamera, bočne stopnice, vlečna kljuka, vozilo prilagojeno invalidu
        ImGui::Checkbox("Isofix", &edit_car.Isofix); // ✅
        ImGui::Checkbox("Ski bag", &edit_car.skibag); // ✅
        ImGui::Checkbox("Mreza prtljaznika", &edit_car.mreza); // ✅
        ImGui::Checkbox("FS sistem parkiranja", &edit_car.FSisPar);  // ✅
        ImGui::Checkbox("PDC", &edit_car.PDC); // ✅
        ImGui::Checkbox("Parkirna kamera", &edit_car.FParKam); // ✅

        ImGui::Text("Ostalo");
        ImGui::InputTextMultiline("Opombe", edit_car.opombe, IM_ARRAYSIZE(edit_car.opombe));
        ImGui::Checkbox("Na zalogi", &edit_car.nazalogi);
    } else if (selected_tab_edit == 2) {
        ImGui::Text("This tab is for images.");
    }
    ImGui::Spacing();
    if (ImGui::Button("Zapri"))
        show_edit_window = !show_edit_window;
    f.close();
    if (ImGui::Button("Shrani"))
        // TODO: Implement saving this
        show_edit_window = !show_edit_window;
    f.close();
    ImGui::End();
}
