#include "Coin.h"
#include "Plant.h"
#include "Board.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "GridItem.h"
#include "ZenGarden.h"
#include "Challenge.h"
#include "Projectile.h"
#include "SeedPacket.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../GameConstants.h"
#include "System/PlayerInfo.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../Sexy.TodLib/TodStringFile.h"

PlantDefinition gPlantDefs[SeedType::NUM_SEED_TYPES] = {  
    { SeedType::SEED_PEASHOOTER,        nullptr, ReanimationType::REANIM_PEASHOOTER,    0, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_SUNFLOWER,         nullptr, ReanimationType::REANIM_SUNFLOWER,     1,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER") },
    { SeedType::SEED_CHERRYBOMB,        nullptr, ReanimationType::REANIM_CHERRYBOMB,    3,  150,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHERRY_BOMB") },
    { SeedType::SEED_WALLNUT,           nullptr, ReanimationType::REANIM_WALLNUT,       2,  50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("WALL_NUT") },
    { SeedType::SEED_POTATOMINE,        nullptr, ReanimationType::REANIM_POTATOMINE,    37, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("POTATO_MINE") },
    { SeedType::SEED_SNOWPEA,           nullptr, ReanimationType::REANIM_SNOWPEA,       4,  150,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SNOW_PEA") },
    { SeedType::SEED_CHOMPER,           nullptr, ReanimationType::REANIM_CHOMPER,       31, 250,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("CHOMPER") },
    { SeedType::SEED_REPEATER,          nullptr, ReanimationType::REANIM_REPEATER,      5,  250,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("REPEATER") },
    { SeedType::SEED_PUFFSHROOM,        nullptr, ReanimationType::REANIM_PUFFSHROOM,    6,  0,      750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PUFF_SHROOM") },
    { SeedType::SEED_SUNSHROOM,         nullptr, ReanimationType::REANIM_SUNSHROOM,     7,  25,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUN_SHROOM") },
    { SeedType::SEED_FUMESHROOM,        nullptr, ReanimationType::REANIM_FUMESHROOM,    9,  125,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("FUME_SHROOM") },
    { SeedType::SEED_GRAVEBUSTER,       nullptr, ReanimationType::REANIM_GRAVE_BUSTER,  40, 50,     1500,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GRAVE_BUSTER") },
    { SeedType::SEED_HYPNOSHROOM,       nullptr, ReanimationType::REANIM_HYPNOSHROOM,   10, 100,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("HYPNO_SHROOM") },
    { SeedType::SEED_SCAREDYSHROOM,     nullptr, ReanimationType::REANIM_SCRAREYSHROOM, 33, 25,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SCAREDY_SHROOM") },
    { SeedType::SEED_ICESHROOM,         nullptr, ReanimationType::REANIM_ICESHROOM,     36, 125,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("ICE_SHROOM") },
    { SeedType::SEED_DOOMSHROOM,        nullptr, ReanimationType::REANIM_DOOMSHROOM,    20, 325,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("DOOM_SHROOM") },
    { SeedType::SEED_LILYPAD,           nullptr, ReanimationType::REANIM_LILYPAD,       19, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("LILY_PAD") },
    { SeedType::SEED_SQUASH,            nullptr, ReanimationType::REANIM_SQUASH,        21, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SQUASH") },
    { SeedType::SEED_THREEPEATER,       nullptr, ReanimationType::REANIM_THREEPEATER,   12, 250,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("THREEPEATER") },
    { SeedType::SEED_TANGLEKELP,        nullptr, ReanimationType::REANIM_TANGLEKELP,    17, 75,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TANGLE_KELP") },
    { SeedType::SEED_JALAPENO,          nullptr, ReanimationType::REANIM_JALAPENO,      11, 125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("JALAPENO") },
    { SeedType::SEED_SPIKEWEED,         nullptr, ReanimationType::REANIM_SPIKEWEED,     22, 150,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEWEED") },
    { SeedType::SEED_TORCHWOOD,         nullptr, ReanimationType::REANIM_TORCHWOOD,     29, 275,    3000,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TORCHWOOD") },
    { SeedType::SEED_TALLNUT,           nullptr, ReanimationType::REANIM_TALLNUT,       28, 125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TALL_NUT") },
    { SeedType::SEED_SEASHROOM,         nullptr, ReanimationType::REANIM_SEASHROOM,     39, 0,      3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SEA_SHROOM") },
    { SeedType::SEED_PLANTERN,          nullptr, ReanimationType::REANIM_PLANTERN,      38, 25,     3000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("PLANTERN") },
    { SeedType::SEED_CACTUS,            nullptr, ReanimationType::REANIM_CACTUS,        15, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CACTUS") },
    { SeedType::SEED_BLOVER,            nullptr, ReanimationType::REANIM_BLOVER,        18, 175,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("BLOVER") },
    { SeedType::SEED_SPLITPEA,          nullptr, ReanimationType::REANIM_SPLITPEA,      32, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SPLIT_PEA") },
    { SeedType::SEED_STARFRUIT,         nullptr, ReanimationType::REANIM_STARFRUIT,     30, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("STARFRUIT") },
    { SeedType::SEED_PUMPKINSHELL,      nullptr, ReanimationType::REANIM_PUMPKIN,       25, 125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("PUMPKIN") },
    { SeedType::SEED_MAGNETSHROOM,      nullptr, ReanimationType::REANIM_MAGNETSHROOM,  35, 100,    750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MAGNET_SHROOM") },
    { SeedType::SEED_CABBAGEPULT,       nullptr, ReanimationType::REANIM_CABBAGEPULT,   13, 100,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("CABBAGE_PULT") },
    { SeedType::SEED_FLOWERPOT,         nullptr, ReanimationType::REANIM_FLOWER_POT,    33, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("FLOWER_POT") },
    { SeedType::SEED_KERNELPULT,        nullptr, ReanimationType::REANIM_KERNELPULT,    13, 125,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("KERNEL_PULT") },
    { SeedType::SEED_INSTANT_COFFEE,    nullptr, ReanimationType::REANIM_COFFEEBEAN,    33, 25,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("COFFEE_BEAN") },
    { SeedType::SEED_GARLIC,            nullptr, ReanimationType::REANIM_GARLIC,        8,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GARLIC") },
    { SeedType::SEED_UMBRELLA,          nullptr, ReanimationType::REANIM_UMBRELLALEAF,  23, 450,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("UMBRELLA_LEAF") },
    { SeedType::SEED_MARIGOLD,          nullptr, ReanimationType::REANIM_MARIGOLD,      24, 50,     3000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("MARIGOLD") },
    { SeedType::SEED_MELONPULT,         nullptr, ReanimationType::REANIM_MELONPULT,     14, 300,    750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("MELON_PULT") },
    { SeedType::SEED_PEPPERPULT,        nullptr, ReanimationType::REANIM_23,            0, 200,    1000,   PlantSubClass::SUBCLASS_SHOOTER,    200,    _S("ICE_CABBAGE") },
    { SeedType::SEED_MOONFLOWER,        nullptr, ReanimationType::REANIM_22,            1,  100,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER") },
    { SeedType::SEED_MAGNIFYING_GRASS,  nullptr, ReanimationType::REANIM_8,             0, 75,     750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MAGNIFYING_GRASS") },
    { SeedType::SEED_TOADSTOOL,         nullptr, ReanimationType::REANIM_3,             0, 200,      750,   PlantSubClass::SUBCLASS_NORMAL, 0, _S("TOADSTOOL") },
    { SeedType::SEED_CUCKUMBER,         nullptr, ReanimationType::REANIM_1,             0, 125,   2500,   PlantSubClass::SUBCLASS_SHOOTER,    200,      _S("COCONUT") },
    { SeedType::SEED_GUARDIAN_SHROOM,   nullptr, ReanimationType::REANIM_13,            44, 225,   5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GUARDIAN_SHROOM") },
    { SeedType::SEED_SAKURA,            nullptr, ReanimationType::REANIM_10,            0, 500,   3000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SAKURA") },
    { SeedType::SEED_SPY_PEPPER,        nullptr, ReanimationType::REANIM_24,      0, 150,   3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPY_PEPPER") },
    { SeedType::SEED_GATLINGPEA,        nullptr, ReanimationType::REANIM_GATLINGPEA,    5,  450,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("GATLING_PEA") },
    { SeedType::SEED_TWINSUNFLOWER,     nullptr, ReanimationType::REANIM_TWIN_SUNFLOWER,1,  125,    5000,   PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("TWIN_SUNFLOWER") },
    { SeedType::SEED_GLOOMSHROOM,       nullptr, ReanimationType::REANIM_GLOOMSHROOM,   27, 275,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    200,    _S("GLOOM_SHROOM") },
    { SeedType::SEED_CATTAIL,           nullptr, ReanimationType::REANIM_CATTAIL,       27, 350,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("CATTAIL") },
    { SeedType::SEED_WINTERMELON,       nullptr, ReanimationType::REANIM_WINTER_MELON,  27, 600,    5000,   PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("WINTER_MELON") },
    { SeedType::SEED_GOLD_MAGNET,       nullptr, ReanimationType::REANIM_GOLD_MAGNET,   27, 50,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GOLD_MAGNET") },
    { SeedType::SEED_SPIKEROCK,         nullptr, ReanimationType::REANIM_SPIKEROCK,     27, 500,    5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPIKEROCK") },
    { SeedType::SEED_COBCANNON,         nullptr, ReanimationType::REANIM_COBCANNON,     16, 1000,    5000,  PlantSubClass::SUBCLASS_NORMAL,     600,    _S("COB_CANNON") },
    { SeedType::SEED_SUNPEA,        nullptr, ReanimationType::REANIM_25,    0, 225,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_GOOPEA,        nullptr, ReanimationType::REANIM_26,    0, 150,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_ROCKPEA,        nullptr, ReanimationType::REANIM_27,    0, 275,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_CPEA,        nullptr, ReanimationType::REANIM_28,    0, 250,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_DROPPEA,        nullptr, ReanimationType::REANIM_29,    0, 150,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("PEASHOOTER") },
    { SeedType::SEED_PRIMALPOTATOMINE,         nullptr, ReanimationType::REANIM_30,     1,  50,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER") },
    { SeedType::SEED_PRIMALSUNFLOWER,         nullptr, ReanimationType::REANIM_31,     1,  100,     750,    PlantSubClass::SUBCLASS_NORMAL,     2500,   _S("SUNFLOWER") },
    { SeedType::SEED_IMITATER,          nullptr, ReanimationType::REANIM_IMITATER,      33, 0,      750,    PlantSubClass::SUBCLASS_NORMAL,     0,      _S("IMITATER") },
    { SeedType::SEED_GIANT_WALLNUT,     nullptr, ReanimationType::REANIM_WALLNUT,       2,  0,      3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("GIANT_WALLNUT") },
    { SeedType::SEED_SPROUT,            nullptr, ReanimationType::REANIM_ZENGARDEN_SPROUT,          33, 0,      3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SPROUT") },
    { SeedType::SEED_LEFTPEATER,        nullptr, ReanimationType::REANIM_REPEATER,      5,  200,    750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("REPEATER") },
    { SeedType::SEED_PLASMAWOOD,        nullptr, ReanimationType::REANIM_PLASMAWOOD,      29,  2500,    7500,    PlantSubClass::SUBCLASS_NORMAL,   0,    _S("TORCHWOOD") },
    { SeedType::SEED_ACID_LEMON,        nullptr, ReanimationType::REANIM_ACID_LEMON,     0, 200,     3000,   PlantSubClass::SUBCLASS_SHOOTER,    1000,    _S("ACID_LEMON") },
    { SeedType::SEED_POISONSHROOM,      nullptr, ReanimationType::REANIM_9, 0, 50, 3000, PlantSubClass::SUBCLASS_NORMAL, 0, _S("POISON_SHROOM") },
    { SeedType::SEED_BEEHIVE,           nullptr, ReanimationType::REANIM_4,              0, 525, 2500, PlantSubClass::SUBCLASS_NORMAL, 0, _S("BEEHIVE") },
    { SeedType::SEED_TRAMPALINE,        nullptr, ReanimationType::REANIM_2,             0, 125,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TRAMPALINE") },
    { SeedType::SEED_EXPLODE_O_NUT,     nullptr, ReanimationType::REANIM_6,       44,  125,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("EXPLODE_O_NUT") },
    { SeedType::SEED_TIME_APPLE,        nullptr, ReanimationType::REANIM_11,    0, 100,     3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("TIME_APPLE") },
    { SeedType::SEED_MINI_GUARDIAN_SHROOM,nullptr, ReanimationType::REANIM_16,           0, 0,     5000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("MINI_GUARDIAN_SHROOM") },
    { SeedType::SEED_SUN_BAN,           nullptr, ReanimationType::REANIM_14,             0, 0,     1500,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUN_BAN") },
    { SeedType::SEED_SPORESHROOM,       nullptr, ReanimationType::REANIM_5,    45,  25,     750,    PlantSubClass::SUBCLASS_SHOOTER,    150,    _S("SPORE_SHROOM") },
    { SeedType::SEED_HAMMER_SHROOM,     nullptr, ReanimationType::REANIM_12,    0, 75,    1500,   PlantSubClass::SUBCLASS_NORMAL,     150,    _S("HAMMER_SHROOM") },
    { SeedType::SEED_XSHROOM,          nullptr, ReanimationType::REANIM_15,             0, 575,   3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("X_SHROOM") },
    { SeedType::SEED_SUN_MAGNET,        nullptr, ReanimationType::REANIM_20,    0, 75,    3000,   PlantSubClass::SUBCLASS_NORMAL,     0,      _S("SUN_MAGNET") },
    { SeedType::SEED_SNAPDRAGON,        nullptr, ReanimationType::REANIM_21,    0, 150,   750,    PlantSubClass::SUBCLASS_SHOOTER,    300,    _S("SNAPDRAGON") }
  
};

Plant::Plant()
{
}

void Plant::PlantInitialize(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType)
{
    mPlantCol = theGridX;
    mRow = theGridY;
    if (mBoard)
    {
        mX = mBoard->GridToPixelX(theGridX, theGridY);
        mY = mBoard->GridToPixelY(theGridX, theGridY);
    }
    mAnimCounter = 0;
    mAnimPing = true;
    mFrame = 0;
    mShootingCounter = 0;
    mShakeOffsetX = 0.0f;
    mShakeOffsetY = 0.0f;
    mFrameLength = RandRangeInt(12, 18);
    mTargetX = -1;
    mTargetY = -1;
    mStartRow = mRow;
    mNumFrames = 5;
    mCoconutClickCount = 0;
    mState = PlantState::STATE_NOTREADY;
    mDead = false;
    mHasBooster = false;
    mIsFlipped = false;
    mSquished = false;
    mSunMagnetActive = false;
    mSunMagnetAuraID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    for (int i = 0; i < 50; i++) {
        mTrackedZombieHealth[i] = 0;
        mTrackedZombieID[i] = ZOMBIEID_NULL;
    }
    mSeedType = theSeedType;
    mImitaterType = theImitaterType;
    mPlantHealth = 300;
    mBoostGlowParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mDoSpecialCountdown = 0;
    mDisappearCountdown = 200;
    mStateCountdown = 0;
    mParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mBodyReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID2 = ReanimationID::REANIMATIONID_NULL;
    mHeadReanimID3 = ReanimationID::REANIMATIONID_NULL;
    mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;
    mLightReanimID = ReanimationID::REANIMATIONID_NULL;
    mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
    mTrampolineReversed = false;
    mBlinkCountdown = 0;
    mRecentlyEatenCountdown = 0;
    mEatenFlashCountdown = 0;
    mBoostTimer = 500;
    mBoostFireTimer = 0;
    mBeghouledFlashCountdown = 0;
    mWidth = 80;
    mHeight = 80;
    memset(mMagnetItems, 0, sizeof(mMagnetItems));
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    mIsAsleep = false;
    mWakeUpCounter = 0;
    mOnBungeeState = PlantOnBungeeState::NOT_ON_BUNGEE;
    mPottedPlantIndex = -1;
    mLaunchRate = aPlantDef.mLaunchRate;
    mSubclass = aPlantDef.mSubClass;
    mRenderOrder = CalcRenderOrder();

    Reanimation* aBodyReanim = nullptr;
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
        aBodyReanim = mApp->AddReanimation(0.0f, aOffsetY, mRenderOrder + 1, aPlantDef.mReanimationType);
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);

        if (aBodyReanim->TrackExists("anim_idle"))
            aBodyReanim->SetFramesForLayer("anim_idle");

        if (mApp->IsWallnutBowlingLevel() && aBodyReanim->TrackExists("_ground"))
        {
            aBodyReanim->SetFramesForLayer("_ground");
            if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT)
                aBodyReanim->mAnimRate = RandRangeFloat(12.0f, 18.0f);
            else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 10.0f);
        }

        aBodyReanim->mIsAttachment = true;
        mBodyReanimID = mApp->ReanimationGetID(aBodyReanim);
        mBlinkCountdown = 400 + Sexy::Rand(400);
    }

    if (mBoard) {
        bool isNight = mBoard->mCurrentlyNight;
        if (IsNocturnal(mSeedType) && !isNight) SetSleeping(true);
        if ((mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_PRIMALSUNFLOWER) && isNight) SetSleeping(true);
    }

    if (mLaunchRate > 0)
    {
        if (MakesSun())
            mLaunchCounter = RandRangeInt(300, mLaunchRate / 2);
        else
            mLaunchCounter = RandRangeInt(0, mLaunchRate);
    }
    else
        mLaunchCounter = 0;

    switch (theSeedType)
    {
    case SeedType::SEED_BLOVER:
    {
        mDoSpecialCountdown = 50;

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_blow");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aBodyReanim->mAnimRate = 20.0f;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_idle");
            aBodyReanim->mAnimRate = 10.0f;
        }

        break;
    }
    case SeedType::SEED_TOADSTOOL:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_TIME_APPLE:
        mState = PlantState::STATE_DOINGSPECIAL;
        mStateCountdown = 500;
        break;
    case SeedType::SEED_BEEHIVE:
        mState = PlantState::STATE_BEEHIVE_IDLE;
        mBeePhase = 1;
        break;
    case SeedType::SEED_TRAMPALINE:
        mTrampalineCharges = 3;
        mTrampalineHiding = false;
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_SPY_PEPPER:
        mState = PlantState::STATE_NOTREADY;
        mStateCountdown = 50;
        break;
    case SeedType::SEED_GUARDIAN_SHROOM:
        mPlantHealth = 4000;
        mStateCountdown = 3000;
        break;
    case SeedType::SEED_MINI_GUARDIAN_SHROOM:
        mPlantHealth = 1000;
        break;
    case SeedType::SEED_HAMMER_SHROOM:
        mStateCountdown = 150;
        break;
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_GATLINGPEA:
    case SeedType::SEED_SUNPEA:
    case SeedType::SEED_GOOPEA:
    case SeedType::SEED_ROCKPEA:
    case SeedType::SEED_CPEA:
    case SeedType::SEED_DROPPEA:
        if (aBodyReanim)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
            Reanimation* aHeadReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            mHeadReanimID = mApp->ReanimationGetID(aHeadReanim);

            if (aBodyReanim->TrackExists("anim_stem"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
            else if (aBodyReanim->TrackExists("anim_idle"))
                aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        }
        break;
    case SeedType::SEED_SPLITPEA:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        break;
    }
    case SeedType::SEED_THREEPEATER:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        Reanimation* aHeadReanim1 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim1->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim1->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim1->SetFramesForLayer("anim_head_idle1");
        aHeadReanim1->AttachToAnotherReanimation(aBodyReanim, "anim_head1");
        mHeadReanimID = mApp->ReanimationGetID(aHeadReanim1);

        Reanimation* aHeadReanim2 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->SetFramesForLayer("anim_head_idle2");
        aHeadReanim2->AttachToAnotherReanimation(aBodyReanim, "anim_head2");
        mHeadReanimID2 = mApp->ReanimationGetID(aHeadReanim2);

        Reanimation* aHeadReanim3 = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, aPlantDef.mReanimationType);
        aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->SetFramesForLayer("anim_head_idle3");
        aHeadReanim3->AttachToAnotherReanimation(aBodyReanim, "anim_head3");
        mHeadReanimID3 = mApp->ReanimationGetID(aHeadReanim3);

        break;
    }
    case SeedType::SEED_WALLNUT:
        mPlantHealth = 4000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_EXPLODE_O_NUT:
        mPlantHealth = 4000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GIANT_WALLNUT:
        mPlantHealth = 4000;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_TALLNUT:
        mPlantHealth = 8000;
        mHeight = 80;
        mBlinkCountdown = 1000 + Sexy::Rand(1000);
        break;
    case SeedType::SEED_GARLIC:
        TOD_ASSERT(aBodyReanim);
        mPlantHealth = 400;
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_GOLD_MAGNET:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_IMITATER:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(25.0f, 30.0f);
        mStateCountdown = 200;
        break;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            mDoSpecialCountdown = 100;

            aBodyReanim->SetFramesForLayer("anim_explode");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
        }

        break;
    }
    case SeedType::SEED_CUCKUMBER:
        mPlantHealth = 2000;
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_PRIMALPOTATOMINE:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mAnimRate = 12.0f;

        if (IsInPlay())
        {
            aBodyReanim->AssignRenderGroupToTrack("anim_glow", RENDER_GROUP_HIDDEN);
            mStateCountdown = 1500;
        }
        else
        {
            aBodyReanim->SetFramesForLayer("anim_armed");
            mState = PlantState::STATE_POTATO_ARMED;
        }

        break;
    }
    case SeedType::SEED_GRAVEBUSTER:
    {
        TOD_ASSERT(aBodyReanim);

        if (IsInPlay())
        {
            aBodyReanim->SetFramesForLayer("anim_land");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;

            mState = PlantState::STATE_GRAVEBUSTER_LANDING;
            mApp->PlayFoley(FoleyType::FOLEY_GRAVEBUSTERCHOMP);
        }

        break;
    }
    case SeedType::SEED_SUNSHROOM:
    {
        TOD_ASSERT(aBodyReanim);

        aBodyReanim->mFrameBasePose = 6;

        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(10) - 5;
        }
        else if (mIsAsleep)
            aBodyReanim->SetFramesForLayer("anim_bigsleep");
        else
            aBodyReanim->SetFramesForLayer("anim_bigidle");

        mState = PlantState::STATE_SUNSHROOM_SMALL;
        mStateCountdown = 12000;

        break;
    }
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_SPORESHROOM:
        if (IsInPlay())
        {
            mX += Sexy::Rand(10) - 5;
            mY += Sexy::Rand(6) - 3;
        }
        break;
    case SeedType::SEED_PUMPKINSHELL:
    {
        mPlantHealth = 4000;
        mWidth = 120;

        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToTrack("Pumpkin_back", 1);
        break;
    }
    case SeedType::SEED_CHOMPER:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_PLANTERN:
    {
        mStateCountdown = 50;

        if (!IsOnBoard() || mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            AddAttachedParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_FOG + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
        }
        if (IsInPlay())
        {
            mApp->PlaySample(Sexy::SOUND_PLANTERN);
        }

        break;
    }
    case SeedType::SEED_TORCHWOOD:
        break;
    case SeedType::SEED_PLASMAWOOD:
        break;
    case SeedType::SEED_MARIGOLD:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->mAnimRate = RandRangeFloat(15.0f, 20.0f);
        break;
    case SeedType::SEED_CACTUS:
        mState = PlantState::STATE_CACTUS_LOW;
        break;
    case SeedType::SEED_INSTANT_COFFEE:
        mDoSpecialCountdown = 100;
        break;
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_POISONSHROOM:
    case SeedType::SEED_SNAPDRAGON:
        mState = PlantState::STATE_READY;
        break;
    case SeedType::SEED_COBCANNON:
        if (IsInPlay())
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 500;

            TOD_ASSERT(aBodyReanim);
            aBodyReanim->SetFramesForLayer("anim_unarmed_idle");
        }
        break;
    case SeedType::SEED_KERNELPULT:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
        break;
    case SeedType::SEED_MAGNETSHROOM:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
        break;
    case SeedType::SEED_SPIKEROCK:
        mPlantHealth = 450;
        TOD_ASSERT(aBodyReanim);
        break;
    case SeedType::SEED_SPROUT:
        break;
    case SeedType::SEED_FLOWERPOT:
        if (IsInPlay())
        {
            mState = PlantState::STATE_FLOWERPOT_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_LILYPAD:
        if (IsInPlay())
        {
            mState = PlantState::STATE_LILYPAD_INVULNERABLE;
            mStateCountdown = 100;
        }
        break;
    case SeedType::SEED_TANGLEKELP:
        TOD_ASSERT(aBodyReanim);
        aBodyReanim->SetTruncateDisappearingFrames();
    }

    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (theSeedType == SeedType::SEED_WALLNUT || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_MARIGOLD))
    {
        mPlantHealth *= 2;
    }
    mPlantMaxHealth = mPlantHealth;

    if (mSeedType != SeedType::SEED_FLOWERPOT && IsOnBoard())
    {
        TOD_ASSERT(mBoard);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot)
            mApp->ReanimationGet(aFlowerPot->mBodyReanimID)->mAnimRate = 0.0f;
    }
}
int Plant::CalcRenderOrder()
{
    PLANT_ORDER anOrder = PLANT_ORDER::PLANT_ORDER_NORMAL;
    RenderLayer aLayer = RenderLayer::RENDER_LAYER_PLANT;

    SeedType aSeedType = mSeedType;
    if (mSeedType == SeedType::SEED_IMITATER && mImitaterType != SeedType::SEED_NONE)
        aSeedType = mImitaterType;

    if (mApp->IsWallnutBowlingLevel())
    {
        aLayer = RenderLayer::RENDER_LAYER_PROJECTILE;
    }
    else if (aSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_PUMPKIN;
    }
    else if (IsFlying(aSeedType))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_FLYER;
    }
    else if (aSeedType == SeedType::SEED_FLOWERPOT || (aSeedType == SeedType::SEED_LILYPAD && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN))
    {
        anOrder = PLANT_ORDER::PLANT_ORDER_LILYPAD;
    }

    int potsBelow = 0;
    int samePlantsBelow = 0;

    if (mBoard)
    {
        Plant* p = nullptr;
        while (mBoard->IteratePlants(p))
        {
            if (p->mPlantCol == mPlantCol && p->mRow == mRow && !p->mDead && p != this)
            {
                if (p->mSeedType == SeedType::SEED_FLOWERPOT && mSeedType == SeedType::SEED_FLOWERPOT)
                {
                    if (mBoard->mPlants.DataArrayGetID(p) < mBoard->mPlants.DataArrayGetID(this)) potsBelow++;
                }
                else if (p->mSeedType == mSeedType && (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_SPORESHROOM))
                {
                    if (mBoard->mPlants.DataArrayGetID(p) < mBoard->mPlants.DataArrayGetID(this)) samePlantsBelow++;
                }
            }
        }
    }
    return Board::MakeRenderOrder(aLayer, mRow, anOrder * 10 - mX + 800 + potsBelow * 2 + samePlantsBelow * 2);
}

void Plant::SetSleeping(bool theIsAsleep)
{
    if (mIsAsleep == theIsAsleep || NotOnGround())
        return;
    mIsAsleep = theIsAsleep;
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (theIsAsleep)
    {
        if ((mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER) &&
            aBodyReanim && aBodyReanim->TrackExists("anim_yawn"))
        {
            PlayBodyReanim("anim_yawn", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
            mStateCountdown = -1;
            return;
        }
        float aPosX = mX + 50.0f;
        float aPosY = mY + 40.0f;
        if (mSeedType == SeedType::SEED_FUMESHROOM)
            aPosX += 12.0f;
        else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
            aPosY -= 20.0f;
        else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            aPosY -= 12.0f;
        Reanimation* aSleepReanim = mApp->AddReanimation(aPosX, aPosY, mRenderOrder + 2, ReanimationType::REANIM_SLEEPING);
        if (aSleepReanim)
        {
            aSleepReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aSleepReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
            aSleepReanim->mAnimTime = RandRangeFloat(0.0f, 0.9f);
            mSleepingReanimID = mApp->ReanimationGetID(aSleepReanim);
        }
        if (aBodyReanim)
        {
            if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
            {
                aBodyReanim->SetFramesForLayer("anim_bigsleep");
                aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
            }
            else if (aBodyReanim->TrackExists("anim_sleep"))
            {
                PlayBodyReanim("anim_sleep", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(6.0f, 8.0f));
            }
            else
            {
                aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
                aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            }
        }

        EndBlink();
    }
    else
    {
        mApp->RemoveReanimation(mSleepingReanimID);
        mSleepingReanimID = ReanimationID::REANIMATIONID_NULL;
        if (aBodyReanim == nullptr)
            return;
        if (mStateCountdown == -1)
        {
            mStateCountdown = 0;
        }
        if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
        {
            aBodyReanim->SetFramesForLayer("anim_bigidle");
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
        else if (aBodyReanim->TrackExists("anim_idle"))
        {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
        }
        else
        {
            if (aBodyReanim->mAnimRate < 2.0f)
                aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
            aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
        }
    }
}

int Plant::GetDamageRangeFlags(PlantWeapon thePlantWeapon)
{
    switch (mSeedType)
    {
    case SeedType::SEED_CACTUS:
        return thePlantWeapon == PlantWeapon::WEAPON_SECONDARY ? 1 : 2;
    case SeedType::SEED_CHERRYBOMB:
    case SeedType::SEED_JALAPENO:
    case SeedType::SEED_COBCANNON:
    case SeedType::SEED_DOOMSHROOM:
    case SeedType::SEED_XSHROOM:
    case SeedType::SEED_CUCKUMBER:
        return 127;
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_KERNELPULT:
    case SeedType::SEED_SPORESHROOM:
    case SeedType::SEED_WINTERMELON:
    case SeedType::SEED_PEPPERPULT:
        return 13;
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_PRIMALPOTATOMINE:
        return 77;
    case SeedType::SEED_SQUASH:
        return 13;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_FUMESHROOM:
    case SeedType::SEED_GLOOMSHROOM:
    case SeedType::SEED_CHOMPER:
        return 9;
    case SeedType::SEED_CATTAIL:
        return 11;
    case SeedType::SEED_TANGLEKELP:
        return 5;
    case SeedType::SEED_GIANT_WALLNUT:
        return 17;
    default:
        return 1;
    }
}

bool Plant::IsOnHighGround()
{
    return mBoard && mBoard->mGridSquareType[mPlantCol][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
}

void Plant::SpikeRockTakeDamage()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    SpikeweedAttack();

    mPlantHealth -= 50;
    if (mPlantHealth <= 300)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike3", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 150)
    {
        aBodyReanim->AssignRenderGroupToTrack("bigspike2", RENDER_GROUP_HIDDEN);
    }
    if (mPlantHealth <= 0)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
        Die();
    }
}

bool Plant::IsSpiky()
{
    return mSeedType == SeedType::SEED_SPIKEWEED ||
        mSeedType == SeedType::SEED_SPIKEROCK ||
        mSeedType == SeedType::SEED_TRAMPALINE;
}

void Plant::DoRowAreaDamage(int theDamage, unsigned int theDamageFlags)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    // حلقة تكرار واحدة لكل الزومبي في الشاشة لمنع الكراش
    while (mBoard->IterateZombies(aZombie))
    {
        // تجاهل الزومبي الميت أو المنوم
        if (aZombie->IsDeadOrDying() || aZombie->mMindControlled)
            continue;

        // التحقق من توافق نوع ضرر النبات مع الزومبي
        if (!aZombie->EffectedByDamage(aDamageRangeFlags))
            continue;

        // ========================================================
        // 1. منطق الجلووم شرووم (Gloom-shroom) - عادي وبوست
        // ========================================================
        if (mSeedType == SeedType::SEED_GLOOMSHROOM)
        {
            bool aIsInsideGloomRange = false;

            if (mHasBooster)
            {
                // في حالة البوست: المدى ممتد 5x5 (صفين لأعلى وصفين لأسفل، وبمسافة أفقية 240 بكسل)
                int aRowDiff = abs(aZombie->mRow - mRow);
                int aPlantCenterX = mX + 40;
                int aZombieCenterX = aZombie->mX + 40;

                if (aRowDiff <= 2 && abs(aZombieCenterX - aPlantCenterX) <= 240)
                {
                    aIsInsideGloomRange = true;
                }
            }
            else
            {
                // في الحالة العادية: يعتمد على مستطيل الهجوم الطبيعي (3x3) المتداخل مع الزومبي
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                {
                    aIsInsideGloomRange = true;
                }
            }

            // إذا كان الزومبي داخل النطاق المحدد (سواء عادي أو بوست)
            if (aIsInsideGloomRange)
            {
                int aDamage = mHasBooster ? 300 : theDamage; // 10 أضعاف الدمج في البوست، وإلا الدمج العادي الاصلي

                aZombie->TakeDamage(aDamage, theDamageFlags);
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);

                // ميكانيكية الدفع تدريجياً 15 بكسل عند تفعيل البوست فقط
                if (mHasBooster && aZombie->mZombieType != ZombieType::ZOMBIE_BOSS && !aZombie->IsFlying())
                {
                    aZombie->mPosX += 15.0f;
                    aZombie->mX = (int)aZombie->mPosX;
                }
            }
            continue; // ننتقل للزومبي التالي
        }

        // ========================================================
        // 2. منطق باقي النباتات (بما فيها الفطر الدخاني Fume-shroom)
        // ========================================================
        int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
        if (aDiffY != 0)
            continue; // الفطر الدخاني وباقي النباتات تضرب في نفس الصف فقط

        if (aZombie->mOnHighGround == IsOnHighGround())
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
            {
                int aDamage = theDamage;

                // مضاعفة ضرر الفطر الدخاني 10 مرات في البوست
                if (mSeedType == SeedType::SEED_FUMESHROOM && mHasBooster)
                {
                    aDamage = 600;
                }

                // منطق الشرك الصخري والمزالج الاصلي للعبة
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_ZAMBONI || aZombie->mZombieType == ZombieType::ZOMBIE_CATAPULT) &&
                    (TestBit(theDamageFlags, DamageFlags::DAMAGE_SPIKE)))
                {
                    aDamage = 1800;

                    if (mSeedType == SeedType::SEED_SPIKEROCK)
                    {
                        SpikeRockTakeDamage();
                    }
                    else
                    {
                        Die();
                    }
                }

                aZombie->TakeDamage(aDamage, theDamageFlags);
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                if (mSeedType == SeedType::SEED_FUMESHROOM && mHasBooster)
                {
                    if (aZombie->mZombieType != ZombieType::ZOMBIE_BOSS && !aZombie->IsFlying())
                    {
                        aZombie->mPosX += 15.0f;
                        aZombie->mX = (int)aZombie->mPosX;
                    }
                }
            }
        }
    }
    // --- إضافة ضرر الاختراق للقبور (الفطر الدخاني والجلووم) ---
    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE)
        {
            int aDiffY = aGridItem->mGridY - mRow;
            bool aIsInsideGloomRange = false;

            if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            {
                if (mHasBooster)
                {
                    int aPlantCenterX = mX + 40;
                    int aGraveCenterX = mBoard->GridToPixelX(aGridItem->mGridX, aGridItem->mGridY) + 40;
                    if (abs(aDiffY) <= 2 && abs(aGraveCenterX - aPlantCenterX) <= 240)
                        aIsInsideGloomRange = true;
                }
                else
                {
                    if (abs(aDiffY) <= 1)
                    {
                        Rect aGridItemRect = aGridItem->GetPVZ2GraveRect();
                        if (GetRectOverlap(aAttackRect, aGridItemRect) > 0)
                            aIsInsideGloomRange = true;
                    }
                }

                if (aIsInsideGloomRange)
                {
                    int aDamage = mHasBooster ? 300 : theDamage;
                    aGridItem->TakeDamage(aDamage);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                }
            }
            else // الفطر الدخاني وبقية النباتات التي تستخدم هذه الدالة
            {
                if (aDiffY == 0)
                {
                    Rect aGridItemRect = aGridItem->GetPVZ2GraveRect();
                    if (GetRectOverlap(aAttackRect, aGridItemRect) > 0)
                    {
                        int aDamage = theDamage;
                        if (mSeedType == SeedType::SEED_FUMESHROOM && mHasBooster)
                            aDamage = 600; // مضاعفة الضرر في حالة البوست

                        aGridItem->TakeDamage(aDamage);
                        mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    }
                }
            }
        }
    }
}
TodParticleSystem* Plant::AddAttachedParticle(int thePosX, int thePosY, int theRenderPosition, ParticleEffect theEffect)
{
    TodParticleSystem* aParticle = mApp->ParticleTryToGet(mParticleID);
    if (aParticle)
        aParticle->ParticleSystemDie();

    TodParticleSystem* aNewParticle = mApp->AddTodParticle(thePosX, thePosY, theRenderPosition, theEffect);
    if (aNewParticle)
        mParticleID = mApp->ParticleGetID(aNewParticle);

    return aNewParticle;
}

bool Plant::FindTargetAndFire(int theRow, PlantWeapon thePlantWeapon)
{
    Zombie* aZombie = FindTargetZombie(theRow, thePlantWeapon);
    GridItem* aGrave = FindTargetPVZ2Grave(theRow, thePlantWeapon);
    if (aZombie == nullptr && aGrave == nullptr)
        return false; 
    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);

    if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        aHeadReanim2->StartBlend(20);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 35.0f;
        aHeadReanim2->SetFramesForLayer("anim_splitpea_shooting");
        aHeadReanim2->mAnimTime = 0.0f;
        mShootingCounter = 26;
    }
    else if (aHeadReanim && aHeadReanim->TrackExists("anim_shooting"))
    {
        aHeadReanim->StartBlend(20);
        aHeadReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim->mAnimRate = 35.0f;
        aHeadReanim->SetFramesForLayer("anim_shooting");
        aHeadReanim->mAnimTime = 0.0f;

        mShootingCounter = 33;
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_SPLITPEA || mSeedType == SeedType::SEED_LEFTPEATER)
        {
            aHeadReanim->mAnimRate = 45.0f;
            mShootingCounter = 26;
        }
        else if (mSeedType == SeedType::SEED_GATLINGPEA || mSeedType == SeedType::SEED_CPEA)
        {
            aHeadReanim->mAnimRate = 38.0f;
            mShootingCounter = 100;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        PlayBodyReanim("anim_shootinghigh", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
        mShootingCounter = 23;
    }
    else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 14.0f);
        mShootingCounter = 200;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        mShootingCounter = 50;
    }
    else if (aBodyReanim && aBodyReanim->TrackExists("anim_shooting"))
    {
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);

        switch (mSeedType)
        {
        case SeedType::SEED_FUMESHROOM:     mShootingCounter = 50;  break;
        case SeedType::SEED_PUFFSHROOM:
        case SeedType::SEED_SPORESHROOM:    mShootingCounter = 29;  break;
        case SeedType::SEED_SCAREDYSHROOM:  mShootingCounter = 25;  break;
        case SeedType::SEED_CABBAGEPULT:    mShootingCounter = 32;  break;
        case SeedType::SEED_PEPPERPULT:    mShootingCounter = 32;  break;
        case SeedType::SEED_MELONPULT:
        case SeedType::SEED_WINTERMELON:    mShootingCounter = 36;  break;
        case SeedType::SEED_KERNELPULT:
        {
            if (Sexy::Rand(4) == 0)
            {
                aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_NORMAL);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_HIDDEN);
                mState = PlantState::STATE_KERNELPULT_BUTTER;
            }

            mShootingCounter = 30;
            break;
        }
        case SeedType::SEED_CACTUS:         mShootingCounter = 35;  break;
        default:                            mShootingCounter = 29;  break;
        }
    }
    else
        Fire(aZombie, theRow, thePlantWeapon);

    return true;
}

void Plant::LaunchThreepeater()
{
    int rowAbove = mRow - 1;
    int rowBelow = mRow + 1;

    if ((FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) || FindTargetPVZ2Grave(mRow, PlantWeapon::WEAPON_PRIMARY)) ||
        (mBoard->RowCanHaveZombies(rowAbove) && (FindTargetZombie(rowAbove, PlantWeapon::WEAPON_PRIMARY) || FindTargetPVZ2Grave(rowAbove, PlantWeapon::WEAPON_PRIMARY))) ||
        (mBoard->RowCanHaveZombies(rowBelow) && (FindTargetZombie(rowBelow, PlantWeapon::WEAPON_PRIMARY) || FindTargetPVZ2Grave(rowBelow, PlantWeapon::WEAPON_PRIMARY))))
    {
        Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (mBoard->RowCanHaveZombies(rowBelow))
        {
            aHeadReanim1->StartBlend(10);
            aHeadReanim1->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim1->mAnimRate = 20.0f;
            aHeadReanim1->SetFramesForLayer("anim_shooting1");
        }

        aHeadReanim2->StartBlend(10);
        aHeadReanim2->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
        aHeadReanim2->mAnimRate = 20.0f;
        aHeadReanim2->SetFramesForLayer("anim_shooting2");

        if (mBoard->RowCanHaveZombies(rowAbove))
        {
            aHeadReanim3->StartBlend(10);
            aHeadReanim3->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aHeadReanim3->mAnimRate = 20.0f;
            aHeadReanim3->SetFramesForLayer("anim_shooting3");
        }

        mShootingCounter = 35;
    }
}

