#ifndef __GRIDITEM_H__
#define __GRIDITEM_H__

#include "../ConstEnums.h"

#define NUM_MOTION_TRAIL_FRAMES 12

class LawnApp;
class Board;
class Zombie;
class Plant;
namespace Sexy
{
	class Graphics;
};

class MotionTrailFrame
{
public:
	float					mPosX;
	float					mPosY;
	float					mAnimTime;
};

class GridItem
{
public:
	LawnApp*				mApp;											
	Board*					mBoard;											
	GridItemType			mGridItemType;									
	GridItemState			mGridItemState;									
	int						mGridX;				
	int     mPVZ2GraveHealth;      // ’Õ… «·ﬁ»—
	int     mJustGotShotCounter;   // ⁄œ«œ Ê„Ì÷  ·ﬁÌ «·÷——
	int						mGridY;											
	int						mGridItemCounter;								
	int						mRenderOrder;									
	bool					mDead;											
	float					mPosX;											
	float					mPosY;		
	int                     mPVZ2GraveVariant;     
	float					mGoalX;											
	float					mGoalY;											
	ReanimationID			mGridItemReanimID;								
	ParticleSystemID		mGridItemParticleID;							
	ZombieType				mZombieType;									
	SeedType				mSeedType;										
	ScaryPotType			mScaryPotType;									
	bool					mHighlighted;									
	int						mTransparentCounter;							
	int						mSunCount;										
	MotionTrailFrame		mMotionTrailFrames[NUM_MOTION_TRAIL_FRAMES];	
	int						mMotionTrailCount;								

public:
	GridItem();

	void					DrawLadder(Sexy::Graphics* g);
	void					DrawCrater(Sexy::Graphics* g);
	void					DrawGraveStone(Sexy::Graphics* g);
	void					GridItemDie();
	void                    UpdateInfection();
	void SpawnInfectionZombie(); // <-- √÷› Â–Â
	void SpawnPoisonEffect(Plant* aPlant); // <-- √÷› Â–Â
	void					AddGraveStoneParticles();
	void					DrawGridItem(Sexy::Graphics* g);
	void					DrawGridItemOverlay(Sexy::Graphics* g);
	void					OpenPortal();
	void					Update();
	void					ClosePortal();
	void					DrawScaryPot(Sexy::Graphics* g);
	void					UpdateScaryPot();
	void					UpdatePortal();
	void    DrawPVZ2GraveStone(Sexy::Graphics* g); // œ«·… «·—”„
	Rect    GetPVZ2GraveRect();                    // ’‰œÊﬁ «·≈’«»… (Hitbox)
	void    TakeDamage(int theDamage);             // œ«·…  ·ﬁÌ «·÷——
	void					DrawSquirrel(Sexy::Graphics* g);
	void					UpdateRake();
	Zombie*					RakeFindZombie();
	void					DrawIZombieBrain(Sexy::Graphics* g);
	void					UpdateBrain();
	void					DrawStinky(Sexy::Graphics* g);
	/*inline*/ bool			IsOpenPortal();
	void UpdateSpikeBoost();
	void DrawSpikeBoost(Sexy::Graphics* g);
};

#endif
