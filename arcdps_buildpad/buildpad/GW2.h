#pragma once
#include "Defines.h"

namespace buildpad
{
struct GW2
{
    enum class Profession : uint8_t
    {
        None,
        Guardian,
        Warrior,
        Engineer,
        Ranger,
        Thief,
        Elementalist,
        Mesmer,
        Necromancer,
        Revenant,
    };
    enum class Specialization : uint8_t;
    struct ProfessionInfo
    {
        Profession Profession = Profession::None;
        std::string_view Name;
        std::array<float, 4> Color;
        std::array<Specialization, 8> Specializations;

        [[nodiscard]] ImVec4 GetDefaultButtonColor() const { return { Color[0], Color[1], Color[2], Color[3] * 0.5f }; }
        [[nodiscard]] bool IsProfessionSpecialization(Specialization specialization) const { return std::find(Specializations.begin(), Specializations.end(), specialization) != Specializations.end(); }
    };
    static auto const& GetProfessionInfos()
    {
        constexpr static std::array<ProfessionInfo, 10> instance
        { {
            { Profession::None,         "",             { 0.34f, 0.30f, 0.36f, 1.00f }, { Specialization::None, Specialization::None, Specialization::None, Specialization::None, Specialization::None, Specialization::None, Specialization::None } },
            { Profession::Guardian,     "Guardian",     { 0.04f, 0.87f, 1.00f, 1.00f }, { Specialization::GuardianZeal, Specialization::GuardianRadiance, Specialization::GuardianValor, Specialization::GuardianHonor, Specialization::GuardianVirtues, Specialization::GuardianDragonhunter, Specialization::GuardianFirebrand, Specialization::GuardianWillbender } },
            { Profession::Warrior,      "Warrior",      { 1.00f, 0.83f, 0.24f, 1.00f }, { Specialization::WarriorStrength, Specialization::WarriorArms, Specialization::WarriorDefense, Specialization::WarriorTactics, Specialization::WarriorDiscipline, Specialization::WarriorBerserker, Specialization::WarriorSpellbreaker, Specialization::WarriorBladesworn } },
            { Profession::Engineer,     "Engineer",     { 0.89f, 0.45f, 0.16f, 1.00f }, { Specialization::EngineerExplosives, Specialization::EngineerFirearms, Specialization::EngineerInventions, Specialization::EngineerAlchemy, Specialization::EngineerTools, Specialization::EngineerScrapper, Specialization::EngineerHolosmith } },
            { Profession::Ranger,       "Ranger",       { 0.53f, 0.87f, 0.04f, 1.00f }, { Specialization::RangerMarksmanship, Specialization::RangerSkirmishing, Specialization::RangerWildernessSurvival, Specialization::RangerNatureMagic, Specialization::RangerBeastmastery, Specialization::RangerDruid, Specialization::RangerSoulbeast } },
            { Profession::Thief,        "Thief",        { 0.80f, 0.37f, 0.45f, 1.00f }, { Specialization::ThiefDeadlyArts, Specialization::ThiefCriticalStrikes, Specialization::ThiefShadowArts, Specialization::ThiefAcrobatics, Specialization::ThiefTrickery, Specialization::ThiefDaredevil, Specialization::ThiefDeadeye } },
            { Profession::Elementalist, "Elementalist", { 0.97f, 0.22f, 0.22f, 1.00f }, { Specialization::ElementalistFire, Specialization::ElementalistAir, Specialization::ElementalistEarth, Specialization::ElementalistWater, Specialization::ElementalistArcane, Specialization::ElementalistTempest, Specialization::ElementalistWeaver, Specialization::ElementalistCatalyst } },
            { Profession::Mesmer,       "Mesmer",       { 0.80f, 0.23f, 0.82f, 1.00f }, { Specialization::MesmerDomination, Specialization::MesmerDueling, Specialization::MesmerChaos, Specialization::MesmerInspiration, Specialization::MesmerIllusions, Specialization::MesmerChronomancer, Specialization::MesmerMirage, Specialization::MesmerVirtuoso } },
            { Profession::Necromancer,  "Necromancer",  { 0.02f, 0.89f, 0.49f, 1.00f }, { Specialization::NecromancerSpite, Specialization::NecromancerCurses, Specialization::NecromancerDeathMagic, Specialization::NecromancerBloodMagic, Specialization::NecromancerSoulReaping, Specialization::NecromancerReaper, Specialization::NecromancerScourge, Specialization::NecromancerHarbinger } },
            { Profession::Revenant,     "Revenant",     { 0.63f, 0.16f, 0.16f, 1.00f }, { Specialization::RevenantCorruption, Specialization::RevenantRetribution, Specialization::RevenantSalvation, Specialization::RevenantDevastation, Specialization::RevenantInvocation, Specialization::RevenantHerald, Specialization::RevenantRenegade, Specialization::RevenantVindicator } },
        } };
        return instance;
    }
    static ProfessionInfo const& GetProfessionInfo(Profession profession) { return *util::find_if(GetProfessionInfos(), util::member_equals(&ProfessionInfo::Profession, profession)); }