bool Plant::FindStarFruitTarget()
{
    if (mRecentlyEatenCountdown > 0)
        return true;

    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    int aCenterStarX = mX + 40;
    int aCenterStarY = mY + 40;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        Rect aZombieRect = aZombie->GetZombieRect();
        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS && mPlantCol >= 5)
                return true;

            if (aZombie->mRow == mRow)
            {
                if (aZombieRect.mX + aZombieRect.mWidth < aCenterStarX)
                    return true;
            }
            else
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
                    aZombieRect.mX += 10;

                float aProjectileTime = Distance2D(aCenterStarX, aCenterStarY, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2) / 3.33f;
                int aZombieHitX = aZombie->ZombieTargetLeadX(aProjectileTime) - aZombieRect.mWidth / 2;
                if ((aZombieHitX + aZombieRect.mWidth > aCenterStarX) && (aZombieHitX < aCenterStarX))
                    return true;

                int aCenterZombieX = aZombieHitX + aZombieRect.mWidth / 2;
                int aCenterZombieY = aZombieRect.mY + aZombieRect.mHeight / 2;
                float angle = RAD_TO_DEG(atan2(aCenterZombieY - aCenterStarY, aCenterZombieX - aCenterStarX));
                if (abs(aZombie->mRow - mRow) < 2)
                {
                    if ((angle > 20.0f && angle < 40.0f) || (angle < -25.0f && angle > -45.0f))
                        return true;
                }
                else
                {
                    if ((angle > 25.0f && angle < 35.0f) || (angle < -28.0f && angle > -38.0f))
                        return true;
                }
            }
        }
    }
    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE)
        {
            Rect aGraveRect = aGridItem->GetPVZ2GraveRect();
            if (aGridItem->mGridY == mRow)
            {
                if (aGraveRect.mX + aGraveRect.mWidth < aCenterStarX)
                    return true;
            }
            else
            {
                int aHitX = aGraveRect.mX;
                if ((aHitX + aGraveRect.mWidth > aCenterStarX) && (aHitX < aCenterStarX))
                    return true;

                int aCenterGraveX = aHitX + aGraveRect.mWidth / 2;
                int aCenterGraveY = aGraveRect.mY + aGraveRect.mHeight / 2;
                float angle = RAD_TO_DEG(atan2(aCenterGraveY - aCenterStarY, aCenterGraveX - aCenterStarX));
                if (abs(aGridItem->mGridY - mRow) < 2)
                {
                    if ((angle > 20.0f && angle < 40.0f) || (angle < -25.0f && angle > -45.0f)) return true;
                }
                else
                {
                    if ((angle > 25.0f && angle < 35.0f) || (angle < -28.0f && angle > -38.0f)) return true;
                }
            }
        }
    }
    return false;
}

void Plant::LaunchStarFruit()
{
    if (FindStarFruitTarget())
    {
        PlayBodyReanim("anim_shoot", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 28.0f);
        mShootingCounter = 40;
    }
}

void Plant::StarFruitFire()
{
    mApp->PlayFoley(FoleyType::FOLEY_THROW);

    float aShootAngleX = cos(DEG_TO_RAD(30.0f)) * 3.33f;
    float aShootAngleY = sin(DEG_TO_RAD(30.0f)) * 3.33f;
    for (int i = 0; i < 5; i++)
    {
        Projectile* aProjectile = mBoard->AddProjectile(mX + 25, mY + 25, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_STAR);
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;

        switch (i)
        {
        case 0:     aProjectile->mVelX = -3.33f;         aProjectile->mVelY = 0.0f;             break;
        case 1:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = 3.33f;             break;
        case 2:     aProjectile->mVelX = 0.0f;          aProjectile->mVelY = -3.33f;            break;
        case 3:     aProjectile->mVelX = aShootAngleX;  aProjectile->mVelY = aShootAngleY;      break;
        case 4:     aProjectile->mVelX = aShootAngleX;  aProjectile->mVelY = -aShootAngleY;     break;
        default:    TOD_ASSERT();                                                               break;
        }
    }
}
void Plant::UpdateShooter()
{
    int launchDecrement = 1;
    if (mBoard && mBoard->mEclipseCounter > 0)
    {
        if (!Plant::IsNocturnal(mSeedType) && mIsAsleep)
        {
            SetSleeping(false);
        }
        else if (Plant::IsNocturnal(mSeedType) && mSeedType != SeedType::SEED_MOONFLOWER)
        {
            if (mBoard->mMainCounter % 2 == 0)
            {
                launchDecrement = 0;
            }
        }
    }
    mLaunchCounter -= launchDecrement;
    if (mLaunchCounter <= 0)
    {
        mLaunchCounter = mLaunchRate - Sexy::Rand(15);
        if (mSeedType == SeedType::SEED_SNAPDRAGON)
        {
            UpdateSnapdragon();
        }
        else if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            LaunchThreepeater();
        }
        else if (mSeedType == SeedType::SEED_STARFRUIT)
        {
            LaunchStarFruit();
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_CACTUS)
        {
            if (mState == PlantState::STATE_CACTUS_HIGH)
            {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            else if (mState == PlantState::STATE_CACTUS_LOW)
            {
                FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }
        else
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }

    if (mLaunchCounter == 50 && mSeedType == SeedType::SEED_CATTAIL)
    {
        FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
    }
    if (mLaunchCounter == 25)
    {
        if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_PRIMARY);
            FindTargetAndFire(mRow, PlantWeapon::WEAPON_SECONDARY);
        }
    }
}

bool Plant::MakesSun()
{
    return mSeedType == SeedType::SEED_SUNFLOWER  || mSeedType == SeedType::SEED_PRIMALSUNFLOWER || mSeedType == SeedType::SEED_MOONFLOWER || mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_SUNSHROOM;
}

void Plant::UpdateProductionPlant()
{
    if (!IsInPlay() || mApp->IsIZombieLevel() || mApp->mGameMode == GameMode::GAMEMODE_UPSELL || mApp->mGameMode == GameMode::GAMEMODE_INTRO)
        return;

    if (mBoard->HasLevelAwardDropped())
        return;

    if (mSeedType == SeedType::SEED_MARIGOLD && mBoard->mCurrentWave == mBoard->mNumWaves)
    {
        if (mState != PlantState::STATE_MARIGOLD_ENDING)
        {
            mState = PlantState::STATE_MARIGOLD_ENDING;
            mStateCountdown = 6000;
        }
        else if (mStateCountdown <= 0)
            return;
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_LAST_STAND && mBoard->mChallenge->mChallengeState != ChallengeState::STATECHALLENGE_LAST_STAND_ONSLAUGHT)
        return;

    mLaunchCounter--;
    if (mLaunchCounter <= 100)
    {
        int aFlashCountdown = TodAnimateCurve(100, 0, mLaunchCounter, 0, 100, TodCurves::CURVE_LINEAR);
        mEatenFlashCountdown = max(mEatenFlashCountdown, aFlashCountdown);
    }
    if (mLaunchCounter <= 0)
    {
        mLaunchCounter = RandRangeInt(mLaunchRate - 150, mLaunchRate);
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

        if (mSeedType == SeedType::SEED_SUNSHROOM)
        {
            if (mState == PlantState::STATE_SUNSHROOM_SMALL)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
        }
        else if (mSeedType == SeedType::SEED_SUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_PRIMALSUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_BIGSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
        {
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
        }
        else if (mSeedType == SeedType::SEED_MOONFLOWER)
        {
            if (mBoard->mEclipseCounter > 0)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else if (mBoard->StageIsNight())
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
                mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
        }
        else if (mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(mX, mY, (Sexy::Rand(100) < 10) ? CoinType::COIN_GOLD : CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
        }

        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME)
        {
            if (mSeedType == SeedType::SEED_SUNFLOWER)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
            else if (mSeedType == SeedType::SEED_MARIGOLD)
            {
                mBoard->AddCoin(mX, mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
        }
    }
}

void Plant::UpdateSunShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SUNSHROOM_SMALL)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
            mState = PlantState::STATE_SUNSHROOM_GROWING;
            mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
        }

        UpdateProductionPlant();
    }
    else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_bigidle", ReanimLoopType::REANIM_LOOP, 10, RandRangeFloat(12.0f, 15.0f));
            mState = PlantState::STATE_SUNSHROOM_BIG;
        }
    }
    else
    {
        UpdateProductionPlant();
    }
}

void Plant::UpdateGraveBuster()
{
    if (mState == PlantState::STATE_GRAVEBUSTER_LANDING)
    {
        if (mApp->ReanimationGet(mBodyReanimID)->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
            mStateCountdown = 400;
            mState = PlantState::STATE_GRAVEBUSTER_EATING;
            AddAttachedParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER);
        }
    }
    else if (mState == PlantState::STATE_GRAVEBUSTER_EATING && mStateCountdown == 0)
    {
        GridItem* aGraveStone = mBoard->GetGraveStoneAt(mPlantCol, mRow);
        if (aGraveStone)
        {
            aGraveStone->GridItemDie();
            mBoard->mGravesCleared++;
        }

        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 4, ParticleEffect::PARTICLE_GRAVE_BUSTER_DIE);
        Die();
        mBoard->DropLootPiece(mX + 40, mY, 12);
    }
}

void Plant::PlayBodyReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (theBlendTime > 0)
        aBodyReanim->StartBlend(theBlendTime);

    if (theAnimRate > 0.0f)
        aBodyReanim->mAnimRate = theAnimRate;
    else if (theLoopType == ReanimLoopType::REANIM_LOOP)
        aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);

    aBodyReanim->mLoopType = theLoopType;
    aBodyReanim->mLoopCount = 0;
    aBodyReanim->SetFramesForLayer(theTrackName);
    aBodyReanim->mAnimTime = 0.0f;
}

void Plant::UpdatePotato()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    if (mState == PlantState::STATE_NOTREADY)
    {
        if (mStateCountdown == 0)
        {
            mApp->AddTodParticle(mX + mWidth / 2, mY + mHeight / 2, mRenderOrder, ParticleEffect::PARTICLE_POTATO_MINE_RISE);
            PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
            mState = PlantState::STATE_POTATO_RISING;
            mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
        }
    }
    else if (mState == PlantState::STATE_POTATO_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            float aRate = RandRangeFloat(12.0f, 15.0f);
            PlayBodyReanim("anim_armed", ReanimLoopType::REANIM_LOOP, 0, aRate);

            Reanimation* aLightReanim = mApp->AddReanimation(0.0f, 0.0f, mRenderOrder + 2, GetPlantDefinition(mSeedType).mReanimationType);
            aLightReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aLightReanim->mAnimRate = aRate - 2.0f;
            aLightReanim->SetFramesForLayer("anim_glow");
            aLightReanim->mFrameCount = 10;
            aLightReanim->ShowOnlyTrack("anim_glow");
            aLightReanim->SetTruncateDisappearingFrames("anim_glow", false);
            mLightReanimID = mApp->ReanimationGetID(aLightReanim);
            aLightReanim->AttachToAnotherReanimation(aBodyReanim, "anim_light");

            mState = PlantState::STATE_POTATO_ARMED;
            mBlinkCountdown = 400 + Sexy::Rand(4000);
        }
    }
    else if (mState == PlantState::STATE_POTATO_ARMED)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
        {
            DoSpecial();
        }
        else
        {
            Reanimation* aLightReanim = mApp->ReanimationTryToGet(mLightReanimID);
            if (aLightReanim)
            {
                aLightReanim->mFrameCount = TodAnimateCurve(200, 50, DistanceToClosestZombie(), 10, 3, TodCurves::CURVE_LINEAR);
            }
        }
    }
}

