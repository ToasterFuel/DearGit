#include "Window.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "TextureHolder.h"
#include <iostream>

const int NAVIGATION_SIZE = 50;
const int MINIMUM_WIDTH = 800;
const int MINIMUM_HEIGHT = 600;

bool Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(MINIMUM_WIDTH, MINIMUM_HEIGHT, "Dear Git", NULL, NULL);
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, MINIMUM_WIDTH, MINIMUM_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0;
    ImGui::StyleColorsDark();
    if(git_libgit2_init() <= 0)
    {
        std::cout << "Failed to initialize libgit2\n";
        return false;
    }
    onlyRepo = Repo("D:\\Workspace\\tmp\\");

    if(!TextureHolder::Instance().Init())
    {
        std::cout << "Failed initializing textures\n";
        return false;
    }
    if(!onlyRepo.Init())
    {
        std::cout << "Failed initializing the repo\n";
        return false;
    }
    if(!localRegion.Init(this, &onlyRepo))
    {
        std::cout << "Failed initializing local region\n";
        return false;
    }

    return true;
}

bool Window::IsKeyPressed(int keyCode)
{
    return glfwGetKey(window, keyCode) == GLFW_PRESS;
}

bool Window::ContinueRunning()
{
    return !glfwWindowShouldClose(window);
}

void Window::DrawFrame()
{
    glfwPollEvents();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Main", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove);

    ImGui::SetWindowSize(ImVec2(width, height));
    ImGui::SetWindowPos(ImVec2(0, 0));
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    DrawNavigation(NAVIGATION_SIZE);
    ImGui::BeginTabBar("ReposTabBar", ImGuiTabBarFlags_Reorderable |
            ImGuiTabBarFlags_TabListPopupButton |
            ImGuiTabBarFlags_FittingPolicyScroll);
    static bool firstRepo = true;
    if(firstRepo && ImGui::BeginTabItem("First Repo", &firstRepo))
        ImGui::EndTabItem();
    ImGui::EndTabBar();
    int regionHeight = ImGui::GetWindowHeight() - ImGui::GetFrameHeight() * 4 - NAVIGATION_SIZE;
    localRegion.DrawRegion(width, regionHeight);
    ImGui::End();

    ImGui::Render();
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Window::DrawNavigation(int size)
{
    ImGui::BeginGroup();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8);
    ImGui::Button("Clone", ImVec2(size, size));
    ImGui::SameLine();
    ImGui::Button("Local", ImVec2(size, size));
    ImGui::SameLine();
    ImGui::Button("Commits", ImVec2(size, size));
    ImGui::SameLine();
    ImGui::Button("Branches", ImVec2(size, size));
    ImGui::PopStyleVar();
    ImGui::EndGroup();
}

void Window::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}