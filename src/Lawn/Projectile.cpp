#include "Board.h"
#include "Plant.h"
#include "Zombie.h"
#include "Cutscene.h"
#include "Projectile.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "GridItem.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/TodParticle.h"

ProjectileDefinition gProjectileDefinition[] = {  
	{ ProjectileType::PROJECTILE_PEA,           0,  20  },
	{ ProjectileType::PROJECTILE_SNOWPEA,       0,  15 },
	{ ProjectileType::PROJECTILE_CABBAGE,       0,  40  },
	{ ProjectileType::PROJECTILE_MELON,         0,  80  },
	{ ProjectileType::PROJECTILE_PUFF,          0,  15  },
	{ ProjectileType::PROJECTILE_WINTERMELON,   0,  100  },
	{ ProjectileType::PROJECTILE_FIREBALL,      0,  40  },
	{ ProjectileType::PROJECTILE_STAR,          0,  20  },
	{ ProjectileType::PROJECTILE_SPIKE,         0,  40  },
	{ ProjectileType::PROJECTILE_BASKETBALL,    0,  100  },
	{ ProjectileType::PROJECTILE_KERNEL,        0,  15  },
	{ ProjectileType::PROJECTILE_COBBIG,        0,  300 },
	{ ProjectileType::PROJECTILE_BUTTER,        0,  80  },
	{ ProjectileType::PROJECTILE_ZOMBIE_PEA,    0,  20  },
	{ ProjectileType::PROJECTILE_CATTAIL_BOOST,    0, 2000 },
	{ ProjectileType::PROJECTILE_CHERRY,           0,  300 },
	{ ProjectileType::PROJECTILE_PLASMABALL,      0, 80  },
	{ ProjectileType::PROJECTILE_ICE_CABBAGE, 0, 60 }, 
    { ProjectileType::PROJECTILE_ACID_LEMON, 0, 20 } ,
	{ ProjectileType::PROJECTILE_SAKURA, 0,40 },
	{ ProjectileType::PROJECTILE_NINJA, 0, 20 },
	{ ProjectileType::PROJECTILE_PUFF2,    0,  15 },
	{ ProjectileType::PROJECTILE_MAGNIFYING_GRASS,    0,  300 },
	{ ProjectileType::PROJECTILE_PEPPER,    0,  60 },
	{ ProjectileType::PROJECTILE_SUN,           0,  50 },
	{ ProjectileType::PROJECTILE_GOO,           0,  20  },
	{ ProjectileType::PROJECTILE_ROCK,           0,  80  },
	{ ProjectileType::PROJECTILE_DROPL,           0,  20  }
};

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
	AttachmentDie(mAttachmentID);
}

void Projectile::ProjectileInitialize(int theX, int theY, int theRenderOrder, int theRow, ProjectileType theProjectileType)
{
	int aGridX = mBoard->PixelToGridXKeepOnBoard(theX, theY);
	mProjectileType = theProjectileType;
	mPosX = theX;
	mPosY = theY;
	mOriginX = theX;
	mOriginY = theY;
	mPosZ = 0.0f;
	mVelX = 0.0f;
	mVelY = 0.0f;
	mVelZ = 0.0f;
	mAccZ = 0.0f;
	mShadowY = mBoard->GridToPixelY(aGridX, theRow) + 67.0f;
	mHitTorchwoodGridX = -1;
	mMotionType = ProjectileMotion::MOTION_STRAIGHT;
	mFrame = 0;
	mNumFrames = 1;
	mRow = theRow;
	mCobTargetX = 0.0f;
	mDamageRangeFlags = 0;
	mDead = false;
	mIsSpore = false;
	mNumHitZombies = 0;
	mIsReturning = false;
	mDoesNotReturn = false;
	for (int i = 0; i < 20; i++) mHitZombies[i] = ZombieID::ZOMBIEID_NULL;
	mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
	mCobTargetRow = 0;
	mTargetZombieID = ZombieID::ZOMBIEID_NULL;
	mOnHighGround = mBoard->mGridSquareType[aGridX][theRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND;
	if (mBoard->StageHasRoof())
	{
		mShadowY -= 12.0f;
	}
	mRenderOrder = theRenderOrder;
	mRotation = 0.0f;
	mRotationSpeed = 0.0f;
	mWidth = 40;
	mHeight = 40;
	mProjectileAge = 0;
	mClickBackoffCounter = 0;
	mAnimTicksPerFrame = 0;

	if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_BUTTER ||
		mProjectileType == ProjectileType::PROJECTILE_ICE_CABBAGE)
	{
		mRotation = -7 * PI / 25;
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_NINJA)
	{
		mRotationSpeed = RandRangeFloat(-0.25f, -0.15f); 
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ACID_LEMON)
	{
		theY += 10;
		mRotation = 0.0f;
		mRotationSpeed = 0.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_PEPPER || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mRotation = -2 * PI / 5; 
		mRotationSpeed = RandRangeFloat(-0.08f, -0.02f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mRotation = 0.0f;
		mRotationSpeed = RandRangeFloat(-0.2f, -0.08f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 8.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_SNOWPEA_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 8.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		TOD_ASSERT();
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		mWidth = IMAGE_REANIM_COBCANNON_COB->GetWidth();
		mHeight = IMAGE_REANIM_COBCANNON_COB->GetHeight();
		mRotation = PI / 2;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF2)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 13.0f, mPosY + 13.0f, 400000, ParticleEffect::PARTICLE_PUFFSHROOM_TRAIL);
		AttachParticle(mAttachmentID, aParticle, 13.0f, 13.0f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		mRotation = RandRangeFloat(0.0f, 2 * PI);
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		mShadowY += 15.0f;
		mRotationSpeed = RandRangeFloat(0.05f, 0.1f);
		if (Rand(2) == 0)
		{
			mRotationSpeed = -mRotationSpeed;
		}
	}

	mAnimCounter = 0;
	mX = (int)mPosX;
	mY = (int)mPosY;
}

Plant* Projectile::FindCollisionTargetPlant()
{
	Rect aProjectileRect = GetProjectileRect();

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mRow != mRow)
			continue;

		if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_NINJA)
		{
			if (aPlant->mSeedType == SeedType::SEED_PUFFSHROOM ||
				aPlant->mSeedType == SeedType::SEED_SUNSHROOM ||
				aPlant->mSeedType == SeedType::SEED_POTATOMINE ||
				aPlant->mSeedType == SeedType::SEED_SPIKEWEED ||
				aPlant->mSeedType == SeedType::SEED_SPIKEROCK ||

				aPlant->mSeedType == SeedType::SEED_LILYPAD)  
				continue;
		}

		Rect aPlantRect = aPlant->GetPlantRect();
		if (GetRectOverlap(aProjectileRect, aPlantRect) > 8)
		{
			if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_NINJA)
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_EATING_ORDER);
			}
			else
			{
				return mBoard->GetTopPlantAt(aPlant->mPlantCol, aPlant->mRow, PlantPriority::TOPPLANT_CATAPULT_ORDER);
			}
		}
	}

	return nullptr;
}