void Plant::UpdateTanglekelp()
{
    if (mState != PlantState::STATE_TANGLEKELP_GRABBING)
    {
        Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
        if (aZombie)
        {
            mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
            mState = PlantState::STATE_TANGLEKELP_GRABBING;
            mStateCountdown = 100;
            aZombie->PoolSplash(false);

            float aVinesPosX = -13.0f;
            float aVinesPosY = 15.0f;
            if (aZombie->mZombieType == ZombieType::ZOMBIE_SNORKEL)
            {
                aVinesPosX = -43.0f;
                aVinesPosY = 55.0f;
            }
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aVinesPosX = -20.0f;
                aVinesPosY = 37.0f;
            }
            Reanimation* aGrabReanim = aZombie->AddAttachedReanim(aVinesPosX, aVinesPosY, ReanimationType::REANIM_TANGLEKELP);
            if (aGrabReanim)
            {
                aGrabReanim->SetFramesForLayer("anim_grab");
                aGrabReanim->mAnimRate = 24.0f;
                aGrabReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            }

            mTargetZombieID = mBoard->ZombieGetID(aZombie);
        }
    }
    else
    {
        if (mStateCountdown == 50)
        {
            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                aZombie->DragUnder();
                aZombie->PoolSplash(false);
            }
        }

        if (mStateCountdown == 20)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            Reanimation* aSplashReanim = mApp->AddReanimation(mX - 23, mY + 7, aRenderPosition, ReanimationType::REANIM_SPLASH);
            aSplashReanim->OverrideScale(1.3f, 1.3f);

            mApp->AddTodParticle(mX + 31, mY + 64, aRenderPosition, ParticleEffect::PARTICLE_PLANTING_POOL);
            mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);
        }

        if (mStateCountdown == 0)
        {
            Die();

            Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
            if (aZombie)
            {
                aZombie->DieWithLoot();
            }
            if (mHasBooster && mBoard)
            {
                mBoard->AddPlant(mPlantCol, mRow, SeedType::SEED_TANGLEKELP, SeedType::SEED_NONE);
            }
        }
    }
}
bool Plant::applyboost()
{
    if (mDead || mIsAsleep)
    {
        mApp->PlaySample(Sexy::SOUND_BUZZER);
        return false;
    }

    if (mHasBooster)
    {
        mApp->PlaySample(Sexy::SOUND_BUZZER);
        return false;
    }

    if (mBoard->mPlantFoodCount <= 0)
    {
        mApp->PlaySample(Sexy::SOUND_BUZZER);
        return false;
    }

    if (mSeedType == SeedType::SEED_CHERRYBOMB ||
        mSeedType == SeedType::SEED_JALAPENO ||
        mSeedType == SeedType::SEED_DOOMSHROOM ||
        mSeedType == SeedType::SEED_XSHROOM ||
        mSeedType == SeedType::SEED_ICESHROOM)
    {
        mApp->PlaySample(Sexy::SOUND_BUZZER);
        return false;
    }

    if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        Plant* p = nullptr;
        while (mBoard->IteratePlants(p))
        {
            if (p->mSeedType == SeedType::SEED_PUFFSHROOM && !p->mDead && !p->mHasBooster)
            {
                p->mHasBooster = true;
                p->mBoostTimer = 100;
                p->mBoostFireTimer = 10;
                if (p == this)
                {
                    TodParticleSystem* glow = p->mApp->AddTodParticle(p->mX + 40, p->mY + 40, p->mRenderOrder - 1, ParticleEffect::PARTICLE_POTTED_PLANT_GLOW);
                    if (glow != nullptr)
                    {
                        glow->OverrideColor(nullptr, Color(100, 255, 100, 255));
                        glow->OverrideScale(nullptr, 1.2f);
                        glow->OverrideExtraAdditiveDraw(nullptr, true);
                        p->mBoostGlowParticleID = p->mApp->ParticleGetID(glow);
                    }
                }
            }
        }
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_PEASHOOTER ||
        mSeedType == SeedType::SEED_REPEATER ||
        mSeedType == SeedType::SEED_THREEPEATER ||
        mSeedType == SeedType::SEED_GATLINGPEA ||
        mSeedType == SeedType::SEED_SNOWPEA ||
        mSeedType == SeedType::SEED_SPLITPEA ||
        mSeedType == SeedType::SEED_LEFTPEATER)
    {
        mApp->PlayFoley(FoleyType::FOLEY_PEAGUN);
    }

    mHasBooster = true;
    mBoostTimer = 150;
    mApp->PlayFoley(FoleyType::FOLEY_PRIZE);
    TodParticleSystem* glowSys = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder - 1, ParticleEffect::PARTICLE_POTTED_PLANT_GLOW);
    if (glowSys != nullptr)
    {
        glowSys->OverrideColor(nullptr, Color(100, 255, 100, 255));
        glowSys->OverrideScale(nullptr, 1.2f);
        glowSys->OverrideExtraAdditiveDraw(nullptr, true);
        mBoostGlowParticleID = mApp->ParticleGetID(glowSys);
    }

    if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_ACID_LEMON || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_CACTUS) {
        mBoostFireTimer = 50;
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER) {
        mBoostFireTimer = 300;
    }
    else if (mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER) {
        mBoostFireTimer = 100;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA) {
        mBoostFireTimer = 150;
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA) {
        mBoostFireTimer = 200;
    }
    else if (mSeedType == SeedType::SEED_STARFRUIT) {
        mBoostFireTimer = 200;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM) {
        mBoostFireTimer = 50;
    }
    else {
        mBoostFireTimer = 100;
    }

    if (MakesSun() || mSeedType == SeedType::SEED_MARIGOLD)
    {
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);

        mBoostTimer = 100;

        if (mSeedType == SeedType::SEED_SUNSHROOM && mState == PlantState::STATE_SUNSHROOM_SMALL)
        {
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 18.0f);
            mState = PlantState::STATE_SUNSHROOM_GROWING;
            mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
            mBoostFireTimer = 8;
        }
        else if (mSeedType == SeedType::SEED_TWINSUNFLOWER || mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoostFireTimer = 10;
        }
        else if (mSeedType == SeedType::SEED_PRIMALSUNFLOWER)
        {
            mBoostFireTimer = 8;
        }
        else if (mSeedType == SeedType::SEED_MOONFLOWER)
        {
            mBoostFireTimer = 7;
        }
        else
        {
            mBoostFireTimer = 5;
        }

        mBoard->mPlantFoodCount--;
        return true;
    }
    else if (mSeedType == SeedType::SEED_GOLD_MAGNET)
    {
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);

        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        mStateCountdown = 100;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->IsDeadOrDying() || aZombie->mMindControlled) continue;

            MagnetItem* aMagnetItem = GetFreeMagnetItem();
            if (aMagnetItem)
            {
                aMagnetItem->mPosX = aZombie->mPosX + 20.0f;
                aMagnetItem->mPosY = aZombie->mPosY + 40.0f;
                aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
                aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;
                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_GOLD_COIN;
            }
        }
    }
    else if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT ||
        mSeedType == SeedType::SEED_PUMPKINSHELL || mSeedType == SeedType::SEED_GARLIC ||
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        mPlantMaxHealth *= 2;
        mPlantHealth = mPlantMaxHealth;
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            if (mSeedType == SeedType::SEED_WALLNUT) aBodyReanim->SetImageOverride("anim_face", nullptr);
            else if (mSeedType == SeedType::SEED_TALLNUT) aBodyReanim->SetImageOverride("anim_idle", nullptr);
            else if (mSeedType == SeedType::SEED_GARLIC) aBodyReanim->SetImageOverride("anim_face", nullptr);
            else if (mSeedType == SeedType::SEED_PUMPKINSHELL) aBodyReanim->SetImageOverride("Pumpkin_front", nullptr);
        }
    }
    else if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_LILYPAD || mSeedType == SeedType::SEED_FLOWERPOT)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0) continue;
                int nx = mPlantCol + dx;
                int ny = mRow + dy;
                if (nx >= 0 && nx < MAX_GRID_SIZE_X && ny >= 0 && ny < MAX_GRID_SIZE_Y)
                {
                    if (mBoard->CanPlantAt(nx, ny, mSeedType) == PlantingReason::PLANTING_OK)
                    {
                        Plant* aNewPlant = mBoard->AddPlant(nx, ny, mSeedType, SeedType::SEED_NONE);
                        if (aNewPlant && mSeedType == SeedType::SEED_POTATOMINE)
                        {
                            aNewPlant->mStateCountdown = 0;
                        }
                    }
                }
            }
        }
    }

    if (mSeedType == SeedType::SEED_SQUASH)
    {
        mBoostFireTimer = 10;
        mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
        mStateCountdown = 1;
        mJumpStartX = mX;
        mJumpStartY = mY;
        PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 30.0f);
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        mBoostTimer = 1000;
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_GUARDIAN_SHROOM)
    {
        mPlantHealth = 6000;
        mPlantMaxHealth = 6000;
        for (int dCol = -1; dCol <= 1; dCol++)
        {
            for (int dRow = -1; dRow <= 1; dRow++)
            {
                int targetCol = mPlantCol + dCol;
                int targetRow = mRow + dRow;
                if (targetCol >= 0 && targetCol < MAX_GRID_SIZE_X && targetRow >= 0 && targetRow < MAX_GRID_SIZE_Y)
                {
                    Plant* existingMini = nullptr;
                    Plant* p = nullptr;
                    while (mBoard->IteratePlants(p))
                    {
                        if (p->mPlantCol == targetCol && p->mRow == targetRow && p->mSeedType == SeedType::SEED_MINI_GUARDIAN_SHROOM)
                        {
                            existingMini = p;
                            break;
                        }
                    }
                    if (existingMini)
                    {
                        existingMini->mPlantMaxHealth = 1500;
                        existingMini->mPlantHealth = 1500;
                    }
                    else if (mBoard->CanPlantAt(targetCol, targetRow, SeedType::SEED_MINI_GUARDIAN_SHROOM) == PlantingReason::PLANTING_OK)
                    {
                        Plant* newMini = mBoard->AddPlant(targetCol, targetRow, SeedType::SEED_MINI_GUARDIAN_SHROOM, SeedType::SEED_NONE);
                        if (newMini)
                        {
                            newMini->mPlantMaxHealth = 1500;
                            newMini->mPlantHealth = 1500;
                        }
                    }
                }
            }
        }
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_MINI_GUARDIAN_SHROOM)
    {
        mPlantMaxHealth = 2000;
        mPlantHealth = 2000;
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_HAMMER_SHROOM)
    {
        mBoostTimer = 500;
        mBoostFireTimer = 10;
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->IsDeadOrDying() || aZombie->mMindControlled) continue;

            bool hasMetal = false;
            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING ||
                aZombie->mZombieType == ZombieType::ZOMBIE_POGO)
            {
                if (aZombie->mHasObject) hasMetal = true;
            }
            else if (aZombie->mHelmType == HelmType::HELMTYPE_PAIL ||
                aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
            {
                hasMetal = true;
            }

            if (hasMetal)
            {
                MagnetShroomAttactItem(aZombie);
            }
        }

        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
            {
                MagnetItem* aMagnetItem = GetFreeMagnetItem();
                if (aMagnetItem)
                {
                    aMagnetItem->mPosX = mBoard->GridToPixelX(aGridItem->mGridX, aGridItem->mGridY) + 40;
                    aMagnetItem->mPosY = mBoard->GridToPixelY(aGridItem->mGridX, aGridItem->mGridY);
                    aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 10.0f;
                    aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
                    aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LADDER_PLACED;
                    aGridItem->GridItemDie();
                }
            }
        }

        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        mStateCountdown = 100;
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 60.0f);
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_CHOMPER)
    {
        if (mIsFlipped)
        {
            mApp->PlaySample(Sexy::SOUND_BUZZER);
            return false;
        }
        mBoostTimer = 300;
        mBoostFireTimer = 10;
        mState = PlantState::STATE_DOINGSPECIAL;
        mStateCountdown = 300;
        PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 45.0f);
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_HYPNOSHROOM)
    {
        mHasBooster = true;
        mBoostTimer = 99999;
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_PLANTERN)
    {
        mBoostTimer = 500;
        mApp->PlayFoley(FoleyType::FOLEY_PRIZE);
        TodParticleSystem* p = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        if (p) p->OverrideScale(nullptr, 5.0f);
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_TANGLEKELP)
    {
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);

        int zombiesDrowned = 0;
        Zombie* aZombie = nullptr;

        while (mBoard->IterateZombies(aZombie))
        {
            if (zombiesDrowned >= 5) break;

            if (aZombie->IsDeadOrDying() || aZombie->mMindControlled || aZombie->IsFlying() ||
                aZombie->mZombieType == ZombieType::ZOMBIE_BOSS ||
                aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR ||
                aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR ||
                aZombie->mZombieType == ZombieType::ZOMBIE_BLEUEYE_GARGANTUAR ||
                aZombie->mZombieType == ZombieType::ZOMBIE_EXA_GARGANTUAR ||
                aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE ||
                aZombie->mZombieType == ZombieType::ZOMBIE_ZAMBONI ||
                aZombie->mZombieType == ZombieType::ZOMBIE_CATAPULT ||
                aZombie->mZombieType == ZombieType::ZOMBIE_BOBSLED)
            {
                continue;
            }

            if (aZombie->mRow == 2 || aZombie->mRow == 3)
            {
                mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
                aZombie->PoolSplash(false);

                float aVinesPosX = -13.0f;
                float aVinesPosY = 15.0f;
                if (aZombie->mZombieType == ZombieType::ZOMBIE_SNORKEL)
                {
                    aVinesPosX = -43.0f;
                    aVinesPosY = 55.0f;
                }
                else if (aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
                {
                    aVinesPosX = -20.0f;
                    aVinesPosY = 37.0f;
                }

                Reanimation* aGrabReanim = aZombie->AddAttachedReanim(aVinesPosX, aVinesPosY, ReanimationType::REANIM_TANGLEKELP);
                if (aGrabReanim)
                {
                    aGrabReanim->SetFramesForLayer("anim_grab");
                    aGrabReanim->mAnimRate = 24.0f;
                    aGrabReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
                }

                int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, aZombie->mRow, 0);
                Reanimation* aSplashReanim = mApp->AddReanimation(aZombie->mX - 23, aZombie->mY + 7, aRenderPosition, ReanimationType::REANIM_SPLASH);
                if (aSplashReanim) aSplashReanim->OverrideScale(1.3f, 1.3f);
                mApp->AddTodParticle(aZombie->mX + 31, aZombie->mY + 64, aRenderPosition, ParticleEffect::PARTICLE_PLANTING_POOL);
                mApp->PlayFoley(FoleyType::FOLEY_ZOMBIE_ENTERING_WATER);

                aZombie->DragUnder();
                aZombie->DieWithLoot();
                zombiesDrowned++;
            }
        }
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK)
    {
        mBoostTimer = 500;
        bool isRock = (mSeedType == SeedType::SEED_SPIKEROCK);
        for (int col = 0; col < MAX_GRID_SIZE_X; col++)
        {
            if (col != mPlantCol && mBoard->mGridSquareType[col][mRow] != GridSquareType::GRIDSQUARE_NONE)
            {
                mBoard->AddSpikeBoostGridItem(col, mRow, isRock);
            }
        }
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mBoard->mPlantFoodCount--;
        return true;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL) {
        mBoostFireTimer = 1;
        mBoostTimer = 10;
    }

    if (mSeedType == SeedType::SEED_COBCANNON)
    {
        mBoostTimer = 150;
        mState = PlantState::STATE_COBCANNON_FIRING;
        mShootingCounter = 206;
        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        mApp->PlayFoley(FoleyType::FOLEY_COB_LAUNCH);
        int aOriginX = mX - 44;
        int aOriginY = mY - 184;
        int aCenterX = 400 + BOARD_ADDITIONAL_WIDTH;
        int aCenterY = 300 + BOARD_OFFSET_Y;
        int aCenterRow = mBoard->PixelToGridYKeepOnBoard(aCenterX, aCenterY);
        Projectile* aMeteor = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder + 1, aCenterRow, ProjectileType::PROJECTILE_COBBIG);
        if (aMeteor)
        {
            aMeteor->mIsMeteorBoost = true;
            aMeteor->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            aMeteor->mMotionType = ProjectileMotion::MOTION_LOBBED;
            aMeteor->mVelX = 0.001f;
            aMeteor->mVelY = 0.0f;
            aMeteor->mVelZ = -8.0f;
            aMeteor->mAccZ = 0.0f;
            aMeteor->mCobTargetX = aCenterX;
            aMeteor->mCobTargetRow = aCenterRow;
        }
        mBoard->mPlantFoodCount--;
        return true;
    }

    if (mSeedType == SeedType::SEED_GARLIC)
    {
        mBoostTimer = 500;
        mBoostFireTimer = 0;
    }

    if (mSeedType == SeedType::SEED_TORCHWOOD)
    {
        int aCol = mPlantCol;
        int aRow = mRow;
        TodParticleSystem* aPart = mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_IMITATER_MORPH);
        if (aPart)
        {
            aPart->OverrideColor(nullptr, Color(0, 150, 255, 255));
        }
        mApp->PlayFoley(FoleyType::FOLEY_FLOOP);
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        Die();
        Plant* aPlasmaWood = mBoard->AddPlant(aCol, aRow, SeedType::SEED_PLASMAWOOD, SeedType::SEED_NONE);
        if (aPlasmaWood)
        {
            aPlasmaWood->mPlantHealth = aPlasmaWood->mPlantMaxHealth;
        }
    }
    if (mSeedType == SeedType::SEED_TRAMPALINE)
    {
        PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
        mBoard->ShakeBoard(3, -4);

        ZombieID zombieArray[100];
        int zombieCount = 0;
        Zombie* aZombie = nullptr;

        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && !aZombie->IsFlying() && aZombie->mZombieType != ZombieType::ZOMBIE_BOSS && aZombie->mZombiePhase != ZombiePhase::PHASE_TRAMPALINE_BOUNCE)
            {
                if (zombieCount < 100) zombieArray[zombieCount++] = mBoard->ZombieGetID(aZombie);
            }
        }
        for (int i = 0; i < zombieCount - 1; i++) {
            for (int j = 0; j < zombieCount - i - 1; j++) {
                Zombie* z1 = mBoard->ZombieTryToGet(zombieArray[j]);
                Zombie* z2 = mBoard->ZombieTryToGet(zombieArray[j + 1]);
                if (z1 && z2) {
                    int health1 = z1->mBodyHealth + z1->mHelmHealth + z1->mShieldHealth;
                    int health2 = z2->mBodyHealth + z2->mHelmHealth + z2->mShieldHealth;
                    if (health1 < health2) {
                        ZombieID temp = zombieArray[j];
                        zombieArray[j] = zombieArray[j + 1];
                        zombieArray[j + 1] = temp;
                    }
                }
            }
        }

        int targets = min(zombieCount, 7);
        for (int i = 0; i < targets; i++)
        {
            Zombie* z = mBoard->ZombieTryToGet(zombieArray[i]);
            if (z) {
                if (z->mZombieType == ZombieType::ZOMBIE_BUNGEE) {
                    mApp->PlaySample(SOUND_BOING);
                    mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
                    z->mZombiePhase = ZombiePhase::PHASE_BUNGEE_RISING;
                    z->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
                    z->mHitUmbrella = true;
                }
                else {
                    int tilesToThrow = (z->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_BLEUEYE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_EXA_GARGANTUAR) ? 2 : 4;

                    z->mLastPortalX = (int)z->mZombiePhase;

                    z->mZombiePhase = ZombiePhase::PHASE_TRAMPALINE_BOUNCE;
                    z->mVelZ = 4.0f;
                    float airTime = (z->mVelZ * 2.0f) / 0.05f;
                    float speedX = (tilesToThrow * 80.0f) / airTime;
                    z->mVelX = mTrampolineReversed ? -speedX : speedX;
                }
            }
        }

        mState = PlantState::STATE_NOTREADY;
        mStateCountdown = 2000;
        RemoveBoost();
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_UMBRELLA)
    {
        mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
        PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);
        mState = PlantState::STATE_UMBRELLA_REFLECTING;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && !aZombie->IsFlying() && aZombie->mZombieType != ZombieType::ZOMBIE_BOSS)
            {
                aZombie->mPosX += 160.0f;
                aZombie->mX = (int)aZombie->mPosX;
                mApp->AddTodParticle(aZombie->mX + 40, aZombie->mY + 40, aZombie->mRenderOrder + 1, ParticleEffect::PARTICLE_POWIE);
            }
        }
        RemoveBoost();
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_ACID_LEMON)
    {
        TodParticleSystem* aParticle = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_ICE_TRAP);
        if (aParticle) aParticle->OverrideColor(nullptr, Color(0, 255, 0, 255));
        mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY)))
            {
                aZombie->ApplyAcid();
                aZombie->mAcidCounter = 1000;
            }
        }
        Die();
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_SPORESHROOM)
    {
        mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 2, ParticleEffect::PARTICLE_STARBURST);

        int killedCount = 0;
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (killedCount >= 5) break;

            if (!aZombie->IsDeadOrDying() && !aZombie->IsTooStrongForBees())
            {
                int zCol = mBoard->PixelToGridXKeepOnBoard(aZombie->mX + 40, aZombie->mY + 40);
                int zRow = aZombie->mRow;

                mApp->AddTodParticle(aZombie->mX + 40, aZombie->mY + 40, aZombie->mRenderOrder + 1, ParticleEffect::PARTICLE_IMITATER_MORPH);
                aZombie->TakeDamage(9999, 0U);
                if (mBoard->CanPlantAt(zCol, zRow, SeedType::SEED_SPORESHROOM) == PlantingReason::PLANTING_OK)
                {
                    mBoard->AddPlant(zCol, zRow, SeedType::SEED_SPORESHROOM, SeedType::SEED_NONE);
                }

                killedCount++;
            }
        }

        RemoveBoost();
        mBoard->mPlantFoodCount--;
        return true;
    }
    if (mSeedType == SeedType::SEED_TOADSTOOL)
    {
        int eatCount = 0;
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (eatCount >= 7) break;

            if (!aZombie->IsDeadOrDying() && !aZombie->IsTooStrongForBees())
            {
                if (aZombie->mRow == mRow && aZombie->mX >= mX && aZombie->mX <= mX + 240)
                {
                    mApp->AddTodParticle(aZombie->mX + 40, aZombie->mY + 40, aZombie->mRenderOrder + 1, ParticleEffect::PARTICLE_IMITATER_MORPH);
                    aZombie->DieWithLoot();
                    eatCount++;
                }
            }
        }

        if (eatCount > 0)
        {
            mState = PlantState::STATE_CHOMPER_SWALLOWING;
            mStateCountdown = 50;
            PlayBodyReanim("anim_swallow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 30.0f);
            mApp->PlayFoley(FoleyType::FOLEY_CHOMP);
            for (int i = 0; i < 6; i++) {
                float aOffsetX = RandRangeFloat(-20.0f, 20.0f);
                float aOffsetY = RandRangeFloat(-20.0f, 20.0f);
                mBoard->AddCoin(mX + 40 + (int)aOffsetX, mY + 40 + (int)aOffsetY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
        }

        RemoveBoost();
    }
    if (mSeedType == SeedType::SEED_POISONSHROOM)
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        TodParticleSystem* aPart = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_POWIE);
        if (aPart) aPart->OverrideColor(nullptr, Color(180, 0, 255, 255));

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (!aZombie->IsDeadOrDying() && !aZombie->mMindControlled)
            {
                aZombie->ApplyAcid();
            }
        }

        RemoveBoost();
        mBoard->mPlantFoodCount--;
        return true;
    }

    mBoard->mPlantFoodCount--;
    return true;
}
Zombie* Plant::FindBoostedSquashTarget()
{
    Zombie* aBestZombie = nullptr;
    int aMaxHealth = -1;
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->IsDeadOrDying() || aZombie->mMindControlled || aZombie->IsFlying() ||
            aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
            aZombie->mZombiePhase == ZombiePhase::PHASE_BUNGEE_DIVING ||
            aZombie->mZombiePhase == ZombiePhase::PHASE_BUNGEE_RISING ||
            aZombie->mZombieHeight == ZombieHeight::HEIGHT_GETTING_BUNGEE_DROPPED)
        {
            continue;
        }
        int aHealth = aZombie->mBodyHealth + aZombie->mHelmHealth + aZombie->mShieldHealth;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) aHealth += 100000;
        if (aHealth > aMaxHealth)
        {
            aMaxHealth = aHealth;
            aBestZombie = aZombie;
        }
    }
    return aBestZombie;
}
void Plant::SpikeweedAttack()
{
    TOD_ASSERT(IsSpiky());

    if (mState != PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 18.0f);
        mApp->PlaySample(SOUND_THROW);
        
        mState = PlantState::STATE_SPIKEWEED_ATTACKING;
        mStateCountdown = 100;
    }
}

void Plant::UpdateSpikeweed()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_SPIKEWEED_ATTACKING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_NOTREADY;
        }
        else if (mSeedType == SeedType::SEED_SPIKEROCK)
        {
            if (mStateCountdown == 69 || mStateCountdown == 33)
            {
                DoRowAreaDamage(20, 33U);
            }
        }
        else if (mStateCountdown == 75)
        {
            DoRowAreaDamage(20, 33U);
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(RandRangeFloat(12.0f, 15.0f));
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        SpikeweedAttack();
    }
}

void Plant::UpdateScaredyShroom()
{
    if (mShootingCounter > 0)
        return;
    bool aHasZombieNearby = false;
    if (!mHasBooster)
    {
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aDiffY = (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) ? 0 : (aZombie->mRow - mRow);
            if (!aZombie->mMindControlled && !aZombie->IsDeadOrDying() && aDiffY <= 1 && aDiffY >= -1 && GetCircleRectOverlap(mX, mY + 20.0f, 120, aZombieRect))
            {
                aHasZombieNearby = true;
                break;
            }
        }
    }
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_READY)
    {
        if (aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_LOWERING;
            PlayBodyReanim("anim_scared", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 10.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_SCARED;
            PlayBodyReanim("anim_scaredidle", ReanimLoopType::REANIM_LOOP, 10, 0.0f);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_SCARED)
    {
        if (!aHasZombieNearby)
        {
            mState = PlantState::STATE_SCAREDYSHROOM_RAISING;
            float aAnimRate = RandRangeFloat(7.0f, 12.0f);
            PlayBodyReanim("anim_grow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, aAnimRate);
        }
    }
    else if (mState == PlantState::STATE_SCAREDYSHROOM_RAISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;
            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayIdleAnim(aAnimRate);
        }
    }
    if (mState != PlantState::STATE_READY)
    {
        mLaunchCounter = mLaunchRate;
    }
}
void Plant::UpdateTorchwood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if ((aProjectile->mRow == mRow) && 
            (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA || aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToFireball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToPea(mPlantCol);
                }
            }
        }
    }
}
void Plant::UpdatePlasmawood()
{
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    Projectile* aProjectile = nullptr;
    while (mBoard->IterateProjectiles(aProjectile))
    {
        if ((aProjectile->mRow == mRow) &&
            (aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA ||
                aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
                aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL))
        {
            Rect aProjectileRect = aProjectile->GetProjectileRect();
            if (GetRectOverlap(aAttackRect, aProjectileRect) >= 10)
            {
                if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
                    aProjectile->mProjectileType == ProjectileType::PROJECTILE_PEA)
                {
                    aProjectile->ConvertToPlasmaball(mPlantCol);
                }
                else if (aProjectile->mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
                {
                    aProjectile->ConvertToFireball(mPlantCol);
                }
            }
        }
    }
}
void Plant::DoSquashDamage()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aAttackRect, aZombieRect) > (aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL ? -20 : 0))
            {
                aZombie->TakeDamage(1800, 18U);
            }
        }
    }
}

Zombie* Plant::FindSquashTarget()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

    int aClosestRange = 0;
    Zombie* aClosestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mRow == mRow || aZombie->mZombieType == ZombieType::ZOMBIE_BOSS) &&
            aZombie->mHasHead && !aZombie->IsTangleKelpTarget() && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();

            if ((
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && aZombieRect.mX < mX + 20
                ) || (
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_POLEVAULTER_IN_VAULT &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_SNORKEL_INTO_POOL && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_INTO_POOL &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_RIDING && 
                    aZombie->mZombiePhase != ZombiePhase::PHASE_DOLPHIN_IN_JUMP &&
                    !aZombie->IsBobsledTeamWithSled()
                ))
            {
                int aRange = -GetRectOverlap(aAttackRect, aZombieRect);
                if (aRange <= (aZombie->mIsEating ? 110 : 70))
                {
                    int aPlantX = aAttackRect.mX;
                    if (aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_POST_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT ||
                        aZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL || aZombie->mZombieType == ZombieType::ZOMBIE_IMP ||
                        aZombie->mZombieType == ZombieType::ZOMBIE_FOOTBALL || mApp->IsScaryPotterLevel())
                    {
                        aPlantX = aAttackRect.mX - 60;
                    }

                    if (aZombie->IsWalkingBackwards() || aZombieRect.mX + aZombieRect.mWidth >= aPlantX)
                    {
                        if (mBoard->ZombieGetID(aZombie) == mTargetZombieID)
                            return aZombie;  

                        if (aClosestZombie == nullptr || aRange < aClosestRange)
                        {
                            aClosestZombie = aZombie;
                            aClosestRange = aRange;
                        }
                    }
                }
            }
        }
    }

    return aClosestZombie;
}

void Plant::UpdateSquash()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);
    if (mState == PlantState::STATE_NOTREADY)
    {
        Zombie* aZombie = FindSquashTarget();
        if (aZombie)
        {
            mTargetZombieID = mBoard->ZombieGetID(aZombie);
            mTargetX = aZombie->ZombieTargetLeadX(0.0f) - mWidth / 2;
            mTargetY = mBoard->GridToPixelY(mPlantCol, mStartRow) + 8;
            mState = PlantState::STATE_SQUASH_LOOK;
            mStateCountdown = 80;
            PlayBodyReanim(mTargetX < mX ? "anim_lookleft" : "anim_lookright", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlayFoley(FoleyType::FOLEY_SQUASH_HMM);
        }
    }
    else if (mState == PlantState::STATE_SQUASH_LOOK)
    {
        if (mStateCountdown <= 0)
        {
            PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
            mStateCountdown = 30;
            mJumpStartX = mX;
            mJumpStartY = mY;
        }
    }
    else if (mState == PlantState::STATE_SQUASH_PRE_LAUNCH)
    {
        if (mStateCountdown <= 0)
        {
            if (mHasBooster)
            {
                if (mBoostFireTimer > 0)
                {
                    Zombie* aTargetZombie = FindBoostedSquashTarget();
                    if (aTargetZombie)
                    {
                        mTargetX = aTargetZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
                        mRow = aTargetZombie->mRow;
                        mTargetY = mBoard->GridToPixelY(0, mRow) + 8;
                        mRenderOrder = CalcRenderOrder();
                    }
                    else
                    {
                        mBoostFireTimer = 0; 
                    }
                }
                if (mBoostFireTimer <= 0)
                {
                    mTargetX = mBoard->GridToPixelX(mPlantCol, mStartRow);
                    mRow = mStartRow;
                    mTargetY = mBoard->GridToPixelY(mPlantCol, mStartRow) + 8;
                    mRenderOrder = CalcRenderOrder();
                }
            }
            else
            {
                Zombie* aZombie = FindSquashTarget();
                if (aZombie)
                {
                    mTargetX = aZombie->ZombieTargetLeadX(30.0f) - mWidth / 2;
                }
                mTargetY = mBoard->GridToPixelY(mPlantCol, mStartRow) + 8;
            }
            mJumpStartX = mX;
            mJumpStartY = mY;
            mState = PlantState::STATE_SQUASH_RISING;
            mStateCountdown = mHasBooster ? 25 : 50;
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        }
    }
    else
    {
        int aDestY = mTargetY;
        int aRiseTime = mHasBooster ? 25 : 50;
        int aFallTime = mHasBooster ? 6 : 10;
        if (mState == PlantState::STATE_SQUASH_RISING)
        {
            int aHeightBoost = mHasBooster ? 250 : 120;
            mX = TodAnimateCurve(aRiseTime, 10, mStateCountdown, mJumpStartX, mTargetX, TodCurves::CURVE_EASE_IN_OUT);
            mY = TodAnimateCurve(aRiseTime, 10, mStateCountdown, mJumpStartY, aDestY - aHeightBoost, TodCurves::CURVE_EASE_IN_OUT);
            if (mStateCountdown == 0)
            {
                PlayBodyReanim("anim_jumpdown", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 60.0f);
                mState = PlantState::STATE_SQUASH_FALLING;
                mStateCountdown = aFallTime;
            }
        }
        else if (mState == PlantState::STATE_SQUASH_FALLING)
        {
            int aHeightBoost = mHasBooster ? 250 : 120;
            mY = TodAnimateCurve(aFallTime, 0, mStateCountdown, aDestY - aHeightBoost, aDestY, TodCurves::CURVE_EASE_IN_OUT);
            if (mStateCountdown == (mHasBooster ? 3 : 5))
            {
                DoSquashDamage();
            }
            if (mStateCountdown == 0)
            {
                int aTargetCol = mBoard->PixelToGridXKeepOnBoard(mTargetX, mY);
                if (mBoard->IsPoolSquare(aTargetCol, mRow) && !mHasBooster)
                {
                    mApp->AddReanimation(mX - 11, mY + 20, mRenderOrder + 1, ReanimationType::REANIM_SPLASH);
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    mApp->PlaySample(SOUND_ZOMBIESPLASH);
                    Die();
                }
                else
                {
                    mState = PlantState::STATE_SQUASH_DONE_FALLING;
                    mStateCountdown = mHasBooster ? 15 : 100; 
                    mBoard->ShakeBoard(mHasBooster ? 3 : 1, 4);
                    mApp->PlayFoley(FoleyType::FOLEY_THUMP);
                    float aOffsetY = mBoard->StageHasRoof() ? 69.0f : 80.0f;
                    mApp->AddTodParticle(mX + 40, mY + aOffsetY, mRenderOrder + 4, ParticleEffect::PARTICLE_DUST_SQUASH);
                }
            }
        }
        else if (mState == PlantState::STATE_SQUASH_DONE_FALLING)
        {
            if (mStateCountdown == 0)
            {
                if (mHasBooster)
                {
                    if (mBoostFireTimer > 0)
                    {
                        mBoostFireTimer--;
                        mState = PlantState::STATE_SQUASH_PRE_LAUNCH;
                        mStateCountdown = 1;
                        PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 35.0f);
                    }
                    else
                    {
                        RemoveBoost(); // تم التعديل هنا
                        mState = PlantState::STATE_NOTREADY;
                        mX = mBoard->GridToPixelX(mPlantCol, mStartRow);
                        mY = mBoard->GridToPixelY(mPlantCol, mStartRow);
                        mRow = mStartRow;
                        mRenderOrder = CalcRenderOrder();
                        PlayIdleAnim(15.0f);
                    }
                }
                else
                {
                    Die();
                }
            }
        }
    }
}
void Plant::UpdateDoomShroom()
{
    if (mIsAsleep || mState == PlantState::STATE_DOINGSPECIAL)
        return;

    mState = PlantState::STATE_DOINGSPECIAL;
    mDoSpecialCountdown = 100;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);

    aBodyReanim->SetFramesForLayer("anim_explode");
    aBodyReanim->mAnimRate = 23.0f;
    aBodyReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
    aBodyReanim->SetShakeOverride("DoomShroom_head1", 1.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head2", 2.0f);
    aBodyReanim->SetShakeOverride("DoomShroom_head3", 2.0f);
    mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
}
void Plant::UpdateXShroom()
{
    if (mIsAsleep || mState == PlantState::STATE_DOINGSPECIAL)
        return;
    mState = PlantState::STATE_DOINGSPECIAL;
    mDoSpecialCountdown = 100;
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    TOD_ASSERT(aBodyReanim);
    aBodyReanim->SetFramesForLayer("anim_explode");
    aBodyReanim->mAnimRate = 23.0f;
    mApp->PlayFoley(FoleyType::FOLEY_REVERSE_EXPLOSION);
}
void Plant::UpdateIceShroom()
{
    if (!mIsAsleep && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        mDoSpecialCountdown = 100;
    }
}

void Plant::UpdateBlover()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (aBodyReanim->mLoopCount > 0 && aBodyReanim->mLoopType != ReanimLoopType::REANIM_LOOP)
    {
        aBodyReanim->SetFramesForLayer("anim_loop");
        aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    }
    if (mState != PlantState::STATE_DOINGSPECIAL && mStateCountdown == 0)
    {
        DoSpecial(); 
        mStateCountdown = 300;
    }
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        if (mBoard->mNuclearRainActive)
        {
            mBoard->mNuclearRainCounter -= 10;
            if (mBoard->mNuclearRainCounter <= 0)
            {
                mBoard->mNuclearRainCounter = 0;
                mBoard->mNuclearRainActive = false;
            }
        }
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_INFECTION)
            {
                aGridItem->GridItemDie();
            }
        }
        if (mStateCountdown > 0)
        {
            mStateCountdown--;
        }
        else if (!mBoard->mNuclearRainActive) 
        {
            Die();
        }
    }
}

void Plant::UpdateFlowerPot()
{
    if (mState == PlantState::STATE_FLOWERPOT_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

void Plant::UpdateLilypad()
{
    if (mState == PlantState::STATE_LILYPAD_INVULNERABLE && mStateCountdown == 0)
        mState = PlantState::STATE_NOTREADY;
}

void Plant::UpdateCoffeeBean()
{
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            Die();
        }
    }
}

void Plant::UpdateUmbrella()
{
    if (mState == PlantState::STATE_UMBRELLA_TRIGGERED)
    {
        if (mStateCountdown == 0)
        {
            mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow + 1, 0);
            mState = PlantState::STATE_UMBRELLA_REFLECTING;
        }
    }
    else if (mState == PlantState::STATE_UMBRELLA_REFLECTING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayIdleAnim(0.0f);
            mState = PlantState::STATE_NOTREADY;
            mRenderOrder = CalcRenderOrder();
        }
    }
}

void Plant::UpdateCobCannon()
{
    if (mState == PlantState::STATE_COBCANNON_ARMING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_COBCANNON_LOADING;
            PlayBodyReanim("anim_charge", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_LOADING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.5f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_SHOOP);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_READY;
            PlayIdleAnim(12.0f);
        }
    }
    else if (mState == PlantState::STATE_COBCANNON_READY)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        ReanimatorTrackInstance* aCobTrack = aBodyReanim->GetTrackInstanceByName("CobCannon_cob");
        aCobTrack->mTrackColor = GetFlashingColor(mBoard->mMainCounter, 75);
    }
    else if (mState == PlantState::STATE_COBCANNON_FIRING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.48f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_COB_LAUNCH);
        }
    }
}

void Plant::UpdateCactus()
{
    if (mShootingCounter > 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    if (mState == PlantState::STATE_CACTUS_RISING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_HIGH;
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mLaunchCounter = 1;
        }
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY) == nullptr)
        {
            mState = PlantState::STATE_CACTUS_LOWERING;
            PlayBodyReanim("anim_lower", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        }
    }
    else if (mState == PlantState::STATE_CACTUS_LOWERING)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_CACTUS_LOW;
            PlayIdleAnim(0.0f);
        }
    }
    else if (FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY))
    {
        mState = PlantState::STATE_CACTUS_RISING;
        PlayBodyReanim("anim_rise", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, aBodyReanim->mDefinition->mFPS);
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
    }
}

void Plant::UpdateChomper()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    if (mHasBooster && mState == PlantState::STATE_DOINGSPECIAL)
    {
        if (mIsFlipped)
        {
            mBoostTimer = 0;
            mState = PlantState::STATE_READY;
            RemoveBoost();
            return;
        }

        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

        bool zombieEatenThisFrame = false;
        Zombie* aZombie = nullptr;

        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->IsFlying() ||
                (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER && aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING) ||
                (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mZombiePhase != ZombiePhase::PHASE_BUNGEE_AT_BOTTOM && aZombie->mZombiePhase != ZombiePhase::PHASE_BUNGEE_GRABBING))
            {
                continue;
            }

            if (aZombie->mRow == mRow && aZombie->mX > mX && aZombie->mX < mX + 800 && !aZombie->IsDeadOrDying())
            {
                bool isGargantuar = aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_EXA_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_BLEUEYE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR;

                aZombie->mPosX -= 5.0f;
                aZombie->mX = (int)aZombie->mPosX;

                if (aZombie->mX <= mX + 30 && mBoostFireTimer > 0)
                {
                    if (isGargantuar)
                    {
                        aZombie->TakeDamage(600, 0);
                        TodParticleSystem* aParticle = mApp->AddTodParticle(mX + 40.0f, mY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POTATO_MINE);
                        if (aParticle) aParticle->OverrideColor(nullptr, Color(180, 0, 255, 255));
                        mApp->PlayFoley(FoleyType::FOLEY_EXPLOSION);
                        mBoard->ShakeBoard(3, -4);
                        Die();
                        return;
                    }
                    else
                    {
                        aZombie->TakeDamage(600, 0);
                        if (aZombie->IsDeadOrDying()) aZombie->DieWithLoot();
                    }

                    mApp->PlayFoley(FoleyType::FOLEY_CHOMP);
                    mBoostFireTimer--;
                    zombieEatenThisFrame = true;
                }
            }
        }

        if (mBoostFireTimer <= 0 || mBoostTimer <= 0)
        {
            RemoveBoost();
            mBoostTimer = 0;
            mState = PlantState::STATE_CHOMPER_DIGESTING;
            mStateCountdown = 50;

            if (aBodyReanim)
            {
                aBodyReanim->mAnimRate = 30.0f;
                aBodyReanim->PlayReanim("anim_chew", ReanimLoopType::REANIM_LOOP, 0, 30.0f);
            }
        }
        return;
    }
    if (mState == PlantState::STATE_READY)
    {
        Zombie* aTargetZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
        GridItem* aGrave = FindTargetPVZ2Grave(mRow, PlantWeapon::WEAPON_PRIMARY);

        bool foundBackward = false;
        if (!aTargetZombie && !aGrave && !mHasBooster)
        {
            Rect aBackwardRect = Rect(mX - 80, mY, 80, mHeight);
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && !aZombie->mMindControlled)
                {
                    if (GetRectOverlap(aBackwardRect, aZombie->GetZombieRect()) > 0)
                    {
                        aTargetZombie = aZombie;
                        foundBackward = true;
                        break;
                    }
                }
            }
        }

        if (aTargetZombie || aGrave)
        {
            mIsFlipped = foundBackward;
            PlayBodyReanim("anim_bite", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            mState = PlantState::STATE_CHOMPER_BITING;
            mStateCountdown = 70;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING)
    {
        if (mStateCountdown == 0)
        {
            mApp->PlayFoley(FoleyType::FOLEY_BIGCHOMP);

            Zombie* aZombie = nullptr;
            GridItem* aGrave = nullptr;

            if (mIsFlipped)
            {
                Rect aBackwardRect = Rect(mX - 80, mY, 80, mHeight);
                Zombie* z = nullptr;
                while (mBoard->IterateZombies(z))
                {
                    if (z->mRow == mRow && !z->IsDeadOrDying() && !z->mMindControlled)
                    {
                        if (GetRectOverlap(aBackwardRect, z->GetZombieRect()) > 0)
                        {
                            aZombie = z;
                            break;
                        }
                    }
                }
            }
            else
            {
                aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
                aGrave = FindTargetPVZ2Grave(mRow, PlantWeapon::WEAPON_PRIMARY);
            }

            bool doBite = false;
            bool doMiss = false;

            if (aZombie)
            {
                if (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_EXA_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_BLEUEYE_GARGANTUAR ||
                    aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mZombieType == ZombieType::ZOMBIE_GIGA_FOOTBALL)
                {
                    doBite = true;
                }
                else if (!aZombie->IsImmobilizied())
                {
                    if (aZombie->IsBouncingPogo() ||
                        aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                    {
                        doMiss = true;
                    }
                }
            }
            else if (aGrave)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aGrave->TakeDamage(20);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
                return;
            }
            else
            {
                doMiss = true;
            }

            if (doBite)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                aZombie->TakeDamage(100, 0U);
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else if (doMiss)
            {
                mState = PlantState::STATE_CHOMPER_BITING_MISSED;
            }
            else
            {
                aZombie->DieWithLoot();
                mState = PlantState::STATE_CHOMPER_BITING_GOT_ONE;
            }
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_BITING_GOT_ONE)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mIsFlipped = false;
            PlayBodyReanim("anim_chew", ReanimLoopType::REANIM_LOOP, 0, 15.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0;
            }

            mState = PlantState::STATE_CHOMPER_DIGESTING;
            mStateCountdown = 2000;
        }
    }
    else if (mState == PlantState::STATE_CHOMPER_DIGESTING)
    {
        if (mStateCountdown == 0)
        {
            PlayBodyReanim("anim_swallow", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mState = PlantState::STATE_CHOMPER_SWALLOWING;
        }
    }
    else if ((mState == PlantState::STATE_CHOMPER_SWALLOWING || mState == PlantState::STATE_CHOMPER_BITING_MISSED) && aBodyReanim->mLoopCount > 0)
    {
        mIsFlipped = false;
        if (mState == PlantState::STATE_CHOMPER_SWALLOWING && mSeedType == SeedType::SEED_TOADSTOOL)
        {
            for (int i = 0; i < 3; i++)
            {
                float aOffsetX = RandRangeFloat(-20.0f, 20.0f);
                float aOffsetY = RandRangeFloat(-20.0f, 20.0f);
                mBoard->AddCoin(mX + 40 + (int)aOffsetX, mY + 40 + (int)aOffsetY, CoinType::COIN_SUN, CoinMotion::COIN_MOTION_FROM_PLANT);
            }
        }
        PlayIdleAnim(aBodyReanim->mDefinition->mFPS);
        mState = PlantState::STATE_READY;
    }
}
MagnetItem* Plant::GetFreeMagnetItem()
{
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        if (mMagnetItems[i].mItemType == MagnetItemType::MAGNET_ITEM_NONE)
        {
            return &mMagnetItems[i];
        }
    }
    return nullptr;
}