    enum class Specialization : uint8_t
    {
        None,
        MesmerDueling,
        NecromancerDeathMagic,
        RevenantInvocation,
        WarriorStrength,
        RangerDruid,
        EngineerExplosives,
        ThiefDaredevil,
        RangerMarksmanship,
        RevenantRetribution,
        MesmerDomination,
        WarriorTactics,
        RevenantSalvation,
        GuardianValor,
        RevenantCorruption,
        RevenantDevastation,
        GuardianRadiance,
        ElementalistWater,
        WarriorBerserker,
        NecromancerBloodMagic,
        ThiefShadowArts,
        EngineerTools,
        WarriorDefense,
        MesmerInspiration,
        MesmerIllusions,
        RangerNatureMagic,
        ElementalistEarth,
        GuardianDragonhunter,
        ThiefDeadlyArts,
        EngineerAlchemy,
        RangerSkirmishing,
        ElementalistFire,
        RangerBeastmastery,
        RangerWildernessSurvival,
        NecromancerReaper,
        ThiefCriticalStrikes,
        WarriorArms,
        ElementalistArcane,
        EngineerFirearms,
        NecromancerCurses,
        MesmerChronomancer,
        ElementalistAir,
        GuardianZeal,
        EngineerScrapper,
        ThiefTrickery,
        MesmerChaos,
        GuardianVirtues,
        EngineerInventions,
        ElementalistTempest,
        GuardianHonor,
        NecromancerSoulReaping,
        WarriorDiscipline,
        RevenantHerald,
        NecromancerSpite,
        ThiefAcrobatics,
        RangerSoulbeast,
        ElementalistWeaver,
        EngineerHolosmith,
        ThiefDeadeye,
        MesmerMirage,
        NecromancerScourge,
        WarriorSpellbreaker,
        GuardianFirebrand,
        RevenantRenegade,
        NecromancerHarbinger,
        GuardianWillbender,
        MesmerVirtuoso,
        ElementalistCatalyst,
        WarriorBladesworn,
        RevenantVindicator,
    };
    struct SpecializationInfo
    {
        Specialization Specialization = Specialization::None;
        std::string_view Name;
        std::array<std::array<uint16_t, 4>, 3> Traits; // Last used trait IDs before ArcDPS build templates discontinuation
        Profession Profession = Profession::None;
        bool Elite = false;

