﻿#include "Handler.h"
#include "BuildStorage.h"
#include "ChatLink.h"
#include "API.h"
#include "Web.h"
#include "../ImGuiExtensions.h"
#include <imgui/imgui_impl_dx9.h>
#include <inih/INIReader.h>
#include <spng/spng.h>
#include <curlpp/cURLpp.hpp>
#include <json/json.hpp>
#ifdef _DEBUG
#include <base64/base64.h>
#endif

namespace buildpad
{
char const* const BUILDPAD_VERSION = "2019-11-06";

namespace resources
{
#include "resources/155143.h"
#include "resources/156058.h"
#include "resources/156293.h"
#include "resources/156297.h"
#include "resources/156298.h"
#include "resources/156299.h"
#include "resources/156300.h"
#include "resources/156301.h"
#include "resources/156302.h"
#include "resources/156306.h"
#include "resources/156307.h"
#include "resources/156308.h"
#include "resources/156309.h"
#include "resources/156310.h"
#include "resources/156311.h"
#include "resources/156313.h"
#include "resources/156316.h"
#include "resources/156320.h"
#include "resources/156361.h"
#include "resources/156599.h"
#include "resources/156600.h"
#include "resources/156630.h"
#include "resources/156632.h"
#include "resources/156634.h"
#include "resources/156636.h"
#include "resources/156638.h"
#include "resources/156640.h"
#include "resources/156641.h"
#include "resources/156643.h"
#include "resources/156675.h"
#include "resources/156721.h"
#include "resources/156723.h"
#include "resources/156794.h"
#include "resources/157086.h"
#include "resources/157121.h"
#include "resources/157262.h"
#include "resources/157325.h"
#include "resources/255430.h"
#include "resources/523389.h"
#include "resources/536048.h"
#include "resources/536052.h"
#include "resources/961390.h"
#include "resources/993598.h"
#include "resources/993687.h"
#include "resources/1128567.h"
#include "resources/1128569.h"
#include "resources/1128571.h"
#include "resources/1128573.h"
#include "resources/1128575.h"
#include "resources/1128577.h"
#include "resources/1128579.h"
#include "resources/1128581.h"
#include "resources/1128583.h"
#include "resources/1293677.h"
#include "resources/1424219.h"
#include "resources/1441449.h"
#include "resources/1444520.h"
#include "resources/1444524.h"
#include "resources/1535141.h"
#include "resources/1670506.h"
#include "resources/1770211.h"
#include "resources/1770213.h"
#include "resources/1770215.h"
#include "resources/1770217.h"
#include "resources/1770219.h"
#include "resources/1770221.h"
#include "resources/1770223.h"
#include "resources/1770225.h"
#include "resources/1988170.h"
#include "resources/1988171.h"
#include "resources/2030307.h"
#include "resources/ErrorItem.h"
#include "resources/ErrorPet.h"
#include "resources/ErrorSkill.h"
#include "resources/ErrorSpecialization.h"
#include "resources/ErrorTrait.h"
#include "resources/ErrorTraitMinor.h"
#include "resources/LoadingItem.h"
#include "resources/LoadingPet.h"
#include "resources/LoadingSkill.h"
#include "resources/LoadingSpecialization.h"
#include "resources/LoadingTrait.h"
#include "resources/LoadingTraitMinor.h"
}

Handler::Handler()
{
    m_versionCurrent = BUILDPAD_VERSION;
    cURLpp::initialize();
}

Handler::~Handler()
{
    
}

void Handler::LoadTextures()
{
    m_d3dDevice = GetDirect3DDevice9();
    LoadIcon(Icons::BuildPad, resources::tex156721).Trim(2);
    LoadIcon(Icons::AddBuild, resources::tex155143);
    LoadIcon(Icons::EditBuild, resources::tex1293677).Trim(3);
    LoadIcon(Icons::AcceptBuildEdit, resources::tex1444520).Trim(6);
    LoadIcon(Icons::CancelBuildEdit, resources::tex1444524).Trim(6);
    LoadIcon(Icons::ClearSearch, resources::tex157325);
    LoadIcon(Icons::Settings, resources::tex156361);
    LoadIcon(Icons::MissingSkill, resources::tex2030307);
    LoadIcon(Icons::LandSkills, resources::tex1988171);
    LoadIcon(Icons::WaterSkills, resources::tex1988170);
    LoadIcon(Icons::LoadingSkill, resources::texLoadingSkill);
    LoadIcon(Icons::ErrorSkill, resources::texErrorSkill);
    LoadIcon(Icons::MissingSpecialization, resources::tex993598);
    LoadIcon(Icons::LoadingSpecialization, resources::texLoadingSpecialization);
    LoadIcon(Icons::ErrorSpecialization, resources::texErrorSpecialization);
    LoadIcon(Icons::MissingTrait, resources::tex157262);
    LoadIcon(Icons::LoadingTrait, resources::texLoadingTrait);
    LoadIcon(Icons::ErrorTrait, resources::texErrorTrait);
    LoadIcon(Icons::LoadingTraitMinor, resources::texLoadingTraitMinor);
    LoadIcon(Icons::ErrorTraitMinor, resources::texErrorTraitMinor);
    LoadIcon(Icons::LoadingItem, resources::texLoadingItem);
    LoadIcon(Icons::ErrorItem, resources::texErrorItem);
    LoadIcon(Icons::MissingPet, resources::tex156794).Trim(52);
    LoadIcon(Icons::LoadingPet, resources::texLoadingPet).Trim(52);
    LoadIcon(Icons::ErrorPet, resources::texErrorPet).Trim(52);
    LoadIcon(Icons::SelectionChevron, resources::tex156058).Trim(2, 4, 2, 3).FlipVertically();
    LoadIcon(GW2::Profession::None, resources::tex156675);
    LoadIcon(GW2::Profession::Guardian, resources::tex156634);
    LoadIcon(GW2::Profession::Warrior, resources::tex156643);
    LoadIcon(GW2::Profession::Engineer, resources::tex156632);
    LoadIcon(GW2::Profession::Ranger, resources::tex156640);
    LoadIcon(GW2::Profession::Thief, resources::tex1128571);
    LoadIcon(GW2::Profession::Elementalist, resources::tex156630);
    LoadIcon(GW2::Profession::Mesmer, resources::tex156636);
    LoadIcon(GW2::Profession::Necromancer, resources::tex156638);
    LoadIcon(GW2::Profession::Revenant, resources::tex961390);
    LoadIcon(GW2::Specialization::None, resources::tex156675);
    LoadIcon(GW2::Specialization::MesmerDueling, resources::tex156636);
    LoadIcon(GW2::Specialization::NecromancerDeathMagic, resources::tex156638);
    LoadIcon(GW2::Specialization::RevenantInvocation, resources::tex961390);
    LoadIcon(GW2::Specialization::WarriorStrength, resources::tex156643);
    LoadIcon(GW2::Specialization::RangerDruid, resources::tex1128575);
    LoadIcon(GW2::Specialization::EngineerExplosives, resources::tex156632);
    LoadIcon(GW2::Specialization::ThiefDaredevil, resources::tex1128571);
    LoadIcon(GW2::Specialization::RangerMarksmanship, resources::tex156640);
    LoadIcon(GW2::Specialization::RevenantRetribution, resources::tex961390);
    LoadIcon(GW2::Specialization::MesmerDomination, resources::tex156636);
    LoadIcon(GW2::Specialization::WarriorTactics, resources::tex156643);
    LoadIcon(GW2::Specialization::RevenantSalvation, resources::tex961390);
    LoadIcon(GW2::Specialization::GuardianValor, resources::tex156634);
    LoadIcon(GW2::Specialization::RevenantCorruption, resources::tex961390);
    LoadIcon(GW2::Specialization::RevenantDevastation, resources::tex961390);
    LoadIcon(GW2::Specialization::GuardianRadiance, resources::tex156634);
    LoadIcon(GW2::Specialization::ElementalistWater, resources::tex156630);
    LoadIcon(GW2::Specialization::WarriorBerserker, resources::tex1128567);
    LoadIcon(GW2::Specialization::NecromancerBloodMagic, resources::tex156638);
    LoadIcon(GW2::Specialization::ThiefShadowArts, resources::tex156641);
    LoadIcon(GW2::Specialization::EngineerTools, resources::tex156632);
    LoadIcon(GW2::Specialization::WarriorDefense, resources::tex156643);
    LoadIcon(GW2::Specialization::MesmerInspiration, resources::tex156636);
    LoadIcon(GW2::Specialization::MesmerIllusions, resources::tex156636);
    LoadIcon(GW2::Specialization::RangerNatureMagic, resources::tex156640);
    LoadIcon(GW2::Specialization::ElementalistEarth, resources::tex156630);
    LoadIcon(GW2::Specialization::GuardianDragonhunter, resources::tex1128573);
    LoadIcon(GW2::Specialization::ThiefDeadlyArts, resources::tex156641);
    LoadIcon(GW2::Specialization::EngineerAlchemy, resources::tex156632);
    LoadIcon(GW2::Specialization::RangerSkirmishing, resources::tex156640);
    LoadIcon(GW2::Specialization::ElementalistFire, resources::tex156630);
    LoadIcon(GW2::Specialization::RangerBeastmastery, resources::tex156640);
    LoadIcon(GW2::Specialization::RangerWildernessSurvival, resources::tex156640);
    LoadIcon(GW2::Specialization::NecromancerReaper, resources::tex1128579);
    LoadIcon(GW2::Specialization::ThiefCriticalStrikes, resources::tex156641);
    LoadIcon(GW2::Specialization::WarriorArms, resources::tex156643);
    LoadIcon(GW2::Specialization::ElementalistArcane, resources::tex156630);
    LoadIcon(GW2::Specialization::EngineerFirearms, resources::tex156632);
    LoadIcon(GW2::Specialization::NecromancerCurses, resources::tex156638);
    LoadIcon(GW2::Specialization::MesmerChronomancer, resources::tex1128569);
    LoadIcon(GW2::Specialization::ElementalistAir, resources::tex156630);
    LoadIcon(GW2::Specialization::GuardianZeal, resources::tex156634);
    LoadIcon(GW2::Specialization::EngineerScrapper, resources::tex1128581);
    LoadIcon(GW2::Specialization::ThiefTrickery, resources::tex156641);
    LoadIcon(GW2::Specialization::MesmerChaos, resources::tex156636);
    LoadIcon(GW2::Specialization::GuardianVirtues, resources::tex156634);
    LoadIcon(GW2::Specialization::EngineerInventions, resources::tex156632);
    LoadIcon(GW2::Specialization::ElementalistTempest, resources::tex1128583);
    LoadIcon(GW2::Specialization::GuardianHonor, resources::tex156634);
    LoadIcon(GW2::Specialization::NecromancerSoulReaping, resources::tex156638);
    LoadIcon(GW2::Specialization::WarriorDiscipline, resources::tex156643);
    LoadIcon(GW2::Specialization::RevenantHerald, resources::tex1128577);
    LoadIcon(GW2::Specialization::NecromancerSpite, resources::tex156638);
    LoadIcon(GW2::Specialization::ThiefAcrobatics, resources::tex156641);
    LoadIcon(GW2::Specialization::RangerSoulbeast, resources::tex1770215);
    LoadIcon(GW2::Specialization::ElementalistWeaver, resources::tex1670506);
    LoadIcon(GW2::Specialization::EngineerHolosmith, resources::tex1770225);
    LoadIcon(GW2::Specialization::ThiefDeadeye, resources::tex1770213);
    LoadIcon(GW2::Specialization::MesmerMirage, resources::tex1770217);
    LoadIcon(GW2::Specialization::NecromancerScourge, resources::tex1770221);
    LoadIcon(GW2::Specialization::WarriorSpellbreaker, resources::tex1770223);
    LoadIcon(GW2::Specialization::GuardianFirebrand, resources::tex1770211);
    LoadIcon(GW2::Specialization::RevenantRenegade, resources::tex1770219);
    LoadIcon(Build::Flags::Favorite, resources::tex523389).Trim(5);
    LoadIcon(Build::Flags::PvE, resources::tex157086).Trim(2);
    LoadIcon(Build::Flags::PvP, resources::tex157121).Trim(1);
    LoadIcon(Build::Flags::WvW, resources::tex255430).Trim(4);
    LoadIcon(Build::Flags::OpenWorld, resources::tex156723).Trim(1);
    LoadIcon(Build::Flags::Dungeons, resources::tex1535141).Trim(6);
    LoadIcon(Build::Flags::Fractals, resources::tex1441449).Trim(4);
    LoadIcon(Build::Flags::Raids, resources::tex1424219).Trim(6);
    LoadIcon(Build::Flags::Power, resources::tex993687).Trim(1);
    LoadIcon(Build::Flags::Condition, resources::tex156600).Trim(3);
    LoadIcon(Build::Flags::Tank, resources::tex536048).Trim(5);
    LoadIcon(Build::Flags::Support, resources::tex156599).Trim(4);
    LoadIcon(Build::Flags::Heal, resources::tex536052).Trim(4);
    LoadIcon(GW2::Slot::Helm, resources::tex156307).Trim(36);
    LoadIcon(GW2::Slot::Shoulders, resources::tex156311).Trim(36);
    LoadIcon(GW2::Slot::Coat, resources::tex156297).Trim(36);
    LoadIcon(GW2::Slot::Gloves, resources::tex156306).Trim(36);
    LoadIcon(GW2::Slot::Leggings, resources::tex156309).Trim(36);
    LoadIcon(GW2::Slot::Boots, resources::tex156300).Trim(36);
    LoadIcon(GW2::Slot::Weapon1M, resources::tex156316).Trim(36);
    LoadIcon(GW2::Slot::Weapon1H, resources::tex156320).Trim(36);
    LoadIcon(GW2::Slot::Weapon2M, resources::tex156316).Trim(36);
    LoadIcon(GW2::Slot::Weapon2H, resources::tex156320).Trim(36);
    LoadIcon(GW2::Slot::Back, resources::tex156293).Trim(36);
    LoadIcon(GW2::Slot::Accessory1, resources::tex156298).Trim(36);
    LoadIcon(GW2::Slot::Accessory2, resources::tex156299).Trim(36);
    LoadIcon(GW2::Slot::Amulet, resources::tex156310).Trim(36);
    LoadIcon(GW2::Slot::Ring1, resources::tex156301).Trim(36);
    LoadIcon(GW2::Slot::Ring2, resources::tex156302).Trim(36);
    LoadIcon(GW2::Slot::HelmAquatic, resources::tex156308).Trim(36);
    LoadIcon(GW2::Slot::WeaponW1, resources::tex156313).Trim(36);
    LoadIcon(GW2::Slot::WeaponW2, resources::tex156313).Trim(36);

    // SELECT CONCAT('    { "id": ', id, ', "type": ', profession, ', "palette": [', GROUP_CONCAT(DISTINCT palette ORDER BY palette SEPARATOR ', '), '], "name": "', REPLACE(REPLACE(name, '\\', '\\\\'), '"', '\\"'), '" },') FROM skilldefs WHERE profession>=2 GROUP BY id ORDER BY id
    VersionCheck();
    Web::Instance().Request("https://buildpad.gw2archive.eu/skillpalette.json", [this](std::string_view const data)
    {
        for (auto const& skill : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
        {
            auto const id = skill["id"];
            auto& info = m_skills[id];
            info.ID = id;
            info.Type = (Skill::SkillType)skill["type"];
            if (auto itr = std::find_if(GW2::GetSpecializationInfos().begin(), GW2::GetSpecializationInfos().end(),
                [training = (std::string_view)skill["training"]](GW2::SpecializationInfo const& info) { return info.Name == training; }); itr != GW2::GetSpecializationInfos().end())
                info.Specialization = itr->Specialization;
            info.Name = skill["name"];
            for (auto& palette : skill["palette"])
            {
                info.Palettes.push_back(palette);
                m_palettes[palette].push_back(info.ID);
            }
        }

        m_loaded = true;

        m_arcdpsMigrationDiscovery = std::async(std::launch::async, [this]
        {
            fs::path const templatesRoot = "./addons/arcdps/arcdps.templates";
            if (!exists(templatesRoot))
                return;

            Build::id_t nextID = -1;

            static auto foreachTemplate = [templatesRoot](std::string_view type, std::string_view prefix, std::function<void(std::string_view data)> const& handler)
            {
                try
                {
                    if (fs::path const traitsPath = templatesRoot / type; exists(traitsPath))
                    {
                        for (auto const& entry : fs::directory_iterator { traitsPath })
                        {
                            if (entry.is_regular_file() && util::starts_with(entry.path().filename().string(), prefix))
                            {
                                try
                                {
                                    std::string data = (std::ostringstream { } << std::ifstream { entry, std::ifstream::in | std::ifstream::binary }.rdbuf()).str();
                                    handler(data);
                                }
                                catch (...) { }
                            }
                        }
                    }
                }
                catch (...) { }
            };

            foreachTemplate("traits", "tr-", [this, &nextID](std::string_view const data)
            {
                using file_t = ChatLink::ArcDPSFile<ChatLink::ArcDPSTraitTemplate>;
                using code_t = ChatLink::ArcDPSCode<ChatLink::ArcDPSTraitTemplate>;
                if (data.size() < sizeof file_t)
                    return;

                file_t const& file = *reinterpret_cast<file_t const*>(data.data());
                code_t code;
                code.Type = 't';
                code.Data.Profession = file.Data.Profession;
                std::copy(file.Data.Specializations.begin(), file.Data.Specializations.end(), code.Data.Specializations.begin());
                std::copy(file.Data.Traits.begin(), file.Data.Traits.end(), code.Data.Traits.begin());

                Build& build = m_arcdpsTraits.emplace_back(nextID--);
                build.SetName(file.Name.data());
                build.SetLink(ChatLink::Encode(code));
            });

            foreachTemplate("skills", "sk-", [this, &nextID](std::string_view const data)
            {
                using file_t = ChatLink::ArcDPSFile<ChatLink::ArcDPSSkillTemplate>;
                using code_t = ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate>;
                if (data.size() < sizeof file_t)
                    return;

                file_t const& file = *reinterpret_cast<file_t const*>(data.data());
                code_t code;
                code.Type = 's';
                code.Data.Profession = file.Data.Profession;
                std::copy(file.Data.Land.begin(), file.Data.Land.end(), code.Data.Land.begin());
                std::copy(file.Data.Water.begin(), file.Data.Water.end(), code.Data.Water.begin());

                Build& build = m_arcdpsSkills.emplace_back(nextID--);
                build.SetName(file.Name.data());
                build.SetLink(ChatLink::Encode(code));
            });
            if (std::find_if(m_arcdpsTraits.begin(), m_arcdpsTraits.end(), [](Build const& build) { return build.GetParsedProfession() == GW2::Profession::Revenant; }) != m_arcdpsTraits.end())
            {
                static std::array<std::array<uint16_t, 5>, 6> const orders
                { {
                    { 4572, 4564, 4651, 4614, 4554 },
                    { 4572, 4614, 4564, 4651, 4554 },
                    { 4572, 4651, 4614, 4564, 4554 },
                    { 4572, 4651, 4564, 4614, 4554 },
                    { 4572, 4564, 4614, 4651, 4554 },
                    { 4572, 4614, 4651, 4564, 4554 },
                } };
                char const* alphabet = "ABCDEF";
                for (auto const& order : orders)
                {
                    using code_t = ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate>;
                    code_t code;
                    code.Type = 's';
                    code.Data.Profession = (uint16_t)GW2::Profession::Revenant;
                    std::copy(order.begin(), order.end(), code.Data.Land.begin());
                    std::copy(order.begin(), order.end(), code.Data.Water.begin());

                    Build& build = m_arcdpsSkills.emplace_back(nextID--);
                    build.SetName(fmt::format("Default {}", *alphabet++));
                    build.SetLink(ChatLink::Encode(code));
                }
            }
            for (auto& container : { &m_arcdpsTraits, &m_arcdpsSkills })
            {
                container->sort([](Build const& a, Build const& b)
                {
                    if (a.GetParsedProfession() != b.GetParsedProfession())
                        return a.GetParsedProfession() < b.GetParsedProfession();
                    return a.GetNormalizedName() < b.GetNormalizedName();
                });
            }

            foreachTemplate("gear", "ge-", [this](std::string_view const data)
            {
                using file_t = ChatLink::ArcDPSGearTemplateFile<ChatLink::ArcDPSGearTemplate>;
                if (data.size() < sizeof file_t)
                    return;

                file_t const& file = *reinterpret_cast<file_t const*>(data.data());
                m_arcdpsGear.emplace_back(file.Data);
            });
            foreachTemplate("legendary", "le-", [this](std::string_view const data)
            {
                using file_t = ChatLink::ArcDPSGearTemplateFile<ChatLink::ArcDPSLegendaryTemplate>;
                if (data.size() < sizeof file_t)
                    return;

                file_t const& file = *reinterpret_cast<file_t const*>(data.data());
                m_arcdpsGear.emplace_back(file.Data);
            });
            m_arcdpsGear.sort([](auto const& a, auto const& b)
            {
                return std::visit(overloaded
                {
                    [](ArcDPSGearTemplate const& a, ArcDPSGearTemplate const& b)
                    {
                        if (a.Profession != b.Profession)
                            return a.Profession < b.Profession;
                        if (a.Character != b.Character)
                            return a.Character < b.Character;
                        return a.Name < b.Name;
                    },
                    [](ArcDPSLegendaryTemplate const& a, ArcDPSLegendaryTemplate const& b)
                    {
                        if (a.Profession != b.Profession)
                            return a.Profession < b.Profession;
                        return a.Name < b.Name;
                    },
                    [](ArcDPSGearTemplate const& a, ArcDPSLegendaryTemplate const& b)
                    {
                        if (a.Profession != b.Profession)
                            return a.Profession < b.Profession;
                        return true;
                    },
                    [](ArcDPSLegendaryTemplate const& a, ArcDPSGearTemplate const& b)
                    {
                        if (a.Profession != b.Profession)
                            return a.Profession < b.Profession;
                        return false;
                    },
                }, a, b);
            });

            m_arcdpsMigrationAvailable = !m_arcdpsTraits.empty() || !m_arcdpsSkills.empty();
            m_arcdpsGearAvailable = !m_arcdpsGear.empty();
        });
    }, [this](auto const&) { m_loaded = true; }, false);
    Web::Instance().Request("https://buildpad.gw2archive.eu/pets.json", [this](std::string_view const data)
    {
        for (auto const& pet : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
        {
            auto& info = m_pets.emplace_back();
            info.ID = pet["id"];
            info.Type = (Pet::PetType)pet["type"];
            m_petIDs.emplace_back(info.ID);
        }
        m_pets.emplace_back().Type = Pet::PetType::Amphibious;
        m_petsLoaded = true;
    }, [this](auto const&) { m_petsLoaded = true; }, false);
}

fs::path Handler::GetConfigPath()
{
    return "./addons/arcdps/arcdps.buildpad/config.ini";
}

bool Handler::IsSaveNeeded() const
{
    return std::any_of(Config::GetFields().begin(), Config::GetFields().end(), [this](auto const& pair)
    {
        return std::visit(overloaded
        {
            [this](uint32_t(Config::*p))
            {
                if (p == &Config::WindowSizeH && m_config.AutoWindowHeight)
                    return false;

                return m_config.*p != m_savedConfig.*p;
            },
            [this](auto(Config::*p)) { return m_config.*p != m_savedConfig.*p; }
        }, pair.second);
    });
}

bool Handler::Load(std::string_view section, std::string_view name, std::string_view value)
{
    if (section == "Settings"sv)
    {
        if (auto const itr = std::find_if(Config::GetFields().begin(), Config::GetFields().end(), [&name](auto const& pair) { return pair.first == name; }); itr != Config::GetFields().end())
        {
            std::visit(overloaded
            {
                [this, &value](bool(Config::*p)) { m_config.*p = (bool)util::from_string<uint32_t>(value); },
                [this, &value](std::string(Config::*p)) { m_config.*p = value; },
                [this, &value](std::array<uint32_t, 10>(Config::*p))
                {
                    auto values = util::split(value, ",");
                    for (size_t i = 0, max = std::min<size_t>(values.size(), (m_config.*p).size()); i < max; ++i)
                    {
                        if (!values[i].empty() && values[i][0] == '#')
                            (m_config.*p)[i] = util::reverse_bytes(util::from_string<uint32_t>(values[i].substr(1), 16));
                    }
                },
                [this, &value](auto(Config::*p)) { m_config.*p = util::from_string<std::remove_reference_t<decltype(m_config.*p)>>(value); }
            }, itr->second);
        }
        return true;
    }
    return false;
}

void Handler::LoadConfig()
{
    FILE* file;
    if (_wfopen_s(&file, GetConfigPath().c_str(), L"r"))
    {
        API::Instance().SetLanguage(m_config.APILocale);
        return;
    }
    auto guard = gsl::finally([file] { fclose(file); });

    m_config = { };

    BuildStorage::Instance().LoadBegin();
    if (ini_parse_file(file, [](void* user, const char* section, const char* name, const char* value) -> int
    {
        if (!Instance().Load(section, name, value))
            BuildStorage::Instance().Load(section, name, value);

        return 1;
    }, this))
    {
        m_config = m_savedConfig;
        API::Instance().SetLanguage(m_config.APILocale);
        return;
    }

    BuildStorage::Instance().LoadEnd();
    m_savedConfig = m_config;

    API::Instance().SetLanguage(m_config.APILocale);
    m_keyBindToggleBuilds.FromString(m_config.KeyBindToggleBuilds);
}

void Handler::SaveConfig()
{
    if (!IsSaveNeeded() && !BuildStorage::Instance().IsSaveNeeded())
        return;

    fs::path const backupPath = "./addons/arcdps/arcdps.buildpad/backups";
    fs::path const savePath = GetConfigPath();
    fs::path tempPath = savePath;
    tempPath += ".tmp";

    try
    {
        create_directories(backupPath);
        create_directories(savePath.parent_path());
    }
    catch (...) { return; }

    std::ofstream file;
    file.open(tempPath.c_str());

    BuildStorage::Instance().Save(file);

    file << "\n[Settings]\n";
    for (auto&& [name, field] : Config::GetFields())
    {
        file << fmt::format("{} = {}\n", name, std::visit(overloaded
        {
            [this](bool(Config::*p)) { return m_config.*p ? "1"s : "0"s; },
            [this](std::string(Config::*p)) { return m_config.*p; },
            [this](std::array<uint32_t, 10>(Config::*p))
            {
                std::array<std::string, 10> values;
                std::transform((m_config.*p).begin(), (m_config.*p).end(), values.begin(), [](uint32_t const& value) { return fmt::format("#{:X}", util::reverse_bytes(value)); });
                return format("{}", fmt::join(values.begin(), values.end(), ","));
            },
            [this](auto(Config::*p)) { return std::to_string(m_config.*p); }
        }, field));
    }

    file.close();
    m_savedConfig = m_config;

    try
    {
        // Move files
        if (exists(savePath))
            fs::rename(savePath, backupPath / fmt::format("{}.ini", time(nullptr)));
        fs::rename(tempPath, savePath);

        // Cleanup backups
        std::set<time_t> backups;
        for (auto const& entry : fs::directory_iterator { backupPath })
            if (entry.is_regular_file())
                if (fs::path path = entry.path().filename(); path.extension() == ".ini")
                    if (auto const timestamp = util::from_string<time_t>(path.replace_extension().string()))
                        backups.emplace(timestamp);

        while (backups.size() > 10)
        {
            fs::remove(backupPath / fmt::format("{}.ini", *backups.begin()));
            backups.erase(backups.begin());
        }
    }
    catch (...) { }
}

void Handler::LoadTest()
{
    m_shown = true;
}

Time constexpr BUILD_FADE_DURATION_FADE = 200ms;
Time constexpr BUILD_FADE_DURATION_FULL = 750ms;
Time constexpr LINK_PASTED_FADE_DURATION_FADE = 200ms;
Time constexpr LINK_PASTED_FADE_DURATION_FULL = 1000ms;
Time constexpr MIGRATION_SUCCESS_FADE_DURATION_FADE = 200ms;
Time constexpr MIGRATION_SUCCESS_FADE_DURATION_FULL = 750ms;
float UI_SCALE = 1.0f;
ImVec2 LINE_SIZE { };
ImVec2 WINDOW_PADDING { };
ImVec2 FRAME_PADDING { };
ImVec2 ITEM_SPACING { };
ImVec2 ITEM_INNER_SPACING { };
float INDENT_SPACING { };
ImVec2 BUTTON_SIZE { 19, 19 };
ImVec4 BUTTON_COLOR { };
ImVec4 BUTTON_COLOR_ACTIVE { };
ImVec4 BUTTON_COLOR_HOVERED { };

// ReSharper disable CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore
#pragma warning(suppress: 4455) // I disagree with the standard, so sue me.
float operator"" px(unsigned long long int const value) { return (float)(signed long long int)value * UI_SCALE; }
// ReSharper restore CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore

bool Handler::HandleKeyBinds()
{
    if (!m_loaded)
        return false;

    BuildStorage& storage = BuildStorage::Instance();

    if (m_keyBindToggleBuilds.IsPressed())
    {
        m_shown ^= true;
        if (!m_keyBindToggleBuilds.PassThrough)
            return ImGui::CaptureKeyboardFromApp(), true;
    }
    for (auto const& build : BuildStorage::Instance().GetBuilds())
    {
        if (KeyBind const& keyBind = build.GetKeyBind(); keyBind && storage.GetCurrentProfession() != GW2::Profession::None && build.GetParsedProfession() == storage.GetCurrentProfession() && keyBind.IsPressed())
        {
            ImGui::SetClipboardText(fmt::format("{}", build.GetLink()).c_str());
            if (!keyBind.PassThrough)
                return ImGui::CaptureKeyboardFromApp(), true;
        }
    }

    return false;
}

void Handler::Update()
{
    if (!m_loaded)
        return;

    BuildStorage& storage = BuildStorage::Instance();

    if (m_config.ClearFiltersOnWindowClose && !m_shown)
    {
        if (storage.IsFilteringName())
            storage.SetNameFilter({});

        if (storage.IsFilteringFlags())
            for (auto const& info : Build::GetFlagInfos())
                if (storage.IsFilteringFlag(info.Flag))
                    storage.ToggleFlagFilter(info.Flag, false);
    }

    if (!m_shown &&
        !m_arcdpsMigrationShown &&
        !m_arcdpsGearShown &&
        !m_detachSettings &&
        !m_versionHistoryShown &&
        (m_versionUpdateState == VersionUpdateState::Idle || m_versionUpdateState == VersionUpdateState::Done || m_versionUpdateState == VersionUpdateState::Error) &&
        !m_aboutShown)
        return;

    {
        std::lock_guard lock { m_mainThreadQueueLock };
        m_mainThreadQueueExecuting = true;
        while (!m_mainThreadQueue.empty())
        {
            m_mainThreadQueueRepeat = false;
            auto& task = m_mainThreadQueue.front();
            task();
            if (m_mainThreadQueueRepeat)
                OnMainThread(std::move(task));
            m_mainThreadQueue.pop_front();
        }
        m_mainThreadQueueExecuting = false;

        {
            std::lock_guard tailLock { m_mainThreadQueueTailLock };
            std::move(m_mainThreadQueueTail.begin(), m_mainThreadQueueTail.end(), std::back_inserter(m_mainThreadQueue));
            m_mainThreadQueueTail.clear();
        }
    }

    if (m_previousUpdate == Clock::time_point { })
        m_previousUpdate = Clock::now();

    Clock::time_point const prev = std::exchange(m_previousUpdate, Clock::now());
    Time const delta = std::max<Time>(1ms, std::chrono::duration_cast<Time>(m_previousUpdate - prev));

    if (m_mumbleLinkUpdateTimer <= delta)
    {
        m_mumbleLinkUpdateTimer = 1s;
        if (m_mumbleLink.Update())
            storage.SetCurrentProfession(m_mumbleLink.GetProfession());
    }
    else
        m_mumbleLinkUpdateTimer -= delta;

#ifdef _DEBUG
    if (delta.count() < 10)
        Sleep(10 - (DWORD)delta.count());
#endif

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.back());
    UI_SCALE = GetUIScale();
    LINE_SIZE = ImGui::CalcTextSize("") * UI_SCALE;
    WINDOW_PADDING = ImGui::GetStyle().WindowPadding;
    FRAME_PADDING = ImGui::GetStyle().FramePadding;
    ITEM_SPACING = ImGui::GetStyle().ItemSpacing;
    ITEM_INNER_SPACING = ImGui::GetStyle().ItemInnerSpacing;
    INDENT_SPACING = ImGui::GetStyle().IndentSpacing;
    BUTTON_SIZE.x = BUTTON_SIZE.y = FRAME_PADDING.y + LINE_SIZE.y + FRAME_PADDING.y;
    BUTTON_COLOR = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button));
    BUTTON_COLOR_ACTIVE = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_ButtonActive));
    BUTTON_COLOR_HOVERED = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_ButtonHovered));

    #pragma region Debug Window
