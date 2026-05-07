#include "KeyBind.h"

namespace buildpad
{
KeyBind KeyBind::GlobalState { };

bool KeyBind::IsPressed() const
{
    return *this
        && Control == GlobalState.Control
        && Alt == GlobalState.Alt
        && Shift == GlobalState.Shift
        && ImGui::IsKeyPressed(Key, false);
}

bool KeyBind::FromString(std::string_view str)
{
    Control = Alt = Shift = false;
    Key = { };
    for (auto&& part : util::split(str, "+ "))
    {
        if (part.empty())
            return false;

        if (part == "CTRL")
            Control = true;
        else if (part == "ALT")
            Alt = true;
        else if (part == "SHIFT")
            Shift = true;
        else if (part == ">GW2")
            PassThrough = true;
        else if (auto const itr = util::find_if(GetKeyMap(), util::second_equals(part)))
            Key = itr->first;
        else
            return false;
    }
    return *this;
}

std::optional<std::string> KeyBind::ToString() const
{
    if (auto key = KeyToString(Key))
        return fmt::format("{}{}{}{}{}",
            Control ? "CTRL+" : "",
            Alt ? "ALT+" : "",
            Shift ? "SHIFT+" : "",
            *key,
            PassThrough ? " >GW2" : "");

    return { };
}

std::optional<std::string> KeyBind::KeyToString(key_t key)
{
    if (!key)
        return { };

    if (auto const itr = GetKeyMap().find(key); itr != GetKeyMap().end())
        return std::string(itr->second);

    return { };
}

std::map<KeyBind::key_t, std::string_view> const& KeyBind::GetKeyMap()
{
    static std::map<key_t, std::string_view> const instance
    {
        { ImGuiKey_Tab,             "TAB"       },
        { ImGuiKey_LeftArrow,       "LEFT"      },
        { ImGuiKey_RightArrow,      "RIGHT"     },
        { ImGuiKey_UpArrow,         "UP"        },
        { ImGuiKey_DownArrow,       "DOWN"      },
        { ImGuiKey_PageUp,          "PGUP"      },
        { ImGuiKey_PageDown,        "PGDN"      },
        { ImGuiKey_Home,            "HOME"      },
        { ImGuiKey_End,             "END"       },
        { ImGuiKey_Insert,          "INSERT"    },
        { ImGuiKey_Delete,          "DELETE"    },
        { ImGuiKey_Backspace,       "BACK"      },
        { ImGuiKey_Space,           "SPACE"     },
        { ImGuiKey_Enter,           "RETURN"    },
        { ImGuiKey_Escape,          "ESCAPE"    },
        { ImGuiKey_0,               "0"         },
        { ImGuiKey_1,               "1"         },
        { ImGuiKey_2,               "2"         },
        { ImGuiKey_3,               "3"         },
        { ImGuiKey_4,               "4"         },
        { ImGuiKey_5,               "5"         },
        { ImGuiKey_6,               "6"         },
        { ImGuiKey_7,               "7"         },
        { ImGuiKey_8,               "8"         },
        { ImGuiKey_9,               "9"         },
        { ImGuiKey_A,               "A"         },
        { ImGuiKey_B,               "B"         },
        { ImGuiKey_C,               "C"         },
        { ImGuiKey_D,               "D"         },
        { ImGuiKey_E,               "E"         },
        { ImGuiKey_F,               "F"         },
        { ImGuiKey_G,               "G"         },
        { ImGuiKey_H,               "H"         },
        { ImGuiKey_I,               "I"         },
        { ImGuiKey_J,               "J"         },
        { ImGuiKey_K,               "K"         },
        { ImGuiKey_L,               "L"         },
        { ImGuiKey_M,               "M"         },
        { ImGuiKey_N,               "N"         },
        { ImGuiKey_O,               "O"         },
        { ImGuiKey_P,               "P"         },
        { ImGuiKey_Q,               "Q"         },
        { ImGuiKey_R,               "R"         },
        { ImGuiKey_S,               "S"         },
        { ImGuiKey_T,               "T"         },
        { ImGuiKey_U,               "U"         },
        { ImGuiKey_V,               "V"         },
        { ImGuiKey_W,               "W"         },
        { ImGuiKey_X,               "X"         },
        { ImGuiKey_Y,               "Y"         },
        { ImGuiKey_Z,               "Z"         },
        { ImGuiKey_F1,              "F1"        },
        { ImGuiKey_F2,              "F2"        },
        { ImGuiKey_F3,              "F3"        },
        { ImGuiKey_F4,              "F4"        },
        { ImGuiKey_F5,              "F5"        },
        { ImGuiKey_F6,              "F6"        },
        { ImGuiKey_F7,              "F7"        },
        { ImGuiKey_F8,              "F8"        },
        { ImGuiKey_F9,              "F9"        },
        { ImGuiKey_F10,             "F10"       },
        { ImGuiKey_F11,             "F11"       },
        { ImGuiKey_F12,             "F12"       },
        { ImGuiKey_F13,             "F13"       },
        { ImGuiKey_F14,             "F14"       },
        { ImGuiKey_F15,             "F15"       },
        { ImGuiKey_F16,             "F16"       },
        { ImGuiKey_F17,             "F17"       },
        { ImGuiKey_F18,             "F18"       },
        { ImGuiKey_F19,             "F19"       },
        { ImGuiKey_F20,             "F20"       },
        { ImGuiKey_F21,             "F21"       },
        { ImGuiKey_F22,             "F22"       },
        { ImGuiKey_F23,             "F23"       },
        { ImGuiKey_F24,             "F24"       },
        { ImGuiKey_Apostrophe,      "'"         },
        { ImGuiKey_Comma,           ","         },
        { ImGuiKey_Minus,           "-"         },
        { ImGuiKey_Period,          "."         },
        { ImGuiKey_Slash,           "/"         },
        { ImGuiKey_Semicolon,       ";"         },
        { ImGuiKey_Equal,           "="         },
        { ImGuiKey_LeftBracket,     "["         },
        { ImGuiKey_Backslash,       "\\"        },
        { ImGuiKey_RightBracket,    "]"         },
        { ImGuiKey_GraveAccent,     "`"         },
        { ImGuiKey_CapsLock,        "CAPSLOCK"  },
        { ImGuiKey_ScrollLock,      "SCROLL"    },
        { ImGuiKey_NumLock,         "NUMLOCK"   },
        { ImGuiKey_PrintScreen,     "PRINT"     },
        { ImGuiKey_Pause,           "PAUSE"     },
        { ImGuiKey_Keypad0,         "NUMPAD0"   },
        { ImGuiKey_Keypad1,         "NUMPAD1"   },
        { ImGuiKey_Keypad2,         "NUMPAD2"   },
        { ImGuiKey_Keypad3,         "NUMPAD3"   },
        { ImGuiKey_Keypad4,         "NUMPAD4"   },
        { ImGuiKey_Keypad5,         "NUMPAD5"   },
        { ImGuiKey_Keypad6,         "NUMPAD6"   },
        { ImGuiKey_Keypad7,         "NUMPAD7"   },
        { ImGuiKey_Keypad8,         "NUMPAD8"   },
        { ImGuiKey_Keypad9,         "NUMPAD9"   },
        { ImGuiKey_KeypadDecimal,   "DECIMAL"   },
        { ImGuiKey_KeypadDivide,    "DIVIDE"    },
        { ImGuiKey_KeypadMultiply,  "MULTIPLY"  },
        { ImGuiKey_KeypadSubtract,  "SUBTRACT"  },
        { ImGuiKey_KeypadAdd,       "ADD"       },
        { ImGuiKey_KeypadEnter,     "ENTER"     },
        { ImGuiKey_KeypadEqual,     "EQUAL"     },
        { ImGuiKey_AppBack,         "APPBACK"   },
        { ImGuiKey_AppForward,      "APPFWD"    },
        { ImGuiKey_Oem102,          "OEM102"    },
    };
    return instance;
}
}