bool Projectile::PeaAboutToHitTorchwood()
{
	if (mMotionType != ProjectileMotion::MOTION_STRAIGHT)
		return false;

	if (mProjectileType != ProjectileType::PROJECTILE_PEA && mProjectileType != ProjectileType::PROJECTILE_SNOWPEA)
		return false;

	Plant* aPlant = nullptr;
	while (mBoard->IteratePlants(aPlant))
	{
		if (aPlant->mSeedType == SeedType::SEED_TORCHWOOD && aPlant->mRow == mRow && !aPlant->NotOnGround() && mHitTorchwoodGridX != aPlant->mPlantCol)
		{
			Rect aPlantAttackRect = aPlant->GetPlantAttackRect(PlantWeapon::WEAPON_PRIMARY);
			Rect aProjectileRect = GetProjectileRect();
			aProjectileRect.mX += 40;

			if (GetRectOverlap(aPlantAttackRect, aProjectileRect) > 10)
			{
				return true;
			}
		}
	}

	return false;
}

Zombie* Projectile::FindCollisionTarget()
{
	if (PeaAboutToHitTorchwood())
		return nullptr;
	Rect aProjectileRect = GetProjectileRect();
	Zombie* aBestZombie = nullptr;
	int aMinX = 0;
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if ((aZombie->mZombieType == ZombieType::ZOMBIE_BOSS || aZombie->mRow == mRow) && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			if (aZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL && mPosZ >= 45.0f)
			{
				continue;
			}
			if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_SAKURA) && mProjectileAge < 25 && mVelX >= 0.0f && aZombie->mZombieType == ZombieType::ZOMBIE_DIGGER)
			{
				continue;
			}
			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) > 0)
			{
				if (aProjectileRect.mY + aProjectileRect.mHeight >= aZombieRect.mY && aProjectileRect.mY <= aZombieRect.mY + aZombieRect.mHeight)
				{
					if (aBestZombie == nullptr || aZombie->mX < aMinX)
					{
						aBestZombie = aZombie;
						aMinX = aZombie->mX;
					}
				}
			}
		}
	}
	return aBestZombie;
}

void Projectile::CheckForCollision()
{
	if (mMotionType == ProjectileMotion::MOTION_PUFF && mProjectileAge >= 75)
	{
		Die();
		return;
	}

	if (mPosX > WIDE_BOARD_WIDTH || mPosX + mWidth < 0.0f + BOARD_ADDITIONAL_WIDTH)
	{
		Die();
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aProjectileRect = GetProjectileRect();
			Rect aZombieRect = aZombie->GetZombieRect();
			if (GetRectOverlap(aProjectileRect, aZombieRect) >= 0 && mPosY > aZombieRect.mY && mPosY < aZombieRect.mY + aZombieRect.mHeight)
			{
				DoImpact(aZombie);
			}
		}
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_SAKURA) && (mPosY > 600.0f + BOARD_OFFSET_Y || mPosY < 0.0f + BOARD_OFFSET_Y))
	{
		Die();
		return;
	}

	if ((mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_GOO || mProjectileType == ProjectileType::PROJECTILE_DROPL || mProjectileType == ProjectileType::PROJECTILE_SUN || mProjectileType == ProjectileType::PROJECTILE_ROCK|| mProjectileType == ProjectileType::PROJECTILE_STAR) && mShadowY - mPosY > 90.0f + BOARD_OFFSET_Y)
	{
		return;
	}

	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		return;
	}

	if (mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || mProjectileType == ProjectileType::PROJECTILE_NINJA)
	{
		Plant* aPlant = FindCollisionTargetPlant();
		if (aPlant)
		{
			const ProjectileDefinition& aProjectileDef = GetProjectileDef();
			aPlant->mPlantHealth -= aProjectileDef.mDamage;
			aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);
			mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
			mApp->AddTodParticle(mPosX - 3.0f, mPosY + 17.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PEA_SPLAT);
			Die();
		}
		return;
	}
	GridItem* aGrave = FindCollisionTargetPVZ2Grave();
	if (aGrave)
	{
		if (mOnHighGround && CantHitHighGround()) return;
		DoGraveImpact(aGrave);
		return;
	}
	else
	{
		Zombie* aZombie = FindCollisionTarget();
		if (aZombie)
		{
			if (aZombie->mOnHighGround && CantHitHighGround()) return;
			DoImpact(aZombie);
		}
	}
}

void Projectile::MeteorExplode()
{
	if (mDead)
		return;

	if (mProjectileType != ProjectileType::PROJECTILE_COBBIG || !mIsMeteorBoost)
		return;
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying() && !aZombie->mMindControlled)
		{
			aZombie->TakeDamage(3000, 0U); 
		}
	}

	float centerX = mPosX + 115.0f;
	float centerY = mPosY + 115.0f;

	int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, mRow, 2);
	TodParticleSystem* aParticle = mApp->AddTodParticle(centerX, centerY, aRenderOrder, ParticleEffect::PARTICLE_DOOM);

	if (aParticle)
	{
		aParticle->OverrideColor(nullptr, Color(255, 100, 0, 255));
		aParticle->OverrideScale(nullptr, 2.5f);
	}

	mApp->PlaySample(Sexy::SOUND_DOOMSHROOM);
	mBoard->ShakeBoard(30, -30);

	int aCenterRow = mBoard->PixelToGridYKeepOnBoard(centerX, centerY);
	const int numProjectilesPerRing = 16;
	float currentRingSpeed = 2.0f;
	for (int ring = 1; ring <= 20; ring++)
	{
		for (int i = 0; i < numProjectilesPerRing; i++)
		{
			float angle = (i * 2.0f * PI) / numProjectilesPerRing;
			Projectile* aRingProj = mBoard->AddProjectile(
				centerX,
				centerY,
				mRenderOrder - 1,
				aCenterRow,
				ProjectileType::PROJECTILE_STAR);

			if (!aRingProj)
				continue;

			aRingProj->mMotionType = ProjectileMotion::MOTION_STAR;
			aRingProj->mVelX = cos(angle) * currentRingSpeed;
			aRingProj->mVelY = sin(angle) * currentRingSpeed;
			aRingProj->mDamageRangeFlags = 20U;
		}
		currentRingSpeed *= 1.5f;
	}
}
bool Projectile::CantHitHighGround()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS || mMotionType == ProjectileMotion::MOTION_HOMING)
		return false;

	return (
		mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_GOO ||
		mProjectileType == ProjectileType::PROJECTILE_DROPL ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_STAR ||
		mProjectileType == ProjectileType::PROJECTILE_SAKURA ||
		mProjectileType == ProjectileType::PROJECTILE_PUFF ||
		mProjectileType == ProjectileType::PROJECTILE_PUFF2 ||
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL
		) && !mOnHighGround;
}