#ifdef _DEBUG
    ImGui::Begin("[DEBUG]", &m_shown);
    ImGui::SetWindowFontScale(UI_SCALE);
    int prof = (int)storage.GetCurrentProfession();
    ImGui::SliderInt("[TEST PROF]", &prof, 0, 9);
    storage.SetCurrentProfession((GW2::Profession)prof);
    UpdateOptions();
    if (ImGui::Button("Default ArcDPS Style"))
    {
        std::copy_n(base64_decode("AACAPwAAgEAAAIBAAAAAQgAAAAAAAAAAAAAAAAAAAD8AAAAAAACAQAAAgEAAAAAAAACgQAAAQEAAAKBAAABAQAAAAAAAAAAAAADIQQAAwEAAABBBAAAAAAAAyEEAAAAAAAAAPwAAAD8AALBBAACwQQAAgEAAAIBAAQEAAAAAoD8=").begin(), sizeof ImGuiStyle - sizeof(ImVec4) * ImGuiCol_COUNT, (char*)&ImGui::GetStyle());
        std::copy_n(base64_decode("zcxMP83MTD/helQ/AACAP4/CdT4fhWs+4XqUPgAAgD+PwnU9zcxMPSlcjz0AAEA/KVyPPSlcjz3sUbg9AAAAAClcjz0pXI897FG4PZqZWT8K1yM/hesRPzMzMz+PwnU+CtcjP1K4Hj8fhSs/AAAAAM3MzD3sUbg9j8L1PQAAQD+PwnU+H4VrPuF6lD4AAEA/KVwPPylcDz/hehQ/AABAP83MzD3sUbg9j8L1PZqZWT/NzMw97FG4PY/C9T2amVk/zczMPexRuD2PwvU9mplZP83MzD3sUbg9j8L1PTMzMz/NzMw97FG4PY/C9T3NzEw/H4XrPmZm5j7Xo/A+FK5HPx+FKz8fhSs/16MwPxSuRz8Urkc/FK5HP83MTD8Urkc/XI9CPuxROD49Clc+zcxMP83MTD/NzEw/4XpUP1K4nj7NzEw/zcxMP+F6VD9SuJ4+j8J1Pc3MTD0pXI89AACAP4/C9T2uR+E9mpkZPmZmZj8pXI8+cT2KPpqZmT5mZmY/ZmbmPq5H4T6PwvU+ZmZmP+xRuD7sUbg+XI/CPjMzMz/sUbg+7FG4PlyPwj4zM7M+7FG4PuxRuD5cj8I+MzMzP4/CdT4fhWs+4XqUPgAAgD+PwnU+H4VrPuF6lD4AAIA/j8J1Ph+Faz7hepQ+AACAPwAAAAAAAAAAAAAAAAAAAAApXA8/KVwPP+F6FD8AAIA/j8J1Pc3MTD0pXI89AACAP83MzD4Ursc+XI/CPgAAAADNzMw+FK7HPlyPwj4AAAAAzczMPhSuxz5cj8I+AAAAADMzMz97FC4/w/UoP4/C9T4AAIA+AACAPwAAAAAAAIA/MzMzP3sULj/D9Sg/j8L1PgAAgD4AAIA/AAAAAAAAgD/sUbg+7FG4PlyPwj4zM7M+AACAP0jhej8zM3M/SOE6Pw==").begin(), sizeof(ImVec4) * ImGuiCol_COUNT, (char*)&ImGui::GetStyle().Colors);
    }
    if (ImGui::Button("My ArcDPS Style"))
    {
        std::copy_n(base64_decode("AACAPwAAwEAAAKBAAAAAQgAAAAAAAEBAAAAAAAAAAD8AAKBAAACAQAAAgEAAAABAAACgQAAAAEAAAKBAAABAQAAAAAAAAAAAAADIQQAAwEAAAHBBAADIQgAAoEAAAAAAAAAAPwAAAD8AALBBAACwQQAAgEAAAIBAAQEAAAAAoD8=").begin(), sizeof ImGuiStyle - sizeof(ImVec4) * ImGuiCol_COUNT, (char*)&ImGui::GetStyle());
        std::copy_n(base64_decode("zcxMP83MTD/helQ/AACAP4/CdT4fhWs+4XqUPgAAgD8AAAAAAAAAAAAAAACamVk/KVyPPSlcjz3sUbg9AAAAAClcjz0pXI897FG4PZqZWT8K1yM/hesRPzMzMz+PwnU+CtcjP1K4Hj8fhSs/AAAAAAAAAAAAAAAAAAAAAAAAQD+PwnU+H4VrPuF6lD4AAEA/KVwPPylcDz/hehQ/AABAP83MzD3sUbg9j8L1PZqZWT/NzMw97FG4PY/C9T2amVk/zczMPexRuD2PwvU9mplZP83MzD3sUbg9j8L1PTMzMz/NzMw97FG4PY/C9T0AAAAAzcxMP83MTD/helQ/UriePuxRuD7sUbg+XI/CPhSuRz8fhes+ZmbmPtej8D4Urkc/XI9CPuxROD49Clc+zcxMP83MTD/NzEw/4XpUP1K4nj7NzEw/zcxMP+F6VD9SuJ4+j8J1Pc3MTD0pXI89AACAP4/C9T2uR+E9mpkZPmZmZj8pXI8+cT2KPpqZmT5mZmY/ZmbmPq5H4T6PwvU+ZmZmP+xRuD7sUbg+XI/CPjMzMz/sUbg+7FG4PlyPwj4zM7M+7FG4PuxRuD5cj8I+MzMzP4/CdT4fhWs+4XqUPgAAgD+PwnU+H4VrPuF6lD4AAIA/j8J1Ph+Faz7hepQ+AACAPwAAAAAAAAAAAAAAAAAAAAApXA8/KVwPP+F6FD8AAIA/j8J1Pc3MTD0pXI89AACAP83MzD4Ursc+XI/CPgAAAADNzMw+FK7HPlyPwj4AAIA/zczMPhSuxz5cj8I+AAAAADMzMz97FC4/w/UoP4/C9T4AAIA+AACAPwAAAAAAAIA/MzMzP3sULj/D9Sg/j8L1PgAAgD4AAIA/AAAAAAAAgD/sUbg+7FG4PlyPwj4zM7M+AACAP0jhej8zM3M/SOE6Pw==").begin(), sizeof(ImVec4) * ImGuiCol_COUNT, (char*)&ImGui::GetStyle().Colors);
    }
    ImGui::End();
