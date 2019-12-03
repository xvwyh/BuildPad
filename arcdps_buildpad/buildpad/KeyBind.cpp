#include "KeyBind.h"
#include <Windows.h>

namespace buildpad
{
bool KeyBind::IsPressed() const
{
    return *this
        && Control == ImGui::GetIO().KeyCtrl
        && Alt == ImGui::GetIO().KeyAlt
        && Shift == ImGui::GetIO().KeyShift
        && ImGui::IsKeyPressed(Key, false);
}

bool KeyBind::FromString(std::string_view str)
{
    Control = Alt = Shift = false;
    Key = 0;
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
        else if (part[0] >= '0' && part[0] <= '9' || part[0] >= 'A' && part[0] <= 'Z')
            Key = part[0];
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

    if (key >= '0' && key <= '9' || key >= 'A' && key <= 'Z')
        return std::string(1, key);

    if (auto const itr = GetKeyMap().find(key); itr != GetKeyMap().end())
        return std::string(itr->second);

    return { };
}

std::map<KeyBind::key_t, std::string_view> const& KeyBind::GetKeyMap()
{
    static std::map<key_t, std::string_view> const instance
    {
        { VK_BACK,          "BACK"      },
        { VK_TAB,           "TAB"       },
        { VK_CLEAR,         "CLEAR"     },
        { VK_RETURN,        "RETURN"    },
        { VK_ESCAPE,        "ESCAPE"    },
        { VK_SPACE,         "SPACE"     },
        { VK_PRIOR,         "PGUP"      },
        { VK_NEXT,          "PGDN"      },
        { VK_END,           "END"       },
        { VK_HOME,          "HOME"      },
        { VK_LEFT,          "LEFT"      },
        { VK_UP,            "UP"        },
        { VK_RIGHT,         "RIGHT"     },
        { VK_DOWN,          "DOWN"      },
        { VK_SELECT,        "SELECT"    },
        { VK_PRINT,         "PRINT"     },
        { VK_EXECUTE,       "EXECUTE"   },
        { VK_SNAPSHOT,      "SNAPSHOT"  },
        { VK_INSERT,        "INSERT"    },
        { VK_DELETE,        "DELETE"    },
        { VK_HELP,          "HELP"      },
        { VK_NUMPAD0,       "NUMPAD0"   },
        { VK_NUMPAD1,       "NUMPAD1"   },
        { VK_NUMPAD2,       "NUMPAD2"   },
        { VK_NUMPAD3,       "NUMPAD3"   },
        { VK_NUMPAD4,       "NUMPAD4"   },
        { VK_NUMPAD5,       "NUMPAD5"   },
        { VK_NUMPAD6,       "NUMPAD6"   },
        { VK_NUMPAD7,       "NUMPAD7"   },
        { VK_NUMPAD8,       "NUMPAD8"   },
        { VK_NUMPAD9,       "NUMPAD9"   },
        { VK_MULTIPLY,      "MULTIPLY"  },
        { VK_ADD,           "ADD"       },
        { VK_SEPARATOR,     "SEPARATOR" },
        { VK_SUBTRACT,      "SUBTRACT"  },
        { VK_DECIMAL,       "DECIMAL"   },
        { VK_DIVIDE,        "DIVIDE"    },
        { VK_F1,            "F1"        },
        { VK_F2,            "F2"        },
        { VK_F3,            "F3"        },
        { VK_F4,            "F4"        },
        { VK_F5,            "F5"        },
        { VK_F6,            "F6"        },
        { VK_F7,            "F7"        },
        { VK_F8,            "F8"        },
        { VK_F9,            "F9"        },
        { VK_F10,           "F10"       },
        { VK_F11,           "F11"       },
        { VK_F12,           "F12"       },
        { VK_F13,           "F13"       },
        { VK_F14,           "F14"       },
        { VK_F15,           "F15"       },
        { VK_F16,           "F16"       },
        { VK_F17,           "F17"       },
        { VK_F18,           "F18"       },
        { VK_F19,           "F19"       },
        { VK_F20,           "F20"       },
        { VK_F21,           "F21"       },
        { VK_F22,           "F22"       },
        { VK_F23,           "F23"       },
        { VK_F24,           "F24"       },
        { VK_NUMLOCK,       "NUMLOCK"   },
        { VK_SCROLL,        "SCROLL"    },
        { VK_OEM_1,         ";"         },
        { VK_OEM_PLUS,      "="         },
        { VK_OEM_COMMA,     ","         },
        { VK_OEM_MINUS,     "-"         },
        { VK_OEM_PERIOD,    "."         },
        { VK_OEM_2,         "/"         },
        { VK_OEM_3,         "`"         },
        { VK_OEM_4,         "["         },
        { VK_OEM_5,         "\\"        },
        { VK_OEM_6,         "]"         },
        { VK_OEM_7,         "'"        },
    };
    return instance;
}
}
