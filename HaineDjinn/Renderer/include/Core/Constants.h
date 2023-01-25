#pragma once

// Maths.hpp defines
#define M_PI				3.14159265358979323846f
#define BOX_MAX_RANGE		10000.f // used because of the way we implemented the box tests
#define HORIZONTAL_MATRIX	true // defines the standard used by mat4x4, WIP

// camera related
#define CAMERA_SPEED		    1.0f
#define CAMERA_LERP		        0.25f
#define CAMERA_MAX_PITCH	    87.5f
#define ORTHOGRAPHIC_SRC_SIZE   10

// misc
#define MAX_LOG_STRING_SIZE		4096 // Max size in characters that Dubug's buffer can have before writing
#define THREADS_NUMBER			4
#define THREADING_TYPE			0 // 0 = multithreading, 1 = monothreading
#define AZERTY_KEYS			    false
#define MOUSE_SENSITIVITY       0.002f
#define GRAVITY				    9.81f

// implementation limitations
#define LIGHTS_NUMBER		    8 // used because an UBO can't have a dynamic size

// rendering related
#define SCR_WIDTH			    1280
#define SCR_HEIGHT			    720
#define PARTICLE_MAX_MODELS     16
#define CUT_RENDERING_CODE		-2
