#include <cmath>
#include <iostream>
#include <algorithm>
#include "../Driver/UTILS/skCrypt.h"
#include "../Lib/imgui/imgui.h"

bool containsString(const std::string& str, const std::string& find) {
    return str.find(find) != std::string::npos;
}

ImVec4 Lerp(const ImVec4& a, const ImVec4& b, float t) {
    return ImVec4(
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    );
}

inline float DegToRad(float degrees) {
    return degrees * (3.14159265358979323846f / 180.0f);
}

ImVec4 ImColorToFloat4(const ImColor& color) {
    float out[4];
    out[0] = color.Value.x;
    out[1] = color.Value.y;
    out[2] = color.Value.z;
    out[3] = color.Value.w;
    return (
        ImVec4(out[0], out[1], out[2], out[3])
        );
}

ImColor Float4ToImColor(const float in[4]) {
    int r = static_cast<int>(in[0] * 255.0f);
    int g = static_cast<int>(in[1] * 255.0f);
    int b = static_cast<int>(in[2] * 255.0f);
    int a = static_cast<int>(in[3] * 255.0f);
    return ImColor(r, g, b, a);
}

namespace overlay {
    bool DrawBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor outlineColor) {
        if (!drawList)
            return false;

        drawList->AddRect(startPos, endPos, outlineColor);
        return true;

    }
    bool DrawFilledBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor outlineColor, ImColor fillColor) {
        if (!drawList)
            return false;

        drawList->AddRectFilled(startPos, endPos, fillColor);
        drawList->AddRect(startPos, endPos, outlineColor);
        return true;
    }
    bool DrawGradientBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor TopColor, ImColor BottomColor, ImColor outlineColor) {
        if (!drawList)
            return false;

        int gradientSteps = 100;
        float stepHeight = (endPos.y - startPos.y) / gradientSteps;

        for (int i = 0; i < gradientSteps; ++i) {
            float t = static_cast<float>(i) / gradientSteps;
            ImVec2 lineStart = ImVec2(startPos.x, startPos.y + stepHeight * i);
            ImVec2 lineEnd = ImVec2(endPos.x, startPos.y + stepHeight * (i + 1));

            ImColor currentColor = ImColor(
                TopColor.Value.x * (1.0f - t) + BottomColor.Value.x * t,
                TopColor.Value.y * (1.0f - t) + BottomColor.Value.y * t,
                TopColor.Value.z * (1.0f - t) + BottomColor.Value.z * t,
                TopColor.Value.w * (1.0f - t) + BottomColor.Value.w * t
            );

            drawList->AddRectFilled(lineStart, lineEnd, currentColor);
        }

        drawList->AddRect(startPos, endPos, outlineColor);
        return true;
    }

    bool DrawText2D(ImDrawList* drawList, ImVec2 pos, float size, ImColor textcolor, const char* text) {
        if (drawList == nullptr || text == nullptr) {
            return false;
        }

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->Fonts[1];

        if (font == nullptr) {
            std::cout << E("No Font\n");
            return false;
        }

        ImColor outlineColor = ImColor(0, 0, 0, 255); // Black outline color
        float outlineThickness = 1.0f; // Outline thickness

        ImGui::PushFont(font);

        // Draw the text outline
        drawList->AddText(font, size, ImVec2(pos.x - outlineThickness, pos.y - outlineThickness), outlineColor, text);
        drawList->AddText(font, size, ImVec2(pos.x + outlineThickness, pos.y - outlineThickness), outlineColor, text);
        drawList->AddText(font, size, ImVec2(pos.x - outlineThickness, pos.y + outlineThickness), outlineColor, text);
        drawList->AddText(font, size, ImVec2(pos.x + outlineThickness, pos.y + outlineThickness), outlineColor, text);

        // Draw the main text
        drawList->AddText(font, size, pos, textcolor, text);

        ImGui::PopFont();

        return true;
    }

    bool DrawLine2D(ImDrawList* drawList, ImVec2 starting_point, ImVec2 end_point, ImColor color, float thickness) {
        if (!drawList)
            return false;

        drawList->AddLine(starting_point, end_point, color, thickness);

        return true;
    }

    bool DrawCircle2D(ImDrawList* drawList, ImVec2 pos, float radius, ImColor outlineColor) {
        if (!drawList)
            return false;

        drawList->AddCircle(pos, radius, outlineColor, 512, 0.6f);

        return true;
    }

    bool DrawFilledCircle2D(ImDrawList* drawList, ImVec2 pos, float radius, ImColor outlineColor, ImColor fillColor) {
        if (!drawList)
            return false;

        drawList->AddCircleFilled(pos, radius, fillColor, 128);
        drawList->AddCircle(pos, radius, outlineColor, 128);

        return true;
    }

    bool DrawFillBar2D(ImDrawList* drawList, ImVec2 StartPos, ImVec2 EndPos, ImColor FillColor, ImColor EmptyColor, ImColor OutlineColor, float HowFull) {
        if (!drawList)
            return false;

        // Clamp HowFull to [0.0, 1.0]
        if (HowFull < 0.0f) HowFull = 0.0f;
        if (HowFull > 1.0f) HowFull = 1.0f;

        float width = EndPos.x - StartPos.x;
        float height = EndPos.y - StartPos.y;
        float fillWidth = width * HowFull;
        float fillHeight = height * HowFull;

        // Draw filled and empty portions
        if (width > height) {
            drawList->AddRectFilled(ImVec2(StartPos.x, StartPos.y), ImVec2(StartPos.x + fillWidth, EndPos.y), FillColor);
            drawList->AddRectFilled(ImVec2(StartPos.x + fillWidth, StartPos.y), EndPos, EmptyColor);
        }
        else {
            drawList->AddRectFilled(ImVec2(StartPos.x, StartPos.y), ImVec2(EndPos.x, StartPos.y + fillHeight), FillColor);
            drawList->AddRectFilled(ImVec2(StartPos.x, StartPos.y + fillHeight), EndPos, EmptyColor);
        }

        // Draw outline
        drawList->AddRect(StartPos, EndPos, OutlineColor);

        return true;
    }
}