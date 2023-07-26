#pragma once

namespace Config
{
    inline bool bHealth = false;
    inline bool bStamina = false;
    inline bool bFullness = false;
    inline bool bHydration = false;
    inline bool bRest = false;
    inline bool bSpeed = false;
    inline bool bJump = false;
    inline bool bFallDamage = false;
    inline bool bGravity = false;
    inline bool bInfiniteAmmo = false;
    inline bool bGlider = false;
    inline bool bKnightV = false;
    inline bool bPistol = false;
    inline bool bRevolver = false;
    inline bool bShotgun = false;
    inline bool bFlashLight = false;
    inline bool bLighter = false;
    inline bool bRebreather = false;
    inline bool bRopeGun = false;
    inline bool bSunControl = false;
    inline bool bWindControl = false;
    inline bool bDayTime = false;
    inline bool bInstantBuild = false;
    inline bool bTreeRegrow = false;

    inline bool bHealthSP = false;
    inline bool bStaminaSP = false;

    namespace MethodToggleCall
    {
        inline bool DebugAddCharacter = false;
        inline bool DebugAddDefinedCharacter = false;
        inline bool SetSeason = false;
        inline bool SpawnItem = false;

        namespace Value
        {
            namespace DebugAddCharacter
            {
                inline std::string character = "";
                inline std::string entity = "";
            }

            namespace SetSeason
            {
                inline std::string season = "";
            }

            namespace SpawnItem
            {
                inline std::string itemTxt = "";
            }
        }
    }

    namespace Value
    {
        inline float fullness = 100.0f;
        inline float hydration = 100.0f;
        inline float rest = 100.0f;
        inline float runSpeed = 5.4f;
        inline float swimSpeedMultiplier = 4.0f;
        inline float jumpHeight = 2.6f;

        namespace Glider
        {
            const inline float Def_constantForwardForce = 60.0f;
            inline float _constantForwardForce = 60.0f;
            inline bool noDownForce = false;
        }

        namespace KnightV
        {
            const inline float DefJumpForce = 4.0f;
            const inline float DefMaxVelocity = 20.0f;
            const inline float DefMinMaxLean = 8.0f;
            inline float JumpForce = 4.0f;
            inline float MaxVelocity = 20.0f;
            inline float MinMaxLean = 8.0f;
        }

        namespace Pistol
        {
            inline bool IsPistolEquipped = false;
            inline bool RapidFire = false;
        }

        namespace Revolver
        {
            inline bool IsRevolverEquipped = false;
            inline bool RapidFire = false;

            inline void* FireWeapon; // method
        }

        namespace Shotgun
        {
            inline bool IsShotgunEquipped = false;
            inline bool RapidFire = false;

            inline void* FireWeapon; // method
        }

        namespace FlashLight
        {
            inline bool IsFlashLightEquipped = false;
            inline float _maxLightIntensity = 16.5f;
            inline bool NoDrain = false;
        }

        namespace Lighter
        {
            inline bool IsLighterEquipped = false;
            inline float intensity = 512.0f;
            inline bool IncRange = false;
        }

        namespace Rebreather
        {
            inline bool IsRebreatherEquipped = false;
            inline float _airConsumptionRate = 2.0f;
            inline float _maxLightIntensity = 18.5f;
            inline bool InfOxigen = false;
        }

        namespace RopeGun
        {
            inline bool IsRopeGunEquipped = false;
            inline float _maxFiringRange = 50.0f;
            inline float _maxRopeLength = 150.0f;
            inline bool InfLength = false;
        }

        namespace SunControl
        {
            inline float Intensity = 100000.0f;
        }

        namespace WindControl
        {
            inline float Intensity = 1.0f;
        }

        namespace DayTimeControl
        {
            inline float _baseSpeedMultiplier = 1.0f;
        }

        namespace TreeRegrow
        {
            inline float _regrowthFactor = 0.1f;
        }
    }

    namespace Offsets
    {
        inline uintptr_t ApplyDamage = 0x338FB70;
        inline uintptr_t AdjustStamina = 0x338FEF0;
        inline uintptr_t RemoveAmmo = 0x365E7A0;
        inline uintptr_t GrabCheats_CheatsBridge_o = 0x59F160;
        inline uintptr_t GodMode = 0x373CAA0;
        inline uintptr_t InfiniteStamina = 0x373CB00;
    }

    namespace Items
    {
        inline int itemID{};
        inline int itemQTY{};

        inline const char* itemText{};

