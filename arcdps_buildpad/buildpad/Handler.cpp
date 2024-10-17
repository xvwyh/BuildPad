﻿#include "Handler.h"
#include "BuildStorage.h"
#include "SkillStorage.h"
#include "ChatLink.h"
#include "API.h"
#include "Web.h"
#include "../ImGuiExtensions.h"
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_dx11.h>
#include <inih/INIReader.h>
#include <spng/spng.h>
#include <curlpp/cURLpp.hpp>
#include <json/json.hpp>
#ifdef _DEBUG
#include <base64/base64.h>
#endif

namespace buildpad
{
char const* const BUILDPAD_VERSION = "2024-10-17";

namespace resources
{
#include "resources/102584.h"
#include "resources/155143.h"
#include "resources/155919.h"
#include "resources/155923.h"
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
#include "resources/156668.h"
#include "resources/156675.h"
#include "resources/156679.h"
#include "resources/156721.h"
#include "resources/156723.h"
#include "resources/156794.h"
#include "resources/157086.h"
#include "resources/157121.h"
#include "resources/157145.h"
#include "resources/157262.h"
#include "resources/157325.h"
#include "resources/255430.h"
#include "resources/523389.h"
#include "resources/536048.h"
#include "resources/536052.h"
#include "resources/961376.h"
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
#include "resources/1414035.h"
#include "resources/1424219.h"
#include "resources/1441449.h"
#include "resources/1444520.h"
#include "resources/1444524.h"
#include "resources/1535141.h"
#include "resources/1670506.h"
#include "resources/1716667.h"
#include "resources/1769850.h"
#include "resources/1770024.h"
#include "resources/1770025.h"
#include "resources/1770026.h"
#include "resources/1770027.h"
#include "resources/1770028.h"
#include "resources/1770029.h"
#include "resources/1770030.h"
#include "resources/1770031.h"
#include "resources/1770032.h"
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
#include "resources/2010284.h"
#include "resources/2010285.h"
#include "resources/2010286.h"
#include "resources/2030307.h"
#include "resources/2192623.h"
#include "resources/2192624.h"
#include "resources/2192625.h"
#include "resources/2192626.h"
#include "resources/2192627.h"
#include "resources/2192628.h"
#include "resources/2192629.h"
#include "resources/2208353.h"
#include "resources/2479354.h"
#include "resources/2479358.h"
#include "resources/2479362.h"
#include "resources/2491558.h"
#include "resources/2491562.h"
#include "resources/2491566.h"
#include "resources/2503659.h"
#include "resources/2503663.h"
#include "resources/2503667.h"
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
    if (auto device = GetDirect3DDevice9())
        m_d3dDevice = device;
    if (auto device = GetDirect3DDevice11())
        m_d3dDevice = device;
    LoadIcon(Icons::BuildPad, resources::tex156721).Trim(2);
    LoadIcon(Icons::AddBuild, resources::tex155143);
    LoadIcon(Icons::EditBuild, resources::tex1293677).Trim(3);
    LoadIcon(Icons::AcceptBuildEdit, resources::tex1444520).Trim(6);
    LoadIcon(Icons::CancelBuildEdit, resources::tex1444524).Trim(6);
    LoadIcon(Icons::ClearSearch, resources::tex157325);
    LoadIcon(Icons::Settings, resources::tex156361);
    LoadIcon(Icons::MissingProfession, resources::tex156675);
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
    LoadIcon(Icons::MissingWeapon, resources::tex2208353);
    LoadIcon(Icons::SelectionChevron, resources::tex156058).Trim(2, 4, 2, 3).FlipVertically();
    LoadIcon(Icons::PaletteNotUnderwater, resources::tex157145);
    LoadIcon(Icons::CheckBoxChecked, resources::tex155919);
    LoadIcon(Icons::CheckBoxUnchecked, resources::tex155923);
    LoadIcon(GW2::Profession::None, resources::tex156675);
    LoadIcon(GW2::Profession::Guardian, resources::tex156634);
    LoadIcon(GW2::Profession::Warrior, resources::tex156643);
    LoadIcon(GW2::Profession::Engineer, resources::tex156632);
    LoadIcon(GW2::Profession::Ranger, resources::tex156640);
    LoadIcon(GW2::Profession::Thief, resources::tex156641);
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
    LoadIcon(GW2::Specialization::NecromancerHarbinger, resources::tex2479362);
    LoadIcon(GW2::Specialization::GuardianWillbender, resources::tex2479354);
    LoadIcon(GW2::Specialization::MesmerVirtuoso, resources::tex2479358);
    LoadIcon(GW2::Specialization::ElementalistCatalyst, resources::tex2491558);
    LoadIcon(GW2::Specialization::WarriorBladesworn, resources::tex2491566);
    LoadIcon(GW2::Specialization::RevenantVindicator, resources::tex2491562);
    LoadIcon(GW2::Specialization::EngineerMechanist, resources::tex2503659);
    LoadIcon(GW2::Specialization::ThiefSpecter, resources::tex2503667);
    LoadIcon(GW2::Specialization::RangerUntamed, resources::tex2503663);
    LoadIcon(Build::Flags::Favorite, resources::tex523389).Trim(5);
    LoadIcon(Build::Flags::PvE, resources::tex157086).Trim(2);
    LoadIcon(Build::Flags::PvP, resources::tex157121).Trim(1);
    LoadIcon(Build::Flags::WvW, resources::tex255430).Trim(4);
    LoadIcon(Build::Flags::OpenWorld, resources::tex156723).Trim(1);
    LoadIcon(Build::Flags::Dungeons, resources::tex1535141).Trim(6);
    LoadIcon(Build::Flags::Fractals, resources::tex1441449).Trim(4);
    LoadIcon(Build::Flags::Raids, resources::tex1424219).Trim(4, 5, 6, 5);
    LoadIcon(Build::Flags::Power, resources::tex993687).Trim(1);
    LoadIcon(Build::Flags::Condition, resources::tex156600).Trim(3);
    LoadIcon(Build::Flags::Tank, resources::tex536048).Trim(5);
    LoadIcon(Build::Flags::Support, resources::tex156599).Trim(4);
    LoadIcon(Build::Flags::Heal, resources::tex536052).Trim(4);
    LoadIcon(Build::Flags::Control, resources::tex156668).Trim(2);
    LoadIcon(Build::Flags::Conquest, resources::tex102584).Trim(1);
    LoadIcon(Build::Flags::Stronghold, resources::tex961376).Trim(3);
    LoadIcon(Build::Flags::Duos, resources::tex156679).Trim(0, 0, 2, 2);
    LoadIcon(Build::Flags::Zerg, resources::tex1716667).Trim(8);
    LoadIcon(Build::Flags::Havoc, resources::tex1769850).Trim(8);
    LoadIcon(Build::Flags::Roaming, resources::tex1414035).Trim(0);
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
    LoadIcon(GW2::Weapon::None, resources::tex2208353);
    LoadIcon(GW2::Weapon::Axe, resources::tex1770024);
    LoadIcon(GW2::Weapon::BowLong, resources::tex2010285);
    LoadIcon(GW2::Weapon::Dagger, resources::tex1770025);
    LoadIcon(GW2::Weapon::Focus, resources::tex2192623);
    LoadIcon(GW2::Weapon::Greatsword, resources::tex2010284);
    LoadIcon(GW2::Weapon::Hammer, resources::tex1770026);
    LoadIcon(GW2::Weapon::Mace, resources::tex2192625);
    LoadIcon(GW2::Weapon::Pistol, resources::tex2192626);
    LoadIcon(GW2::Weapon::Rifle, resources::tex1770027);
    LoadIcon(GW2::Weapon::Scepter, resources::tex2192627);
    LoadIcon(GW2::Weapon::Shield, resources::tex1770028);
    LoadIcon(GW2::Weapon::Staff, resources::tex1770030);
    LoadIcon(GW2::Weapon::Sword, resources::tex1770031);
    LoadIcon(GW2::Weapon::Torch, resources::tex1770032);
    LoadIcon(GW2::Weapon::Warhorn, resources::tex2010286);
    LoadIcon(GW2::Weapon::BowShort, resources::tex1770029);
    LoadIcon(GW2::Weapon::Trident, resources::tex2192629);
    LoadIcon(GW2::Weapon::Spear, resources::tex2192624);
    LoadIcon(GW2::Weapon::HarpoonGun, resources::tex2192628);

    VersionCheck();
    m_mumbleLink.Update();
    m_skillLoading = std::async(std::launch::async, [this] { try
    {
        API::Instance().LoadSkillData();

        while (SkillStorage::Instance().GetLoadingState() == SkillStorage::LoadingState::Loading)
            std::this_thread::sleep_for(250ms);

        m_loaded = true;

        if (!SkillStorage::Instance().AreSkillsLoaded())
            return;

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
            if (util::find_if(m_arcdpsTraits, util::method_equals(&Build::GetParsedProfession, GW2::Profession::Revenant)))
            {
                static std::array<uint16_t, 5> const order { 4572, 4564, 4651, 4614, 4554 };

                using code_t = ChatLink::ArcDPSCode<ChatLink::ArcDPSSkillTemplate>;
                code_t code;
                code.Type = 's';
                code.Data.Profession = (uint16_t)GW2::Profession::Revenant;
                std::copy(order.begin(), order.end(), code.Data.Land.begin());
                std::copy(order.begin(), order.end(), code.Data.Water.begin());

                Build& build = m_arcdpsSkills.emplace_back(nextID--);
                build.SetName("Default");
                build.SetLink(ChatLink::Encode(code));
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
    } catch (...) { m_loaded = true; } });
    Web::Instance().RequestAny({ URL_WITH_FALLBACK("/pets.json") }, [this](std::string_view const data)
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
        if (auto const itr = util::find_if(Config::GetFields(), util::first_equals(name)))
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
    auto postConfigLoad = gsl::finally([this]
    {
        API::Instance().SetLanguage(m_config.APILocale);
        m_keyBindToggleBuilds.FromString(m_config.KeyBindToggleBuilds);

        if (m_config.LastViewedVersionHistory.empty())
            m_config.LastViewedVersionHistory = m_config.LastLaunchedVersion;
        if (m_config.LastViewedVersionHistory.empty())
            m_config.LastViewedVersionHistory = m_versionCurrent;
        m_versionLastViewedHistory = m_config.LastViewedVersionHistory;

        if (m_config.LastLaunchedVersion < "2020-03-14")
        {
            for (auto const& info : Build::GetFlagInfos())
                if (!info.VisibleByDefault)
                    m_config.HideFlagsMask |= (uint32_t)info.Flag;
        }
        if (m_config.LastLaunchedVersion < "2023-07-19")
        {
            Web::Instance().ResetRenderCache();
        }
        if (m_config.LastLaunchedVersion < "2024-10-17")
        {
            for (auto& build : BuildStorage::Instance().GetBuilds())
            {
                if (auto link = ChatLink::Decode(build.GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                {
                    auto& data = std::get<ChatLink::BuildTemplate>(*link);
                    bool changed = false;
                    for (auto& weapon : data.Weapons)
                    {
                        switch (weapon)
                        {
                            case (GW2::Weapon)108: weapon = GW2::Weapon::Trident;    changed = true; break;
                            case (GW2::Weapon)52:  weapon = GW2::Weapon::Spear;      changed = true; break;
                            case (GW2::Weapon)88:  weapon = GW2::Weapon::HarpoonGun; changed = true; break;
                        }
                    }
                    if (changed)
                        build.SetLink(ChatLink::Encode(data));
                }
            }
        }

        m_config.LastLaunchedVersion = m_versionCurrent;
        m_configLoaded = true;
    });

    FILE* file;
    if (_wfopen_s(&file, GetConfigPath().c_str(), L"r"))
        return;
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
        return;
    }

    BuildStorage::Instance().LoadEnd();
    m_savedConfig = m_config;
}

void Handler::SaveConfig()
{
    // Safeguard to prevent empty config from being written if for whatever reason buildpad gets unloaded before it finished loading
    if (!m_configLoaded)
        return;

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
                return fmt::format("{}", fmt::join(values.begin(), values.end(), ","));
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

void Handler::Unload()
{
    if (!m_loaded)
        return;

    m_loaded = false;

    API::Instance().Unload(); // Must be unloaded before icons, as it relies on icons to know which textures are shared and shouldn't be unloaded yet

    static auto const unloadContainer = [this](auto& container)
    {
        for (auto&& [key, icon] : container)
            UnloadTexture(icon);
    };
    unloadContainer(GetIconContainer<Icons>());
    unloadContainer(GetIconContainer<GW2::Profession>());
    unloadContainer(GetIconContainer<GW2::Specialization>());
    unloadContainer(GetIconContainer<GW2::Slot>());
    unloadContainer(GetIconContainer<GW2::Weapon>());
    unloadContainer(GetIconContainer<Build::Flags>());

    SkillStorage::Instance().ClearProfessionSkills();

    while (!m_loadedTextures.empty())
        UnloadTexture(m_loadedTextures.front());
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
ImVec2 CELL_PADDING { };
float INDENT_SPACING { };
ImVec2 BUTTON_SIZE { 19, 19 };
ImVec4 BUTTON_COLOR { };
ImVec4 BUTTON_COLOR_ACTIVE { };
ImVec4 BUTTON_COLOR_HOVERED { };

// ReSharper disable CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore
#pragma warning(suppress: 4455) // I disagree with the standard, so sue me.
float operator"" px(unsigned long long int const value) { return (float)(signed long long int)value * UI_SCALE; }
// ReSharper restore CppUserDefinedLiteralSuffixDoesNotStartWithUnderscore

void Handler::BeginUI()
{
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts.back());
    UI_SCALE = GetUIScale();
    LINE_SIZE = ImGui::CalcTextSize("") * UI_SCALE;
    WINDOW_PADDING = ImGui::GetStyle().WindowPadding;
    FRAME_PADDING = ImGui::GetStyle().FramePadding;
    ITEM_SPACING = ImGui::GetStyle().ItemSpacing;
    ITEM_INNER_SPACING = ImGui::GetStyle().ItemInnerSpacing;
    CELL_PADDING = ImGui::GetStyle().CellPadding;
    INDENT_SPACING = ImGui::GetStyle().IndentSpacing;
    BUTTON_SIZE.x = BUTTON_SIZE.y = FRAME_PADDING.y + LINE_SIZE.y + FRAME_PADDING.y;
    BUTTON_COLOR = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    BUTTON_COLOR_ACTIVE = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
    BUTTON_COLOR_HOVERED = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, { 0.5f, 0.5f });
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, { 0, 0, 0, ImGui::GetStyleColorVec4(ImGuiCol_ModalWindowDimBg).w });
}

void Handler::EndUI()
{
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PopFont();
}

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
        m_editedBuilds.empty() &&
        !m_arcdpsMigrationShown &&
        !m_arcdpsGearShown &&
        !m_detachSettings &&
        !m_versionHistoryShown &&
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

    BeginUI();

    #pragma region Debug Window
#ifdef _DEBUG
    ImGui::Begin("[DEBUG]", &m_shown, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowFontScale(UI_SCALE);
    int prof = (int)storage.GetCurrentProfession();
    ImGui::SliderInt("[TEST PROF]", &prof, 0, 9);
    storage.SetCurrentProfession((GW2::Profession)prof);
    UpdateOptions();
    ImGui::End();
#endif
    #pragma endregion

    if (m_shown)
        RenderMainWindow(delta);

    if (!m_editedBuilds.empty())
        RenderBuildEditors();

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
        ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_Appearing, { 0.5f, 0.5f });
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
        RenderSettings(RenderSettingsType::Detached);
        ImGui::End();
    }