#endif
    #pragma endregion

    float const originalWindowTitleAlign = std::exchange(ImGui::GetStyle().WindowTitleAlign.x, 0.5f);
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDarkening, { 0, 0, 0, ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_ModalWindowDarkening)).w });

    if (m_shown)
        RenderMainWindow(delta);

    if (m_arcdpsMigrationShown)
    {
        ImGui::OpenPopup("BuildPad##ArcDPSMigration");
        ImGui::SetNextWindowSizeConstraints({ 750px, 650px }, { 10000px, 10000px });
        if (ImGui::BeginPopupModal("BuildPad##ArcDPSMigration", &m_arcdpsMigrationShown, ImGuiWindowFlags_NoScrollbar))
        {
            ImGui::SetWindowFontScale(UI_SCALE);
            if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
            {
                m_arcdpsMigrationShown = false;
                ImGui::CaptureKeyboardFromApp();
            }
            auto const oldUseProfessionColors               = std::exchange(m_config.UseProfessionColors,               true);
            auto const oldProfessionColorsUnfilteredOnly    = std::exchange(m_config.ProfessionColorsUnfilteredOnly,    false);
            auto const oldSortBuildsByProfession            = std::exchange(m_config.SortBuildsByProfession,            true);
            auto const oldSortBuildsByProfessionShowIcons   = std::exchange(m_config.SortBuildsByProfessionShowIcons,   true);
            auto const oldSortBuildsByProfessionShowNames   = std::exchange(m_config.SortBuildsByProfessionShowNames,   true);
            auto const oldSortBuildsByProfessionSpacing     = std::exchange(m_config.SortBuildsByProfessionSpacing,     8);
            RenderArcDPSMigration(delta);
            m_config.UseProfessionColors                = oldUseProfessionColors;
            m_config.ProfessionColorsUnfilteredOnly     = oldProfessionColorsUnfilteredOnly;
            m_config.SortBuildsByProfession             = oldSortBuildsByProfession;
            m_config.SortBuildsByProfessionShowIcons    = oldSortBuildsByProfessionShowIcons;
            m_config.SortBuildsByProfessionShowNames    = oldSortBuildsByProfessionShowNames;
            m_config.SortBuildsByProfessionSpacing      = oldSortBuildsByProfessionSpacing;
            ImGui::EndPopup();
        }
    }

    if (m_arcdpsGearShown)
    {
        ImGui::SetNextWindowSizeConstraints({ 750px, 600px }, { 10000px, 10000px });
        ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
        ImGui::Begin("BuildPad##ArcDPSGear", &m_arcdpsGearShown, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
        ImGui::SetWindowFontScale(UI_SCALE);
        if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
        {
            m_arcdpsGearShown = false;
            ImGui::CaptureKeyboardFromApp();
        }
        auto const oldUseProfessionColors               = std::exchange(m_config.UseProfessionColors,               true);
        auto const oldProfessionColorsUnfilteredOnly    = std::exchange(m_config.ProfessionColorsUnfilteredOnly,    false);
        auto const oldSortBuildsByProfession            = std::exchange(m_config.SortBuildsByProfession,            true);
        auto const oldSortBuildsByProfessionShowIcons   = std::exchange(m_config.SortBuildsByProfessionShowIcons,   true);
        auto const oldSortBuildsByProfessionShowNames   = std::exchange(m_config.SortBuildsByProfessionShowNames,   true);
        auto const oldSortBuildsByProfessionSpacing     = std::exchange(m_config.SortBuildsByProfessionSpacing,     8);
        RenderArcDPSGear(delta);
        m_config.UseProfessionColors                = oldUseProfessionColors;
        m_config.ProfessionColorsUnfilteredOnly     = oldProfessionColorsUnfilteredOnly;
        m_config.SortBuildsByProfession             = oldSortBuildsByProfession;
        m_config.SortBuildsByProfessionShowIcons    = oldSortBuildsByProfessionShowIcons;
        m_config.SortBuildsByProfessionShowNames    = oldSortBuildsByProfessionShowNames;
        m_config.SortBuildsByProfessionSpacing      = oldSortBuildsByProfessionSpacing;
        ImGui::End();
    }

    if (m_versionHistoryShown)
        RenderVersionHistory();

    if (m_aboutShown)
        RenderAbout();

    if (m_detachSettings)
    {
        ImGui::Begin("BuildPad Settings", &m_detachSettings, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
        ImGui::SetWindowFontScale(UI_SCALE);
        if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
        {
            m_detachSettings = false;
            ImGui::CaptureKeyboardFromApp();
        }
        RenderSettings(false);
        ImGui::End();
    }

    ImGui::PopStyleColor();
    ImGui::GetStyle().WindowTitleAlign.x = originalWindowTitleAlign;
    ImGui::PopFont();
}

void Handler::UpdateOptions()
{
    if (!m_loaded)
        return;

    if (TextureData const& icon = GetIcon(Icons::BuildPad);
        ImGui::ImageButtonWithText(icon.Texture, "BuildPad ", { ImGui::GetContentRegionAvailWidth(), 0 }, icon.TrimmedSize() * 0.75f, icon.GetUV0(), icon.GetUV1(), -1, 3))
        m_shown ^= true;
}

void Handler::RenderMainWindow(Time const& delta)
{
    BuildStorage& storage = BuildStorage::Instance();

    static Build::id_t m_fadingBuild { };
    static Time m_fadingBuildProgress { };
    if (m_fadingBuildProgress.count())
    {
        if (m_fadingBuildProgress <= delta)
        {
            m_fadingBuild = { };
            m_fadingBuildProgress = { };
        }
        else
            m_fadingBuildProgress -= delta;
    }

    static Build::id_t m_tooltipBuild { };
    static bool m_tooltipShown = false;
    static Time m_tooltipDelay { };
    m_tooltipShown = false;
    if (m_tooltipBuild)
    {
        if (m_tooltipDelay <= delta)
        {
            m_tooltipShown = true;
            m_tooltipDelay = { };
        }
        else
            m_tooltipDelay -= delta;
    }

    static Build::id_t copyAsTextWorkingPopupBuildID { };
    static Build::id_t copyAsTextClosePopupBuildID { };
    bool copyAsTextExistsPopupBuildID = false;

    static Build::id_t m_focusNewBuild { };

    static Build::id_t m_linkPastedFadingBuild { };
    static Time m_linkPastedFadingProgress { };
    if (m_linkPastedFadingProgress.count())
    {
        if (m_linkPastedFadingProgress <= delta)
        {
            m_linkPastedFadingBuild = { };
            m_linkPastedFadingProgress = { };
        }
        else
            m_linkPastedFadingProgress -= delta;
    }

    std::function<void()> postAction;

    #pragma region BuildPad Window
    {
        auto flags = (ImGuiWindowFlags_)ImGuiWindowFlags_NoCollapse;
        if (m_config.LockWindowPosition)
        {
            ImGui::SetNextWindowPos({ (float)m_config.WindowPositionX, (float)m_config.WindowPositionY }, ImGuiSetCond_Always);
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoMove);
        }
        else
            ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
        if (m_config.LockWindowSize)
        {
            ImGui::SetNextWindowSize({ (float)m_config.WindowSizeW, (float)m_config.WindowSizeH }, ImGuiSetCond_Always);
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoResize);
        }
        else
            ImGui::SetNextWindowSize({ 300px, 400px }, ImGuiSetCond_FirstUseEver);
        if (m_config.HideWindowHeader)
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoTitleBar);
        if (m_config.AutoWindowHeight)
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetNextWindowSizeConstraints({ 150px, 0 }, ImGui::GetIO().DisplaySize);
        ImGui::Begin("BuildPad", &m_shown, flags);
        ImGui::SetWindowFontScale(UI_SCALE);
        if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
        {
            m_shown = false;
            ImGui::CaptureKeyboardFromApp();
        }
        if (m_config.KeepWindowInBounds)
        {
            if (ImGui::GetWindowPos().x < 0 || ImGui::GetWindowPos().x + ImGui::GetWindowSize().x > ImGui::GetIO().DisplaySize.x ||
                ImGui::GetWindowPos().y < 0 || ImGui::GetWindowPos().y + ImGui::GetWindowSize().y > ImGui::GetIO().DisplaySize.y)
            {
                ImGui::SetWindowPos({ std::clamp<float>(ImGui::GetWindowPos().x, 0, std::max<float>(0, ImGui::GetIO().DisplaySize.x - ImGui::GetWindowSize().x)),
                                      std::clamp<float>(ImGui::GetWindowPos().y, 0, std::max<float>(0, ImGui::GetIO().DisplaySize.y - ImGui::GetWindowSize().y)) }, ImGuiSetCond_Always);
            }
        }
        m_config.WindowPositionX = (int32_t)ImGui::GetWindowPos().x;
        m_config.WindowPositionY = (int32_t)ImGui::GetWindowPos().y;
        m_config.WindowSizeW = (uint32_t)ImGui::GetWindowSize().x;
        if (!ImGui::IsWindowCollapsed())
            m_config.WindowSizeH = (uint32_t)ImGui::GetWindowSize().y;
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
        {
            ImRect const rect = ImGui::GetCurrentWindow()->TitleBarRect();
            if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max, false))
                ImGui::OpenPopup("##BuildPadContextMenu");
        }
        if (ImGui::BeginPopup("##BuildPadContextMenu"))
        {
            ImGui::SetWindowFontScale(UI_SCALE);
            RenderSettings(true);
            ImGui::EndPopup();
        }
    }
    #pragma endregion

    if ((m_arcdpsMigrationAvailable || m_arcdpsGearAvailable) && !m_config.ArcDPSMigrationHintHidden)
    {
        ImGui::BeginGroup();
        ImGui::TextWrapped("ArcDPS templates were detected on your drive");
        if (m_arcdpsMigrationAvailable && ImGui::Button("Migrate Traits/Skills Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("Migrate Traits/Skills Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
            m_arcdpsMigrationShown = true;
        if (m_arcdpsGearAvailable && ImGui::Button("View Equipment/Legendary Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("View Equipment/Legendary Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
            m_arcdpsGearShown = true;
        if (ImGui::Button("Dismiss##ArcDPSMigrationHint"))
            m_config.ArcDPSMigrationHintHidden = true;
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("You can always access these options from the settings menu");
        ImGui::EndGroup();
        ImGui::NewLine();

        ImVec2 const offset { 2px, 2px };
        ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetItemRectMin() - offset, ImVec2 { ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvailWidth(), ImGui::GetItemRectMax().y } +offset, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildWindowRounding);
    }

    #pragma region Hidden Filter Hint
    if (!m_config.HiddenFiltersHintHidden)
    {
        if (storage.IsFilteringProfession() && (!m_config.ShowFilterPanel || !m_config.ShowProfessionFilter) ||
            storage.IsFilteringName() && (!m_config.ShowFilterPanel || !m_config.ShowNameFilter) ||
            storage.IsFilteringFlags() && (!m_config.ShowFilterPanel || !m_config.ShowFlagsFilter) ||
            std::any_of(Build::GetFlagInfos().begin(), Build::GetFlagInfos().end(), [this, &storage](auto const& info) { return storage.IsFilteringFlag(info.Flag) && m_config.HideFlagsMask & (uint32_t)info.Flag; }))
        {
            ImGui::BeginGroup();
            ImGui::TextWrapped("You are using one or more filters that are currently hidden. You might not be able to see all your builds.");
            if (ImGui::Button("Clear Filters"))
            {
                if (storage.IsFilteringProfession() && (!m_config.ShowFilterPanel || !m_config.ShowProfessionFilter))
                    storage.ToggleProfessionFilter(false);
                if (storage.IsFilteringName() && (!m_config.ShowFilterPanel || !m_config.ShowNameFilter))
                    storage.SetNameFilter({});
                for (auto const& info : Build::GetFlagInfos())
                    if (storage.IsFilteringFlag(info.Flag) && (!m_config.ShowFilterPanel || !m_config.ShowFlagsFilter || m_config.HideFlagsMask & (uint32_t)info.Flag))
                        storage.ToggleFlagFilter(info.Flag, false);
            }
            ImGui::SameLine();
            ImGui::Checkbox("Don't warn me", &m_config.HiddenFiltersHintHidden);
            ImGui::EndGroup();
            ImGui::NewLine();

            ImVec2 const offset { 2px, 2px };
            ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetItemRectMin() - offset, ImVec2 { ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvailWidth(), ImGui::GetItemRectMax().y } + offset, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildWindowRounding);
        }
    }
    #pragma endregion

    if (m_needsUpdate && (m_versionLatest.Version != m_config.SkipUpdateVersion || m_versionUpdateState != VersionUpdateState::Idle))
    {
        switch (m_versionUpdateState)
        {
            case VersionUpdateState::Idle:
            {
                ImGui::TextWrapped("There is a new version available: %s", m_versionLatest.Version.c_str());
                bool on = m_config.SkipUpdateVersion == m_versionLatest.Version;
                if (ImGui::Checkbox("Stop asking about this version", &on))
                    m_config.SkipUpdateVersion = m_versionLatest.Version;
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip("You can always update through the settings menu");

                if (ImGui::Button("Update"))
                    VersionUpdate();
                ImGui::SameLine();
                break;
            }
            case VersionUpdateState::Downloading:
                ImGui::TextWrapped("Updating to %s...", m_versionLatest.Version.c_str());
                break;
            case VersionUpdateState::Manifesting:
                ImGui::TextWrapped("Finalizing...");
                break;
            case VersionUpdateState::Done:
                ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "Updated to %s!", m_versionLatest.Version.c_str());
                ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "Restart Guild Wars 2 to apply the update.");
                if (ImGui::Button("Dismiss##VersionUpdate"))
                    m_needsUpdate = false;
                ImGui::SameLine();
                break;
            case VersionUpdateState::Error:
                ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Update to %s failed", m_versionLatest.Version.c_str());
                ImGui::TextWrapped("Error: %s", m_versionUpdateError.c_str());
                if (ImGui::Button("Retry"))
                    VersionUpdate();
                ImGui::SameLine();
                break;
        }
        if (ImGui::Button("Version History"))
            m_versionHistoryShown = true;
        ImGui::Separator();
        ImGui::NewLine();
    }

    auto const renderSettingsButton = [this](float spacing)
    {
        ImGui::SameLine(0, spacing);
        bool on = []
        {
            ImGuiContext& g = *ImGui::GetCurrentContext();
            return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == ImGui::GetCurrentWindow()->GetID("##BuildPadContextMenu");
        }();
        if (TextureData const& icon = GetIcon(Icons::Settings);
            ImGui::CheckboxImage(icon.Texture, "##Settings", &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
            ImGui::OpenPopup("##BuildPadContextMenu");

        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Can also be accessed by right-clicking the window header");
    };

    #pragma region Filter Panel
    if (m_config.ShowFilterPanel)
    {
        #pragma region Profession Filter Toggle
        if (!(m_config.ShowProfessionFilter && !storage.CanFilterProfession() && !m_config.ShowNameFilter && !m_config.ShowFlagsFilter))
            ImGui::SameLine(0, WINDOW_PADDING.x);
        if (m_config.ShowProfessionFilter && storage.CanFilterProfession())
        {
            bool on = storage.IsFilteringProfession();
            if (TextureData const& icon = GetIcon(storage.GetCurrentProfession());
                ImGui::CheckboxImage(icon.Texture, "##ToggleProfessionFilter", &on, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                storage.ToggleProfessionFilter(on);
            if (ImGui::IsItemHovered())
                ImGui::Tooltip("Only show builds for my profession");

            if (m_config.ShowSettingsButton && !m_config.ShowNameFilter && !m_config.ShowFlagsFilter)
            {
                ImGui::SameLine(0, 0);
                renderSettingsButton(ImGui::GetContentRegionAvailWidth() - WINDOW_PADDING.x - BUTTON_SIZE.x);
            }
            else
            {
                ImGui::SameLine(0, ITEM_INNER_SPACING.x);
                if (!m_config.ShowNameFilter && !m_config.ShowFlagsFilter)
                    ImGui::NewLine();
            }
        }
        #pragma endregion
        #pragma region Name Filter
        if (m_config.ShowNameFilter)
        {
            float const x = ImGui::GetCursorPosX();
            auto buffer = util::to_buffer(storage.GetNameFilter());
            if (ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - WINDOW_PADDING.x - (storage.IsFilteringName() || m_config.ShowSettingsButton ? ITEM_INNER_SPACING.x + BUTTON_SIZE.x : 0));
                ImGui::InputText("##FilterName", buffer.data(), buffer.size()))
                storage.SetNameFilter(buffer.data());
            ImGui::PopItemWidth();
            if (storage.IsFilteringName())
            {
                // Clear Name Filter
                ImGui::SameLine(0, ITEM_INNER_SPACING.x);
                bool on = true;
                if (TextureData const& icon = GetIcon(Icons::ClearSearch);
                    ImGui::CheckboxImage(icon.Texture, "##ClearSearch", &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                    storage.SetNameFilter({});
            }
            else
            {
                if (m_config.ShowSettingsButton)
                    renderSettingsButton(ITEM_INNER_SPACING.x);

                // Name Filter Placeholder
                ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                color.w /= 4;
                ImGui::SameLine(x, FRAME_PADDING.x);
                ImGui::TextColored(color, "Search");
            }
            if (m_config.ShowFlagsFilter)
                ImGui::NewLine();
        }
        #pragma endregion

        #pragma region Flags Filter Toggles
        if (m_config.ShowFlagsFilter)
        {
            ImGui::SameLine(0, 0);
            uint32_t inlineProfessionFiler = storage.CanFilterProfession() && m_config.ShowProfessionFilter && !m_config.ShowNameFilter ? 1 : 0;
            uint32_t inlineSettingsButton = m_config.ShowSettingsButton && !m_config.ShowNameFilter ? 1 : 0;
            float separatorWidth = ImGui::GetContentRegionAvailWidth() - (m_config.ShowNameFilter || !inlineProfessionFiler ? WINDOW_PADDING.x : 0) - (inlineSettingsButton ? BUTTON_SIZE.x : 0) - WINDOW_PADDING.x;
            uint32_t separatorCount = 0;
            bool hiddenSeparator = false;
            for (auto const& info : Build::GetFlagInfos())
            {
                if (m_config.HideFlagsMask & (uint32_t)info.Flag)
                {
                    hiddenSeparator |= info.Separator;
                    continue;
                }

                if (info.Separator || hiddenSeparator)
                    ++separatorCount;
                hiddenSeparator = false;
                separatorWidth -= BUTTON_SIZE.x;
            }
            separatorWidth /= (float)(inlineProfessionFiler + separatorCount + inlineSettingsButton);
            //float separatorWidth = (ImGui::GetContentRegionAvailWidth() - (m_config.ShowNameFilter || !inlineProfessionFiler ? WINDOW_PADDING.x : 0) - Build::GetFlagInfos().size() * BUTTON_SIZE.x - WINDOW_PADDING.x)
            //                       / (inlineProfessionFiler + std::count_if(Build::GetFlagInfos().begin(), Build::GetFlagInfos().end(), [](auto const& info) { return info.Separator; }));

            hiddenSeparator = false;
            bool first = true;
            for (auto const& info : Build::GetFlagInfos())
            {
                if (m_config.HideFlagsMask & (uint32_t)info.Flag)
                {
                    hiddenSeparator |= info.Separator;
                    continue;
                }

                bool on = storage.IsFilteringFlag(info.Flag);
                ImGui::SameLine(0, first && !inlineProfessionFiler
                                   ? WINDOW_PADDING.x
                                   : info.Separator || hiddenSeparator || inlineProfessionFiler && first
                                   ? separatorWidth
                                   : 0);
                hiddenSeparator = false;
                first = false;
                if (TextureData const& icon = GetIcon(info.Flag);
                    ImGui::CheckboxImage(icon.Texture, fmt::format("##{}", (int)info.Flag).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                    storage.ToggleFlagFilter(info.Flag, on);
                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::SetWindowFontScale(UI_SCALE);
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w /= 2;
                    ImGui::TextColored(color, "Show builds flagged as");
                    ImGui::Text("%s", info.Name.c_str());
                    ImGui::EndTooltip();
                }
            }

            if (inlineSettingsButton)
                renderSettingsButton(separatorWidth);
        }
        #pragma endregion
    }
    #pragma endregion

    #pragma region Builds
    ImGui::BeginChild("Builds", { 0, 0 }, true);
    ImGui::SetWindowFontScale(UI_SCALE);

    static Build::id_t newestAddedBuild { };
    static Build::id_t removeBuildPrompt { };
    static Build::id_t draggedBuild { };
    static GW2::Profession draggedBuildProfession { };
    static std::optional<Build::id_t> draggedOverBuild { };
    static bool draggedAfterBuild = false;
    bool firstVisibleBuild = true;

    #pragma region Build List
    auto const buildListPass = [&](auto&& builds, std::optional<GW2::Profession> const profession = { }, std::function<bool(Build const&)> const& filter = nullptr)
    {
        bool firstSortedBuild = true;
        for (Build& build : builds)
        {
            if (profession.has_value() && build.GetParsedProfession() != profession)
                continue;

            if (filter && !filter(build))
                continue;

            if (!storage.IsEditingBuild(build) && !storage.IsBuildMatchingFilter(build))
                continue;

            BeginRenderBuildList(build.GetParsedProfession(), firstVisibleBuild, firstSortedBuild, storage.IsFilteringProfession(), storage.IsEditingBuild(build) ? storage.GetEditedBuild().GetParsedProfession() : build.GetParsedProfession());

            ImVec2 const min = ImGui::GetWindowPos() + ImGui::GetCursorPos() - ImVec2 { ImGui::GetScrollX(), ImGui::GetScrollY() };
            ImVec2 const max = min +  ImVec2 { ImGui::GetContentRegionAvailWidth(), BUTTON_SIZE.y };
            bool hovered = ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseHoveringWindow();

            #pragma region Build Dragging (Before)
            if (draggedBuild && ImGui::IsMouseHoveringRect(min - ImVec2 { 0, ITEM_SPACING.y }, max) && (!m_config.SortBuildsByProfession || build.GetParsedProfession() == draggedBuildProfession))
            {
                if (firstVisibleBuild && ImGui::IsMouseHoveringRect(min - ImVec2 { 0, ITEM_SPACING.y }, { max.x, min.y }) && !m_config.SortBuildsByProfession)
                {
                    draggedOverBuild = 0;
                    draggedAfterBuild = false;
                }
                else
                {
                    draggedAfterBuild = ImGui::IsMouseHoveringRect({ min.x, min.y + (max.y - min.y) / 2 }, max);
                    if (draggedOverBuild || draggedBuild != build.GetID())
                        draggedOverBuild = build.GetID();
                }
            }

            if (draggedBuild && !draggedAfterBuild && (!m_config.SortBuildsByProfession || build.GetParsedProfession() == draggedBuildProfession))
            {
                if (draggedOverBuild && draggedOverBuild == 0 && firstVisibleBuild)
                    ImGui::HorizontalLine(-ITEM_SPACING.y, -WINDOW_PADDING.x / 2);
                if (draggedOverBuild && draggedOverBuild == build.GetID())
                    ImGui::HorizontalLine(-ITEM_SPACING.y / 2, -WINDOW_PADDING.x / 4);
            }
            #pragma endregion

            #pragma region Build Editor
            if (storage.IsEditingBuild(build))
            {
                Build& edited = storage.GetEditedBuild();

                #pragma region Build Name Input
                float x;
                {
                    TextureData const& icon = GetIcon(edited.GetParsedSpecialization());

                    auto buffer = util::to_buffer(edited.GetName());
                    if (ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button))),
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button))),
                        ImGui::ImageButtonWithText(icon.Texture, "", BUTTON_SIZE, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 1 | 8),
                        hovered = ImGui::IsItemHovered(),
                        ImGui::PopStyleColor(2),
                        ImGui::SameLine(0, 2px),
                        x = ImGui::GetCursorPosX(),
                        ImGui::PushItemWidth(-1 - BUTTON_SIZE.x - 2px - BUTTON_SIZE.x - 2px),
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { FRAME_PADDING.x, (BUTTON_SIZE.y - LINE_SIZE.y) / 2 }),
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0),
                        [&] { if (m_focusNewBuild == edited.GetID()) { ImGui::SetKeyboardFocusHere(); m_focusNewBuild = { }; } }();
                        ImGui::InputText("##EditName", buffer.data(), buffer.size()))
                        edited.SetName(buffer.data());
                    ImGui::PopStyleVar(2);
                    ImGui::PopItemWidth();
                }
                #pragma endregion
                #pragma region Build Edit Accept Button
                if (ImGui::SameLine(0, 2px);
                    ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), fmt::format("##AcceptBuildEdit:{0}", build.GetID()).c_str(), BUTTON_SIZE, 0))
                {
                    newestAddedBuild = { };
                    postAction = [&storage] { storage.AcceptBuildEdit(); };
                }
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip("Accept Changes");
                #pragma endregion
                #pragma region Build Edit Cancel Button
                if (ImGui::SameLine(0, 2px);
                    ImGui::ImageButtonWithText(GetIcon(Icons::CancelBuildEdit), fmt::format("##CancelBuildEdit:{0}", build.GetID()).c_str(), BUTTON_SIZE, 2 | 4))
                {
                    if (newestAddedBuild == edited.GetID())
                    {
                        newestAddedBuild = { };
                        postAction = [&storage, removeBuild = edited.GetID()]
                        {
                            if (Build* build = storage.FindBuild(removeBuild))
                                storage.RemoveBuild(*build);
                        };
                        storage.CancelBuildEdit();
                    }
                    else
                        postAction = [&storage] { storage.CancelBuildEdit(); };
                }
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip("Cancel Changes");
                #pragma endregion
                #pragma region Build Name Input Placeholder
                if (edited.GetName().empty())
                {
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w /= 4;
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::TextColored(color, "Build Name");
                }
                #pragma endregion

                #pragma region Build Link Label
                {
                    char const* icon = edited.GetParsedInfo().NeedsSecondaryLink ? "[*]" : "[&]";
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (ImGui::CalcTextSize(icon).x - BUTTON_SIZE.x));
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + FRAME_PADDING.y);
                    ImGui::Text("%s", icon);
                }
                #pragma endregion

                #pragma region Settings up fade alpha
                std::optional<float> alpha;
                if (m_linkPastedFadingProgress.count() && m_linkPastedFadingBuild == build.GetID())
                {
                    if (m_linkPastedFadingProgress <= LINK_PASTED_FADE_DURATION_FADE)
                        alpha = 1 - (float)m_linkPastedFadingProgress.count() / LINK_PASTED_FADE_DURATION_FADE.count();
                    else
                        alpha = 0.0f;

                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w *= *alpha;
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                }
                #pragma endregion

                #pragma region Build Link Input
                {
                    auto buffer = util::to_buffer(edited.GetLink());
                    if (ImGui::SameLine(x, 0),
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - FRAME_PADDING.y),
                        ImGui::PushItemWidth(-1);
                        ImGui::InputText("##EditLink", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
                        edited.SetLink(buffer.data());
                    ImGui::PopItemWidth();
                }
                #pragma endregion
                #pragma region Build Link Input Placeholder
                if (edited.GetLink().empty())
                {
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w /= 4;
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::TextColored(color, "Chat Link");
                }
                #pragma endregion

                #pragma region Build Link Fade Text
                if (alpha)
                {
                    ImGui::PopStyleColor();
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w *= 1 - *alpha;
                    ImVec2 const cursor = ImGui::GetCursorPos();
                    ImVec2 const size = ImGui::CalcTextSize("Pasted from clipboard");
                    float const space = ImGui::GetContentRegionAvailWidth();
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::SetCursorPos({ cursor.x + BUTTON_SIZE.x + (space - size.x - BUTTON_SIZE.x) / 2, cursor.y - ITEM_SPACING.y - BUTTON_SIZE.y + (BUTTON_SIZE.y - size.y) / 2 });
                    ImGui::PushClipRect(ImGui::GetCurrentWindow()->Pos - ImGui::GetCurrentWindow()->Scroll + ImVec2 { cursor.x + BUTTON_SIZE.x, ImGui::GetCursorPosY() }, ImGui::GetCurrentWindow()->Pos - ImGui::GetCurrentWindow()->Scroll + ImVec2 { cursor.x, ImGui::GetCursorPosY() } + ImVec2 { space, BUTTON_SIZE.y }, true);
                    ImGui::Text("Pasted from clipboard");
                    ImGui::PopClipRect();
                    ImGui::SetCursorPos(cursor);
                    ImGui::PopStyleColor();
                }
                #pragma endregion

                auto const& parsed = edited.GetParsedInfo();
                if (parsed.NeedsSecondaryLink)
                {
                    #pragma region Build Secondary Link Input
                    {
                        auto buffer = util::to_buffer(edited.GetSecondaryLink());
                        if (ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (ImGui::CalcTextSize("[*]").x - BUTTON_SIZE.x)),
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + FRAME_PADDING.y),
                            ImGui::Text("[*]"),
                            ImGui::SameLine(x, 0),
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - FRAME_PADDING.y),
                            ImGui::PushItemWidth(-1);
                            ImGui::InputText("##EditSecondaryLink", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
                            edited.SetSecondaryLink(buffer.data());
                        ImGui::PopItemWidth();
                    }
                    #pragma endregion
                    #pragma region Build Secondary Link Input Placeholder
                    if (edited.GetSecondaryLink().empty())
                    {
                        ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                        color.w /= 4;
                        ImGui::SameLine(x, FRAME_PADDING.x);
                        ImGui::TextColored(color, parsed.SecondaryLinkIsTraits ? "ArcDPS Traits Link" : "ArcDPS Skills Link");
                    }
                    #pragma endregion
                }

                if (!parsed.Error.empty())
                {
                    ImGui::NewLine();
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
                    ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "%s", parsed.Error.c_str());
                }

                if (!parsed.Info.empty())
                {
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w /= 2;
                    ImGui::NewLine();
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
                    ImGui::TextColored(color, "%s", parsed.Info.c_str());
                }

                if (ImGui::NewLine(),
                    ImGui::SameLine(x, 0),
                    ImGui::PushStyleColor(ImGuiCol_Button, BUTTON_COLOR),
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_ACTIVE),
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_HOVERED);
                    ImGui::Button(fmt::format("{}##KeyBindBuild{}", edited.GetKeyBind().ToString().value_or("No Copy Keybind Set"), build.GetID()).c_str(), { -1, 0 }))
                    EditKeyBind(edited.GetKeyBind(), [](KeyBind const& keyBind) { BuildStorage::Instance().GetEditedBuild().SetKeyBind(keyBind.ToString().value_or("")); });
                if (ImGui::IsItemHovered())
                    ImGui::TooltipWithHeader("Pressing the keybind will copy the build to clipboard", "Only when playing the build's profession");
                ImGui::PopStyleColor(3);

                #pragma region Build Flags Toggle
                float separatorWidth = ImGui::GetContentRegionAvailWidth();
                uint32_t separatorCount = 0;
                bool hiddenSeparator = false;
                for (auto const& info : Build::GetFlagInfos())
                {
                    if (m_config.HideFlagsMask & (uint32_t)info.Flag)
                    {
                        hiddenSeparator |= info.Separator;
                        continue;
                    }

                    if (info.Separator || hiddenSeparator)
                        ++separatorCount;
                    hiddenSeparator = false;
                    separatorWidth -= BUTTON_SIZE.x;
                }
                separatorWidth /= separatorCount;
                //float separatorWidth = (ImGui::GetContentRegionAvailWidth() - Build::GetFlagInfos().size() * BUTTON_SIZE.x)
                //                       / std::count_if(Build::GetFlagInfos().begin(), Build::GetFlagInfos().end(), [](auto const& info) { return info.Separator; });

                hiddenSeparator = false;
                bool first = true;
                for (auto const& info : Build::GetFlagInfos())
                {
                    if (m_config.HideFlagsMask & (uint32_t)info.Flag)
                    {
                        hiddenSeparator |= info.Separator;
                        continue;
                    }

                    bool on = edited.HasFlag(info.Flag);
                    if (first)
                        ImGui::NewLine();
                    ImGui::SameLine(0, first ? 0 : info.Separator || hiddenSeparator ? separatorWidth : 0);
                    hiddenSeparator = false;
                    first = false;
                    if (TextureData const& icon = GetIcon(info.Flag);
                        ImGui::CheckboxImage(icon.Texture, fmt::format("##{}", (int)info.Flag).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                        edited.ToggleFlag(info.Flag, on);
                    if (ImGui::IsItemHovered())
                        ImGui::Tooltip(info.Name.c_str());
                }
                #pragma endregion
            }
            #pragma endregion
            #pragma region Build Presenter
            else if (storage.IsBuildMatchingFilter(build))
            {
                #pragma region Settings up fade alpha
                std::optional<float> alpha;
                if (m_fadingBuildProgress.count() && m_fadingBuild == build.GetID())
                {
                    if (m_fadingBuildProgress >= BUILD_FADE_DURATION_FULL - BUILD_FADE_DURATION_FADE)
                        alpha = (float)(m_fadingBuildProgress - (BUILD_FADE_DURATION_FULL - BUILD_FADE_DURATION_FADE)).count() / BUILD_FADE_DURATION_FADE.count();
                    else if (m_fadingBuildProgress <= BUILD_FADE_DURATION_FADE)
                        alpha = 1 - (float)m_fadingBuildProgress.count() / BUILD_FADE_DURATION_FADE.count();
                    else
                        alpha = 0.0f;

                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w *= *alpha;
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                }
                #pragma endregion

                #pragma region Build Button
                ImGui::BeginGroup();
                ImGui::PushItemWidth(hovered ? ImGui::GetContentRegionAvailWidth() - BUTTON_SIZE.x : -1);
                std::string keyBindText;
                if (KeyBind const& keyBind = build.GetKeyBind(); keyBind && storage.GetCurrentProfession() != GW2::Profession::None && build.GetParsedProfession() == storage.GetCurrentProfession())
                {
                    keyBindText = keyBind.ToString().value_or("");
                    if (util::ends_with(keyBindText, " >GW2"))
                        keyBindText = keyBindText.substr(0, keyBindText.length() - " >GW2"sv.length());
                }
                if (TextureData const& icon = build.GetParsedSpecialization() != GW2::Specialization::None ? GetIcon(build.GetParsedSpecialization()) : GetIcon(build.GetParsedProfession());
                    ImGui::ImageButtonWithText(icon.Texture, fmt::format(build.IsSaveNeeded() ? "{}*##{}" : "{}##{}", build.GetName(), build.GetID()).c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, hovered ? 1 | 8 : 1 | 2 | 4 | 8, 0, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, false, keyBindText.c_str())
                    && !draggedOverBuild)
                {
                    ImGui::SetClipboardText(fmt::format("{}", build.GetLink()).c_str());
                    m_fadingBuild = build.GetID();
                    m_fadingBuildProgress = BUILD_FADE_DURATION_FULL;
                }
                ImGui::PopItemWidth();
                #pragma endregion

                #pragma region Build Dragging
                if (m_config.AllowBuildReordering)
                {
                    if (ImVec2 const drag = ImGui::GetMouseDragDelta(); ImGui::IsItemActive() && (drag.x || drag.y))
                    {
                        // Drag
                        draggedBuild = build.GetID();
                        draggedBuildProfession = build.GetParsedProfession();
                    }
                    else if (draggedBuild == build.GetID())
                    {
                        // Drop
                        if (draggedOverBuild.has_value() && draggedOverBuild != draggedBuild)
                        {
                            postAction = [&storage, draggedBuild = draggedBuild, draggedOverBuild = *draggedOverBuild, draggedAfterBuild = draggedAfterBuild]
                            {
                                if (Build* build = storage.FindBuild(draggedBuild))
                                {
                                    if (Build* dest = draggedOverBuild ? storage.FindBuild(draggedOverBuild) : nullptr)
                                    {
                                        if (draggedAfterBuild)
                                            storage.MoveBuildAfter(*build, *dest);
                                        else
                                            storage.MoveBuildBefore(*build, *dest);
                                    }
                                    else
                                    {
                                        if (draggedAfterBuild)
                                            storage.MoveBuildToBottom(*build);
                                        else
                                            storage.MoveBuildToTop(*build);
                                    }
                                }
                            };
                        }
                        draggedBuild = { };
                        draggedBuildProfession = { };
                        draggedOverBuild.reset();
                    }
                }
                #pragma endregion

                #pragma region Build Context Menu
                if (ImGui::BeginPopupContextItem(fmt::format("##BuildContextMenu:{}", build.GetID()).c_str()))
                {
                    ImGui::SetWindowFontScale(UI_SCALE);
                    bool working = copyAsTextWorkingPopupBuildID == build.GetID();
                    if (working)
                    {
                        copyAsTextExistsPopupBuildID = true;
                        if (copyAsTextClosePopupBuildID == build.GetID())
                        {
                            copyAsTextWorkingPopupBuildID = { };
                            copyAsTextClosePopupBuildID = { };
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (ImGui::Selectable(working ? "Please wait..." : "Copy as Text", false, working ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_DontClosePopups))
                    {
                        auto task = [this, build, popup = ImGui::GetCurrentWindow()->GetID("##BuildPadContextMenu")]
                        {
                            if (!API::Instance().PreloadAllBuildInfos(build))
                                return RepeatCurrentMainThreadTask(), false;

                            auto const& parsed = build.GetParsedInfo();

                            std::ostringstream stream;
                            stream << fmt::format("{}\n{}", build.GetName(), build.GetLink());
                            if (parsed.TraitLines)
                            {
                                stream << "\n";
                                for (auto const& line : *parsed.TraitLines)
                                    if (line.Specialization != GW2::Specialization::None)
                                        if (auto const& info = API::Specialization::Get((uint32_t)line.Specialization); info.Loaded)
                                            stream << fmt::format("\n{}\n- {}\n- {}\n- {}",
                                                info.Name,
                                                line.Traits[0] ? API::Trait::Get(info.GetTrait(0, line.Traits[0])).Name : "",
                                                line.Traits[1] ? API::Trait::Get(info.GetTrait(1, line.Traits[1])).Name : "",
                                                line.Traits[2] ? API::Trait::Get(info.GetTrait(2, line.Traits[2])).Name : "");
                            }
                            if (parsed.SkillsLand || parsed.SkillsWater)
                            {
                                stream << "\n";
                                if (parsed.SkillsLand)
                                    stream << fmt::format("\nTerrestrial Skills:\n- {}\n- {}\n- {}\n- {}\n- {}",
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[0], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[1], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[2], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[3], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[4], parsed.RevenantLegendsLand[0])).Name);
                                if (parsed.SkillsWater)
                                    stream << fmt::format("\nAquatic Skills:\n- {}\n- {}\n- {}\n- {}\n- {}",
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[0], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[1], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[2], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[3], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[4], parsed.RevenantLegendsWater[0])).Name);
                            }
                            switch (parsed.Profession)
                            {
                                case GW2::Profession::Ranger:
                                    stream << "\n";
                                    stream << fmt::format("\nTerrestrial Pets:\n- {}\n- {}",
                                        API::Pet::Get(parsed.RangerPetsLand[0]).Name,
                                        API::Pet::Get(parsed.RangerPetsLand[1]).Name);
                                    stream << fmt::format("\nAquatic Pets:\n- {}\n- {}",
                                        API::Pet::Get(parsed.RangerPetsWater[0]).Name,
                                        API::Pet::Get(parsed.RangerPetsWater[1]).Name);
                                    break;
                                case GW2::Profession::Revenant:
                                    stream << "\n";
                                    stream << fmt::format("\nTerrestrial Legends:\n- {}\n- {}",
                                        API::Skill::Get(GW2::GetRevenantLegendInfo(parsed.RevenantLegendsLand[0]).SwapSkill).Name,
                                        API::Skill::Get(GW2::GetRevenantLegendInfo(parsed.RevenantLegendsLand[1]).SwapSkill).Name);
                                    stream << fmt::format("\nAquatic Legends:\n- {}\n- {}",
                                        API::Skill::Get(GW2::GetRevenantLegendInfo(parsed.RevenantLegendsWater[0]).SwapSkill).Name,
                                        API::Skill::Get(GW2::GetRevenantLegendInfo(parsed.RevenantLegendsWater[1]).SwapSkill).Name);
                                    break;
                                default:
                                    break;
                            }
                            ImGui::SetClipboardText(stream.str().c_str());
                            copyAsTextClosePopupBuildID = build.GetID();
                            return true;
                        };

                        if (task())
                            ImGui::CloseCurrentPopup();
                        else
                        {
                            copyAsTextWorkingPopupBuildID = build.GetID();
                            copyAsTextExistsPopupBuildID = true;
                            OnMainThread(std::move(task));
                        }
                    }
                    if (ImGui::Selectable("Copy Terrestrial Skills as Chat Links", false, !build.GetParsedInfo().SkillsLand ? ImGuiSelectableFlags_Disabled : 0))
                    {
                        auto const& parsed = build.GetParsedInfo();
                        if (parsed.SkillsLand)
                            ImGui::SetClipboardText(fmt::format("{}{}{}{}{}",
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[0], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[1], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[2], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[3], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsLand)[4], parsed.RevenantLegendsLand[0])).ToChatLink()).c_str());
                    }
                    if (ImGui::Selectable("Copy Aquatic Skills as Chat Links", false, !build.GetParsedInfo().SkillsWater ? ImGuiSelectableFlags_Disabled : 0))
                    {
                        auto const& parsed = build.GetParsedInfo();
                        if (parsed.SkillsWater)
                            ImGui::SetClipboardText(fmt::format("{}{}{}{}{}",
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[0], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[1], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[2], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[3], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillPaletteToSkill((*parsed.SkillsWater)[4], parsed.RevenantLegendsWater[0])).ToChatLink()).c_str());
                    }
                    if (ImGui::Selectable(working ? "Please wait..." : "Copy Traits as Chat Links", false, !build.GetParsedInfo().TraitLines || working ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_DontClosePopups))
                    {
                        auto task = [this, build, popup = ImGui::GetCurrentWindow()->GetID("##BuildPadContextMenu")]
                        {
                            if (!API::Instance().PreloadAllBuildInfos(build))
                                return RepeatCurrentMainThreadTask(), false;

                            auto const& parsed = build.GetParsedInfo();
                            std::ostringstream stream;
                            for (auto const& line : *parsed.TraitLines)
                                if (line.Specialization != GW2::Specialization::None)
                                    if (auto const& info = API::Specialization::Get((uint32_t)line.Specialization); info.Loaded)
                                        stream << fmt::format(" {} {}{}{}",
                                            info.Name,
                                            line.Traits[0] ? API::Trait::Get(info.GetTrait(0, line.Traits[0])).ToChatLink() : "-",
                                            line.Traits[1] ? API::Trait::Get(info.GetTrait(1, line.Traits[1])).ToChatLink() : "-",
                                            line.Traits[2] ? API::Trait::Get(info.GetTrait(2, line.Traits[2])).ToChatLink() : "-");

                            if (std::string const str = stream.str(); !str.empty())
                                ImGui::SetClipboardText(str.substr(1).c_str());

                            copyAsTextClosePopupBuildID = build.GetID();
                            return true;
                        };

                        if (task())
                            ImGui::CloseCurrentPopup();
                        else
                        {
                            copyAsTextWorkingPopupBuildID = build.GetID();
                            copyAsTextExistsPopupBuildID = true;
                            OnMainThread(std::move(task));
                        }
                    }
                    if (ImGui::Selectable("Copy Traits as Numbers", false, !build.GetParsedInfo().TraitLines ? ImGuiSelectableFlags_Disabled : 0))
                    {
                        auto const& parsed = build.GetParsedInfo();
                        std::ostringstream stream;
                        for (auto const& line : *parsed.TraitLines)
                            if (line.Specialization != GW2::Specialization::None)
                                stream << fmt::format(" {} {}{}{}",
                                    GW2::GetSpecializationInfo(line.Specialization).Name,
                                    (uint32_t)line.Traits[0],
                                    (uint32_t)line.Traits[1],
                                    (uint32_t)line.Traits[2]);

                        if (std::string const str = stream.str(); !str.empty())
                            ImGui::SetClipboardText(str.substr(1).c_str());
                    }

                    ImGui::Separator();

                    if (ImGui::Selectable("Delete"))
                        removeBuildPrompt = build.GetID();

                    ImGui::EndPopup();
                }
                #pragma endregion

                #pragma region Build Edit Button
                if (hovered)
                    if (ImGui::SameLine(0, 0);
                        ImGui::ImageButtonWithText(GetIcon(Icons::EditBuild), fmt::format("##EditBuild:{0}", build.GetID()).c_str(), BUTTON_SIZE, 2 | 4))
                        postAction = [&storage, editedBuild = build.GetID()]
                        {
                            if (Build* build = storage.FindBuild(editedBuild))
                                storage.EditBuild(*build);
                        };
                #pragma endregion
                ImGui::EndGroup();
                hovered = ImGui::IsItemHovered();

                #pragma region Build Dragging (After)
                if (draggedBuild && draggedAfterBuild && (!m_config.SortBuildsByProfession || build.GetParsedProfession() == draggedBuildProfession))
                    if (draggedOverBuild && draggedOverBuild == build.GetID())
                        ImGui::HorizontalLine(-ITEM_SPACING.y / 2, -WINDOW_PADDING.x / 4);
                #pragma endregion

                #pragma region Build Fade Text
                if (alpha)
                {
                    ImGui::PopStyleColor();
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
                    color.w *= 1 - *alpha;
                    ImVec2 const cursor = ImGui::GetCursorPos();
                    ImVec2 const size = ImGui::CalcTextSize("Copied to clipboard");
                    float const space = ImGui::GetContentRegionAvailWidth();
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::SetCursorPos({ cursor.x + (space - size.x) / 2, cursor.y - ITEM_SPACING.y - BUTTON_SIZE.y + (BUTTON_SIZE.y - size.y) / 2 });
                    ImGui::PushClipRect(ImGui::GetCurrentWindow()->Pos - ImGui::GetCurrentWindow()->Scroll + ImVec2 { cursor.x, ImGui::GetCursorPosY() }, ImGui::GetCurrentWindow()->Pos - ImGui::GetCurrentWindow()->Scroll + ImVec2 { cursor.x, ImGui::GetCursorPosY() } + ImVec2 { space, BUTTON_SIZE.y }, true);
                    ImGui::Text("Copied to clipboard");
                    ImGui::PopClipRect();
                    ImGui::SetCursorPos(cursor);
                    ImGui::PopStyleColor();
                }
                #pragma endregion
            }
            #pragma endregion

            if (m_tooltipBuild == build.GetID())
            {
                if (!hovered)
                    m_tooltipBuild = { };
                else if (m_tooltipShown)
                {
                    ImGui::BeginTooltip();
                    ImGui::SetWindowFontScale(UI_SCALE);
                    RenderBuildTooltip(storage.IsEditingBuild(build) ? storage.GetEditedBuild() : build, !storage.IsEditingBuild(build));
                    ImGui::EndTooltip();
                }
            }
            else if (hovered)
            {
                m_tooltipBuild = build.GetID();
                m_tooltipShown = false;
                m_tooltipDelay = std::chrono::milliseconds { m_config.TooltipDelay };
            }

            EndRenderBuildList(storage.IsFilteringProfession());
            firstVisibleBuild = false;
        }
    };
    auto const buildList = [&](auto&& builds)
    {
        if (m_config.SortBuildsByProfession && !storage.IsFilteringProfession())
        {
            bool const currentFirst = m_config.SortBuildsByProfessionCurrentFirst && storage.GetCurrentProfession() != GW2::Profession::None;
            if (currentFirst)
                buildListPass(builds, storage.GetCurrentProfession());

            for (auto const& info : GW2::GetProfessionInfos())
            {
                if (info.Profession == GW2::Profession::None)
                    continue;

                if (currentFirst && info.Profession == storage.GetCurrentProfession())
                    continue;

                buildListPass(builds, info.Profession);
            }
            buildListPass(builds, GW2::Profession::None);
        }
        else
            buildListPass(builds);
    };

    if (m_config.SortBuildsAlphabetically)
        buildList(storage.GetSortedBuilds());
    else
        buildList(storage.GetBuilds());
    #pragma endregion

    #pragma region Build Dragging (Bottom)
    {
        ImVec2 const min = ImGui::GetWindowPos() + ImGui::GetCursorPos() - ImVec2 { ImGui::GetScrollX(), ImGui::GetScrollY() };
        ImVec2 const max = min + ImGui::GetContentRegionAvail();
        if (draggedBuild && ImGui::IsMouseHoveringRect(min - ImVec2 { 0, ITEM_SPACING.y }, max) && !m_config.SortBuildsByProfession)
        {
            draggedOverBuild = 0;
            draggedAfterBuild = true;
            ImGui::HorizontalLine(-1, -WINDOW_PADDING.x / 2);
        }
    }
    #pragma endregion

    #pragma region Add Build Button
    if (!storage.IsEditingBuild())
    {
        bool on = true;
        static bool addBuildButtonHovered = false;
        if (!addBuildButtonHovered)
        {
            ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
            color.w *= 0.5f;
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        }
        if (TextureData const& icon = GetIcon(Icons::AddBuild);
            ImGui::CheckboxImage(icon.Texture, "Add Build", &on, icon.GetUV0(), icon.GetUV1(), 0, 0, 0, 0.5f, 1.0f, 0.75f))
        {
            Build& build = storage.AddBuild();
            newestAddedBuild = build.GetID();
            m_focusNewBuild = build.GetID();
            storage.EditBuild(build);
            if (char const* clipboard = ImGui::GetClipboardText())
            {
                if (ChatLink::Decode(clipboard))
                {
                    storage.GetEditedBuild().SetLink(clipboard);
                    m_linkPastedFadingBuild = build.GetID();
                    m_linkPastedFadingProgress = LINK_PASTED_FADE_DURATION_FULL;
                }
            }
            for (auto const& info : Build::GetFlagInfos())
                storage.GetEditedBuild().ToggleFlag(info.Flag, storage.IsFilteringFlag(info.Flag));
        }
        if (!addBuildButtonHovered)
            ImGui::PopStyleColor();
        addBuildButtonHovered = ImGui::IsItemHovered();
    }
    #pragma endregion

    if (m_config.AutoWindowHeight)
        m_config.WindowSizeH -= (int32_t)(ImGui::GetContentRegionAvail().y + (firstVisibleBuild || storage.IsEditingBuild() ? ITEM_SPACING.y : WINDOW_PADDING.y - FRAME_PADDING.y));

    if (!copyAsTextExistsPopupBuildID)
    {
        copyAsTextClosePopupBuildID = { };
        copyAsTextWorkingPopupBuildID = { };
    }

    ImGui::EndChild();
    #pragma endregion

    #pragma region Confirm Build Deletion Popup
    if (removeBuildPrompt)
    {
        ImGui::OpenPopup("BuildPad##ConfirmBuildDeletion");

        if (Build* build = storage.FindBuild(removeBuildPrompt))
        {
            if (ImGui::BeginPopupModal("BuildPad##ConfirmBuildDeletion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetWindowFontScale(UI_SCALE);
                ImGui::Text("Are you sure you want to delete this build?");
                bool on = true;
                ImGui::CheckboxImage(GetIcon(build->GetParsedSpecialization()).Texture, fmt::format("{}", build->GetName()).c_str(), &on, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                ImGui::NewLine();
                if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 50px - ITEM_SPACING.x - 50px); ImGui::Button("Yes", { 50px, 0 }))
                {
                    postAction = [&storage, removeBuild = removeBuildPrompt]
                    {
                        if (Build* build = storage.FindBuild(removeBuild))
                            storage.RemoveBuild(*build);
                    };
                    removeBuildPrompt = { };
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::SameLine(); ImGui::Button("No", { 50px, 0 }))
                {
                    removeBuildPrompt = { };
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }
    #pragma endregion

    if (postAction)
        postAction();

    RenderKeyBindEditor();

    ImGui::End();
}

void Handler::RenderSettings(bool menu)
{
    if (menu)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        if (ImGui::Button("Detach Menu", { -1, 0 }))
        {
            ImGui::PopStyleVar();
            m_detachSettings = true;
            return;
        }
        ImGui::PopStyleVar();
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Opens these settings in a separate window");
        ImGui::Separator();
    }

    // Save Config Button
    {
        bool const disabled = !IsSaveNeeded() && !BuildStorage::Instance().IsSaveNeeded();
        if (disabled)
        {
            ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button));
            color.x = color.y = color.z = (color.x + color.y + color.z) / 3;
            ImGui::PushStyleColor(ImGuiCol_Button, color);
        }
        if (ImGui::ButtonEx("Save Builds & Settings", { ImGui::GetContentRegionAvailWidth() - ITEM_SPACING.x - (FRAME_PADDING.x + ImGui::CalcTextSize("Load").x + FRAME_PADDING.x), 0 }, disabled ? ImGuiButtonFlags_Disabled : 0))
            SaveConfig();
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Everything is saved automatically when you close Guild Wars 2");
        if (disabled)
            ImGui::PopStyleColor();
    }
    // Load Config Button
    if (ImGui::SameLine(); ImGui::Button("Load"))
        LoadConfig();

    if (m_arcdpsMigrationAvailable || m_arcdpsGearAvailable)
    {
        ImGui::Separator();
        ImGui::Text("ArcDPS Templates:");
        if (m_arcdpsMigrationAvailable && ImGui::Button("Migrate Traits/Skills Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("Migrate Traits/Skills Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
            m_arcdpsMigrationShown = true;
        if (m_arcdpsGearAvailable && ImGui::Button("View Equipment/Legendary Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("View Equipment/Legendary Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
            m_arcdpsGearShown = true;
    }

    ImGui::Separator();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1, 1 });

    if (ImGui::Text("Keybind"), ImGui::SameLine(); ImGui::Button(fmt::format("{}##KeyBindToggleBuilds", m_config.KeyBindToggleBuilds).c_str(), { -1, 0 }))
        EditKeyBind(m_keyBindToggleBuilds, [this](KeyBind const& keyBind) { m_config.KeyBindToggleBuilds = (m_keyBindToggleBuilds = keyBind).ToString().value_or(""); });

    ImGui::PushItemWidth(LINE_SIZE.y * 7);
    if (int lang = (int)std::distance(API::GetLanguageInfos().begin(), std::find_if(API::GetLanguageInfos().begin(), API::GetLanguageInfos().end(), [tag = API::Instance().GetLanguage()](API::LanguageInfo const& l) { return l.Tag == tag; }));
        ImGui::Combo("API Language", &lang, [](void* data, int const index, char const** text) { return *text = API::GetLanguageInfos()[index].Name.c_str(), true; }, nullptr, (int)API::GetLanguageInfos().size()))
    {
        auto itr = API::GetLanguageInfos().begin();
        std::advance(itr, lang);
        API::Instance().SetLanguage(m_config.APILocale = itr->Tag);
    }
    ImGui::PopItemWidth();
    if (ImGui::IsItemHovered())
        ImGui::Tooltip("Some characters might require a custom font to display correctly");
    ImGui::Checkbox("Show Filter Panel", &m_config.ShowFilterPanel);
    if (m_config.ShowFilterPanel)
    {
        if (!m_config.ShowProfessionFilter && !m_config.ShowNameFilter && !m_config.ShowFlagsFilter)
            m_config.ShowProfessionFilter = m_config.ShowNameFilter = m_config.ShowFlagsFilter = true;

        ImGui::Indent(INDENT_SPACING);
        ImGui::Checkbox("Show Profession Filter", &m_config.ShowProfessionFilter);
        ImGui::Checkbox("Show Search Box", &m_config.ShowNameFilter);
        ImGui::Checkbox("Show Flags", &m_config.ShowFlagsFilter);
        if (ImGui::Checkbox("Show Settings Button", &m_config.ShowSettingsButton) && !m_config.ShowSettingsButton && m_config.HideWindowHeader)
            m_config.HideWindowHeader = false;
        ImGui::Checkbox("Clear Filters When Hidden", &m_config.ClearFiltersOnWindowClose);
        if (ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_WindowBg)).w < 0.75f - 0.01f) // 0.75f in ImGuiExtensions.h, but there is some precision loss in U32<->float conversion
            ImGui::Checkbox("Less Transparent Buttons", &m_config.LessTransparentButtons);
        if (m_config.HiddenFiltersHintHidden)
            ImGui::Checkbox("Hide warning about active hidden filters", &m_config.HiddenFiltersHintHidden);
        ImGui::Unindent(INDENT_SPACING);

        if (!m_config.ShowProfessionFilter && !m_config.ShowNameFilter && !m_config.ShowFlagsFilter)
        {
            m_config.ShowFilterPanel = false;
            m_config.ClearFiltersOnWindowClose = false;
            m_config.LessTransparentButtons = false;
        }
    }
    if (!m_config.ShowFilterPanel && m_config.HideWindowHeader)
        m_config.HideWindowHeader = false;
    ImGui::Checkbox("Use Profession Colors", &m_config.UseProfessionColors);
    if (m_config.UseProfessionColors)
    {
        ImGui::Indent(INDENT_SPACING);
        ImGui::Checkbox("Only When Not My Profession", &m_config.ProfessionColorsUnfilteredOnly);
        if (ImGui::FixedCollapsingHeader("Customize Colors"))
        {
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - BUTTON_SIZE.x - ITEM_SPACING.x - BUTTON_SIZE.x);
            for (auto const& info : GW2::GetProfessionInfos())
            {
                uint32_t& packed = m_config.ProfessionColors[(size_t)info.Profession];
                ImVec4 const color = ImGui::ColorConvertU32ToFloat4(packed);
                float values[] { color.x, color.y, color.z, color.w };

                {
                    TextureData const& icon = GetIcon(info.Profession);
                    bool on = true;
                    ImGui::CheckboxImage(icon.Texture, "", &on, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                }

                if (ImGui::SameLine();
                    ImGui::ColorEdit4(fmt::format("##ProfessionColor:{}", (int)info.Profession).c_str(), values))
                    packed = ImGui::ColorConvertFloat4ToU32({ values[0], values[1], values[2], values[3] });

                if (uint32_t const def = ImGui::ColorConvertFloat4ToU32(info.GetDefaultButtonColor()); packed != def)
                {
                    ImGui::SameLine(0, 0);
                    bool on = true;
                    if (TextureData const& icon = GetIcon(Icons::ClearSearch);
                        ImGui::CheckboxImage(icon.Texture, fmt::format("##ResetProfessionColor:{}", (int)info.Profession).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                        packed = def;
                }
            }
            ImGui::PopItemWidth();
        }
        ImGui::Unindent(INDENT_SPACING);
    }
    ImGui::Checkbox("Sort Builds by Profession", &m_config.SortBuildsByProfession);
    if (m_config.SortBuildsByProfession)
    {
        ImGui::Indent(INDENT_SPACING);
        ImGui::Checkbox("Current Profession First", &m_config.SortBuildsByProfessionCurrentFirst);
        ImGui::Checkbox("Show Profession Icons", &m_config.SortBuildsByProfessionShowIcons);
        ImGui::Checkbox("Show Profession Names", &m_config.SortBuildsByProfessionShowNames);
        ImGui::PushItemWidth(LINE_SIZE.y * 5);
        ImGui::DragInt("Profession Spacing", (int*)&m_config.SortBuildsByProfessionSpacing, 0.25f, 0, 50, "%.0f px");
        ImGui::PopItemWidth();
        ImGui::Unindent(INDENT_SPACING);
    }
    if (ImGui::Checkbox("Sort Builds Alphabetically", &m_config.SortBuildsAlphabetically) && m_config.SortBuildsAlphabetically)
        m_config.AllowBuildReordering = false;
    if (ImGui::Checkbox("Allow Build Dragging", &m_config.AllowBuildReordering) && m_config.AllowBuildReordering)
        m_config.SortBuildsAlphabetically = false;
    ImGui::PushItemWidth(LINE_SIZE.y * 5);
    ImGui::DragInt("Tooltip Delay", (int*)&m_config.TooltipDelay, 10, 0, 5000, "%.0f ms");
    ImGui::PopItemWidth();

    ImGui::Separator();

    if (ImGui::FixedCollapsingHeader("Toggle Flags"))
    {
        std::vector<float> columns;
        for (auto const& info : Build::GetFlagInfos())
        {
            if (info.Separator)
                columns.emplace_back();
            if (!columns.empty())
                columns.back() = std::max<float>(columns.back(), ITEM_SPACING.x + BUTTON_SIZE.x + ITEM_INNER_SPACING.x + ImGui::CalcTextSize(info.Name.c_str()).x + ITEM_SPACING.x);
        }

        bool columnsStarted = false;
        for (Build::FlagInfo const& info : Build::GetFlagInfos())
        {
            bool on = !(m_config.HideFlagsMask & (uint32_t)info.Flag);

            if (info.Separator)
            {
                if (!columnsStarted)
                {
                    columnsStarted = true;
                    ImGui::Separator();
                    ImGui::Columns((int)columns.size());
                    for (size_t i = 0; i < columns.size(); ++i)
                        ImGui::SetColumnOffset((int)i + 1, ImGui::GetColumnOffset((int)i) + columns[i]);
                }
                else
                    ImGui::NextColumn();
            }
            if (TextureData const& icon = GetIcon(info.Flag);
                ImGui::CheckboxImage(icon.Texture, fmt::format("{}##{}", info.Name, (int)info.Flag).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
            {
                if (on)
                    m_config.HideFlagsMask &= ~(uint32_t)info.Flag;
                else
                    m_config.HideFlagsMask |= (uint32_t)info.Flag;
            }
        }
        ImGui::Columns(1);
    }

    ImGui::Separator();

    ImGui::PushItemWidth(LINE_SIZE.y * 5);
    if (ImGui::DragInt("UI Scale", (int*)&m_config.UIScale, 1, 20, 500, "%.0f%%"))
        m_config.UIScale = (m_config.UIScale + 5) / 10 * 10; // Some manual rounding to the nearest 10% step
    if (ImGui::IsItemHovered())
        ImGui::Tooltip("Try changing ArcDPS font size first before touching this");
    ImGui::PopItemWidth();
    ImGui::Checkbox("Lock Window Position", &m_config.LockWindowPosition);
    if (m_config.LockWindowPosition)
    {
        ImGui::Indent(INDENT_SPACING);
        int pos[] { m_config.WindowPositionX, m_config.WindowPositionY };
        ImGui::PushItemWidth(LINE_SIZE.y * 10);
        if (ImGui::DragInt2("Position", pos, 0.5f, -std::max<int>(m_config.WindowPositionX, m_config.WindowPositionY), (int)std::max<float>(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y)))
        {
            m_config.WindowPositionX = (int32_t)pos[0];
            m_config.WindowPositionY = (int32_t)pos[1];
        }
        ImGui::PopItemWidth();
        ImGui::Unindent(INDENT_SPACING);
    }
    ImGui::Checkbox("Lock Window Size", &m_config.LockWindowSize);
    if (m_config.LockWindowSize)
    {
        ImGui::Indent(INDENT_SPACING);
        int size[] { (int)m_config.WindowSizeW, (int)m_config.WindowSizeH };
        ImGui::PushItemWidth(LINE_SIZE.y * 10);
        if (ImGui::DragInt2("Size", size, 0.5f, 0, (int)std::max<float>(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y)))
        {
            m_config.WindowSizeW = (uint32_t)size[0];
            if (!m_config.AutoWindowHeight)
                m_config.WindowSizeH = (uint32_t)size[1];
        }
        ImGui::PopItemWidth();
        ImGui::Checkbox("Auto Window Height", &m_config.AutoWindowHeight);
        ImGui::Unindent(INDENT_SPACING);
    }
    else
        m_config.AutoWindowHeight = false;
    if (ImGui::Checkbox("Hide Window Header", &m_config.HideWindowHeader) && m_config.HideWindowHeader && (!m_config.ShowFilterPanel || !m_config.ShowSettingsButton))
        m_config.ShowFilterPanel = m_config.ShowSettingsButton = true;
    ImGui::Checkbox("Keep Window Within Boundaries", &m_config.KeepWindowInBounds);
    if (ImGui::IsItemHovered())
        ImGui::TooltipWithHeader("Prevent window from leaving the screen boundaries", "Enabling this might move the window undesirably if you change resolution!");

    ImGui::Separator();

    if (m_versionUpdateState == VersionUpdateState::Done)
        ImGui::ButtonEx("Restart to Apply Update", { -1, 0 }, ImGuiButtonFlags_Disabled);
    else if (m_needsUpdate)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(0xFF44BBEE));
        if (ImGui::Button(fmt::format("Update to {}", m_versionLatest.Version).c_str(), { -1, 0 }))
            VersionUpdate();
        ImGui::PopStyleColor();
    }
    else if (m_versionCheckResult)
    {
        if (ImGui::Button(*m_versionCheckResult ? "Check for Updates" : "Update Check Failed", { -1, 0 }))
            VersionCheck();
    }
    else
        ImGui::ButtonEx("Checking for Updates...", { -1, 0 }, ImGuiButtonFlags_Disabled);

    if (ImGui::Button(fmt::format("Version {}", m_versionCurrent).c_str(), { -1, 0 }))
        m_aboutShown ^= true;

    RenderKeyBindEditor();

    ImGui::PopStyleVar();
}

void Handler::EditKeyBind(KeyBind const& keyBind, KeyBindCallback&& callback)
{
    m_keyBindEditing = true;
    m_keyBindEdited = keyBind;
    m_keyBindCallback = std::move(callback);
    auto const window = ImGui::GetCurrentWindow();
    if (window)
        ImGui::GetCurrentContext()->CurrentWindow = window->RootWindow;
    ImGui::OpenPopup("BuildPad##KeyBindEditor");
    if (window)
        ImGui::GetCurrentContext()->CurrentWindow = window;
}

void Handler::RenderKeyBindEditor()
{
    if (!m_keyBindEditing)
        return;

    ImGui::SetNextWindowSizeConstraints({ 60px + ITEM_SPACING.x + 60px + ITEM_SPACING.x + 60px, 0 }, { 10000px, 10000px });
    if (ImGui::BeginPopupModal("BuildPad##KeyBindEditor", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetWindowFontScale(UI_SCALE);
        ImGui::CaptureKeyboardFromApp();

        KeyBind::key_t foundKey = 0;

        for (KeyBind::key_t key = '0'; !foundKey && key <= '9'; ++key)
            if (ImGui::IsKeyPressed(key, false))
                foundKey = key;

        for (KeyBind::key_t key = 'A'; !foundKey && key <= 'Z'; ++key)
            if (ImGui::IsKeyPressed(key, false))
                foundKey = key;

        for (auto&& [key, _] : KeyBind::GetKeyMap())
            if (ImGui::IsKeyPressed(key, false))
                foundKey = key;

        if (foundKey && KeyBind::KeyToString(foundKey))
            m_keyBindEdited.Key = foundKey;

        ImGui::Text("Press the key you want to bind...");

        ImVec4 active = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
        ImVec4 inactive = active;
        inactive.w /= 4;

        ImGui::PushStyleColor(ImGuiCol_Text, m_keyBindEdited.Control ? active : inactive);
        if (ImGui::Button("Ctrl"))
            m_keyBindEdited.Control ^= true;
        ImGui::PopStyleColor();
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        ImGui::Text("+");
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        ImGui::PushStyleColor(ImGuiCol_Text, m_keyBindEdited.Alt ? active : inactive);
        if (ImGui::Button("Alt"))
            m_keyBindEdited.Alt ^= true;
        ImGui::PopStyleColor();
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        ImGui::Text("+");
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        ImGui::PushStyleColor(ImGuiCol_Text, m_keyBindEdited.Shift ? active : inactive);
        if (ImGui::Button("Shift"))
            m_keyBindEdited.Shift ^= true;
        ImGui::PopStyleColor();
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        ImGui::Text("+");
        ImGui::SameLine(0, ITEM_SPACING.x / 2);
        {
            auto buffer = util::to_buffer<20>(KeyBind::KeyToString(m_keyBindEdited.Key).value_or(""));
            ImGui::PushItemWidth(-1);
            ImGui::InputTextEx("", buffer.data(), (int)buffer.size(), { -1, 0 }, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopItemWidth();
        }

        ImGui::Checkbox("Pass to Guild Wars 2", &m_keyBindEdited.PassThrough);
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Enable to allow Guild Wars 2 to react to pressed keys");

        if (ImGui::Button("Clear", { 60px, 0 }))
            m_keyBindEdited = { };
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 60px - ITEM_SPACING.x - 60px); ImGui::Button("OK", { 60px, 0 }))
        {
            if (m_keyBindCallback)
                m_keyBindCallback(m_keyBindEdited);
            m_keyBindEditing = false;
            m_keyBindCallback = { };
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::SameLine(); ImGui::Button("Cancel", { 60px, 0 }))
        {
            m_keyBindEditing = false;
            m_keyBindCallback = { };
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Handler::BeginRenderBuildList(GW2::Profession profession, bool& firstVisible, bool& firstSorted, bool singleProfession, GW2::Profession professionColor, float colorMultiplier) const
{
    if (firstSorted && m_config.SortBuildsByProfession && !singleProfession)
    {
        if (!firstVisible && m_config.SortBuildsByProfessionSpacing)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ITEM_SPACING.x, (float)m_config.SortBuildsByProfessionSpacing });
            ImGui::Spacing();
            ImGui::PopStyleVar();
        }

        if ((m_config.SortBuildsByProfessionShowIcons || m_config.SortBuildsByProfessionShowNames) && profession != GW2::Profession::None)
        {
            std::string const name { m_config.SortBuildsByProfessionShowNames ? GW2::GetProfessionInfo(profession).Name : ""sv };
            if (m_config.SortBuildsByProfessionShowIcons)
            {
                TextureData const& icon = GetIcon(profession);
                bool on = true;
                if (name.empty())
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + FRAME_PADDING.y);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
                ImGui::CheckboxImage(icon.Texture, name.c_str(), &on, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
                ImGui::PopStyleVar();
                if (name.empty())
                {
                    ImGui::SameLine(0, 2px);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - FRAME_PADDING.y);
                }
            }
            else
                ImGui::Text("%s", name.c_str());
        }
    }
    firstSorted = false;

    if (m_config.SortBuildsByProfession && !singleProfession && m_config.SortBuildsByProfessionShowIcons && !m_config.SortBuildsByProfessionShowNames)
        ImGui::Indent(LINE_SIZE.y + 2px);

    if (m_config.UseProfessionColors && (!m_config.ProfessionColorsUnfilteredOnly || !singleProfession))
    {
        ImVec4 color = ImGui::ColorConvertU32ToFloat4(m_config.ProfessionColors[(size_t)professionColor]);
        color.x *= colorMultiplier;
        color.y *= colorMultiplier;
        color.z *= colorMultiplier;
        ImGui::PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, color.w * 0.6f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { color.x, color.y, color.z, color.w * 0.8f }); //{ std::min<float>(1.0f, color.x * 1.25f), std::min<float>(1.0f, color.y * 1.25f), std::min<float>(1.0f, color.z * 1.25f), color.w });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    }
}