void Projectile::CheckForHighGround()
{
	float aShadowDelta = mShadowY - mPosY;

	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_GOO ||
		mProjectileType == ProjectileType::PROJECTILE_DROPL ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL ||
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_SPIKE ||
		mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		if (aShadowDelta < 28.0f)
		{
			DoImpact(nullptr);
			return;
		}
	}

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF && aShadowDelta < 0.0f)
	{
		DoImpact(nullptr);
		return;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_PUFF2 && aShadowDelta < 0.0f)
	{
		DoImpact(nullptr);
		return;
	}
	if ((mProjectileType == ProjectileType::PROJECTILE_STAR || mProjectileType == ProjectileType::PROJECTILE_SAKURA) && aShadowDelta < 23.0f)
	{
		DoImpact(nullptr);
		return;
	}

	if (CantHitHighGround())
	{
		int aGridX = mBoard->PixelToGridXKeepOnBoard(mPosX + 30, mPosY);
		if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
		{
			DoImpact(nullptr);
		}
	}
}

bool Projectile::IsSplashDamage(Zombie* theZombie)
{
	if (mProjectileType && theZombie && theZombie->IsFireResistant())
		return false;

	return 
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_PEPPER ||
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_PLASMABALL ||
		mProjectileType == ProjectileType::PROJECTILE_FIREBALL;
}

unsigned int Projectile::GetDamageFlags(Zombie* theZombie)
{
	unsigned int aDamageFlags = 0U;

	if (IsSplashDamage(theZombie))
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_HITS_SHIELD_AND_BODY, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_LOBBED || mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR && mVelX < 0.0f)
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_BYPASSES_SHIELD, true);
	}
	if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON ||
		mProjectileType == ProjectileType::PROJECTILE_ICE_CABBAGE) 
	{
		SetBit(aDamageFlags, (int)DamageFlags::DAMAGE_FREEZE, true);
	}

	return aDamageFlags;
}

bool Projectile::IsZombieHitBySplash(Zombie* theZombie)
{
	Rect aProjectileRect = GetProjectileRect();
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL) 
	{
		aProjectileRect.mWidth = 100;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		aProjectileRect.mWidth = 100;
	}
	int aRowDeviation = theZombie->mRow - mRow;
	Rect aZombieRect = theZombie->GetZombieRect();
	if (theZombie->IsFireResistant() && mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		return false;
	}
	if (theZombie->IsFireResistant() && mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		return false;
	}
	if (theZombie->mZombieType == ZombieType::ZOMBIE_BOSS)
	{
		aRowDeviation = 0;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		if (aRowDeviation != 0)
		{
			return false;
		}
	}
	if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		if (aRowDeviation != 0)
		{
			return false;
		}
	}
	else if (aRowDeviation > 1 || aRowDeviation < -1)
	{
		return false;
	}

	return theZombie->EffectedByDamage((unsigned int)mDamageRangeFlags) && GetRectOverlap(aProjectileRect, aZombieRect) >= 0;
}

void Projectile::DoSplashDamage(Zombie* theZombie)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	int aZombiesGetSplashed = 0;
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (aZombie != theZombie && IsZombieHitBySplash(aZombie))
		{
			aZombiesGetSplashed++;
		}
	}

	int aOriginalDamage = aProjectileDef.mDamage;
	int aSplashDamage = aProjectileDef.mDamage / 3;
	int aMaxSplashDamageAmount = aSplashDamage * 7;
	if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		aMaxSplashDamageAmount = aOriginalDamage;
	}
	int aSplashDamageAmount = aSplashDamage * aZombiesGetSplashed;
	if (aSplashDamageAmount > aMaxSplashDamageAmount)
	{
		//aSplashDamage *= aMaxSplashDamageAmount / aSplashDamage;
		aSplashDamage = aOriginalDamage * aMaxSplashDamageAmount / (aSplashDamageAmount * 3);
		aSplashDamage = max(aSplashDamage, 1);
	}

	aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (IsZombieHitBySplash(aZombie))
		{
			unsigned int aDamageFlags = GetDamageFlags(aZombie);
			if (aZombie == theZombie)
			{
				aZombie->TakeDamage(aOriginalDamage, aDamageFlags);
			}
			else
			{
				aZombie->TakeDamage(aSplashDamage, aDamageFlags);
			}
		}
	}
}

void Projectile::UpdateLobMotion()
{
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG && mPosZ < -700.0f)
	{
		mVelZ = 8.0f;
		mRow = mCobTargetRow;
		mPosX = mCobTargetX;
		int aCobTargetCol = mBoard->PixelToGridXKeepOnBoard(mCobTargetX, 0);
		mPosY = mBoard->GridToPixelY(aCobTargetCol, mCobTargetRow);
		mShadowY = mPosY + 67.0f;
		mRotation = -PI / 2;
	}

	mVelZ += mAccZ;
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		mVelZ += mAccZ;
	}
	mPosX += mVelX;
	mPosY += mVelY;
	mPosZ += mVelZ;

	bool isRising = mVelZ < 0.0f;
	if (isRising && (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_COBBIG))
	{
		return;
	}
	if (mProjectileAge > 20)
	{
		if (isRising)
		{
			return;
		}

		float aMinCollisionZ = 0.0f;
		if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
		{
			aMinCollisionZ = -32.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
		{
			aMinCollisionZ = 60.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_PEPPER|| mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
		{
			aMinCollisionZ = -35.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE || mProjectileType == ProjectileType::PROJECTILE_KERNEL)
		{
			aMinCollisionZ = -30.0f;
		}
		else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
		{
			aMinCollisionZ = -60.0f;
		}
		if (mBoard->mPlantRow[mRow] == PlantRowType::PLANTROW_POOL)
		{
			aMinCollisionZ += 40.0f;
		}

		if (mPosZ <= aMinCollisionZ)
		{
			return;
		}
	}

	Plant* aPlant = nullptr;
	Zombie* aZombie = nullptr;
	if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA)
	{
		aPlant = FindCollisionTargetPlant();
	}
	else
	{
		aZombie = FindCollisionTarget();
	}

	float aGroundZ = 80.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aGroundZ = -40.0f;
	}
	bool hitGround = mPosZ > aGroundZ;
	if (aZombie == nullptr && aPlant == nullptr && !hitGround)
	{
		return;
	}

	if (aPlant)
	{
		Plant* aUmbrellaPlant = mBoard->FindUmbrellaPlant(aPlant->mPlantCol, aPlant->mRow);
		if (aUmbrellaPlant)
		{
			if (aUmbrellaPlant->mState == PlantState::STATE_UMBRELLA_REFLECTING)
			{
				mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
				int aRenderPosition = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, 0, 1);
				mApp->AddTodParticle(mPosX + 20.0f, mPosY + 20.0f, aRenderPosition, ParticleEffect::PARTICLE_UMBRELLA_REFLECT);
				Die();
			}
			else if (aUmbrellaPlant->mState != PlantState::STATE_UMBRELLA_TRIGGERED)
			{
				mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
				aUmbrellaPlant->DoSpecial();
			}
		}
		else
		{
			aPlant->mPlantHealth -= GetProjectileDef().mDamage;
			aPlant->mEatenFlashCountdown = max(aPlant->mEatenFlashCountdown, 25);
			mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
			Die();
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		if (mIsMeteorBoost)
		{
			MeteorExplode();
		}
		else
		{
			if (GetGargantuars(mRow, mPosX + 80, mPosY + 40, 115, 1)) {
				mBoard->mGargantuarsKilled++;
				if (mBoard->mGargantuarsKilled >= 2 && !mApp->mPlayingQuickplay)
					mApp->GetAchievement(ACHIEVEMENT_POPCORN_PARTY);
			}
			mBoard->KillAllZombiesInRadius(mRow, mPosX + 80, mPosY + 40, 115, 1, true, mDamageRangeFlags);
		}
		DoImpact(nullptr);
	}
	else
	{
		DoImpact(aZombie);
	}
}