void Plant::MagnetShroomAttactItem(Zombie* theZombie)
{
    mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
    mStateCountdown = 1500;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);

    MagnetItem* aMagnetItem = GetFreeMagnetItem();
    if (!aMagnetItem) return;
    if (theZombie->mHelmType == HelmType::HELMTYPE_PAIL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("anim_bucket", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("anim_bucket", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_BUCKET1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 25.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_PAIL_1 + aDamageIndex);
    }
    else if (theZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL)
    {
        int aDamageIndex = theZombie->GetHelmDamageIndex();

        theZombie->mHelmHealth = 0;
        theZombie->mHelmType = HelmType::HELMTYPE_NONE;
        theZombie->GetTrackPosition("zombie_football_helmet", aMagnetItem->mPosX, aMagnetItem->mPosY);
        theZombie->ReanimShowPrefix("zombie_football_helmet", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("anim_hair", RENDER_GROUP_NORMAL);

        aMagnetItem->mPosX += 37.0f;
        aMagnetItem->mPosY -= 60.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        if (!theZombie->mIsEating)
        {
            TOD_ASSERT(theZombie->mZombieHeight == ZombieHeight::HEIGHT_ZOMBIE_NORMAL);
            theZombie->StartWalkAnim(0);
        }
        theZombie->GetTrackPosition("anim_screendoor", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_SCREENDOOR1->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_DOOR_1 + aDamageIndex);
    }
    else if (theZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER)
    {
        int aDamageIndex = theZombie->GetShieldDamageIndex();

        theZombie->DetachShield();

        aMagnetItem->mPosX = theZombie->mPosX + 31.0f;
        aMagnetItem->mPosY = theZombie->mPosY + 20.0f;
        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = (MagnetItemType)((int)MagnetItemType::MAGNET_ITEM_LADDER_1 + aDamageIndex);
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_POGO)
    {
        theZombie->PogoBreak(16U);
        // ZombieDrawPosition aDrawPos;
        // theZombie->GetDrawPos(aDrawPos);
        theZombie->GetTrackPosition("Zombie_pogo_stick", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX += 40.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetWidth() / 2;
        aMagnetItem->mPosY += 84.0f - IMAGE_REANIM_ZOMBIE_LADDER_5->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 30.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
        aMagnetItem->mItemType = theZombie->mHasArm ? MagnetItemType::MAGNET_ITEM_POGO_1 : MagnetItemType::MAGNET_ITEM_POGO_3;
    }
    else if (theZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING)
    {
        theZombie->StopZombieSound();
        theZombie->PickRandomSpeed();
        theZombie->mZombiePhase = ZombiePhase::PHASE_ZOMBIE_NORMAL;
        theZombie->ReanimShowPrefix("Zombie_jackbox_box", RENDER_GROUP_HIDDEN);
        theZombie->ReanimShowPrefix("Zombie_jackbox_handle", RENDER_GROUP_HIDDEN);
        theZombie->GetTrackPosition("Zombie_jackbox_box", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_JACKBOX_BOX->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 20.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX;
    }
    else if (theZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
    {
        theZombie->DiggerLoseAxe();
        theZombie->GetTrackPosition("Zombie_digger_pickaxe", aMagnetItem->mPosX, aMagnetItem->mPosY);

        aMagnetItem->mPosX -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetWidth() / 2;
        aMagnetItem->mPosY -= IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE->GetHeight() / 2;
        aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 45.0f;
        aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f) + 15.0f;
        aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_PICK_AXE;
    }
}

bool Plant::DrawMagnetItemsOnTop()
{
    if (mSeedType == SeedType::SEED_GOLD_MAGNET || mSeedType == SeedType::SEED_SUN_MAGNET)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            if (mMagnetItems[i].mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                return true;
            }
        }
        return false;
    }

    if (mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
        {
            MagnetItem* aMagnetItem = &mMagnetItems[i];
            if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
            {
                SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
                if (aVectorToPlant.Magnitude() > 20.0f)
                {
                    return true;
                }
            }
        }

        return false;
    }
}

void Plant::UpdateMagnetShroom()
{
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            if (aVectorToPlant.Magnitude() > 20.0f)
            {
                aMagnetItem->mPosX += aVectorToPlant.x * 0.05f;
                aMagnetItem->mPosY += aVectorToPlant.y * 0.05f;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;

            float aAnimRate = RandRangeFloat(10.0f, 15.0f);
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 30, aAnimRate);
            if (mApp->IsIZombieLevel())
            {
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

        
            for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
            {
                mMagnetItems[i].mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_nonactive_idle2", ReanimLoopType::REANIM_LOOP, 20, 2.0f);
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                aBodyReanim->mAnimRate = 0.0f;
            }

            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
        }
    }
    else
    {
        float aClosestDistance = 0.0f;
        Zombie* aClosestZombie = nullptr;

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            int aDiffY = aZombie->mRow - mRow;
            Rect aZombieRect = aZombie->GetZombieRect();

            if (aZombie->mMindControlled)
                continue;

            if (!aZombie->mHasHead)
                continue;

            if (aZombie->mZombieHeight != ZombieHeight::HEIGHT_ZOMBIE_NORMAL || aZombie->mZombiePhase == ZombiePhase::PHASE_RISING_FROM_GRAVE)
                continue;

            if (aZombie->IsDeadOrDying())
                continue;

            if (aZombieRect.mX > BOARD_WIDTH || aDiffY > 2 || aDiffY < -2)
                continue;

            if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING ||
                aZombie->mZombieType == ZombieType::ZOMBIE_POGO)
            {
                if (!aZombie->mHasObject)
                    continue;
            }
            else if (!(aZombie->mHelmType == HelmType::HELMTYPE_PAIL ||
                aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR ||
                aZombie->mShieldType == ShieldType::SHIELDTYPE_LADDER ||
                aZombie->mZombiePhase == ZombiePhase::PHASE_JACK_IN_THE_BOX_RUNNING))
                continue;

            int aRadius = aZombie->mIsEating ? 320 : 270;
            if (GetCircleRectOverlap(mX, mY + 20, aRadius, aZombieRect))
            {
                float aDistance = Distance2D(mX, mY, aZombieRect.mX, aZombieRect.mY);
                aDistance += abs(aDiffY) * 80.0f;

                if (aClosestZombie == nullptr || aDistance < aClosestDistance)
                {
                    aClosestZombie = aZombie;
                    aClosestDistance = aDistance;
                }
            }
        }

        if (aClosestZombie)
        {
            MagnetShroomAttactItem(aClosestZombie);
            return;
        }

        ////////////////////

        float aClosestLadderDist = 0.0f;
        GridItem* aClosestLadder = nullptr;
        
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
            {
                int aDiffX = abs(aGridItem->mGridX - mPlantCol);
                int aDiffY = abs(aGridItem->mGridY - mRow);
                int aSquareDistance = max(aDiffX, aDiffY);
                if (aSquareDistance <= 2)
                {
                    float aDistance = aSquareDistance + aDiffY * 0.05f;
                    if (aClosestLadder == nullptr || aDistance < aClosestLadderDist)
                    {
                        aClosestLadder = aGridItem;
                        aClosestLadderDist = aDistance;
                    }
                }
            }
        }

        if (aClosestLadder)
        {
            mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
            mStateCountdown = 1500;
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            aClosestLadder->GridItemDie();
            MagnetItem* aMagnetItem = GetFreeMagnetItem();
            if (aMagnetItem)
            {
                aMagnetItem->mPosX = mBoard->GridToPixelX(aClosestLadder->mGridX, aClosestLadder->mGridY) + 40;
                aMagnetItem->mPosY = mBoard->GridToPixelY(aClosestLadder->mGridX, aClosestLadder->mGridY);
                aMagnetItem->mDestOffsetX = RandRangeFloat(-10.0f, 10.0f) + 10.0f;
                aMagnetItem->mDestOffsetY = RandRangeFloat(-10.0f, 10.0f);
                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LADDER_PLACED;
            }
        }
    }
}

Coin* Plant::FindGoldMagnetTarget()
{
    Coin* aClosestCoin = nullptr;
    float aClosestDistance = 0.0f;

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if (aCoin->IsMoney() && aCoin->mCoinMotion != CoinMotion::COIN_MOTION_FROM_PRESENT && !aCoin->mIsBeingCollected && aCoin->mCoinAge >= 50)
        {
            float aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aCoin->mPosX + aCoin->mWidth / 2, aCoin->mPosY + aCoin->mHeight / 2);
            if (aClosestCoin == nullptr || aDistance < aClosestDistance)
            {
                aClosestCoin = aCoin;
                aClosestDistance = aDistance;
            }
        }
    }

    return aClosestCoin;
}

void Plant::GoldMagnetFindTargets()
{
    if (GetFreeMagnetItem() == nullptr)
    {
        TOD_ASSERT();
        return;
    }

    for (;;)
    {
        MagnetItem* aMagnetItem = GetFreeMagnetItem();
        if (aMagnetItem == nullptr)
            break;

        Coin* aCoin = FindGoldMagnetTarget();
        if (aCoin == nullptr)
            break;

        aMagnetItem->mPosX = aCoin->mPosX + 15.0f;
        aMagnetItem->mPosY = aCoin->mPosY + 15.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
        aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;

        switch (aCoin->mType)
        {
        case CoinType::COIN_SILVER:     aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SILVER_COIN;   break;
        case CoinType::COIN_GOLD:       aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_GOLD_COIN;     break;
        case CoinType::COIN_DIAMOND:    aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_DIAMOND;       break;
        default:                        TOD_ASSERT();                                                       return;
        }
        
        aCoin->Die();
    }
}

bool Plant::IsAGoldMagnetAboutToSuck()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_GOLD_MAGNET && aPlant->mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(aPlant->mBodyReanimID);
            if (aBodyReanim->mAnimTime < 0.5f)
            {
                return true;
            }
        }
    }

    return false;
}

void Plant::UpdateGoldMagnetShroom()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    bool aIsSuckingCoin = false;
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            float aDistance = aVectorToPlant.Magnitude();
            if (aDistance < 20.0f)
            {
                CoinType aCoinType;
                switch (aMagnetItem->mItemType)
                {
                case MagnetItemType::MAGNET_ITEM_SILVER_COIN:   aCoinType = CoinType::COIN_SILVER;      break;
                case MagnetItemType::MAGNET_ITEM_GOLD_COIN:     aCoinType = CoinType::COIN_GOLD;        break;
                case MagnetItemType::MAGNET_ITEM_DIAMOND:       aCoinType = CoinType::COIN_DIAMOND;     break;
                default:                                        TOD_ASSERT();                           return;
                }

                int aValue = Coin::GetCoinValue(aCoinType);
                mApp->mPlayerInfo->AddCoins(aValue);
                mBoard->mCoinsCollected += aValue;
                mApp->PlayFoley(FoleyType::FOLEY_COIN);

                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
            else
            {
                float aSpeed = TodAnimateCurveFloatTime(30.0f, 0.0f, aDistance, 0.02f, 0.05f, TodCurves::CURVE_LINEAR);
                aMagnetItem->mPosX += aVectorToPlant.x * aSpeed;
                aMagnetItem->mPosY += aVectorToPlant.y * aSpeed;

                aIsSuckingCoin = true;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            GoldMagnetFindTargets();
        }

        if (aBodyReanim->mLoopCount > 0 && !aIsSuckingCoin)
        {
            PlayIdleAnim(14.0f);
            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = RandRangeInt(200, 300);
        }
    }
    else if (!IsAGoldMagnetAboutToSuck() && Sexy::Rand(50) == 0 && FindGoldMagnetTarget())
    {
        mBoard->ShowCoinBank();
        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    }
}

void Plant::RemoveEffects()
{
    mApp->RemoveParticle(mParticleID);
    if (mBoostGlowParticleID != ParticleSystemID::PARTICLESYSTEMID_NULL)
    {
        mApp->RemoveParticle(mBoostGlowParticleID);
        mBoostGlowParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    }
    if (mSunMagnetAuraID != ParticleSystemID::PARTICLESYSTEMID_NULL)
    {
        mApp->RemoveParticle(mSunMagnetAuraID);
        mSunMagnetAuraID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    }
    mApp->RemoveReanimation(mBodyReanimID);
    mApp->RemoveReanimation(mHeadReanimID);
    mApp->RemoveReanimation(mHeadReanimID2);
    mApp->RemoveReanimation(mHeadReanimID3);
    mApp->RemoveReanimation(mLightReanimID);
    mApp->RemoveReanimation(mBlinkReanimID);
    mApp->RemoveReanimation(mSleepingReanimID);
}

void Plant::Squish()
{
    if (mHasBooster) 
        return;
    if (NotOnGround())
        return;
    if (mBoostGlowParticleID != ParticleSystemID::PARTICLESYSTEMID_NULL)
    {
        mApp->RemoveParticle(mBoostGlowParticleID);
        mBoostGlowParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    }
    if (!mIsAsleep)
    {
        if (mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO || mSeedType == SeedType::SEED_CUCKUMBER ||
            mSeedType == SeedType::SEED_DOOMSHROOM || mSeedType == SeedType::SEED_XSHROOM || mSeedType == SeedType::SEED_ICESHROOM || mSeedType == SeedType::SEED_EXPLODE_O_NUT)
        {
            DoSpecial();
            return;
        }
        else if ((mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_PRIMALPOTATOMINE) && mState != PlantState::STATE_NOTREADY)
        {
            DoSpecial();
            return;
        }
    }

    if (mSeedType == SeedType::SEED_SQUASH && mState != PlantState::STATE_NOTREADY)
        return;

    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GRAVE_STONE, mRow, 8);
    mSquished = true;
    mDisappearCountdown = 500;
    mApp->PlayFoley(FoleyType::FOLEY_SQUISH);
    RemoveEffects();

    GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
    if (aLadder)
    {
        aLadder->GridItemDie();
    }

    if (mApp->IsIZombieLevel())
    {
        mBoard->mChallenge->IZombiePlantDropRemainingSun(this);
    }
}

void Plant::UpdateBowling()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim && aBodyReanim->TrackExists("_ground"))
    {
        float aSpeed = aBodyReanim->GetTrackVelocity("_ground");
        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSpeed *= 2;
        }

        mX -= aSpeed;
        if (mX > 800 + BOARD_ADDITIONAL_WIDTH)
            Die();
    }

    if (mState == PlantState::STATE_BOWLING_UP)
    {
        mY -= 2;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN)
    {
        mY += 2;
    }

    int aDistToGrid = mBoard->GridToPixelY(0, mRow) - mY;
    if (aDistToGrid < -2 || aDistToGrid > 2)
        return;

    PlantState aNewState = mState;
    if (mState == PlantState::STATE_BOWLING_UP && mRow <= 0)
    {
        aNewState = PlantState::STATE_BOWLING_DOWN;
    }
    else if (mState == PlantState::STATE_BOWLING_DOWN && mRow >= 4)
    {
        aNewState = PlantState::STATE_BOWLING_UP;
    }

    Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
    if (aZombie)
    {
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;

        if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)
        {
            mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
            mApp->PlaySample(SOUND_BOWLINGIMPACT2);

            int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY) | 32U;
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 90, 1, true, aDamageRangeFlags);
            mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
            mBoard->ShakeBoard(3, -4);

            mState = PlantState::STATE_DOINGSPECIAL;
            Die();
            return;
        }

        mApp->PlayFoley(FoleyType::FOLEY_BOWLINGIMPACT);
        mBoard->ShakeBoard(1, -2);

        if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aZombie->TakeDamage(1800, 0U);
        }
        else if (aZombie->mShieldType == ShieldType::SHIELDTYPE_DOOR && mState != PlantState::STATE_NOTREADY)
        {
            aZombie->TakeDamage(1800, 0U);
        }
        else if (aZombie->mShieldType != ShieldType::SHIELDTYPE_NONE)
        {
            aZombie->TakeShieldDamage(400, 0U);
        }
        else if (aZombie->mHelmType != HelmType::HELMTYPE_NONE)
        {
            if (aZombie->mHelmType == HelmType::HELMTYPE_PAIL)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
            }
            else if (aZombie->mHelmType == HelmType::HELMTYPE_TRAFFIC_CONE || aZombie->mHelmType == HelmType::HELMTYPE_BRICK || aZombie->mHelmType == HelmType::HELMTYPE_DIGGER || aZombie->mHelmType == HelmType::HELMTYPE_FOOTBALL)
            {
                mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
            }
            aZombie->TakeHelmDamage(900, 0U);
        }
        else
        {
            aZombie->TakeDamage(1800, 0U);
        }

        if ((!mApp->IsFirstTimeAdventureMode() || mApp->mPlayerInfo->mLevel > 10) && mSeedType == SeedType::SEED_WALLNUT && !mApp->mPlayingQuickplay)
        {
            mLaunchCounter++;
            if (mLaunchCounter == 2)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 3)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 5.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter == 4)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX - 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
                mBoard->AddCoin(aPosX + 10.0f, aPosY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
            }
            else if (mLaunchCounter >= 5)
            {
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
                mBoard->AddCoin(aPosX, aPosY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
                mApp->GetAchievement(ACHIEVEMENT_ROLL_SOME_HEADS);
            }
        }

        if (mSeedType != SeedType::SEED_GIANT_WALLNUT)
        {
            if (mRow == 4 || mState == PlantState::STATE_BOWLING_DOWN)
            {
                aNewState = PlantState::STATE_BOWLING_UP;
            }
            else if (mRow == 0 || mState == PlantState::STATE_BOWLING_UP)
            {
                aNewState = PlantState::STATE_BOWLING_DOWN;
            }
            else
            {
                aNewState = Sexy::Rand(2) ? PlantState::STATE_BOWLING_UP : PlantState::STATE_BOWLING_DOWN;
            }
        }
    }

    if (aNewState == PlantState::STATE_BOWLING_UP)
    {
        mRow--;
        mState = PlantState::STATE_BOWLING_UP;
        mRenderOrder = CalcRenderOrder();
    }
    else if (aNewState == PlantState::STATE_BOWLING_DOWN)
    {
        mRow++;
        mState = PlantState::STATE_BOWLING_DOWN;
        mRenderOrder = CalcRenderOrder();
    }
}

