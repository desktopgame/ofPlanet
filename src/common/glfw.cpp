#include "glfw.hpp"

#include <ofAppGLFWWindow.h>
#include <ofApp.h>
#include <GLFW/glfw3.h>
namespace planet {

namespace glfw {
void showCursor() {
        ofAppGLFWWindow* glfwWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
        glfwSetInputMode(glfwWindow->getGLFWWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_NORMAL);
}

void hideCursor() {
        ofAppGLFWWindow* glfwWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
        glfwSetInputMode(glfwWindow->getGLFWWindow(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
}
int getKey(int keycode) {
        ofAppGLFWWindow* glfwWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
        return glfwGetKey(glfwWindow->getGLFWWindow(), keycode);
}
extern int Version_major = 3;
extern int Version_minor = 3;
extern int Version_revision = 0;
extern int True = 1;
extern int False = 0;
extern int Release = 0;
extern int Press = 1;
extern int Repeat = 2;
extern int Hat_centered = 0;
extern int Hat_up = 1;
extern int Hat_right = 2;
extern int Hat_down = 4;
extern int Hat_left = 8;
extern int Hat_right_up = (GLFW_HAT_RIGHT);
extern int Hat_right_down = (GLFW_HAT_RIGHT);
extern int Hat_left_up = (GLFW_HAT_LEFT);
extern int Hat_left_down = (GLFW_HAT_LEFT);
extern int Key_unknown = -1;
extern int Key_space = 32;
extern int Key_apostrophe = 39;
extern int Key_comma = 44;
extern int Key_minus = 45;
extern int Key_period = 46;
extern int Key_slash = 47;
extern int Key_0 = 48;
extern int Key_1 = 49;
extern int Key_2 = 50;
extern int Key_3 = 51;
extern int Key_4 = 52;
extern int Key_5 = 53;
extern int Key_6 = 54;
extern int Key_7 = 55;
extern int Key_8 = 56;
extern int Key_9 = 57;
extern int Key_semicolon = 59;
extern int Key_equal = 61;
extern int Key_a = 65;
extern int Key_b = 66;
extern int Key_c = 67;
extern int Key_d = 68;
extern int Key_e = 69;
extern int Key_f = 70;
extern int Key_g = 71;
extern int Key_h = 72;
extern int Key_i = 73;
extern int Key_j = 74;
extern int Key_k = 75;
extern int Key_l = 76;
extern int Key_m = 77;
extern int Key_n = 78;
extern int Key_o = 79;
extern int Key_p = 80;
extern int Key_q = 81;
extern int Key_r = 82;
extern int Key_s = 83;
extern int Key_t = 84;
extern int Key_u = 85;
extern int Key_v = 86;
extern int Key_w = 87;
extern int Key_x = 88;
extern int Key_y = 89;
extern int Key_z = 90;
extern int Key_left_bracket = 91;
extern int Key_backslash = 92;
extern int Key_right_bracket = 93;
extern int Key_grave_accent = 96;
extern int Key_world_1 = 161;
extern int Key_world_2 = 162;
extern int Key_escape = 256;
extern int Key_enter = 257;
extern int Key_tab = 258;
extern int Key_backspace = 259;
extern int Key_insert = 260;
extern int Key_delete = 261;
extern int Key_right = 262;
extern int Key_left = 263;
extern int Key_down = 264;
extern int Key_up = 265;
extern int Key_page_up = 266;
extern int Key_page_down = 267;
extern int Key_home = 268;
extern int Key_end = 269;
extern int Key_caps_lock = 280;
extern int Key_scroll_lock = 281;
extern int Key_num_lock = 282;
extern int Key_print_screen = 283;
extern int Key_pause = 284;
extern int Key_f1 = 290;
extern int Key_f2 = 291;
extern int Key_f3 = 292;
extern int Key_f4 = 293;
extern int Key_f5 = 294;
extern int Key_f6 = 295;
extern int Key_f7 = 296;
extern int Key_f8 = 297;
extern int Key_f9 = 298;
extern int Key_f10 = 299;
extern int Key_f11 = 300;
extern int Key_f12 = 301;
extern int Key_f13 = 302;
extern int Key_f14 = 303;
extern int Key_f15 = 304;
extern int Key_f16 = 305;
extern int Key_f17 = 306;
extern int Key_f18 = 307;
extern int Key_f19 = 308;
extern int Key_f20 = 309;
extern int Key_f21 = 310;
extern int Key_f22 = 311;
extern int Key_f23 = 312;
extern int Key_f24 = 313;
extern int Key_f25 = 314;
extern int Key_kp_0 = 320;
extern int Key_kp_1 = 321;
extern int Key_kp_2 = 322;
extern int Key_kp_3 = 323;
extern int Key_kp_4 = 324;
extern int Key_kp_5 = 325;
extern int Key_kp_6 = 326;
extern int Key_kp_7 = 327;
extern int Key_kp_8 = 328;
extern int Key_kp_9 = 329;
extern int Key_kp_decimal = 330;
extern int Key_kp_divide = 331;
extern int Key_kp_multiply = 332;
extern int Key_kp_subtract = 333;
extern int Key_kp_add = 334;
extern int Key_kp_enter = 335;
extern int Key_kp_equal = 336;
extern int Key_left_shift = 340;
extern int Key_left_control = 341;
extern int Key_left_alt = 342;
extern int Key_left_super = 343;
extern int Key_right_shift = 344;
extern int Key_right_control = 345;
extern int Key_right_alt = 346;
extern int Key_right_super = 347;
extern int Key_menu = 348;
extern int Key_last = GLFW_KEY_MENU;
extern int Mod_shift = 0x0001;
extern int Mod_control = 0x0002;
extern int Mod_alt = 0x0004;
extern int Mod_super = 0x0008;
extern int Mod_caps_lock = 0x0010;
extern int Mod_num_lock = 0x0020;
extern int Mouse_button_1 = 0;
extern int Mouse_button_2 = 1;
extern int Mouse_button_3 = 2;
extern int Mouse_button_4 = 3;
extern int Mouse_button_5 = 4;
extern int Mouse_button_6 = 5;
extern int Mouse_button_7 = 6;
extern int Mouse_button_8 = 7;
extern int Mouse_button_last = GLFW_MOUSE_BUTTON_8;
extern int Mouse_button_left = GLFW_MOUSE_BUTTON_1;
extern int Mouse_button_right = GLFW_MOUSE_BUTTON_2;
extern int Mouse_button_middle = GLFW_MOUSE_BUTTON_3;
extern int Joystick_1 = 0;
extern int Joystick_2 = 1;
extern int Joystick_3 = 2;
extern int Joystick_4 = 3;
extern int Joystick_5 = 4;
extern int Joystick_6 = 5;
extern int Joystick_7 = 6;
extern int Joystick_8 = 7;
extern int Joystick_9 = 8;
extern int Joystick_10 = 9;
extern int Joystick_11 = 10;
extern int Joystick_12 = 11;
extern int Joystick_13 = 12;
extern int Joystick_14 = 13;
extern int Joystick_15 = 14;
extern int Joystick_16 = 15;
extern int Joystick_last = GLFW_JOYSTICK_16;
extern int Gamepad_button_a = 0;
extern int Gamepad_button_b = 1;
extern int Gamepad_button_x = 2;
extern int Gamepad_button_y = 3;
extern int Gamepad_button_left_bumper = 4;
extern int Gamepad_button_right_bumper = 5;
extern int Gamepad_button_back = 6;
extern int Gamepad_button_start = 7;
extern int Gamepad_button_guide = 8;
extern int Gamepad_button_left_thumb = 9;
extern int Gamepad_button_right_thumb = 10;
extern int Gamepad_button_dpad_up = 11;
extern int Gamepad_button_dpad_right = 12;
extern int Gamepad_button_dpad_down = 13;
extern int Gamepad_button_dpad_left = 14;
extern int Gamepad_button_last = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
extern int Gamepad_button_cross = GLFW_GAMEPAD_BUTTON_A;
extern int Gamepad_button_circle = GLFW_GAMEPAD_BUTTON_B;
extern int Gamepad_button_square = GLFW_GAMEPAD_BUTTON_X;
extern int Gamepad_button_triangle = GLFW_GAMEPAD_BUTTON_Y;
extern int Gamepad_axis_left_x = 0;
extern int Gamepad_axis_left_y = 1;
extern int Gamepad_axis_right_x = 2;
extern int Gamepad_axis_right_y = 3;
extern int Gamepad_axis_left_trigger = 4;
extern int Gamepad_axis_right_trigger = 5;
extern int Gamepad_axis_last = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
extern int No_error = 0;
extern int Not_initialized = 0x00010001;
extern int No_current_context = 0x00010002;
extern int Invalid_enum = 0x00010003;
extern int Invalid_value = 0x00010004;
extern int Out_of_memory = 0x00010005;
extern int Api_unavailable = 0x00010006;
extern int Version_unavailable = 0x00010007;
extern int Platform_error = 0x00010008;
extern int Format_unavailable = 0x00010009;
extern int No_window_context = 0x0001000A;
extern int Focused = 0x00020001;
extern int Iconified = 0x00020002;
extern int Resizable = 0x00020003;
extern int Visible = 0x00020004;
extern int Decorated = 0x00020005;
extern int Auto_iconify = 0x00020006;
extern int Floating = 0x00020007;
extern int Maximized = 0x00020008;
extern int Center_cursor = 0x00020009;
extern int Transparent_framebuffer = 0x0002000A;
extern int Hovered = 0x0002000B;
extern int Red_bits = 0x00021001;
extern int Green_bits = 0x00021002;
extern int Blue_bits = 0x00021003;
extern int Alpha_bits = 0x00021004;
extern int Depth_bits = 0x00021005;
extern int Stencil_bits = 0x00021006;
extern int Accum_red_bits = 0x00021007;
extern int Accum_green_bits = 0x00021008;
extern int Accum_blue_bits = 0x00021009;
extern int Accum_alpha_bits = 0x0002100A;
extern int Aux_buffers = 0x0002100B;
extern int Stereo = 0x0002100C;
extern int Samples = 0x0002100D;
extern int Srgb_capable = 0x0002100E;
extern int Refresh_rate = 0x0002100F;
extern int Doublebuffer = 0x00021010;
extern int Client_api = 0x00022001;
extern int Context_version_major = 0x00022002;
extern int Context_version_minor = 0x00022003;
extern int Context_revision = 0x00022004;
extern int Context_robustness = 0x00022005;
extern int Opengl_forward_compat = 0x00022006;
extern int Opengl_debug_context = 0x00022007;
extern int Opengl_profile = 0x00022008;
extern int Context_release_behavior = 0x00022009;
extern int Context_no_error = 0x0002200A;
extern int Context_creation_api = 0x0002200B;
extern int Cocoa_retina_framebuffer = 0x00023001;
extern int Cocoa_frame_name = 0x00023002;
extern int Cocoa_graphics_switching = 0x00023003;
extern int X11_class_name = 0x00024001;
extern int X11_instance_name = 0x00024002;
extern int No_api = 0;
extern int Opengl_api = 0x00030001;
extern int Opengl_es_api = 0x00030002;
extern int No_robustness = 0;
extern int No_reset_notification = 0x00031001;
extern int Lose_context_on_reset = 0x00031002;
extern int Opengl_any_profile = 0;
extern int Opengl_core_profile = 0x00032001;
extern int Opengl_compat_profile = 0x00032002;
extern int Cursor = 0x00033001;
extern int Sticky_keys = 0x00033002;
extern int Sticky_mouse_buttons = 0x00033003;
extern int Lock_key_mods = 0x00033004;
extern int Cursor_normal = 0x00034001;
extern int Cursor_hidden = 0x00034002;
extern int Cursor_disabled = 0x00034003;
extern int Any_release_behavior = 0;
extern int Release_behavior_flush = 0x00035001;
extern int Release_behavior_none = 0x00035002;
extern int Native_context_api = 0x00036001;
extern int Egl_context_api = 0x00036002;
extern int Osmesa_context_api = 0x00036003;
extern int Arrow_cursor = 0x00036001;
extern int Ibeam_cursor = 0x00036002;
extern int Crosshair_cursor = 0x00036003;
extern int Hand_cursor = 0x00036004;
extern int Hresize_cursor = 0x00036005;
extern int Vresize_cursor = 0x00036006;
extern int Connected = 0x00040001;
extern int Disconnected = 0x00040002;
extern int Joystick_hat_buttons = 0x00050001;
extern int Cocoa_chdir_resources = 0x00051001;
extern int Cocoa_menubar = 0x00051002;
extern int Dont_care = -1;

}  // namespace glfw
}  // namespace planet