    if (m_keyBindEditRequested)
    {
        m_keyBindEditRequested = false;
        ImGui::OpenPopup("BuildPad##KeyBindEditor");
    }
    RenderKeyBindEditor();

    EndUI();
}

void Handler::UpdateOptions()
{
    if (!m_loaded)
    {
        ImGui::TextUnformatted("Loading data from the web, please wait...");
        return;
    }

    if (TextureData const& icon = GetIcon(Icons::BuildPad);
        ImGui::ImageButtonWithText(icon.Texture, "Toggle BuildPad ", { ImGui::GetContentRegionAvailWidth(), 0 }, icon.TrimmedSize() * 0.75f, icon.GetUV0(), icon.GetUV1(), ImDrawCornerFlags_All, 3))
        m_shown ^= true;

    BeginUI();

    ImGui::Spacing();

    if (ImGui::FixedCollapsingHeader("Settings"))
        RenderSettings(RenderSettingsType::ArcDPS);

    EndUI();
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
        auto flags = (ImGuiWindowFlags_)(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing);
        if (m_config.LockWindowPosition)
        {
            ImGui::SetNextWindowPos({ (float)m_config.WindowPositionX, (float)m_config.WindowPositionY }, ImGuiCond_Always);
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoMove);
        }
        else
            ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_FirstUseEver, { 0.5f, 0.5f });
        if (m_config.LockWindowSize)
        {
            ImGui::SetNextWindowSize({ (float)m_config.WindowSizeW, (float)m_config.WindowSizeH }, ImGuiCond_Always);
            flags = (ImGuiWindowFlags_)(flags | ImGuiWindowFlags_NoResize);
        }
        else
            ImGui::SetNextWindowSize({ 300px, 400px }, ImGuiCond_FirstUseEver);
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
                                      std::clamp<float>(ImGui::GetWindowPos().y, 0, std::max<float>(0, ImGui::GetIO().DisplaySize.y - ImGui::GetWindowSize().y)) }, ImGuiCond_Always);
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
        ImGui::SetNextWindowSize({ 400px, 0 }, ImGuiCond_Appearing);
        ImGui::SetNextWindowSizeConstraints({ 400px, 0 }, ImGui::GetIO().DisplaySize);
        if (ImGui::BeginPopup("##BuildPadContextMenu", ImGuiWindowFlags_NoMove))
        {
            ImGui::SetWindowFontScale(UI_SCALE);
            RenderSettings(RenderSettingsType::Menu);
            ImGui::EndPopup();
        }
    }
    #pragma endregion

    if (SkillStorage::Instance().GetLoadingState() == SkillStorage::LoadingState::Failed)
    {
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
        ImGui::TextWrapped("Failed to load skill data from the web. Guild Wars 2 API or BuildPad website might be unavailable, or BuildPad might need to be updated.\n\nSome functionality is disabled.\n\nRestart Guild Wars 2 to try again.");
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        ImGui::NewLine();

        ImVec2 const offset { 2px, 2px };
        ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetItemRectMin() - offset, ImVec2 { ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvailWidth(), ImGui::GetItemRectMax().y } +offset, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildRounding);
    }

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
        ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetItemRectMin() - offset, ImVec2 { ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvailWidth(), ImGui::GetItemRectMax().y } +offset, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildRounding);
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
            ImGui::GetCurrentWindow()->DrawList->AddRect(ImGui::GetItemRectMin() - offset, ImVec2 { ImGui::GetItemRectMin().x + ImGui::GetContentRegionAvailWidth(), ImGui::GetItemRectMax().y } + offset, ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildRounding);
        }
    }
    #pragma endregion

    if (m_config.LastViewedVersionHistory < m_versionCurrent)
    {
        ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "BuildPad was updated to version %s!", m_versionCurrent.c_str());
        ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "See what changed since %s:", m_config.LastViewedVersionHistory.c_str());
        if (ImGui::Button("View Version History"))
        {
            m_versionHistoryShown = true;
            m_config.LastViewedVersionHistory = m_versionCurrent;
        }
        ImGui::SameLine();
        if (ImGui::Button("Dismiss"))
            m_config.LastViewedVersionHistory = m_versionCurrent;
        ImGui::Separator();
        ImGui::NewLine();
    }

    auto const renderSettingsButton = [this](float spacing)
    {
        ImGui::SameLine(0, spacing);
        bool on = ImGui::IsPopupOpen("##BuildPadContextMenu");
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
            if (ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - WINDOW_PADDING.x - (storage.IsFilteringName() || m_config.ShowSettingsButton ? ITEM_INNER_SPACING.x + BUTTON_SIZE.x : 0)),
                ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
                ImGui::InputTextWithHint("##FilterName", "Search", buffer.data(), buffer.size()))
                storage.SetNameFilter(buffer.data());
            ImGui::PopStyleColor();
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
                    ImGui::TextColored(ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f), "Show builds flagged as");
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
    ImGui::BeginChild("Builds", { 0, 0 }, true, ImGuiWindowFlags_NoMove);
    ImGui::SetWindowFontScale(UI_SCALE);

    static Build::id_t newestAddedBuild { };
    static Build::id_t removeBuildPrompt { };
    static Build::id_t draggedBuild { };
    static GW2::Profession draggedBuildProfession { };
    static std::optional<Build::id_t> draggedOverBuild { };
    static bool draggedAfterBuild = false;
    bool firstVisibleBuild = true;

    #pragma region Build List
    auto const buildListPass = [&](auto&& builds, std::optional<GW2::Profession> const profession = std::nullopt, std::function<bool(Build const&)> const& filter = nullptr)
    {
        bool firstSortedBuild = true;
        for (Build& build : builds)
        {
            if (profession.has_value() && build.GetParsedProfession() != profession)
                continue;

            if (filter && !filter(build))
                continue;

            if (!storage.IsEditingBuild(build) && !storage.IsBuildMatchingFilter(build, m_config.SimpleFlagsFilter))
                continue;

            BeginRenderBuildList(build.GetParsedProfession(), firstVisibleBuild, firstSortedBuild, storage.IsFilteringProfession(), storage.IsEditingBuild(build) ? storage.GetEditedBuild().GetParsedProfession() : build.GetParsedProfession(), draggedBuild && draggedBuild != build.GetID() && draggedOverBuild ? 0.4f : 1.0f);

            ImVec2 const min = ImGui::GetWindowPos() + ImGui::GetCursorPos() - ImVec2 { ImGui::GetScrollX(), ImGui::GetScrollY() };
            ImVec2 const max = min +  ImVec2 { ImGui::GetContentRegionAvailWidth(), BUTTON_SIZE.y };
            bool hovered = ImGui::IsMouseHoveringRect(min, max) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

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
                    TextureData const& icon = edited.GetParsedSpecialization() != GW2::Specialization::None ? GetIcon(edited.GetParsedSpecialization()) : GetIcon(edited.GetParsedProfession());

                    auto buffer = util::to_buffer(edited.GetName());
                    if (ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button)),
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button)),
                        ImGui::ImageButtonWithText(icon.Texture, "", BUTTON_SIZE, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, ImDrawCornerFlags_Left),
                        hovered = ImGui::IsItemHovered(),
                        ImGui::PopStyleColor(2),
                        ImGui::SameLine(0, 2px),
                        x = ImGui::GetCursorPosX(),
                        ImGui::PushItemWidth(-1 - BUTTON_SIZE.x - 2px - BUTTON_SIZE.x - 2px),
                        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { FRAME_PADDING.x, (BUTTON_SIZE.y - LINE_SIZE.y) / 2 }),
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0),
                        ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f)),
                        [&] { if (m_focusNewBuild == edited.GetID()) { ImGui::SetKeyboardFocusHere(); m_focusNewBuild = { }; } }();
                        ImGui::InputTextWithHint("##EditName", "Build Name", buffer.data(), buffer.size()))
                        edited.SetName(buffer.data());
                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar(2);
                    ImGui::PopItemWidth();
                }
                #pragma endregion
                #pragma region Build Edit Accept Button
                if (ImGui::SameLine(0, 2px);
                    ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), fmt::format("##AcceptBuildEdit:{0}", build.GetID()).c_str(), BUTTON_SIZE, ImDrawCornerFlags_None))
                {
                    if (auto const& context = util::find_if(m_editedBuilds, util::member_equals(&BuildEditContext::ID, edited.GetID())); context && context->BuildStorageEditedBuild)
                        CloseBuildEditor(edited);

                    newestAddedBuild = { };
                    postAction = [&storage] { storage.AcceptBuildEdit(); };
                }
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip("Accept Changes");
                #pragma endregion
                #pragma region Build Edit Cancel Button
                if (ImGui::SameLine(0, 2px);
                    ImGui::ImageButtonWithText(GetIcon(Icons::CancelBuildEdit), fmt::format("##CancelBuildEdit:{0}", build.GetID()).c_str(), BUTTON_SIZE, ImDrawCornerFlags_Right))
                {
                    if (auto const& context = util::find_if(m_editedBuilds, util::member_equals(&BuildEditContext::ID, edited.GetID())); context && context->BuildStorageEditedBuild)
                        CloseBuildEditor(edited);

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

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, *alpha));
                }
                #pragma endregion

                auto const& parsed = edited.GetParsedInfo();
                bool canEdit = (!alpha || alpha >= 1.0f) && (edited.GetLink().empty() || parsed.SkillsLand && parsed.SkillsWater && parsed.TraitLines) && SkillStorage::Instance().AreSkillsLoaded();
                #pragma region Build Link Input
                {
                    auto buffer = util::to_buffer(edited.GetLink());
                    if (ImGui::SameLine(x, 0),
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - FRAME_PADDING.y),
                        ImGui::PushItemWidth(canEdit ? -1 - (2px + BUTTON_SIZE.x + 2px + BUTTON_SIZE.x) : -1),
                        ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
                        ImGui::InputTextWithHint("##EditLink", "Chat Link", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
                        edited.SetLink(buffer.data());
                    ImGui::PopStyleColor();
                    ImGui::PopItemWidth();
                }
                #pragma endregion
                #pragma region Build Link Edit Button
                if (canEdit)
                {
                    auto itr = util::find_if(m_editedBuilds, util::member_equals(&BuildEditContext::ID, edited.GetID()));
                    bool isEdited = itr && itr->BuildStorageEditedBuild;
                    if (ImGui::SameLine(0, 2px),
                        ImGui::PushStyleColor(ImGuiCol_Button, isEdited ? BUTTON_COLOR_HOVERED : BUTTON_COLOR),
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, BUTTON_COLOR_ACTIVE),
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, BUTTON_COLOR_HOVERED);
                        ImGui::ButtonEx("Edit", { BUTTON_SIZE.x + 2px + BUTTON_SIZE.x, 0 }))
                    {
                        if (isEdited)
                            CloseBuildEditor(edited);
                        else
                            OpenBuildEditor(edited, true);
                    }
                    ImGui::PopStyleColor(3);
                }
                #pragma endregion

                #pragma region Build Link Fade Text
                if (alpha)
                {
                    ImGui::PopStyleColor();
                    ImVec2 const cursor = ImGui::GetCursorScreenPos();
                    ImVec2 const size = ImGui::CalcTextSize("Pasted from clipboard");
                    float const space = ImGui::GetContentRegionAvailWidth();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 1 - *alpha));
                    ImGui::SetCursorScreenPos({ cursor.x + BUTTON_SIZE.x + (space - size.x - BUTTON_SIZE.x) / 2, cursor.y - ITEM_SPACING.y - BUTTON_SIZE.y + (BUTTON_SIZE.y - size.y) / 2 });
                    ImGui::PushClipRect({ cursor.x + BUTTON_SIZE.x, ImGui::GetCursorScreenPos().y }, { cursor.x + space, ImGui::GetCursorScreenPos().y + BUTTON_SIZE.y }, true);
                    ImGui::Text("Pasted from clipboard");
                    ImGui::PopClipRect();
                    ImGui::SetCursorScreenPos(cursor);
                    ImGui::PopStyleColor();
                }
                #pragma endregion

                #pragma region Build Secondary Link Input
                if (parsed.NeedsSecondaryLink)
                {
                    auto buffer = util::to_buffer(edited.GetSecondaryLink());
                    if (ImGui::SetCursorPosX(ImGui::GetCursorPosX() - (ImGui::CalcTextSize("[*]").x - BUTTON_SIZE.x)),
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + FRAME_PADDING.y),
                        ImGui::Text("[*]"),
                        ImGui::SameLine(x, 0),
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - FRAME_PADDING.y),
                        ImGui::PushItemWidth(-1),
                        ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
                        ImGui::InputTextWithHint("##EditSecondaryLink", parsed.SecondaryLinkIsTraits ? "ArcDPS Traits Link" : "ArcDPS Skills Link", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
                        edited.SetSecondaryLink(buffer.data());
                    ImGui::PopStyleColor();
                    ImGui::PopItemWidth();
                }
                #pragma endregion

                if (!parsed.Error.empty())
                {
                    ImGui::NewLine();
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
                    ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "%s", parsed.Error.c_str());
                }

                if (!parsed.Info.empty())
                {
                    ImGui::NewLine();
                    ImGui::SameLine(x, FRAME_PADDING.x);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
                    ImGui::TextColored(ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f), "%s", parsed.Info.c_str());
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
                    if (edited.HasFlagIcon(info.Flag))
                    {
                        ImGui::StoreCursor();
                        ImGui::ButtonEx("", BUTTON_SIZE, ImGuiButtonFlags_Disabled);
                        ImGui::RestoreCursor();
                    }
                    if (TextureData const& icon = GetIcon(info.Flag);
                        ImGui::CheckboxImage(icon.Texture, fmt::format("##{}", (int)info.Flag).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                        edited.ToggleFlag(info.Flag, on);
                    if (edited.HasFlagIcon(info.Flag) && !edited.HasFlag(info.Flag))
                        edited.ToggleFlagIcon(info.Flag, false);
                    if (ImGui::IsItemClicked(1) && on)
                        edited.ToggleFlagIcon(info.Flag, !edited.HasFlagIcon(info.Flag));
                    if (ImGui::IsItemHovered())
                        ImGui::TooltipWithHeader(info.Name.c_str(), on ? "Right click to {} this icon on the build" : nullptr, edited.HasFlagIcon(info.Flag) ? "hide" : "show");
                }
                #pragma endregion
            }
            #pragma endregion
            #pragma region Build Presenter
            else if (storage.IsBuildMatchingFilter(build, m_config.SimpleFlagsFilter))
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

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, *alpha));
                }
                #pragma endregion

                #pragma region Build Button
                ImGui::BeginGroup();
                ImGui::PushItemWidth(hovered ? ImGui::GetContentRegionAvailWidth() - 1 - BUTTON_SIZE.x : -1);
                std::string keyBindText;
                if (KeyBind const& keyBind = build.GetKeyBind(); keyBind && storage.GetCurrentProfession() != GW2::Profession::None && build.GetParsedProfession() == storage.GetCurrentProfession())
                {
                    keyBindText = keyBind.ToString().value_or("");
                    if (util::ends_with(keyBindText, " >GW2"))
                        keyBindText = keyBindText.substr(0, keyBindText.length() - " >GW2"sv.length());
                }

                std::vector<TextureData const*> additionalIcons;
                if (build.GetFlagIcons() != Build::Flags::None)
                    for (auto const& info : Build::GetFlagInfos())
                        if (build.HasFlagIcon(info.Flag))
                            additionalIcons.emplace_back(&GetIcon(info.Flag));

                if (TextureData const& icon = build.GetParsedSpecialization() != GW2::Specialization::None ? GetIcon(build.GetParsedSpecialization()) : GetIcon(build.GetParsedProfession());
                    ImGui::ImageButtonWithText(icon.Texture, fmt::format(build.IsSaveNeeded() ? "{}*##{}" : "{}##{}", build.GetName(), build.GetID()).c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, hovered ? ImDrawCornerFlags_Left : ImDrawCornerFlags_All, 0, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, false, keyBindText.c_str(), additionalIcons)
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
                    if (ImVec2 const drag = ImGui::GetMouseDragDelta(); ImGui::IsItemActive() && (draggedBuild || std::abs(drag.y) > 5px))
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
                        auto task = [this, build]
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
                                        if (auto const& info = API::Specialization::Get((uint32_t)line.Specialization))
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
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[0], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[1], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[2], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[3], parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[4], parsed.RevenantLegendsLand[0])).Name);
                                if (parsed.SkillsWater)
                                    stream << fmt::format("\nAquatic Skills:\n- {}\n- {}\n- {}\n- {}\n- {}",
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[0], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[1], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[2], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[3], parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[4], parsed.RevenantLegendsWater[0])).Name);
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
                                        API::Skill::Get(SkillStorage::Instance().GetRevenantLegendSwapSkill(parsed.RevenantLegendsLand[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().GetRevenantLegendSwapSkill(parsed.RevenantLegendsLand[1])).Name);
                                    stream << fmt::format("\nAquatic Legends:\n- {}\n- {}",
                                        API::Skill::Get(SkillStorage::Instance().GetRevenantLegendSwapSkill(parsed.RevenantLegendsWater[0])).Name,
                                        API::Skill::Get(SkillStorage::Instance().GetRevenantLegendSwapSkill(parsed.RevenantLegendsWater[1])).Name);
                                    break;
                                default:
                                    break;
                            }