void Plant::UpdateAbilities()
{
    if (!IsInPlay() || mDead)
        return;
    if (mSquished)
    {
        mDisappearCountdown--;
        if (mDisappearCountdown < 0)
        {
            Die();
        }
        return;
    }
    if (mSeedType == SeedType::SEED_MAGNIFYING_GRASS)
    {
        UpdateMagnifyingGrass();
    }
    if (mSeedType == SeedType::SEED_SPY_PEPPER)
    {
        if (mState == PlantState::STATE_NOTREADY)
        {
            if (mStateCountdown > 0) mStateCountdown--;
            if (mStateCountdown <= 0)
            {
                Zombie* aTarget = nullptr;
                int maxHealth = -1;
                Zombie* z = nullptr;
                while (mBoard->IterateZombies(z))
                {
                    if (z->mRow == mRow && !z->IsDeadOrDying() && !z->mMindControlled && z->mZombieType != ZombieType::ZOMBIE_BOSS)
                    {
                        int h = z->mBodyHealth + z->mHelmHealth + z->mShieldHealth;
                        if (h > maxHealth)
                        {
                            maxHealth = h;
                            aTarget = z;
                        }
                    }
                }

                if (aTarget)
                {
                    TodParticleSystem* p = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_IMITATER_MORPH);
                    if (p) p->OverrideColor(nullptr, Color(139, 69, 19, 255));
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);

                    Zombie* newZ = mBoard->AddZombieInRow(aTarget->mZombieType, mRow, mBoard->mCurrentWave);
                    if (newZ)
                    {
                        newZ->mPosX = mX;
                        newZ->mPosY = mY;
                        newZ->mX = mX;
                        newZ->mY = (int)newZ->mPosY;
                        newZ->StartMindControlled();
                        newZ->mIsSpyPepper = true;
                        newZ->mBodyMaxHealth = (int)(newZ->mBodyMaxHealth * 1.25f);
                        newZ->mBodyHealth = newZ->mBodyMaxHealth;
                        newZ->mHelmMaxHealth = (int)(newZ->mHelmMaxHealth * 1.25f);
                        newZ->mHelmHealth = newZ->mHelmMaxHealth;
                        newZ->mShieldMaxHealth = (int)(newZ->mShieldMaxHealth * 1.25f);
                        newZ->mShieldHealth = newZ->mShieldMaxHealth;
                        mTargetZombieID = mBoard->ZombieGetID(newZ);
                    }
                    mState = PlantState::STATE_DOINGSPECIAL;
                    mX = -2000; 
                }
                else
                {
                    mStateCountdown = 50;
                }
            }
        }
        else if (mState == PlantState::STATE_DOINGSPECIAL)
        {
            Zombie* z = mBoard->ZombieTryToGet(mTargetZombieID);
            if (z && z->mPosX >= 1150.0f) 
            {
                z->TakeDamage(99999, 0U);
                z->ApplyBurn();
            }

            if (!z || z->IsDeadOrDying())
            {
                mTargetZombieID = ZombieID::ZOMBIEID_NULL;
                float deathX = z ? z->mPosX : 1150.0f;

                mX = deathX;
                mY = mBoard->GridToPixelY(mPlantCol, mRow);
                mState = PlantState::STATE_SPIKEWEED_ATTACKING; 
                mStateCountdown = 50;
            }
        }
        else if (mState == PlantState::STATE_SPIKEWEED_ATTACKING)
        {
            if (mStateCountdown > 0) mStateCountdown--;
            if (mStateCountdown <= 0)
            {
                TodParticleSystem* p1 = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_FIREBALL_DEATH);
                if (p1) p1->OverrideColor(nullptr, Color(139, 0, 0, 255)); 
                mX = mBoard->GridToPixelX(mPlantCol, mRow);
                mY = mBoard->GridToPixelY(mPlantCol, mRow);
                TodParticleSystem* p2 = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_FIREBALL_DEATH);
                if (p2) p2->OverrideColor(nullptr, Color(139, 0, 0, 255));
                mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                mCoconutClickCount++; 
                if (mCoconutClickCount >= 3)
                {
                    Die();
                }
                else
                {
                    mState = PlantState::STATE_NOTREADY;
                    mStateCountdown = 100;
                }
            }
        }
    }
    if (mSeedType == SeedType::SEED_TRAMPALINE)
    {
        if (mState == PlantState::STATE_NOTREADY)
        {
            if (mStateCountdown > 0) mStateCountdown--;
            if (mStateCountdown <= 0)
            {
                mTrampalineCharges = 3;
                mState = PlantState::STATE_READY;
                PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
            }
        }
        else if (mState == PlantState::STATE_READY)
        {
            float plantCenterX = mX + 40.0f;
            ZombieID zombiesToLaunch[10];
            int launchCount = 0;

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && !aZombie->IsFlying() &&
                    aZombie->mZombieType != ZombieType::ZOMBIE_BOSS &&
                    aZombie->mZombieType != ZombieType::ZOMBIE_DIGGER &&
                    aZombie->mZombiePhase != ZombiePhase::PHASE_TRAMPALINE_BOUNCE)
                {
                    float zombieCenterX = aZombie->mPosX + 40.0f;
                    if (abs(zombieCenterX - plantCenterX) <= 20.0f)
                    {
                        if (launchCount < 10)
                        {
                            zombiesToLaunch[launchCount++] = mBoard->ZombieGetID(aZombie);
                        }
                    }
                }
            }

            if (launchCount > 0)
            {
                mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
                PlayBodyReanim("anim_jumpup", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);

                for (int i = 0; i < launchCount; i++)
                {
                    Zombie* z = mBoard->ZombieTryToGet(zombiesToLaunch[i]);
                    if (z) {
                        if (z->mZombieType == ZombieType::ZOMBIE_BUNGEE) {
                            mApp->PlaySample(SOUND_BOING);
                            mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
                            z->mZombiePhase = ZombiePhase::PHASE_BUNGEE_RISING;
                            z->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
                            z->mHitUmbrella = true;
                        }
                        else {
                            int tilesToThrow = (z->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_BLEUEYE_GARGANTUAR || z->mZombieType == ZombieType::ZOMBIE_EXA_GARGANTUAR) ? 2 : 4;

                            z->mLastPortalX = (int)z->mZombiePhase;

                            z->mZombiePhase = ZombiePhase::PHASE_TRAMPALINE_BOUNCE;
                            z->mVelZ = 4.0f;
                            float airTime = (z->mVelZ * 2.0f) / 0.05f;
                            float speedX = (tilesToThrow * 80.0f) / airTime;
                            z->mVelX = mTrampolineReversed ? -speedX : speedX;
                        }
                    }
                }

                mTrampalineCharges--;
                if (mTrampalineCharges <= 0)
                {
                    mState = PlantState::STATE_NOTREADY;
                    mStateCountdown = 2000;
                    PlayBodyReanim("anim_armed", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 12.0f);
                }
            }
            else
            {
                Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
                if (aBodyReanim && aBodyReanim->IsAnimPlaying("anim_jumpup") && aBodyReanim->mLoopCount > 0 && mTrampalineCharges > 0)
                {
                    PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 12.0f);
                }
            }
        }
    }
    if (mSeedType == SeedType::SEED_HAMMER_SHROOM && !mIsAsleep)
    {
        if (mStateCountdown > 0)
        {
            mStateCountdown--;
        }

        if (mState == PlantState::STATE_NOTREADY || mState == PlantState::STATE_READY)
        {
            if (mStateCountdown <= 0)
            {
                bool hasTarget = false;
                Zombie* aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie))
                {
                    if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying())
                    {
                        float dist = aZombie->mPosX - mX;
                        if (dist >= -40.0f && dist <= 120.0f)
                        {
                            hasTarget = true;
                            break;
                        }
                    }
                }

                if (hasTarget)
                {
                    mState = PlantState::STATE_DOINGSPECIAL;
                    PlayBodyReanim("anim_attack", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 30.0f);
                }
            }
        }
        else if (mState == PlantState::STATE_DOINGSPECIAL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim && aBodyReanim->ShouldTriggerTimedEvent(0.5f))
            {
                bool hitTarget = false;
                Zombie* aZombie = nullptr;
                while (mBoard->IterateZombies(aZombie))
                {
                    if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying())
                    {
                        float dist = aZombie->mPosX - mX;
                        if (dist >= -40.0f && dist <= 120.0f)
                        {
                            hitTarget = true;
                            aZombie->TakeDamage(25, 0U);
                            if (aZombie->mZombieType != ZombieType::ZOMBIE_BOSS && !aZombie->IsFlying())
                            {
                                aZombie->mPosX += 5.0f;
                                aZombie->mX = (int)aZombie->mPosX;
                            }
                        }
                    }
                }

                if (hitTarget)
                {
                    mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                }
            }
            if (aBodyReanim && aBodyReanim->mLoopCount > 0)
            {
                mState = PlantState::STATE_READY;
                mStateCountdown = 135;
                PlayIdleAnim(15.0f);
            }
        }
    }
    if (mSeedType == SeedType::SEED_TIME_APPLE)
    {
        if (mState == PlantState::STATE_DOINGSPECIAL)
        {
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying())
                {
                    aZombie->mVelX = 0.0f;
                    aZombie->mIsEating = false;
                    aZombie->mAnimCounter = 0;
                    Reanimation* aZombieReanim = mApp->ReanimationTryToGet(aZombie->mBodyReanimID);
                    if (aZombieReanim)
                    {
                        aZombieReanim->mAnimRate = 0.0f;
                    }
                }
            }
            for (int i = 0; i < mBoard->mSeedBank->mNumPackets; i++)
            {
                SeedPacket* aPacket = &mBoard->mSeedBank->mSeedPackets[i];
                if (aPacket->mPacketType != SeedType::SEED_NONE && aPacket->mRefreshCounter > 0 && aPacket->mRefreshCounter < aPacket->mRefreshTime)
                {
                    aPacket->mRefreshCounter++;
                }
            }
            mStateCountdown--;
            if (mStateCountdown <= 0)
            {
                Zombie* aZombieToFix = nullptr;
                while (mBoard->IterateZombies(aZombieToFix))
                {
                    if (!aZombieToFix->IsDeadOrDying())
                    {
                        aZombieToFix->mIsEating = false;
                        aZombieToFix->PickRandomSpeed();
                        aZombieToFix->UpdateAnimSpeed();
                    }
                }
                TodParticleSystem* p = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_IMITATER_MORPH);
                if (p) p->OverrideColor(nullptr, Color(255, 0, 0, 255));
                Die();
            }
        }
    }
    if (mHasBooster && (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK))
    {
        int toggleSpeed = 15;
        bool isVisible = (mBoostTimer / toggleSpeed) % 2 == 0;
        bool isDamageFrame = (isVisible && mBoostTimer % toggleSpeed == toggleSpeed - 1);
        if (isDamageFrame)
        {
            mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }

        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && !aZombie->IsFlying() && !aZombie->mMindControlled)
            {
                float plantCenterX = mX + 40.0f;
                float zombieCenterX = aZombie->mPosX + 40.0f;
                if (abs(zombieCenterX - plantCenterX) > 4.0f)
                {
                    float pullSpeed = (mSeedType == SeedType::SEED_SPIKEROCK) ? 4.0f : 3.0f;

                    if (zombieCenterX > plantCenterX)
                    {
                        aZombie->mPosX -= pullSpeed;
                    }
                    else
                    {
                        aZombie->mPosX += pullSpeed;
                    }
                    aZombie->mX = (int)aZombie->mPosX;
                }
                if (isDamageFrame)
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
                    if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                    {
                        int damage = (mSeedType == SeedType::SEED_SPIKEROCK) ? 450 : 150;
                        aZombie->TakeDamage(damage, 33U);
                        mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
                    }
                }
            }
        }
    }
    // في نهاية الدالة UpdateAbilities حيث تتم معالجة mHasBooster
    if (mHasBooster)
    {
        if (mSeedType != SeedType::SEED_HYPNOSHROOM)
        {
            if (mBoostTimer > 0)
            {
                mBoostTimer--;
            }
            else
            {
                RemoveBoost();
            }
        }
        if (mSeedType == SeedType::SEED_CATTAIL && mBoostFireTimer > 0)
        {
            mBoostFireTimer--;
            Zombie* aTarget = FindBoostedSquashTarget();
            if (aTarget)
            {
                int aOriginX = mX + 20;
                int aOriginY = mY - 3;
                Projectile* p = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_CATTAIL_BOOST);
                if (p)
                {
                    p->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
                    p->mMotionType = ProjectileMotion::MOTION_HOMING;
                    p->mTargetZombieID = mBoard->ZombieGetID(aTarget);
                    p->mVelX = 4.0f;
                }
                mApp->PlayFoley(FoleyType::FOLEY_THROW);
            }
            RemoveBoost();
            mBoostTimer = 0;
        }

        if (mSeedType == SeedType::SEED_STARFRUIT && mBoostFireTimer > 0)
        {
            mBoostFireTimer--;
            if (mBoostFireTimer % 2 == 0)
            {
                int aOriginX = mX + 25;
                int aOriginY = mY + 25;
                float rotationOffset = (200.0f - (float)mBoostFireTimer) * 7.5f;
                float baseAngles[5] = { 180.0f, 90.0f, -90.0f, 30.0f, -30.0f };

                for (int i = 0; i < 5; i++)
                {
                    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_STAR);
                    if (aProjectile)
                    {
                        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
                        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
                        float finalAngle = DEG_TO_RAD(baseAngles[i] + rotationOffset);
                        float speed = 4.0f;
                        aProjectile->mVelX = cos(finalAngle) * speed;
                        aProjectile->mVelY = sin(finalAngle) * speed;
                    }
                }
                if (mBoostFireTimer % 4 == 0) mApp->PlayFoley(FoleyType::FOLEY_THROW);
            }
        }

        if (mSeedType == SeedType::SEED_GARLIC)
        {
            bool isParticleFrame = false;
            if (mBoostFireTimer <= 0) { isParticleFrame = true; mBoostFireTimer = 50; }
            else { mBoostFireTimer--; }
            bool isEnding = (mBoostTimer == 1);

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (aZombie->IsDeadOrDying() || aZombie->mMindControlled) continue;
                if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS ||
                    (aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER && (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_TUNNELING || aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_STUNNED)) ||
                    (aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING) ||
                    (aZombie->mZombieType == ZombieType::ZOMBIE_SNORKEL && aZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL))
                    continue;

                int zCol = mBoard->PixelToGridXKeepOnBoard(aZombie->mX, aZombie->mY);
                int colDiff = abs(zCol - mPlantCol);
                int rowDiff = abs(aZombie->mRow - mRow);

                if (colDiff <= 2 && rowDiff <= 3)
                {
                    Reanimation* aZombieReanim = mApp->ReanimationTryToGet(aZombie->mBodyReanimID);
                    if (!isEnding) {
                        aZombie->mVelX = 0.0f; aZombie->mIsEating = false;
                        if (aZombieReanim) aZombieReanim->mAnimRate = 0.0f;
                        if (isParticleFrame) {
                            int aRenderPos = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, aZombie->mRow, 0);
                            TodParticleSystem* aToxic = mApp->AddTodParticle(aZombie->mX + 40, aZombie->mY + 40, aRenderPos, ParticleEffect::PARTICLE_FIREBALL_DEATH);
                            if (aToxic) { aToxic->OverrideScale(nullptr, 0.4f); aToxic->OverrideColor(nullptr, Color(130, 150, 130, 220)); }
                        }
                    }
                    else {
                        if (aZombieReanim) aZombieReanim->mAnimRate = aZombieReanim->mDefinition->mFPS;
                    }
                }
            }
            mBoostTimer--;
            if (mBoostTimer <= 0) RemoveBoost();
            return;
        }
        if (MakesSun() && mBoostFireTimer > 0)
        {
            if (mBoostTimer % 10 == 0)
            {
                mBoostFireTimer--;
                CoinType aType = CoinType::COIN_SUN;
                if (mSeedType == SeedType::SEED_PRIMALSUNFLOWER) aType = CoinType::COIN_BIGSUN;
                else if (mSeedType == SeedType::SEED_MARIGOLD) aType = (Sexy::Rand(100) < 10) ? CoinType::COIN_GOLD : CoinType::COIN_SILVER;

                float aOffsetX = RandRangeFloat(-40.0f, 40.0f);
                float aOffsetY = RandRangeFloat(-40.0f, 40.0f);
                mBoard->AddCoin(mX + 40 + (int)aOffsetX, mY + 40 + (int)aOffsetY, aType, CoinMotion::COIN_MOTION_FROM_PLANT);
                mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
            }
        }

        if (mSeedType == SeedType::SEED_PUFFSHROOM && mBoostFireTimer > 0)
        {
            if (mBoostTimer % 5 == 0)
            {
                mBoostFireTimer--;
                Reanimation* aHead = mApp->ReanimationTryToGet(mHeadReanimID);
                if (aHead) {
                    if (aHead->mLoopType != ReanimLoopType::REANIM_LOOP) {
                        aHead->SetFramesForLayer("anim_shooting"); aHead->mLoopType = ReanimLoopType::REANIM_LOOP; aHead->mAnimRate = 45.0f;
                    }
                }
                else {
                    Reanimation* aBody = mApp->ReanimationTryToGet(mBodyReanimID);
                    if (aBody && aBody->mLoopType != ReanimLoopType::REANIM_LOOP) {
                        PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_LOOP, 0, 45.0f);
                    }
                }

                Projectile* aProjectile = mBoard->AddProjectile(mX + 40, mY + 20, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_PUFF);
                if (aProjectile)
                {
                    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
                    aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
                    aProjectile->mVelX = 5.0f;
                    aProjectile->mVelY = 0.0f;
                }
                mApp->PlayFoley(FoleyType::FOLEY_PUFF);
            }
        }

        bool isPeaStyleShooter =
            mSeedType == SeedType::SEED_PEASHOOTER ||
            mSeedType == SeedType::SEED_REPEATER ||
            mSeedType == SeedType::SEED_LEFTPEATER ||
            mSeedType == SeedType::SEED_THREEPEATER ||
            mSeedType == SeedType::SEED_GATLINGPEA ||
            mSeedType == SeedType::SEED_SPLITPEA ||
            mSeedType == SeedType::SEED_SNOWPEA ||
            mSeedType == SeedType::SEED_SEASHROOM ||
            mSeedType == SeedType::SEED_SCAREDYSHROOM ||
            mSeedType == SeedType::SEED_CACTUS;

        if (isPeaStyleShooter && mBoostFireTimer > 0)
        {
            mBoostFireTimer--;

            Reanimation* aHead = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHead) {
                if (aHead->mLoopType != ReanimLoopType::REANIM_LOOP) {
                    aHead->SetFramesForLayer("anim_shooting");
                    aHead->mLoopType = ReanimLoopType::REANIM_LOOP;
                    aHead->mAnimRate = 45.0f;
                }
            }
            else {
                Reanimation* aBody = mApp->ReanimationTryToGet(mBodyReanimID);
                if (aBody && aBody->mLoopType != ReanimLoopType::REANIM_LOOP) {
                    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_LOOP, 0, 45.0f);
                }
            }

            ProjectileType aProjectileType = ProjectileType::PROJECTILE_PEA;
            switch (mSeedType)
            {
            case SeedType::SEED_SNOWPEA:        aProjectileType = ProjectileType::PROJECTILE_SNOWPEA; break;
            case SeedType::SEED_PUFFSHROOM:
            case SeedType::SEED_SCAREDYSHROOM:  aProjectileType = ProjectileType::PROJECTILE_PUFF; break;
            case SeedType::SEED_SEASHROOM:      aProjectileType = ProjectileType::PROJECTILE_PUFF2; break;
            default:                            aProjectileType = ProjectileType::PROJECTILE_PEA; break;
            }

            bool shootBackward = false;
            float totalBullets = 100.0f;

            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_SNOWPEA) {
                totalBullets = 50.0f;
            }
            else if (mSeedType == SeedType::SEED_THREEPEATER) {
                totalBullets = 300.0f;
            }
            else if (mSeedType == SeedType::SEED_GATLINGPEA) {
                totalBullets = 200.0f;
            }
            else if (mSeedType == SeedType::SEED_LEFTPEATER) {
                shootBackward = true;
                totalBullets = 100.0f;
            }
            else if (mSeedType == SeedType::SEED_SPLITPEA) {
                totalBullets = 150.0f;
                if (mBoostFireTimer < 100) {
                    shootBackward = true;
                    Reanimation* aHead2 = mApp->ReanimationTryToGet(mHeadReanimID2);
                    if (aHead2) {
                        aHead2->SetFramesForLayer("anim_splitpea_shooting");
                        aHead2->mAnimTime = 0.0f;
                        aHead2->mAnimRate = 45.0f;
                    }
                }
            }

            int aOriginX = mX + 40;
            int aOriginY = mY + 20;

            if (mSeedType != SeedType::SEED_PUFFSHROOM && mSeedType != SeedType::SEED_SEASHROOM && mSeedType != SeedType::SEED_SCAREDYSHROOM && mSeedType != SeedType::SEED_CACTUS && mSeedType != SeedType::SEED_THREEPEATER)
            {
                int aOffsetX = 0, aOffsetY = 0;
                GetPeaHeadOffset(aOffsetX, aOffsetY);
                aOriginX = mX + aOffsetX + 24;
                aOriginY = mY + aOffsetY - 33;
            }
            else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
            {
                aOriginX = mX + 29;
                aOriginY = mY + 26;
            }
            else if (mSeedType == SeedType::SEED_THREEPEATER)
            {
                aOriginX = mX + 45;
                aOriginY = mY + 10;
            }

            if (shootBackward)
            {
                if (mSeedType == SeedType::SEED_SPLITPEA) aOriginX = mX - 40;
                else if (mSeedType == SeedType::SEED_LEFTPEATER) aOriginX = mX - 6;
                else aOriginX = mX - 10;
            }

            float progress = (float)mBoostFireTimer / totalBullets;

            if (mSeedType == SeedType::SEED_THREEPEATER)
            {
                float baseVelY[3] = { -3.5f, 0.0f, 3.5f };
                int rows[3] = { mRow - 1, mRow, mRow + 1 };

                for (int i = 0; i < 3; i++)
                {
                    if (rows[i] >= 0 && rows[i] < MAX_GRID_SIZE_Y)
                    {
                        Projectile* p = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, rows[i], aProjectileType);
                        if (p)
                        {
                            p->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
                            p->mMotionType = ProjectileMotion::MOTION_STAR;
                            float speedX = 3.5f + (progress * 4.0f);
                            p->mVelX = shootBackward ? -speedX : speedX;
                            p->mVelY = baseVelY[i] + (sin((float)mBoostFireTimer * 0.5f) * 2.50f);
                        }
                    }
                }
            }
            else if (mSeedType == SeedType::SEED_CACTUS)
            {
                float speedX = 3.5f + (progress * 4.0f);
                Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_SPIKE);
                if (aProjectile)
                {
                    aProjectile->mDamageRangeFlags = 3U;
                    aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
                    aProjectile->mVelX = speedX;
                    aProjectile->mVelY = sin((float)mBoostFireTimer * 0.5f) * 1.30f;
                }
                if (mBoostFireTimer % 3 == 0) mApp->PlayFoley(FoleyType::FOLEY_THROW);
            }
            else
            {
                Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, mRow, aProjectileType);
                if (aProjectile)
                {
                    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(shootBackward ? PlantWeapon::WEAPON_SECONDARY : PlantWeapon::WEAPON_PRIMARY);

                    if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
                    {
                        aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
                        aProjectile->mVelY = sin((float)mBoostFireTimer * 0.4f) * 2.5f;
                    }
                    else
                    {
                        aProjectile->mMotionType = ProjectileMotion::MOTION_STAR;
                        float speedX = 3.5f + (progress * 4.0f);
                        aProjectile->mVelX = shootBackward ? -speedX : speedX;
                        aProjectile->mVelY = sin((float)mBoostFireTimer * 0.5f) * 1.30f;
                    }
                }
            }

            if (mSeedType != SeedType::SEED_CACTUS && mBoostFireTimer % 3 == 0) mApp->PlayFoley(FoleyType::FOLEY_THROW);
        }
        bool isLobbedStyleShooter =
            mSeedType == SeedType::SEED_CABBAGEPULT ||
            mSeedType == SeedType::SEED_KERNELPULT ||
            mSeedType == SeedType::SEED_MELONPULT ||
            mSeedType == SeedType::SEED_PEPPERPULT ||
            mSeedType == SeedType::SEED_WINTERMELON;

        if (isLobbedStyleShooter && mBoostFireTimer > 0)
        {
            mBoostFireTimer--;
          

            ProjectileType aProjType = ProjectileType::PROJECTILE_MELON;
            if (mSeedType == SeedType::SEED_WINTERMELON) aProjType = ProjectileType::PROJECTILE_WINTERMELON;
            else if (mSeedType == SeedType::SEED_CABBAGEPULT) aProjType = ProjectileType::PROJECTILE_CABBAGE;
            else if (mSeedType == SeedType::SEED_PEPPERPULT) aProjType = ProjectileType::PROJECTILE_PEPPER;
            else if (mSeedType == SeedType::SEED_KERNELPULT) aProjType = (Sexy::Rand(2) == 0) ? ProjectileType::PROJECTILE_BUTTER : ProjectileType::PROJECTILE_KERNEL;

            int maxRows = mBoard->StageHas6Rows() ? 6 : 5;
            for (int i = 0; i < 2; i++)
            {
                int randRow = Sexy::Rand(maxRows);
                int randCol = Sexy::Rand(MAX_GRID_SIZE_X);
                int targetX = mBoard->GridToPixelX(randCol, randRow) + Sexy::Rand(60) - 30;
                int targetY = mBoard->GridToPixelY(randCol, randRow) + Sexy::Rand(40) - 20;

                Projectile* p = mBoard->AddProjectile(targetX, targetY, Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, randRow, 0), randRow, aProjType);
                if (p)
                {
                    p->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
                    p->mMotionType = ProjectileMotion::MOTION_LOBBED;
                    p->mPosX = (float)targetX;
                    p->mPosY = (float)targetY;
                    p->mPosZ = -700.0f - (float)(Sexy::Rand(250));
                    p->mVelX = 0.0f;
                    p->mVelY = 0.0f;
                    p->mVelZ = 14.0f + (float)(Sexy::Rand(5));
                    p->mAccZ = 0.0f;
                    p->mProjectileAge = 0;
                    p->mRotationSpeed = ((float)(Sexy::Rand(100)) / 100.0f) * 0.15f - 0.075f;
                    p->mShadowY = (float)targetY + 60.0f;
                    if (mBoard->StageHasRoof()) p->mShadowY -= 12.0f;
                }
            }
        }
    }
    if (mStateCountdown == -1)
    {
        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim && aBodyReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && aBodyReanim->mLoopCount > 0)
        {
            float aPosX = mX + 50.0f;
            float aPosY = mY + 40.0f;
            if (mSeedType == SeedType::SEED_FUMESHROOM)
                aPosX += 12.0f;
            else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
                aPosY -= 20.0f;
            else if (mSeedType == SeedType::SEED_GLOOMSHROOM)
                aPosY -= 12.0f;

            Reanimation* aSleepReanim = mApp->AddReanimation(aPosX, aPosY, mRenderOrder + 2, ReanimationType::REANIM_SLEEPING);
            if (aSleepReanim)
            {
                aSleepReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aSleepReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
                aSleepReanim->mAnimTime = RandRangeFloat(0.0f, 0.9f);
                mSleepingReanimID = mApp->ReanimationGetID(aSleepReanim);
            }

            if (aBodyReanim)
            {
                if (!IsInPlay() && mSeedType == SeedType::SEED_SUNSHROOM)
                {
                    aBodyReanim->SetFramesForLayer("anim_bigsleep");
                    aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                    aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
                }
                else if (aBodyReanim->TrackExists("anim_sleep"))
                {
                    PlayBodyReanim("anim_sleep", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(6.0f, 8.0f));
                }
                else
                {
                    aBodyReanim->mAnimRate = RandRangeFloat(6.0f, 8.0f);
                    aBodyReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                }
            }

            EndBlink();
            mStateCountdown = 0;
        }
    }
    if (mWakeUpCounter > 0)
    {
        mWakeUpCounter--;
        if (mWakeUpCounter == 60)
        {
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
        }
        if (mWakeUpCounter == 0)
        {
            SetSleeping(false);
        }
    }

    if (mIsAsleep || mSquished || mOnBungeeState != PlantOnBungeeState::NOT_ON_BUNGEE)
        return;

    UpdateShooting();

    if (mStateCountdown > 0)
        mStateCountdown--;

    if (mApp->IsWallnutBowlingLevel())
    {
        UpdateBowling();
        return;
    }

    if (mSeedType == SeedType::SEED_SQUASH)                                                     UpdateSquash();
    else if (mSeedType == SeedType::SEED_DOOMSHROOM)                                            UpdateDoomShroom();
    else if (mSeedType == SeedType::SEED_XSHROOM)                                               UpdateXShroom();
    else if (mSeedType == SeedType::SEED_ICESHROOM)                                             UpdateIceShroom();
    else if (mSeedType == SeedType::SEED_CHOMPER)                                               UpdateChomper();
    else if (mSeedType == SeedType::SEED_BLOVER)                                                UpdateBlover();
    else if (mSeedType == SeedType::SEED_FLOWERPOT)                                             UpdateFlowerPot();
    else if (mSeedType == SeedType::SEED_LILYPAD)                                               UpdateLilypad();
    else if (mSeedType == SeedType::SEED_IMITATER)                                              UpdateImitater();
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE)                                        UpdateCoffeeBean();
    else if (mSeedType == SeedType::SEED_UMBRELLA)                                              UpdateUmbrella();
    else if (mSeedType == SeedType::SEED_COBCANNON)                                             UpdateCobCannon();
    else if (mSeedType == SeedType::SEED_CACTUS)                                                UpdateCactus();
    else if (mSeedType == SeedType::SEED_MAGNETSHROOM)                                          UpdateMagnetShroom();
    else if (mSeedType == SeedType::SEED_GOLD_MAGNET)                                           UpdateGoldMagnetShroom();
    else if (mSeedType == SeedType::SEED_SUN_MAGNET)                                            UpdateSunMagnet();
    else if (mSeedType == SeedType::SEED_SUNSHROOM)                                             UpdateSunShroom();
    else if (MakesSun() || mSeedType == SeedType::SEED_MARIGOLD)                                UpdateProductionPlant();
    else if (mSeedType == SeedType::SEED_GRAVEBUSTER)                                           UpdateGraveBuster();
    else if (mSeedType == SeedType::SEED_TORCHWOOD)                                             UpdateTorchwood();
    else if (mSeedType == SeedType::SEED_PLASMAWOOD)                                            UpdatePlasmawood();
    else if (mSeedType == SeedType::SEED_TOADSTOOL)                                             UpdateChomper();
    else if (mSeedType == SeedType::SEED_BEEHIVE)                                               UpdateBeehive();
    else if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_PRIMALPOTATOMINE)                                            UpdatePotato();
    else if (mSeedType == SeedType::SEED_SPIKEWEED || mSeedType == SeedType::SEED_SPIKEROCK)    UpdateSpikeweed();
    else if (mSeedType == SeedType::SEED_TANGLEKELP)                                            UpdateTanglekelp();
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)                                         UpdateScaredyShroom();

    if (mSubclass == PlantSubClass::SUBCLASS_SHOOTER)
    {
        UpdateShooter();
    }
    if (mDoSpecialCountdown > 0)
    {
        mDoSpecialCountdown--;
        if (mDoSpecialCountdown == 0)
        {
            DoSpecial();
        }
    }
}
bool Plant::IsPartOfUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow) || mBoard->IsValidCobCannonSpot(mPlantCol - 1, mRow);
    }

    return IsUpgradableTo(theUpgradedType);
}

bool Plant::IsUpgradableTo(SeedType theUpgradedType)
{
    if (theUpgradedType == SeedType::SEED_GATLINGPEA && mSeedType == SeedType::SEED_REPEATER)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_WINTERMELON && mSeedType == SeedType::SEED_MELONPULT)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_TWINSUNFLOWER && mSeedType == SeedType::SEED_SUNFLOWER)
    {
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_DYNAMIC_ROOF)
        {
            return false;
        }
        return true;
    }
    if (theUpgradedType == SeedType::SEED_SPIKEROCK && mSeedType == SeedType::SEED_SPIKEWEED)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT)
    {
        return mBoard->IsValidCobCannonSpot(mPlantCol, mRow);
    }
    if (theUpgradedType == SeedType::SEED_GOLD_MAGNET && mSeedType == SeedType::SEED_MAGNETSHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_GLOOMSHROOM && mSeedType == SeedType::SEED_FUMESHROOM)
    {
        return true;
    }
    if (theUpgradedType == SeedType::SEED_CATTAIL && mSeedType == SeedType::SEED_LILYPAD)
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        return aPlant == nullptr || aPlant->mSeedType != SeedType::SEED_CATTAIL;
    }
    return false;
}

void Plant::UpdateReanimColor()
{
    if (!IsOnBoard())
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    Color aColorOverride;

    bool isOnGlove = false;
    if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
    {
        Plant* aPlant = mBoard->mPlants.DataArrayTryToGet((unsigned int)mBoard->mCursorObject->mGlovePlantID);
        if (aPlant && aPlant->mPlantCol == mPlantCol && aPlant->mRow == mRow)
        {
            isOnGlove = true;
        }
    }

    if (isOnGlove)
    {
        aColorOverride = Color(128, 128, 128);
    }
    else if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (aSeedType == SeedType::SEED_COBCANNON && mSeedType == SeedType::SEED_KERNELPULT && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PLANTING_OK)
    {
        aColorOverride = GetFlashingColor(mBoard->mMainCounter, 90);
    }
    else if (mSeedType == SeedType::SEED_TRAMPALINE && mState == PlantState::STATE_NOTREADY)
    {
        aColorOverride = Color(64, 64, 64);
    }
    else if (mSeedType == SeedType::SEED_TRAMPALINE && mTrampolineReversed)
    {
        aColorOverride = Color(255, 0, 0, 255); 
    }
    else if (mSeedType == SeedType::SEED_BEEHIVE && mState == PlantState::STATE_BEEHIVE_COOLDOWN)
    {
        aColorOverride = Color(64, 64, 64); 
    }
    else if (mIsAsleep && (mSeedType == SeedType::SEED_HAMMER_SHROOM || mSeedType == SeedType::SEED_GUARDIAN_SHROOM || mSeedType == SeedType::SEED_MINI_GUARDIAN_SHROOM))
    {
        aColorOverride = Color(64, 64, 64);
    }
    else if (mHasBooster && MakesSun())
    {
        aColorOverride = Color(255, 215, 0, 255);
    }
    else
    {
        aColorOverride = Color(255, 255, 255);
    }
 
    aBodyReanim->mColorOverride = aColorOverride;

    if (mHighlighted)
    {
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 196);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
        if (mImitaterType == SeedType::SEED_IMITATER)
        {
            aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, 92);
        }
    }
    else if (mBeghouledFlashCountdown > 0)
    {
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraAdditiveColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else if (mEatenFlashCountdown > 0)
    {
        int aGrayness = ClampInt(mEatenFlashCountdown * 3, 0, mImitaterType == SeedType::SEED_IMITATER ? 128 : 255);
        aBodyReanim->mExtraAdditiveColor = Color(aGrayness, aGrayness, aGrayness);
        aBodyReanim->mEnableExtraAdditiveDraw = true;
    }
    else
    {
        aBodyReanim->mEnableExtraAdditiveDraw = false;
    }

    if (mBeghouledFlashCountdown > 0)
    {
        int anAlpha = TodAnimateCurve(50, 0, mBeghouledFlashCountdown % 50, 0, 128, TodCurves::CURVE_BOUNCE);
        aBodyReanim->mExtraOverlayColor = Color(255, 255, 255, anAlpha);
        aBodyReanim->mEnableExtraOverlayDraw = true;
    }
    else
    {
        aBodyReanim->mEnableExtraOverlayDraw = false;
    }

    aBodyReanim->PropogateColorToAttachments();
}

bool Plant::IsOnBoard()
{
    if (!mIsOnBoard)
        return false;

    TOD_ASSERT(mBoard);
    return true;
}

bool Plant::IsInPlay()
{
    return IsOnBoard() && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mGameMode != GameMode::GAMEMODE_TREE_OF_WISDOM;
}

void Plant::UpdateReanim()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    UpdateReanimColor();

    float aOffsetX = mShakeOffsetX;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    float aScaleX = 1.0f, aScaleY = 1.0f;
    if ((mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME) &&
        (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_SUNFLOWER || mSeedType == SeedType::SEED_MARIGOLD))
    {
        aScaleX = 1.5f;
        aScaleY = 1.5f;
        aOffsetX -= 20.0f;
        aOffsetY -= 40.0f;
    }
    if (mSeedType == SeedType::SEED_SUNFLOWER)
    {
        if (mHasBooster)
        {
            aBodyReanim->SetImageOverride("anim_idle", IMAGE_REANIM_SUNFLOWER_HEAD_WINK);
        }
        else if (mRecentlyEatenCountdown > 0)
        {
            aBodyReanim->SetImageOverride("anim_idle", IMAGE_REANIM_SUNFLOWER_HEAD_SING1);
        }
        else
        {
            aBodyReanim->SetImageOverride("anim_idle", nullptr);
        }
    }
    if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aScaleX = 2.0f;
        aScaleY = 2.0f;
        aOffsetX -= 76.0f;
        aOffsetY -= 64.0f;
    }
    if (mSeedType == SeedType::SEED_CUCKUMBER)
    {
        aOffsetX -= 17.0f;
        aOffsetY -= 22.0f;
        aScaleX = 1.25f;
        aScaleY = 1.25f;
    }
    if (mSeedType == SeedType::SEED_SNAPDRAGON)
    {
        aOffsetX -= 8.0f;
    }
    if (mSeedType == SeedType::SEED_GUARDIAN_SHROOM)
    {
        aScaleX = 0.85f;
        aScaleY = 0.85f;
    }
    if (mSeedType == SeedType::SEED_MINI_GUARDIAN_SHROOM)
    {
        aScaleX = 0.85f;
        aScaleY = 0.85f;
        aOffsetY -= 5.0f;
    }
    if (mSeedType == SeedType::SEED_SUN_MAGNET)
    {
        aScaleX = 0.85f;
        aScaleY = 0.85f;
        aOffsetY -= 5.0f;
    }
    if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 10.0f;
    }
    if (mSeedType == SeedType::SEED_BEEHIVE)
    {
        aScaleX *= 1.2f;
        aScaleY *= 1.2f;
        aOffsetX -= 8.0f;
        aOffsetY -= 16.0f;
    }
    if (mSeedType == SeedType::SEED_POTATOMINE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    if (mSeedType == SeedType::SEED_PRIMALPOTATOMINE)
    {
        aScaleX = 0.8f;
        aScaleY = 0.8f;
        aOffsetX += 12.0f;
        aOffsetY += 12.0f;
    }
    if (mState == PlantState::STATE_GRAVEBUSTER_EATING)
    {
        aOffsetY += TodAnimateCurveFloat(400, 0, mStateCountdown, 0.0f, 30.0f, TodCurves::CURVE_LINEAR);
    }
    if (mWakeUpCounter > 0)
    {
        float aScaleFactor = TodAnimateCurveFloat(70, 0, mWakeUpCounter, 1.0f, 0.8f, TodCurves::CURVE_EASE_SIN_WAVE);
        aScaleY *= aScaleFactor;
        aOffsetY += 80.0f - 80.0f * aScaleFactor;
    }

    aBodyReanim->Update();

    if (mSeedType == SeedType::SEED_LEFTPEATER || mIsFlipped)
    {
        aOffsetX += 80.0f * aScaleX;
        aScaleX *= -1.0f;
    }
    if (mPottedPlantIndex != -1)
    {
        PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mPottedPlantIndex];

        if (aPottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT)
        {
            aOffsetX += 80.0f * aScaleX;
            aScaleX *= -1.0f;
        }

        float aOffsetXStart, aOffsetXEnd;
        float aOffsetYStart, aOffsetYEnd;
        float aScaleStart, aScaleEnd;
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 20.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 40.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.5f;
        }
        else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
        {
            aOffsetXStart = 20.0f;
            aOffsetXEnd = 10.0f;
            aOffsetYStart = 40.0f;
            aOffsetYEnd = 20.0f;
            aScaleStart = 0.5f;
            aScaleEnd = 0.75f;
        }
        else
        {
            aOffsetXStart = 10.0f;
            aOffsetXEnd = 0.0f;
            aOffsetYStart = 20.0f;
            aOffsetYEnd = 0.0f;
            aScaleStart = 0.75f;
            aScaleEnd = 1.0f;
        }

        float aAnimatedOffsetX = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetXStart, aOffsetXEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedOffsetY = TodAnimateCurveFloat(100, 0, mStateCountdown, aOffsetYStart, aOffsetYEnd, TodCurves::CURVE_LINEAR);
        float aAnimatedScale = TodAnimateCurveFloat(100, 0, mStateCountdown, aScaleStart, aScaleEnd, TodCurves::CURVE_LINEAR);

        aOffsetX += aAnimatedOffsetX * aScaleX;
        aOffsetY += aAnimatedOffsetY * aScaleY;
        aScaleX *= aAnimatedScale;
        aScaleY *= aAnimatedScale;
        aOffsetX += mApp->mZenGarden->ZenPlantOffsetX(aPottedPlant);
        aOffsetY += mApp->mZenGarden->PlantPottedDrawHeightOffset(mSeedType, aScaleY);
    }
    aBodyReanim->SetPosition(aOffsetX, aOffsetY);
    aBodyReanim->OverrideScale(aScaleX, aScaleY);
}
void Plant::Update()
{
    bool doUpdate = false;
    if (IsOnBoard() && mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO && mApp->IsWallnutBowlingLevel())
        doUpdate = true;
    else if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        doUpdate = true;
    else if (IsOnBoard() && mBoard->mCutScene->ShouldRunUpsellBoard())
        doUpdate = true;
    else if (!IsOnBoard() || mApp->mGameScene == GameScenes::SCENE_PLAYING)
        doUpdate = true;
    if (doUpdate)
    {
        bool isBowlingWallnut = mApp->IsWallnutBowlingLevel() && (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT);
        if (!mSquished && !mDead && mOnBungeeState == PlantOnBungeeState::NOT_ON_BUNGEE && mBoard)
        {
            int expectedY = mBoard->GridToPixelY(mPlantCol, mRow);
            int totalPots = 0;
            int potsBelow = 0;
            int samePlantsBelow = 0; // لمتغير التكديس الجديد

            Plant* p2 = nullptr;
            while (mBoard->IteratePlants(p2))
            {
                if (p2->mPlantCol == mPlantCol && p2->mRow == mRow && !p2->mDead)
                {
                    if (p2->mSeedType == SeedType::SEED_FLOWERPOT)
                    {
                        totalPots++;
                        if (p2 != this && mBoard->mPlants.DataArrayGetID(p2) < mBoard->mPlants.DataArrayGetID(this))
                        {
                            potsBelow++;
                        }
                    }
                    // التحقق من النباتات المتشابهة تحت هذه النبتة للتكديس
                    else if (p2->mSeedType == mSeedType && p2 != this)
                    {
                        if (mBoard->mPlants.DataArrayGetID(p2) < mBoard->mPlants.DataArrayGetID(this))
                        {
                            samePlantsBelow++;
                        }
                    }
                }
            }

            if (mSeedType == SeedType::SEED_FLOWERPOT)
            {
                expectedY -= potsBelow * 25;
            }
            else if (totalPots > 0 && !Plant::IsFlying(mSeedType) && mSeedType != SeedType::SEED_COBCANNON)
            {
                expectedY -= (totalPots - 1) * 25;
            }
            if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_SPORESHROOM)
            {
                expectedY -= samePlantsBelow * 15; 
            }

            if (mY < expectedY)
            {
                mY += 2;
                if (mY > expectedY) mY = expectedY;
            }
            else if (mY > expectedY)
            {
                mY = expectedY;
            }
        }

        UpdateAbilities();
        Animate();
        if (mPlantHealth < 0 && !mDead)
        {
            if (mSeedType == SeedType::SEED_EXPLODE_O_NUT)
                DoSpecial();
            else
                Die();
        }
        UpdateReanim();
    }
}
bool Plant::NotOnGround()
{
    if (mSeedType == SeedType::SEED_SQUASH)
    {
        if (mState == PlantState::STATE_SQUASH_RISING || mState == PlantState::STATE_SQUASH_FALLING || mState == PlantState::STATE_SQUASH_DONE_FALLING)
            return true;
    }

    return mSquished || mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE || mDead;
}

