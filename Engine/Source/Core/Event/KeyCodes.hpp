#pragma once

#include <cstdint>
#include <map>

#include "Utility/String.hpp"

namespace CAL
{

enum class KeyCodeID : uint8_t
{
    Unknown,

    Backspace,
    Tab,
    Enter,
    Shift,    // generic; not left/right specific
    Control,  // generic
    Alt,      // generic (VK_MENU in Win32 headers)
    Pause,
    CapsLock,
    Escape,
    Space,

    PageUp,
    PageDown,
    End,
    Home,
    Left,
    Up,
    Right,
    Down,
    PrintScreen,
    Insert,
    Delete,

    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    LeftSuper,  // Windows / Cmd / Super
    RightSuper,
    Applications,  // context-menu key

    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,
    Multiply,
    Add,
    Separator,
    Subtract,
    Decimal,
    Divide,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,

    Numlock,
    ScrollLock,

    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,

    Semicolon,  // ;:
    Plus,       // =+
    Comma,      // ,<
    Minus,      // -_
    Period,     // .>
    Slash,      // /?
    Grave,      // `~

    LeftBrace,   // [{
    Backslash,   // backslash / pipe
    RightBrace,  // ]}
    Apostrophe,  // '"

    Count  // sentinel - total number of KeyCodeID values, keep last
};

inline constexpr std::size_t KeyCodeCount = size_t(KeyCodeID::Count);

namespace KeyCodeDetail
{

inline const std::map<KeyCodeID, StringView> keyCodeNameTable{
    { KeyCodeID::Unknown, "Unknown" },
    { KeyCodeID::Backspace, "Backspace" },
    { KeyCodeID::Tab, "Tab" },
    { KeyCodeID::Enter, "Enter" },
    { KeyCodeID::Shift, "Shift" },
    { KeyCodeID::Control, "Control" },
    { KeyCodeID::Alt, "Alt" },
    { KeyCodeID::Pause, "Pause" },
    { KeyCodeID::CapsLock, "CapsLock" },
    { KeyCodeID::Escape, "Escape" },
    { KeyCodeID::Space, "Space" },

    { KeyCodeID::PageUp, "PageUp" },
    { KeyCodeID::PageDown, "PageDown" },
    { KeyCodeID::End, "End" },
    { KeyCodeID::Home, "Home" },
    { KeyCodeID::Left, "Left" },
    { KeyCodeID::Up, "Up" },
    { KeyCodeID::Right, "Right" },
    { KeyCodeID::Down, "Down" },
    { KeyCodeID::PrintScreen, "PrintScreen" },
    { KeyCodeID::Insert, "Insert" },
    { KeyCodeID::Delete, "Delete" },

    { KeyCodeID::Num0, "0" },
    { KeyCodeID::Num1, "1" },
    { KeyCodeID::Num2, "2" },
    { KeyCodeID::Num3, "3" },
    { KeyCodeID::Num4, "4" },
    { KeyCodeID::Num5, "5" },
    { KeyCodeID::Num6, "6" },
    { KeyCodeID::Num7, "7" },
    { KeyCodeID::Num8, "8" },
    { KeyCodeID::Num9, "9" },

    { KeyCodeID::Numpad0, "Numpad0" },
    { KeyCodeID::Numpad1, "Numpad1" },
    { KeyCodeID::Numpad2, "Numpad2" },
    { KeyCodeID::Numpad3, "Numpad3" },
    { KeyCodeID::Numpad4, "Numpad4" },
    { KeyCodeID::Numpad5, "Numpad5" },
    { KeyCodeID::Numpad6, "Numpad6" },
    { KeyCodeID::Numpad7, "Numpad7" },
    { KeyCodeID::Numpad8, "Numpad8" },
    { KeyCodeID::Numpad9, "Numpad9" },

    { KeyCodeID::A, "A" },
    { KeyCodeID::B, "B" },
    { KeyCodeID::C, "C" },
    { KeyCodeID::D, "D" },
    { KeyCodeID::E, "E" },
    { KeyCodeID::F, "F" },
    { KeyCodeID::G, "G" },
    { KeyCodeID::H, "H" },
    { KeyCodeID::I, "I" },
    { KeyCodeID::J, "J" },
    { KeyCodeID::K, "K" },
    { KeyCodeID::L, "L" },
    { KeyCodeID::M, "M" },
    { KeyCodeID::N, "N" },
    { KeyCodeID::O, "O" },
    { KeyCodeID::P, "P" },
    { KeyCodeID::Q, "Q" },
    { KeyCodeID::R, "R" },
    { KeyCodeID::S, "S" },
    { KeyCodeID::T, "T" },
    { KeyCodeID::U, "U" },
    { KeyCodeID::V, "V" },
    { KeyCodeID::W, "W" },
    { KeyCodeID::X, "X" },
    { KeyCodeID::Y, "Y" },
    { KeyCodeID::Z, "Z" },

    { KeyCodeID::F1, "F1" },
    { KeyCodeID::F2, "F2" },
    { KeyCodeID::F3, "F3" },
    { KeyCodeID::F4, "F4" },
    { KeyCodeID::F5, "F5" },
    { KeyCodeID::F6, "F6" },
    { KeyCodeID::F7, "F7" },
    { KeyCodeID::F8, "F8" },
    { KeyCodeID::F9, "F9" },
    { KeyCodeID::F10, "F10" },
    { KeyCodeID::F11, "F11" },
    { KeyCodeID::F12, "F12" },
    { KeyCodeID::F13, "F13" },
    { KeyCodeID::F14, "F14" },
    { KeyCodeID::F15, "F15" },
    { KeyCodeID::F16, "F16" },
    { KeyCodeID::F17, "F17" },
    { KeyCodeID::F18, "F18" },
    { KeyCodeID::F19, "F19" },
    { KeyCodeID::F20, "F20" },
    { KeyCodeID::F21, "F21" },
    { KeyCodeID::F22, "F22" },
    { KeyCodeID::F23, "F23" },
    { KeyCodeID::F24, "F24" },

    { KeyCodeID::LeftSuper, "LeftSuper" },
    { KeyCodeID::RightSuper, "RightSuper" },
    { KeyCodeID::Applications, "Applications" },

    { KeyCodeID::Multiply, "Multiply" },
    { KeyCodeID::Add, "Add" },
    { KeyCodeID::Separator, "Separator" },
    { KeyCodeID::Subtract, "Subtract" },
    { KeyCodeID::Decimal, "Decimal" },
    { KeyCodeID::Divide, "Divide" },

    { KeyCodeID::Numlock, "NumLock" },
    { KeyCodeID::ScrollLock, "ScrollLock" },

    { KeyCodeID::LeftShift, "LeftShift" },
    { KeyCodeID::RightShift, "RightShift" },
    { KeyCodeID::LeftControl, "LeftControl" },
    { KeyCodeID::RightControl, "RightControl" },
    { KeyCodeID::LeftAlt, "LeftAlt" },
    { KeyCodeID::RightAlt, "RightAlt" },

    { KeyCodeID::Semicolon, "Semicolon" },
    { KeyCodeID::Plus, "Plus" },
    { KeyCodeID::Comma, "Comma" },
    { KeyCodeID::Minus, "Minus" },
    { KeyCodeID::Period, "Period" },
    { KeyCodeID::Slash, "Slash" },
    { KeyCodeID::Grave, "Grave" },
    { KeyCodeID::LeftBrace, "LeftBrace" },
    { KeyCodeID::Backslash, "Backslash" },
    { KeyCodeID::RightBrace, "RightBrace" },
    { KeyCodeID::Apostrophe, "Apostrophe" },
};

#if defined(_WIN32)

inline const std::map<uint8_t, KeyCodeID> Win32KeyTable{
    { 0x08, KeyCodeID::Backspace },
    { 0x09, KeyCodeID::Tab },
    { 0x0D, KeyCodeID::Enter },
    { 0x10, KeyCodeID::Shift },
    { 0x11, KeyCodeID::Control },
    { 0x12, KeyCodeID::Alt },
    { 0x13, KeyCodeID::Pause },
    { 0x14, KeyCodeID::CapsLock },
    { 0x1B, KeyCodeID::Escape },
    { 0x20, KeyCodeID::Space },

    { 0x21, KeyCodeID::PageUp },
    { 0x22, KeyCodeID::PageDown },
    { 0x23, KeyCodeID::End },
    { 0x24, KeyCodeID::Home },
    { 0x25, KeyCodeID::Left },
    { 0x26, KeyCodeID::Up },
    { 0x27, KeyCodeID::Right },
    { 0x28, KeyCodeID::Down },
    { 0x2C, KeyCodeID::PrintScreen },
    { 0x2D, KeyCodeID::Insert },
    { 0x2E, KeyCodeID::Delete },

    { 0x30, KeyCodeID::Num0 },
    { 0x31, KeyCodeID::Num1 },
    { 0x32, KeyCodeID::Num2 },
    { 0x33, KeyCodeID::Num3 },
    { 0x34, KeyCodeID::Num4 },
    { 0x35, KeyCodeID::Num5 },
    { 0x36, KeyCodeID::Num6 },
    { 0x37, KeyCodeID::Num7 },
    { 0x38, KeyCodeID::Num8 },
    { 0x39, KeyCodeID::Num9 },

    { 0x41, KeyCodeID::A },
    { 0x42, KeyCodeID::B },
    { 0x43, KeyCodeID::C },
    { 0x44, KeyCodeID::D },
    { 0x45, KeyCodeID::E },
    { 0x46, KeyCodeID::F },
    { 0x47, KeyCodeID::G },
    { 0x48, KeyCodeID::H },
    { 0x49, KeyCodeID::I },
    { 0x4A, KeyCodeID::J },
    { 0x4B, KeyCodeID::K },
    { 0x4C, KeyCodeID::L },
    { 0x4D, KeyCodeID::M },
    { 0x4E, KeyCodeID::N },
    { 0x4F, KeyCodeID::O },
    { 0x50, KeyCodeID::P },
    { 0x51, KeyCodeID::Q },
    { 0x52, KeyCodeID::R },
    { 0x53, KeyCodeID::S },
    { 0x54, KeyCodeID::T },
    { 0x55, KeyCodeID::U },
    { 0x56, KeyCodeID::V },
    { 0x57, KeyCodeID::W },
    { 0x58, KeyCodeID::X },
    { 0x59, KeyCodeID::Y },
    { 0x5A, KeyCodeID::Z },

    { 0x5B, KeyCodeID::LeftSuper },
    { 0x5C, KeyCodeID::RightSuper },
    { 0x5D, KeyCodeID::Applications },

    { 0x60, KeyCodeID::Numpad0 },
    { 0x61, KeyCodeID::Numpad1 },
    { 0x62, KeyCodeID::Numpad2 },
    { 0x63, KeyCodeID::Numpad3 },
    { 0x64, KeyCodeID::Numpad4 },
    { 0x65, KeyCodeID::Numpad5 },
    { 0x66, KeyCodeID::Numpad6 },
    { 0x67, KeyCodeID::Numpad7 },
    { 0x68, KeyCodeID::Numpad8 },
    { 0x69, KeyCodeID::Numpad9 },
    { 0x6A, KeyCodeID::Multiply },
    { 0x6B, KeyCodeID::Add },
    { 0x6C, KeyCodeID::Separator },
    { 0x6D, KeyCodeID::Subtract },
    { 0x6E, KeyCodeID::Decimal },
    { 0x6F, KeyCodeID::Divide },

    { 0x70, KeyCodeID::F1 },
    { 0x71, KeyCodeID::F2 },
    { 0x72, KeyCodeID::F3 },
    { 0x73, KeyCodeID::F4 },
    { 0x74, KeyCodeID::F5 },
    { 0x75, KeyCodeID::F6 },
    { 0x76, KeyCodeID::F7 },
    { 0x77, KeyCodeID::F8 },
    { 0x78, KeyCodeID::F9 },
    { 0x79, KeyCodeID::F10 },
    { 0x7A, KeyCodeID::F11 },
    { 0x7B, KeyCodeID::F12 },
    { 0x7C, KeyCodeID::F13 },
    { 0x7D, KeyCodeID::F14 },
    { 0x7E, KeyCodeID::F15 },
    { 0x7F, KeyCodeID::F16 },
    { 0x80, KeyCodeID::F17 },
    { 0x81, KeyCodeID::F18 },
    { 0x82, KeyCodeID::F19 },
    { 0x83, KeyCodeID::F20 },
    { 0x84, KeyCodeID::F21 },
    { 0x85, KeyCodeID::F22 },
    { 0x86, KeyCodeID::F23 },
    { 0x87, KeyCodeID::F24 },

    { 0x90, KeyCodeID::Numlock },
    { 0x91, KeyCodeID::ScrollLock },

    { 0xA0, KeyCodeID::LeftShift },
    { 0xA1, KeyCodeID::RightShift },
    { 0xA2, KeyCodeID::LeftControl },
    { 0xA3, KeyCodeID::RightControl },
    { 0xA4, KeyCodeID::LeftAlt },
    { 0xA5, KeyCodeID::RightAlt },

    { 0xBA, KeyCodeID::Semicolon },
    { 0xBB, KeyCodeID::Plus },
    { 0xBC, KeyCodeID::Comma },
    { 0xBD, KeyCodeID::Minus },
    { 0xBE, KeyCodeID::Period },
    { 0xBF, KeyCodeID::Slash },
    { 0xC0, KeyCodeID::Grave },

    { 0xDB, KeyCodeID::LeftBrace },
    { 0xDC, KeyCodeID::Backslash },
    { 0xDD, KeyCodeID::RightBrace },
    { 0xDE, KeyCodeID::Apostrophe },
};
}

constexpr KeyCodeID KeyCodeFromWParam(uint8_t wParam) noexcept
{
    return KeyCodeDetail::Win32KeyTable.find(wParam)->second;
}

#endif

constexpr StringView keyCodeToString(KeyCodeID key) noexcept
{
    return KeyCodeDetail::keyCodeNameTable.find(key)->second;
}
}  // namespace CAL
