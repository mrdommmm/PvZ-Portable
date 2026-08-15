#include "Bush.h"
#include "Board.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/TodDebug.h"
#include "../Sexy.TodLib/Reanimator.h"

const ReanimationType cBushReanims[] = { ReanimationType::REANIM_BUSH3, ReanimationType::REANIM_BUSH5, ReanimationType::REANIM_BUSH4, ReanimationType::REANIM_BUSH3_NIGHT, ReanimationType::REANIM_BUSH5_NIGHT, ReanimationType::REANIM_BUSH4_NIGHT };

const int cBushPos[][2] = {
    { 950, 40 },
    { 962, 168 },
    { 968, 258 },
    { 972, 378 },
    { 964, 459 },
    { 980, 510 }
};
const int cBushPos6Rows[][2] = {
    { 952, 42 },
    { 964, 170 },
    { 968, 258 },
    { 974, 380 },
    { 966, 461 },
    { 979, 509 }
};

void Bush::BushInitialize(int theRow, bool theNight)
{
    mDead = false;
    int aIndex = (theRow + 3) % 3;
    mPosX = mBoard->StageHas6Rows() ? cBushPos6Rows[theRow][0] : cBushPos[theRow][0];
    mPosY = mBoard->StageHas6Rows() ? cBushPos6Rows[theRow][1] : cBushPos[theRow][1];
    mID = theRow;
    mBushIndex = aIndex;
    mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ZOMBIE, theRow + 1, 0);

    Reanimation* oldDay = mApp->ReanimationTryToGet(mReanimID);
    if (oldDay) oldDay->ReanimationDie();
    Reanimation* oldNight = mApp->ReanimationTryToGet(mReanimIDNight);
    if (oldNight) oldNight->ReanimationDie();

    Reanimation* aDayReanim = mApp->AddReanimation(mPosX, mPosY, mRenderOrder, cBushReanims[mBushIndex]);
    mReanimID = mApp->ReanimationGetID(aDayReanim);
    aDayReanim->PlayReanim("base bush", REANIM_PLAY_ONCE_AND_HOLD, 0, 0.001f);

    Reanimation* aNightReanim = mApp->AddReanimation(mPosX, mPosY, mRenderOrder, cBushReanims[mBushIndex + 3]);
    mReanimIDNight = mApp->ReanimationGetID(aNightReanim);
    aNightReanim->PlayReanim("base bush", REANIM_PLAY_ONCE_AND_HOLD, 0, 0.001f);
}

void Bush::AnimateBush()
{
    Reanimation* aDayReanim = mApp->ReanimationTryToGet(mReanimID);
    if (aDayReanim)
        aDayReanim->PlayReanim("anim_rustle", REANIM_PLAY_ONCE_AND_HOLD, 10, RandRangeFloat(8.0f, 10.0f));

    Reanimation* aNightReanim = mApp->ReanimationTryToGet(mReanimIDNight);
    if (aNightReanim)
        aNightReanim->PlayReanim("anim_rustle", REANIM_PLAY_ONCE_AND_HOLD, 10, RandRangeFloat(8.0f, 10.0f));
}

void Bush::Update()
{
    Reanimation* aDayReanim = mApp->ReanimationTryToGet(mReanimID);
    if (aDayReanim)
        aDayReanim->Update();

    Reanimation* aNightReanim = mApp->ReanimationTryToGet(mReanimIDNight);
    if (aNightReanim)
        aNightReanim->Update();
}

void Bush::Draw(Graphics* g)
{
    Reanimation* aDayReanim = mApp->ReanimationTryToGet(mReanimID);
    Reanimation* aNightReanim = mApp->ReanimationTryToGet(mReanimIDNight);

    if (!aDayReanim || !aNightReanim) return;

    float nightAlpha = 0.0f;
    bool isEclipseLevel = (mBoard->mApp->IsAdventureMode() && mBoard->mLevel >= 51 && mBoard->mLevel <= 59 && mBoard->mLevel != 57 && mBoard->mLevel != 55);

    if (mBoard->mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_DYNAMIC_ROOF) {
        nightAlpha = 255.0f - mBoard->mRoofTransitionAlpha;
    }
    else if (isEclipseLevel) {
        nightAlpha = mBoard->mEclipseCounter > 0 ? 255.0f : 0.0f;
    }
    else {
        if (mBoard->mStartsAsDay) {
            nightAlpha = mBoard->mGlobalTimeAlpha;
        }
        else {
            nightAlpha = 255.0f - mBoard->mGlobalTimeAlpha;
        }
    }

    int alphaNight = (int)nightAlpha;
    if (alphaNight > 255) alphaNight = 255;
    if (alphaNight < 0) alphaNight = 0;

    if (alphaNight < 255) {
        g->SetColorizeImages(true);
        g->SetColor(Color(255, 255, 255, 255 - alphaNight));
        aDayReanim->Draw(g);
        g->SetColorizeImages(false);
    }

    if (alphaNight > 0) {
        g->SetColorizeImages(true);
        g->SetColor(Color(255, 255, 255, alphaNight));
        aNightReanim->Draw(g);
        g->SetColorizeImages(false);
    }
}