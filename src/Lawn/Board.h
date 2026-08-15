#ifndef __BOARD_H__
#define __BOARD_H__

#include "../ConstEnums.h"
#include "../Sexy.TodLib/DataArray.h"
#include "../SexyAppFramework/Widget.h"
#include "../SexyAppFramework/ButtonListener.h"

#include "Plant.h"
#include "Zombie.h"
#include "Projectile.h"
#include "Coin.h"
#include "LawnMower.h"
#include "GridItem.h"
#include "Bush.h"

using namespace Sexy;

#define MAX_GRID_SIZE_X 9
#define MAX_GRID_SIZE_Y 6
#define MAX_ZOMBIES_IN_WAVE 50
#define MAX_ZOMBIE_WAVES 100
#define MAX_GRAVE_STONES (MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y)
#define MAX_POOL_GRID_SIZE 10
#define MAX_RENDER_ITEMS 8192
#define PROGRESS_METER_COUNTER 150

class LawnApp;
class CursorObject;
class CursorPreview;
class GameButton;
class MessageWidget;
class SeedBank;
class ToolTipWidget;
class CutScene;
class Challenge;
class Reanimation;
class DataSync;
class TodParticleSystem;

namespace Sexy
{
    class Graphics;
    class ButtonWidget;
    class WidgetManager;
    class Image;
    class MTRand;
}

class HitResult
{
public:
    void* mObject;
    GameObjectType                  mObjectType;
};

class RenderItem
{
public:
    RenderObjectType                mRenderObjectType;
    int                             mZPos;
    union
    {
        GameObject* mGameObject;
        Plant* mPlant;
        Zombie* mZombie;
        Coin* mCoin;
        Projectile* mProjectile;
        CursorPreview* mCursorPreview;
        TodParticleSystem* mParticleSytem;
        Reanimation* mReanimation;
        GridItem* mGridItem;
        LawnMower* mMower;
        Bush* mBush;
        BossPart                    mBossPart;
        int                         mBoardGridY;
    };
};

bool RenderItemSortFunc(const RenderItem& theItem1, const RenderItem& theItem2);

struct ZombiePicker
{
    int                             mZombieCount;
    int                             mZombiePoints;
    int                             mZombieTypeCount[NUM_ZOMBIE_TYPES];
    int                             mAllWavesZombieTypeCount[NUM_ZOMBIE_TYPES];
};

void ZombiePickerInitForWave(ZombiePicker* theZombiePicker);
void ZombiePickerInit(ZombiePicker* theZombiePicker);

struct PlantsOnLawn
{
    Plant* mUnderPlant;
    Plant* mTopUnderPlant;
    Plant* mPumpkinPlant;
    Plant* mFlyingPlant;
    Plant* mNormalPlant;
};

struct BungeeDropGrid
{
    TodWeightedGridArray            mGridArray[MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y];
    int                             mGridArrayCount;
};

class Board : public Widget, public ButtonListener
{
public:
    LawnApp* mApp;
    DataArray<Zombie>               mZombies;
    DataArray<Plant>                mPlants;
    DataArray<Projectile>           mProjectiles;
    DataArray<Coin>                 mCoins;
    DataArray<LawnMower>            mLawnMowers;
    int                             mSpecialShovelCooldown;
    DataArray<GridItem>             mGridItems;
    DataArray<Bush>                 mBushes;
    int                             mEclipseCounter;
    CursorObject*                   mCursorObject;
    CursorPreview*                  mCursorPreview;
    MessageWidget*                  mAdvice;
    SeedBank*                       mSeedBank;
    int                             mBeehivePlantedCount;
    GameButton*                     mMenuButton;
    GameButton*                     mStoreButton;
    GameButton*                     mFastButton;
    GameButton*                     mNuclearButton;
    GameButton*                     mSlowButton;       
    bool                            mNuclearRainActive;
    int                             mFertilizeDelayCounter;   
    int                             mNuclearRainCounter;
    Reanimation*                    mActiveFertilizerReanim; 
    Plant*                          mFertilizeTargetPlant;  
    bool                            mNuclearButtonUsed;
    int                             mFertilizerCooldown; 
    bool                            mIgnoreMouseUp;
    int                             mTotalSunCollected;
    ToolTipWidget*                  mToolTip;
    Font*                           mDebugFont;
    int                             mGloveDelayCounter;    
    SeedType                        mGlovePlantType;        
    int                             mGlovePlantHealth;     
    CutScene* mCutScene;
    Challenge* mChallenge;
    float                           mRoofTransitionAlpha;
    float                           mGlobalTimeAlpha;
    bool                            mStartsAsDay;
    bool                            mCurrentlyNight;
    bool                            mPaused;
    int mFertilizerMaxCooldown; 
    int mPlantFoodCount;
    GridSquareType                  mGridSquareType[MAX_GRID_SIZE_X][MAX_GRID_SIZE_Y];
    int                             mGridCelLook[MAX_GRID_SIZE_X][MAX_GRID_SIZE_Y];
    int                             mGridCelOffset[MAX_GRID_SIZE_X][MAX_GRID_SIZE_Y][2];
    int                             mGridCelFog[MAX_GRID_SIZE_X][MAX_GRID_SIZE_Y + 1];
    Bush* mBushList[MAX_GRID_SIZE_Y];