#if _WIN32
                            ImGui::SetClipboardText(util::replace_all(stream.str(), "\n", "\r\n").c_str());
#else
                            ImGui::SetClipboardText(stream.str().c_str());
#endif
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
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[0], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[1], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[2], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[3], parsed.RevenantLegendsLand[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsLand)[4], parsed.RevenantLegendsLand[0])).ToChatLink()).c_str());
                    }
                    if (ImGui::Selectable("Copy Aquatic Skills as Chat Links", false, !build.GetParsedInfo().SkillsWater ? ImGuiSelectableFlags_Disabled : 0))
                    {
                        auto const& parsed = build.GetParsedInfo();
                        if (parsed.SkillsWater)
                            ImGui::SetClipboardText(fmt::format("{}{}{}{}{}",
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[0], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[1], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[2], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[3], parsed.RevenantLegendsWater[0])).ToChatLink(),
                                API::Skill::Get(SkillStorage::Instance().FromPalette((*parsed.SkillsWater)[4], parsed.RevenantLegendsWater[0])).ToChatLink()).c_str());
                    }
                    if (ImGui::Selectable(working ? "Please wait..." : "Copy Traits as Chat Links", false, !build.GetParsedInfo().TraitLines || working ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_DontClosePopups))
                    {
                        auto task = [this, build]
                        {
                            if (!API::Instance().PreloadAllBuildInfos(build))
                                return RepeatCurrentMainThreadTask(), false;

                            auto const& parsed = build.GetParsedInfo();
                            std::ostringstream stream;
                            for (auto const& line : *parsed.TraitLines)
                                if (line.Specialization != GW2::Specialization::None)
                                    if (auto const& info = API::Specialization::Get((uint32_t)line.Specialization))
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
                    if (ImGui::Selectable(working ? "Please wait..." : "Copy Traits as Numbers", false, !build.GetParsedInfo().TraitLines || working ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_DontClosePopups))
                    {
                        auto task = [this, build]
                        {
                            if (!API::Instance().PreloadAllBuildInfos(build))
                                return RepeatCurrentMainThreadTask(), false;

                            auto const& parsed = build.GetParsedInfo();
                            std::ostringstream stream;
                            for (auto const& line : *parsed.TraitLines)
                                if (line.Specialization != GW2::Specialization::None)
                                    if (auto const& info = API::Specialization::Get((uint32_t)line.Specialization))
                                        stream << fmt::format(" {} {}{}{}",
                                            info.Name,
                                            (uint32_t)line.Traits[0],
                                            (uint32_t)line.Traits[1],
                                            (uint32_t)line.Traits[2]);

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

                    ImGui::Separator();

                    if (ImGui::Selectable("Edit", false, !(build.GetLink().empty() || build.GetParsedInfo().SkillsLand && build.GetParsedInfo().SkillsWater && build.GetParsedInfo().TraitLines) || !SkillStorage::Instance().AreSkillsLoaded() ? ImGuiSelectableFlags_Disabled : 0))
                        OpenBuildEditor(build, false);

                    ImGui::Separator();

                    if (ImGui::Selectable("Delete"))
                        removeBuildPrompt = build.GetID();

                    ImGui::EndPopup();
                }
                #pragma endregion

                #pragma region Build Edit Button
                if (hovered)
                {
                    if (ImGui::SameLine(0, 0);
                        ImGui::ImageButtonWithText(GetIcon(Icons::EditBuild), fmt::format("##EditBuild:{0}", build.GetID()).c_str(), BUTTON_SIZE, ImDrawCornerFlags_Right))
                    {
                        if (storage.IsEditingBuild())
                            if (auto const& context = util::find_if(m_editedBuilds, util::member_equals(&BuildEditContext::ID, storage.GetEditedBuild().GetID())); context && context->BuildStorageEditedBuild)
                                CloseBuildEditor(storage.GetEditedBuild());

                        postAction = [&storage, editedBuild = build.GetID()]
                        {
                            if (Build* build = storage.FindBuild(editedBuild))
                                storage.EditBuild(*build);
                        };
                    }
                }
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
                    ImVec2 const cursor = ImGui::GetCursorScreenPos();
                    ImVec2 const size = ImGui::CalcTextSize("Copied to clipboard");
                    float const space = ImGui::GetContentRegionAvailWidth();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 1 - *alpha));
                    ImGui::SetCursorScreenPos({ cursor.x + (space - size.x) / 2, cursor.y - ITEM_SPACING.y - BUTTON_SIZE.y + (BUTTON_SIZE.y - size.y) / 2 });
                    ImGui::PushClipRect({ cursor.x, ImGui::GetCursorScreenPos().y }, { cursor.x + space, ImGui::GetCursorScreenPos().y + BUTTON_SIZE.y }, true);
                    ImGui::Text("Copied to clipboard");
                    ImGui::PopClipRect();
                    ImGui::SetCursorScreenPos(cursor);
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
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f));
        if (TextureData const& icon = GetIcon(Icons::AddBuild);
            ImGui::CheckboxImage(icon.Texture, "Add Build", &on, icon.GetUV0(), icon.GetUV1(), 0, 0, 0, 0.5f, 1.0f, 0.75f))
        {
            if (storage.IsEditingBuild())
                if (auto const& context = util::find_if(m_editedBuilds, util::member_equals(&BuildEditContext::ID, storage.GetEditedBuild().GetID())); context && context->BuildStorageEditedBuild)
                    CloseBuildEditor(storage.GetEditedBuild());

            Build& build = storage.AddBuild();
            newestAddedBuild = build.GetID();
            m_focusNewBuild = build.GetID();
            storage.EditBuild(build);
            if (char const* rawClipboard = ImGui::GetClipboardText())
            {
                // Sanitize clipboard data. Can't do it right away, because clipboard contains UTF-8 text,
                // and the bytes we seek to expurge may very well be parts of other unicode characters.
                // UTF-8 clipboard -> ImWchar string
                std::basic_string<ImWchar> wide;
                wide.reserve(strlen(rawClipboard));
                uint32_t c = 1;
                for (const char* s = rawClipboard; *s && c;)
                {
                    s += ImTextCharFromUtf8(&c, s, nullptr);
                    if (c >= 0x20 && c != IM_UNICODE_CODEPOINT_INVALID) // Only accept printable characters, strip newlines which will corrupt the save file
                        wide.push_back((ImWchar)c);
                }
                // ImWchar string -> UTF-8 buffer
                auto utf8 = util::to_buffer<1001>({});
                std::string_view const clipboard { utf8.data(), (size_t)ImTextStrToUtf8(utf8.data(), (int)utf8.size(), wide.c_str(), nullptr) };

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
                if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() + WINDOW_PADDING.x - 50px - ITEM_SPACING.x - 50px); ImGui::Button("Yes", { 50px, 0 }))
                {
                    postAction = [this, &storage, removeBuild = removeBuildPrompt]
                    {
                        if (Build* build = storage.FindBuild(removeBuild))
                        {
                            CloseBuildEditor(*build);
                            storage.RemoveBuild(*build);
                        }
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

    #pragma region Snow
    static bool snow = [] { time_t t = time(nullptr); tm tm { }; localtime_s(&tm, &t); return tm.tm_mon >= 11 && tm.tm_mday >= 22 || tm.tm_mon <= 0 && tm.tm_mday <= 4; }();
    if (snow && m_config.Snow && delta < 1s)
    {
        static std::mt19937 mt { std::random_device{}() };
        static auto const random = [](uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max()) { return std::uniform_int_distribution<uint32_t> { min, max }(mt); };
        struct Snowflake
        {
            uint32_t Lifetime = 0;
            uint32_t Seed;
            ImVec2 Position;
        };
        static std::array<Snowflake, 500> snowflakes { };
        static ImU32 snowflakeColor = ImGui::ColorConvertFloat4ToU32({ 1.0f, 1.0f, 1.0f, 1.0f });
        static Time nextSnowflakeCooldown { };
        float const perSecond = (float)delta.count() / 1000ms .count();

        bool spawned = false;
        if (nextSnowflakeCooldown > delta)
            nextSnowflakeCooldown -= delta;

        auto* window = ImGui::GetCurrentWindow();
        static std::optional<ImVec2> prevWindowPos;
        static std::optional<ImVec2> prevWindowSize;
        bool resizing = prevWindowSize && (prevWindowSize->x != window->Size.x || prevWindowSize->y != window->Size.y);
        static ImVec2 drag { };
        if (prevWindowPos && (prevWindowPos->x != window->Pos.x || prevWindowPos->y != window->Pos.y))
            drag += (window->Pos - *prevWindowPos) / 20;
        drag *= std::max<float>(0.0f, 1.0f - 5.0f * perSecond);

        for (auto& snowflake : snowflakes)
        {
            if (!snowflake.Lifetime)
            {
                if (!spawned && nextSnowflakeCooldown <= delta)
                {
                    spawned = true;
                    nextSnowflakeCooldown = Time(100 + random(0, 500));
                    snowflake.Seed = random();
                    snowflake.Position.x = window->Pos.x + (float)random(0, (uint32_t)window->Size.x);
                    snowflake.Position.y = window->Pos.y;
                }
                else
                    continue;
            }

            float const phase = 3.141592653589f * (float)snowflake.Lifetime / 1000ms .count();
            snowflake.Lifetime += (uint32_t)delta.count();
            snowflake.Position.x += drag.x * 0.1f * (float)(5 + snowflake.Seed % 5) + UI_SCALE * (float)( 5 + snowflake.Seed % 30) * perSecond * std::sin(phase) * (snowflake.Seed % 2 ? 1.0f : -1.0f);
            snowflake.Position.y += drag.y * 0.1f * (float)(5 + snowflake.Seed % 5) + UI_SCALE * (float)(20 + snowflake.Seed % 10) * perSecond;

            float size;
            switch (snowflake.Seed % 10) { case 0: case 1: case 2: case 3: case 4: case 5: size = 1.0f; break; case 6: case 7: case 8: size = 1.5f; break; case 9: size = 2.0f; break; }
            size *= UI_SCALE;
            window->DrawList->AddRectFilled(snowflake.Position - ImVec2 { size, size }, snowflake.Position + ImVec2 { size, size }, snowflakeColor, size <= 1.5f ? 0 : size * 2);

            if (snowflake.Position.x < window->Pos.x)
            {
                if (resizing)
                    snowflake.Lifetime = 0;
                else
                    snowflake.Position.x += window->Size.x;
            }
            if (snowflake.Position.x > window->Pos.x + window->Size.x)
            {
                if (resizing)
                    snowflake.Lifetime = 0;
                else
                    snowflake.Position.x -= window->Size.x;
            }
            if (snowflake.Position.y < window->Pos.y - 50 ||
                snowflake.Position.y > window->Pos.y + window->Size.y + 50)
                snowflake.Lifetime = 0;
        }
        prevWindowPos = window->Pos;
        prevWindowSize = window->Size;
    }
    #pragma endregion

    if (postAction)
        postAction();

    ImGui::End();
}

void Handler::RenderSettings(RenderSettingsType const type)
{
    /*
    if (type == RenderSettingsType::Menu)
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
    */

    struct CategoryInfo
    {
        int Index;
        std::string Name;
        std::function<bool(RenderSettingsType)> DisplayCondition;
    };
    static int categoryGeneral, categoryBuildList, categoryFlags, categoryWindow, categoryImport;
    static auto const categories = [&]
    {
        std::vector<CategoryInfo> result;
        auto AddCategory = [&](std::string name, int& categoryVariable, decltype(CategoryInfo::DisplayCondition) displayCondition = nullptr)
        {
            categoryVariable = (int)result.size();
            result.emplace_back(categoryVariable, std::move(name), std::move(displayCondition));
        };
        int _;
        AddCategory("General", categoryGeneral);
        AddCategory("Build List", categoryBuildList);
        AddCategory("Flags", categoryFlags);
        AddCategory("Window", categoryWindow);
        AddCategory("", _, [this](RenderSettingsType type) { return m_arcdpsMigrationAvailable || m_arcdpsGearAvailable; });
        AddCategory("ArcDPS Import", categoryImport, [this](RenderSettingsType type) { return m_arcdpsMigrationAvailable || m_arcdpsGearAvailable; });
        return result;
    }();
    static int currentCategory = categoryGeneral;

    float categoryColumnWidth = 0.0f;
    if (ImGui::BeginTable("BuildPadSettings", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_NoSavedSettings))
    {
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextColumn();
        categoryColumnWidth = ImGui::GetContentRegionAvailWidth();
        ImGui::BeginGroup();
        for (auto const& category : categories)
        {
            if (!category.DisplayCondition || category.DisplayCondition(type))
            {
                if (category.Name.empty())
                    ImGui::Spacing();
                else if (ImGui::Selectable(category.Name.c_str(), currentCategory == category.Index, ImGuiSelectableFlags_SelectOnClick | ImGuiSelectableFlags_DontClosePopups))
                    currentCategory = category.Index;
            }
        }
        ImGui::Spacing();
        ImGui::InvisibleButton("BuildPadSettingsSaveLoadPlaceHolder", { FRAME_PADDING.x + ImGui::CalcTextSize("Save").x + FRAME_PADDING.x + CELL_PADDING.x + 1 + CELL_PADDING.x + FRAME_PADDING.x + ImGui::CalcTextSize("Load").x + FRAME_PADDING.x, FRAME_PADDING.y + ImGui::CalcTextSize(" ").y + FRAME_PADDING.y }, ImGuiButtonFlags_Disabled);
        ImGui::EndGroup();

        ImGui::TableNextColumn();
        ImGui::BeginGroup();

        if (currentCategory == categoryImport)
        {
            ImGui::Text("ArcDPS Templates:");
            if (m_arcdpsMigrationAvailable && ImGui::Button("Migrate Traits/Skills Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("Migrate Traits/Skills Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
                m_arcdpsMigrationShown = true;
            if (m_arcdpsGearAvailable && ImGui::Button("View Equipment/Legendary Templates", { std::max<float>(FRAME_PADDING.x + ImGui::CalcTextSize("View Equipment/Legendary Templates").x + FRAME_PADDING.x, ImGui::GetContentRegionAvailWidth()), 0 }))
                m_arcdpsGearShown = true;
        }

        if (currentCategory == categoryGeneral)
        {
            ImGui::Text("Toggle BuildPad Keybind:");
            if (ImGui::Button(fmt::format("{}##KeyBindToggleBuilds", m_config.KeyBindToggleBuilds).c_str(), { -1, 0 }))
                EditKeyBind(m_keyBindToggleBuilds, [this](KeyBind const& keyBind) { m_config.KeyBindToggleBuilds = (m_keyBindToggleBuilds = keyBind).ToString().value_or(""); });

            ImGui::Spacing();
            ImGui::PushItemWidth(LINE_SIZE.y * 7);
            if (int lang = (int)util::distance_if(API::GetLanguageInfos(), util::member_equals(&API::LanguageInfo::Tag, API::Instance().GetLanguage()));
                ImGui::Combo("API Language", &lang, [](void* data, int const index, char const** text) { return *text = API::GetLanguageInfos()[index].Name.c_str(), true; }, nullptr, (int)API::GetLanguageInfos().size()))
            {
                auto itr = API::GetLanguageInfos().begin();
                std::advance(itr, lang);
                API::Instance().SetLanguage(m_config.APILocale = itr->Tag);
            }
            ImGui::PopItemWidth();
            if (ImGui::IsItemHovered())
                ImGui::Tooltip("Some characters might require a custom font to display correctly");

            ImGui::Spacing();
            ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "Current Version: ");
            ImGui::SameLine(0, 0);
            ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 1.0f }, "%s", m_versionCurrent.c_str());

            if (ImGui::Button("Version History"))
                m_versionHistoryShown ^= true;

            ImGui::Spacing();
            if (ImGui::Button("About BuildPad"))
                m_aboutShown ^= true;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 1, 1 });

        if (currentCategory == categoryBuildList)
        {
            static bool snow = [] { time_t t = time(nullptr); tm tm { }; localtime_s(&tm, &t); return tm.tm_mon >= 11 && tm.tm_mday >= 22 || tm.tm_mon <= 0 && tm.tm_mday <= 4; }();
            if (snow) ImGui::Checkbox("Snow", &m_config.Snow);

            ImGui::Checkbox("Show Filter Panel", &m_config.ShowFilterPanel);
            if (m_config.ShowFilterPanel)
            {
                if (!m_config.ShowProfessionFilter && !m_config.ShowNameFilter && !m_config.ShowFlagsFilter)
                    m_config.ShowProfessionFilter = m_config.ShowNameFilter = m_config.ShowFlagsFilter = true;

                ImGui::Indent(INDENT_SPACING);
                ImGui::Checkbox("Show Profession Filter", &m_config.ShowProfessionFilter);
                ImGui::Checkbox("Show Search Box", &m_config.ShowNameFilter);
                ImGui::Checkbox("Show Flags", &m_config.ShowFlagsFilter);
                ImGui::Checkbox("Show Settings Button", &m_config.ShowSettingsButton);
                ImGui::Checkbox("Always Filter Flags Additively", &m_config.SimpleFlagsFilter);
                if (ImGui::IsItemHovered())
                    ImGui::TooltipWithHeader("Enable to revert to the old flag filtering behavior", "Leaving this disabled allows you to filter builds by intersection between separate groups of flags.\n\nExample:\nIf you're filtering on Raid, Power, Condition flags... \n\nWhen enabled (old behavior): shows builds that have Raid OR Power OR Condition flags (or any combination of those).\n\nWhen disabled (new behavior): shows builds that have both Raid AND either Power OR Condition flags (or both).");
                ImGui::Checkbox("Clear Filters When Hidden", &m_config.ClearFiltersOnWindowClose);
                if (ImGui::GetStyleColorVec4(ImGuiCol_WindowBg).w < 0.75f - 0.01f) // 0.75f in ImGuiExtensions.h, but there is some precision loss in U32<->float conversion
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

            ImGui::Spacing();
            ImGui::Checkbox("Use Profession Colors", &m_config.UseProfessionColors);
            if (m_config.UseProfessionColors)
            {
                ImGui::Indent(INDENT_SPACING);
                ImGui::Checkbox("Only When Not My Profession", &m_config.ProfessionColorsUnfilteredOnly);
                if (ImGui::IsItemHovered())
                    ImGui::Tooltip("Enable to use neutral colors when build list is filtered to only show the currently played profession");
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

            ImGui::Spacing();
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

            ImGui::Spacing();
            if (ImGui::Checkbox("Sort Builds Alphabetically", &m_config.SortBuildsAlphabetically) && m_config.SortBuildsAlphabetically)
                m_config.AllowBuildReordering = false;
            if (ImGui::Checkbox("Allow Build Dragging", &m_config.AllowBuildReordering) && m_config.AllowBuildReordering)
                m_config.SortBuildsAlphabetically = false;

            ImGui::Spacing();
            ImGui::PushItemWidth(LINE_SIZE.y * 5);
            ImGui::DragInt("Tooltip Delay", (int*)&m_config.TooltipDelay, 10, 0, 5000, "%.0f ms");
            ImGui::PopItemWidth();
        }

        if (currentCategory == categoryFlags)
        {
            ImGui::Text("Click to toggle filter flag visibility:");
            for (Build::FlagInfo const& info : Build::GetFlagInfos())
            {
                bool on = !(m_config.HideFlagsMask & (uint32_t)info.Flag);

                if (info.Separator)
                    ImGui::Separator();
                if (TextureData const& icon = GetIcon(info.Flag);
                    ImGui::CheckboxImage(icon.Texture, fmt::format("{}##{}", info.Name, (int)info.Flag).c_str(), &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
                {
                    if (on)
                        m_config.HideFlagsMask &= ~(uint32_t)info.Flag;
                    else
                        m_config.HideFlagsMask |= (uint32_t)info.Flag;
                }
            }
        }

        if (currentCategory == categoryWindow)
        {
            ImGui::PushItemWidth(LINE_SIZE.y * 5);
            if (ImGui::DragInt("UI Scale", (int*)&m_config.UIScale, 1, 20, 500, "%.0f%%"))
                m_config.UIScale = (m_config.UIScale + 5) / 10 * 10; // Some manual rounding to the nearest 10% step
            if (ImGui::IsItemHovered())
                ImGui::Tooltip("Try changing ArcDPS font size first before touching this");
            ImGui::PopItemWidth();

            ImGui::Spacing();
            ImGui::Checkbox("Hide Window Header", &m_config.HideWindowHeader);

            ImGui::Spacing();
            ImGui::Checkbox("Keep Window Within Boundaries", &m_config.KeepWindowInBounds);
            if (ImGui::IsItemHovered())
                ImGui::TooltipWithHeader("Prevent window from leaving the screen boundaries", "Enabling this might move the window undesirably if you change resolution!");
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
        }

        ImGui::PopStyleVar();
        ImGui::EndGroup();
        ImGui::EndTable();
    }

    ImGui::StoreCursor();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - std::max(8px, ITEM_SPACING.y + CELL_PADDING.y + FRAME_PADDING.y + ImGui::CalcTextSize(" ").y + FRAME_PADDING.y));
    if (ImGui::BeginTable("BuildPadSettingsSaveLoad", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoSavedSettings, { categoryColumnWidth, 0 }))
    {
        ImGui::TableNextColumn();
        bool const disabled = !IsSaveNeeded() && !BuildStorage::Instance().IsSaveNeeded();
        if (disabled)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4Grayscale(ImGuiCol_Button));
        if (ImGui::ButtonEx("Save", { -1, 0 }, disabled ? ImGuiButtonFlags_Disabled : 0))
            SaveConfig();
        if (ImGui::IsItemHovered())
            ImGui::TooltipWithHeader("Saves builds and settings", "Everything is saved automatically when you close Guild Wars 2.");
        if (disabled)
            ImGui::PopStyleColor();

        ImGui::TableNextColumn();
        if (ImGui::Button("Load", { -1, 0 }))
            LoadConfig();

        ImGui::EndTable();
    }
    ImGui::RestoreCursor();
}

void Handler::EditKeyBind(KeyBind const& keyBind, KeyBindCallback&& callback)
{
    m_keyBindEditRequested = true;
    m_keyBindEditing = true;
    m_keyBindEdited = keyBind;
    m_keyBindCallback = std::move(callback);
}

void Handler::RenderKeyBindEditor()
{
    if (!m_keyBindEditing)
        return;

    ImGui::SetNextWindowSizeConstraints({ WINDOW_PADDING.x + 60px + ITEM_SPACING.x + 60px + ITEM_SPACING.x + 60px + WINDOW_PADDING.x, 0 }, { 10000px, 10000px });
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

        ImVec4 active = ImGui::GetStyleColorVec4(ImGuiCol_Text);
        ImVec4 inactive = ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f);

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
            ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
            ImGui::InputTextEx("", "Key", buffer.data(), (int)buffer.size(), { -1, 0 }, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
        }

        ImGui::Checkbox("Pass to Guild Wars 2", &m_keyBindEdited.PassThrough);
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Enable to allow Guild Wars 2 to react to pressed keys");

        if (ImGui::Button("Clear", { 60px, 0 }))
            m_keyBindEdited = { };
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() + WINDOW_PADDING.x - 60px - ITEM_SPACING.x - 60px); ImGui::Button("OK", { 60px, 0 }))
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
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4ModColor(ImGuiCol_Button, colorMultiplier));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4ModColor(ImGuiCol_ButtonActive, colorMultiplier));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4ModColor(ImGuiCol_ButtonHovered, colorMultiplier));
    }
}

