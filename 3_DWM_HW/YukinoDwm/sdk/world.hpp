#include "../Driver/API/Driver.h"
#include "../utils/pimraryXor.h"
#include <stdio.h>
#include "../utils/vector.h"

using namespace cs2_dumper;

class Camera {
private:

public:
	uint64_t modBase = NULL;
	view_matrix_t ViewMatrix;

	Camera() {
		this->modBase = Driver->ClientDLL;
		this->ViewMatrix = Driver->RPM<view_matrix_t>(this->modBase + offsets::client_dll::dwViewMatrix);
	}

	view_matrix_t GetViewMatrix() {
		return Driver->RPM<view_matrix_t>(this->modBase + offsets::client_dll::dwViewMatrix);
	}

	bool w2s(const Vector3& pos, Vector3& out, view_matrix_t matrix) {
		out.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
		out.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

		float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

		if (w < 0.01f)
			return false;

		float inv_w = 1.f / w;
		out.x *= inv_w;
		out.y *= inv_w;

		int ww = GetSystemMetrics(SM_CXSCREEN);
		int wh = GetSystemMetrics(SM_CYSCREEN);

		float x = ww * 0.5f;
		float y = wh * 0.5f;

		x += 0.5f * out.x * ww + 0.5f;
		y -= 0.5f * out.y * wh + 0.5f;

		if (x <= 0 or x >= 1920) {
			return false;
		}
		if (y <= 0 or y >= 1080) {
			return false;
		}

		out.x = x;
		out.y = y;

		return true;
	}
};

class World {
private:

public:
	uint64_t modBase = NULL;

	World() {
		this->modBase = Driver->ClientDLL;
	}

	std::string GetMapName() {
		uintptr_t global_vars = Driver->RPM<uintptr_t>(this->modBase + offsets::client_dll::dwGlobalVars);
		uintptr_t current_map_name = Driver->RPM<uintptr_t>(global_vars + 0x180);
		char buf[MAX_PATH];
		Driver->readArray(current_map_name, buf, MAX_PATH);
		return std::string(buf);
	}
};