    bool                            mEnableGraveStones;
    int                             mSpecialGraveStoneX;
    int                             mSpecialGraveStoneY;
    float                           mFogOffset;
    int                             mFogBlownCountDown;

    PlantRowType                    mPlantRow[MAX_GRID_SIZE_Y];
    int                             mWaveRowGotLawnMowered[MAX_GRID_SIZE_Y];
    int                             mBonusLawnMowersRemaining;
    int                             mIceMinX[MAX_GRID_SIZE_Y];
    int                             mIceTimer[MAX_GRID_SIZE_Y];
    ParticleSystemID                mIceParticleID[MAX_GRID_SIZE_Y];
    TodSmoothArray                  mRowPickingArray[MAX_GRID_SIZE_Y];

    ZombieType                      mZombiesInWave[MAX_ZOMBIE_WAVES][MAX_ZOMBIES_IN_WAVE];
    bool                            mZombieAllowed[100];
    int                             mSunCountDown;
    int                             mNumSunsFallen;
    int                             mShakeCounter;
    int                             mShakeAmountX;
    int                             mShakeAmountY;

    BackgroundType                  mBackground;
    int                             mLevel;
    int                             mSodPosition;
    int                             mRoofPoleOffset;
    int                             mRoofTreeOffset;
    int                             mPrevMouseX;
    int                             mPrevMouseY;
    int                             mSunMoney;
    int                             mNumWaves;
    int                             mMainCounter;
    int                             mEffectCounter;
    int                             mDrawCount;
    int                             mRiseFromGraveCounter;
    int                             mOutOfMoneyCounter;
    int mNuclearRainStartCounter; // ·  »⁄ „ Ï »œ√ «·„ÿ—
    int                             mCurrentWave;
    int                             mTotalSpawnedWaves;
    int                             mGloveCooldown;
    TutorialState                   mTutorialState;
    ParticleSystemID                mTutorialParticleID;
    int                             mTutorialTimer;
    int                             mLastBungeeWave;
    int                             mZombieHealthToNextWave;
    int                             mZombieHealthWaveStart;
    int                             mZombieCountDown;
    int                             mZombieCountDownStart;
    int                             mHugeWaveCountDown;
    bool                            mHelpDisplayed[NUM_ADVICE_TYPES];
    AdviceType                      mHelpIndex;

    bool                            mFinalBossKilled;
    bool                            mShowShovel;
    int                             mCoinBankFadeCount;
    DebugTextMode                   mDebugTextMode;
    bool                            mLevelComplete;
    int                             mBoardFadeOutCounter;
    int                             mNextSurvivalStageCounter;
    int                             mScoreNextMowerCounter;
    bool                            mLevelAwardSpawned;
    int                             mProgressMeterWidth;
    int                             mFlagRaiseCounter;
    int                             mIceTrapCounter;
    int                             mBoardRandSeed;

    ParticleSystemID                mPoolSparklyParticleID;
    ReanimationID                   mFwooshID[MAX_GRID_SIZE_Y][12];
    int                             mFwooshCountDown;
    int                             mTimeStopCounter;
    bool                            mDroppedFirstCoin;
    int                             mFinalWaveSoundCounter;
    int                             mCobCannonCursorDelayCounter;
    int                             mCobCannonMouseX;
    int                             mCobCannonMouseY;

    bool                            mKilledYeti;
    bool                            mMustacheMode;
    bool                            mSuperMowerMode;
    bool                            mFutureMode;
    bool                            mPinataMode;
    bool                            mDanceMode;
    bool                            mDaisyMode;
    bool                            mSukhbirMode;