void Handler::EndRenderBuildList(bool singleProfession) const
{
    ImGui::PopStyleColor(3);

    if (m_config.SortBuildsByProfession && !singleProfession && m_config.SortBuildsByProfessionShowIcons && !m_config.SortBuildsByProfessionShowNames)
        ImGui::Unindent(LINE_SIZE.y + 2px);
}

template<typename DataType, typename InfoType, typename InfoSourceType, typename APIType>
void Handler::RenderPaletteBar(PaletteContext<DataType, InfoType, InfoSourceType, APIType> const& context) const
{
    if (context.Preload && context.EditTarget)
        context.Preload();

    ImVec2 typeSize = context.Water ? context.TypeSize : ImVec2 { };
    uint32_t buttonPerRow = context.BarButtonPerRow ? context.BarButtonPerRow : context.BarButtonCount;
    uint32_t buttonCount = std::min<uint32_t>(context.BarButtonCount, buttonPerRow);
    float const width = context.BarVertical
                        ? (typeSize.x + context.ButtonSize.x * buttonCount)
                        : (typeSize.x + context.ButtonSize.x * buttonCount + context.BarSpacing + typeSize.x + context.ButtonSize.x * buttonCount);
    for (uint8_t water = 0; water < (uint8_t)(context.Water ? 2 : 1); ++water)
    {
        if (context.Water)
        {
            if (context.BarVertical || !water)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - width) / 2)));

            float const offset = (context.ButtonSize.y - typeSize.y) / 2 + (context.EditTarget ? GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE : 0);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
            ImGui::Image(GetIcon(water ? Icons::WaterSkills : Icons::LandSkills).Texture, typeSize);
            ImGui::SameLine(0, 0);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - offset);
        }
        for (uint8_t index = 0; index < context.BarButtonCount; ++index)
        {
            std::string guid = fmt::format("##Palette{}{}{}", context.Context, water ? "Water" : "Land", index);
            ImVec2 cursor = ImGui::GetCursorScreenPos();
            bool hovered = ImGui::IsMouseHoveringRect(cursor, cursor + ImVec2 { context.ButtonSize.x, GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE + context.ButtonSize.y }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
            bool active = ImGui::IsPopupOpen(guid.c_str());
            float multiplier = context.EditTarget ? (active ? 0.25f : hovered ? 1.0f : 0.8f) : 1.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
            ImGui::BeginGroup();
            if (context.EditTarget)
            {
                auto const& icon = GetIcon(Icons::SelectionChevron);
                ImGui::NewLine();
                ImGui::SameLine(0, context.ButtonSize.x - icon.TrimmedSize().x * UI_SCALE);
                ImGui::GetCurrentWindow()->DC.CurrLineSize = { };
                ImGui::GetCurrentWindow()->DC.CurrLineTextBaseOffset = 0.0f;
                ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                color.x *= multiplier;
                color.y *= multiplier;
                color.z *= multiplier;
                if (!context.PaletteSourceLoaded)
                    color.x = color.y = color.z = 0.0f;
                ImGui::Image(icon.Texture, icon.TrimmedSize() * UI_SCALE, icon.GetUV0(), icon.GetUV1(), color);
            }
            {
                auto const& data = context.Getter(water, index);
                auto const& id = context.DataToAPITransform(data, water, index);
                auto const& api = APIType::Get(id);
                auto const& icon = id ? (context.IconGetter ? context.IconGetter(data, false) : (api.*context.APIIcon).Get()) : GetIcon(context.MissingAPIIcon);
                if (index < context.BarButtonCount / 2 ? context.DarkenFirstHalf : context.DarkenSecondHalf)
                    multiplier *= id ? 0.5f : 0.8f;
                ImVec4 color { 1.0f, 1.0f, 1.0f, 1.0f };
                color.x *= multiplier;
                color.y *= multiplier;
                color.z *= multiplier;
                ImGui::Image(icon.Texture, context.ButtonSize, icon.GetUV0(), icon.GetUV1(), color);
                if (ImGui::IsItemHovered())
                    ImGui::TooltipWithHeader(api ? (api.*context.APIName).c_str() : nullptr, context.ButtonTooltip.c_str());
            }
            ImGui::EndGroup();
            if (context.EditTarget && context.PaletteSourceLoaded)
            {
                if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1))
                    ImGui::OpenPopup(guid.c_str());
            }
            if (ImGui::IsPopupOpen(guid.c_str()))
            {
                std::vector<InfoType const*> palette;
                palette.reserve(context.PaletteSource.size());
                for (auto const& info : context.PaletteSource)
                    if (context.PaletteFilter(info, water, index))
                        palette.push_back(&info);

                if (context.PaletteSorter)
                    std::sort(palette.begin(), palette.end(), context.PaletteSorter);

                auto const window = ImGui::GetCurrentWindow()->RootWindow;
                ImVec2 pos
                {
                    cursor.x + context.PaletteSize.x / 2 - (WINDOW_PADDING.y + (context.PaletteSize.x + context.PaletteSpacing.x) * std::min<size_t>(palette.size(), context.PalettePerRow) - context.PaletteSpacing.x + WINDOW_PADDING.y) / 2,
                    cursor.y - (WINDOW_PADDING.y + (context.PaletteSize.x + context.PaletteSpacing.y) * ((palette.size() + context.PalettePerRow - 1) / context.PalettePerRow) - context.PaletteSpacing.y + WINDOW_PADDING.y)
                };
                if (pos.y < 0)
                {
                    float const y = cursor.y + GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE + context.ButtonSize.y;
                    pos.y = y + (cursor.y - pos.y) < ImGui::GetIO().DisplaySize.y ? y : 0;
                }
                ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
                if (ImGui::BeginPopup(guid.c_str(), ImGuiWindowFlags_NoMove))
                {
                    ImGui::SetWindowFontScale(UI_SCALE);
                    std::optional<DataType> selection;
                    size_t const paletteSlots = palette.size() < context.PalettePerRow
                                                ? palette.size()
                                                : (palette.size() + context.PalettePerRow - 1) / context.PalettePerRow * context.PalettePerRow;
                    for (size_t i = 0; i < paletteSlots; ++i)
                    {
                        size_t paletteIndex = i;
                        if (context.PaletteReverse && !palette.empty())
                            paletteIndex = (palette.size() - 1) / context.PalettePerRow * context.PalettePerRow - i / context.PalettePerRow * context.PalettePerRow + i % context.PalettePerRow;

                        if (paletteIndex < palette.size())
                        {
                            auto const& info = *palette[paletteIndex];
                            auto const& data = context.InfoToDataTransform(info);
                            auto const& id = context.DataToAPITransform(data, water, index);
                            auto const& api = APIType::Get(id);
                            auto const& icon = id ? (context.IconGetter ? context.IconGetter(data, true) : (api.*context.APIIcon).Get()) : GetIcon(context.MissingAPIIcon);
                            cursor = ImGui::GetCursorScreenPos();
                            hovered = ImGui::IsMouseHoveringRect(cursor, cursor + context.PaletteSize) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
                            active = id && context.PaletteActive(info, water, index);
                            bool const usable = !context.PaletteUsable || context.PaletteUsable(info, water, index);
                            multiplier = context.EditTarget ? (active ? (hovered ? 0.33f : 0.25f) : hovered ? 1.0f : 0.8f) : 1.0f;
                            ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                            color.x *= multiplier;
                            color.y *= multiplier;
                            color.z *= multiplier;
                            if (!usable)
                                color = { 0.5f, 0.0f, 0.0f, color.w };
                            ImGui::Image(icon.Texture, context.PaletteSize, icon.GetUV0(), icon.GetUV1(), color);
                            if (ImGui::IsItemHovered() && api)
                                ImGui::TooltipWithHeader((api.*context.APIName).c_str(), !usable ? "Unavailable" : nullptr);
                            if (ImGui::IsItemClicked() && usable)
                                selection = data;

                            ImVec2 const overlayOutset { (context.PaletteSize / 128px) * 16px };
                            if (!usable)
                            {
                                auto const& overlay = GetIcon(Icons::PaletteNotUnderwater);
                                ImGui::StoreCursor();
                                ImGui::SameLine(0, 0);
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2 { context.PaletteSize.x, 0 } - overlayOutset);
                                ImGui::Image(overlay.Texture, context.PaletteSize + overlayOutset * 2, overlay.GetUV0(), overlay.GetUV1());
                                ImGui::RestoreCursor();
                            }
                        }
                        else
                            ImGui::ItemSize(context.PaletteSize);

                        if ((i + 1) % context.PalettePerRow)
                            ImGui::SameLine(0, context.PaletteSpacing.x);
                        else if (i + 1 != paletteSlots)
                            ImGui::ItemSize(context.PaletteSpacing);
                    }

                    if (selection)
                    {
                        // Hack to prevent modal window from closing
                        ImGui::CloseCurrentPopup();
                        ImGui::FocusWindow(window);
                        window->WasActive = false;

                        if (auto link = ChatLink::Decode(context.EditTarget->GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                        {
                            auto& data = std::get<ChatLink::BuildTemplate>(*link);
                            context.Setter(data, water, index, *selection);
                            context.EditTarget->SetLink(ChatLink::Encode(data));
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::PopStyleVar();
            if (!((index + 1) % buttonPerRow) && index + 1 != context.BarButtonCount)
            {
                if (context.BarVertical || !water)
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - width) / 2)));
                ImGui::ItemSize(typeSize);
            }
            ImGui::SameLine(0, context.ButtonSpacing);
        }

        if (context.BarVertical)
            ImGui::NewLine();
        else
            ImGui::SameLine(0, context.BarSpacing);
    }
    if (!context.BarVertical)
        ImGui::NewLine();
}