void Projectile::UpdateNormalMotion()
{
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		if (mProjectileType == ProjectileType::PROJECTILE_NINJA)
		{
			mPosX += mVelX; 
		}
		else
		{
			mPosX -= 3.33f;
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_HOMING)
	{
		Zombie* aZombie = mBoard->ZombieTryToGet(mTargetZombieID);
		if (aZombie && aZombie->EffectedByDamage((unsigned int)mDamageRangeFlags))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			SexyVector2 aTargetCenter(aZombie->ZombieTargetLeadX(0.0f), aZombieRect.mY + aZombieRect.mHeight / 2);
			SexyVector2 aProjectileCenter(mPosX + mWidth / 2, mPosY + mHeight / 2);
			SexyVector2 aToTarget = (aTargetCenter - aProjectileCenter).Normalize();
			SexyVector2 aMotion(mVelX, mVelY);

			aMotion += aToTarget * (0.001f * mProjectileAge);
			aMotion = aMotion.Normalize();
			aMotion *= 2.0f;

			mVelX = aMotion.x;
			mVelY = aMotion.y;
			mRotation = -atan2(mVelY, mVelX);
		}

		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
	}
	else if (mMotionType == ProjectileMotion::MOTION_STAR)
	{
		mPosY += mVelY;
		mPosX += mVelX;
		mShadowY += mVelY;

		if (mVelY != 0.0f)
		{
			mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		}
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX += 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		if (mVelZ < 0.0f)
		{
			mVelZ += 0.002f;
			mVelZ = min(mVelZ, 0.0f);
			mPosY += mVelZ;
			mRotation = 0.3f - 0.7f * mVelZ * PI * 0.25f;
		}
		mPosX += 0.4f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		if (mProjectileAge < 60)
		{
			mPosY -= 0.5f;
		}
		mPosX -= 3.33f;
	}
	else if (mMotionType == ProjectileMotion::MOTION_THREEPEATER)
	{
		mPosX += 3.33f;
		mPosY += mVelY;
		mVelY *= 0.97f;
		mShadowY += mVelY;
	}
	else
	{
		mPosX += 3.33f;
	}

	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_HIGH_GRAVITY)
	{
		if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
		{
			mVelZ += 0.004f;
		}
		else
		{
			mVelZ += 0.2f;
		}

		mPosY += mVelZ;
	}

	CheckForCollision();
	CheckForHighGround();
}

void Projectile::UpdateMotion()
{
	if (mAnimTicksPerFrame > 0)
	{
		mAnimCounter = (mAnimCounter + 1) % (mNumFrames * mAnimTicksPerFrame);
		mFrame = mAnimCounter / mAnimTicksPerFrame;
	}
	// ================== ﬂÊœ «·›Ê »Ê· «·ÃœÌœ ==================
	// ================== ﬂÊœ «·›Ê »Ê· «·ÃœÌœ ==================
	if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL && mApp->IsFootballLevel())
	{
		mPosX += mVelX;
		mPosY += mVelY;
		mRow = mBoard->PixelToGridYKeepOnBoard(mPosX, mPosY);
		mShadowY = mPosY + 67.0f;

		mRotation += mRotationSpeed;
		if (mPosX < -40.0f) { mPosX = -40.0f; mVelX = fabs(mVelX); }
		if (mPosX > BOARD_WIDTH + 40.0f) { mPosX = BOARD_WIDTH + 40.0f; mVelX = -fabs(mVelX); }
		if (mPosY < 80.0f) { mPosY = 80.0f; mVelY = fabs(mVelY); }
		if (mPosY > 610.0f) { mPosY = 610.0f; mVelY = -fabs(mVelY); }

		// 3. «·«’ÿœ«„ „⁄ «·‰»« «  («·„Ÿ·…° √Ê «·ÃÊ“ ≈–« ﬂ‰   ” Œœ„Â)
		Plant* aPlant = nullptr;
		while (mBoard->IteratePlants(aPlant))
		{
			if (aPlant->mRow == mRow && !aPlant->mDead && GetCircleRectOverlap(mPosX + 20, mPosY + 20, 20, aPlant->GetPlantRect()))
			{
				// ≈÷«›… ‘—ÿ mVelX < 0 ·÷„«‰ «’ÿœ«„ «·ﬂ—… ›ﬁÿ ⁄‰œ ﬁœÊ„Â« „‰ «·Ì„Ì‰ ··Ì”«—
				if (aPlant->mSeedType == SeedType::SEED_WALLNUT && mVelX < 0)
				{
					aPlant->mPlantHealth -= 200;
					if (aPlant->mPlantHealth <= 0) aPlant->Die();
					mApp->PlayFoley(FoleyType::FOLEY_SPLAT);

					mVelX = 5.0f; // «— œ«œ ‰ÕÊ «·Ì„Ì‰ »ﬁÊ…
					mVelY = (mPosY > aPlant->mY + 40) ? RandRangeFloat(2.0f, 4.0f) : RandRangeFloat(-4.0f, -2.0f);
					mApp->PlayFoley(FoleyType::FOLEY_BASKETBALL);
					mPosX += 20.0f; // œ›⁄ «·ﬂ—… Œ«—Ã «· œ«Œ·
					break;
				}
				else if (aPlant->mSeedType == SeedType::SEED_UMBRELLA && mVelX < 0)
				{
					if (aPlant->mState != PlantState::STATE_UMBRELLA_REFLECTING)
					{
						aPlant->DoSpecial();
					}
					mVelX = 7.5f; // «— œ«œ ﬁÊÌ ‰ÕÊ «·Ì„Ì‰
					mVelY = (mPosY > aPlant->mY + 40) ? RandRangeFloat(2.0f, 4.0f) : RandRangeFloat(-4.0f, -2.0f);
					mApp->PlayFoley(FoleyType::FOLEY_UMBRELLA);
					mPosX += 20.0f; // œ›⁄ «·ﬂ—… Œ«—Ã «· œ«Œ·
					break;
				}
			}
		}

		// 4. «·«’ÿœ«„ „⁄ «·ﬁ»Ê—
		GridItem* aGrave = nullptr;
		while (mBoard->IterateGridItems(aGrave))
		{
			if (aGrave->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE && aGrave->mGridItemCounter >= 0)
			{
				if (GetCircleRectOverlap(mPosX + 20, mPosY + 20, 20, aGrave->GetPVZ2GraveRect()))
				{
					// ≈÷«›… ‘—ÿ mVelX > 0 ·÷„«‰ «’ÿœ«„ «·ﬂ—… »«·ﬁ»— ›ﬁÿ ⁄‰œ ﬁœÊ„Â« „‰ «·Ì”«— ··Ì„Ì‰
					if (mVelX > 0)
					{
						aGrave->TakeDamage(100);

						mVelX = -6.0f; // «— œ«œ ‰ÕÊ «·Ì”«—
						mPosX -= 20.0f; // œ›⁄ «·ﬂ—… Œ«—Ã «· œ«Œ·
						mVelY = (mPosY > aGrave->mPosY + 40) ? RandRangeFloat(2.0f, 4.0f) : RandRangeFloat(-4.0f, -2.0f);
						mApp->PlayFoley(FoleyType::FOLEY_BASKETBALL);
						break;
					}
				}
			}
		}
		if (mVelX > 0)
		{
			Zombie* aZombie = nullptr;
			while (mBoard->IterateZombies(aZombie))
			{
				if (!aZombie->IsDeadOrDying() && GetCircleRectOverlap(mPosX + 20, mPosY + 20, 20, aZombie->GetZombieRect()))
				{
					// ‰”»… 50% ·’œ «·ﬂ—…° Ê≈·« ” „— „‰ Œ·«·Â ‰ÕÊ «·ﬁ»—
					if (Rand(100) < 50)
					{
						mVelX = -7.0f;
						mVelY = (mPosY > aZombie->mPosY + 40) ? RandRangeFloat(2.0f, 4.0f) : RandRangeFloat(-4.0f, -2.0f);
						mPosX -= 20.0f;
						mApp->PlayFoley(FoleyType::FOLEY_BASKETBALL);
					}
					// »„Ã—œ √‰   œ«Œ· «·ﬂ—… „⁄Â ‰Œ—Ã „‰ «·Õ·ﬁ… (”Ê«¡ ’œÂ« √„ ·«)
					break;
				}
			}
		}

		mX = (int)mPosX;
		mY = (int)mPosY;
		return;
	}
	int aOldRow = mRow;
	float aOldY = mBoard->GetPosYBasedOnRow(mPosX, mRow);
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		UpdateLobMotion();
	}
	else
	{
		UpdateNormalMotion();
	}

	float aSlopeHeightChange = mBoard->GetPosYBasedOnRow(mPosX, aOldRow) - aOldY;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aSlopeHeightChange = 0.0f;  
	}
	if (mMotionType == ProjectileMotion::MOTION_FLOAT_OVER)
	{
		mPosY += aSlopeHeightChange;
	}
	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		mPosY += aSlopeHeightChange;
		mPosZ -= aSlopeHeightChange;
	}
	
	mShadowY += aSlopeHeightChange;
	mX = (int)mPosX;
	mY = (int)(mPosY + mPosZ);
}