        [[nodiscard]] uint8_t GetTraitIndex(uint8_t rank, uint16_t trait) const
        {
            if (rank < Traits.size())
            {
                auto const& traits = Traits[rank];
                if (auto const itr = std::find(traits.begin(), traits.end(), trait); itr != traits.end())
                    return (uint8_t)std::distance(traits.begin(), itr);
            }
            return 0;
        }
    };
    static auto const& GetSpecializationInfos()
    {
        constexpr static std::array<SpecializationInfo, 70> instance
        { {
            { Specialization::None,                     "",                     { { { 0,    0,    0,    0 }, { 0,    0,    0,    0 }, { 0,    0,    0,    0 } } }, Profession::None                 },
            { Specialization::MesmerDueling,            "Dueling",              { { { 0,  701,  705,  700 }, { 0, 1889, 1960,  708 }, { 0,  692, 1950,  704 } } }, Profession::Mesmer               },
            { Specialization::NecromancerDeathMagic,    "Death Magic",          { { { 0,  820,  857, 1922 }, { 0,  858,  860,  855 }, { 0,  842, 1940, 1694 } } }, Profession::Necromancer          },
            { Specialization::RevenantInvocation,       "Invocation",           { { { 0, 1732, 1761, 1784 }, { 0, 1774, 1760, 1781 }, { 0, 1749, 1791, 1719 } } }, Profession::Revenant             },
            { Specialization::WarriorStrength,          "Strength",             { { { 0, 1447, 1451, 1444 }, { 0, 2000, 1338, 1449 }, { 0, 1437, 1454, 1440 } } }, Profession::Warrior              },
            { Specialization::RangerDruid,              "Druid",                { { { 0, 1868, 2016, 1935 }, { 0, 2053, 2001, 2056 }, { 0, 2057, 2058, 2055 } } }, Profession::Ranger,         true },
            { Specialization::EngineerExplosives,       "Explosives",           { { { 0,  514,  525, 1882 }, { 0,  482, 1892, 1944 }, { 0, 1541,  505, 1947 } } }, Profession::Engineer             },
            { Specialization::ThiefDaredevil,           "Daredevil",            { { { 0, 1933, 2023, 1949 }, { 0, 1884, 1893, 1975 }, { 0, 1833, 1964, 2047 } } }, Profession::Thief,          true },
            { Specialization::RangerMarksmanship,       "Marksmanship",         { { { 0, 1021, 1014,  986 }, { 0, 1001, 1000, 1070 }, { 0,  996, 1015, 1698 } } }, Profession::Ranger               },
            { Specialization::RevenantRetribution,      "Retribution",          { { { 0, 1811, 1728, 1810 }, { 0, 1766, 1782, 1740 }, { 0, 1779, 1770, 1790 } } }, Profession::Revenant             },
            { Specialization::MesmerDomination,         "Domination",           { { { 0,  686,  682,  687 }, { 0,  693,  713,  712 }, { 0,  681,  680, 1688 } } }, Profession::Mesmer               },
            { Specialization::WarriorTactics,           "Tactics",              { { { 0, 1469, 1474, 1471 }, { 0, 1486, 1479, 1482 }, { 0, 1667, 1470, 1711 } } }, Profession::Warrior              },
            { Specialization::RevenantSalvation,        "Salvation",            { { { 0, 1823, 1824, 1822 }, { 0, 1819, 1817, 1818 }, { 0, 1815, 1825, 1820 } } }, Profession::Revenant             },
            { Specialization::GuardianValor,            "Valor",                { { { 0,  588,  581,  633 }, { 0,  580,  584, 1684 }, { 0,  585,  586,  589 } } }, Profession::Guardian             },
            { Specialization::RevenantCorruption,       "Corruption",           { { { 0, 1793, 1789, 1741 }, { 0, 1727, 1726, 1714 }, { 0, 1795, 1720, 1721 } } }, Profession::Revenant             },
            { Specialization::RevenantDevastation,      "Devastation",          { { { 0, 1776, 1767, 1755 }, { 0, 1786, 1765, 1802 }, { 0, 1715, 1800, 1754 } } }, Profession::Revenant             },
            { Specialization::GuardianRadiance,         "Radiance",             { { { 0,  577,  566,  574 }, { 0,  578,  567,  565 }, { 0, 1686,  579, 1683 } } }, Profession::Guardian             },
            { Specialization::ElementalistWater,        "Water",                { { { 0,  348,  363,  360 }, { 0,  364,  358,  349 }, { 0,  362,  361, 2028 } } }, Profession::Elementalist         },
            { Specialization::WarriorBerserker,         "Berserker",            { { { 0, 2049, 2039, 1977 }, { 0, 2011, 2042, 2002 }, { 0, 1928, 2038, 2043 } } }, Profession::Warrior,        true },
            { Specialization::NecromancerBloodMagic,    "Blood Magic",          { { { 0,  780,  788, 1876 }, { 0,  789,  799, 1844 }, { 0,  782, 1692,  778 } } }, Profession::Necromancer          },
            { Specialization::ThiefShadowArts,          "Shadow Arts",          { { { 0, 1160, 1293, 1284 }, { 0, 1297, 1130, 1300 }, { 0, 1134, 1135, 1162 } } }, Profession::Thief                },
            { Specialization::EngineerTools,            "Tools",                { { { 0,  532, 1997,  531 }, { 0,  512, 1946, 1832 }, { 0, 1856,  523, 1679 } } }, Profession::Engineer             },
            { Specialization::WarriorDefense,           "Defense",              { { { 0, 1376, 1488, 1372 }, { 0, 1368, 1379, 1367 }, { 0, 1375, 1649, 1708 } } }, Profession::Warrior              },
            { Specialization::MesmerInspiration,        "Inspiration",          { { { 0,  756,  738,  744 }, { 0,  751,  740, 1980 }, { 0, 2005, 1866,  752 } } }, Profession::Mesmer               },
            { Specialization::MesmerIllusions,          "Illusions",            { { { 0,  721, 1869,  691 }, { 0,  722,  729, 1690 }, { 0,  733, 2035,  753 } } }, Profession::Mesmer               },
            { Specialization::RangerNatureMagic,        "Nature Magic",         { { { 0, 1062,  978, 1060 }, { 0, 1054,  965,  964 }, { 0, 1038, 1988, 1697 } } }, Profession::Ranger               },
            { Specialization::ElementalistEarth,        "Earth",                { { { 0,  282, 1507,  289 }, { 0,  275,  281,  277 }, { 0, 1508,  287, 1674 } } }, Profession::Elementalist         },
            { Specialization::GuardianDragonhunter,     "Dragonhunter",         { { { 0, 1898, 1983, 1911 }, { 0, 2037, 1835, 1943 }, { 0, 1908, 1963, 1955 } } }, Profession::Guardian,       true },
            { Specialization::ThiefDeadlyArts,          "Deadly Arts",          { { { 0, 1245, 1276, 1164 }, { 0, 1169, 1292, 1704 }, { 0, 1291, 1167, 1269 } } }, Profession::Thief                },
            { Specialization::EngineerAlchemy,          "Alchemy",              { { { 0,  396,  509,  521 }, { 0,  520,  469,  470 }, { 0,  473, 1871, 1854 } } }, Profession::Engineer             },
            { Specialization::RangerSkirmishing,        "Skirmishing",          { { { 0, 1069, 1067, 1075 }, { 0, 1016, 1700, 1846 }, { 0, 1064, 1912, 1888 } } }, Profession::Ranger               },
            { Specialization::ElementalistFire,         "Fire",                 { { { 0,  296,  328,  335 }, { 0,  325,  340,  334 }, { 0, 1510,  294, 1675 } } }, Profession::Elementalist         },
            { Specialization::RangerBeastmastery,       "Beastmastery",         { { { 0, 1861, 1072, 1606 }, { 0,  975, 1047,  970 }, { 0, 1945,  968, 1066 } } }, Profession::Ranger               },
            { Specialization::RangerWildernessSurvival, "Wilderness Survival",  { { { 0, 1098, 1086, 1099 }, { 0, 1101, 2032, 1100 }, { 0, 1094, 1699, 1701 } } }, Profession::Ranger               },
            { Specialization::NecromancerReaper,        "Reaper",               { { { 0, 1974, 2020, 2026 }, { 0, 1969, 2008, 2031 }, { 0, 1932, 1919, 2021 } } }, Profession::Necromancer,    true },
            { Specialization::ThiefCriticalStrikes,     "Critical Strikes",     { { { 0, 1209, 1267, 1268 }, { 0, 1170, 1272, 1299 }, { 0, 1904, 1215, 1702 } } }, Profession::Thief                },
            { Specialization::WarriorArms,              "Arms",                 { { { 0, 1455, 1344, 1334 }, { 0, 1315, 1316, 1333 }, { 0, 1336, 1346, 1707 } } }, Profession::Warrior              },
            { Specialization::ElementalistArcane,       "Arcane",               { { { 0,  253,  266, 1487 }, { 0,  265, 1673,  257 }, { 0,  238,  263, 1511 } } }, Profession::Elementalist         },
            { Specialization::EngineerFirearms,         "Firearms",             { { { 0, 1878, 1930, 1914 }, { 0, 1984, 2006, 1923 }, { 0,  510,  526,  433 } } }, Profession::Engineer             },
            { Specialization::NecromancerCurses,        "Curses",               { { { 0, 1883, 2013,  815 }, { 0,  816, 1693,  812 }, { 0,  813, 1696,  801 } } }, Profession::Necromancer          },
            { Specialization::MesmerChronomancer,       "Chronomancer",         { { { 0, 1838, 1995, 1987 }, { 0, 2009, 1913, 1978 }, { 0, 1942, 2022, 1890 } } }, Profession::Mesmer,         true },
            { Specialization::ElementalistAir,          "Air",                  { { { 0,  227,  224,  232 }, { 0,  229,  214, 1502 }, { 0,  226, 1503, 1672 } } }, Profession::Elementalist         },
            { Specialization::GuardianZeal,             "Zeal",                 { { { 0,  563,  634, 1925 }, { 0,  628,  653, 1556 }, { 0,  635,  637, 2017 } } }, Profession::Guardian             },
            { Specialization::EngineerScrapper,         "Scrapper",             { { { 0, 1917, 1971, 1867 }, { 0, 1954, 1999, 1860 }, { 0, 1981, 2052, 1849 } } }, Profession::Engineer,       true },
            { Specialization::ThiefTrickery,            "Trickery",             { { { 0, 1159, 1252, 1163 }, { 0, 1277, 1286, 1190 }, { 0, 1187, 1158, 1706 } } }, Profession::Thief                },
            { Specialization::MesmerChaos,              "Chaos",                { { { 0,  670,  675,  677 }, { 0,  673,  668,  669 }, { 0,  671,  674, 1687 } } }, Profession::Mesmer               },
            { Specialization::GuardianVirtues,          "Virtues",              { { { 0,  624,  625,  617 }, { 0,  603,  610,  587 }, { 0,  622,  554,  612 } } }, Profession::Guardian             },
            { Specialization::EngineerInventions,       "Inventions",           { { { 0,  394, 1901,  507 }, { 0, 1678, 1834,  445 }, { 0,  472, 1680, 1916 } } }, Profession::Engineer             },
            { Specialization::ElementalistTempest,      "Tempest",              { { { 0, 1952, 1962, 1886 }, { 0, 1891, 1902, 2015 }, { 0, 1839, 2033, 1986 } } }, Profession::Elementalist,   true },
            { Specialization::GuardianHonor,            "Honor",                { { { 0, 1899,  559,  654 }, { 0,  557,  549,  562 }, { 0,  553,  558, 1682 } } }, Profession::Guardian             },
            { Specialization::NecromancerSoulReaping,   "Soul Reaping",         { { { 0,  875,  898,  888 }, { 0,  894,  861,  892 }, { 0,  889,  893,  905 } } }, Profession::Necromancer          },
            { Specialization::WarriorDiscipline,        "Discipline",           { { { 0, 1329, 1413, 1381 }, { 0, 1484, 1489, 1709 }, { 0, 1369, 1317, 1657 } } }, Profession::Warrior              },
            { Specialization::RevenantHerald,           "Herald",               { { { 0, 1813, 1806, 1716 }, { 0, 1738, 1743, 1730 }, { 0, 1746, 1772, 1803 } } }, Profession::Revenant,       true },
            { Specialization::NecromancerSpite,         "Spite",                { { { 0,  914,  916, 1863 }, { 0,  899,  829,  909 }, { 0,  919,  853,  903 } } }, Profession::Necromancer          },
            { Specialization::ThiefAcrobatics,          "Acrobatics",           { { { 0, 1112, 1289, 1237 }, { 0, 1241, 1192, 1290 }, { 0, 1238, 1295, 1703 } } }, Profession::Thief                },
            { Specialization::RangerSoulbeast,          "Soulbeast",            { { { 0, 2134, 2071, 2072 }, { 0, 2119, 2085, 2161 }, { 0, 2155, 2128, 2143 } } }, Profession::Ranger,         true },
            { Specialization::ElementalistWeaver,       "Weaver",               { { { 0, 2177, 2165, 2115 }, { 0, 2180, 2061, 2170 }, { 0, 2131, 2090, 2138 } } }, Profession::Elementalist,   true },
            { Specialization::EngineerHolosmith,        "Holosmith",            { { { 0, 2114, 2157, 2106 }, { 0, 2103, 2152, 2091 }, { 0, 2066, 2137, 2064 } } }, Profession::Engineer,       true },
            { Specialization::ThiefDeadeye,             "Deadeye",              { { { 0, 2145, 2173, 2136 }, { 0, 2118, 2078, 2160 }, { 0, 2111, 2093, 2146 } } }, Profession::Thief,          true },
            { Specialization::MesmerMirage,             "Mirage",               { { { 0, 2141, 2082, 2110 }, { 0, 2178, 2174, 2098 }, { 0, 2070, 2113, 2169 } } }, Profession::Mesmer,         true },
            { Specialization::NecromancerScourge,       "Scourge",              { { { 0, 2167, 2074, 2102 }, { 0, 2059, 2067, 2123 }, { 0, 2112, 2164, 2080 } } }, Profession::Necromancer,    true },
            { Specialization::WarriorSpellbreaker,      "Spellbreaker",         { { { 0, 2107, 2153, 2140 }, { 0, 2126, 2097, 2095 }, { 0, 2163, 2168, 2060 } } }, Profession::Warrior,        true },
            { Specialization::GuardianFirebrand,        "Firebrand",            { { { 0, 2075, 2101, 2086 }, { 0, 2063, 2076, 2116 }, { 0, 2105, 2179, 2159 } } }, Profession::Guardian,       true },
            { Specialization::RevenantRenegade,         "Renegade",             { { { 0, 2166, 2079, 2120 }, { 0, 2133, 2092, 2108 }, { 0, 2094, 2100, 2182 } } }, Profession::Revenant,       true },
            { Specialization::NecromancerHarbinger,     "Harbinger",            { { { 0, 2188, 2219, 2185 }, { 0, 2192, 2220, 2209 }, { 0, 2218, 2194, 2203 } } }, Profession::Necromancer,    true },
            { Specialization::GuardianWillbender,       "Willbender",           { { { 0, 2191, 2190, 2187 }, { 0, 2197, 2210, 2199 }, { 0, 2195, 2201, 2198 } } }, Profession::Guardian,       true },
            { Specialization::MesmerVirtuoso,           "Virtuoso",             { { { 0, 2212, 2208, 2202 }, { 0, 2215, 2205, 2207 }, { 0, 2211, 2206, 2223 } } }, Profession::Mesmer,         true },
            { Specialization::ElementalistCatalyst,     "Catalyst",             { { { 0, 2230, 2252, 2224 }, { 0, 2247, 2249, 2234 }, { 0, 2233, 2241, 2251 } } }, Profession::Elementalist,   true },
            { Specialization::WarriorBladesworn,        "Bladesworn",           { { { 0, 2237, 2260, 2225 }, { 0, 2253, 2244, 2240 }, { 0, 2261, 2239, 2245 } } }, Profession::Warrior,        true },
            { Specialization::RevenantVindicator,       "Vindicator",           { { { 0, 2258, 2248, 2228 }, { 0, 2259, 2243, 2255 }, { 0, 2257, 2232, 2238 } } }, Profession::Revenant,       true },
        } };
        return instance;
    }
    static SpecializationInfo const& GetSpecializationInfo(Specialization specialization) { return *util::find_if(GetSpecializationInfos(), util::member_equals(&SpecializationInfo::Specialization, specialization)); }