bool Handler::BuildEditContext::IsChanged()
{
    auto const& original = GetOriginal();
    auto const& editTarget = GetEditTarget();

    return original.GetName() != editTarget.GetName()
        || original.GetLink() != editTarget.GetLink();
}

void Handler::BuildEditContext::RevertChanges()
{
    auto const& original = GetOriginal();
    auto& editTarget = GetEditTarget();

    editTarget.SetName(original.GetName());
    editTarget.SetLink(original.GetLink());
}

Build const& Handler::BuildEditContext::GetOriginal()
{
    return BuildStorageEditedBuild ? BuildStorage::Instance().GetEditTargetBuild() : Original;
}

Build& Handler::BuildEditContext::GetEditTarget()
{
    return BuildStorageEditedBuild ? BuildStorage::Instance().GetEditedBuild() : TemporaryEditTarget;
}

void Handler::RenderBuildEditor(BuildEditContext& context) const
{
    Build& editTarget = context.GetEditTarget();

    if (editTarget.GetLink().empty())
        editTarget.SetLink(ChatLink::Encode(ChatLink::BuildTemplate { }));

    bool open = true;
    float const chevron = GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE;
    float extraSpace = 0;
    switch (editTarget.GetParsedProfession())
    {
        case GW2::Profession::Ranger:
            extraSpace += ITEM_SPACING.y + 40px + chevron; // Pets
            break;
        case GW2::Profession::Revenant:
            extraSpace += 40px + chevron; // Secondary Legend Land Skills
            extraSpace += 40px + chevron; // Secondary Legend Water Skills
            extraSpace += ITEM_SPACING.y + 32px + chevron; // Legends
            break;
    }
    extraSpace += ITEM_SPACING.y + 24px + chevron; // V2: Weapons
    if (!editTarget.GetParsedInfo().WeaponSkills.empty())
        extraSpace += ITEM_SPACING.y + 24px + chevron; // V2: Weapon Skills

    ImGui::SetNextWindowSizeConstraints({ 250px, 600px + extraSpace }, { 10000px, 10000px });
    ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_Appearing, { 0.5f, 0.5f });
    ImGui::Begin(fmt::format("BuildPad##EditBuild{}", context.WindowID).c_str(), &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowFontScale(UI_SCALE);

    ImGui::PushItemWidth(-1);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
    auto buffer = util::to_buffer(editTarget.GetName());
    if (ImGui::InputTextWithHint("##EditBuildEditName", "Build Name", buffer.data(), buffer.size()))
        editTarget.SetName(buffer.data());

    buffer = util::to_buffer(editTarget.GetLink());
    if (ImGui::InputTextWithHint("##EditBuildEditLink", "Chat Link", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
        editTarget.SetLink(buffer.data());
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    Build::ParsedInfo const& parsed = editTarget.GetParsedInfo();
    if (!parsed.Error.empty())
    {
        ImGui::NewLine();
        ImGui::SameLine(-1, 1 + FRAME_PADDING.x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "%s", parsed.Error.c_str());
    }

    if (!parsed.Info.empty())
    {
        ImGui::NewLine();
        ImGui::SameLine(-1, 1 + FRAME_PADDING.x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ITEM_SPACING.y / 2);
        ImGui::TextColored(ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f), "%s", parsed.Info.c_str());
    }

    // TODO: Add flags

    ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Border), "Preview:");
    if (context.IsChanged())
    {
        ImGui::SameLine(0, 0);
        ImGui::SameLine(0, ImGui::GetContentRegionAvailWidth() - BUTTON_SIZE.x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (BUTTON_SIZE.y - LINE_SIZE.y) + ITEM_SPACING.y);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        bool on = true;
        if (TextureData const& icon = GetIcon(Icons::ClearSearch);
            ImGui::CheckboxImage(icon.Texture, "##ResetPreviewBuild", &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
            context.RevertChanges();
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Revert manual changes");
        ImGui::PopStyleVar();
    }
    ImGui::Separator();
    ImGui::BeginChild("Preview##EditBuild", { 0, -(ITEM_SPACING.y + BUTTON_SIZE.y) }, false, ImGuiWindowFlags_NoMove);
    ImGui::SetWindowFontScale(UI_SCALE);
    RenderBuildTooltip(editTarget, false, false, &editTarget, true);
    ImGui::EndChild();

    ImGui::NewLine();
    if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() + WINDOW_PADDING.x - 60px - ITEM_SPACING.x - 60px); ImGui::Button("OK", { 60, 0 }))
    {
        BuildStorage& storage = BuildStorage::Instance();
        if (Build* build = BuildStorage::Instance().FindBuild(context.ID))
        {
            // If we're currently editing that build - change the edited build instead of the original,
            // to prevent confusion with what changes can be accepted or cancelled
            if (storage.IsEditingBuild(*build))
                build = &storage.GetEditedBuild();

            build->SetName(editTarget.GetName());
            build->SetLink(editTarget.GetLink());
        }
        open = false;
    }
    if (ImGui::SameLine(); ImGui::Button("Cancel", { 60px, 0 }))
    {
        context.RevertChanges();
        open = false;
    }
    ImGui::End();

    context.Closed = !open;
}

void Handler::RenderBuildEditors()
{
    for (BuildEditContext& context : m_editedBuilds)
        RenderBuildEditor(context);

    m_editedBuilds.remove_if(util::member_equals(&BuildEditContext::Closed, true));
}