void Projectile::PlayImpactSound(Zombie* theZombie)
{
	bool aPlayHelmSound = true;
	bool aPlaySplatSound = true;
	if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		mApp->PlayFoley(FoleyType::FOLEY_KERNEL_SPLAT);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		mApp->PlayFoley(FoleyType::FOLEY_BUTTER);
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL && IsSplashDamage(theZombie))
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL && IsSplashDamage(theZombie))
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlayHelmSound = false;
		aPlaySplatSound = false;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		mApp->PlayFoley(FoleyType::FOLEY_MELONIMPACT);
		aPlaySplatSound = false;
	}
	else if ( mProjectileType == ProjectileType::PROJECTILE_PEPPER)
	{
		mApp->PlayFoley(FoleyType::FOLEY_IGNITE);
		aPlaySplatSound = false;
	}
	if (aPlayHelmSound && theZombie)
	{
		if (theZombie->mHelmType == HELMTYPE_PAIL)
		{
			mApp->PlayFoley(FoleyType::FOLEY_SHIELD_HIT);
			aPlaySplatSound = false;
		}
		else if (theZombie->mHelmType == HELMTYPE_TRAFFIC_CONE || theZombie->mHelmType == HELMTYPE_BRICK|| theZombie->mHelmType == HELMTYPE_DIGGER || theZombie->mHelmType == HELMTYPE_FOOTBALL )
		{
			mApp->PlayFoley(FoleyType::FOLEY_PLASTIC_HIT);
		}
	}

	if (aPlaySplatSound)
	{
		mApp->PlayFoley(FoleyType::FOLEY_SPLAT);
	}
}