        enum ItemsID
        {
            PrintedArrow = 618,
            AirTank = 469,
            AlbumCover = 467,
            AloeVera = 451,
            AloreVeraSeed = 596,
            AnimalHide = 472,
            Arm = 480,
            ArrowLeaf = 454,
            ArrowLeafSeed = 597,
            Batteries = 527,
            Binocular = 341,
            Blackberries = 595,
            BlackberrySeed = 598,
            Blazer = 491,
            BlueTshirt = 488,
            Blueberries = 445,
            BlueberrySeed = 599,
            Bone = 405,
            BoneArmor = 494,
            ShotgunBuckshotAmmo = 364,
            ShotgunSlugAmmo = 363,
            CamouflageSuit = 558,
            C4Brick = 420,
            CannedFoods = 434,
            CanOpener = 432,
            CarbonFibreArrow = 373,
            Cash = 496,
            CatFood = 464,
            Cereal = 425,
            Chicory = 465,
            ChicorySeed = 605,
            CircuitBoard = 416,
            Cloth = 415,
            Coins = 502,
            CompoundBow = 360,
            CraftedBow = 443,
            CraftedClub = 477,
            CraftedSpear = 474,
            CreepyArmor = 593,
            Cross = 468,
            Crossbow = 365,
            CrossbowBolt = 368,
            DeepSleepBook = 424,
            DevilsClub = 449,
            DevilsClubSeed = 600,
            DuctTape = 419,
            ElectricChainsaw = 394,
            Snack = 441,
            Soda = 439,
            EnergyMix = 461,
            EnergyMixPlus = 462,
            Feather = 479,
            FirefighterAxe = 431,
            Fireweed = 453,
            FireweedSeed = 601,
            Fish = 436,
            Flare = 440,
            Flashlight = 471,
            FlashlightAttachment = 378,
            Flippers = 463,
            FlyAmanita = 400,
            FragGrenade = 381,
            GoldMask = 435,
            GoldenArmor = 572,
            GolfBall = 524,
            GpsLocator = 529,
            GpsTracker = 412,
            GuaranaBerries = 594,
            GuaranaSeed = 602,
            GuestKeyCard = 567,
            GuideBook = 589,
            Guitar = 340,
            Head = 482,
            HealthMix = 455,
            HealthMixPlus = 456,
            HideArmor = 519,
            Hoodie = 490,
            Horsetail = 450,
            HorsetailSeed = 603,
            HidnumRepandum = 399,
            Katana = 367,
            KingOyster = 398,
            LaserSight = 375,
            Leaf = 484,
            LeafArmor = 473,
            LeatherJacket = 493,
            Leg = 481,
            Lightbulb = 635,
            Machete = 359,
            MaintenanceKeyCard = 566,
            Meds = 437,
            ModernAxe = 356,
            Molotov = 388,
            MREPack = 438,
            NightVisionGoggles = 354,
            Oyster = 466,
            Pajamas = 487,
            PaperTarget = 518,
            PennantLineBook = 423,
            Pistol = 355,
            PistolAmmo = 362,
            PistolRail = 376,
            PistolSilencer = 374,
            PrintedFlask = 426,
            PrintedGrapplingHook = 560,
            PrintedResin = 390,
            PrintedSled = 428,
            Putter = 525,
            Radio = 590,
            RamenNoodles = 421,
            RawMeat = 433,
            Rebreather = 444,
            RedMask = 391,
            RepairTool = 422,
            Revolver = 386,
            Rifle = 361,
            Rock = 393,
            Rope = 403,
            RopeGun = 522,
            SalmonBerries = 447,
            SalmonberrySeed = 604,
            Scope = 377,
            Shiitake = 397,
            Shotgun = 358,
            ShotgunRail = 346,
            Shovel = 485,
            SkinPouch = 508,
            Skull = 430,
            Slingshot = 459,
            SmallRocks = 476,
            Snowberries = 448,
            SolarPanel = 634,
            SteakBite = 570,
            Stick = 392,
            StoneArrow = 507,
            StunBaton = 396,
            StunGunAmmo = 369,
            Swimsuit = 619,
            Tarp = 504,
            Taser = 353,
            TechArmor = 554,
            TechMesh = 553,
            TimedBomb = 417,
            Torch = 503,
            TurtleEgg = 401,
            TurtleShell = 506,
            Twinberries = 446,
            VIPKeyCard = 568,
            Watch = 410,
            Wetsuit = 499,
            WinterJacket = 500,
            Wire = 418,
            Yarrow = 452,
            YarrowSeed = 606,
            ZiplineRope = 523
        };
    }

    namespace SeasonsManager
    {
        enum Season
        {
            Spring  = 0,
            Summer  = 1,
            Fall    = 2,
            Winter  = 3
        };
    }

    namespace Lobby
    {
        inline bool gotLobbyInfo = false;
        inline int PlayerLimit{20};
    }
}