void Handler::RenderBuildTooltip(Build const& build, bool footer, bool errorMissing, Build* editTarget, bool allowChangeProfession) const
{
    API::Instance().PreloadAllBuildInfos(build);

    auto const& parsed = build.GetParsedInfo();

    ImGui::PushTextWrapPos(std::max<float>(ImGui::GetContentRegionAvailWidth(), ImGui::GetCurrentWindow()->Flags & ImGuiWindowFlags_AlwaysAutoResize ? 400px : 50px));
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "%s", fmt::format("{}", build.GetName()).c_str());
    ImGui::PopTextWrapPos();
    if (parsed.SkillsLand || parsed.SkillsWater || parsed.TraitLines)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

        float offset = 0;
        if (editTarget && allowChangeProfession)
        {
            PaletteContext<GW2::Profession, GW2::ProfessionInfo, decltype(GW2::GetProfessionInfos()), API::Profession> context { "Profession", GW2::GetProfessionInfos(), &API::Profession::Name, &API::Profession::Icon };
            context.EditTarget = editTarget;
            context.Preload = [] { API::Instance().PreloadAllProfessions(); };
            context.Getter = [&parsed](bool water, uint8_t index) { return parsed.Profession; };
            context.Setter = [](ChatLink::BuildTemplate& data, bool water, uint8_t index, GW2::Profession selection)
            {
                if (std::exchange(data.Profession, selection) != selection)
                {
                    data.ProfessionSpecific.Raw = { };
                    data.Specializations = { };
                    data.Skills = { };
                    data.WeaponSkills.clear();
                }
            };
            context.PaletteSourceLoaded = true;
            context.PaletteFilter = [](GW2::ProfessionInfo const& info, bool water, uint8_t index)
            {
                return info.Profession != GW2::Profession::None;
            };
            context.PaletteActive = [&parsed](GW2::ProfessionInfo const& info, bool water, uint8_t index)
            {
                return parsed.Profession == info.Profession;
            };
            context.PaletteSorter = [](GW2::ProfessionInfo const* a, GW2::ProfessionInfo const* b)
            {
                static std::array<GW2::Profession, 9> const professions
                {
                    GW2::Profession::Warrior,
                    GW2::Profession::Guardian,
                    GW2::Profession::Revenant,
                    GW2::Profession::Ranger,
                    GW2::Profession::Thief,
                    GW2::Profession::Engineer,
                    GW2::Profession::Necromancer,
                    GW2::Profession::Elementalist,
                    GW2::Profession::Mesmer,
                };
                return std::find(professions.begin(), professions.end(), a->Profession)
                     < std::find(professions.begin(), professions.end(), b->Profession);
            };
            context.MissingAPIIcon = Icons::MissingProfession;
            context.IconGetter = [this, &parsed](GW2::Profession const& selection, bool palette) -> TextureData const&
            {
                return palette
                    ? GetIcon(selection)
                    : parsed.Specialization != GW2::Specialization::None
                    ? GetIcon(parsed.Specialization)
                    : GetIcon(parsed.Profession);
            };
            context.InfoToDataTransform = [](GW2::ProfessionInfo const& info) { return info.Profession; };
            context.DataToAPITransform = [](GW2::Profession selection, bool water, uint8_t index) { return (uint32_t)selection; };
            context.Water = false;
            context.BarVertical = false;
            context.BarSpacing = 0px;
            context.BarButtonCount = 1;
            context.ButtonSize = { 32px, 32px };
            context.ButtonSpacing = 0px;
            context.PaletteSize = { 32px, 32px };
            context.PaletteSpacing = { 4px, 4px };
            context.PalettePerRow = 3;
            context.ButtonTooltip = "Click to change profession";
            RenderPaletteBar(context);

            offset += GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE;
        }
        else
            ImGui::Image((parsed.Specialization != GW2::Specialization::None ? GetIcon(parsed.Specialization) : GetIcon(parsed.Profession)).Texture, { 32px, 32px });

        ImGui::SameLine(0, 3px);

        auto const& info = GW2::GetSpecializationInfo(parsed.Specialization);
        std::string const text = fmt::format((parsed.SkillsLand || parsed.SkillsWater) && parsed.TraitLines ? "{} Build" : parsed.TraitLines ? "{} Traits" : "{} Skills", info.Elite ? info.Name : GW2::GetProfessionInfo(build.GetParsedProfession()).Name);
        offset += std::ceil((32px - ImGui::CalcTextSize(text.c_str()).y) / 2);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
        ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 0.5f }, "%s", text.c_str());
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - offset);
        ImGui::PopStyleVar();
    }

    if (parsed.Profession == GW2::Profession::Revenant && (parsed.SkillsLand || parsed.SkillsWater) && !parsed.TraitLines)
    {
        if (!parsed.TraitLines)
            ImGui::Text("Combine traits and skills and\nselect legends to see skills");
    }
    else if (parsed.SkillsLand || parsed.SkillsWater)
    {
        PaletteContext<uint32_t, SkillStorage::Skill*, std::vector<SkillStorage::Skill*>, API::Skill> context { "Skill", SkillStorage::Instance().GetProfessionSkills(parsed.Profession), &API::Skill::Name, &API::Skill::Icon };
        context.EditTarget = parsed.TraitLines ? editTarget : nullptr;
        context.Preload = [&] { API::Instance().PreloadAllProfessionSkills(parsed.Profession); };
        context.Getter = [&](bool water, uint8_t index)
        {
            if (parsed.Profession == GW2::Profession::Revenant && index >= 5)
            {
                if (index >= 6 && index <= 8)
                    return (water ? parsed.RevenantInactiveSkillsWater : parsed.RevenantInactiveSkillsLand)[index - 6];

                index -= 5;
            }

            return (water ? *parsed.SkillsWater : *parsed.SkillsLand)[index];
        };
        context.Setter = [&](ChatLink::BuildTemplate& data, bool water, uint8_t index, uint32_t selection)
        {
            if (parsed.Profession == GW2::Profession::Revenant && index >= 5)
            {
                if (index >= 6 && index <= 8)
                {
                    index -= 6;

                    if (selection)
                        for (uint8_t i = 0; i < 3; ++i)
                            if (data.ProfessionSpecific.Revenant.InactiveSkills.Select(water)[i] == selection)
                                data.ProfessionSpecific.Revenant.InactiveSkills.Select(water)[i] = data.ProfessionSpecific.Revenant.InactiveSkills.Select(water)[index];

                    data.ProfessionSpecific.Revenant.InactiveSkills.Select(water)[index] = selection;
                    return;
                }

                index -= 5;
            }

            if (selection)
                for (uint8_t i = 0; i < 5; ++i)
                    if (data.Skills[i].Select(water) == selection)
                        data.Skills[i].Select(water) = data.Skills[index].Select(water);

            data.Skills[index].Select(water) = selection;
        };
        context.PaletteSourceLoaded = API::Profession::Get((uint32_t)parsed.Profession);
        context.PaletteFilter = [&](SkillStorage::Skill* const& info, bool water, uint8_t index)
        {
            if (info->Specialization != GW2::Specialization::None &&
                info->Specialization != parsed.Specialization)
                return false;

            if (info->Palettes.empty())
                return false;

            if (parsed.Profession == GW2::Profession::Revenant &&
                info->ID != SkillStorage::Instance().FromPalette(info->Palettes.front(), (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[parsed.Profession == GW2::Profession::Revenant && index >= 5 ? 1 : 0]))
                return false;

            if (parsed.Profession == GW2::Profession::Revenant && index >= 5)
                index -= 5;

            switch (index)
            {
                case 0: return info->Type == SkillStorage::SkillType::Heal;
                case 1:
                case 2:
                case 3: return info->Type == SkillStorage::SkillType::Utility;
                case 4: return info->Type == SkillStorage::SkillType::Elite;
                default: return false;
            }
        };
        context.PaletteActive = [&](SkillStorage::Skill* const& info, bool water, uint8_t index) -> bool
        {
            if (parsed.Profession == GW2::Profession::Revenant && index >= 6 && index <= 8)
                return util::find_if(water ? parsed.RevenantInactiveSkillsWater : parsed.RevenantInactiveSkillsLand, util::equals(info->ToPalette()));

            return util::find_if(water ? *parsed.SkillsWater : *parsed.SkillsLand, util::equals(info->ToPalette()));
        };
        context.PaletteUsable = [&](SkillStorage::Skill* const& info, bool water, uint8_t index)
        {
            if (parsed.Profession == GW2::Profession::Revenant)
                return true;

            if (water)
                if (auto const& skill = API::Skill::Get(info->ID))
                    return !skill.NoUnderwater;

            return true;
        };
        context.MissingAPIIcon = Icons::MissingSkill;
        context.InfoToDataTransform = [&](SkillStorage::Skill* const& info) { return info->ToPalette(); };
        context.DataToAPITransform = [&](uint32_t selection, bool water, uint8_t index) { return SkillStorage::Instance().FromPalette(selection, (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[parsed.Profession == GW2::Profession::Revenant && index >= 5 ? 1 : 0]); };
        context.Water = true;
        context.DarkenSecondHalf = parsed.Profession == GW2::Profession::Revenant;
        context.TypeSize = { 24px, 24px };
        context.BarVertical = true;
        context.BarSpacing = 10px;
        context.BarButtonCount = parsed.Profession == GW2::Profession::Revenant ? 10 : 5;
        context.BarButtonPerRow = 5;
        context.ButtonSize = { 40px, 40px };
        context.ButtonSpacing = 0px;
        context.PaletteSize = { 45px, 45px };
        context.PaletteSpacing = { 5px, 5px };
        context.PalettePerRow = 4;
        context.PaletteReverse = true;
        context.ButtonTooltip = "Click to change skill";
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        RenderPaletteBar(context);
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
                PaletteContext<uint8_t, Pet, decltype(m_pets), API::Pet> context { "RangerPet", m_pets, &API::Pet::Name, &API::Pet::Icon };
                context.EditTarget = editTarget;
                context.Preload = [] { API::Instance().PreloadAllPets(); };
                context.Getter = [&parsed](bool water, uint8_t index) { return (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[index]; };
                context.Setter = [](ChatLink::BuildTemplate& data, bool water, uint8_t index, uint8_t selection)
                {
                    if (selection && data.ProfessionSpecific.RangerPets.Select(water)[1 - index] == selection)
                        data.ProfessionSpecific.RangerPets.Select(water)[1 - index] = data.ProfessionSpecific.RangerPets.Select(water)[index];

                    data.ProfessionSpecific.RangerPets.Select(water)[index] = selection;
                };
                context.PaletteSourceLoaded = m_petsLoaded;
                context.PaletteFilter = [](Pet const& info, bool water, uint8_t index)
                {
                    if (!water && info.Type != Pet::PetType::Terrestrial && info.Type != Pet::PetType::Amphibious)
                        return false;

                    if (water && info.Type != Pet::PetType::Aquatic && info.Type != Pet::PetType::Amphibious)
                        return false;

                    return true;
                };
                context.PaletteActive = [&parsed](Pet const& info, bool water, uint8_t index)
                {
                    return (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[1 - index] == info.ID
                        || (water ? parsed.RangerPetsWater : parsed.RangerPetsLand)[    index] == info.ID;
                };
                context.MissingAPIIcon = Icons::MissingPet;
                context.InfoToDataTransform = [](Pet const& info) { return info.ID; };
                context.DataToAPITransform = [](uint8_t selection, bool water, uint8_t index) { return selection; };
                context.Water = true;
                context.TypeSize = { 16px, 16px };
                context.BarVertical = false;
                context.BarSpacing = 10px;
                context.BarButtonCount = 2;
                context.ButtonSize = { 40px, 40px };
                context.ButtonSpacing = 0px;
                context.PaletteSize = { 56px, 56px };
                context.PaletteSpacing = { 0px, 0px };
                context.PalettePerRow = 10;
                context.ButtonTooltip = "Click to change pet";
                ImGui::Spacing();
                RenderPaletteBar(context);
                break;
            }
            case GW2::Profession::Revenant:
            {
                PaletteContext<GW2::RevenantLegend, GW2::RevenantLegendInfo, decltype(GW2::GetRevenantLegendInfos()), API::Skill> context { "RevenantLegend", GW2::GetRevenantLegendInfos(), &API::Skill::Name, &API::Skill::Icon };
                context.EditTarget = editTarget;
                context.Getter = [&parsed](bool water, uint8_t index) { return (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[1 - index]; };
                context.Setter = [](ChatLink::BuildTemplate& data, bool water, uint8_t index, GW2::RevenantLegend selection)
                {
                    if (selection != GW2::RevenantLegend::None && data.ProfessionSpecific.Revenant.Legends.Select(water)[index] == selection)
                        data.ProfessionSpecific.Revenant.Legends.Select(water)[index] = data.ProfessionSpecific.Revenant.Legends.Select(water)[1 - index];

                    data.ProfessionSpecific.Revenant.Legends.Select(water)[1 - index] = selection;
                };
                context.PaletteSourceLoaded = true;
                context.PaletteFilter = [&parsed](GW2::RevenantLegendInfo const& info, bool water, uint8_t index)
                {
                    return info.RequiredSpecialization == GW2::Specialization::None || info.RequiredSpecialization == parsed.Specialization;
                };
                context.PaletteActive = [&parsed](GW2::RevenantLegendInfo const& info, bool water, uint8_t index)
                {
                    return info.Legend != GW2::RevenantLegend::None
                        && ((water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[    index] == info.Legend
                        ||  (water ? parsed.RevenantLegendsWater : parsed.RevenantLegendsLand)[1 - index] == info.Legend);
                };
                context.PaletteSorter = [](GW2::RevenantLegendInfo const* a, GW2::RevenantLegendInfo const* b)
                {
                    static std::array<GW2::RevenantLegend, 8> const legends
                    {
                        GW2::RevenantLegend::Jallis,
                        GW2::RevenantLegend::Mallix,
                        GW2::RevenantLegend::Ventari,
                        GW2::RevenantLegend::Shiro,
                        GW2::RevenantLegend::Glint,
                        GW2::RevenantLegend::Kalla,
                        GW2::RevenantLegend::Alliance,
                        GW2::RevenantLegend::None,
                    };
                    return std::find(legends.begin(), legends.end(), a->Legend)
                         < std::find(legends.begin(), legends.end(), b->Legend);
                };
                context.PaletteUsable = [&](GW2::RevenantLegendInfo const& info, bool water, uint8_t index)
                {
                    return water ? info.Aquatic : info.Terrestrial;
                };
                context.MissingAPIIcon = Icons::MissingSkill;
                context.InfoToDataTransform = [](GW2::RevenantLegendInfo const& info) { return info.Legend; };
                context.DataToAPITransform = [](GW2::RevenantLegend selection, bool water, uint8_t index) { return SkillStorage::Instance().GetRevenantLegendSwapSkill(selection); };
                context.Water = true;
                context.DarkenFirstHalf = true;
                context.TypeSize = { 16px, 16px };
                context.BarVertical = false;
                context.BarSpacing = 16px;
                context.BarButtonCount = 2;
                context.ButtonSize = { 32px, 32px };
                context.ButtonSpacing = 0px;
                context.PaletteSize = { 45px, 45px };
                context.PaletteSpacing = { 5px, 5px };
                context.PalettePerRow = 4;
                context.PaletteReverse = true;
                context.ButtonTooltip = "Click to change legend";
                ImGui::Spacing();
                RenderPaletteBar(context);
                break;
            }
            default:
                break;
        }
    }

    if (parsed.TraitLines)
    {
        uint8_t lineIndex = 0;
        for (auto const& line : *parsed.TraitLines)
        {
            auto const& info = API::Specialization::Get((uint32_t)line.Specialization);

            std::string const text { info ? info.Name : line.Specialization != GW2::Specialization::None ? GW2::GetSpecializationInfo(line.Specialization).Name : "Empty" };
            if (info.WeaponTrait)
            {
                auto const& traitInfo = API::Trait::Get(info.WeaponTrait);
                auto const& icon = traitInfo.Icon.Get();
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
                float const height = 48px + (parsed.SkillsLand && parsed.SkillsWater && editTarget ? GetIcon(Icons::SelectionChevron).TrimmedSize().y * UI_SCALE * 2 : 0);
                ImGui::SameLine(0, std::max<float>(0, std::ceil((ImGui::GetContentRegionAvailWidth() - (48px + 5px + 24px + 5px + 24px + 5px + 24px)) / 2)));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (24px * 3 - height) / 2);

                PaletteContext<GW2::Specialization, GW2::Specialization, decltype(API::Profession::Specializations), API::Specialization> context { fmt::format("Specialization{0}", lineIndex), API::Profession::Get((uint32_t)parsed.Profession).Specializations, &API::Specialization::Name, &API::Specialization::Icon };
                context.EditTarget = parsed.SkillsLand && parsed.SkillsWater ? editTarget : nullptr;
                context.Preload = [&] { API::Instance().PreloadAllProfessionSpecializations(parsed.Profession); };
                context.Getter = [&](bool water, uint8_t index) { return (*parsed.TraitLines)[lineIndex].Specialization; };
                context.Setter = [&](ChatLink::BuildTemplate& data, bool water, uint8_t index, GW2::Specialization selection)
                {
                    if (selection != GW2::Specialization::None)
                        for (uint8_t i = 0; i < 3; ++i)
                            if (data.Specializations[i].Specialization == selection && i != lineIndex)
                            {
                                std::swap(data.Specializations[i], data.Specializations[lineIndex]);
                                // Clear if this is an elite specialization (can't be swapped with anything, as it can only reside in last line)
                                if (GW2::GetSpecializationInfo(data.Specializations[i].Specialization).Elite)
                                    data.Specializations[i] = ChatLink::BuildTemplate::SpecializationData { };
                            }

                    if (std::exchange(data.Specializations[lineIndex].Specialization, selection) != selection)
                        data.Specializations[lineIndex].Trait0 = data.Specializations[lineIndex].Trait1 = data.Specializations[lineIndex].Trait2 = 0;

                    // Clear skills and legends when switching away from an elite specialization
                    GW2::Specialization const oldSpecialization = parsed.Specialization;
                    GW2::Specialization const newSpecialization = data.Specializations[2].Specialization;
                    if (oldSpecialization != newSpecialization && GW2::GetSpecializationInfo(oldSpecialization).Elite)
                    {
                        if (data.Profession == GW2::Profession::Revenant)
                        {
                            for (uint8_t type = 0; type < 2; ++type)
                                for (auto& legend : data.ProfessionSpecific.Revenant.Legends.Select((bool)type))
                                    if (legend != GW2::RevenantLegend::None)
                                        if (GW2::Specialization const legendSpec = GW2::GetRevenantLegendInfo(legend).RequiredSpecialization; legendSpec != GW2::Specialization::None && legendSpec != newSpecialization)
                                            legend = GW2::RevenantLegend::None;
                        }
                        else
                        {
                            for (uint8_t type = 0; type < 2; ++type)
                                for (auto& skill : data.Skills)
                                    if (uint16_t& palette = skill.Select((bool)type))
                                        if (GW2::Specialization const skillSpec = SkillStorage::Instance().GetPaletteSpecialization(palette, data.ProfessionSpecific.Revenant.Legends.Select((bool)type)[0]); GW2::GetSpecializationInfo(skillSpec).Elite && skillSpec != newSpecialization)
                                            palette = 0;
                        }
                    }
                };
                context.PaletteSourceLoaded = API::Profession::Get((uint32_t)parsed.Profession);
                context.PaletteFilter = [&lineIndex](GW2::Specialization const& info, bool water, uint8_t index)
                {
                    return lineIndex == 2 || !GW2::GetSpecializationInfo(info).Elite;
                };
                context.PaletteActive = [&parsed](GW2::Specialization const& info, bool water, uint8_t index)
                {
                    return util::find_if(*parsed.TraitLines, util::member_equals(&Build::ParsedInfo::TraitLine::Specialization, info));
                };
                context.MissingAPIIcon = Icons::MissingSpecialization;
                context.InfoToDataTransform = [](GW2::Specialization const& info) { return info; };
                context.DataToAPITransform = [](GW2::Specialization selection, bool water, uint8_t index) { return (uint32_t)selection; };
                context.Water = false;
                context.BarVertical = false;
                context.BarSpacing = 0px;
                context.BarButtonCount = 1;
                context.ButtonSize = { 48px, 48px };
                context.ButtonSpacing = 0px;
                context.PaletteSize = { 48px, 48px };
                context.PaletteSpacing = { 0px, 0px };
                context.PalettePerRow = 5;
                context.ButtonTooltip = "Click to change specialization";
                ImGui::BeginChild(fmt::format("SpecializationContainer{}", lineIndex).c_str(), { 48px, height }, false, ImGuiWindowFlags_NoMove);
                RenderPaletteBar(context);
                ImGui::EndChild();

                ImGui::SameLine();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (24px * 3 - height) / 2);
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
                    auto const& icon = trait ? traitInfo.Icon.Get() : GetIcon(Icons::LoadingTrait);
                    ImVec2 cursor = ImGui::GetCursorScreenPos();
                    bool hovered = ImGui::IsMouseHoveringRect(cursor, cursor + ImVec2 { 24px, 24px }) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
                    float multiplier = editTarget ? (hovered ? 1.0f : 0.8f) : 1.0f;
                    multiplier *= line.Traits[i / 3] == i % 3 + 1 ? 1.0f : 0.25f;
                    ImVec4 color = ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f };
                    color.x *= multiplier;
                    color.y *= multiplier;
                    color.z *= multiplier;
                    ImGui::Image(icon.Texture, { 24px, 24px }, icon.GetUV0(), icon.GetUV1(), color);
                    if (ImGui::IsItemHovered())
                    {
                        if (editTarget)
                            ImGui::TooltipWithHeader(traitInfo.Name.c_str(), "Click to change trait");
                        else
                            ImGui::Tooltip(traitInfo.Name.c_str());
                    }
                    if (ImGui::IsItemClicked() && editTarget)
                    {
                        if (auto link = ChatLink::Decode(editTarget->GetLink()); link && std::holds_alternative<ChatLink::BuildTemplate>(*link))
                        {
                            auto& data = std::get<ChatLink::BuildTemplate>(*link);
                            auto& specialization = data.Specializations[lineIndex];
                            uint8_t value = i % 3 + 1;
                            switch (i / 3)
                            {
                                case 0: specialization.Trait0 = value; break;
                                case 1: specialization.Trait1 = value; break;
                                case 2: specialization.Trait2 = value; break;
                            }
                            editTarget->SetLink(ChatLink::Encode(data));
                        }
                    }
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

            ++lineIndex;
        }
    }
    else if (errorMissing)
        ImGui::TextColored({ 1, 0, 0, 1 }, "No Traits Selected!");

    // V2: Weapons
    if (!parsed.Weapons.empty() || editTarget)
    {
        PaletteContext<GW2::Weapon, GW2::WeaponInfo, decltype(GW2::GetWeaponInfos()), API::Weapon> context { "Weapon", GW2::GetWeaponInfos(), &API::Weapon::Name, &API::Weapon::Icon };
        context.EditTarget = editTarget;
        context.Preload = [] { API::Instance().PreloadAllWeapons(); };
        context.Getter = [&parsed](bool water, uint8_t index) { return index < parsed.Weapons.size() ? parsed.Weapons[index] : GW2::Weapon::None; };
        context.Setter = [](ChatLink::BuildTemplate& data, bool water, uint8_t index, GW2::Weapon selection)
        {
            if (selection != GW2::Weapon::None)
            {
                if (index < data.Weapons.size())
                    data.Weapons[index] = selection;
                else
                    data.Weapons.push_back(selection);
            }
            else if (index < data.Weapons.size())
                data.Weapons.erase(data.Weapons.begin() + index);
        };
        context.PaletteSourceLoaded = true;
        context.PaletteFilter = [](GW2::WeaponInfo const& info, bool water, uint8_t index) { return true; };
        context.PaletteActive = [&parsed](GW2::WeaponInfo const& info, bool water, uint8_t index)
        {
            return index < parsed.Weapons.size() && parsed.Weapons[index] == info.Weapon || info.Weapon == GW2::Weapon::None;
        };
        context.PaletteUsable = [](GW2::WeaponInfo const& info, bool water, uint8_t index)
        {
            // TODO: Determine usability by profession
            return true;
        };
        context.PaletteSorter = [](GW2::WeaponInfo const* a, GW2::WeaponInfo const* b)
        {
            if ((a->Weapon != GW2::Weapon::None) != (b->Weapon != GW2::Weapon::None))
                return a->Weapon != GW2::Weapon::None;
            auto order = [](GW2::WeaponInfo const& info) { return !info.Aquatic ? 0 : info.Aquatic && info.Terrestrial ? 1 : !info.Terrestrial ? 2 : 3; };
            if (auto const aOrder = order(*a), bOrder = order(*b); aOrder != bOrder)
                return aOrder < bOrder;
            return a->Name < b->Name;
        };
        context.MissingAPIIcon = Icons::MissingWeapon;
        context.InfoToDataTransform = [](GW2::WeaponInfo const& info) { return info.Weapon; };
        context.DataToAPITransform = [](GW2::Weapon selection, bool water, uint8_t index) { return (uint32_t)selection; };
        context.Water = false;
        context.BarVertical = false;
        context.BarSpacing = 0px;
        context.BarButtonCount = std::min<uint8_t>(ChatLink::BuildTemplate::MAX_WEAPONS, parsed.Weapons.size() + (editTarget ? 1 : 0));
        context.ButtonSize = { 24px, 24px };
        context.ButtonSpacing = 4px;
        context.PaletteSize = { 32px, 32px };
        context.PaletteSpacing = { 0px, 0px };
        context.PalettePerRow = 4;
        context.ButtonTooltip = "Click to change weapon";
        RenderPaletteBar(context);
    }

    // V2: Weapon Skills
    if (!parsed.WeaponSkills.empty() || editTarget)
    {
        PaletteContext<uint32_t, SkillStorage::Skill*, std::vector<SkillStorage::Skill*>, API::Skill> context { "WeaponSkill", SkillStorage::Instance().GetProfessionSkills(parsed.Profession), &API::Skill::Name, &API::Skill::Icon };
        context.EditTarget = editTarget;
        context.Preload = [] { /* TODO */ };
        context.Getter = [&parsed](bool water, uint8_t index) { return index < parsed.WeaponSkills.size() ? parsed.WeaponSkills[index] : 0; };
        context.Setter = [](ChatLink::BuildTemplate& data, bool water, uint8_t index, uint32_t selection)
        {
            if (selection)
            {
                if (index < data.WeaponSkills.size())
                    data.WeaponSkills[index] = selection;
                else
                    data.WeaponSkills.push_back(selection);
            }
            else if (index < data.WeaponSkills.size())
                data.WeaponSkills.erase(data.WeaponSkills.begin() + index);
        };
        context.PaletteSourceLoaded = false; // TODO
        context.PaletteFilter = [](SkillStorage::Skill* const& info, bool water, uint8_t index)
        {
            // TODO
            return false;
        };
        context.PaletteActive = [](SkillStorage::Skill* const& info, bool water, uint8_t index)
        {
            // TODO
            return false;
        };
        context.PaletteSorter = [](SkillStorage::Skill* const* a, SkillStorage::Skill* const* b)
        {
            // TODO
            return true;
        };
        context.PaletteUsable = [](SkillStorage::Skill* const& info, bool water, uint8_t index)
        {
            // TODO
            return true;
        };
        context.MissingAPIIcon = Icons::MissingSkill;
        context.InfoToDataTransform = [&](SkillStorage::Skill* const& info) { return info->ID; };
        context.DataToAPITransform = [&](uint32_t selection, bool water, uint8_t index) { return selection; };
        context.Water = false;
        context.BarVertical = true;
        context.BarSpacing = 10px;
        context.BarButtonCount = std::min<uint8_t>(ChatLink::BuildTemplate::MAX_WEAPON_SKILLS, parsed.WeaponSkills.size() /* TODO: + (editTarget ? 1 : 0)*/);
        context.BarButtonPerRow = 5;
        context.ButtonSize = { 24px, 24px };
        context.ButtonSpacing = 4px;
        context.PaletteSize = { 45px, 45px };
        context.PaletteSpacing = { 5px, 5px };
        context.PalettePerRow = 4;
        context.PaletteReverse = true;
        context.ButtonTooltip = "Click to change weapon skill\n(not supported yet)"; // TODO
        RenderPaletteBar(context);
    }

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

        // Don't allow combining elite spec skills with other traits of a different elite spec or none at all
        Build const& traits = a.GetParsedInfo().TraitLines ? a : b;
        Build const& skills = a.GetParsedInfo().SkillsLand || a.GetParsedInfo().SkillsWater ? a : b;
        if (GW2::GetSpecializationInfo(skills.GetParsedSpecialization()).Elite && traits.GetParsedSpecialization() != skills.GetParsedSpecialization())
            return false;

        Build test { 0 };
        test.SetLink(a.GetLink());
        test.SetSecondaryLink(b.GetLink());
        auto& savedBuilds = BuildStorage::Instance().GetBuilds();
        if (util::find_if(savedBuilds, util::method_equals(&Build::GetLink, test.GetLink())))
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
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Header, std::abs((float)blinkingPeriod.count() / 500 - 1)));
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
                    if (util::find_if(convertedBuilds, [&test](Build const& b) { return b.GetLink() == test.GetLink() && b.GetName() == test.GetName(); }))
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
        ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), fmt::format("{} Build{} Converted!", convertedBuildsSuccessCount, convertedBuildsSuccessCount != 1 ? "s" : "").c_str(), { 0, 0 }, ImDrawCornerFlags_None, -1, { 0, 0, 0, 0 }, { 1.0f, 1.0f, 1.0f, color.w });
        ImGui::PopStyleColor(4);
        if (util::find_if(m_arcdpsTraits, util::method_equals(&Build::GetParsedProfession, GW2::Profession::Ranger)) ||
            util::find_if(m_arcdpsSkills, util::method_equals(&Build::GetParsedProfession, GW2::Profession::Ranger)))
        {
            ImGui::Spacing();
            ImGui::TextWrapped("Ranger builds will not be converted automatically due to missing information about selected pets.");
        }
        if (util::find_if(m_arcdpsTraits, util::method_equals(&Build::GetParsedProfession, GW2::Profession::Revenant)) ||
            util::find_if(m_arcdpsSkills, util::method_equals(&Build::GetParsedProfession, GW2::Profession::Revenant)))
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
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
                }
                if (exists || incompatible)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4Grayscale(ImGuiCol_Button));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                    ImGui::PushStyleColor(ImGuiCol_Text, exists ? ImVec4 { 0, 0, 0, 1.0f } : ImVec4 { 0.25f, 0, 0, 1.0f });
                }

                ImGui::PushItemWidth(-1);
                if (TextureData const& icon = build.GetParsedSpecialization() != GW2::Specialization::None ? GetIcon(build.GetParsedSpecialization()) : GetIcon(build.GetParsedProfession());
                    ImGui::ImageButtonWithText(icon.Texture, fmt::format("{}##{}", build.GetName(), build.GetID()).c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, ImDrawCornerFlags_All, 0, { 0, 0, 0, 0 }, exists ? ImVec4 { 0, 0, 0, 1.0f } : incompatible ? ImVec4 { 0.75f, 0, 0, 1.0f } : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f }, selected)
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
                {
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor(2);
                }

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
    ImGui::BeginChild("Traits##ArcDPSMigration", { 0, 0 }, false, ImGuiWindowFlags_NoMove);
    ImGui::SetWindowFontScale(UI_SCALE);
    buildList(m_arcdpsTraits, false);
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Skills").x) / 2);
    ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(0xFF44BBEE), "Skills");
    ImGui::BeginChild("Skills##ArcDPSMigration", { 0, 0 }, false, ImGuiWindowFlags_NoMove);
    ImGui::SetWindowFontScale(UI_SCALE);
    buildList(m_arcdpsSkills, true);
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("PreviewHeader##ArcDPSMigration", { 0, -1 }, false, ImGuiWindowFlags_NoMove);
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
                    data.ProfessionSpecific.Revenant.Legends.Land[0] = previousRevenantLegendsLand[0];
                    data.ProfessionSpecific.Revenant.Legends.Land[1] = previousRevenantLegendsLand[1];
                    data.ProfessionSpecific.Revenant.Legends.Water[0] = previousRevenantLegendsWater[0];
                    data.ProfessionSpecific.Revenant.Legends.Water[1] = previousRevenantLegendsWater[1];
                    for (uint8_t water = 0; water < 2; ++water)
                        for (uint8_t i = 0; i < 3; ++i)
                            if (!data.ProfessionSpecific.Revenant.InactiveSkills.Select((bool)water)[i])
                                data.ProfessionSpecific.Revenant.InactiveSkills.Select((bool)water)[i] = data.Skills[1 + i].Select((bool)water);
                    previewBuild.SetLink(ChatLink::Encode(data));
                }
                break;
            default:
                break;
        }

        previewBuild.SetSaved();
    }

    ImGui::PushItemWidth(-1);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.25f));
    auto buffer = util::to_buffer(previewBuild.GetName());
    if (ImGui::InputTextWithHint("##ArcDPSMigrationEditName", "Build Name", buffer.data(), buffer.size(), ImGuiInputTextFlags_AutoSelectAll))
        previewBuild.SetName(buffer.data());
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

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
        ImGui::ImageButtonWithText(GetIcon(Icons::AcceptBuildEdit), "Build Saved!", { 0, 0 }, ImDrawCornerFlags_None, -1, { 0, 0, 0, 0 }, { 1.0f, 1.0f, 1.0f, color.w });
        ImGui::PopStyleColor(4);
    }

    bool const disabled = !selectedTraits || !selectedSkills;
    if (disabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4Grayscale(ImGuiCol_Button));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_Button));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_Button));
    }

    if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 1 - FRAME_PADDING.x - ImGui::CalcTextSize("Save").x - FRAME_PADDING.x, 0);
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

    ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_Border), "Preview:");
    if (previewBuild.IsSaveNeeded())
    {
        ImGui::SameLine(0, 0);
        ImGui::SameLine(0, ImGui::GetContentRegionAvailWidth() - BUTTON_SIZE.x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (BUTTON_SIZE.y - LINE_SIZE.y) + ITEM_SPACING.y);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        bool on = true;
        if (TextureData const& icon = GetIcon(Icons::ClearSearch);
            ImGui::CheckboxImage(icon.Texture, "##ResetPreviewBuild", &on, icon.GetUV0(), icon.GetUV1(), 0.4f, 0.6f, 0.5f, 0.9f, 1.0f, 0.95f))
        {
            previewBuild.SetLink({});
            selectionChanged = true;
        }
        if (ImGui::IsItemHovered())
            ImGui::Tooltip("Revert manual changes");
        ImGui::PopStyleVar();
    }
    ImGui::Separator();
    ImGui::GetCurrentWindowRead()->Size.y = ImGui::GetCursorPosY();
    ImGui::EndChild();
    ImGui::BeginChild("Preview##ArcDPSMigration", { 0, 0 }, false, ImGuiWindowFlags_NoMove);
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
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() + WINDOW_PADDING.x - 60px - ITEM_SPACING.x - 60px); ImGui::Button("OK", { 60, 0 }))
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
        if (ImGui::SameLine(ImGui::GetContentRegionAvailWidth() + WINDOW_PADDING.x - 60px); ImGui::Button("Close", { 60px, 0 }))
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
    static std::list<ItemStatsIconsInfo> iconSets;
    static bool iconSetsInited = false;
    if (!iconSetsInited)
    {
        iconSetsInited = true;
        iconSets.emplace_back();
        Web::Instance().RequestAny({ URL_WITH_FALLBACK("/itemstatsicons.json") }, [this](std::string_view const data)
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
                            Web::Instance().Request(stats["icon"], [this, &icon](std::string_view const data) { icon.Icon = LoadTexture(std::pair { data.data(), data.size() }); });
                    }
                }
            }
        });
    }

    ImGui::Columns(2);

    ImGui::BeginChild("Gear##ArcDPSGear", { 0, 0 }, false, ImGuiWindowFlags_NoMove);
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
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
            }

            std::string name;
            if (legendary)
                name = fmt::format("{}##{}", base.Name.data(), (size_t)&temp);
            else
                name = fmt::format("[{}] {}##{}", std::get<gear_t>(temp).Character.data(), base.Name.data(), (size_t)&temp);

            ImGui::PushItemWidth(-1);
            if (TextureData const& icon = GetIcon((GW2::Profession)base.Profession);
                ImGui::ImageButtonWithText(icon.Texture, name.c_str(), { -1, BUTTON_SIZE.y }, BUTTON_SIZE, { 0.0625f, 0.0625f }, { 0.9375f, 0.9375f }, ImDrawCornerFlags_All, 0, { 0, 0, 0, 0 }, legendary ? ImGui::ColorConvertU32ToFloat4(0xFFFF3399) : ImVec4 { 1.0f, 1.0f, 1.0f, 1.0f }, selected))
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
            {
                ImGui::PopStyleVar();
                ImGui::PopStyleColor(2);
            }

            EndRenderBuildList(false);
            firstVisibleTemplate = false;
        }
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Preview##ArcDPSGear", { 0, iconSets.size() > 1 ? -(ITEM_SPACING.y + BUTTON_SIZE.y) : 0 }, false, ImGuiWindowFlags_NoMove);
    ImGui::SetWindowFontScale(UI_SCALE);
    float const iconHeight = LINE_SIZE.y + ITEM_SPACING.y + LINE_SIZE.y + ITEM_SPACING.y + LINE_SIZE.y;
    ImVec2 const iconSize { iconHeight, iconHeight };
    if (selection)
    {
        bool const legendary = std::holds_alternative<legendary_t>(*selection);
        legendary_t const& base = legendary ? std::get<legendary_t>(*selection) : std::get<gear_t>(*selection);

        auto const& iconSet = [this]() -> ItemStatsIconsInfo const&
        {
            if (auto const itr = util::find_if(iconSets, util::member_equals(&ItemStatsIconsInfo::Name, m_config.GearIconSet)))
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
                    ImGui::Image(info.Icon.Get().Texture, iconSize);
                }
                if (auto const icon = iconSet.GetIcon(item.Stats))
                {
                    ImGui::StoreCursor();
                    ImGui::SameLine(0.001f);
                    ImGui::SetCursorPos(ImGui::GetCursorPos() + (iconSize - iconSizeOverlay));
                    ImGui::Image(icon->Texture, iconSizeOverlay, icon->GetUV0(), icon->GetUV1());
                    ImGui::RestoreCursor();
                }
                if (!legendary)
                {
                    if (auto const visibility = std::get<gear_t>(*selection).GetSlotVisibility(slot.Slot))
                    {
                        auto const& icon = GetIcon(*visibility ? Icons::CheckBoxChecked : Icons::CheckBoxUnchecked);
                        ImGui::StoreCursor();
                        ImGui::SameLine(0.001f);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6 + (iconSize.x - icon.TrimmedSize().x));
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6);
                        ImGui::Image(icon.Texture, icon.TrimmedSize(), icon.GetUV0(), icon.GetUV1());
                        ImGui::RestoreCursor();
                    }
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
                    ImGui::Image(info.Icon.Get().Texture, { LINE_SIZE.y, LINE_SIZE.y });
                    ImGui::SameLine();
                }
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(GW2::GetRarityInfo(info.Rarity).Color), "%s", info.Name.c_str());
            }
            for (uint32_t upgrade : item.Upgrades)
            {
                if (!upgrade)
                    continue;

                auto const& info = API::Item::Get(upgrade);
                ImGui::Image(info.Icon.Get().Texture, { LINE_SIZE.y, LINE_SIZE.y });
                ImGui::SameLine();
                ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(GW2::GetRarityInfo(info.Rarity).Color), "%s", info.Name.c_str());
            }
            for (uint32_t infusion : item.Infusions)
            {
                if (!infusion)
                    continue;

                auto const& info = API::Item::Get(infusion);
                ImGui::Image(info.Icon.Get().Texture, { LINE_SIZE.y, LINE_SIZE.y });
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
        if (int iconSet = (int)util::distance_if(iconSets, util::member_equals(&ItemStatsIconsInfo::Name, m_config.GearIconSet));
            ImGui::Combo("", &iconSet, [](void* data, int const index, char const** text) { auto itr = iconSets.begin(); std::advance(itr, index); return *text = (!itr->Name.empty() ? itr->Name : DEFAULT_NAME).c_str(), true; }, nullptr, (int)iconSets.size()))
        {
            auto itr = iconSets.begin();
            std::advance(itr, iconSet);
            m_config.GearIconSet = itr->Name;
        }
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
    }

    Web::Instance().RequestAny({ URL_WITH_FALLBACK("/version.json") }, [this](std::string_view const data)
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

        m_versionCheckResult = true;
    }, [this](auto const&) { m_versionCheckResult = false; }, false);
}