    enum class RevenantLegend : uint8_t
    {
        None,
        Glint,
        Shiro,
        Jallis,
        Mallix,
        Kalla,
        Ventari,
        Alliance,
    };
    struct RevenantLegendInfo
    {
        RevenantLegend Legend = RevenantLegend::None;
        Specialization RequiredSpecialization = Specialization::None;
        bool Terrestrial = true;
        bool Aquatic = true;

        static std::optional<uint8_t> GetSkillIndex(uint32_t palette)
        {
            switch (palette)
            {
                case 4572: return 0;
                case 4614: return 1;
                case 4651: return 2;
                case 4564: return 3;
                case 4554: return 4;
                default: return { };
            }
        }
    };
    static auto const& GetRevenantLegendInfos()
    {
        constexpr static std::array<RevenantLegendInfo, 8> instance
        { {
            { RevenantLegend::None,    Specialization::None,                            },
            { RevenantLegend::Glint,   Specialization::RevenantHerald,                  },
            { RevenantLegend::Shiro,   Specialization::None,                            },
            { RevenantLegend::Jallis,  Specialization::None,                            },
            { RevenantLegend::Mallix,  Specialization::None,                            },
            { RevenantLegend::Kalla,   Specialization::RevenantRenegade,    true, false },
            { RevenantLegend::Ventari, Specialization::None,                true, false },
            { RevenantLegend::Alliance,Specialization::RevenantVindicator,  true, false },
        } };
        return instance;
    }
    static RevenantLegendInfo const& GetRevenantLegendInfo(RevenantLegend legend) { return *util::find_if(GetRevenantLegendInfos(), util::member_equals(&RevenantLegendInfo::Legend, legend)); }