void Handler::EndRenderBuildList(bool singleProfession) const
{
    if (m_config.UseProfessionColors && (!m_config.ProfessionColorsUnfilteredOnly || !singleProfession))
        ImGui::PopStyleColor(3);

    if (m_config.SortBuildsByProfession && !singleProfession && m_config.SortBuildsByProfessionShowIcons && !m_config.SortBuildsByProfessionShowNames)
        ImGui::Unindent(LINE_SIZE.y + 2px);
}

void Handler::RenderBuildTooltip(Build const& build, bool footer, bool errorMissing, Build* editTarget) const
{
    API::Instance().PreloadAllBuildInfos(build);

    auto const& parsed = build.GetParsedInfo();

    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "%s", fmt::format("{}", build.GetName()).c_str());
    if (parsed.SkillsLand || parsed.SkillsWater || parsed.TraitLines)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        ImGui::Image(GetIcon(build.GetParsedSpecialization()).Texture, { 32px, 32px });
        ImGui::SameLine(0, 3px);

        auto const& info = GW2::GetSpecializationInfo(build.GetParsedSpecialization());
        std::string const text = fmt::format((parsed.SkillsLand || parsed.SkillsWater) && parsed.TraitLines ? "{} Build" : parsed.TraitLines ? "{} Traits" : "{} Skills", info.Elite ? info.Name : GW2::GetProfessionInfo(build.GetParsedProfession()).Name);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + std::ceil((32px - ImGui::CalcTextSize(text.c_str()).y) / 2));
        ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "%s", text.c_str());
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - std::ceil((32px - ImGui::CalcTextSize(text.c_str()).y) / 2));
        ImGui::PopStyleVar();
    }

    if (parsed.Profession == GW2::Profession::Revenant && (parsed.SkillsLand || parsed.SkillsWater) && !parsed.TraitLines)
    {
        if (!parsed.TraitLines)
            ImGui::Text("Combine traits and skills and\nselect legends to see skills");
    }
    else if (parsed.SkillsLand || parsed.SkillsWater)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        ImGui::NewLine();

        float x = ImGui::GetCursorPosX();
        ImGui::SameLine(0, std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - (24px + 40px + 40px + 40px + 40px + 40px)) / 2)));
        if (parsed.SkillsLand)
        {
            x = ImGui::GetCursorPosX();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (40px - 24px) / 2);
            ImGui::Image(GetIcon(Icons::LandSkills).Texture, { 24px, 24px });
            ImGui::SameLine(0, 0);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (40px - 24px) / 2);
            for (uint32_t skill : *parsed.SkillsLand)
            {
                auto const& info = API::Skill::Get(SkillPaletteToSkill(skill, parsed.RevenantLegendsLand[0]));
                auto const& icon = skill && info.ID ? info.Icon : GetIcon(Icons::MissingSkill);
                ImGui::Image(icon.Texture, { 40px, 40px }, icon.GetUV0(), icon.GetUV1());
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip(info.Name.c_str());
                ImGui::SameLine(0, 0);
            }
            ImGui::NewLine();
        }

        if (x != 0)
            ImGui::SetCursorPosX(x);
        if (parsed.SkillsWater)
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (40px - 24px) / 2);
            ImGui::Image(GetIcon(Icons::WaterSkills).Texture, { 24px, 24px });
            ImGui::SameLine(0, 0);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (40px - 24px) / 2);
            for (uint32_t skill : *parsed.SkillsWater)
            {
                auto const& info = API::Skill::Get(SkillPaletteToSkill(skill, parsed.RevenantLegendsWater[0]));
                auto const& icon = skill && info.ID ? info.Icon : GetIcon(Icons::MissingSkill);
                ImGui::Image(icon.Texture, { 40px, 40px }, icon.GetUV0(), icon.GetUV1());
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip(info.Name.c_str());
                ImGui::SameLine(0, 0);
            }
            ImGui::NewLine();
        }

        ImGui::PopStyleVar();
        ImGui::Spacing();
    }
    else if (errorMissing)
    {
        ImGui::TextColored({ 1, 0, 0, 1 }, "No Skills Selected!");
        ImGui::Spacing();
    }

    if (parsed.SkillsLand && parsed.SkillsWater && parsed.TraitLines)
    {
        switch (parsed.Profession)
        {
            case GW2::Profession::Ranger:
            {
                if (editTarget)
                    API::Instance().PreloadAllPets();

                ImVec2 const TYPE_SIZE { 16px, 16px };
                ImVec2 const PET_SIZE { 40px, 40px };
                float const PET_SPACING = 10px;
                ImVec2 const PET_SELECTION_SIZE { 56px, 56px };
                float const PET_SELECTION_SPACING = 0px;
                uint8_t const MAX_PETS_PER_ROW = 6;

                ImGui::Spacing();
                ImGui::SameLine(0, std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - (TYPE_SIZE.x + PET_SIZE.x * 2 + PET_SPACING + TYPE_SIZE.x + PET_SIZE.x * 2)) / 2)));
                for (uint8_t water = 0; water < 2; ++water)
                {
                    float const offset = (PET_SIZE.y - TYPE_SIZE.y) / 2 + (editTarget ? GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE : 0);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
                    ImGui::Image(GetIcon(water ? Icons::WaterSkills : Icons::LandSkills).Texture, TYPE_SIZE);
                    ImGui::SameLine(0, 0);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - offset);
                    for (uint8_t i = 0; i < 2; ++i)
                    {
                        std::string const guid = fmt::format("##RangerPetSelection{}{}", water ? "Water" : "Land", i);
                        ImVec2 cursor = ImGui::GetCurrentWindow()->Pos + ImGui::GetCursorPos();
                        bool hovered = ImGui::IsMouseHoveringRect(cursor, cursor + ImVec2 { PET_SIZE.x, GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE + PET_SIZE.y }) && ImGui::IsMouseHoveringWindow();
                        bool active = ImGui::IsPopupOpenPublic(guid.c_str());
                        float multiplier = editTarget ? (active ? 0.25f : hovered ? 1.0f : 0.8f) : 1.0f;
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
                        ImGui::BeginGroup();
                        if (editTarget)
                        {
                            auto const& icon = GetIcon(Icons::SelectionChevron);
                            ImGui::NewLine();
                            ImGui::SameLine(0, PET_SIZE.x - icon.TrimmedSize().x * UI_SCALE);
                            ImGui::GetCurrentWindow()->DC.CurrentLineHeight = 0.0f;
                            ImGui::GetCurrentWindow()->DC.CurrentLineTextBaseOffset = 0.0f;
                            ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                            color.x *= multiplier;
                            color.y *= multiplier;
                            color.z *= multiplier;
                            ImGui::Image(icon.Texture, icon.TrimmedSize() * UI_SCALE, icon.GetUV0(), icon.GetUV1(), color);
                        }
                        {
                            auto const& id = (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[i];
                            auto const& pet = API::Pet::Get(id);
                            auto const& icon = id ? pet.Icon : GetIcon(Icons::MissingPet);
                            ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                            color.x *= multiplier;
                            color.y *= multiplier;
                            color.z *= multiplier;
                            ImGui::Image(icon.Texture, PET_SIZE, icon.GetUV0(), icon.GetUV1(), color);
                            if (ImGui::IsItemHovered())
                                ImGui::TooltipWithHeader(pet ? pet.Name.c_str() : nullptr, "Click to change pet");
                        }
                        ImGui::EndGroup();
                        if (editTarget && m_petsLoaded)
                        {
                            auto window = ImGui::GetCurrentWindow()->RootWindow;
                            if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
                                ImGui::OpenPopup(guid.c_str());

                            std::vector<Pet const*> list;
                            list.reserve(m_pets.size());
                            util::transform_if(m_pets.begin(), m_pets.end(), std::back_inserter(list),
                                [&](Pet const& info)
                            {
                                if (!water && info.Type != Pet::PetType::Terrestrial && info.Type != Pet::PetType::Amphibious)
                                    return false;

                                if (water && info.Type != Pet::PetType::Aquatic && info.Type != Pet::PetType::Amphibious)
                                    return false;

                                return true;
                            }, [](Pet const& info) { return &info; });
                            ImVec2 pos =
                            {
                                cursor.x + PET_SIZE.x / 2 - (1 + WINDOW_PADDING.y + (PET_SELECTION_SIZE.x + PET_SELECTION_SPACING) * std::min<size_t>(list.size(), MAX_PETS_PER_ROW) - PET_SELECTION_SPACING + WINDOW_PADDING.y + 1) / 2,
                                cursor.y - (1 + WINDOW_PADDING.y + (PET_SELECTION_SIZE.x + PET_SELECTION_SPACING) * ((list.size() + MAX_PETS_PER_ROW - 1) / MAX_PETS_PER_ROW) - PET_SELECTION_SPACING + WINDOW_PADDING.y + 1)
                            };
                            if (pos.y < 0)
                                pos.y = 0;
                            ImGui::SetNextWindowPos(pos, ImGuiSetCond_Always);
                            if (ImGui::BeginPopup(guid.c_str()))
                            {
                                ImGui::SetWindowFontScale(UI_SCALE);
                                std::optional<uint8_t> selection;
                                uint32_t j = 0;
                                for (auto const& ptr : list)
                                {
                                    auto const& info = *ptr;
                                    auto const& pet = API::Pet::Get(info.ID);
                                    auto const& icon = info.ID ? pet.Icon : GetIcon(Icons::MissingPet);
                                    cursor = ImGui::GetCurrentWindow()->Pos + ImGui::GetCursorPos();
                                    hovered = ImGui::IsMouseHoveringRect(cursor, cursor + PET_SELECTION_SIZE) && ImGui::IsMouseHoveringWindow();
                                    active = info.ID && (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[1 - i] == info.ID || (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[i] == info.ID;
                                    multiplier = editTarget ? (active ? 0.25f : hovered ? 1.0f : 0.8f) : 1.0f;
                                    ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                                    color.x *= multiplier;
                                    color.y *= multiplier;
                                    color.z *= multiplier;
                                    ImGui::Image(icon.Texture, PET_SELECTION_SIZE, icon.GetUV0(), icon.GetUV1(), color);
                                    if (ImGui::IsItemHovered() && pet)
                                        ImGui::Tooltip(pet.Name.c_str());
                                    if (ImGui::IsItemClicked())
                                        selection = info.ID;
                                    if (++j % MAX_PETS_PER_ROW)
                                        ImGui::SameLine(0, PET_SELECTION_SPACING);
                                    else
                                        ImGui::InvisibleButton("", { PET_SELECTION_SPACING, PET_SELECTION_SPACING });
                                }

                                if (selection)
                                {
                                    // Hack to prevent modal window from closing
                                    ImGui::CloseCurrentPopup();
                                    ImGui::FocusWindow(window);
                                    window->WasActive = false;

                                    if (!*selection || (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[1 - i] != selection)
                                    {
                                        if (auto link = ChatLink::Decode(editTarget->GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                                        {
                                            auto& data = std::get<ChatLink::BuildTemplate>(*link);
                                            (water ? data.ProfessionSpecific.RangerPets.Water : data.ProfessionSpecific.RangerPets.Land)[i] = *selection;
                                            editTarget->SetLink(ChatLink::Encode(data));
                                        }
                                    }
                                }
                                ImGui::EndPopup();
                            }
                        }
                        ImGui::PopStyleVar();
                        ImGui::SameLine(0, 0);
                    }
                    ImGui::SameLine(0, PET_SPACING);
                }
                ImGui::NewLine();
                break;
            }
            case GW2::Profession::Revenant:
            {
                ImVec2 const TYPE_SIZE { 16px, 16px };
                ImVec2 const LEGEND_SIZE { 32px, 32px };
                float const LEGEND_SPACING = 16px;
                ImVec2 const LEGEND_SELECTION_SIZE { 45px, 45px };
                float const LEGEND_SELECTION_SPACING = 10px;
                uint8_t const MAX_LEGENDS_PER_ROW = 4;

                ImGui::Spacing();
                ImGui::SameLine(0, std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - (TYPE_SIZE.x + LEGEND_SIZE.x * 2 + LEGEND_SPACING + TYPE_SIZE.x + LEGEND_SIZE.x * 2)) / 2)));
                for (uint8_t water = 0; water < 2; ++water)
                {
                    float const offset = (LEGEND_SIZE.y - TYPE_SIZE.y) / 2 + (editTarget ? GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE : 0);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
                    ImGui::Image(GetIcon(water ? Icons::WaterSkills : Icons::LandSkills).Texture, TYPE_SIZE);
                    ImGui::SameLine(0, 0);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - offset);
                    for (uint8_t i = 0; i < 2; ++i)
                    {
                        std::string const guid = fmt::format("##RevenantLegendSelection{}{}", water ? "Water" : "Land", 1 - i);
                        ImVec2 cursor = ImGui::GetCurrentWindow()->Pos + ImGui::GetCursorPos();
                        bool hovered = ImGui::IsMouseHoveringRect(cursor, cursor + ImVec2 { LEGEND_SIZE.x, GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE + LEGEND_SIZE.y }) && ImGui::IsMouseHoveringWindow();
                        bool active = ImGui::IsPopupOpenPublic(guid.c_str());
                        float multiplier = editTarget ? (active ? 0.25f : hovered ? 1.0f : 0.8f) : 1.0f;
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
                        ImGui::BeginGroup();
                        if (editTarget)
                        {
                            auto const& icon = GetIcon(Icons::SelectionChevron);
                            ImGui::NewLine();
                            ImGui::SameLine(0, LEGEND_SIZE.x - icon.TrimmedSize().x * UI_SCALE);
                            ImGui::GetCurrentWindow()->DC.CurrentLineHeight = 0.0f;
                            ImGui::GetCurrentWindow()->DC.CurrentLineTextBaseOffset = 0.0f;
                            ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                            color.x *= multiplier;
                            color.y *= multiplier;
                            color.z *= multiplier;
                            ImGui::Image(icon.Texture, icon.TrimmedSize() * UI_SCALE, icon.GetUV0(), icon.GetUV1(), color);
                        }
                        {
                            auto const& info = GW2::GetRevenantLegendInfo((water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[1 - i]);
                            auto const& skill = API::Skill::Get(info.SwapSkill);
                            auto const& icon = info.Legend != GW2::RevenantLegend::None ? skill.Icon : GetIcon(Icons::MissingSkill);
                            ImVec4 color = i || info.Legend == GW2::RevenantLegend::None ? ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f } : ImVec4 { 0.5f, 0.5f, 0.5f, 1.0f };
                            color.x *= multiplier;
                            color.y *= multiplier;
                            color.z *= multiplier;
                            ImGui::Image(icon.Texture, LEGEND_SIZE, icon.GetUV0(), icon.GetUV1(), color);
                            if (ImGui::IsItemHovered())
                                ImGui::TooltipWithHeader(skill ? skill.Name.c_str() : nullptr, "Click to change pet");
                        }
                        ImGui::EndGroup();
                        if (editTarget)
                        {
                            auto window = ImGui::GetCurrentWindow()->RootWindow;
                            if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
                                ImGui::OpenPopup(guid.c_str());

                            std::vector<GW2::RevenantLegendInfo const*> list;
                            list.reserve(GW2::GetRevenantLegendInfos().size());
                            util::transform_if(GW2::GetRevenantLegendInfos().begin(), GW2::GetRevenantLegendInfos().end(), std::back_inserter(list),
                                [&](GW2::RevenantLegendInfo const& info)
                                {
                                    if (GW2::GetSpecializationInfo(editTarget->GetParsedSpecialization()).Elite &&
                                        info.RequiredSpecialization != GW2::Specialization::None &&
                                        info.RequiredSpecialization != editTarget->GetParsedSpecialization())
                                        return false;

                                    if (!water && !info.Terrestrial)
                                        return false;

                                    if (water && !info.Aquatic)
                                        return false;

                                    return true;
                                }, [](GW2::RevenantLegendInfo const& info) { return &info; });
                            ImVec2 pos =
                            {
                                cursor.x + LEGEND_SIZE.x / 2 - (1 + WINDOW_PADDING.y + (LEGEND_SELECTION_SIZE.x + LEGEND_SELECTION_SPACING) * std::min<size_t>(list.size(), MAX_LEGENDS_PER_ROW) - LEGEND_SELECTION_SPACING + WINDOW_PADDING.y + 1) / 2,
                                cursor.y - (1 + WINDOW_PADDING.y + (LEGEND_SELECTION_SIZE.x + LEGEND_SELECTION_SPACING) * ((list.size() + MAX_LEGENDS_PER_ROW - 1) / MAX_LEGENDS_PER_ROW) - LEGEND_SELECTION_SPACING + WINDOW_PADDING.y + 1)
                            };
                            if (pos.y < 0)
                                pos.y = 0;
                            ImGui::SetNextWindowPos(pos, ImGuiSetCond_Always);
                            if (ImGui::BeginPopup(guid.c_str()))
                            {
                                ImGui::SetWindowFontScale(UI_SCALE);
                                std::optional<GW2::RevenantLegend> selection;
                                uint32_t j = 0;
                                for (auto const& ptr : list)
                                {
                                    auto const& info = *ptr;
                                    auto const& skill = API::Skill::Get(info.SwapSkill);
                                    auto const& icon = info.Legend != GW2::RevenantLegend::None ? skill.Icon : GetIcon(Icons::MissingSkill);
                                    cursor = ImGui::GetCurrentWindow()->Pos + ImGui::GetCursorPos();
                                    hovered = ImGui::IsMouseHoveringRect(cursor, cursor + LEGEND_SELECTION_SIZE) && ImGui::IsMouseHoveringWindow();
                                    active = info.Legend != GW2::RevenantLegend::None && (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[i] == info.Legend || (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[1 - i] == info.Legend;
                                    multiplier = editTarget ? (active ? 0.25f : hovered ? 1.0f : 0.8f) : 1.0f;
                                    ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                                    color.x *= multiplier;
                                    color.y *= multiplier;
                                    color.z *= multiplier;
                                    ImGui::Image(icon.Texture, LEGEND_SELECTION_SIZE, icon.GetUV0(), icon.GetUV1(), color);
                                    if (ImGui::IsItemHovered() && skill)
                                        ImGui::Tooltip(skill.Name.c_str());
                                    if (ImGui::IsItemClicked())
                                        selection = info.Legend;
                                    if (++j % MAX_LEGENDS_PER_ROW)
                                        ImGui::SameLine(0, LEGEND_SELECTION_SPACING);
                                    else
                                        ImGui::InvisibleButton("", { LEGEND_SELECTION_SPACING, LEGEND_SELECTION_SPACING });
                                }

                                if (selection)
                                {
                                    // Hack to prevent modal window from closing
                                    ImGui::CloseCurrentPopup();
                                    ImGui::FocusWindow(window);
                                    window->WasActive = false;

                                    if (selection == GW2::RevenantLegend::None || (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[i] != selection)
                                    {
                                        if (auto link = ChatLink::Decode(editTarget->GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                                        {
                                            auto& data = std::get<ChatLink::BuildTemplate>(*link);
                                            (water ? data.ProfessionSpecific.RevenantLegends.Water : data.ProfessionSpecific.RevenantLegends.Land)[1 - i] = *selection;
                                            editTarget->SetLink(ChatLink::Encode(data));
                                        }
                                    }
                                }
                                ImGui::EndPopup();
                            }
                        }
                        ImGui::PopStyleVar();
                        ImGui::SameLine(0, 0);
                    }
                    ImGui::SameLine(0, LEGEND_SPACING);
                }
                ImGui::NewLine();
                break;
            }
            default:
                break;
        }
    }

    if (parsed.TraitLines)
    {
        for (auto const& line : *parsed.TraitLines)
        {
            auto const& info = API::Specialization::Get((uint32_t)line.Specialization);

            std::string const text { info ? info.Name : line.Specialization != GW2::Specialization::None ? GW2::GetSpecializationInfo(line.Specialization).Name : "Empty" };
            if (info.WeaponTrait)
            {
                auto const& traitInfo = API::Trait::Get(info.WeaponTrait);
                auto const& icon = traitInfo.Icon;
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + std::ceil((24px - ImGui::CalcTextSize(text.c_str()).y) / 2));
                ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "%s", text.c_str());
                ImGui::SameLine(0, 5);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - std::ceil((24px - ImGui::CalcTextSize(text.c_str()).y) / 2));
                ImGui::Image(icon.Texture, { 24px, 24px }, icon.GetUV0(), icon.GetUV1());
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip(traitInfo.Name.c_str());
            }
            else
                ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "%s", text.c_str());
            ImGui::NewLine();

            if (!info && line.Specialization != GW2::Specialization::None)
                continue;

            {
                auto const& icon = info && info.Icon ? *info.Icon : GetIcon(Icons::MissingSpecialization);
                ImGui::SameLine(0, std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - (48px + 5px + 24px + 5px + 24px + 5px + 24px)) / 2)));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (24px * 3 - 48px) / 2);
                ImGui::Image(icon.Texture, { 48px, 48px }, icon.GetUV0(), icon.GetUV1());
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip((info ? info.Name : "Empty").c_str());
                ImGui::SameLine();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (24px * 3 - 48px) / 2);
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            for (size_t i = 0; i < 9; ++i)
            {
                if (i % 3 == 0)
                {
                    if (i)
                        ImGui::SameLine(0, 5px);
                    ImGui::BeginGroup();
                }

                if (!info)
                {
                    auto const& icon = GetIcon(Icons::MissingTrait);
                    ImGui::Image(icon.Texture, { 24px, 24px }, icon.GetUV0(), icon.GetUV1());
                    if (ImGui::IsItemHovered())
                        ImGui::Tooltip("Locked");
                }
                else if (uint32_t const trait = info.MajorTraits[i])
                {
                    auto const& traitInfo = API::Trait::Get(trait);
                    auto const& icon = trait ? traitInfo.Icon : GetIcon(Icons::LoadingTrait);
                    ImGui::Image(icon.Texture, { 24px, 24px }, icon.GetUV0(), icon.GetUV1(), line.Traits[i / 3] == i % 3 + 1 ? ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f } : ImVec4 { 0.25f, 0.25f, 0.25f, 1.0f });
                    if (ImGui::IsItemHovered())
                        ImGui::Tooltip(traitInfo.Name.c_str());
                }
                else
                    ImGui::ItemSize({ 24px, 24px });

                if (i % 3 == 2)
                {
                    if (i == 8)
                        ImGui::PopStyleVar();

                    ImGui::EndGroup();
                }
            }
        }
    }
    else if (errorMissing)
        ImGui::TextColored({ 1, 0, 0, 1 }, "No Traits Selected!");

    if (footer)
    {
        ImGui::Spacing();
        ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "Right-click for more options.");
    }
}