void Handler::RenderVersionHistory(bool all)
{
    ImGui::SetNextWindowSizeConstraints({ 400px, 300px }, { 10000px, 10000px });
    ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_Appearing, { 0.5f, 0.5f });
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

        bool const old = m_versionLastViewedHistory < m_versionLatest.Version && info.Version <= m_versionLastViewedHistory;
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
    ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize / 2, ImGuiCond_Appearing, { 0.5f, 0.5f });
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
    ImVec4 const color = ImGui::GetStyleColorVec4ModAlpha(ImGuiCol_Text, 0.5f);
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
        { "ImGui", "Copyright (c) 2014-2021 Omar Cornut", "https://github.com/ocornut/imgui" },
        { "{fmt}", "Copyright (c) 2012 - 2019, Victor Zverovich", "https://github.com/fmtlib/fmt" },
        { "JSON for Modern C++", "Copyright (c) 2013-2019 Niels Lohmann", "https://github.com/nlohmann/json" },
        { "cpp-base64", (char const*)u8"Copyright (C) 2004-2017 René Nyffenegger", "https://github.com/ReneNyffenegger/cpp-base64" },
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

    spng_format const format = std::visit(overloaded
    {
        [&](IDirect3DDevice9*) { return SPNG_FMT_BGRA8; },
        [&](ID3D11Device*) { return SPNG_FMT_RGBA8; },
        [](auto&&) { return SPNG_FMT_RGBA8; },
    }, m_d3dDevice);

    spng_ctx* ctx = spng_ctx_new(0);
    auto guard = gsl::finally([ctx] { spng_ctx_free(ctx); });
    CHECK(spng_set_png_buffer(ctx, buffer.data(), buffer.size()));
    spng_ihdr header { };
    CHECK(spng_get_ihdr(ctx, &header));
    size_t size;
    CHECK(spng_decoded_image_size(ctx, format, &size));
    std::vector<char> out(size);
    CHECK(spng_decode_image(ctx, (byte*)out.data(), out.size(), format, 0));
    return { std::move(out), header.width, header.height };