    enum class Rarity : uint8_t
    {
        Junk,
        Basic,
        Fine,
        Masterwork,
        Rare,
        Exotic,
        Ascended,
        Legendary,
    };
    struct RarityInfo
    {
        Rarity Rarity = Rarity::Junk;
        std::string_view Name;
        uint32_t Color;
    };
    static auto const& GetRarityInfos()
    {
        constexpr static std::array<RarityInfo, 8> instance
        { {
            { Rarity::Junk,         "Junk",         0xFFAAAAAA },
            { Rarity::Basic,        "Basic",        0xFFFFFFFF },
            { Rarity::Fine,         "Fine",         0xFFFF9955 },
            { Rarity::Masterwork,   "Masterwork",   0xFF11CC33 },
            { Rarity::Rare,         "Rare",         0xFF22DDFF },
            { Rarity::Exotic,       "Exotic",       0xFF00AAFF },
            { Rarity::Ascended,     "Ascended",     0xFF8844FF },
            { Rarity::Legendary,    "Legendary",    0xFFFF3399 },
        } };
        return instance;
    }
    static RarityInfo const& GetRarityInfo(Rarity rarity) { return *util::find_if(GetRarityInfos(), util::member_equals(&RarityInfo::Rarity, rarity)); }

    enum class Slot : uint8_t
    {
        Helm,
        Shoulders,
        Coat,
        Gloves,
        Leggings,
        Boots,
        Weapon1M,
        Weapon1H,
        Weapon2M,
        Weapon2H,
        Back,
        Accessory1,
        Accessory2,
        Amulet,
        Ring1,
        Ring2,
        HelmAquatic,
        WeaponW1,
        WeaponW2,
    };
    struct SlotInfo
    {
        Slot Slot { };
        std::string_view Name;
    };
    static auto const& GetSlotInfos()
    {
        constexpr static std::array<SlotInfo, 19> instance
        { {
            { Slot::Helm,           "Helm"          },
            { Slot::Shoulders,      "Shoulders"     },
            { Slot::Coat,           "Coat"          },
            { Slot::Gloves,         "Gloves"        },
            { Slot::Leggings,       "Leggings"      },
            { Slot::Boots,          "Boots"         },
            { Slot::Weapon1M,       "Weapon1M"      },
            { Slot::Weapon1H,       "Weapon1H"      },
            { Slot::Weapon2M,       "Weapon2M"      },
            { Slot::Weapon2H,       "Weapon2H"      },
            { Slot::Back,           "Back"          },
            { Slot::Accessory1,     "Accessory1"    },
            { Slot::Accessory2,     "Accessory2"    },
            { Slot::Amulet,         "Amulet"        },
            { Slot::Ring1,          "Ring1"         },
            { Slot::Ring2,          "Ring2"         },
            { Slot::HelmAquatic,    "HelmAquatic"   },
            { Slot::WeaponW1,       "WeaponW1"      },
            { Slot::WeaponW2,       "WeaponW2"      },
        } };
        return instance;
    }
    static SlotInfo const& GetSlotInfo(Slot slot) { return *util::find_if(GetSlotInfos(), util::member_equals(&SlotInfo::Slot, slot)); }
};
}