void Projectile::DoImpact(Zombie* theZombie)
{
	PlayImpactSound(theZombie);

	int hpBefore = theZombie ? theZombie->mBodyHealth : 0;

	if (IsSplashDamage(theZombie))
	{
		if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL && theZombie)
		{
			theZombie->RemoveColdEffects();
		}
		if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL && theZombie)
		{
			theZombie->RemoveColdEffects();
		}
		DoSplashDamage(theZombie);
	}
	else if (theZombie)
	{
		unsigned int aDamageFlags = GetDamageFlags(theZombie);
		theZombie->TakeDamage(GetProjectileDef().mDamage, aDamageFlags);
	}
	bool isDead = theZombie && theZombie->IsDeadOrDying();
	if (mIsSpore && theZombie && isDead)
	{
		int zCol = mBoard->PixelToGridXKeepOnBoard(theZombie->mX + 40, theZombie->mY + 40);
		int zRow = theZombie->mRow;
		TodParticleSystem* p = mApp->AddTodParticle(theZombie->mX + 40, theZombie->mY + 40, theZombie->mRenderOrder + 1, ParticleEffect::PARTICLE_IMITATER_MORPH);
		if (p) p->OverrideColor(nullptr, Color(160, 32, 240, 255));
		if (mBoard->CanPlantAt(zCol, zRow, SeedType::SEED_SPORESHROOM) == PlantingReason::PLANTING_OK) {
			mBoard->AddPlant(zCol, zRow, SeedType::SEED_SPORESHROOM, SeedType::SEED_NONE);
		}
	}

	float aLastPosX = mPosX - mVelX;
	float aLastPosY = mPosY + mPosZ - mVelY - mVelZ;
	ParticleEffect aEffect = ParticleEffect::PARTICLE_NONE;
	float aSplatPosX = mPosX + 12.0f;
	float aSplatPosY = mPosY + 12.0f;

	if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_MELONSPLASH);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		mApp->AddTodParticle(aLastPosX + 30.0f, aLastPosY + 30.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_WINTERMELON);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_GROUND, mCobTargetRow, 2);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, aRenderOrder, ParticleEffect::PARTICLE_BLASTMARK);
		mApp->AddTodParticle(mPosX + 80.0f, mPosY + 40.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POPCORNSPLASH);
		mApp->PlaySample(SOUND_DOOMSHROOM);
		mBoard->ShakeBoard(3, -4);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_PEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SUN)
	{
		aSplatPosX -= 15.0f;
		mBoard->AddCoin(mX, mY, CoinType::COIN_SMALLSUN, CoinMotion::COIN_MOTION_COIN);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_GOO)
	{
		aSplatPosX -= 15.0f;
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 12.0f, mPosY + 12.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PEA_SPLAT);
		if (aParticle)
		{
			aParticle->OverrideColor(nullptr, Color(220, 20, 255));
		}
		if (theZombie)
		{
			theZombie->ApplyAcid();
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_DROPL)
	{
		aSplatPosX -= 15.0f;
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 12.0f, mPosY + 12.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PEA_SPLAT);
		if (aParticle)
		{
			aParticle->OverrideColor(nullptr, Color(0, 220, 255));
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ROCK)
	{
		aSplatPosX -= 15.0f;
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 12.0f, mPosY + 12.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_PEA_SPLAT);
		if (aParticle)
		{
			aParticle->OverrideColor(nullptr, Color(210, 140, 50));
		}
		if (theZombie)
		{
			theZombie->mPosX += 4.0f; 
			theZombie->mX = (int)theZombie->mPosX;
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SAKURA)
	{
		aSplatPosX -= 15.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS)
	{
		aSplatPosX -= 15.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CHERRY)
	{
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 12.0f, mPosY + 12.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_POWIE);
		if (aParticle)
		{
			aParticle->OverrideScale(nullptr, 0.5f); 
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		aSplatPosX -= 15.0f;
		aEffect = ParticleEffect::PARTICLE_SNOWPEA_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEPPER)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		if (IsSplashDamage(theZombie))
		{
			Reanimation* aFireReanim = mApp->AddReanimation(mPosX + 38.0f, mPosY - 20.0f, mRenderOrder + 1, ReanimationType::REANIM_JALAPENO_FIRE);
			aFireReanim->mAnimTime = 0.25f;
			aFireReanim->mAnimRate = 24.0f;
			aFireReanim->OverrideScale(0.7f, 0.4f);
			aFireReanim->mColorOverride = Color(180, 230, 255, 255);
		}
	}
	if (mProjectileType == ProjectileType::PROJECTILE_SAKURA)
	{
		if (theZombie)
		{
			if (Rand(1000) < 2) theZombie->ApplyAcid();
			if (Rand(100000) < 5) theZombie->StartMindControlled();
		}
		TodParticleSystem* aParticle = mApp->AddTodParticle(mPosX + 12.0f, mPosY + 12.0f, mRenderOrder + 1, ParticleEffect::PARTICLE_FIREBALL_DEATH);
		if (aParticle)
		{
			aParticle->OverrideColor(nullptr, Color(255, 105, 180, 255)); 
			aParticle->OverrideScale(nullptr, 0.05f);
		}
		aEffect = ParticleEffect::PARTICLE_NONE;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		aEffect = ParticleEffect::PARTICLE_STAR_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		aSplatPosX -= 20.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF2)
	{
		aSplatPosX -= 20.0f;
		aEffect = ParticleEffect::PARTICLE_PUFF_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
	{
		aSplatPosX = aLastPosX - 38.0f;
		aSplatPosY = aLastPosY + 23.0f;
		aEffect = ParticleEffect::PARTICLE_CABBAGE_SPLAT;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;

		if (theZombie)
		{
			theZombie->ApplyButter();
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ACID_LEMON)
	{
		aSplatPosX = aLastPosX - 20.0f;
		aSplatPosY = aLastPosY + 63.0f;
		aEffect = ParticleEffect::PARTICLE_BUTTER_SPLAT;
	}
	if (aEffect != ParticleEffect::PARTICLE_NONE)
	{
		if (theZombie)
		{
			float aPosX = aSplatPosX + 52.0f - theZombie->mX;
			float aPosY = aSplatPosY - theZombie->mY;
			if (theZombie->mZombiePhase == ZombiePhase::PHASE_SNORKEL_WALKING_IN_POOL || theZombie->mZombiePhase == ZombiePhase::PHASE_DOLPHIN_WALKING_IN_POOL)
			{
				aPosY += 60.0f;
			}
			if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
			{
				aPosX -= 80.0f;
			}
			else if (mPosX > theZombie->mX + 40 && mMotionType != ProjectileMotion::MOTION_LOBBED)
			{
				aPosX -= 60.0f;
			}

			aPosY = ClampFloat(aPosY, 20.0f, 100.0f);
			theZombie->AddAttachedParticle(aPosX, aPosY, aEffect);
		}
		else
		{
			mApp->AddTodParticle(aSplatPosX, aSplatPosY, mRenderOrder + 1, aEffect);
		}
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CATTAIL_BOOST)
	{
		float aExplodeX = mPosX;
		float aExplodeY = mPosY;
		if (theZombie)
		{
			Rect zRect = theZombie->GetZombieRect();
			aExplodeX = zRect.mX + zRect.mWidth / 2 - 40.0f;
			aExplodeY = zRect.mY + zRect.mHeight / 2 - 40.0f;
		}
		Zombie* aSplashZombie = nullptr;
		while (mBoard->IterateZombies(aSplashZombie))
		{
			if (aSplashZombie != theZombie && !aSplashZombie->IsDeadOrDying() && !aSplashZombie->mMindControlled)
			{
				Rect aZombieRect = aSplashZombie->GetZombieRect();
				if (GetCircleRectOverlap(aExplodeX + 40, aExplodeY + 40, 300, aZombieRect))
				{
					aSplashZombie->TakeDamage(3600, 0U);
				}
			}
		}
		int aRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_TOP, mRow, 2);
		TodParticleSystem* aParticle = mApp->AddTodParticle(aExplodeX, aExplodeY, aRenderOrder, ParticleEffect::PARTICLE_DOOM);
		if (aParticle)
		{
			aParticle->OverrideColor(nullptr, Color(0, 191, 255, 255));
			aParticle->OverrideScale(nullptr, 1.2f);
		}
		mApp->PlaySample(SOUND_DOOMSHROOM);
		mBoard->ShakeBoard(3, -4);
	}
	Die();
}

void Projectile::Update()
{
	mProjectileAge++;
	if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->ShouldRunUpsellBoard())
		return;
	int aTime = 20;
	if (mProjectileType == ProjectileType::PROJECTILE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA || 
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_GOO ||
		mProjectileType == ProjectileType::PROJECTILE_DROPL ||
		mProjectileType == ProjectileType::PROJECTILE_ROCK ||
		mProjectileType == ProjectileType::PROJECTILE_SAKURA||
		mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS ||
		mProjectileType == ProjectileType::PROJECTILE_CHERRY ||
		mProjectileType == ProjectileType::PROJECTILE_CABBAGE || 
		mProjectileType == ProjectileType::PROJECTILE_MELON || 
		mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || 
		mProjectileType == ProjectileType::PROJECTILE_PEPPER||
		mProjectileType == ProjectileType::PROJECTILE_KERNEL || 
		mProjectileType == ProjectileType::PROJECTILE_BUTTER || 
		mProjectileType == ProjectileType::PROJECTILE_COBBIG || 
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA || 
		mProjectileType == ProjectileType::PROJECTILE_SPIKE)
	{
		aTime = 0;
	}
	if (mProjectileAge > aTime)
	{
		mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PROJECTILE, mRow, 0);
	}

	if (mClickBackoffCounter > 0)
	{
		mClickBackoffCounter--;
	}
	mRotation += mRotationSpeed;

	UpdateMotion();
	AttachmentUpdateAndMove(mAttachmentID, mPosX, mPosY + mPosZ);
}