    BoardResult                     mPrevBoardResult;
    int                             mTriggeredLawnMowers;
    int                             mPlayTimeActiveLevel;
    int                             mPlayTimeInactiveLevel;
    int                             mMaxSunPlants;
    DWORD                           mStartDrawTime;
    DWORD                           mIntervalDrawTime;
    int                             mIntervalDrawCountStart;
    float                           mMinFPS;
    int                             mPreloadTime;
    int                             mGameID;
    int                             mGravesCleared;
    int                             mPlantsEaten;
    int                             mPlantsShoveled;
    int                             mCoinsCollected;
    int                             mDiamondsCollected;
    int                             mPottedPlantsCollected;
    int                             mChocolateCollected;

    bool                            mPeashootersUsed;
    bool                            mCatapultsUsed;
    bool                            mMushroomsUsed;
    bool                            mMushroomsNCoffeeUsed;
    bool                            mUsedNonMushrooms;
    bool                            mCoinFaded;
    int                             mAchievementCoinCount;
    int                             mGargantuarsKilled;
    int                             mCoinBankX;
    int                             mCoinBankY;

public:
    Board(LawnApp* theApp);
    virtual ~Board();

    void                            DisposeBoard();
    int                             CountSunBeingCollected();
    void                            DrawGameObjects(Graphics* g);
    void                            ClearCursor();
    bool                            AreEnemyZombiesOnScreen();
    LawnMower* FindLawnMowerInRow(int theRow);
    inline bool                     SyncState(DataSync& theDataSync) { return true; }
    void                            SaveGame(const std::string& theFileName);
    bool                            LoadGame(const std::string& theFileName);
    void                            InitLevel();
    void                            DisplayAdvice(const SexyString& theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex);
    void                            StartLevel();
    Plant* AddPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    Projectile* AddProjectile(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType);
    Coin* AddCoin(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion);
    void                            RefreshSeedPacketFromCursor();
    ZombieType                      PickGraveRisingZombieType(int theZombiePoints);
    ZombieType                      PickZombieType(int theZombiePoints, int theWaveIndex, ZombiePicker* theZombiePicker);
    int                             PickRowForNewZombie(ZombieType theZombieType);
    Zombie* AddZombie(ZombieType theZombieType, int theFromWave, bool skipBushAnimation = false);
    void                            SpawnZombieWave();
    GridItem* GetPVZ2GraveAt(int theGridX, int theGridY);
    void                            RemoveAllZombies();
    void                            RemoveCutsceneZombies();
    void                            SpawnZombiesFromGraves();
    PlantingReason                  CanPlantAt(int theGridX, int theGridY, SeedType theSeedType);
    GridItem* AddSpikeBoostGridItem(int theGridX, int theGridY, bool isRock);
    virtual void                    MouseMove(int x, int y) override;
    virtual void                    MouseDrag(int x, int y) override;
    virtual void                    MouseDown(int x, int y, int theClickCount) override;
    virtual void                    MouseUp(int x, int y, int theClickCount) override;
    virtual void                    KeyChar(SexyChar theChar) override;
    virtual void                    KeyDown(KeyCode theKey) override;
    virtual void                    KeyUp(KeyCode theKey) override { ; }
    virtual void                    Update() override;
    virtual void                    Draw(Graphics* g) override;

    void                            UpdateLayers();
    void                            DrawBackdrop(Graphics* g);
    virtual void                    ButtonMouseEnter(int theId) override { ; }
    virtual void                    ButtonMouseLeave(int theId) override { ; }
    virtual void                    ButtonPress(int theId) override { ; }

