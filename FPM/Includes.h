#pragma once
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <tlhelp32.h>

#include <dwmapi.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <random>

#include "Driver/UTILS/skCrypt.h"
#include "Driver/UTILS/lazy_importer.hpp"
#include "Driver/Callstack-Spoofer.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <d2d1.h>
#include <d3d9types.h>
#include <d3d11.h>
#include "Lib/imgui/imgui.h"
#include "Lib/imgui/imgui_impl_dx11.h"
#include "Lib/imgui/imgui_impl_win32.h"
#include "Lib/imgui/imgui_internal.h"

namespace g
{

}