Reanimation* Plant::AttachBlinkAnim(Reanimation* theReanimBody)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(mSeedType);
    LawnApp* aApp = (LawnApp*)gSexyAppBase;
    Reanimation* aAnimToAttach = theReanimBody;
    const char* aTrackToPlay = "anim_blink";
    const char* aTrackToAttach = nullptr;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || 
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        int aHit = Rand(10);
        if (aHit < 1 && theReanimBody->TrackExists("anim_blink_twitch"))
        {
            aTrackToPlay = "anim_blink_twitch";
        }
        else
        {
            aTrackToPlay = aHit < 7 ? "anim_blink_twice" : "anim_blink_thrice";
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        int aHit = Rand(3);
        if (aHit == 0)
        {
            aTrackToPlay = "anim_blink1";
            aTrackToAttach = "anim_face1";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head1");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (aHit == 1)
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head2");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else
        {
            aTrackToPlay = "anim_blink3";
            aTrackToAttach = "anim_face3";
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_head3");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID);
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
            aAnimToAttach = mApp->ReanimationTryToGet(mHeadReanimID2);
        }
    }
    else if (mSeedType == SeedType::SEED_TWINSUNFLOWER)
    {
        if (Rand(2) == 0)
        {
            aTrackToPlay = "anim_blink";
            aTrackToAttach = "anim_face";
        }
        else
        {
            aTrackToPlay = "anim_blink2";
            aTrackToAttach = "anim_face2";
        }
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_ACID_LEMON || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER || mSeedType == SeedType::SEED_LEFTPEATER || mSeedType == SeedType::SEED_GATLINGPEA || mSeedType == SeedType::SEED_SUNPEA
        || mSeedType == SeedType::SEED_GOOPEA || mSeedType == SeedType::SEED_ROCKPEA || mSeedType == SeedType::SEED_DROPPEA || mSeedType == SeedType::SEED_CPEA)
    {
        if (theReanimBody->TrackExists("anim_stem"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_stem");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
        else if (theReanimBody->TrackExists("anim_idle"))
        {
            ReanimatorTrackInstance* aTrackInstance = theReanimBody->GetTrackInstanceByName("anim_idle");
            aAnimToAttach = FindReanimAttachment(aTrackInstance->mAttachmentID);
        }
    }

    if (aAnimToAttach == nullptr)
    {
        TodTrace("Missing head anim");
        return nullptr;
    }

    if (!theReanimBody->TrackExists(aTrackToPlay))
        return nullptr;

    Reanimation* aBlinkReanim = aApp->mEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aPlantDef.mReanimationType);
    aBlinkReanim->SetFramesForLayer(aTrackToPlay);
    aBlinkReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD;
    aBlinkReanim->mAnimRate = 15.0f;
    aBlinkReanim->mColorOverride = theReanimBody->mColorOverride;

    if (aTrackToAttach && aAnimToAttach->TrackExists(aTrackToAttach))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, aTrackToAttach);
    }
    else if (aAnimToAttach->TrackExists("anim_face"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_face");
    }
    else if (aAnimToAttach->TrackExists("anim_idle"))
    {
        aBlinkReanim->AttachToAnotherReanimation(aAnimToAttach, "anim_idle");
    }
    else
    {
        TodTrace("Missing anim_idle for blink");
    }

    aBlinkReanim->mFilterEffect = theReanimBody->mFilterEffect;
    return aBlinkReanim;
}

void Plant::DoBlink()
{
    mBlinkCountdown = 400 + Rand(400);

    if (NotOnGround() || mShootingCounter != 0)
        return;

    if (mSeedType == SeedType::SEED_POTATOMINE && mState != PlantState::STATE_POTATO_ARMED)
        return;

    if (mSeedType == SeedType::SEED_PRIMALPOTATOMINE && mState != PlantState::STATE_POTATO_ARMED)
        return;

    if (mState == PlantState::STATE_CACTUS_RISING || mState == PlantState::STATE_CACTUS_HIGH || mState == PlantState::STATE_CACTUS_LOWERING ||
        mState == PlantState::STATE_MAGNETSHROOM_SUCKING || mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
        return;

    EndBlink();
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    if ((mSeedType == SeedType::SEED_TALLNUT && aBodyReanim->GetImageOverride("anim_idle") == IMAGE_REANIM_TALLNUT_CRACKED2) || 
        (mSeedType == SeedType::SEED_GARLIC && aBodyReanim->GetImageOverride("anim_face") == IMAGE_REANIM_GARLIC_BODY3))
        return;

    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT || 
        mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        mBlinkCountdown = 1000 + Rand(1000);
    }

    Reanimation* aBlinkReanim = AttachBlinkAnim(aBodyReanim);
    if (aBlinkReanim)
    {
        mBlinkReanimID = mApp->ReanimationGetID(aBlinkReanim);
    }
    aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_HIDDEN);
}

void Plant::EndBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        mApp->RemoveReanimation(mBlinkReanimID);
        mBlinkReanimID = ReanimationID::REANIMATIONID_NULL;

        Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
        if (aBodyReanim)
        {
            aBodyReanim->AssignRenderGroupToPrefix("anim_eye", RENDER_GROUP_NORMAL);
        }
    }
}

void Plant::UpdateBlink()
{
    if (mBlinkReanimID != ReanimationID::REANIMATIONID_NULL)
    {
        Reanimation* aBlinkReanim = mApp->ReanimationTryToGet(mBlinkReanimID);
        if (aBlinkReanim == nullptr || aBlinkReanim->mLoopCount > 0)
        {
            EndBlink();
        }
    }

    if (mIsAsleep)
        return;

    if (mBlinkCountdown > 0)
    {
        mBlinkCountdown--;
        if (mBlinkCountdown == 0)
        {
            DoBlink();
        }
    }
}

void Plant::AnimateNuts()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;

    Image* aCracked1;
    Image* aCracked2;
    const char* aTrackToOverride;

 
    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_EXPLODE_O_NUT || mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aCracked1 = (mSeedType == SeedType::SEED_EXPLODE_O_NUT) ? IMAGE_REANIM_EXPLODE_O_NUT_CRACKED1 : IMAGE_REANIM_WALLNUT_CRACKED1;
        aCracked2 = (mSeedType == SeedType::SEED_EXPLODE_O_NUT) ? IMAGE_REANIM_EXPLODE_O_NUT_CRACKED2 : IMAGE_REANIM_WALLNUT_CRACKED2;
        aTrackToOverride = "anim_face";
    }
    else if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aCracked1 = IMAGE_REANIM_TALLNUT_CRACKED1;
        aCracked2 = IMAGE_REANIM_TALLNUT_CRACKED2;
        aTrackToOverride = "anim_idle";
    }
    else return;

    int aPosX = mX + 40;
    int aPosY = mY + 10;
    if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aPosY -= 32;
    }

    Image* aImageOverride = aBodyReanim->GetImageOverride(aTrackToOverride);
    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != aCracked2)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked2);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != aCracked1)
        {
            aBodyReanim->SetImageOverride(aTrackToOverride, aCracked1);
            mApp->AddTodParticle(aPosX, aPosY, mRenderOrder + 4, ParticleEffect::PARTICLE_WALLNUT_EAT_LARGE);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride(aTrackToOverride, nullptr);
    }

    if (IsInPlay() && !mApp->IsIZombieLevel())
    {
        if (mRecentlyEatenCountdown > 0)
        {
            aBodyReanim->mAnimRate = 0.1f;
            return;
        }

        if (aBodyReanim->mAnimRate < 1.0f && mOnBungeeState != PlantOnBungeeState::RISING_WITH_BUNGEE)
        {
            aBodyReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }
}
void Plant::AnimateSunflower()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim == nullptr)
        return;
    const char* possibleHeadTracks[] = {
        "anim_head",
        "anim_head1",
        "Head",
        "head",
        "Sunflower_head",
        "anim_face",
        "anim_idel",
        "face"
    };
    int numTracks = sizeof(possibleHeadTracks) / sizeof(possibleHeadTracks[0]);
    for (int i = 0; i < numTracks; ++i)
    {
        if (mHasBooster)
        {
            aBodyReanim->SetImageOverride(possibleHeadTracks[i], IMAGE_REANIM_SUNFLOWER_HEAD_WINK);
        }
        else if (mRecentlyEatenCountdown > 0)
        {
            aBodyReanim->SetImageOverride(possibleHeadTracks[i], IMAGE_REANIM_SUNFLOWER_HEAD_SING1);
        }
        else
        {
            aBodyReanim->SetImageOverride(possibleHeadTracks[i], nullptr);
        }
    }
}
void Plant::AnimateGarlic()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("anim_face");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY3)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY3);
            aBodyReanim->AssignRenderGroupToPrefix("Garlic_stem", RENDER_GROUP_HIDDEN);
        }
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_GARLIC_BODY2)
        {
            aBodyReanim->SetImageOverride("anim_face", IMAGE_REANIM_GARLIC_BODY2);
        }
    }
    else
    {
        aBodyReanim->SetImageOverride("anim_face", nullptr);
    }
}

void Plant::AnimatePumpkin()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    Image* aImageOverride = aBodyReanim->GetImageOverride("Pumpkin_front");

    if (mPlantHealth < mPlantMaxHealth / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE3)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE3);
    }
    else if (mPlantHealth < mPlantMaxHealth * 2 / 3)
    {
        if (aImageOverride != IMAGE_REANIM_PUMPKIN_DAMAGE1)
            aBodyReanim->SetImageOverride("Pumpkin_front", IMAGE_REANIM_PUMPKIN_DAMAGE1);
    }
    else
    {
        aBodyReanim->SetImageOverride("Pumpkin_front", nullptr);
    }
}

void Plant::UpdateShooting()
{
    if (NotOnGround() || mShootingCounter == 0)
        return;

    if (mShootingCounter > 0)
    {
        mShootingCounter--;
    }

    if (mSeedType == SeedType::SEED_FUMESHROOM && mShootingCounter == 15)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        TodParticleSystem* aFumeParticle = AddAttachedParticle(mX + 85, mY + 31, aRenderPosition, ParticleEffect::PARTICLE_FUMECLOUD);
        if (aFumeParticle && mHasBooster)
        {
            aFumeParticle->OverrideScale(nullptr, 2.0f);
        }
    }

    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        if (mShootingCounter == 136 || mShootingCounter == 108 || mShootingCounter == 80 || mShootingCounter == 52)
        {
            int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
            TodParticleSystem* aGloomParticle1 = AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);

            if (mHasBooster)
            {
                TodParticleSystem* aGloomParticle2 = AddAttachedParticle(mX + 40, mY + 40, aRenderPosition, ParticleEffect::PARTICLE_GLOOMCLOUD);
                if (aGloomParticle2)
                {
                    aGloomParticle2->OverrideScale(nullptr, 3.0f);
                }
            }
            mApp->PlayFoley(FoleyType::FOLEY_FUME);
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51 || mShootingCounter == 68)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_CPEA)
    {
        if (mShootingCounter == 18 || mShootingCounter == 35 || mShootingCounter == 51)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        if (mShootingCounter == 19)
        {
            Zombie* aZombie = FindTargetZombie(mRow, PlantWeapon::WEAPON_PRIMARY);
            if (aZombie)
            {
                Fire(aZombie, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
        }
    }
    else if (mShootingCounter == 1)
    {
        if (mSeedType == SeedType::SEED_SNAPDRAGON)
        {
            return;
        }
        else if (mSeedType == SeedType::SEED_THREEPEATER)
        {
            int rowAbove = mRow - 1;
            int rowBelow = mRow + 1;
            Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
            Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);
            Reanimation* aHeadReanim1 = mApp->ReanimationGet(mHeadReanimID);

            if (aHeadReanim1->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowBelow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim2->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, rowAbove, PlantWeapon::WEAPON_PRIMARY);
            }
        }
        else if (mSeedType == SeedType::SEED_SPLITPEA)
        {
            Reanimation* aHeadBackReanim = mApp->ReanimationTryToGet(mHeadReanimID2);
            Reanimation* aHeadFrontReanim = mApp->ReanimationTryToGet(mHeadReanimID);
            if (aHeadFrontReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && mLaunchCounter <= 1)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
            }
            if (aHeadBackReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
            }
        }
        else if (mState == PlantState::STATE_CACTUS_LOW)
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_SECONDARY);
        }
        else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_SPORESHROOM || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_PEPPERPULT || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
        {
            PlantWeapon aPlantWeapon = PlantWeapon::WEAPON_PRIMARY;
            if (mState == PlantState::STATE_KERNELPULT_BUTTER)
            {
                Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_butter", RENDER_GROUP_HIDDEN);
                aBodyReanim->AssignRenderGroupToPrefix("Cornpult_kernal", RENDER_GROUP_NORMAL);
                mState = PlantState::STATE_NOTREADY;
                aPlantWeapon = PlantWeapon::WEAPON_SECONDARY;
            }

            Zombie* aZombie = FindTargetZombie(mRow, aPlantWeapon);
            Fire(aZombie, mRow, aPlantWeapon);
        }
        else
        {
            Fire(nullptr, mRow, PlantWeapon::WEAPON_PRIMARY);
        }

        return;
    }

    if (mShootingCounter != 0)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(mHeadReanimID);
    if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);
        Reanimation* aHeadReanim3 = mApp->ReanimationGet(mHeadReanimID3);

        if (aHeadReanim2->mLoopCount > 0)
        {
            if (aHeadReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim->StartBlend(20);
                aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim->SetFramesForLayer("anim_head_idle1");
                aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            }

            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_head_idle2");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;

            if (aHeadReanim3->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD)
            {
                aHeadReanim3->StartBlend(20);
                aHeadReanim3->mLoopType = ReanimLoopType::REANIM_LOOP;
                aHeadReanim3->SetFramesForLayer("anim_head_idle3");
                aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
                aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
            }

            return;
        }
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        Reanimation* aHeadReanim2 = mApp->ReanimationGet(mHeadReanimID2);

        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
        }

        if (aHeadReanim2->mLoopCount > 0)
        {
            aHeadReanim2->StartBlend(20);
            aHeadReanim2->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim2->SetFramesForLayer("anim_splitpea_idle");
            aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
        }

        return;
    }
    else if (mState == PlantState::STATE_CACTUS_HIGH)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            PlayBodyReanim("anim_idlehigh", ReanimLoopType::REANIM_LOOP, 20, 0.0f);

            aBodyReanim->mAnimRate = aBodyReanim->mDefinition->mFPS;
            if (mApp->IsIZombieLevel())
            {
                aBodyReanim->mAnimRate = 0.0f;
            }

            return;
        }
    }
    else if (aHeadReanim)
    {
        if (aHeadReanim->mLoopCount > 0)
        {
            aHeadReanim->StartBlend(20);
            aHeadReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aHeadReanim->SetFramesForLayer("anim_head_idle");
            aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
            aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
            return;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_COBCANNON_ARMING;
            mStateCountdown = 3000;
            PlayBodyReanim("anim_unarmed_idle", ReanimLoopType::REANIM_LOOP, 20, aBodyReanim->mDefinition->mFPS);
            return;
        }
    }
    else if (aBodyReanim && aBodyReanim->mLoopCount > 0)
    {
        float aRate = aBodyReanim->mDefinition->mFPS;
        if (aRate <= 0.0f)
        {
            aRate = RandRangeFloat(10.0f, 15.0f);
        }
        PlayIdleAnim(aRate);
        return;
    }

    mShootingCounter = 1;
}

void Plant::Animate()
{
    if ((mSeedType == SeedType::SEED_CHERRYBOMB || mSeedType == SeedType::SEED_JALAPENO) && mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
    {
        mShakeOffsetX = RandRangeFloat(-1.0f, 1.0f);
        mShakeOffsetY = RandRangeFloat(-1.0f, 1.0f);
    }

    if (mRecentlyEatenCountdown > 0)
    {
        mRecentlyEatenCountdown--;
    }
    if (mEatenFlashCountdown > 0)
    {
        mEatenFlashCountdown--;
    }
    if (mBeghouledFlashCountdown > 0)
    {
        mBeghouledFlashCountdown--;
    }

    if (mSquished)
    {
        mFrame = 0;
        return;
    }
    if (mSeedType == SeedType::SEED_WALLNUT || mSeedType == SeedType::SEED_TALLNUT ||
        mSeedType == SeedType::SEED_EXPLODE_O_NUT)
    {
        AnimateNuts();
    }
    else if (mSeedType == SeedType::SEED_SUNFLOWER)
    {
        AnimateSunflower();
    }
    else if (mSeedType == SeedType::SEED_GARLIC)
    {
        AnimateGarlic();
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        AnimatePumpkin();
    }

    UpdateBlink();

    if (mAnimPing)
    {
        if (mAnimCounter < mFrameLength * mNumFrames - 1)
        {
            mAnimCounter++;
        }
        else
        {
            mAnimPing = false;
            mAnimCounter -= mFrameLength;
        }
    }
    else if (mAnimCounter > 0)
    {
        mAnimCounter--;
    }
    else
    {
        mAnimPing = true;
        mAnimCounter += mFrameLength;
    }
    mFrame = mAnimCounter / mFrameLength;
}

float PlantFlowerPotHeightOffset(SeedType theSeedType, float theFlowerPotScale)
{
    float aHeightOffset = -5.0f * theFlowerPotScale;
    float aScaleOffsetFix = 0.0f;

    switch (theSeedType)
    {
    case SeedType::SEED_CHOMPER:
    case SeedType::SEED_PLANTERN:
        aHeightOffset -= 5.0f;
        break;
    case SeedType::SEED_SCAREDYSHROOM:
        aHeightOffset += 5.0f;
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SUNSHROOM:
    case SeedType::SEED_PUFFSHROOM:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_HYPNOSHROOM:
    case SeedType::SEED_POISONSHROOM:
    case SeedType::SEED_MAGNETSHROOM:
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_SUNPEA:
    case SeedType::SEED_DROPPEA:
    case SeedType::SEED_ROCKPEA:
    case SeedType::SEED_CPEA:
    case SeedType::SEED_GOOPEA:
    case SeedType::SEED_ACID_LEMON:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_LEFTPEATER:
    case SeedType::SEED_SNOWPEA:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SUNFLOWER:
    case SeedType::SEED_PRIMALSUNFLOWER:
    case SeedType::SEED_MOONFLOWER:
    case SeedType::SEED_MARIGOLD:
    case SeedType::SEED_CABBAGEPULT:
    case SeedType::SEED_SPORESHROOM:
    case SeedType::SEED_PEPPERPULT:
    case SeedType::SEED_MELONPULT:
    case SeedType::SEED_TANGLEKELP:
    case SeedType::SEED_BLOVER:
    case SeedType::SEED_SPIKEWEED:
        aScaleOffsetFix -= 8.0f;
        break;
    case SeedType::SEED_SEASHROOM:
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_PRIMALPOTATOMINE:
        aScaleOffsetFix -= 4.0f;
        break;
    case SeedType::SEED_LILYPAD:
        aScaleOffsetFix -= 16.0f;
        break;
    case SeedType::SEED_INSTANT_COFFEE:
        aScaleOffsetFix -= 20.0f;
        break;
    }

    return aHeightOffset + (theFlowerPotScale * aScaleOffsetFix - aScaleOffsetFix);
}

float PlantDrawHeightOffset(Board* theBoard, Plant* thePlant, SeedType theSeedType, int theCol, int theRow)
{
    float aHeightOffset = 0.0f;

    bool doFloating = false;
    if (Plant::IsFlying(theSeedType))
    {
        doFloating = false;
    }
    else if (theBoard == nullptr)
    {
        if (Plant::IsAquatic(theSeedType))
        {
            doFloating = true;
        }
    }
    else if (theBoard->IsPoolSquare(theCol, theRow))
    {
        doFloating = true;

        Plant* aLily = theBoard->GetTopPlantAt(theCol, theRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
        if (theSeedType == SeedType::SEED_COBCANNON && (!aLily || aLily->mSeedType != SeedType::SEED_LILYPAD))
        {
            aHeightOffset += 30.0f;
        }
    }

    if (doFloating)
    {
        int aCounter;
        if (theBoard)
        {
            aCounter = theBoard->mMainCounter;
        }
        else
        {
            aCounter = gLawnApp->mAppCounter;
        }

        float aPos = theRow * PI + theCol * 0.25f * PI;
        float aTime = aCounter * 2.0f * PI / 200.0f;
        float aFloatingHeight = sin(aPos + aTime) * 2.0f;
        aHeightOffset += aFloatingHeight;
    }

    if (theBoard && (thePlant == nullptr || !thePlant->mSquished))
    {
        Plant* aPot = theBoard->GetFlowerPotAt(theCol, theRow);
        if (aPot && !aPot->mSquished && theSeedType != SeedType::SEED_FLOWERPOT)
        {
            aHeightOffset += PlantFlowerPotHeightOffset(theSeedType, 1.0f);
        }
    }

    if (theSeedType == SeedType::SEED_FLOWERPOT)
    {
        aHeightOffset += 26.0f;
    }
    else if (theSeedType == SeedType::SEED_LILYPAD)
    {
        aHeightOffset += 25.0f;
    }
    else if (theSeedType == SeedType::SEED_STARFRUIT)
    {
        aHeightOffset += 10.0f;
    }
    else if (theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aHeightOffset += 24.0f;
    }
    else if (theSeedType == SeedType::SEED_SEASHROOM)
    {
        aHeightOffset += 28.0f;
    }
    else if (theSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aHeightOffset -= 20.0f;
    }
    else if (theSeedType == SeedType::SEED_CACTUS)
    {
        return aHeightOffset;
    }
    else if (theSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aHeightOffset += 15.0f;
    }
    else if (theSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aHeightOffset += 5.0f;
    }
    else if (theSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aHeightOffset -= 14.0f;
    }
    else if (theSeedType == SeedType::SEED_GRAVEBUSTER)
    {
        aHeightOffset -= 40.0f;
    }
    else if (theSeedType == SeedType::SEED_SPIKEWEED || theSeedType == SeedType::SEED_SPIKEROCK)
    {
        int aBottomRow = 4;
        if (theBoard && theBoard->StageHas6Rows())
        {
            aBottomRow = 5;
        }

        if (theSeedType == SeedType::SEED_SPIKEROCK)
        {
            aHeightOffset += 6.0f;
        }

        if (theBoard && theBoard->GetFlowerPotAt(theCol, theRow) && gLawnApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
        {
            aHeightOffset += 5.0f;
        }
        else if (theBoard && theBoard->StageHasRoof())
        {
            aHeightOffset += 15.0f;
        }
        else if (theBoard && theBoard->IsPoolSquare(theCol, theRow))
        {
            aHeightOffset += 0.0f;
        }
        else if (theRow == aBottomRow && theCol >= 7 && theBoard->StageHas6Rows())
        {
            aHeightOffset += 1.0f;
        }
        else if (theRow == aBottomRow && theCol < 7)
        {
            aHeightOffset += 12.0f;
        }
        else
        {
            aHeightOffset += 15.0f;
        }
    }

    return aHeightOffset;
}
void Plant::GetPeaHeadOffset(int& theOffsetX, int& theOffsetY)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);

    int aTrackIndex = 0;
    if (aBodyReanim->TrackExists("anim_stem"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_stem");
    }
    else if(aBodyReanim->TrackExists("anim_idle"))
    {
        aTrackIndex = aBodyReanim->FindTrackIndex("anim_idle");
    }

    ReanimatorTransform aTransform;
    aBodyReanim->GetCurrentTransform(aTrackIndex, &aTransform);
    theOffsetX = aTransform.mTransX;
    theOffsetY = aTransform.mTransY;
}

void Plant::DrawMagnetItems(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);

    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            int aCelRow = 0, aCelCol = 0;
            Image* aImage = nullptr;
            float aScale = 0.8f;

            if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PAIL_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_BUCKET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_FOOTBALL_HELMET_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_FOOTBALL_HELMET3;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DOOR_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_SCREENDOOR3;
            }
            else if (aMagnetItem->mItemType >= MagnetItemType::MAGNET_ITEM_POGO_1 && aMagnetItem->mItemType <= MagnetItemType::MAGNET_ITEM_POGO_3)
            {
                aCelCol = (int)aMagnetItem->mItemType - (int)MagnetItemType::MAGNET_ITEM_POGO_1;
                aImage = IMAGE_ZOMBIEPOGO;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_1)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_2)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE1;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_3)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_1_DAMAGE2;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LADDER_PLACED)
            {
                aImage = IMAGE_REANIM_ZOMBIE_LADDER_5;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_JACK_IN_THE_BOX)
            {
                aImage = IMAGE_REANIM_ZOMBIE_JACKBOX_BOX;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_PICK_AXE)
            {
                aImage = IMAGE_REANIM_ZOMBIE_DIGGER_PICKAXE;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SILVER_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_SILVER_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_GOLD_COIN)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_COIN_GOLD_DOLLAR;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_DIAMOND)
            {
                aScale = 1.0f;
                aImage = IMAGE_REANIM_DIAMOND;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SUN)
            {
                aScale = 1.0f;
                aImage = IMAGE_SUN;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SMALLSUN)
            {
                aScale = 0.5f;
                aImage = IMAGE_SUN;
            }
            else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LARGESUN)
            {
                aScale = 2.0f;
                aImage = IMAGE_SUN;
            }
            else
            {
                TOD_ASSERT();
            }

            if (aScale == 1.0f)
            {
                g->DrawImageCel(aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow);
            }
            else
            {
                TodDrawImageCelScaledF(g, aImage, aMagnetItem->mPosX - mX + aOffsetX, aMagnetItem->mPosY - mY + aOffsetY, aCelCol, aCelRow, aScale, aScale);
            }
        }
    }
}
Image* Plant::GetImage(SeedType theSeedType)
{
    Image** aImages = GetPlantDefinition(theSeedType).mPlantImage;
    return aImages ? aImages[0] : nullptr;
}

void Plant::DrawShadow(Sexy::Graphics* g, float theOffsetX, float theOffsetY)
{
    if (mSeedType == SeedType::SEED_LILYPAD || mSeedType == SeedType::SEED_STARFRUIT || mSeedType == SeedType::SEED_TANGLEKELP || 
        mSeedType == SeedType::SEED_SEASHROOM || mSeedType == SeedType::SEED_COBCANNON || mSeedType == SeedType::SEED_SPIKEWEED || 
        mSeedType == SeedType::SEED_SPIKEROCK || mSeedType == SeedType::SEED_GRAVEBUSTER || mSeedType == SeedType::SEED_CATTAIL || 
        mOnBungeeState == PlantOnBungeeState::RISING_WITH_BUNGEE)
        return;

    if (IsOnBoard() && mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mApp->mZenGarden->mGardenType == GardenType::GARDEN_MAIN)
        return;

    int aShadowType = 0;
    float aShadowOffsetX = -3.0f;
    float aShadowOffsetY = 51.0f;
    float aScale = 1.0f;
    if (mBoard && mBoard->StageIsNight())
    {
        aShadowType = 1;
    }

    if (mSeedType == SeedType::SEED_SQUASH)
    {
        if (mBoard)
        {
            aShadowOffsetY += mBoard->GridToPixelY(mPlantCol, mRow) - mY;
        }
        aShadowOffsetY += 5.0f;
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aScale = 0.5f;
        aShadowOffsetY = 42.0f;
    }
    else if (mSeedType == SeedType::SEED_SUNSHROOM)
    {
        aShadowOffsetY = 42.0f;
        if (mState == PlantState::STATE_SUNSHROOM_SMALL)
        {
            aScale = 0.5f;
        }
        else if (mState == PlantState::STATE_SUNSHROOM_GROWING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
            aScale = 0.5f + 0.5f * aBodyReanim->mAnimTime;
        }
    }
    else if (mSeedType == SeedType::SEED_UMBRELLA)
    {
        aScale = 0.5f;
        aShadowOffsetX = -7.0f;
        aShadowOffsetY = 52.0f;
    }
    else if (mSeedType == SeedType::SEED_FUMESHROOM || mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        aScale = 1.3f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_SPORESHROOM || mSeedType == SeedType::SEED_PEPPERPULT  || mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT)
    {
        aShadowOffsetX = 0.0f;
        aShadowOffsetY = 47.0f;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aShadowOffsetX = -9.0f;
        aShadowOffsetY = 55.0f;
    }
    else if (mSeedType == SeedType::SEED_CHOMPER)
    {
        if (mIsFlipped)
        {
            aShadowOffsetX = 35.0f;
            aShadowOffsetY = 59.0f;
        }
        else
        {
            aShadowOffsetX = -21.0f;
            aShadowOffsetY = 57.0f;
        }
    }
    else if (mSeedType == SeedType::SEED_FLOWERPOT)
    {
        aShadowOffsetX = -4.0f;
        aShadowOffsetY = 46.0f;
    }
    else if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aShadowOffsetY = 54.0f;
        aScale = 1.3f;
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aShadowOffsetY = 46.0f;
        aScale = 1.4f;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        aShadowOffsetX = -8.0f;
        aShadowOffsetY = 50.0f;
    }
    else if (mSeedType == SeedType::SEED_PLANTERN)
    {
        aShadowOffsetY = 57.0f;
    }
    else if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
    {
        aShadowOffsetY = 71.0f;
    }
    else if (mSeedType == SeedType::SEED_GIANT_WALLNUT)
    {
        aShadowOffsetX = -33.0f;
        aShadowOffsetY = 56.0f;
        aScale = 1.7f;
    }

    if (Plant::IsFlying(mSeedType))
    {
        aShadowOffsetY += 10.0f;
        if (mBoard && (mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_NORMAL_POSITION) || mBoard->GetTopPlantAt(mPlantCol, mRow, TOPPLANT_ONLY_PUMPKIN)))
            return;
    }

    if (aShadowType == 0)
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
    else
    {
        TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, theOffsetX + aShadowOffsetX, theOffsetY + aShadowOffsetY, 0, aScale, aScale);
    }
}

