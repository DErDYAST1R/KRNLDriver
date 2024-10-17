#include <cmath>
#include <iostream>
#include <algorithm>
#include "../../Driver/UTILS/skCrypt.h"
#include "../../IMGUI/imgui.h"

bool containsString(const std::string& str, const std::string& find) {
    return str.find(find) != std::string::npos;
}

char* intToHex(int value) {
    static char buffer[16]; // 8 hex digits + null terminator for 32-bit int
    snprintf(buffer, sizeof(buffer), "%08X", value); // Convert to hex
    return buffer;
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
    bool DrawBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor outlineColor, float rounding, int thickness = 1) {
        if (!drawList)
            return false;

        float radius = (endPos.x - startPos.x) * rounding * 0.5f; // Calculate radius based on rounding
        drawList->AddRect(startPos, endPos, ImColor(0, 0, 0, 255), radius, ImDrawFlags_None, thickness);
        drawList->AddRect(startPos, endPos, outlineColor, radius, ImDrawFlags_None, thickness);
        return true;
    }

    bool DrawFilledBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor outlineColor, ImColor fillColor, float rounding, int thickness = 1) {
        if (!drawList)
            return false;

        fillColor.Value.w = 0.3f;

        float radius = (endPos.x - startPos.x) * rounding * 0.5f;


        drawList->AddRectFilled(startPos, endPos, fillColor, radius);
        drawList->AddRect(startPos, endPos, ImColor(0, 0, 0, 255), radius, ImDrawFlags_None, thickness);
        drawList->AddRect(startPos, endPos, outlineColor, radius, ImDrawFlags_None, thickness);

        return true;
    }

    bool DrawGradientBox2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor TopColor, ImColor BottomColor, ImColor outlineColor, float rounding, int thickness = 1) {
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

        drawList->AddRect(startPos, endPos, outlineColor, 0.0f, thickness);
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

    bool DrawLine2D(ImDrawList* drawList, ImVec2 starting_point, ImVec2 end_point, ImColor color, float thickness, bool outline = FALSE) {
        if (!drawList)
            return false;

        if (outline == TRUE) {
            drawList->AddLine(starting_point, end_point, ImColor(0, 0, 0, 255), thickness + 0.2f);
        }
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


    bool DrawFillBar2D(ImDrawList* drawList, ImVec2 startPos, ImVec2 endPos, ImColor fillColor, ImColor emptyColor, ImColor outlineColor, float howFull, int thickness = 1) {
        // Clamp howFull between 0.0 and 1.0
        if (howFull < 0.0f) {
            howFull = 0.0f;
        }
        else if (howFull > 1.0f) {
            howFull = 1.0f;
        }

        // Calculate the width and height of the FillBar
        float width = endPos.x - startPos.x;
        float height = endPos.y - startPos.y;

        // Draw the outline of the FillBar
        //drawList->AddRect(startPos, endPos, outlineColor, 0.0f, ImDrawFlags_None, 1.0f);

        // Calculate the filled area based on howFull
        ImVec2 fillEndPos = ImVec2(endPos.x, startPos.y + (height * (1.0f - howFull))); // Fill from top to bottom

        float radius = (endPos.x - startPos.x) * 0.7f * 0.5f; // Calculate radius based on rounding


        // Draw the filled area
        drawList->AddRectFilled(ImVec2(startPos.x, fillEndPos.y), endPos, fillColor); // Draw filled area from fillEndPos to bottom

        // Draw the empty area
        // drawList->AddRect(startPos, endPos, ImColor(0, 0, 0, 255), 0.0f, ImDrawFlags_None, 0.3);
        drawList->AddRectFilled(startPos, ImVec2(fillEndPos.x, fillEndPos.y), emptyColor); // Draw empty area above filled area

        drawList->AddRect(startPos, endPos, ImColor(0, 0, 0, 255), radius, ImDrawFlags_None, 0.1);

        return true;
    }

}