void Projectile::Draw(Graphics* g)
{
	const ProjectileDefinition& aProjectileDef = GetProjectileDef();

	Image* aImage;
	float aScale = 1.0f;
	if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		aImage = IMAGE_REANIM_COBCANNON_COB;
		aScale = mIsMeteorBoost ? (0.9f * 1.5f) : 0.9f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_NINJA)
	{
		aImage = IMAGE_PROJECTILE_NINJA;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CATTAIL_BOOST)
	{
		aImage = IMAGE_PROJECTILECACTUS2;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEA || mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA)
	{
		aImage = IMAGE_PROJECTILEPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ROCK)
	{
		aImage = IMAGE_PROJECTILEROCK;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SUN)
	{
		aImage = IMAGE_SUN;
		aScale = 0.75f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_GOO)
	{
		aImage = IMAGE_PROJECTILEGOO;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_DROPL)
	{
		aImage = IMAGE_PROJECTILEDROPL;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		aImage = IMAGE_PROJECTILESNOWPEA;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS)
	{
		aImage = IMAGE_PROJECTILEMAGNIFYING_GRASS;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_ICE_CABBAGE)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE2; 
		aScale = 1.0f;
	}
	else if ( mProjectileType == ProjectileType::PROJECTILE_ACID_LEMON)
	{
		aImage = IMAGE_ACID;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CHERRY)
	{
		aImage = IMAGE_PROJECTILECHERRY;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SAKURA)
	{
		aImage = IMAGE_PROJECTILECHERRY;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		aImage = nullptr;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		aImage = nullptr;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE)
	{
		aImage = IMAGE_PROJECTILECACTUS;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_STAR)
	{
		aImage = IMAGE_PROJECTILE_STAR;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF1;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PUFF2)
	{
		aImage = IMAGE_PUFFSHROOM_PUFF3;
		aScale = TodAnimateCurveFloat(0, 30, mProjectileAge, 0.3f, 1.0f, TodCurves::CURVE_LINEAR);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BASKETBALL)
	{
		aImage = IMAGE_REANIM_ZOMBIE_CATAPULT_BASKETBALL;
		aScale = 1.1f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CABBAGE)
	{
		aImage = IMAGE_REANIM_CABBAGEPULT_CABBAGE;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_KERNEL)
	{
		aImage = IMAGE_REANIM_CORNPULT_KERNAL;
		aScale = 0.95f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_BUTTER)
	{
		aImage = IMAGE_REANIM_CORNPULT_BUTTER;
		aScale = 0.8f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON)
	{
		aImage = IMAGE_REANIM_MELONPULT_MELON;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PEPPER)
	{
		aImage = IMAGE_P;
		aScale = 1.0f;
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		aImage = IMAGE_REANIM_WINTERMELON_PROJECTILE;
		aScale = 1.0f;
	}
	else
	{
		TOD_ASSERT();
	}

	bool aMirror = false;
	if (mMotionType == ProjectileMotion::MOTION_BEE_BACKWARDS)
	{
		aMirror = true;
	}

	if (aImage)
	{
		TOD_ASSERT(aProjectileDef.mImageRow < aImage->mNumRows);
		TOD_ASSERT(mFrame < aImage->mNumCols);

		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		Rect aSrcRect(aCelWidth * mFrame, aCelHeight * aProjectileDef.mImageRow, aCelWidth, aCelHeight);
		if (FloatApproxEqual(mRotation, 0.0f) && FloatApproxEqual(aScale, 1.0f))
		{
			Rect aDestRect(0, 0, aCelWidth, aCelHeight);
			g->DrawImageMirror(aImage, aDestRect, aSrcRect, aMirror);
		}
		else
		{
			float aOffsetX = mPosX + aCelWidth * 0.5f;
			float aOffsetY = mPosZ + mPosY + aCelHeight * 0.5f;
			SexyTransform2D aTransform;
			TodScaleRotateTransformMatrix(aTransform, aOffsetX + mBoard->mX, aOffsetY + mBoard->mY, mRotation, aScale, aScale);
			Color aColorTint = (mProjectileType == ProjectileType::PROJECTILE_COBBIG && mIsMeteorBoost) ? Color(255, 80, 0, 255) : Color::White;
			TodBltMatrix(g, aImage, aTransform, g->mClipRect, aColorTint, g->mDrawMode, aSrcRect);
		}
	}

	if (mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
	{
		Graphics theParticleGraphics(*g);
		MakeParentGraphicsFrame(&theParticleGraphics);
		AttachmentDraw(mAttachmentID, &theParticleGraphics, false);
	}
}

void Projectile::DrawShadow(Graphics* g)
{
	int aCelCol = 0;
	float aScale = 1.0f;
	float aStretch = 1.0f;
	float aOffsetX = mPosX - mX;
	float aOffsetY = mPosY - mY;

	int aGridX = mBoard->PixelToGridXKeepOnBoard(mX, mY);
	bool isHighGround = false;
	if (mBoard->mGridSquareType[aGridX][mRow] == GridSquareType::GRIDSQUARE_HIGH_GROUND)
	{
		isHighGround = true;
	}
	if (mOnHighGround && !isHighGround)
	{
		aOffsetY += HIGH_GROUND_HEIGHT;
	}
	else if (!mOnHighGround && isHighGround)
	{
		aOffsetY -= HIGH_GROUND_HEIGHT;
	}

	if (mBoard->StageIsNight())
	{
		aCelCol = 1;
	}

	switch (mProjectileType)
	{
	case ProjectileType::PROJECTILE_PEA:
	case ProjectileType::PROJECTILE_SUN:
	case ProjectileType::PROJECTILE_GOO:
	case ProjectileType::PROJECTILE_DROPL:
	case ProjectileType::PROJECTILE_ROCK:
	case ProjectileType::PROJECTILE_SAKURA:
	case ProjectileType::PROJECTILE_MAGNIFYING_GRASS:
	case ProjectileType::PROJECTILE_CHERRY:
	case ProjectileType::PROJECTILE_ZOMBIE_PEA:
		aOffsetX += 3.0f;
		break;
	case ProjectileType::PROJECTILE_ICE_CABBAGE:
	case ProjectileType::PROJECTILE_ACID_LEMON:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		aScale = 1.6f;
		break;
	case ProjectileType::PROJECTILE_SNOWPEA:
		aOffsetX += -1.0f;
		aScale = 1.3f;
		break;
	case ProjectileType::PROJECTILE_CATTAIL_BOOST:
		aOffsetX += 3.0f;
		aScale = 2.0f;
		break;
	case ProjectileType::PROJECTILE_STAR:
		aOffsetX += 7.0f;
		break;

	case ProjectileType::PROJECTILE_CABBAGE:
	case ProjectileType::PROJECTILE_KERNEL:
	case ProjectileType::PROJECTILE_BUTTER:
	case ProjectileType::PROJECTILE_MELON:
	case ProjectileType::PROJECTILE_PEPPER:
	case ProjectileType::PROJECTILE_WINTERMELON:
	case ProjectileType::PROJECTILE_BASKETBALL:
		aOffsetX += 3.0f;
		aOffsetY += 10.0f;
		aScale = 1.6f;
		break;

	case ProjectileType::PROJECTILE_PUFF:
	case ProjectileType::PROJECTILE_PUFF2:
		return;
		
	case ProjectileType::PROJECTILE_COBBIG:
		aScale = mIsMeteorBoost ? 1.5f : 1.0f;
		aStretch = 3.0f;
		aOffsetX += 57.0f;
		break;

	case ProjectileType::PROJECTILE_FIREBALL:
		aScale = 1.4f;
		break;
	case ProjectileType::PROJECTILE_PLASMABALL:
		aScale = 1.4f;
		break;
	}

	if (mMotionType == ProjectileMotion::MOTION_LOBBED)
	{
		float aHeight = ClampFloat(-mPosZ, 0.0f, 200.0f);
		aScale *= 200.0f / (aHeight + 200.0f);
	}

	TodDrawImageCelScaledF(g, IMAGE_PEA_SHADOWS, aOffsetX, (mShadowY - mPosY + aOffsetY), aCelCol, 0, aScale * aStretch, aScale);
}