void Plant::Draw(Graphics* g)
{
    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, this, mSeedType, mPlantCol, mRow);
    if (Plant::IsFlying(mSeedType) && mSquished)
    {
        aOffsetY += 30.0f;
    }
    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INVISIPLANTS && !mSquished)
    {
       return;
    }
    int aImageIndex = mFrame;
    Image* aPlantImage = Plant::GetImage(mSeedType);

    if (mSquished)
    {
        if (mSeedType == SeedType::SEED_FLOWERPOT)
        {
            aOffsetY -= 15.0f;
        }
        if (mSeedType == SeedType::SEED_INSTANT_COFFEE)
        {
            aOffsetY -= 20.0f;
        }

        g->SetScale(1.0f, 0.25f, 0.0f, 0.0f);
        DrawSeedType(g, mSeedType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, 60.0f + aOffsetY);
        g->SetScale(1.0f, 1.0f, 0.0f, 0.0f);
    }
    else
    {
        bool aDrawPumpkinBack = false;
        Plant* aPumpkin = nullptr;

        if (IsOnBoard())
        {
            aPumpkin = mBoard->GetPumpkinAt(mPlantCol, mRow);
            if (aPumpkin)
            {
                Plant* aPlantInPumpkin = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
                if (aPlantInPumpkin)
                {
                    if (aPlantInPumpkin->mRenderOrder > aPumpkin->mRenderOrder || aPlantInPumpkin->mOnBungeeState == GETTING_GRABBED_BY_BUNGEE)
                    {
                        aPlantInPumpkin = nullptr;
                    }
                }

                if (aPlantInPumpkin == this)
                {
                    aDrawPumpkinBack = true;
                }
                if (aPlantInPumpkin == nullptr && mSeedType == SeedType::SEED_PUMPKINSHELL)
                {
                    aDrawPumpkinBack = true;
                }
            }
            else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
            {
                aDrawPumpkinBack = true;
                aPumpkin = this;
            }
        }
        else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
        {
            aDrawPumpkinBack = true;
            aPumpkin = this;
        }

        // 🌟 [Pro Mode] 1. التحقق من غوص مدفع الذرة في المسبح دون زنبقة
        bool isSubmergedCobCannon = false;
        if (mSeedType == SeedType::SEED_COBCANNON && IsOnBoard() && mBoard->IsPoolSquare(mPlantCol, mRow))
        {
            Plant* aLily = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
            if (!aLily || aLily->mSeedType != SeedType::SEED_LILYPAD)
            {
                isSubmergedCobCannon = true;
            }
        }

        DrawShadow(g, aOffsetX, aOffsetY);

        // 🌟 [Pro Mode] تمت إزالة رسم IMAGE_WHITEWATER_SHADOW من هنا بالكامل

        if (Plant::IsFlying(mSeedType))
        {
            int aCounter;
            if (IsOnBoard())
            {
                aCounter = mBoard->mMainCounter;
            }
            else
            {
                aCounter = mApp->mAppCounter;
            }

            float aTime = (mRow * 97 + mPlantCol * 61 + aCounter) * 0.03f;
            float aWave = sin(aTime) * 2.0f;
            aOffsetY += aWave;
        }

        if (aDrawPumpkinBack)
        {
            Reanimation* aPumpkinReanim = mApp->ReanimationGet(aPumpkin->mBodyReanimID);
            Graphics aPumpkinGraphics(*g);
            aPumpkinGraphics.mTransX += aPumpkin->mX - mX;
            aPumpkinGraphics.mTransY += aPumpkin->mY - mY;
            aPumpkinReanim->DrawRenderGroup(&aPumpkinGraphics, 1);
        }

        aOffsetX += mShakeOffsetX;
        aOffsetY += mShakeOffsetY;

        // 🌟 [Pro Mode] 2. إخفاء الجزء السفلي مع كشف 5 بكسل إضافية (القص عند 265)
        if (isSubmergedCobCannon)
        {
            g->ClipRect(-100, -200, 300, 280);
        }

        if (IsInPlay() && mApp->IsIZombieLevel())
        {
            mBoard->mChallenge->IZombieDrawPlant(g, this);
        }
        else if (mBodyReanimID != ReanimationID::REANIMATIONID_NULL)
        {
            Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
            if (aBodyReanim)
            {
                if (!mApp->Is3dAccel() && mSeedType == SeedType::SEED_FLOWERPOT && IsOnBoard() &&
                    aBodyReanim->mAnimRate == 0.0f && aBodyReanim->IsAnimPlaying("anim_idle"))
                {
                    mApp->mReanimatorCache->DrawCachedPlant(g, aOffsetX, aOffsetY, mSeedType, DrawVariation::VARIATION_NORMAL);
                }
                else
                {
                    aBodyReanim->Draw(g);
                }
            }
        }
        else
        {
            SeedType aSeedType = SeedType::SEED_NONE;
            if (mBoard)
            {
                aSeedType = mBoard->GetSeedTypeInCursor();
            }

            if (IsPartOfUpgradableTo(aSeedType) && mBoard->CanPlantAt(mPlantCol, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (aSeedType == SeedType::SEED_COBCANNON && mBoard->CanPlantAt(mPlantCol - 1, mRow, aSeedType) == PlantingReason::PLANTING_OK)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }
            else if (mBoard && mBoard->mTutorialState == TutorialState::TUTORIAL_SHOVEL_DIG)
            {
                g->SetColorizeImages(true);
                g->SetColor(GetFlashingColor(mBoard->mMainCounter, 90));
            }

            TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
            g->SetColorizeImages(false);
            if (mHighlighted)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, 196));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
            else if (mEatenFlashCountdown > 0)
            {
                g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
                g->SetColorizeImages(true);
                g->SetColor(Color(255, 255, 255, ClampInt(mEatenFlashCountdown * 3, 0, 255)));
                TodDrawImageCelF(g, aPlantImage, aOffsetX, aOffsetY, aImageIndex, 0);
                g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
                g->SetColorizeImages(false);
            }
        }
        if (isSubmergedCobCannon)
        {
            g->ClearClipRect();
        }
        if ((mSeedType == SeedType::SEED_MAGNETSHROOM || mSeedType == SeedType::SEED_SUN_MAGNET) && !DrawMagnetItemsOnTop())
        {
            DrawMagnetItems(g);
        }
    }
}
void Plant::DrawSeedType(Graphics* g, SeedType theSeedType, SeedType theImitaterType, DrawVariation theDrawVariation, float thePosX, float thePosY)
{
    Graphics aSeedG(*g);
    int aCelRow = 0;
    int aCelCol = 2;
    float aOffsetX = 0.0f;
    float aOffsetY = 0.0f;
    SeedType aSeedType = theSeedType;
    DrawVariation aDrawVariation = theDrawVariation;

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        aSeedType = theImitaterType;
        aDrawVariation = DrawVariation::VARIATION_IMITATER;
        if (theImitaterType == SeedType::SEED_HYPNOSHROOM || theImitaterType == SeedType::SEED_SQUASH || theImitaterType == SeedType::SEED_PRIMALPOTATOMINE|| theImitaterType == SeedType::SEED_POTATOMINE ||
            theImitaterType == SeedType::SEED_GARLIC || theImitaterType == SeedType::SEED_LILYPAD)
            aDrawVariation = DrawVariation::VARIATION_IMITATER_LESS;
    }
    else if (theDrawVariation == DrawVariation::VARIATION_NORMAL && theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aDrawVariation = DrawVariation::VARIATION_AQUARIUM;
    }

    if (((LawnApp*)gSexyAppBase)->mGameMode == GameMode::GAMEMODE_CHALLENGE_BIG_TIME &&
        (aSeedType == SeedType::SEED_WALLNUT || aSeedType == SeedType::SEED_SUNFLOWER || aSeedType == SeedType::SEED_MARIGOLD))
    {
        aSeedG.mScaleX *= 1.5f;
        aSeedG.mScaleY *= 1.5f;
        aOffsetX = -20.0f;
        aOffsetY = -40.0f;
    }
    if (aSeedType == SeedType::SEED_LEFTPEATER)
    {
        aOffsetX += aSeedG.mScaleX * 80.0f;
        aSeedG.mScaleX *= -1.0f;
    }

    if (Challenge::IsZombieSeedType(aSeedType))
    {
        ZombieType aZombieType = Challenge::IZombieSeedTypeToZombieType(aSeedType);
        gLawnApp->mReanimatorCache->DrawCachedZombie(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aZombieType);
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(aSeedType);

        if (aSeedType == SeedType::SEED_GIANT_WALLNUT)
        {
            aSeedG.mScaleX *= 1.4f;
            aSeedG.mScaleY *= 1.4f;
            TodDrawImageScaledF(&aSeedG, IMAGE_REANIM_WALLNUT_BODY, thePosX - 53.0f, thePosY - 56.0f, aSeedG.mScaleX, aSeedG.mScaleY);
        }
        else if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
        {
            if (aSeedType == SeedType::SEED_SUNPEA || aSeedType == SeedType::SEED_GOOPEA || aSeedType == SeedType::SEED_ROCKPEA || aSeedType == SeedType::SEED_CPEA || aSeedType == SeedType::SEED_DROPPEA)
            {
                Reanimation* aBodyReanim = gLawnApp->AddReanimation(thePosX + aOffsetX, thePosY + aOffsetY, 0, aPlantDef.mReanimationType);
                aBodyReanim->SetFramesForLayer("anim_idle");
                Reanimation* aHeadReanim = gLawnApp->AddReanimation(0.0f, 0.0f, 0, aPlantDef.mReanimationType);
                aHeadReanim->SetFramesForLayer("anim_head_idle");
                if (aBodyReanim->TrackExists("anim_stem"))
                    aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_stem");
                else if (aBodyReanim->TrackExists("anim_idle"))
                    aHeadReanim->AttachToAnotherReanimation(aBodyReanim, "anim_idle");
                aBodyReanim->OverrideScale(aSeedG.mScaleX, aSeedG.mScaleY);
                aBodyReanim->Update();
                aBodyReanim->Draw(&aSeedG);
                gLawnApp->RemoveReanimation(gLawnApp->ReanimationGetID(aHeadReanim));
                gLawnApp->RemoveReanimation(gLawnApp->ReanimationGetID(aBodyReanim));
            }
            else
            {
                gLawnApp->mReanimatorCache->DrawCachedPlant(&aSeedG, thePosX + aOffsetX, thePosY + aOffsetY, aSeedType, aDrawVariation);
            }
        }
        else
        {
            if (aSeedType == SeedType::SEED_KERNELPULT)
            {
                aCelRow = 2;
            }
            else if (aSeedType == SeedType::SEED_TWINSUNFLOWER)
            {
                aCelRow = 1;
            }

            Image* aPlantImage = Plant::GetImage(aSeedType);
            if (aPlantImage->mNumCols <= 2)
            {
                aCelCol = aPlantImage->mNumCols - 1;
            }

            TodDrawImageCelScaledF(&aSeedG, aPlantImage, thePosX + aOffsetX, thePosY + aOffsetY, aCelCol, aCelRow, aSeedG.mScaleX, aSeedG.mScaleY);
        }
    }
}

void Plant::MouseDown(int x, int y, int theClickCount)
{
    if (theClickCount < 0)
        return;

    if (mState == PlantState::STATE_COBCANNON_READY)
    {
        mBoard->ClearCursor();
        mBoard->mCursorObject->mType = SeedType::SEED_NONE;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_COBCANNON_TARGET;
        mBoard->mCursorObject->mSeedBankIndex = -1;
        mBoard->mCursorObject->mCoinID = CoinID::COINID_NULL;
        mBoard->mCursorObject->mCobCannonPlantID = (PlantID)mBoard->mPlants.DataArrayGetID(this);
        mBoard->mCobCannonCursorDelayCounter = 30;
        mBoard->mCobCannonMouseX = x;
        mBoard->mCobCannonMouseY = y;
    }
    if (mSeedType == SeedType::SEED_BEEHIVE && mState == PlantState::STATE_BEEHIVE_IDLE)
    {
        mState = PlantState::STATE_BEEHIVE_COOLDOWN;
        mStateCountdown = (mBeePhase == 1) ? 4000 : 8000;
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW); 
    }
    if (mSeedType == SeedType::SEED_TRAMPALINE && mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL)
    {
        mTrampolineReversed = !mTrampolineReversed;
        mApp->PlaySample(Sexy::SOUND_TAP);
        return;
    }
    if (mSeedType == SeedType::SEED_MAGNIFYING_GRASS)
    {
        if (mIsAsleep || mDead || mSquished || mState == PlantState::STATE_DOINGSPECIAL)
        {
            mApp->PlaySample(Sexy::SOUND_BUZZER);
            return;
        }
        if (mBoard->mSunMoney >= 50)
        {
            mBoard->mSunMoney -= 50;
            mState = PlantState::STATE_DOINGSPECIAL; 
            PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 24.0f);
            mApp->PlaySample(Sexy::SOUND_THROW);
        }
        else
        {
            mApp->PlaySample(Sexy::SOUND_BUZZER);
        }
        return;
    }
    if (mSeedType == SeedType::SEED_SUN_MAGNET)
    {
        if (!mSunMagnetActive)
        {
            if (mBoard->mSunMoney >= 25)
            {
                mBoard->mSunMoney -= 25;
                mSunMagnetActive = true;
                mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);

                TodParticleSystem* p = mApp->AddTodParticle(mX + 40, mY + 40, mRenderOrder + 1, ParticleEffect::PARTICLE_LANTERN_SHINE);
                if (p) {
                    p->OverrideScale(nullptr, 1.5f);
                    p->OverrideColor(nullptr, Color(255, 255, 0, 255));
                    mSunMagnetAuraID = mApp->ParticleGetID(p);
                }
                PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 20.0f);
            }
            else
            {
                mApp->PlaySample(Sexy::SOUND_BUZZER);
            }
        }
        else
        {
            mSunMagnetActive = false;
            mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
            if (mSunMagnetAuraID != ParticleSystemID::PARTICLESYSTEMID_NULL)
            {
                mApp->RemoveParticle(mSunMagnetAuraID);
                mSunMagnetAuraID = ParticleSystemID::PARTICLESYSTEMID_NULL;
            }
            for (int i = 0; i < 50; i++) {
                mTrackedZombieID[i] = ZOMBIEID_NULL;
            }
        }
        return;
    }
    if (mSeedType == SeedType::SEED_CUCKUMBER)
    {
        mCoconutClickCount++;
        mApp->PlaySample(Sexy::SOUND_TAP);

        if (mCoconutClickCount >= 3)
        {
            mState = PlantState::STATE_DOINGSPECIAL;
            DoSpecial();
            return;
        }
    }
}

void Plant::IceZombies()
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        aZombie->HitIceTrap();
    }

    mBoard->mIceTrapCounter = 300;
    TodParticleSystem* aPoolSparklyParticle = mApp->ParticleTryToGet(mBoard->mPoolSparklyParticleID);
    if (aPoolSparklyParticle)
    {
        aPoolSparklyParticle->mDontUpdate = false;
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie)
    {
        aBossZombie->BossDestroyFireball();
    }
}

void Plant::BurnRow(int theRow)
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == theRow) && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            aZombie->RemoveColdEffects();
            aZombie->ApplyBurn();
        }
    }

    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridY == theRow && aGridItem->mGridItemType == GridItemType::GRIDITEM_LADDER)
        {
            aGridItem->GridItemDie();
        }
        else if (aGridItem->mGridY == theRow && aGridItem->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE)
        {
            aGridItem->TakeDamage(900);
        }
    }

    Zombie* aBossZombie = mBoard->GetBossZombie();
    if (aBossZombie && aBossZombie->mFireballRow == theRow)
    {
        aBossZombie->BossDestroyIceballInRow(theRow);
    }
}

void Plant::BlowAwayFliers(int theX, int theRow)
{
    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying())
        {
            if (aZombie->IsFlying() || aZombie->mZombieType == ZombieType::ZOMBIE_FLY)
            {
                aZombie->mBlowingAway = true;
            }
        }
    }
    mApp->PlaySample(SOUND_BLOVER);
    mBoard->mFogBlownCountDown = 4000;
}

void Plant::KillAllPlantsNearDoom()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mRow == mRow && aPlant->mPlantCol == mPlantCol)
        {
            aPlant->Die();
        }
    }
}

void Plant::DoSpecial()
{
    int aPosX = mX + mWidth / 2;
    int aPosY = mY + mHeight / 2;
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);

    switch (mSeedType)
    {
    case SeedType::SEED_BLOVER:
    {
        if (mState != PlantState::STATE_DOINGSPECIAL)
        {
            mState = PlantState::STATE_DOINGSPECIAL;
            BlowAwayFliers(mX, mRow);
        }
        break;
    }
    case SeedType::SEED_EXPLODE_O_NUT:
    case SeedType::SEED_CHERRYBOMB:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        if (mBoard->GetAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, aDamageRangeFlags) >= 10 && !mApp->mPlayingQuickplay)
        {
            mApp->GetAchievement(ACHIEVEMENT_EXPLODONATOR);
        }
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags);

        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_XSHROOM:
    {
        mApp->PlaySample(SOUND_DOOMSHROOM);
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->EffectedByDamage(127))
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetCircleRectOverlap(aPosX, aPosY, 250, aZombieRect))
                {
                    aZombie->TakeDamage(3600, 0U);
                    aZombie->ApplyBurn();
                }
            }
        }
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
        if (aParticle)
        {
            aParticle->OverrideColor(nullptr, Color(255, 0, 0, 255));
            aParticle->OverrideScale(nullptr, 1.5f);
        }
        mBoard->ShakeBoard(6, -8);
        Die();
        break;
    }
    case SeedType::SEED_CUCKUMBER:
    {
        mApp->PlayFoley(FoleyType::FOLEY_CHERRYBOMB);
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->EffectedByDamage(aDamageRangeFlags))
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                if (GetCircleRectOverlap(aPosX, aPosY, 115, aZombieRect))
                {
                    aZombie->TakeDamage(1800, 0U);
                }
            }
        }
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POWIE);
        if (aParticle)
        {
            aParticle->OverrideColor(nullptr, Color(255, 255, 255, 255));
        }
        mBoard->ShakeBoard(3, -4);
        Die();
        break;
    }
    case SeedType::SEED_DOOMSHROOM:
    {
        mApp->PlaySample(SOUND_DOOMSHROOM);
        if (mBoard->mNuclearRainActive)
        {
            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying())
                {
                    aZombie->TakeDamage(20000, 0U);
                }
            }
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 5000, MAX_GRID_SIZE_Y, true, aDamageRangeFlags);
            mBoard->KillAllPlantsInRadius(aPosX, aPosY, 200);
            TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
            if (aParticle)
            {
                aParticle->OverrideColor(nullptr, Color(127, 255, 0, 255));
                aParticle->OverrideScale(nullptr, 2.5f);
            }
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    int nx = mPlantCol + dx;
                    int ny = mRow + dy;
                    if (nx >= 0 && nx < MAX_GRID_SIZE_X && ny >= 0 && ny < MAX_GRID_SIZE_Y)
                    {
                        if (mBoard->GetGridItemAt(GridItemType::GRIDITEM_INFECTION, nx, ny) == nullptr)
                        {
                            GridItem* aInfection = mBoard->AddInfectionGridItem(nx, ny);
                            if (aInfection)
                            {
                                aInfection->mGridItemCounter = 18000;
                            }
                        }
                    }
                }
            }
            mBoard->ShakeBoard(60, -60);
        }
        else
        {
            mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 250, 3, true, aDamageRangeFlags);
            KillAllPlantsNearDoom();
            TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_DOOM);
            if (aParticle){  aParticle->OverrideColor(nullptr, Color(255, 255, 0, 255));   aParticle->OverrideScale(nullptr, 2.5f);  }
            GridItem* aCrater = mBoard->AddACrater(mPlantCol, mRow);
            if (aCrater)
            {
                aCrater->mGridItemCounter = 3000;
            }

            mBoard->ShakeBoard(3, -4);
        }
        Die();
        break;
    }
    case SeedType::SEED_JALAPENO:
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mApp->PlayFoley(FoleyType::FOLEY_JUICY);

        if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
        {
            Plant* p = nullptr;
            while (mBoard->IteratePlants(p))
            {
                if (p->mRow == mRow && p != this && !p->mDead)
                {
                    p->Die();
                }
            }
            BurnRow(mRow);
        }
        else
        {
            mBoard->DoFwoosh(mRow);
            BurnRow(mRow);
            mBoard->mIceTimer[mRow] = 20;
        }

        mBoard->ShakeBoard(3, -4);
        Die();
        break;
    }
    case SeedType::SEED_UMBRELLA:
    {
        if (mState != PlantState::STATE_UMBRELLA_TRIGGERED && mState != PlantState::STATE_UMBRELLA_REFLECTING)
        {
            mState = PlantState::STATE_UMBRELLA_TRIGGERED;
            mStateCountdown = 5;

            PlayBodyReanim("anim_block", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 22.0f);
        }

        break;
    }
    case SeedType::SEED_ICESHROOM:
    {
        mApp->PlayFoley(FoleyType::FOLEY_FROZEN);
        IceZombies();
        mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_ICE_TRAP);

        Die();
        break;
    }
    case SeedType::SEED_POTATOMINE:
    {
        aPosX = mX + mWidth / 2 - 20;
        aPosY = mY + mHeight / 2;

        mApp->PlaySample(SOUND_POTATO_MINE);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 60, 0, false, aDamageRangeFlags);
        if (!mApp->IsIZombieLevel() && !mApp->mPlayingQuickplay)
            mApp->GetAchievement(AchievementType::ACHIEVEMENT_SPUDOW);

        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        mApp->AddTodParticle(aPosX + 20.0f, aPosY, aRenderPosition, ParticleEffect::PARTICLE_POTATO_MINE);
        mBoard->ShakeBoard(3, -4);

        Die();
        break;
    }
    case SeedType::SEED_PRIMALPOTATOMINE:
    {
        aPosX = mX + mWidth / 2 - 20;
        aPosY = mY + mHeight / 2;
        mApp->PlaySample(SOUND_POTATO_MINE);
        mBoard->KillAllZombiesInRadius(mRow, aPosX, aPosY, 115, 1, true, aDamageRangeFlags);
        if (!mApp->IsIZombieLevel() && !mApp->mPlayingQuickplay)
            mApp->GetAchievement(AchievementType::ACHIEVEMENT_SPUDOW);
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, mRow, 0);
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_POTATO_MINE);
        if (aParticle)
        {
            aParticle->OverrideScale(nullptr, 2.0f);
        }
        mBoard->ShakeBoard(3, -4);
        Die();
        break;
    }
    case SeedType::SEED_INSTANT_COFFEE:
    {
        Plant* aPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
        if (aPlant && aPlant->mIsAsleep)
        {
            aPlant->mWakeUpCounter = 100;
        }

        mState = PlantState::STATE_DOINGSPECIAL;
        PlayBodyReanim("anim_crumble", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 22.0f);
        mApp->PlayFoley(FoleyType::FOLEY_COFFEE);

        break;
    }
    }
}
void Plant::ImitaterMorph()
{
    Die();
    Plant* aPlant = mBoard->AddPlant(mPlantCol, mRow, mImitaterType, SeedType::SEED_IMITATER);

    FilterEffect aFilter = FilterEffect::FILTER_EFFECT_WASHED_OUT;
    if (mImitaterType == SeedType::SEED_HYPNOSHROOM || mImitaterType == SeedType::SEED_SQUASH || mImitaterType == SeedType::SEED_POTATOMINE ||
        mImitaterType == SeedType::SEED_GARLIC || mImitaterType == SeedType::SEED_LILYPAD)
        aFilter = FilterEffect::FILTER_EFFECT_LESS_WASHED_OUT;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aBodyReanim)
    {
        aBodyReanim->mFilterEffect = aFilter;
    }
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(aPlant->mHeadReanimID);
    if (aHeadReanim)
    {
        aHeadReanim->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID2);
    if (aHeadReanim2)
    {
        aHeadReanim2->mFilterEffect = aFilter;
    }Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(aPlant->mHeadReanimID3);
    if (aHeadReanim3)
    {
        aHeadReanim3->mFilterEffect = aFilter;
    }
}

void Plant::UpdateImitater()
{
    if (mState != PlantState::STATE_IMITATER_MORPHING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_IMITATER_MORPHING;
            PlayBodyReanim("anim_explode", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 26.0f);
        }
    }
    else
    {
        Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
        if (aBodyReanim->ShouldTriggerTimedEvent(0.8f))
        {
            mApp->AddTodParticle(mX + 40, mY + 40, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_IMITATER_MORPH);
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            ImitaterMorph();
        }
    }
}

void Plant::CobCannonFire(int theTargetX, int theTargetY)
{
    TOD_ASSERT(mState == PlantState::STATE_COBCANNON_READY);

    mState = PlantState::STATE_COBCANNON_FIRING;
    mShootingCounter = 206;
    PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);

    mTargetX = theTargetX - 47.0f;
    mTargetY = theTargetY;

    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);
    ReanimatorTrackInstance* aTrackInstance = aBodyReanim->GetTrackInstanceByName("CobCannon_Cob");
    aTrackInstance->mTrackColor = Color::White;
}

void Plant::Fire(Zombie* theTargetZombie, int theRow, PlantWeapon thePlantWeapon)
{
    if (mSeedType == SeedType::SEED_FUMESHROOM)
    {
        int aDamage = mHasBooster ? 200 : 20;
        DoRowAreaDamage(aDamage, 2U);
        mApp->PlayFoley(FoleyType::FOLEY_FUME);
        return;
    }
    if (mSeedType == SeedType::SEED_GLOOMSHROOM)
    {
        int aDamage = mHasBooster ? 200 : 20;
        DoRowAreaDamage(aDamage, 2U);
        return;
    }
    if (mSeedType == SeedType::SEED_STARFRUIT)
    {
        StarFruitFire();
        return;
    }

    ProjectileType aProjectileType;
    switch (mSeedType)
    {
    case SeedType::SEED_PEASHOOTER:
    case SeedType::SEED_CPEA:
    case SeedType::SEED_REPEATER:
    case SeedType::SEED_THREEPEATER:
    case SeedType::SEED_SPLITPEA:
    case SeedType::SEED_GATLINGPEA:
    case SeedType::SEED_LEFTPEATER:
        aProjectileType = ProjectileType::PROJECTILE_PEA;
        break;
    case SeedType::SEED_SUNPEA:
        aProjectileType = ProjectileType::PROJECTILE_SUN;
        break;
    case SeedType::SEED_GOOPEA:
        aProjectileType = ProjectileType::PROJECTILE_GOO;
        break;
    case SeedType::SEED_DROPPEA:
        aProjectileType = ProjectileType::PROJECTILE_DROPL;
        break;
    case SeedType::SEED_ROCKPEA:
        aProjectileType = ProjectileType::PROJECTILE_ROCK;
        break;
    case SeedType::SEED_SNOWPEA:
        aProjectileType = ProjectileType::PROJECTILE_SNOWPEA;
        break;
    case SeedType::SEED_PEPPERPULT:
        aProjectileType = ProjectileType::PROJECTILE_PEPPER;
        break;
    case SeedType::SEED_CUCKUMBER:
    case SeedType::SEED_ACID_LEMON:
        aProjectileType = ProjectileType::PROJECTILE_ACID_LEMON;
        break;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SCAREDYSHROOM:
    case SeedType::SEED_SPORESHROOM:
        aProjectileType = ProjectileType::PROJECTILE_PUFF;
        break;
    case SeedType::SEED_SEASHROOM:
        aProjectileType = ProjectileType::PROJECTILE_PUFF2;
        break;
    case SeedType::SEED_CACTUS:
    case SeedType::SEED_CATTAIL:
        aProjectileType = ProjectileType::PROJECTILE_SPIKE;
        break;
    case SeedType::SEED_CABBAGEPULT:
        aProjectileType = ProjectileType::PROJECTILE_CABBAGE;
        break;
    case SeedType::SEED_KERNELPULT:
        aProjectileType = ProjectileType::PROJECTILE_KERNEL;
        break;
    case SeedType::SEED_MELONPULT:
        aProjectileType = ProjectileType::PROJECTILE_MELON;
        break;
    case SeedType::SEED_WINTERMELON:
        aProjectileType = ProjectileType::PROJECTILE_WINTERMELON;
        break;
    case SeedType::SEED_COBCANNON:
        aProjectileType = ProjectileType::PROJECTILE_COBBIG;
        break;
    case SeedType::SEED_SAKURA:
        aProjectileType = ProjectileType::PROJECTILE_SAKURA;
        break;
    default:
        TOD_ASSERT();
        break;
    }
    if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectileType = ProjectileType::PROJECTILE_BUTTER;
    }
    mApp->PlayFoley(FoleyType::FOLEY_THROW);
    if (mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_WINTERMELON)
    {
        mApp->PlayFoley(FoleyType::FOLEY_SNOW_PEA_SPARKLES);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SCAREDYSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        mApp->PlayFoley(FoleyType::FOLEY_PUFF);
    }

    int aOriginX, aOriginY;
    if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aOriginX = mX + 40;
        aOriginY = mY + 40;
    }
    else if (mSeedType == SeedType::SEED_SEASHROOM)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 63;
    }
    else if (mSeedType == SeedType::SEED_SAKURA)
    {
        aOriginX = mX + 40;
        aOriginY = mY + 25;
    }
    else if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_PEPPERPULT)
    {
        aOriginX = mX + 5;
        aOriginY = mY - 12;
    }
    else if (mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON)
    {
        aOriginX = mX + 25;
        aOriginY = mY - 46;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aOriginX = mX + 20;
        aOriginY = mY - 3;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
    {
        aOriginX = mX + 19;
        aOriginY = mY - 37;
    }
    else if (mSeedType == SeedType::SEED_KERNELPULT && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aOriginX = mX + 12;
        aOriginY = mY - 56;
    }
    else if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_ACID_LEMON || mSeedType == SeedType::SEED_SNOWPEA || mSeedType == SeedType::SEED_REPEATER
        || mSeedType == SeedType::SEED_SUNPEA || mSeedType == SeedType::SEED_ROCKPEA || mSeedType == SeedType::SEED_GOOPEA || mSeedType == SeedType::SEED_DROPPEA || mSeedType == SeedType::SEED_CPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 24;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX - 30;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_GATLINGPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginX = mX + aOffsetX + 34;
        aOriginY = mY + aOffsetY - 33;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA)
    {
        int aOffsetX, aOffsetY;
        GetPeaHeadOffset(aOffsetX, aOffsetY);
        aOriginY = mY + aOffsetY - 33;

        if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
        {
            aOriginX = mX + aOffsetX - 64;
        }
        else
        {
            aOriginX = mX + aOffsetX + 24;
        }
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        aOriginX = mX + 45;
        aOriginY = mY + 10;
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aOriginX = mX + 29;
        aOriginY = mY + 21;
    }
    else if (mSeedType == SeedType::SEED_CACTUS)
    {
        if (thePlantWeapon == PlantWeapon::WEAPON_PRIMARY)
        {
            aOriginX = mX + 93;
            aOriginY = mY - 50;
        }
        else
        {
            aOriginX = mX + 70;
            aOriginY = mY + 23;
        }
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aOriginX = mX - 44;
        aOriginY = mY - 184;
    }
    else
    {
        aOriginX = mX + 10;
        aOriginY = mY + 5;
    }

    // === تم التعديل هنا ليعتمد على mY الفيزيائي ===
    if (mBoard->GetFlowerPotAt(mPlantCol, mRow))
    {
        aOriginY -= 5;
    }

    if (mSeedType == SeedType::SEED_SNOWPEA)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 8, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_SNOWPEA_PUFF);
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 18, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    else if (mSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_LAWN_MOWER, mRow, 1);
        mApp->AddTodParticle(aOriginX + 27, aOriginY + 13, aRenderPosition, ParticleEffect::PARTICLE_PUFFSHROOM_MUZZLE);
    }
    if (mSeedType == SeedType::SEED_SAKURA)
    {
        for (int i = 0; i < 10; i++)
        {
            float angle = (i * 2 * PI) / 10.0f;
            Projectile* p = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aProjectileType);
            if (p)
            {
                p->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
                p->mVelX = cos(angle) * 4.5f;
                p->mVelY = sin(angle) * 4.5f;
                p->mMotionType = ProjectileMotion::MOTION_STAR;
            }
        }
        return;
    }

    Projectile* aProjectile = mBoard->AddProjectile(aOriginX, aOriginY, mRenderOrder - 1, theRow, aProjectileType);
    aProjectile->mDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    if (mSeedType == SeedType::SEED_SPORESHROOM) aProjectile->mIsSpore = true;

    if (mSeedType == SeedType::SEED_CABBAGEPULT || mSeedType == SeedType::SEED_KERNELPULT || mSeedType == SeedType::SEED_PEPPERPULT ||
        mSeedType == SeedType::SEED_MELONPULT || mSeedType == SeedType::SEED_WINTERMELON || mSeedType == SeedType::SEED_SPORESHROOM)
    {
        float aRangeX, aRangeY;
        if (theTargetZombie)
        {
            Rect aZombieRect = theTargetZombie->GetZombieRect();
            aRangeX = theTargetZombie->ZombieTargetLeadX(50.0f) - aOriginX - 30.0f;
            aRangeY = aZombieRect.mY - aOriginY;

            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_RIDING)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_POGO && theTargetZombie->mHasObject)
            {
                aRangeX -= 60.0f;
            }
            if (theTargetZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL)
            {
                aRangeX -= 40.0f;
            }
            if (theTargetZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
            {
                aRangeY = mBoard->GridToPixelY(8, mRow) - aOriginY;
            }
        }
        else
        {
            GridItem* aGrave = FindTargetPVZ2Grave(theRow, thePlantWeapon);
            if (aGrave)
            {
                Rect aGraveRect = aGrave->GetPVZ2GraveRect();
                aRangeX = aGraveRect.mX + (aGraveRect.mWidth / 2.0f) - aOriginX - 30.0f;
                aRangeY = aGraveRect.mY - aOriginY;
            }
            else
            {
                aRangeX = 700.0f - aOriginX;
                aRangeY = 0.0f;
            }
        }
        if (aRangeX < 40.0f)
        {
            aRangeX = 40.0f;
        }

        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelX = aRangeX / 120.0f;
        aProjectile->mVelY = 0.0f;
        aProjectile->mVelZ = aRangeY / 120.0f - 7.0f;
        aProjectile->mAccZ = 0.115f;
    }
    else if (mSeedType == SeedType::SEED_THREEPEATER)
    {
        if (theRow < mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = -3.0f;
            aProjectile->mShadowY += 80.0f;
        }
        else if (theRow > mRow)
        {
            aProjectile->mMotionType = ProjectileMotion::MOTION_THREEPEATER;
            aProjectile->mVelY = 3.0f;
            aProjectile->mShadowY -= 80.0f;
        }
    }
    else if (mSeedType == SeedType::SEED_PUFFSHROOM || mSeedType == SeedType::SEED_SEASHROOM)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_PUFF;
    }
    else if (mSeedType == SeedType::SEED_SPLITPEA && thePlantWeapon == PlantWeapon::WEAPON_SECONDARY)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    }
    else if (mSeedType == SeedType::SEED_LEFTPEATER)
    {
        aProjectile->mMotionType = ProjectileMotion::MOTION_BACKWARDS;
    }
    else if (mSeedType == SeedType::SEED_CATTAIL)
    {
        aProjectile->mVelX = 2.0f;
        aProjectile->mMotionType = ProjectileMotion::MOTION_HOMING;
        aProjectile->mTargetZombieID = mBoard->ZombieGetID(theTargetZombie);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aProjectile->mVelX = 0.001f;
        aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
        aProjectile->mMotionType = ProjectileMotion::MOTION_LOBBED;
        aProjectile->mVelY = 0.0f;
        aProjectile->mAccZ = 0.0f;
        aProjectile->mVelZ = -8.0f;
        if (mHasBooster)
        {
            aProjectile->mIsMeteorBoost = true;
            aProjectile->mCobTargetX = mTargetX;
            aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
            RemoveBoost();
        }
        else
        {
            aProjectile->mCobTargetX = mTargetX - 40;
            aProjectile->mCobTargetRow = mBoard->PixelToGridYKeepOnBoard(mTargetX, mTargetY);
        }
    }
}
Zombie* Plant::FindTargetZombie(int theRow, PlantWeapon thePlantWeapon)
{
    int aDamageRangeFlags = GetDamageRangeFlags(thePlantWeapon);
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    int aHighestWeight = 0;
    Zombie* aBestZombie = nullptr;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        int aRowDeviation = aZombie->mRow - theRow;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
        {
            aRowDeviation = 0;
        }

        if (!aZombie->mHasHead || aZombie->IsTangleKelpTarget())
        {
            if (mSeedType == SeedType::SEED_POTATOMINE || mSeedType == SeedType::SEED_PRIMALPOTATOMINE|| mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_TANGLEKELP)
            {
                continue;
            }
        }
        if (aZombie->mZombieType == ZombieType::ZOMBIE_BUG && aZombie->mPlantsEaten < 3 && mSeedType != SeedType::SEED_TOADSTOOL) continue;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_COCOON && mSeedType != SeedType::SEED_TOADSTOOL) continue;
        if (aZombie->mZombieType == ZombieType::ZOMBIE_FLY && mSeedType != SeedType::SEED_TOADSTOOL) continue;

        bool needPortalCheck = false;
        if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_PORTAL_COMBAT)
        {
            if (mSeedType == SeedType::SEED_PEASHOOTER || mSeedType == SeedType::SEED_ACID_LEMON || mSeedType == SeedType::SEED_CACTUS || mSeedType == SeedType::SEED_REPEATER)
            {
                needPortalCheck = true;
            }
        }

        if (mSeedType != SeedType::SEED_CATTAIL && mSeedType != SeedType::SEED_SAKURA)
        {
            if (mSeedType == SeedType::SEED_GLOOMSHROOM)
            {
                if (aRowDeviation < -1 || aRowDeviation > 1)
                {
                    continue;
                }
            }
            else if (needPortalCheck)
            {
                if (!mBoard->mChallenge->CanTargetZombieWithPortals(this, aZombie))
                {
                    continue;
                }
            }
            else if (aRowDeviation)
            {
                continue;
            }
        }

        if (aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            int aExtraRange = 0;

            if (mSeedType == SeedType::SEED_CHOMPER || mSeedType == SeedType::SEED_TOADSTOOL)
            {
                if (aZombie->mZombiePhase == ZombiePhase::PHASE_DIGGER_WALKING)
                {
                    aAttackRect.mX += 20;
                    aAttackRect.mWidth -= 20;
                }

                if (aZombie->mZombiePhase == ZombiePhase::PHASE_POGO_BOUNCING || (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol == mPlantCol))
                {
                    continue;
                }

                if (aZombie->mIsEating || mState == PlantState::STATE_CHOMPER_BITING)
                {
                    aExtraRange = 60;
                }
            }

            if (mSeedType == SeedType::SEED_POTATOMINE)
            {
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mHasObject) ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    continue;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
                {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != mPlantCol)
                {
                    continue;
                }

                if (aZombie->mIsEating)
                {
                    aExtraRange = 30;
                }
            }
            if (mSeedType == SeedType::SEED_PRIMALPOTATOMINE)
            {
                if ((aZombie->mZombieType == ZombieType::ZOMBIE_POGO && aZombie->mHasObject) ||
                    aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT || aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_PRE_VAULT)
                {
                    continue;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_POLEVAULTER)
                {
                    aAttackRect.mX += 40;
                    aAttackRect.mWidth -= 40;
                }

                if (aZombie->mZombieType == ZombieType::ZOMBIE_BUNGEE && aZombie->mTargetCol != mPlantCol)
                {
                    continue;
                }

                if (aZombie->mIsEating)
                {
                    aExtraRange = 30;
                }
            }
            if ((mSeedType == SeedType::SEED_EXPLODE_O_NUT && aZombie->mZombiePhase == ZombiePhase::PHASE_POLEVAULTER_IN_VAULT) ||
                (mSeedType == SeedType::SEED_TANGLEKELP && !aZombie->mInPool))
            {
                continue;
            }

            Rect aZombieRect = aZombie->GetZombieRect();
            if (!needPortalCheck && GetRectOverlap(aAttackRect, aZombieRect) < -aExtraRange)
            {
                continue;
            }

            int aWeight = -aZombieRect.mX;
            if (mSeedType == SeedType::SEED_CATTAIL)
            {
                aWeight = -Distance2D(mX + 40.0f, mY + 40.0f, aZombieRect.mX + aZombieRect.mWidth / 2, aZombieRect.mY + aZombieRect.mHeight / 2);
                if (aZombie->IsFlying())
                {
                    aWeight += 10000;
                }
            }

            if (aBestZombie == nullptr || aWeight > aHighestWeight)
            {
                aHighestWeight = aWeight;
                aBestZombie = aZombie;
            }
        }
    }

    return aBestZombie;
}