void Handler::RenderArcDPSMigration(Time const& delta)
{
    static Build const* selectedTraits = nullptr;
    static Build const* selectedSkills = nullptr;
    static Build previewBuild { 0 };
    static std::optional<bool> lastSelectionWasSkills;
    static bool selectionChanged = false;
    static std::vector<Build> convertedBuilds;
    static size_t convertedBuildsSuccessCount = 0;

    static Time m_fadingSuccessProgress { };
    if (m_fadingSuccessProgress.count())
    {
        if (m_fadingSuccessProgress <= delta)
            m_fadingSuccessProgress = { };
        else
            m_fadingSuccessProgress -= delta;
    }

    static Build::id_t m_tooltipBuild { };
    static bool m_tooltipShown = false;
    static Time m_tooltipDelay { };
    m_tooltipShown = false;
    if (m_tooltipBuild)
    {
        if (m_tooltipDelay <= delta)
        {
            m_tooltipShown = true;
            m_tooltipDelay = { };
        }
        else
            m_tooltipDelay -= delta;
    }

    static auto const areBuildsCompatible = [](Build const& a, Build const& b, bool* outUniquenessTestFailed = nullptr)
    {
        if (a.GetParsedProfession() != b.GetParsedProfession())
            return false;
        if (a.GetParsedSpecialization() != b.GetParsedSpecialization() && GW2::GetSpecializationInfo(a.GetParsedSpecialization()).Elite && GW2::GetSpecializationInfo(b.GetParsedSpecialization()).Elite)
            return false;

        Build test { 0 };
        test.SetLink(a.GetLink());
        test.SetSecondaryLink(b.GetLink());
        auto& savedBuilds = BuildStorage::Instance().GetBuilds();
        if (auto const itr = std::find_if(savedBuilds.begin(), savedBuilds.end(), [&test](Build const& b) { return b.GetLink() == test.GetLink(); }); itr != savedBuilds.end())
        {
            if (outUniquenessTestFailed)
                *outUniquenessTestFailed = true;
            return false;
        }
        return true;
    };

    static bool readMeOpenedOnce = false;
    bool const blinking = !readMeOpenedOnce && BuildStorage::Instance().GetBuilds().empty();
    if (blinking)
    {
        static Time blinkingPeriod;
        blinkingPeriod += delta;
        blinkingPeriod %= 1000;
        ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Header));
        color.w = std::abs((float)blinkingPeriod.count() / 500 - 1);
        ImGui::PushStyleColor(ImGuiCol_Header, color);
    }

    if (ImGui::FixedCollapsingHeader("Read Me"))
    {
        readMeOpenedOnce = true;
        ImGui::TextWrapped("Because ArcDPS stored trait and skill templates separately, and Guild Wars 2 build templates must contain both traits and skills, automatic conversion of all your ArcDPS templates to native build templates is impossible.");
        ImGui::Spacing();
        ImGui::TextWrapped("In this window you can mix and match your old trait and skill templates and combine them together to form a new build template that's usable in the game.");
        ImGui::Spacing();
        ImGui::TextWrapped("Incompatible combinations (e.g. those that use skills or traits from 2 different elite specializations), as well as those that you already have saved, are grayed out for your convenience.");
        ImGui::Spacing();
        ImGui::TextWrapped("If you used to have trait and skill templates with the same name, they can automatically be converted by clicking the button below. Converting the rest manually is up to you.");
        if (ImGui::Button("Automatically convert templates with the same name"))
        {
            convertedBuildsSuccessCount = 0;
            convertedBuilds.clear();
            convertedBuilds.reserve(std::min<size_t>(m_arcdpsTraits.size(), m_arcdpsSkills.size()));

            for (Build const& traits : m_arcdpsTraits)
            {
                for (Build const& skills : m_arcdpsSkills)
                {
                    switch (traits.GetParsedProfession())
                    {
                        case GW2::Profession::Ranger:
                        case GW2::Profession::Revenant:
                            continue;
                        default:
                            break;
                    }

                    if (!areBuildsCompatible(traits, skills))
                        continue;
                    if (traits.GetName() != skills.GetName())
                        continue;

                    Build test { 0 };
                    test.SetName(traits.GetName());
                    test.SetLink(traits.GetLink());
                    test.SetSecondaryLink(skills.GetLink());
                    if (auto itr = std::find_if(convertedBuilds.begin(), convertedBuilds.end(), [&test](Build const& b) { return b.GetLink() == test.GetLink() && b.GetName() == test.GetName(); }); itr != convertedBuilds.end())
                        continue;

                    convertedBuilds.emplace_back(test);
                }
            }

            ImGui::OpenPopup(convertedBuilds.empty() ? "BuildPad##ArcDPSMigrationAutoNone" : "BuildPad##ArcDPSMigrationAutoConfirm");
        }

        ImVec4 color = ImGui::ColorConvertU32ToFloat4(0xFF29DF77);
        color.w = convertedBuildsSuccessCount ? 1.0f : 0.0f;
        ImGui::SameLine(0, 0);
        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), fmt::format("{} Build{} Converted!", convertedBuildsSuccessCount, convertedBuildsSuccessCount != 1 ? "s" : "").c_str(), { 0, 0 }, 0, -1, { 0, 0, 0, 0 }, { 1.0f, 1.0f, 1.0f, color.w });
        ImGui::PopStyleColor(4);
        if (std::find_if(m_arcdpsTraits.begin(), m_arcdpsTraits.end(), [](Build const& build) { return build.GetParsedProfession() == GW2::Profession::Ranger; }) != m_arcdpsTraits.end() ||
            std::find_if(m_arcdpsSkills.begin(), m_arcdpsSkills.end(), [](Build const& build) { return build.GetParsedProfession() == GW2::Profession::Ranger; }) != m_arcdpsSkills.end())
        {
            ImGui::Spacing();
            ImGui::TextWrapped("Ranger builds will not be converted automatically due to missing information about selected pets.");
        }
        if (std::find_if(m_arcdpsTraits.begin(), m_arcdpsTraits.end(), [](Build const& build) { return build.GetParsedProfession() == GW2::Profession::Revenant; }) != m_arcdpsTraits.end() ||
            std::find_if(m_arcdpsSkills.begin(), m_arcdpsSkills.end(), [](Build const& build) { return build.GetParsedProfession() == GW2::Profession::Revenant; }) != m_arcdpsSkills.end())
        {
            ImGui::Spacing();
            ImGui::TextWrapped("Revenant builds will not be converted automatically due to missing information about selected legends.");
        }
    }

    if (blinking)
        ImGui::PopStyleColor();

    ImGui::Columns(3);

    static auto const deselect = [](bool skills)
    {
        (skills ? selectedSkills : selectedTraits) = nullptr;
        if (selectedSkills || selectedTraits)
            lastSelectionWasSkills = selectedSkills;
        else
            lastSelectionWasSkills.reset();
    };
    static auto const buildList = [this](std::list<Build>& builds, bool skills)
    {
        bool firstVisibleBuild = true;
        auto const buildListPass = [this, &firstVisibleBuild](std::list<Build>& builds, bool skills, GW2::Profession profession)
        {
            Build const*& other = skills ? selectedTraits : selectedSkills;

            bool firstSortedBuild = true;
            for (Build const& build : builds)
            {
                if (build.GetParsedProfession() != profession)
                    continue;

                bool exists = false;
                bool incompatible = false;
                if (other)
                {
                    if (build.GetParsedProfession() != other->GetParsedProfession())
                        continue;

                    if (!areBuildsCompatible(build, *other, &exists))
                        incompatible = true;
                }

                bool const selected = (skills ? selectedSkills : selectedTraits) == &build;
                BeginRenderBuildList(build.GetParsedProfession(), firstVisibleBuild, firstSortedBuild, false, build.GetParsedProfession(), selected ? 0.4f : 1.0f);
                if (selected)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(0xFF44BBEE));
                    ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(0xFF44BBEE));
                }

                if (exists || incompatible)
                {
                    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button));
                    color.x = color.y = color.z = (color.x + color.y + color.z) / 3;
                    ImGui::PushStyleColor(ImGuiCol_Button, color);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                    ImGui::PushStyleColor(ImGuiCol_Text, exists ? ImVec4 { 0, 0, 0, 1.0f } : ImVec4 { 0.25f, 0, 0, 1.0f });
                }

                ImGui::PushItemWidth(-1);
                if (TextureData const& icon = build.GetParsedSpecialization() != GW2::Specialization::None ? GetIcon(build.GetParsedSpecialization()) : GetIcon(build.GetParsedProfession());
                    ImGui::ImageButtonWithText(icon.Texture, fmt::format("{}##{}", build.GetName(), build.GetID()).c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, -1, 0, { 0, 0, 0, 0 }, exists ? ImVec4 { 0, 0, 0, 1.0f } : incompatible ? ImVec4 { 0.75f, 0, 0, 1.0f } : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f }, selected)
                    && !exists && !incompatible)
                {
                    if (selected)
                        deselect(skills);
                    else
                    {
                        if (other && build.GetParsedProfession() != other->GetParsedProfession())
                            deselect(!skills);

                        (skills ? selectedSkills : selectedTraits) = &build;
                        lastSelectionWasSkills = skills;
                    }
                    selectionChanged = true;
                }
                ImGui::PopItemWidth();

                if (exists || incompatible)
                    ImGui::PopStyleColor(4);

                if (selected)
                    ImGui::PopStyleColor(2);

                bool const hovered = ImGui::IsItemHovered();
                if (m_tooltipBuild == build.GetID())
                {
                    if (!hovered)
                        m_tooltipBuild = { };
                    else if (m_tooltipShown)
                    {
                        ImGui::BeginTooltip();
                        ImGui::SetWindowFontScale(UI_SCALE);
                        if (exists)
                            ImGui::Text("You already have a build with this combination of traits and skills saved");
                        else if (incompatible)
                            ImGui::Text("This combination of builds has incompatible specializations");
                        else
                            RenderBuildTooltip(build, false);
                        ImGui::EndTooltip();
                    }
                }
                else if (hovered)
                {
                    m_tooltipBuild = build.GetID();
                    m_tooltipShown = false;
                    m_tooltipDelay = std::chrono::milliseconds { m_config.TooltipDelay };
                }

                EndRenderBuildList(false);
                firstVisibleBuild = false;
            }
        };

        for (auto const& info : GW2::GetProfessionInfos())
        {
            if (info.Profession == GW2::Profession::None)
                continue;

            buildListPass(builds, skills, info.Profession);
        }
    };
    if (selectedTraits && selectedSkills && !areBuildsCompatible(*selectedTraits, *selectedSkills))
    {
        if (lastSelectionWasSkills)
            deselect(*lastSelectionWasSkills);
        else
            selectedSkills = selectedTraits = nullptr;
        selectionChanged = true;
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Traits").x) / 2);
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "Traits");
    ImGui::BeginChild("Traits##ArcDPSMigration", { 0, 0 });
    ImGui::SetWindowFontScale(UI_SCALE);
    buildList(m_arcdpsTraits, false);
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Skills").x) / 2);
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "Skills");
    ImGui::BeginChild("Skills##ArcDPSMigration", { 0, 0 });
    ImGui::SetWindowFontScale(UI_SCALE);
    buildList(m_arcdpsSkills, true);
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("PreviewHeader##ArcDPSMigration", { 0, -1 });
    ImGui::SetWindowFontScale(UI_SCALE);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Combined Build").x) / 2);
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "Combined Build");
    if (selectionChanged)
    {
        selectionChanged = false;

        auto const previousRangerPetsLand = previewBuild.GetParsedInfo().RangerPetsLand;
        auto const previousRangerPetsWater = previewBuild.GetParsedInfo().RangerPetsWater;
        auto previousRevenantLegendsLand = previewBuild.GetParsedInfo().RevenantLegendsLand;
        auto previousRevenantLegendsWater = previewBuild.GetParsedInfo().RevenantLegendsWater;

        previewBuild.SetName({});
        previewBuild.SetSecondaryLink({});
        previewBuild.SetLink({});

        if (selectedTraits && selectedSkills && selectedTraits->GetName() == selectedSkills->GetName())
            previewBuild.SetName(selectedTraits->GetName());
        else if (selectedTraits && selectedSkills)
            previewBuild.SetName(fmt::format("{} / {}", selectedTraits->GetName(), selectedSkills->GetName()));
        else if (selectedTraits)
            previewBuild.SetName(selectedTraits->GetName());
        else if (selectedSkills)
            previewBuild.SetName(selectedSkills->GetName());

        std::string_view a = selectedTraits ? selectedTraits->GetLink() : "";
        std::string_view b = selectedSkills ? selectedSkills->GetLink() : "";
        previewBuild.SetLink(a.empty() ? b : a);
        if (!a.empty() && !b.empty())
            previewBuild.SetSecondaryLink(b);

        switch (previewBuild.GetParsedProfession())
        {
            case GW2::Profession::Ranger:
                if (auto link = ChatLink::Decode(previewBuild.GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                {
                    auto& data = std::get<ChatLink::BuildTemplate>(*link);
                    data.ProfessionSpecific.RangerPets.Land[0] = previousRangerPetsLand[0];
                    data.ProfessionSpecific.RangerPets.Land[1] = previousRangerPetsLand[1];
                    data.ProfessionSpecific.RangerPets.Water[0] = previousRangerPetsWater[0];
                    data.ProfessionSpecific.RangerPets.Water[1] = previousRangerPetsWater[1];
                    previewBuild.SetLink(ChatLink::Encode(data));
                }
                break;
            case GW2::Profession::Revenant:
                if (GW2::GetSpecializationInfo(previewBuild.GetParsedSpecialization()).Elite)
                    for (uint8_t water = 0; water < 2; ++water)
                        for (uint8_t i = 0; i < 2; ++i)
                            if (auto& legend = (water ? previousRevenantLegendsWater : previousRevenantLegendsLand)[i]; legend != GW2::RevenantLegend::None)
                                if (auto const& info = GW2::GetRevenantLegendInfo(legend); info.RequiredSpecialization != GW2::Specialization::None && info.RequiredSpecialization != previewBuild.GetParsedSpecialization())
                                    legend = GW2::RevenantLegend::None;

                if (auto link = ChatLink::Decode(previewBuild.GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                {
                    auto& data = std::get<ChatLink::BuildTemplate>(*link);
                    data.ProfessionSpecific.RevenantLegends.Land[0] = previousRevenantLegendsLand[0];
                    data.ProfessionSpecific.RevenantLegends.Land[1] = previousRevenantLegendsLand[1];
                    data.ProfessionSpecific.RevenantLegends.Water[0] = previousRevenantLegendsWater[0];
                    data.ProfessionSpecific.RevenantLegends.Water[1] = previousRevenantLegendsWater[1];
                    previewBuild.SetLink(ChatLink::Encode(data));
                }
                break;
            default:
                break;
        }
    }

    ImGui::PushItemWidth(-1);
    auto buffer = util::to_buffer(previewBuild.GetName());
    if (ImGui::InputText("##ArcDPSMigrationEditName", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
        previewBuild.SetName(buffer.data());
    ImGui::PopItemWidth();

    if (previewBuild.GetName().empty())
    {
        ImGui::SameLine(0, 0);
        ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
        color.w /= 4;
        ImGui::SameLine(-1, 1 + FRAME_PADDING.x);
        ImGui::TextColored(color, "Build Name");
    }

    // TODO: Add flags

    ImGui::NewLine();
    //if (m_fadingSuccessProgress.count())
    {
        float const alpha = std::min<float>(1.0f, (float)m_fadingSuccessProgress.count() / MIGRATION_SUCCESS_FADE_DURATION_FADE.count());
        ImVec4 color = ImGui::ColorConvertU32ToFloat4(0xFF29DF77);
        color.w = alpha;
        ImGui::SameLine(0, 0);
        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0, 0 });
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), "Build Saved!", { 0, 0 }, 0, -1, { 0, 0, 0, 0 }, { 1.0f, 1.0f, 1.0f, color.w });
        ImGui::PopStyleColor(4);
    }

    bool const disabled = !selectedTraits || !selectedSkills;
    if (disabled)
    {
        ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Button));
        color.x = color.y = color.z = (color.x + color.y + color.z) / 3;
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    }

    if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - FRAME_PADDING.x - ImGui::CalcTextSize("Save").x - FRAME_PADDING.x, 0);
        ImGui::ButtonEx("Save##ArcDPSMigrationEditName", { 0, 0 }, disabled ? ImGuiButtonFlags_Disabled : 0))
    {
        Build& build = BuildStorage::Instance().AddBuild();
        build.SetName(previewBuild.GetName());
        build.SetLink(previewBuild.GetLink());
        build.ToggleFlag(previewBuild.GetFlags(), true);

        if (lastSelectionWasSkills)
            deselect(*lastSelectionWasSkills);
        else
            selectedTraits = selectedSkills = nullptr;
        selectionChanged = true;
        m_fadingSuccessProgress = MIGRATION_SUCCESS_FADE_DURATION_FULL;
    }

    if (disabled)
        ImGui::PopStyleColor(3);

    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Border)), "Preview:");
    ImGui::Separator();
    ImGui::GetCurrentWindowRead()->Size.y = ImGui::GetCursorPosY();
    ImGui::EndChild();
    ImGui::BeginChild("Preview##ArcDPSMigration", { 0, 0 });
    ImGui::SetWindowFontScale(UI_SCALE);
    RenderBuildTooltip(previewBuild, false, true, &previewBuild);
    ImGui::EndChild();
    ImGui::Columns(1);

    if (ImGui::BeginPopupModal("BuildPad##ArcDPSMigrationAutoConfirm", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetWindowFontScale(UI_SCALE);
        ImGui::Text("The following builds will be saved:");

        for (Build const& build : convertedBuilds)
        {
            bool on = true;
            ImGui::CheckboxImage(GetIcon(build.GetParsedSpecialization()).Texture, fmt::format("{}", build.GetName()).c_str(), &on, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        }
        ImGui::NewLine();
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 60px - ITEM_SPACING.x - 60px); ImGui::Button("OK", { 60, 0 }))
        {
            for (Build const& converted : convertedBuilds)
            {
                Build& build = BuildStorage::Instance().AddBuild();
                build.SetName(converted.GetName());
                build.SetLink(converted.GetLink());
                build.ToggleFlag(converted.GetFlags(), true);
            }
            convertedBuildsSuccessCount = convertedBuilds.size();
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::SameLine(); ImGui::Button("Cancel", { 60px, 0 }))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopupModal("BuildPad##ArcDPSMigrationAutoNone", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetWindowFontScale(UI_SCALE);
        ImGui::Text("No new builds were found that aren't already saved.");

        ImGui::NewLine();
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 60px); ImGui::Button("Close", { 60px, 0 }))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void Handler::RenderArcDPSGear(Time const& delta)
{
    using gear_t = ArcDPSGearTemplate;
    using legendary_t = ArcDPSLegendaryTemplate;
    static decltype(m_arcdpsGear)::const_pointer selection;

    struct ItemStatsIconsInfo
    {
        std::string Name;
        bool Items = true;
        std::optional<TextureData> Atlas;
        uint32_t IconWidth;
        uint32_t IconHeight;
        struct IconInfo
        {
            std::optional<TextureData> Icon;
            uint32_t OffsetX = 0;
            uint32_t OffsetY = 0;
        };
        std::map<uint32_t, IconInfo> Icons;
        std::map<uint32_t, uint32_t> StatsLUT;

        [[nodiscard]] std::optional<TextureData> GetIcon(uint32_t stats) const
        {
            if (auto const itr = StatsLUT.find(stats); itr != StatsLUT.end())
                stats = itr->second;

            if (auto const itr = Icons.find(stats); itr != Icons.end())
            {
                auto const& info = itr->second;
                if (auto const& icon = info.Icon ? info.Icon : Atlas)
                    return TextureData { *icon }.Trim(info.OffsetX, info.OffsetY, icon->Width - info.OffsetX - IconWidth, icon->Height - info.OffsetY - IconHeight);
            }
            return { };
        }
    };
    static std::vector<ItemStatsIconsInfo> iconSets;
    static bool iconSetsInited = false;
    if (!iconSetsInited)
    {
        iconSetsInited = true;
        iconSets.emplace_back();
        Web::Instance().Request("https://buildpad.gw2archive.eu/itemstatsicons.json", [this](std::string_view const data)
        {
            for (auto const& icons : nlohmann::json::parse(data.begin(), data.end(), nullptr, false))
            {
                auto& info = iconSets.emplace_back();
                info.Name = icons["name"];
                info.Items = icons["items"];
                info.IconWidth = icons["width"];
                info.IconHeight = icons["height"];
                if (icons.contains("atlas"))
                    Web::Instance().Request(icons["atlas"], [this, &info](std::string_view const data) { info.Atlas = LoadTexture(std::pair { data.data(), data.size() }); });

                for (auto const& stats : icons["stats"])
                {
                    uint32_t firstID = 0;
                    for (auto const& id : stats["ids"])
                    {
                        if (!firstID)
                            firstID = id;

                        info.StatsLUT[id] = firstID;
                    }

                    if (firstID)
                    {
                        auto& icon = info.Icons[firstID];
                        if (stats.contains("x"))
                            icon.OffsetX = stats["x"];
                        if (stats.contains("y"))
                            icon.OffsetX = stats["y"];
                        if (stats.contains("icon"))
                            Web::Instance().Request(icons["icon"], [this, &icon](std::string_view const data) { icon.Icon = LoadTexture(std::pair { data.data(), data.size() }); });
                    }
                }
            }
        });
    }

    ImGui::Columns(2);

    ImGui::BeginChild("Gear##ArcDPSGear", { 0, 0 });
    ImGui::SetWindowFontScale(UI_SCALE);
    bool firstVisibleTemplate = true;
    for (auto const& info : GW2::GetProfessionInfos())
    {
        if (info.Profession == GW2::Profession::None)
            continue;

        bool firstSortedTemplate = true;
        for (auto const& temp : m_arcdpsGear)
        {
            bool const legendary = std::holds_alternative<legendary_t>(temp);
            legendary_t const& base = legendary ? std::get<legendary_t>(temp) : std::get<gear_t>(temp);

            if ((GW2::Profession)base.Profession != info.Profession)
                continue;

            bool const selected = selection == &temp;
            BeginRenderBuildList((GW2::Profession)base.Profession, firstVisibleTemplate, firstSortedTemplate, false, (GW2::Profession)base.Profession, selected ? 0.4f : 1.0f);
            if (legendary)
            {
                float const colorMultiplier = selected ? 0.4f : 1.0f;
                ImVec4 color = ImGui::ColorConvertU32ToFloat4(0xFFFF3399);
                color.x *= colorMultiplier;
                color.y *= colorMultiplier;
                color.z *= colorMultiplier;
                color.w *= ImGui::ColorConvertU32ToFloat4(m_config.ProfessionColors[base.Profession]).w;
                ImGui::PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, color.w * 0.6f });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, { color.x, color.y, color.z, color.w * 0.8f });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
            }
            if (selected)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(0xFF44BBEE));
                ImGui::PushStyleColor(ImGuiCol_Border, ImGui::ColorConvertU32ToFloat4(0xFF44BBEE));
            }

            std::string name;
            if (legendary)
                name = fmt::format("{}##{}", base.Name.data(), (size_t)&temp);
            else
                name = fmt::format("[{}] {}##{}", std::get<gear_t>(temp).Character.data(), base.Name.data(), (size_t)&temp);

            ImGui::PushItemWidth(-1);
            if (TextureData const& icon = GetIcon((GW2::Profession)base.Profession);
                ImGui::ImageButtonWithText(icon.Texture, name.c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, -1, 0, { 0, 0, 0, 0 }, legendary ? ImGui::ColorConvertU32ToFloat4(0xFFFF3399) : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f }, selected))
            {
                if (selected)
                    selection = { };
                else
                    selection = &temp;
            }
            ImGui::PopItemWidth();

            if (legendary)
                ImGui::PopStyleColor(3);
            if (selected)
                ImGui::PopStyleColor(2);

            EndRenderBuildList(false);
            firstVisibleTemplate = false;
        }
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Preview##ArcDPSGear", { 0, iconSets.size() > 1 ? -(ITEM_SPACING.y + BUTTON_SIZE.y) : 0 });
    ImGui::SetWindowFontScale(UI_SCALE);
    float const iconHeight = LINE_SIZE.y + ITEM_SPACING.y + LINE_SIZE.y + ITEM_SPACING.y + LINE_SIZE.y;
    ImVec2 const iconSize { iconHeight, iconHeight };
    if (selection)
    {
        bool const legendary = std::holds_alternative<legendary_t>(*selection);
        legendary_t const& base = legendary ? std::get<legendary_t>(*selection) : std::get<gear_t>(*selection);

        auto const& iconSet = [this]() -> ItemStatsIconsInfo const&
        {
            if (auto const itr = std::find_if(iconSets.begin(), iconSets.end(), [this](ItemStatsIconsInfo const& info) { return info.Name == m_config.GearIconSet; }); itr != iconSets.end())
                return *itr;
            return iconSets.front();
        }();
        ImVec2 const iconSizeOverlay { std::min<float>(iconSize.x, iconSet.IconWidth * UI_SCALE), std::min<float>(iconSize.y, iconSet.IconHeight * UI_SCALE) };

        API::Instance().PreloadAllGearInfos(base);

        for (auto const& slot : GW2::GetSlotInfos())
        {
            auto const& item = base.Items[(size_t)slot.Slot];

            {
                auto const& icon = GetIcon(slot.Slot);
                ImGui::Image(icon.Texture, iconSize, icon.GetUV0(), icon.GetUV1());
            }
            if (!item.Item)
                continue;

            {
                auto const& info = API::Item::Get(item.Item);
                if (iconSet.Items)
                {
                    ImGui::SameLine(0.001f);
                    ImGui::Image(info.Icon.Texture, iconSize);
                }
                if (auto const icon = iconSet.GetIcon(item.Stats))
                {
                    ImGui::StoreCursor();
                    ImGui::SameLine(0.001f);
                    ImGui::SetCursorPos(ImGui::GetCursorPos() + (iconSize - iconSizeOverlay));
                    ImGui::Image(icon->Texture, iconSizeOverlay, icon->GetUV0(), icon->GetUV1());
                    ImGui::RestoreCursor();
                }
                ImGui::SameLine();
                ImGui::BeginGroup();
                if (item.Stats)
                {
                    ImGui::Text("%s ", API::ItemStats::Get(item.Stats).Name.c_str());
                    ImGui::SameLine(0, 0);
                }
                if (!iconSet.Items)
                {
                    ImGui::Image(info.Icon.Texture, { LINE_SIZE.y, LINE_SIZE.y });
                    ImGui::SameLine();
                }
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(GW2::GetRarityInfo(info.Rarity).Color), "%s", info.Name.c_str());
            }
            for (uint32_t upgrade : item.Upgrades)
            {
                if (!upgrade)
                    continue;

                auto const& info = API::Item::Get(upgrade);
                ImGui::Image(info.Icon.Texture, { LINE_SIZE.y, LINE_SIZE.y });
                ImGui::SameLine();
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(GW2::GetRarityInfo(info.Rarity).Color), "%s", info.Name.c_str());
            }
            for (uint32_t infusion : item.Infusions)
            {
                if (!infusion)
                    continue;

                auto const& info = API::Item::Get(infusion);
                ImGui::Image(info.Icon.Texture, { LINE_SIZE.y, LINE_SIZE.y });
                ImGui::SameLine();
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(GW2::GetRarityInfo(info.Rarity).Color), "%s", info.Name.c_str());
            }
            ImGui::EndGroup();
        }
    }
    else
    {
        for (auto const& slot : GW2::GetSlotInfos())
        {
            auto const& icon = GetIcon(slot.Slot);
            ImGui::Image(icon.Texture, iconSize, icon.GetUV0(), icon.GetUV1());
        }
    }
    ImGui::EndChild();

    if (iconSets.size() > 1)
    {
        static std::string const DEFAULT_NAME = "Default";
        static uint32_t width = []
        {
            uint32_t result = 0;
            for (auto const& info : iconSets)
                result = std::max<uint32_t>(width, (uint32_t)ImGui::CalcTextSize((!info.Name.empty() ? info.Name : DEFAULT_NAME).c_str()).x);
            return result;
        }();

        ImGui::ItemSize({ 0, 0 }, FRAME_PADDING.y);
        ImGui::SameLine(WINDOW_PADDING.x, 0);
        ImGui::Text("Display as:");
        ImGui::SameLine();
        ImGui::PushItemWidth(FRAME_PADDING.x + width + FRAME_PADDING.x + (FRAME_PADDING.x + ImGui::GetFontSize() + FRAME_PADDING.x));
        if (int iconSet = (int)std::distance(iconSets.begin(), std::find_if(iconSets.begin(), iconSets.end(), [this](ItemStatsIconsInfo const& info) { return info.Name == m_config.GearIconSet; }));
            ImGui::Combo("", &iconSet, [](void* data, int const index, char const** text) { return *text = (!iconSets[index].Name.empty() ? iconSets[index].Name : DEFAULT_NAME).c_str(), true; }, nullptr, (int)iconSets.size()))
            m_config.GearIconSet = iconSets[iconSet].Name;
        ImGui::PopItemWidth();
    }
    ImGui::Columns(1);
}