    void                            AddSunMoney(int theAmount);
    bool                            TakeSunMoney(int theAmount);
    bool                            CanTakeSunMoney(int theAmount);
    void                            Pause(bool thePause);
    inline bool                     MakeEasyZombieType() { return false; }
    void                            TryToSaveGame();
    bool                            NeedSaveGame();
    bool                            RowCanHaveZombies(int theRow);
    void                            ProcessDeleteQueue();
    bool                            ChooseSeedsOnCurrentLevel();
    int                             GetNumSeedsInBank();
    void                            MovePlant(Plant* thePlant, int theGridX, int theGridY);
    bool                            StageIsNight();
    bool                            StageHasPool();
    bool                            StageHas6Rows();
    bool                            StageHasFog();
    bool                            StageHasGraveStones();
    bool                            StageHasBushes();
    void                            AddBushes();
    void                            AnimateBush(int mRow);
    int                             PixelToGridX(int theX, int theY);
    int                             PixelToGridY(int theX, int theY);
    int                             GridToPixelX(int theGridX, int theGridY);
    int                             GridToPixelY(int theGridX, int theGridY);
    int                             PixelToGridXKeepOnBoard(int theX, int theY);
    int                             PixelToGridYKeepOnBoard(int theX, int theY);
    void                            UpdateGameObjects();
    bool                            MouseHitTest(int x, int y, HitResult* theHitResult);
    void                            MouseDownWithPlant(int x, int y, int theClickCount);
    void                            MouseDownWithTool(int x, int y, int theClickCount, CursorType theCursorType);
    inline void                     MouseDownNormal(int x, int y, int theClickCount) { ; }
    bool                            CanInteractWithBoardButtons();
    void                            DrawProgressMeter(Graphics* g);
    void                            UpdateToolTip();
    Plant* GetTopPlantAt(int theGridX, int theGridY, PlantPriority thePriority);
    bool                            IsStackablePlant(SeedType theSeedType);
    void                            UpdateStackedPlantPositions(int theGridX, int theGridY, SeedType theSeedType);
    void                            GetPlantsOnLawn(int theGridX, int theGridY, PlantsOnLawn* thePlantOnLawn);
    int                             CountSunFlowers();
    int                             GetSeedPacketPositionX(int theIndex);
    void                            AddGraveStones(int theGridX, int theCount, MTRand& theLevelRNG);
    int                             GetGraveStoneCount();
    void                            ZombiesWon(Zombie* theZombie = nullptr);
    void                            DrawLevel(Graphics* g);
    void                            DrawShovel(Graphics* g);
    void                            UpdateZombieSpawning();
    void                            UpdateSunSpawning();
    void                            ClearAdvice(AdviceType theHelpIndex);
    bool                            RowCanHaveZombieType(int theRow, ZombieType theZombieType);
    int                             NumberZombiesInWave(int theWaveIndex);
    int                             TotalZombiesHealthInWave(int theWaveIndex);
    void                            DrawDebugText(Graphics* g);
    void                            DrawUICoinBank(Graphics* g);
    void                            ShowCoinBank(int theDuration = 1000);
    void                            FadeOutLevel();
    void                            DrawFadeOut(Graphics* g);
    void                            DrawIce(Graphics* g, int theGridY);
    bool                            IsIceAt(int theGridX, int theGridY);

    ZombieID                        ZombieGetID(Zombie* theZombie);
    Zombie* ZombieGet(ZombieID theZombieID);
    Zombie* ZombieTryToGet(ZombieID theZombieID);
    void                            DrawDebugObjectRects(Graphics* g);
    void                            UpdateIce();
    int                             GetIceZPos(int theRow);
    bool                            CanAddBobSled();
    void                            ShakeBoard(int theShakeAmountX, int theShakeAmountY);
    int                             CountUntriggerLawnMowers();

    bool                            IterateZombies(Zombie*& theZombie);
    bool                            IteratePlants(Plant*& thePlant);
    bool                            IterateProjectiles(Projectile*& theProjectile);
    bool                            IterateCoins(Coin*& theCoin);
    bool                            IterateLawnMowers(LawnMower*& theLawnMower);
    bool                            IterateParticles(TodParticleSystem*& theParticle);
    bool                            IterateReanimations(Reanimation*& theReanimation);
    bool                            IterateGridItems(GridItem*& theGridItem);
    bool                            IterateBushes(Bush*& theBush);