#undef CHECK
}

std::optional<TextureData> Handler::LoadTexture(std::variant<fs::path, std::pair<char const*, size_t>>&& source)
{
    try
    {
        auto&& [data, width, height] = LoadImageFile(std::move(source));
        if (data.empty())
            return { };

        TextureID texture;
        if (std::visit(overloaded
        {
            [&](IDirect3DDevice9* device)
            {
                if (HRESULT const result = device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture.D3D9, 0); result != S_OK)
                    return result;

                D3DLOCKED_RECT rect;
                texture.D3D9->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
                memcpy(rect.pBits, data.data(), sizeof(char) * width * height * 4);
                texture.D3D9->UnlockRect(0);
                return S_OK;
            },
            [&](ID3D11Device* device)
            {
                D3D11_TEXTURE2D_DESC desc;
                ZeroMemory(&desc, sizeof desc);
                desc.Width = width;
                desc.Height = height;
                desc.MipLevels = 1;
                desc.ArraySize = 1;
                desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                desc.SampleDesc.Count = 1;
                desc.Usage = D3D11_USAGE_DEFAULT;
                desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

                D3D11_SUBRESOURCE_DATA subResource;
                ZeroMemory(&subResource, sizeof subResource);
                subResource.pSysMem = data.data();
                subResource.SysMemPitch = desc.Width * 4;
                subResource.SysMemSlicePitch = 0;

                ID3D11Texture2D* tex = nullptr;
                if (HRESULT const result = device->CreateTexture2D(&desc, &subResource, &tex); result != S_OK)
                    return result;
                auto guard = gsl::finally([tex] { tex->Release(); });

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
                ZeroMemory(&srvDesc, sizeof srvDesc);
                srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = desc.MipLevels;
                srvDesc.Texture2D.MostDetailedMip = 0;
                return device->CreateShaderResourceView(tex, &srvDesc, &texture.D3D11);
            },
            [](auto&&) { return S_OK; },
        }, m_d3dDevice))
            return { };

        if (!texture)
            return { };

        m_loadedTextures.push_back(texture);

        return TextureData { texture, width, height };
    }
    catch (...) { return { }; }
}

void Handler::UnloadTexture(TextureID const& texture)
{
    if (texture)
    {
        std::visit(overloaded
        {
            [&](IDirect3DDevice9*) { texture.D3D9->Release(); },
            [&](ID3D11Device*) { texture.D3D11->Release(); },
            [](auto&&) { },
        }, m_d3dDevice);
        m_loadedTextures.remove(texture);
    }
}

}