void Handler::VersionCheck()
{
    {
        std::lock_guard lock { m_versionLock };
        m_versionLatest = { };
        m_versionHistory.clear();
        m_versionHistoryColors.clear();
        m_versionHistoryShown = false;
        m_versionCheckResult.reset();
        m_needsUpdate = false;
    }

    Web::Instance().Request("https://buildpad.gw2archive.eu/version.json", [this](std::string_view const data)
    {
        auto json = nlohmann::json::parse(data.begin(), data.end(), nullptr, false);

        std::lock_guard lock { m_versionLock };

        m_versionLatest.Version = json["latest"];
        m_versionLatest.URL = json["url"];

        for (auto const& color : json["colors"].items())
            if (std::string value = color.value(); value.size() == 9 && value[0] == '#')
                m_versionHistoryColors.emplace(color.key(), util::reverse_bytes(util::from_string<uint32_t>(value.substr(1), 16)));

        for (auto const& version : json["history"])
        {
            auto const id = version["version"];
            auto& info = m_versionHistory[id];
            info.Version = id;
            info.URL = version["url"];
            for (auto const& change : version["changes"])
                info.Changes.emplace_back(change["type"], change["text"]);
        }

        m_needsUpdate = m_versionCurrent < m_versionLatest.Version;
        m_versionCheckResult = true;
    }, [this](auto const&) { m_versionCheckResult = false; }, false);
}