    Zombie* AddZombieInRow(ZombieType theZombieType, int theRow, int theFromWave, bool skipBushAnimation = false);
    bool                            IsPoolSquare(int theGridX, int theGridY);
    void                            PickZombieWaves();
    void                            StopAllZombieSounds();
    bool                            HasLevelAwardDropped();
    void                            UpdateProgressMeter();
    void                            DrawUIBottom(Graphics* g);
    void                            DrawUITop(Graphics* g);
    void                            UpdateNuclearRain();
    void                            DrawNuclearRain(Graphics* g);
    void                            KillAllPlantsInRadius(int theX, int theY, int theRadius);
    Plant* GetPumpkinAt(int theGridX, int theGridY);
    Plant* GetFlowerPotAt(int theGridX, int theGridY);
    bool                            LawnHasNocturnal();
    static bool                     CanZombieSpawnOnLevel(ZombieType theZombieType, int theLevel);
    bool                            IsZombieWaveDistributionOk();
    void                            PickBackground();
    void                            InitZombieWaves();
    void                            InitSurvivalStage();
    static int                      MakeRenderOrder(RenderLayer theRenderLayer, int theRow, int theLayerOffset);
    void                            UpdateGame();
    void                            InitZombieWavesForLevel(int theForLevel);
    unsigned int                    SeedNotRecommendedForLevel(SeedType theSeedType);
    void                            DrawTopRightUI(Graphics* g);
    void                            DrawFog(Graphics* g);
    void                            DrawCover(Graphics* g);
    void                            UpdateFog();
    int                             LeftFogColumn();
    static bool                     IsZombieTypePoolOnly(ZombieType theZombieType);
    void                            DropLootPiece(int thePosX, int thePosY, int theDropFactor);
    void                            UpdateLevelEndSequence();
    LawnMower* GetBottomLawnMower();
    bool                            CanDropLoot();
    ZombieType                      GetIntroducedZombieType();
    void                            PickSpecialGraveStone();
    float                           GetPosYBasedOnRow(float thePosX, int theRow);
    void                            NextWaveComing();
    bool                            BungeeIsTargetingCell(int theGridX, int theGridY);
    int                             PlantingPixelToGridX(int theX, int theY, SeedType theSeedType);
    int                             PlantingPixelToGridY(int theX, int theY, SeedType theSeedType);
    Plant* FindUmbrellaPlant(int theGridX, int theGridY);
    void                            SetTutorialState(TutorialState theTutorialState);
    void                            DoFwoosh(int theRow);
    void                            UpdateFwoosh();
    Plant* SpecialPlantHitTest(int x, int y);
    void                            UpdateMousePosition();
    Plant* ToolHitTestHelper(HitResult* theHitResult);
    Plant* ToolHitTest(int theX, int theY);
    bool                            CanAddGraveStoneAt(int theGridX, int theGridY);
    void                            UpdateGridItems();
    GridItem* AddAGraveStone(int theGridX, int theGridY);
    int                             GetSurvivalFlagsCompleted();
    bool                            HasProgressMeter();
    void                            UpdateCursor();
    void                            UpdateTutorial();
    SeedType                        GetSeedTypeInCursor();
    int                             CountPlantByType(SeedType theSeedType);
    bool                            PlantingRequirementsMet(SeedType theSeedType);
    bool                            HasValidCobCannonSpot();
    bool                            IsValidCobCannonSpot(int theGridX, int theGridY);
    bool                            IsValidCobCannonSpotHelper(int theGridX, int theGridY);
    void                            MouseDownCobcannonFire(int x, int y, int theClickCount);
    void                            KillAllZombiesInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, bool theBurn, int theDamageRangeFlags);
    int                             GetAllZombiesInRadius(int theRow, int theX, int theY, int theRadius, int theRowRange, int theDamageRangeFlags);
    int                             GetSeedBankExtraWidth();
    bool                            IsFlagWave(int theWaveNumber);
    void                            DrawHouseDoorTop(Graphics* g);
    void                            DrawHouseDoorBottom(Graphics* g);
    Zombie* GetBossZombie();
    bool                            HasConveyorBeltSeedBank();
    bool                            StageHasRoof();
    void                            SpawnZombiesFromPool();
    void                            SpawnZombiesFromSky();
    void                            PickUpTool(GameObjectType theObjectType);
    void                            TutorialArrowShow(int theX, int theY);
    void                            TutorialArrowRemove();
    int                             CountCoinsBeingCollected();
    void                            BungeeDropZombie(BungeeDropGrid* theBungeeDropGrid, ZombieType theZombieType);
    void                            SetupBungeeDrop(BungeeDropGrid* theBungeeDropGrid);
    void                            PutZombieInWave(ZombieType theZombieType, int theWaveNumber, ZombiePicker* theZombiePicker);
    void                            PutInMissingZombies(int theWaveNumber, ZombiePicker* theZombiePicker);
    Rect                            GetShovelButtonRect();
    void                            GetZenButtonRect(GameObjectType theObjectType, Rect& theRect);
    Plant* NewPlant(int theGridX, int theGridY, SeedType theSeedType, SeedType theImitaterType = SeedType::SEED_NONE);
    void                            DoPlantingEffects(int theGridX, int theGridY, Plant* thePlant);
    bool                            IsFinalSurvivalStage();
    void                            SurvivalSaveScore();
    int                             CountZombiesOnScreen();
    int                             GetNumWavesPerSurvivalStage();
    int                             GetLevelRandSeed();
    void                            AddBossRenderItem(RenderItem* theRenderList, int& theCurRenderItem, Zombie* theBossZombie);
    GridItem* GetCraterAt(int theGridX, int theGridY);
    GridItem* GetGraveStoneAt(int theGridX, int theGridY);
    GridItem* GetLadderAt(int theGridX, int theGridY);
    GridItem* AddALadder(int theGridX, int theGridY);
    GridItem* AddACrater(int theGridX, int theGridY);
    GridItem* AddInfectionGridItem(int theGridX, int theGridY);
    void                            InitLawnMowers();
    bool                            IsPlantInCursor();
    void                            HighlightPlantsForMouse(int theMouseX, int theMouseY);
    void                            ClearFogAroundPlant(Plant* thePlant, int theSize);
    void                            RemoveParticleByType(ParticleEffect theEffectType);
    GridItem* GetScaryPotAt(int theGridX, int theGridY);
    void                            PuzzleSaveStreak();
    void                            ClearAdviceImmediately();
    bool                            IsFinalScaryPotterStage();
    void                            DisplayAdviceAgain(const SexyString& theAdvice, MessageStyle theMessageStyle, AdviceType theHelpIndex);
    GridItem* GetSquirrelAt(int theGridX, int theGridY);
    GridItem* GetZenToolAt(int theGridX, int theGridY);
    bool                            IsPlantInGoldWateringCanRange(int theMouseX, int theMouseY, Plant* thePlant);
    bool                            StageHasZombieWalkInFromRight();
    void                            PlaceRake();
    GridItem* GetRake();
    bool                            IsScaryPotterDaveTalking();
    Zombie* GetWinningZombie();
    void                            ResetFPSStats();
    int                             CountEmptyPotsOrLilies(SeedType theSeedType);
    GridItem* GetGridItemAt(GridItemType theGridItemType, int theGridX, int theGridY);
    bool                            ProgressMeterHasFlags();
    bool                            IsLastStandFinalStage();
    int                             GetNumWavesPerFlag();
    int                             GetCurrentPlantCost(SeedType theSeedType, SeedType theImitaterType);
    bool                            PlantUsesAcceleratedPricing(SeedType theSeedType);
    void                            FreezeEffectsForCutscene(bool theFreeze);
    void                            LoadBackgroundImages();
    bool                            CanUseGameObject(GameObjectType theGameObject);
    void                            SetMustacheMode(bool theEnableMustache);
    int                             CountCoinByType(CoinType theCoinType);
    void                            SetSuperMowerMode(bool theEnableSuperMower);
    void                            DrawZenWheelBarrowButton(Graphics* g, int theOffsetY);
    void                            DrawZenButtons(Graphics* g);
    void                            OffsetYForPlanting(int& theY, SeedType theSeedType);
    void                            SetDanceMode(bool theEnableDance);
    void                            SetFutureMode(bool theEnableFuture);
    void                            SetPinataMode(bool theEnablePinata);
    void                            SetDaisyMode(bool theEnableDaisy);
    void                            SetSukhbirMode(bool theEnableSukhbir);
    bool                            MouseHitTestPlant(int x, int y, HitResult* theHitResult);

    Reanimation* CreateRakeReanim(float theRakeX, float theRakeY, int theRenderOrder);
    void                            CompleteEndLevelSequenceForSaving();
    void                            RemoveZombiesForRepick();
    int                             GetGraveStonesCount();
    bool                            IsSurvivalStageWithRepick();
    bool                            IsLastStandStageWithRepick();
    void                            DoTypingCheck(KeyCode theKey);
    int                             CountZombieByType(ZombieType theZombieType);
    static bool                     IsZombieTypeSpawnedOnly(ZombieType theZombieType);
    void                            DrawHealthbar(Graphics* g, Rect rect, Color maxColor, int maxNumber, Color baseColor, int baseNumber, int barWidth, int barHeight, int barOffsetX, int barOffsetY, Color textColor, Font* textFont, int textOffsetY, Color textOutlineColor, int textOutlineOffset, bool drawBarOutline);
};

extern bool gShownMoreSunTutorial;

int                                 GetRectOverlap(const Rect& rect1, const Rect& rect2);
bool                                GetCircleRectOverlap(int theCircleX, int theCircleY, int theRadius, const Rect& theRect);
void                                BoardInitForPlayer();

#endif // __BOARD_H__