void Projectile::Die()
{
	mDead = true;

	if (mProjectileType == ProjectileType::PROJECTILE_PUFF || mProjectileType == ProjectileType::PROJECTILE_PUFF2 || mProjectileType == ProjectileType::PROJECTILE_SNOWPEA)
	{
		AttachmentCrossFade(mAttachmentID, "FadeOut");
		AttachmentDetach(mAttachmentID);
	}
	else
	{
		AttachmentDie(mAttachmentID);
	}

}

Rect Projectile::GetProjectileRect()
{
	if (mProjectileType == ProjectileType::PROJECTILE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_SNOWPEA ||
		mProjectileType == ProjectileType::PROJECTILE_SUN ||
		mProjectileType == ProjectileType::PROJECTILE_GOO ||
		mProjectileType == ProjectileType::PROJECTILE_DROPL ||
		mProjectileType == ProjectileType::PROJECTILE_ROCK ||
		mProjectileType == ProjectileType::PROJECTILE_SAKURA ||
		mProjectileType == ProjectileType::PROJECTILE_MAGNIFYING_GRASS ||
		mProjectileType == ProjectileType::PROJECTILE_ZOMBIE_PEA ||
		mProjectileType == ProjectileType::PROJECTILE_NINJA)
	{
		return Rect(mX - 15, mY, mWidth + 15, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_COBBIG)
	{
		return Rect(mX + mWidth / 2 - 115, mY + mHeight / 2 - 115, 230, 230);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_PEPPER || mProjectileType == ProjectileType::PROJECTILE_WINTERMELON)
	{
		return Rect(mX + 20, mY, 60, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_FIREBALL)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_CHERRY)
	{
		return Rect(mX, mY, mWidth - 10, mHeight);
	}
	else if (mProjectileType == ProjectileType::PROJECTILE_SPIKE)
	{
		return Rect(mX - 25, mY, mWidth + 25, mHeight);
	}
	else
	{
		return Rect(mX, mY, mWidth, mHeight);
	}
}

void Projectile::ConvertToFireball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	mProjectileType = ProjectileType::PROJECTILE_FIREBALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);

	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_FIRE_PEA);
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}

	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}
void Projectile::ConvertToPlasmaball(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;
	mProjectileType = ProjectileType::PROJECTILE_PLASMABALL;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_FIREPEA);
	float aOffsetX = -25.0f;
	float aOffsetY = -25.0f;
	Reanimation* aFirePeaReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_PLASMA_PEA);
	if (mMotionType == ProjectileMotion::MOTION_BACKWARDS)
	{
		aFirePeaReanim->OverrideScale(-1.0f, 1.0f);
		aOffsetX += 80.0f;
	}
	aFirePeaReanim->SetPosition(mPosX + aOffsetX, mPosY + aOffsetY);
	aFirePeaReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
	aFirePeaReanim->mAnimRate = RandRangeFloat(50.0f, 80.0f);
	AttachReanim(mAttachmentID, aFirePeaReanim, aOffsetX, aOffsetY);
}
void Projectile::ConvertToPea(int theGridX)
{
	if (mHitTorchwoodGridX == theGridX)
		return;

	AttachmentDie(mAttachmentID);
	mProjectileType = ProjectileType::PROJECTILE_PEA;
	mHitTorchwoodGridX = theGridX;
	mApp->PlayFoley(FoleyType::FOLEY_THROW);
}

ProjectileDefinition& Projectile::GetProjectileDef()
{
	ProjectileDefinition& aProjectileDef = gProjectileDefinition[(int)mProjectileType];
	TOD_ASSERT(aProjectileDef.mProjectileType == mProjectileType);

	return aProjectileDef;
}

bool Projectile::GetGargantuars(int theRow, int theX, int theY, int theRadius, int theRowRange)
{
	Zombie* aZombie = nullptr;
	while (mBoard->IterateZombies(aZombie))
	{
		if (!aZombie->IsDeadOrDying() && !aZombie->mMindControlled && (aZombie->mZombieType == ZombieType::ZOMBIE_GARGANTUAR || aZombie->mZombieType == ZombieType::ZOMBIE_REDEYE_GARGANTUAR))
		{
			Rect aZombieRect = aZombie->GetZombieRect();
			int aRowDist = aZombie->mRow - theRow;

			if (aRowDist <= theRowRange && aRowDist >= -theRowRange && GetCircleRectOverlap(theX, theY, theRadius, aZombieRect))
				return true;
		}
	}
	return false;
}
GridItem* Projectile::FindCollisionTargetPVZ2Grave()
{
	if (PeaAboutToHitTorchwood()) return nullptr;

	Rect aProjectileRect = GetProjectileRect();
	GridItem* aBestGrave = nullptr;
	// ÌÃ»  ⁄ÌÌ‰ ﬁÌ„… ÷Œ„… · ”„Õ »«Œ Ì«— «·ﬁ»— «·√ﬁ—» ··„ﬁ–Ê›
	int aMinX = 999999;

	GridItem* aGridItem = nullptr;
	while (mBoard->IterateGridItems(aGridItem))
	{
		// «· Õﬁﬁ «·’«—„ „‰ «·”ÿ— (mRow) Ì„‰⁄ œ„Ã √Ì ﬁ»— ›Ì „”«— ¬Œ—
		if (aGridItem->mGridItemType == GridItemType::GRIDITEM_PVZ2_GRAVE && aGridItem->mGridY == mRow)
		{
			Rect aGraveRect = aGridItem->GetPVZ2GraveRect();
			if (GetRectOverlap(aProjectileRect, aGraveRect) > 0)
			{
				if (aBestGrave == nullptr || aGridItem->mPosX < aMinX)
				{
					aBestGrave = aGridItem;
					aMinX = aGridItem->mPosX;
				}
			}
		}
	}
	return aBestGrave;
}
void Projectile::DoGraveImpact(GridItem* theGrave)
{
	PlayImpactSound(nullptr); 
	if (mProjectileType == ProjectileType::PROJECTILE_MELON || mProjectileType == ProjectileType::PROJECTILE_PEPPER|| mProjectileType == ProjectileType::PROJECTILE_WINTERMELON || mProjectileType == ProjectileType::PROJECTILE_FIREBALL || mProjectileType == ProjectileType::PROJECTILE_PLASMABALL)
	{
		DoSplashDamage(nullptr); 
	}
	theGrave->TakeDamage(GetProjectileDef().mDamage);
	Die(); 
}