void Handler::VersionUpdate()
{
    if (m_versionCurrent == m_versionLatest.Version)
        return;

    if (m_versionUpdateState != VersionUpdateState::Idle && m_versionUpdateState != VersionUpdateState::Error)
        return;

    fs::path const downloadPath = fmt::format("./bin64/d3d9_arcdps_buildpad_{}.dll", m_versionLatest.Version);
    fs::path const versionPath = "./addons/arcdps/arcdps.buildpad/version";

    m_versionUpdateState = VersionUpdateState::Downloading;
    Web::Instance().Request(m_versionLatest.URL, [this, downloadPath, versionPath](std::string_view const data)
    {
        if (data.empty() || data[0] != 0x4D)
            throw std::exception("Failed to download DLL");

        create_directories(downloadPath.parent_path());
        create_directories(versionPath.parent_path());

        std::ofstream { downloadPath, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc } << data;

        m_versionUpdateState = VersionUpdateState::Manifesting;
        std::ofstream { versionPath, std::ofstream::out | std::ofstream::trunc } << m_versionLatest.Version;

        m_versionUpdateState = VersionUpdateState::Done;
    }, [this](std::exception const& e)
    {
        m_versionUpdateError = e.what();
        m_versionUpdateState = VersionUpdateState::Error;
    });
}

