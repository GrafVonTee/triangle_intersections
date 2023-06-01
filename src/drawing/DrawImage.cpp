#include <iomanip>
#include <sstream>
#include "DrawImage.h"
#include "GetImVecFromPolygon.h"
#include "ConstantsForDrawing.h"
#include "imgui_demo.cpp"
#include "CalculateIntersections.h"


namespace DrawOutput {
    void draw_triangles_and_intersection(Geometry::Polygon &tr1,
                                         Geometry::Polygon &tr2,
                                         Geometry::Intersection &intersection) {

        #if defined(IMGUI_IMPL_OPENGL_ES2)
                const char* glsl_version = "#version 100";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        #elif defined(__APPLE__)
                const char *glsl_version = "#version 120";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #else
                const char* glsl_version = "#version 130";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // Initialize GLFW
        glfwInit();

        // Create a GLFW window
        GLFWwindow *window = glfwCreateWindow((int) DrawConst::WINDOW_WIDTH, (int) DrawConst::WINDOWS_HEIGHT,
                                              "Triangle Intersection",
                                              nullptr, nullptr);

        // Make the OpenGL context current
        glfwMakeContextCurrent(window);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        // Initialize GLAD
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.DisplaySize = ImVec2(DrawConst::DISPLAY_SIZE, DrawConst::DISPLAY_SIZE);
        io.DisplayFramebufferScale = ImVec2(DrawConst::DISPLAY_SCALE, DrawConst::DISPLAY_SCALE);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // create a new window
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();                   

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            // Set ImGui window properties
            ImGui::SetNextWindowPos(ImVec2(DrawConst::WINDOW_WIDTH / 3, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(2 * DrawConst::WINDOW_WIDTH / 3, DrawConst::WINDOWS_HEIGHT), ImGuiCond_Once);

            ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
            {
                ImDrawList *drawList = ImGui::GetWindowDrawList();
                ImVec2 pos = ImGui::GetCursorScreenPos();

                DrawUtils::scalingParameters parameters = DrawUtils::findParameters(tr1, tr2, DrawConst::SQUARE_SIDE_SIZE);

                DrawPolygon(drawList, tr1, parameters, pos, RED_COLOR);
                DrawPolygon(drawList, tr2, parameters, pos, GREEN_COLOR);
                DrawPolygon(drawList, intersection.polygon, parameters, pos, YELLOW_COLOR);

                for (const Geometry::Polygon& figurePtr : {tr1, tr2, intersection.polygon})
                    DrawPoints(drawList, figurePtr, parameters, pos, WHITE_COLOR);
            }
            ImGui::End();

            // Set ImGui window properties
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(DrawConst::WINDOW_WIDTH / 3, DrawConst::WINDOWS_HEIGHT), ImGuiCond_Once);
            
            ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
            {
                DisplayPolygon(tr1, "Polygon 1");
                DisplayPolygon(tr2, "Polygon 2");
                intersection = Math::findPolygonsInter(tr1, tr2);
                DrawUtils::setActualPointsLabels(tr1, tr2, intersection);                
                DisplayPolygon(intersection.polygon, "Intersection", true);
            }
            ImGui::End();

            // Rendering
            ImGui::Render();
            int displayW, displayH;
            glfwGetFramebufferSize(window, &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }

        ImGui_ImplGlfw_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();

        // Shutdown GLFW
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DrawPoints(
        ImDrawList *drawList,
        const Geometry::Polygon& polygon, 
        const DrawUtils::scalingParameters& parameters, 
        const ImVec2& offset, 
        const ImU32& col)
    {
        Geometry::Polygon drawnPolygon = DrawUtils::scaleAndTranslate(polygon, parameters);
        const std::vector<Geometry::Point>& points = drawnPolygon.getPointsRef();                
        for (const Geometry::Point& point : points) {
            ImVec2 relativePoint = ImVec2(offset.x + (float) point.getX(), offset.y + (float) point.getY());
            drawList->AddCircleFilled(
                relativePoint,
                DrawConst::POINT_SIZE,
                col
            );
            drawList->AddText(
                nullptr, 
                DrawConst::LETTER_FONT_SIZE, 
                relativePoint,
                col,
                (" " + point.getLabel()).c_str()
            );
        }
    }

    void DrawPolygon(
        ImDrawList *drawList,
        const Geometry::Polygon& polygon, 
        const DrawUtils::scalingParameters& parameters, 
        const ImVec2& offset, 
        const ImU32& col)
    {
        Geometry::Polygon drawnPolygon = DrawUtils::scaleAndTranslate(polygon, parameters);
        auto polygon_points = DrawUtils::getVectorOfPointsFromPolygon(drawnPolygon, offset);
        // draw the intersection
        if (polygon_points.size() >= 3)
            drawList->AddConvexPolyFilled(
                &polygon_points[0],
                (int) polygon_points.size(),
                col
            );

        else if (polygon_points.size() == 2)
            drawList->AddLine(
                polygon_points[0],
                polygon_points[1],
                col,
                DrawConst::LINE_THICKNESS
            );

        else if (polygon_points.size() == 1)
            drawList->AddCircleFilled(
                polygon_points[0], 
                DrawConst::INTERSECTION_POINT_SIZE, 
                col
            );
    }

    void DisplayPolygon(Geometry::Polygon &polygon, const std::string& title, bool muted) {
        std::vector<Geometry::Point> &points1 = polygon.getPointsRef();
        if (muted) points1 = polygon.getPointsRef();
        ImGui::Text(title.c_str());

        for (Geometry::Point& point : points1) {   
            DisplayPoint(point, muted);                    
            polygon.sortPoints();
        }

        if (!muted) {
            DisplayAddButton(polygon);            
            ImGui::SameLine();
            DisplayDeleteButton(polygon);
        }
    }

    void DisplayAddButton(Geometry::Polygon& polygon) {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));

