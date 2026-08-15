#include "GameButton.h"
#include "../../LawnApp.h"
#include "../System/Music.h"
#include "MiniCreditsScreen.h"
#include "../../Resources.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Debug.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/WidgetManager.h"
#include "../../GameConstants.h"

MiniCreditsScreen::MiniCreditsScreen(LawnApp* theApp)
{
    mApp = theApp;
    mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_CREDITS_ZOMBIES_ON_YOUR_LAWN);
    TodLoadResources("DelayLoad_ChallengeScreen");
    TodLoadResources("DelayLoad_Credits");
    mScrollY = 720.0f;

    // --- إعداد زر الرجوع (الخروج) ---
    mBackButton = MakeNewButton(0, this, _S("[BACK_TO_MENU_BUTTON]"), nullptr, Sexy::IMAGE_SEEDCHOOSER_BUTTON2,
        Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW, Sexy::IMAGE_SEEDCHOOSER_BUTTON2_GLOW);
    mBackButton->mTextDownOffsetX = 1;
    mBackButton->mTextDownOffsetY = 1;
    mBackButton->mColors[ButtonWidget::COLOR_LABEL] = Color(42, 42, 90);
    mBackButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(42, 42, 90);

    // توسيط الزر في المركز 640
    int aBackW = IMAGE_SEEDCHOOSER_BUTTON2->mWidth;
    int aBackH = IMAGE_SEEDCHOOSER_BUTTON2->mHeight;
    mBackButton->Resize(640 - (aBackW / 2), 580, aBackW, aBackH);
    mBackButton->mVisible = false;
    mBackButton->mDisabled = true;

    // --- إعداد زر الموسيقى ---
    mMusicButton = MakeNewButton(1, this, _S("[CREDITS_BUTTON]"), nullptr, Sexy::IMAGE_CREDITS_PLAYBUTTON,
        Sexy::IMAGE_CREDITS_PLAYBUTTON, Sexy::IMAGE_CREDITS_PLAYBUTTON);
    mMusicButton->SetFont(Sexy::FONT_HOUSEOFTERROR20);
    mMusicButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 255, 255);
    mMusicButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(213, 159, 43);

    // قياس الإحداثيات ليكون فوق زر الخروج ومضبوط في النص تماماً
    int aMusW = 400; // العرض المحدد للزر
    int aMusH = 73;
    // وضعه عند Y=490 ليكون فوق زر الخروج (580) بمسافة كافية
    mMusicButton->Resize(640 - (aMusW / 2), 490, aMusW, aMusH);

    mMusicButton->mTextOffsetX = -30;
    mMusicButton->mTextOffsetY = -2;
    mMusicButton->mButtonOffsetY = 8;
    mMusicButton->mVisible = false;
    mMusicButton->mDisabled = true;

    mApp->mDetails = _S("[DISCORD_CREDITS_SCREEN]");
}

MiniCreditsScreen::~MiniCreditsScreen()
{
    delete mBackButton;
    if (mMusicButton)
        delete mMusicButton;
}