void Handler::RenderVersionHistory(bool all)
{
    ImGui::SetNextWindowSizeConstraints({ 400px, 300px }, { 10000px, 10000px });
    ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
    ImGui::Begin("BuildPad##VersionHistory", &m_versionHistoryShown, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(UI_SCALE);
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
    {
        m_versionHistoryShown = false;
        ImGui::CaptureKeyboardFromApp();
    }
    ImGui::BeginChild("Version History", { 0, -ITEM_SPACING.y - LINE_SIZE.y - FRAME_PADDING.y * 2 });
    ImGui::SetWindowFontScale(UI_SCALE);
    for (auto itr = m_versionHistory.rbegin(); itr != m_versionHistory.rend(); ++itr)
    {
        auto const& info = itr->second;

        bool const old = m_versionCurrent < m_versionLatest.Version && info.Version <= m_versionCurrent;
        if (!m_versionHistoryShownAll && old)
            continue;

        ImVec4 color = ImGui::ColorConvertU32ToFloat4(0xFF44BBEE);
        color.w /= old ? 2 : 1;
        ImGui::TextColored(color, info.Version == m_versionCurrent ? "%s - Current Version" : "%s", info.Version.c_str());
        for (auto const&[type, text] : info.Changes)
        {
            if (auto const itr2 = m_versionHistoryColors.find(type); itr2 != m_versionHistoryColors.end())
                color = ImGui::ColorConvertU32ToFloat4(itr2->second);
            else
                color = { 1.0f, 1.0f, 1.0f, 1.0f };
            color.w /= old ? 2 : 1;
            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextWrapped("- %s", text.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    ImGui::Checkbox("Show Previous Versions", &m_versionHistoryShownAll);
    ImGui::End();
}

void Handler::RenderAbout()
{
    ImGui::SetNextWindowSizeConstraints({ 400px, 300px }, { 10000px, 10000px });
    ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
    ImGui::Begin("BuildPad##About", &m_aboutShown, ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(UI_SCALE);
    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(VK_ESCAPE, false))
    {
        m_aboutShown = false;
        ImGui::CaptureKeyboardFromApp();
    }

    ImGui::BeginGroup();
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "BuildPad");
    ImGui::SameLine(0, 0);
    ImVec4 color = ImGui::ColorConvertU32ToFloat4(ImGui::GetColorU32(ImGuiCol_Text));
    color.w /= 2;
    ImGui::TextColored(color, " - version %s", m_versionCurrent.c_str());

    ImGui::TextColored(color, "By ");
    ImGui::SameLine(0, 0);
    ImGui::Hyperlink("/u/e-scrape-artist", L"https://www.reddit.com/message/compose/?to=e-scrape-artist");
    ImGui::EndGroup();

    ImGui::SameLine(0, 0);
    ImGui::SameLine(0, ImGui::GetContentRegionAvailWidth() - FRAME_PADDING.x - ImGui::CalcTextSize("View Version History").x - FRAME_PADDING.x);
    if (ImGui::Button("View Version History"))
    {
        m_versionHistoryShownAll = true;
        m_versionHistoryShown = true;
    }

    ImGui::BeginChild("Credits");
    ImGui::SetWindowFontScale(UI_SCALE);
    static struct
    {
        std::string Name;
        std::string Copyright;
        std::string URL;
    } credits[]
    {
        { "ImGui", "Copyright (c) 2014-2019 Omar Cornut", "https://github.com/ocornut/imgui" },
        { "{fmt}", "Copyright (c) 2012 - 2019, Victor Zverovich", "https://github.com/fmtlib/fmt" },
        { "JSON for Modern C++", "Copyright (c) 2013-2019 Niels Lohmann", "https://github.com/nlohmann/json" },
        { "cpp-base64", u8"Copyright (C) 2004-2017 René Nyffenegger", "https://github.com/ReneNyffenegger/cpp-base64" },
        { "INI Not Invented Here", "Copyright (c) 2009, Ben Hoyt", "https://github.com/jtilly/inih" },
        { "libspng", "Copyright (c) 2018-2019, Randy", "https://libspng.org" },
        { "curlpp", "Copyright (c) <2002-2004> <Jean-Philippe Barrette-LaPierre>", "http://www.curlpp.org" },
        { "curl", "Copyright (c) 1996 - 2019, Daniel Stenberg", "https://curl.haxx.se" },
        { "zlib", "Copyright (C) 1995-2017 Jean-loup Gailly and Mark Adler", "https://www.zlib.net" },
    };
    float const indent = ImGui::CalcTextSize("Uses ").x;
    for (auto const& credit : credits)
    {
        ImGui::NewLine();
        ImGui::Text("Uses %s", credit.Name.c_str());
        ImGui::Indent(indent);
        ImGui::Hyperlink(credit.URL.c_str(), fmt::format(L"{}", credit.URL).c_str());
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s", credit.Copyright.c_str());
        ImGui::PopStyleColor();
        ImGui::Unindent(indent);
    }
    ImGui::EndChild();
    ImGui::End();
}

std::tuple<std::vector<char>, uint32_t, uint32_t> Handler::LoadImageFile(std::variant<fs::path, std::pair<char const*, size_t>>&& source) const
{
#define CHECK(expr) if ((expr)) return { }  // NOLINT(cppcoreguidelines-macro-usage)
    std::vector<char> buffer;

    CHECK(!std::visit(overloaded
    {
        [&buffer](fs::path const& path)
        {
            assert(path.has_filename());

            FILE* png;
            if (_wfopen_s(&png, path.c_str(), L"rb"))
                return false;
            auto guard = gsl::finally([png] { fclose(png); });
            fseek(png, 0, SEEK_END);
            buffer.resize(ftell(png));
            rewind(png);
            return fread(buffer.data(), buffer.size(), 1, png) == 1;
        },
        [&buffer](std::pair<char const*, size_t>& data)
        {
            buffer.assign(data.first, data.first + data.second);
            return true;
        },
    }, source));

    spng_ctx* ctx = spng_ctx_new(0);
    auto guard = gsl::finally([ctx] { spng_ctx_free(ctx); });
    CHECK(spng_set_png_buffer(ctx, buffer.data(), buffer.size()));
    spng_ihdr header { };
    CHECK(spng_get_ihdr(ctx, &header));
    size_t size;
    CHECK(spng_decoded_image_size(ctx, SPNG_FMT_BGRA8, &size));
    std::vector<char> out(size);
    CHECK(spng_decode_image(ctx, (byte*)out.data(), out.size(), SPNG_FMT_BGRA8, 0));
    return { std::move(out), header.width, header.height };
#undef CHECK
}

std::optional<TextureData> Handler::LoadTexture(std::variant<fs::path, std::pair<char const*, size_t>>&& source) const
{
    try
    {
        auto&& [data, width, height] = LoadImageFile(std::move(source));
        if (data.empty())
            return { };

        TextureID texture;
        if (m_d3dDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, 0))
            return { };

        if (!texture)
            return { };

        D3DLOCKED_RECT rect;
        texture->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
        memcpy(rect.pBits, data.data(), sizeof(char) * width * height * 4);
        texture->UnlockRect(0);

        return TextureData { texture, width, height };
    }
    catch (...) { return { }; }
}
}