int Plant::DistanceToClosestZombie()
{
    int aDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
    Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
    int aClosestDistance = 1000;

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (aZombie->mRow == mRow && aZombie->EffectedByDamage(aDamageRangeFlags))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            int aDistance = -GetRectOverlap(aAttackRect, aZombieRect);
            if (aDistance < aClosestDistance)
            {
                aClosestDistance = max(aDistance, 0);
            }
        }
    }

    return aClosestDistance;
}

void Plant::Die()
{
    if (mDead) return;
   
    if (IsOnBoard() && mSeedType == SeedType::SEED_EXPLODE_O_NUT && IsInPlay() && mState != PlantState::STATE_DOINGSPECIAL)
    {
        mState = PlantState::STATE_DOINGSPECIAL;
        DoSpecial();
        return;
    }
    if (IsOnBoard() && mSeedType == SeedType::SEED_HYPNOSHROOM && mHasBooster)
    {
        Zombie* aTargetZombie = nullptr;
        Zombie* aZombie = nullptr;
        while (mBoard->IterateZombies(aZombie))
        {
            if (aZombie->mRow == mRow && !aZombie->IsDeadOrDying() && !aZombie->mMindControlled)
            {
                Rect aZombieRect = aZombie->GetZombieRect();
                Rect aPlantRect = GetPlantRect();
                if (GetRectOverlap(aPlantRect, aZombieRect) >= 0 && aZombie->mIsEating)
                {
                    aTargetZombie = aZombie;
                    break;
                }
            }
        }
        if (aTargetZombie)
        {
            float aSpawnX = aTargetZombie->mPosX;
            float aSpawnY = aTargetZombie->mPosY;
            int aRealRow = aTargetZombie->mRow;
            aTargetZombie->TakeDamage(1800, 0U);
            Zombie* aGarg = mBoard->AddZombie(
                ZombieType::ZOMBIE_GARGANTUAR,
                aRealRow
            );
            if (aGarg)
            {
                aGarg->mRow = aRealRow;
                aGarg->mPosX = aSpawnX - 40.0f;
                aGarg->mPosY = aSpawnY;
                aGarg->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, aRealRow, 0);
                aGarg->StartMindControlled();
            }
        }
        int aPosX = mX + mWidth / 2;
        int aPosY = mY + mHeight / 2;
        TodParticleSystem* aParticle = mApp->AddTodParticle(aPosX, aPosY, (int)RenderLayer::RENDER_LAYER_TOP, ParticleEffect::PARTICLE_FIREBALL_DEATH);
        if (aParticle)
        {
            aParticle->OverrideColor(nullptr, Color(218, 165, 232, 255));
        }
        RemoveBoost();
    }
    if (IsOnBoard() && mSeedType == SeedType::SEED_TANGLEKELP)
    {
        Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
        if (aZombie)
        {
            aZombie->DieWithLoot();
        }
    }
    if (IsOnBoard() && mSeedType == SeedType::SEED_TORCHWOOD || mSeedType == SeedType::SEED_PLASMAWOOD)
    {
        mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
        mBoard->DoFwoosh(mRow);       
        mBoard->ShakeBoard(3, -4);    
        mBoard->mIceTimer[mRow] = 20; 
        BurnRow(mRow);
    }
    int aSavedCol = mPlantCol;
    int aSavedRow = mRow;
    SeedType aSavedType = mSeedType;
    mDead = true;
    RemoveEffects();
    if (IsOnBoard() && mBoard->IsStackablePlant(aSavedType))
    {
        mBoard->UpdateStackedPlantPositions(aSavedCol, aSavedRow, aSavedType);
    }

    if (!Plant::IsFlying(mSeedType) && IsOnBoard())
    {
        GridItem* aLadder = mBoard->GetLadderAt(mPlantCol, mRow);
        if (aLadder)
        {
            aLadder->GridItemDie();
        }
    }

    if (IsOnBoard())
    {
        Plant* aTopPlant = mBoard->GetTopPlantAt(mPlantCol, mRow, PlantPriority::TOPPLANT_BUNGEE_ORDER);
        Plant* aFlowerPot = mBoard->GetFlowerPotAt(mPlantCol, mRow);
        if (aFlowerPot && aTopPlant == aFlowerPot)
        {
            Reanimation* aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
            aPotReanim->mAnimRate = RandRangeFloat(10.0f, 15.0f);
        }
    }

}

PlantDefinition& GetPlantDefinition(SeedType theSeedType)
{
    TOD_ASSERT(gPlantDefs[theSeedType].mSeedType == theSeedType);
    TOD_ASSERT(theSeedType >= 0 && theSeedType < (int)SeedType::NUM_SEED_TYPES);
    
    return gPlantDefs[theSeedType];
}

int Plant::GetCost(SeedType theSeedType, SeedType theImitaterType)
{
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED || gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_BEGHOULED_TWIST)
    {
        if (theSeedType == SeedType::SEED_REPEATER)
        {
            return 1000;
        }
        else if (theSeedType == SeedType::SEED_FUMESHROOM)
        {
            return 500;
        }
        else if (theSeedType == SeedType::SEED_TALLNUT)
        {
            return 250;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_SHUFFLE)
        {
            return 100;
        }
        else if (theSeedType == SeedType::SEED_BEGHOULED_BUTTON_CRATER)
        {
            return 200;
        }
    }

    int aCost = 0;

    switch (theSeedType)
    {
    case SeedType::SEED_SLOT_MACHINE_SUN:           aCost = 0; break;
    case SeedType::SEED_SLOT_MACHINE_DIAMOND:       aCost = 0; break;
    case SeedType::SEED_ZOMBIQUARIUM_SNORKLE:       aCost = 100; break;
    case SeedType::SEED_ZOMBIQUARIUM_TROPHY:        aCost = 1000; break;
    case SeedType::SEED_ZOMBIE_NORMAL:              aCost = 50; break;
    case SeedType::SEED_ZOMBIE_TRAFFIC_CONE:        aCost = 75; break;
    case SeedType::SEED_ZOMBIE_POLEVAULTER:         aCost = 75; break;
    case SeedType::SEED_ZOMBIE_PAIL:                aCost = 125; break;
    case SeedType::SEED_ZOMBIE_LADDER:              aCost = 150; break;
    case SeedType::SEED_ZOMBIE_DIGGER:              aCost = 125; break;
    case SeedType::SEED_ZOMBIE_BUNGEE:              aCost = 125; break;
    case SeedType::SEED_ZOMBIE_FOOTBALL:            aCost = 175; break;
    case SeedType::SEED_ZOMBIE_BALLOON:             aCost = 150; break;
    case SeedType::SEED_ZOMBIE_SCREEN_DOOR:         aCost = 100; break;
    case SeedType::SEED_ZOMBONI:                    aCost = 175; break;
    case SeedType::SEED_ZOMBIE_POGO:                aCost = 200; break;
    case SeedType::SEED_ZOMBIE_DANCER:              aCost = 350; break;
    case SeedType::SEED_ZOMBIE_GARGANTUAR:          aCost = 300; break;
    case SeedType::SEED_ZOMBIE_IMP:                 aCost = 50; break;
    default:
    {
        if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
            aCost = aPlantDef.mSeedCost;
        }
        else
        {
            const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
            aCost = aPlantDef.mSeedCost;
        }
        break;
    }
    }
    if (theSeedType == SeedType::SEED_SAKURA)
    {
        int count = 0;
        if (gLawnApp && gLawnApp->mBoard) {
            Plant* p = nullptr;
            while (gLawnApp->mBoard->IteratePlants(p)) {
                if (p->mSeedType == SeedType::SEED_SAKURA) count++;
            }
        }
        return 500 + (count * 25);
    }
    if (gLawnApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_INFLATION)
    {
        aCost *= 2;
    }

    return aCost;
}

SexyString Plant::GetNameString(SeedType theSeedType, SeedType theImitaterType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aName = StrFormat(_S("[%s]"), aPlantDef.mPlantName);
    SexyString aTranslatedName = TodStringTranslate(StringToSexyStringFast(aName));

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aImitaterDef = GetPlantDefinition(theImitaterType);
        std::string aImitaterName = StrFormat(_S("[%s]"), aImitaterDef.mPlantName);
        std::string aTranslatedImitaterName = TodStringTranslate(StringToSexyStringFast(aImitaterName));
        return StrFormat(_S("%s %s"), aTranslatedName.c_str(), aTranslatedImitaterName.c_str());
    }

    return aTranslatedName;
}

SexyString Plant::GetToolTip(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    SexyString aToolTip = StrFormat(_S("[%s_TOOLTIP]"), aPlantDef.mPlantName);
    return TodStringTranslate(aToolTip);
}

int Plant::GetRefreshTime(SeedType theSeedType, SeedType theImitaterType)
{
    if (Challenge::IsZombieSeedType(theSeedType))
    {
        return 0;
    }

    if (theSeedType == SeedType::SEED_IMITATER && theImitaterType != SeedType::SEED_NONE)
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theImitaterType);
        return aPlantDef.mRefreshTime;
    }
    else
    {
        const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
        return aPlantDef.mRefreshTime;
    }
}

bool Plant::IsNocturnal(SeedType theSeedtype)
{
    return
        theSeedtype == SeedType::SEED_PUFFSHROOM ||
        theSeedtype == SeedType::SEED_SEASHROOM ||
        theSeedtype == SeedType::SEED_SUNSHROOM ||
        theSeedtype == SeedType::SEED_FUMESHROOM ||
        theSeedtype == SeedType::SEED_HYPNOSHROOM ||
        theSeedtype == SeedType::SEED_DOOMSHROOM ||
        theSeedtype == SeedType::SEED_ICESHROOM ||
        theSeedtype == SeedType::SEED_MAGNETSHROOM ||
        theSeedtype == SeedType::SEED_SCAREDYSHROOM ||
        theSeedtype == SeedType::SEED_SPORESHROOM ||
        theSeedtype == SeedType::SEED_POISONSHROOM ||
        theSeedtype == SeedType::SEED_HAMMER_SHROOM ||
        theSeedtype == SeedType::SEED_XSHROOM ||
        theSeedtype == SeedType::SEED_HAMMER_SHROOM ||
        theSeedtype == SeedType::SEED_GUARDIAN_SHROOM ||
        theSeedtype == SeedType::SEED_MINI_GUARDIAN_SHROOM ||
        theSeedtype == SeedType::SEED_GLOOMSHROOM;
}

bool Plant::IsAquatic(SeedType theSeedType)
{
    return
        theSeedType == SeedType::SEED_LILYPAD ||
        theSeedType == SeedType::SEED_TANGLEKELP ||
        theSeedType == SeedType::SEED_SEASHROOM ||
        theSeedType == SeedType::SEED_CATTAIL;
}

bool Plant::IsFlying(SeedType theSeedtype)
{
    return theSeedtype == SeedType::SEED_INSTANT_COFFEE;
}

bool Plant::IsUpgrade(SeedType theSeedtype)
{
    return 
        theSeedtype == SeedType::SEED_GATLINGPEA || 
        theSeedtype == SeedType::SEED_WINTERMELON || 
        theSeedtype == SeedType::SEED_TWINSUNFLOWER || 
        theSeedtype == SeedType::SEED_SPIKEROCK || 
        theSeedtype == SeedType::SEED_COBCANNON || 
        theSeedtype == SeedType::SEED_GOLD_MAGNET || 
        theSeedtype == SeedType::SEED_GLOOMSHROOM || 
        theSeedtype == SeedType::SEED_CATTAIL;
}

Rect Plant::GetPlantRect()
{
    Rect aRect;
    if (mSeedType == SeedType::SEED_TALLNUT)
    {
        aRect = Rect(mX + 10, mY, mWidth, mHeight);
    }
    else if (mSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (mSeedType == SeedType::SEED_COBCANNON)
    {
        aRect = Rect(mX, mY, 140, 80);
    }
    else
    {
        aRect = Rect(mX + 10, mY, mWidth - 20, mHeight);
    }

    return aRect;
}

Rect Plant::GetPlantAttackRect(PlantWeapon thePlantWeapon)
{
    Rect aRect;
    if (mApp->IsWallnutBowlingLevel())
    {
        aRect = Rect(mX, mY, mWidth - 20, mHeight);
    }
    else if (thePlantWeapon == PlantWeapon::WEAPON_SECONDARY && mSeedType == SeedType::SEED_SPLITPEA)
    {
        aRect = Rect(0, mY, mX + 16, mHeight);
    }
    else switch (mSeedType)
    {
    case SeedType::SEED_LEFTPEATER:     aRect = Rect(0,             mY,             mX,                 mHeight);               break;
    case SeedType::SEED_SPORESHROOM:    aRect = Rect(mX + 40, mY, 360, mHeight);                break;
    case SeedType::SEED_SQUASH:         aRect = Rect(mX + 20,       mY,             mWidth - 35,        mHeight);               break;
    case SeedType::SEED_CHOMPER:        aRect = Rect(mX + 80,       mY,             40,                 mHeight);               break;
    case SeedType::SEED_TOADSTOOL:      aRect = Rect(mX + 80, mY, 160, mHeight);               break; 
    case SeedType::SEED_SPIKEWEED:
    case SeedType::SEED_SPIKEROCK:      aRect = Rect(mX + 20,       mY,             mWidth - 50,        mHeight);               break;
    case SeedType::SEED_POTATOMINE:
    case SeedType::SEED_PRIMALPOTATOMINE:     aRect = Rect(mX,            mY,             mWidth - 25,        mHeight);               break;
    case SeedType::SEED_PLASMAWOOD:
    case SeedType::SEED_TORCHWOOD:      aRect = Rect(mX + 50,       mY,             30,                 mHeight);               break;
    case SeedType::SEED_PUFFSHROOM:
    case SeedType::SEED_SEASHROOM:      aRect = Rect(mX + 60,       mY,             230,                mHeight);               break;
    case SeedType::SEED_FUMESHROOM:     aRect = Rect(mX + 60,       mY,             340,                mHeight);               break;
    case SeedType::SEED_GLOOMSHROOM:
        if (mHasBooster)
        {
            aRect = Rect(mX - 160, mY - 160, 400, 400);
        }
        else
        {
            aRect = Rect(mX - 80, mY - 80, 240, 240);
        }
        break;
    case SeedType::SEED_TANGLEKELP:     aRect = Rect(mX,            mY,             mWidth,             mHeight);               break;
    case SeedType::SEED_SNAPDRAGON:     aRect = Rect(mX + 40, mY - 80, 240, 240);                   break;
    case SeedType::SEED_SAKURA:
    case SeedType::SEED_CATTAIL:        aRect = Rect(-BOARD_WIDTH,  -BOARD_HEIGHT,  BOARD_WIDTH * 2,    BOARD_HEIGHT * 2);      break;
    default:                            aRect = Rect(mX + 60,       mY,             BOARD_WIDTH,        mHeight);               break;
    }

    return aRect;
}

void Plant::PreloadPlantResources(SeedType theSeedType)
{
    const PlantDefinition& aPlantDef = GetPlantDefinition(theSeedType);
    if (aPlantDef.mReanimationType != ReanimationType::REANIM_NONE)
    {
        ReanimatorEnsureDefinitionLoaded(aPlantDef.mReanimationType, true);
    }

    if (theSeedType == SeedType::SEED_CHERRYBOMB)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_ZOMBIE_CHARRED, true);
    }
    else if (theSeedType == SeedType::SEED_JALAPENO)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_TORCHWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_FIRE_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (theSeedType == SeedType::SEED_PLASMAWOOD)
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_FIRE_PEA, true);
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_JALAPENO_FIRE, true);
    }
    else if (Plant::IsNocturnal(theSeedType))
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_SLEEPING, true);
    }
}

void Plant::PlayIdleAnim(float theRate)
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (aBodyReanim)
    {
        if (theRate <= 0.0f)
        {
            theRate = RandRangeFloat(10.0f, 15.0f);
        }

        PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, theRate);

        if (mApp->IsIZombieLevel())
        {
            aBodyReanim->mAnimRate = 0.0f;
        }
    }
}
GridItem* Plant::FindTargetPVZ2Grave(int theRow, PlantWeapon thePlantWeapon)
{
    Rect aAttackRect = GetPlantAttackRect(thePlantWeapon);
    GridItem* aBestGrave = nullptr;
    int aHighestWeight = 0;

    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE && aGridItem->mGridY == theRow)
        {
            Rect aGraveRect = aGridItem->GetPVZ2GraveRect();
            if (GetRectOverlap(aAttackRect, aGraveRect) >= 0)
            {
                int aWeight = -aGraveRect.mX;
                if (aBestGrave == nullptr || aWeight > aHighestWeight)
                {
                    aHighestWeight = aWeight;
                    aBestGrave = aGridItem;
                }
            }
        }
    }
    return aBestGrave;
}
void Plant::UpdateBeehive()
{
    if (mState == PlantState::STATE_BEEHIVE_COOLDOWN)
    {
        mStateCountdown--;
        if (mStateCountdown <= 0)
        {
            mState = PlantState::STATE_BEEHIVE_IDLE;
            if (mBeePhase < 3) mBeePhase++; 
        }
    }
}
void Plant::RemoveBoost()
{
    bool wasMainBoostTarget = (mBoostGlowParticleID != ParticleSystemID::PARTICLESYSTEMID_NULL);

    if (mHasBooster || wasMainBoostTarget)
    {
        mHasBooster = false;
        mBoostTimer = 0;

        if (wasMainBoostTarget)
        {
            mApp->RemoveParticle(mBoostGlowParticleID);
            mBoostGlowParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;

            if (IsOnBoard())
            {
                TodParticleSystem* aParticle = mApp->AddTodParticle(mX + 40.0f, mY + 40.0f, mRenderOrder + 2, ParticleEffect::PARTICLE_FIREBALL_DEATH);
                if (aParticle)
                {
                    aParticle->OverrideColor(nullptr, Color(0, 255, 0, 255));
                    aParticle->OverrideScale(nullptr, 0.5f);
                }
            }
        }
    }
}
void Plant::UpdateMagnifyingGrass()
{
    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (!aBodyReanim) return;
    bool hasLight = !mBoard->StageIsNight();
    if (!hasLight)
    {
        for (int dCol = -1; dCol <= 1; dCol++) {
            for (int dRow = -1; dRow <= 1; dRow++) {
                int checkCol = mPlantCol + dCol;
                int checkRow = mRow + dRow;
                if (checkCol >= 0 && checkCol < MAX_GRID_SIZE_X && checkRow >= 0 && checkRow < MAX_GRID_SIZE_Y) {
                    Plant* p = mBoard->GetTopPlantAt(checkCol, checkRow, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
                    if (p && p->mSeedType == SeedType::SEED_PLANTERN && !p->mIsAsleep) {
                        hasLight = true; break;
                    }
                }
            }
            if (hasLight) break;
        }
    }
    if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.60f))
        {
            Projectile* aProjectile = mBoard->AddProjectile(mX + 45, mY + 10, mRenderOrder - 1, mRow, ProjectileType::PROJECTILE_MAGNIFYING_GRASS);
            if (aProjectile) {
                aProjectile->mDamageRangeFlags = GetDamageRangeFlags(PlantWeapon::WEAPON_PRIMARY);
            }
        }
        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;
            if (hasLight) {
                PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
                SetSleeping(false);
            }
            else {
                PlayBodyReanim("anim_no_light_idle", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
                SetSleeping(true);
            }
        }
    }
    else 
    {
        if (hasLight && mIsAsleep)
        {
            SetSleeping(false);
            PlayBodyReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
        }
        else if (!hasLight && !mIsAsleep)
        {
            SetSleeping(true);
            PlayBodyReanim("anim_no_light_idle", ReanimLoopType::REANIM_LOOP, 20, RandRangeFloat(10.0f, 15.0f));
        }
    }
}
void Plant::UpdateSnapdragon()
{
    if (mState == PlantState::STATE_NOTREADY || mIsAsleep)
        return;

    Reanimation* aBodyReanim = mApp->ReanimationTryToGet(mBodyReanimID);
    if (!aBodyReanim) return;

    if (mState == PlantState::STATE_READY)
    {
        if (mShootingCounter > 0) mShootingCounter--;
        if (mShootingCounter <= 0)
        {
            Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
            bool hasTarget = false;
            Zombie* aZombie = nullptr;

            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1))
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                    {
                        hasTarget = true;
                        break;
                    }
                }
            }

            if (hasTarget)
            {
                mState = PlantState::STATE_DOINGSPECIAL;
                PlayBodyReanim("anim_shooting", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 24.0f);
            }
            else
            {
                mShootingCounter = 15;
            }
        }
    }
    else if (mState == PlantState::STATE_DOINGSPECIAL)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.5f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_JALAPENO_IGNITE);
            Rect aAttackRect = GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);

            for (int dCol = 1; dCol <= 3; dCol++)
            {
                for (int dRow = -1; dRow <= 1; dRow++)
                {
                    int c = mPlantCol + dCol;
                    int r = mRow + dRow;
                    if (c >= 0 && c < MAX_GRID_SIZE_X && r >= 0 && r < MAX_GRID_SIZE_Y)
                    {
                        float pX = mBoard->GridToPixelX(c, r) + 10.0f;
                        float pY = mBoard->GridToPixelY(c, r) - 10.0f;
                        int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PARTICLE, r, 1);
                        Reanimation* aFwoosh = mApp->AddReanimation(pX, pY, aRenderOrder, ReanimationType::REANIM_JALAPENO_FIRE);
                        if (aFwoosh)
                        {
                            aFwoosh->SetFramesForLayer("anim_flame");
                            aFwoosh->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
                            aFwoosh->mAnimRate *= RandRangeFloat(0.7f, 1.3f);
                            float aScale = RandRangeFloat(0.9f, 1.1f);
                            aFwoosh->OverrideScale(aScale, aScale);
                        }
                    }
                }
            }

            Zombie* aZombie = nullptr;
            while (mBoard->IterateZombies(aZombie))
            {
                if (!aZombie->IsDeadOrDying() && aZombie->EffectedByDamage(1))
                {
                    Rect aZombieRect = aZombie->GetZombieRect();
                    if (GetRectOverlap(aAttackRect, aZombieRect) > 0)
                    {
                        aZombie->RemoveColdEffects();
                        aZombie->TakeDamage(30, 0U);
                    }
                }
            }
        }

        if (aBodyReanim->mLoopCount > 0)
        {
            mState = PlantState::STATE_READY;
            mShootingCounter = 150;
            PlayIdleAnim(15.0f);
        }
    }
}
Coin* Plant::FindSunMagnetTarget()
{
    Coin* aClosestCoin = nullptr;
    float aClosestDistance = 0.0f;

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if ((aCoin->mType == CoinType::COIN_SUN || aCoin->mType == CoinType::COIN_SMALLSUN || aCoin->mType == CoinType::COIN_BIGSUN) &&
            aCoin->mCoinMotion != CoinMotion::COIN_MOTION_FROM_PRESENT && !aCoin->mIsBeingCollected && aCoin->mCoinAge >= 50)
        {
            float aDistance = Distance2D(mX + mWidth / 2, mY + mHeight / 2, aCoin->mPosX + aCoin->mWidth / 2, aCoin->mPosY + aCoin->mHeight / 2);
            if (aClosestCoin == nullptr || aDistance < aClosestDistance)
            {
                aClosestCoin = aCoin;
                aClosestDistance = aDistance;
            }
        }
    }

    return aClosestCoin;
}

bool Plant::IsASunMagnetAboutToSuck()
{
    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (!aPlant->NotOnGround() && aPlant->mSeedType == SeedType::SEED_SUN_MAGNET && aPlant->mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
        {
            Reanimation* aBodyReanim = mApp->ReanimationGet(aPlant->mBodyReanimID);
            if (aBodyReanim->mAnimTime < 0.5f)
            {
                return true;
            }
        }
    }
    return false;
}

void Plant::SunMagnetFindTargets()
{
    if (GetFreeMagnetItem() == nullptr)
    {
        return;
    }

    for (;;)
    {
        MagnetItem* aMagnetItem = GetFreeMagnetItem();
        if (aMagnetItem == nullptr)
            break;

        Coin* aCoin = FindSunMagnetTarget();
        if (aCoin == nullptr)
            break;

        aMagnetItem->mPosX = aCoin->mPosX + 15.0f;
        aMagnetItem->mPosY = aCoin->mPosY + 15.0f;
        aMagnetItem->mDestOffsetX = RandRangeFloat(20.0f, 40.0f);
        aMagnetItem->mDestOffsetY = RandRangeFloat(-20.0f, 0.0f) + 20.0f;

        if (aCoin->mType == CoinType::COIN_SMALLSUN) aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SMALLSUN;
        else if (aCoin->mType == CoinType::COIN_BIGSUN) aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_LARGESUN;
        else aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_SUN;

        aCoin->Die();
    }
}

void Plant::UpdateSunMagnet()
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(mBodyReanimID);

    bool aIsSuckingCoin = false;
    for (int i = 0; i < MAX_MAGNET_ITEMS; i++)
    {
        MagnetItem* aMagnetItem = &mMagnetItems[i];
        if (aMagnetItem->mItemType != MagnetItemType::MAGNET_ITEM_NONE)
        {
            SexyVector2 aVectorToPlant(mX + aMagnetItem->mDestOffsetX - aMagnetItem->mPosX, mY + aMagnetItem->mDestOffsetY - aMagnetItem->mPosY);
            float aDistance = aVectorToPlant.Magnitude();

            if (aDistance < 20.0f)
            {
                int aValue = 25;
                if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_SMALLSUN) aValue = 15;
                else if (aMagnetItem->mItemType == MagnetItemType::MAGNET_ITEM_LARGESUN) aValue = 50;

                mBoard->AddSunMoney(aValue);
                mApp->PlayFoley(FoleyType::FOLEY_SUN);

                aMagnetItem->mItemType = MagnetItemType::MAGNET_ITEM_NONE;
            }
            else
            {
                float aSpeed = TodAnimateCurveFloatTime(30.0f, 0.0f, aDistance, 0.02f, 0.05f, TodCurves::CURVE_LINEAR);
                aMagnetItem->mPosX += aVectorToPlant.x * aSpeed;
                aMagnetItem->mPosY += aVectorToPlant.y * aSpeed;

                aIsSuckingCoin = true;
            }
        }
    }

    if (mState == PlantState::STATE_MAGNETSHROOM_CHARGING)
    {
        if (mStateCountdown == 0)
        {
            mState = PlantState::STATE_READY;
        }
    }
    else if (mState == PlantState::STATE_MAGNETSHROOM_SUCKING)
    {
        if (aBodyReanim->ShouldTriggerTimedEvent(0.4f))
        {
            mApp->PlayFoley(FoleyType::FOLEY_MAGNETSHROOM);
            SunMagnetFindTargets();
        }

        if (aBodyReanim->mLoopCount > 0 && !aIsSuckingCoin)
        {
            PlayIdleAnim(14.0f);
            mState = PlantState::STATE_MAGNETSHROOM_CHARGING;
            mStateCountdown = RandRangeInt(200, 300);
        }
    }
    else if (!IsASunMagnetAboutToSuck() && Sexy::Rand(50) == 0 && FindSunMagnetTarget())
    {
        mState = PlantState::STATE_MAGNETSHROOM_SUCKING;
        PlayBodyReanim("anim_attract", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);
    }
}