void MiniCreditsScreen::Draw(Graphics* g)
{
    g->DrawImage(Sexy::IMAGE_CREDITS_BG, 0, 0);

    Color colorTitle = Color(255, 215, 0);       // ذهبي
    Color colorName = Color(255, 255, 255);      // أبيض
    Color colorDate = Color(144, 238, 144);      // أخضر
    Color colorSpecial = Color(255, 165, 0);      // برتقالي
    Color colorVIP = Color(0, 255, 255);        // سماوي
    Color colorSoftCyan = Color(135, 206, 235);
    Color colorApology = Color(220, 220, 220);

    int centerX = 640;
    int leftX = 240;
    int rightX = 1040;
    float y = mScrollY;

    // 1. اللوجو
    if (Sexy::IMAGE_PVZ_LOGO) {
        g->DrawImage(Sexy::IMAGE_PVZ_LOGO, centerX - (Sexy::IMAGE_PVZ_LOGO->mWidth / 2), y);
        y += Sexy::IMAGE_PVZ_LOGO->mHeight + 15;
    }

    TodDrawString(g, "Credits", centerX, y, Sexy::FONT_HOUSEOFTERROR28, colorSoftCyan, DS_ALIGN_CENTER);
    y += 100;

    // 2. الفريق الأساسي
    TodDrawString(g, "Mod Core Team", centerX, y, Sexy::FONT_HOUSEOFTERROR28, colorTitle, DS_ALIGN_CENTER); y += 60;

    TodDrawString(g, "Programmer", leftX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_LEFT);
    TodDrawString(g, "Code Assistant", rightX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_RIGHT);
    y += 30;
    TodDrawString(g, "Mr Doom", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "bonel_bonel", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT);
    y += 60;

    TodDrawString(g, "Advertisement", leftX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_LEFT);
    TodDrawString(g, "Game Tester", rightX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_RIGHT);
    y += 30;
    TodDrawString(g, "Puff Shroom", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "the_shade50050", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT);
    y += 100;

    // 3. الصوتيات
    TodDrawString(g, "Audio & Music", centerX, y, Sexy::FONT_HOUSEOFTERROR28, colorTitle, DS_ALIGN_CENTER); y += 45;
    TodDrawString(g, "Night Roof Music", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorSoftCyan, DS_ALIGN_CENTER); y += 30;
    TodDrawString(g, "Mr. Johnson Speckles", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER);
    y += 100;

    // 4. الرسوميات والأنميشن (توزيع منظم جداً)
    TodDrawString(g, "Art & Textures", leftX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_LEFT);
    TodDrawString(g, "Animations", rightX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_RIGHT);
    y += 45;

    // تم زيادة المسافة الرأسية لـ 35 لمنع التداخل
    TodDrawString(g, "Acidic Lemon: Grassguy12", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Acidic Lemon Anim: Unnamed", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Beehive & Cucumber: jacobpl123", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Particles: Unnamed", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Plant Food: green shadow", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Trampoline: jacobpl123", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Iceberg Cabbage: inliothixie", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Umbrella Leaf: coolhasan", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Butter: J.S.T PvZ", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Umbrella Anim: Gabbiejams", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Megagargantuar: Raducu", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Pults & Vehicles: 4ntonellasrk", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Almanac: Pipebomb", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Fume-shroom: BeAnimate's", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Metal & Daisies: Fabián 2do", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Imitater: Masterfabian895", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Explode-o-nut: Spooklass", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Coffee Bean: 00ga_booga", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "Endangered: person6423__72195", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Brickhead: Simon3676", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT); y += 35;

    TodDrawString(g, "General Textures: Pipebomb", leftX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_LEFT);
    TodDrawString(g, "Sunday Zombie: Chinese Modding", rightX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_RIGHT);
    y += 120;

    // 5. شكر خاص
    TodDrawString(g, "MASSIVE SPECIAL THANKS", centerX, y, Sexy::FONT_HOUSEOFTERROR28, colorSpecial, DS_ALIGN_CENTER); y += 50;
    TodDrawString(g, "MA SERVER", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorVIP, DS_ALIGN_CENTER); y += 35;
    TodDrawString(g, "omeo  &  TG", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorVIP, DS_ALIGN_CENTER); y += 35;
    TodDrawString(g, "To all my amazing Subscribers!", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorVIP, DS_ALIGN_CENTER); y += 70;

    TodDrawString(g, "Special Support", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_CENTER); y += 35;
    TodDrawString(g, "thrruquiredcraftmaskgbfinal123", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER); y += 30;
    TodDrawString(g, "zomboss_92", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER); y += 30;
    TodDrawString(g, "mr trex", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER);
    y += 100;

    // 6. المطورين الأصليين
    TodDrawString(g, "Original PVZ Team", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_CENTER); y += 40;
    TodDrawString(g, "George Fan, Rich Werner, Stephen Notley", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER); y += 30;
    TodDrawString(g, "Laura Shigihara, Tod Semple", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorName, DS_ALIGN_CENTER);
    y += 110;

    // 7. النهاية (آخر الكلمات)
    TodDrawString(g, "Demo 1.0.1 Completed On", centerX, y, Sexy::FONT_HOUSEOFTERROR20, colorTitle, DS_ALIGN_CENTER); y += 40;
    TodDrawString(g, " 6 / 29 / 2026 ", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorDate, DS_ALIGN_CENTER); y += 35;
    TodDrawString(g, "Total Time: ~ 2.5 Months", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorDate, DS_ALIGN_CENTER);
    y += 100;

    TodDrawString(g, "We apologize to anyone we missed.", centerX, y, Sexy::FONT_HOUSEOFTERROR16, colorApology, DS_ALIGN_CENTER); y += 30;
    TodDrawString(g, "Thank you for playing!", centerX, y, Sexy::FONT_HOUSEOFTERROR28, colorTitle, DS_ALIGN_CENTER);
}

void MiniCreditsScreen::Update()
{
    Widget::Update();
    mBackButton->Update();
    if (mApp->HasFinishedAdventure())
        mMusicButton->Update();

    mScrollY -= 0.6f; // سرعة الرفع

    // المنطق الجديد: إذا صعدت آخر كلمة (الموجودة تقريباً عند مسافة -3200 من البداية) يظهر الزر
    // قمنا بضبط الرقم ليظهر الزر بمجرد خروج النصوص من الشاشة من الأعلى
    if (mScrollY < -1500.0f && !mBackButton->mVisible)
    {
        mBackButton->mVisible = true;
        mBackButton->mDisabled = false;

        if (mApp->HasFinishedAdventure()) {
            mMusicButton->mVisible = true;
            mMusicButton->mDisabled = false;
        }
    }
    MarkDirty();
}
void MiniCreditsScreen::AddedToManager(WidgetManager* theWidgetManager)
{
    Widget::AddedToManager(theWidgetManager);
    AddWidget(mBackButton);
    if (mApp->HasFinishedAdventure())
        AddWidget(mMusicButton);
}

void MiniCreditsScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
    Widget::RemovedFromManager(theWidgetManager);
    RemoveWidget(mBackButton);
    if (mApp->HasFinishedAdventure())
        RemoveWidget(mMusicButton);
}

void MiniCreditsScreen::ButtonPress(int theId)
{
    mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
}



void MiniCreditsScreen::ButtonDepress(int theId)
{
    if (theId == 0)
    {
        mApp->KillMiniCreditScreen();
        mApp->DoBackToMain();
    }
    if (theId == 1)
    {
        mApp->KillMiniCreditScreen();
        mApp->ShowCreditScreen();
    }
}