        char labelNumber = polygon.getPointsRef().front().getLabel().front();
        std::string name = std::string("Add point for ") + labelNumber;

        if (ImGui::Button(name.c_str())) {
            Geometry::Point front = polygon.getPointsRef().front();
            Geometry::Point back = polygon.getPointsRef().back();
            float offset = pow((pow(front.getX() - back.getX(), 2) + pow(front.getX() - back.getX(), 2)), 0.5);
            Geometry::Point newPoint = Geometry::Point((front.getX() + back.getX())/2 + offset/5, (front.getY() + back.getY())/2);
            polygon.emplaceBack(newPoint);
        }
        ImGui::PopStyleColor(3);
    }

    void DisplayDeleteButton(Geometry::Polygon& polygon) {
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));

        std::string label = polygon.getPointsRef().front().getLabel();
        label.pop_back();
        std::string name = "Delete point from " + label;

        if (ImGui::Button(name.c_str()))
            polygon.popBack();

        ImGui::PopStyleColor(3);
    }

    void DisplayPoint(Geometry::Point &point, bool muted) {
        int readonly = 0;
        if (muted) readonly = ImGuiInputTextFlags_ReadOnly;

        char x_buffer[DrawConst::BUFFER_SIZE];
        char y_buffer[DrawConst::BUFFER_SIZE];

        std::stringstream stream1;
        std::stringstream stream2;
        stream1 << std::fixed << std::setprecision(2) << point.getX();
        stream2 << std::fixed << std::setprecision(2) << point.getY();
        std::string s1 = stream1.str();
        std::string s2 = stream2.str();
        
        strcpy(x_buffer, s1.c_str());
        strcpy(y_buffer, s2.c_str());

        std::string prefix = (muted) ? "  " : " ";
        std::string name = prefix + point.getLabel() + ".x";

        
        ImGui::PushItemWidth(150);

        bool modified = false;

        modified = ImGui::InputText(name.c_str(), x_buffer, DrawConst::BUFFER_SIZE,
                            ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | readonly);

        if (!muted && modified) point.setX(atof(x_buffer));

        ImGui::SameLine();
        name = point.getLabel() + ".y";
        modified = ImGui::InputText(name.c_str(), y_buffer, DrawConst::BUFFER_SIZE,
                            ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | readonly);

        if (!muted && modified) point.setY(atof(y_buffer));
    }
}