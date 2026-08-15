#include "../Board.h"
#include "GameButton.h"
#include "../Cutscene.h"
#include "AlmanacDialog.h"
#include "../LawnCommon.h"
#include "../../LawnApp.h"
#include "../System/Music.h"
#include "../../Resources.h"
#include "NewOptionsDialog.h"
#include "../../ConstEnums.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../SexyAppFramework/Slider.h"
#include "../../SexyAppFramework/Checkbox.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/Font.h"
using namespace Sexy;
const Color cTextColor(107, 109, 145);
NewOptionsDialog::NewOptionsDialog(LawnApp* theApp, bool theFromGameSelector,
	bool theAdvanced) :
	Dialog(nullptr, nullptr, Dialogs::DIALOG_NEWOPTIONS, true, _S(""), _S(""), _S(""),
		Dialog::BUTTONS_NONE)
{
	TodLoadResources("DelayLoad_QuickPlay");
	mApp = theApp;
	mFromGameSelector = theFromGameSelector;
	mAdvancedMode = theAdvanced;
	mAdvancedPage = 0;
	SetColor(Dialog::COLOR_BUTTON_TEXT, Color(255, 255, 100));
	mAlmanacButton = MakeButton(NewOptionsDialog::NewOptionsDialog_Almanac, this,
		_S("[VIEW_ALMANAC_BUTTON]"));
	mRestartButton = MakeButton(NewOptionsDialog::NewOptionsDialog_Restart, this,
		_S("[RESTART_LEVEL_BUTTON]"));
	mBackToMainButton = MakeButton(NewOptionsDialog::NewOptionsDialog_MainMenu,
		this, _S("[MAIN_MENU_BUTTON]"));
	mAdvancedButton = MakeButton(NewOptionsDialog::NewOptionsDialog_Advanced,
		this, _S("[ADVANCED_OPTIONS_BUTTON]"));
	mBackToGameButton = MakeNewButton(Dialog::ID_OK, this, _S("[BACK_TO_GAME]"),
		nullptr, IMAGE_OPTIONS_BACKTOGAMEBUTTON0,
		IMAGE_OPTIONS_BACKTOGAMEBUTTON0,
		IMAGE_OPTIONS_BACKTOGAMEBUTTON2);
	mBackToGameButton->mTranslateX = 0;
	mBackToGameButton->mTranslateY = 0;
	mBackToGameButton->mTextOffsetX = -2;
	mBackToGameButton->mTextOffsetY = -5;
	mBackToGameButton->mTextDownOffsetX = 0;
	mBackToGameButton->mTextDownOffsetY = 1;
	mBackToGameButton->SetFont(FONT_DWARVENTODCRAFT36GREENINSET);
	mBackToGameButton->SetColor(ButtonWidget::COLOR_LABEL, Color::White);
	mBackToGameButton->SetColor(ButtonWidget::COLOR_LABEL_HILITE, Color::White);
	mBackToGameButton->mHiliteFont =
		FONT_DWARVENTODCRAFT36BRIGHTGREENINSET;
	mMusicVolumeSlider = new Sexy::Slider(IMAGE_OPTIONS_SLIDERSLOT,
		IMAGE_OPTIONS_SLIDERKNOB2, NewOptionsDialog::NewOptionsDialog_MusicVolume,
		this);
	double aMusicVolume = mApp->GetMusicVolume();
	aMusicVolume = max(0.0, min(1.0, aMusicVolume));
	mMusicVolumeSlider->SetValue(aMusicVolume);
	mSfxVolumeSlider = new Sexy::Slider(IMAGE_OPTIONS_SLIDERSLOT,
		IMAGE_OPTIONS_SLIDERKNOB2,
		NewOptionsDialog::NewOptionsDialog_SoundVolume, this);
	mSfxVolumeSlider->SetValue(mApp->GetSfxVolume());
	mDifficultySlider = new Sexy::Slider(IMAGE_OPTIONS_SLIDERSLOT,
		IMAGE_OPTIONS_SLIDERKNOB2, NewOptionsDialog::NewOptionsDialog_Difficulty, this);
	if (mApp->mDifficulty == 0) mDifficultySlider->SetValue(0.0);
	else if (mApp->mDifficulty == 1) mDifficultySlider->SetValue(0.5);
	else mDifficultySlider->SetValue(1.0);
	mDifficultySlider->SetVisible(false);
	mFullscreenCheckbox =
		MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_Fullscreen, this,
			!mApp->mIsWindowed);
	mHardwareAccelerationCheckbox =
		MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_HardwareAcceleration, this,
			mApp->Is3dAccel());
	mDebugModeCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_DebugMode, this, mApp->mTodCheatKeys);
	mDebugModeCheckbox->SetVisible(false);
	mDiscordCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_Discord, this, mApp->mDiscordPresence);
	mDiscordCheckbox->SetVisible(false);
	mBankKeybindsCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_BankKeybinds, this, mApp->mBankKeybinds);
	mBankKeybindsCheckbox->SetVisible(false);
	m09FormatCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_09Format, this, mApp->mZeroNineBankFormat);
	m09FormatCheckbox->SetVisible(false);
	mAutoCollectSunsCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_AutoCollectSuns, this, mApp->mAutoCollectSuns);
	mAutoCollectSunsCheckbox->SetVisible(false);
	mAutoCollectCoinsCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_AutoCollectCoins, this, mApp->mAutoCollectCoins);
	mAutoCollectCoinsCheckbox->SetVisible(false);
	mZombieHealthbarsCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_ZombieHealthbars, this, mApp->mZombieHealthbars);
	mZombieHealthbarsCheckbox->SetVisible(false);
	mPlantHealthbarsCheckbox = MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_PlantHealthbars, this, mApp->mPlantHealthbars);
	mPlantHealthbarsCheckbox->SetVisible(false);
	mLeftPageButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_LeftPage,
		this, "", nullptr, Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT);
	mLeftPageButton->SetVisible(false);
	mRightPageButton =
		MakeNewButton(NewOptionsDialog::NewOptionsDialog_RightPage, this, "", nullptr,
			Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON,
			Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT,
			Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT);
	mRightPageButton->SetVisible(false);
	mBitsValue = mApp->mFullscreenBits;
	mBitsLeftButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_BitsLeft, this,
		"", nullptr, Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT);
	mBitsLeftButton->SetVisible(false);
	mBitsRightButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_BitsRight,
		this, "", nullptr, Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON,
		Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT,
		Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT);
	mBitsRightButton->SetVisible(false);
	mDifficultyLeftButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_DifficultyLeft, this,
		"", nullptr, Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT,
		Sexy::IMAGE_QUICKPLAY_LEFT_BUTTON_HIGHLIGHT);
	mDifficultyLeftButton->SetVisible(false);
	mDifficultyRightButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_DifficultyRight, this,
		"", nullptr, Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON,
		Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT,
		Sexy::IMAGE_QUICKPLAY_RIGHT_BUTTON_HIGHLIGHT);
	mDifficultyRightButton->SetVisible(false);
	mSpeedEditWidget = CreateEditWidget(-1, this, this);
	mSpeedEditWidget->mMaxChars = 1;
	mSpeedEditWidget->SetFont(FONT_DWARVENTODCRAFT18GREENINSET);
	mSpeedEditWidget->AddWidthCheckFont(FONT_DWARVENTODCRAFT18GREENINSET,
		IMAGE_OPTIONS_CHECKBOX0->mWidth);
	mSpeedEditWidget->SetText(StrFormat(_S("%d"), mApp->mSpeedModifier));
	mSpeedEditWidget->SetColor(ButtonWidget::COLOR_LIGHT_OUTLINE, Color(1, 233,
		1));
	mSpeedEditWidget->SetVisible(false);
	mGameAdvancedButton =
		MakeNewButton(NewOptionsDialog::NewOptionsDialog_Advanced, this,
			_S("[ADVANCED_OPTIONS_BUTTON_SHORT]"), nullptr,
			Sexy::IMAGE_BUTTON_SMALL,
			Sexy::IMAGE_BUTTON_SMALL, Sexy::IMAGE_BUTTON_DOWN_SMALL);
	mGameAdvancedButton->SetFont(FONT_DWARVENTODCRAFT18GREENINSET);
	mGameAdvancedButton->SetColor(ButtonWidget::COLOR_LABEL, Color::White);
	mGameAdvancedButton->SetColor(ButtonWidget::COLOR_LABEL_HILITE,
		Color::White);
	mGameAdvancedButton->mHiliteFont =
		FONT_DWARVENTODCRAFT18BRIGHTGREENINSET;
	mGameAdvancedButton->SetVisible(false);
	mReloadLanguagesButton =
		MakeButton(NewOptionsDialog::NewOptionsDialog_ReloadLanguages, this,
			_S("[OPTIONS_RELOAD_LANGUAGES]"));
	mReloadLanguagesButton->SetVisible(false);
	mLanguageButton = MakeNewButton(NewOptionsDialog::NewOptionsDialog_Language,
		this, _S("[LANGUAGE_NAME]"), nullptr, Sexy::IMAGE_BLANK, Sexy::IMAGE_BLANK,
		Sexy::IMAGE_BLANK);
	mLanguageButton->SetFont(FONT_DWARVENTODCRAFT18);
	mLanguageButton->mColors[ButtonWidget::COLOR_LABEL] = cTextColor;
	mLanguageButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(1, 233, 1);
	mLanguageButton->SetVisible(false);
	mReloadResourcePacksButton =
		MakeButton(NewOptionsDialog::NewOptionsDialog_ReloadResourcePacks, this,
			_S("[OPTIONS_RELOAD_RESOURCE_PACKS]"));
	mReloadResourcePacksButton->SetVisible(false);
	mResourcePackButton =
		MakeNewButton(NewOptionsDialog::NewOptionsDialog_ResourcePack, this,
			mApp->GetResourcePackString(), nullptr, Sexy::IMAGE_BLANK, Sexy::IMAGE_BLANK,
			Sexy::IMAGE_BLANK);
	mResourcePackButton->SetFont(FONT_DWARVENTODCRAFT18);
	mResourcePackButton->mColors[ButtonWidget::COLOR_LABEL] = cTextColor;
	mResourcePackButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(1,
		233, 1);
	mResourcePackButton->SetVisible(false);
	mRealHardwareAccelerationCheckbox =
		MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_Real_HardwareAcceleration,
			this, mApp->Is3DAccelerated());
	mRealHardwareAccelerationCheckbox->SetVisible(false);
	mCustomCursorCheckbox =
		MakeNewCheckbox(NewOptionsDialog::NewOptionsDialog_CustomCursor, this,
			mApp->mCustomCursor);
	mCustomCursorCheckbox->SetVisible(false);
	if (mFromGameSelector)
	{
		mRestartButton->SetVisible(false);
		mBackToGameButton->mLabel = _S("[DIALOG_BUTTON_OK]");
		if (mApp->HasFinishedAdventure() && !mApp->IsTrialStageLocked())
		{
			mBackToMainButton->SetVisible(false);
			mBackToMainButton->mLabel = _S("[CREDITS]");
		}
	}
	else
	{
		mAdvancedButton->SetVisible(false);
		mGameAdvancedButton->SetVisible(true);
	}
	if (mAdvancedMode)
	{
		mAdvancedPage = 1;
		mRestartButton->SetVisible(false);
		mAlmanacButton->SetVisible(false);
		mBackToMainButton->SetVisible(false);
		mAdvancedButton->SetVisible(false);
		mGameAdvancedButton->SetVisible(false);
		mBackToGameButton->mLabel = _S("[DIALOG_BUTTON_BACK]");
		mBackToGameButton->mId = NewOptionsDialog::NewOptionsDialog_Back;
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ICE ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN ||
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		mRestartButton->SetVisible(false);
	}
	if (mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO &&
		!mApp->mBoard->mCutScene->IsSurvivalRepick())
	{
		mRestartButton->SetVisible(false);
	}
	if (!mApp->CanShowAlmanac() ||
		mApp->mGameScene == GameScenes::SCENE_LEVEL_INTRO ||
		mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN ||
		mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM ||
		mFromGameSelector)
	{
		mAlmanacButton->SetVisible(false);
	}
	if ((!mRestartButton->mVisible || !mAlmanacButton->mVisible) && !mFromGameSelector
		&& !mAdvancedMode)
	{
		mAdvancedButton->SetVisible(true);
		mGameAdvancedButton->SetVisible(false);
	}
}
NewOptionsDialog::~NewOptionsDialog()
{
	delete mMusicVolumeSlider;
	delete mSfxVolumeSlider;
	delete mDifficultySlider;
	delete mFullscreenCheckbox;
	delete mHardwareAccelerationCheckbox;
	delete mDebugModeCheckbox;
	delete mDiscordCheckbox;
	delete m09FormatCheckbox;
	delete mBankKeybindsCheckbox;
	delete mAlmanacButton;
	delete mRestartButton;
	delete mBackToMainButton;
	delete mAdvancedButton;
	delete mBackToGameButton;
	delete mLeftPageButton;
	delete mRightPageButton;
	delete mBitsLeftButton;
	delete mBitsRightButton;
	delete mDifficultyLeftButton;
	delete mDifficultyRightButton;
	delete mSpeedEditWidget;
	delete mGameAdvancedButton;
	delete mAutoCollectSunsCheckbox;
	delete mAutoCollectCoinsCheckbox;
	delete mZombieHealthbarsCheckbox;
	delete mReloadLanguagesButton;
	delete mLanguageButton;
	delete mReloadResourcePacksButton;
	delete mResourcePackButton;
	delete mRealHardwareAccelerationCheckbox;
	delete mCustomCursorCheckbox;
}
int NewOptionsDialog::GetPreferredHeight(int theWidth)
{
	return IMAGE_OPTIONS_MENUBACK->mWidth;
}
void NewOptionsDialog::AddedToManager(Sexy::WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	AddWidget(mAlmanacButton);
	AddWidget(mRestartButton);
	AddWidget(mBackToMainButton);
	AddWidget(mAdvancedButton);
	AddWidget(mMusicVolumeSlider);
	AddWidget(mSfxVolumeSlider);
	AddWidget(mDifficultySlider);
	AddWidget(mHardwareAccelerationCheckbox);
	AddWidget(mDebugModeCheckbox);
	AddWidget(mDiscordCheckbox);
	AddWidget(mBankKeybindsCheckbox);
	AddWidget(m09FormatCheckbox);
	AddWidget(mFullscreenCheckbox);
	AddWidget(mBackToGameButton);
	AddWidget(mLeftPageButton);
	AddWidget(mRightPageButton);
	AddWidget(mBitsLeftButton);
	AddWidget(mBitsRightButton);
	AddWidget(mDifficultyLeftButton);
	AddWidget(mDifficultyRightButton);
	AddWidget(mSpeedEditWidget);
	AddWidget(mGameAdvancedButton);
	AddWidget(mAutoCollectSunsCheckbox);
	AddWidget(mAutoCollectCoinsCheckbox);
	AddWidget(mZombieHealthbarsCheckbox);
	AddWidget(mPlantHealthbarsCheckbox);
	AddWidget(mReloadLanguagesButton);
	AddWidget(mLanguageButton);
	AddWidget(mReloadResourcePacksButton);
	AddWidget(mResourcePackButton);
	AddWidget(mRealHardwareAccelerationCheckbox);
	AddWidget(mCustomCursorCheckbox);
}
void NewOptionsDialog::RemovedFromManager(Sexy::WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	mApp->mFullscreenBits = mBitsValue;
	mApp->WriteCurrentUserConfig();
	RemoveWidget(mAlmanacButton);
	RemoveWidget(mMusicVolumeSlider);
	RemoveWidget(mSfxVolumeSlider);
	RemoveWidget(mDifficultySlider);
	RemoveWidget(mFullscreenCheckbox);
	RemoveWidget(mHardwareAccelerationCheckbox);
	RemoveWidget(mDebugModeCheckbox);
	RemoveWidget(mDiscordCheckbox);
	RemoveWidget(mBankKeybindsCheckbox);
	RemoveWidget(m09FormatCheckbox);
	RemoveWidget(mBackToMainButton);
	RemoveWidget(mAdvancedButton);
	RemoveWidget(mBackToGameButton);
	RemoveWidget(mRestartButton);
	RemoveWidget(mLeftPageButton);
	RemoveWidget(mRightPageButton);
	RemoveWidget(mBitsLeftButton);
	RemoveWidget(mBitsRightButton);
	RemoveWidget(mDifficultyLeftButton);
	RemoveWidget(mDifficultyRightButton);
	RemoveWidget(mSpeedEditWidget);
	RemoveWidget(mGameAdvancedButton);
	RemoveWidget(mAutoCollectSunsCheckbox);
	RemoveWidget(mAutoCollectCoinsCheckbox);
	RemoveWidget(mZombieHealthbarsCheckbox);
	RemoveWidget(mPlantHealthbarsCheckbox);
	RemoveWidget(mReloadLanguagesButton);
	RemoveWidget(mLanguageButton);
	RemoveWidget(mReloadResourcePacksButton);
	RemoveWidget(mResourcePackButton);
	RemoveWidget(mRealHardwareAccelerationCheckbox);
	RemoveWidget(mCustomCursorCheckbox);
}

void NewOptionsDialog::SliderVal(int theId, double theVal)
{
	switch (theId)
	{
	case NewOptionsDialog::NewOptionsDialog_MusicVolume:
		mApp->SetMusicVolume(theVal);
		mApp->mSoundSystem->RehookupSoundWithMusicVolume();
		mApp->WriteCurrentUserConfig(); // Instant save
		break;
	case NewOptionsDialog::NewOptionsDialog_SoundVolume:
		mApp->SetSfxVolume(theVal);
		mApp->mSoundSystem->RehookupSoundWithMusicVolume();
		if (!mSfxVolumeSlider->mDragging)
		{
			mApp->PlaySample(SOUND_BUTTONCLICK);
		}
		mApp->WriteCurrentUserConfig(); // Instant save
		break;
	case NewOptionsDialog::NewOptionsDialog_Difficulty:
		if (theVal < 0.33)
		{

			mApp->mDifficulty = 0;
			mDifficultySlider->mVal = 0.0;
		}
		else if (theVal < 0.66)
		{
			mApp->mDifficulty = 1;
			mDifficultySlider->mVal = 0.5;
		}
		else
		{
			mApp->mDifficulty = 2;
			mDifficultySlider->mVal = 1.0;
		}
		mApp->WriteCurrentUserConfig(); // Instant save
		break;
	}
}
void NewOptionsDialog::CheckboxChecked(int theId, bool checked)
{
	switch (theId)
	{
	case NewOptionsDialog::NewOptionsDialog_Fullscreen:
		if (!checked && mApp->mForceFullscreen)
		{
			mApp->DoDialog(
				Dialogs::DIALOG_COLORDEPTH_EXP,
				true,
				_S("[NO_WINDOWED_MODE_HEADER]"),
				_S("[NO_WINDOWED_MODE]"),
				_S("[DIALOG_BUTTON_OK]"),
				Dialog::BUTTONS_FOOTER
			);
			mFullscreenCheckbox->SetChecked(true, false);
		}
		break;
	case NewOptionsDialog::NewOptionsDialog_HardwareAcceleration:
	case NewOptionsDialog::NewOptionsDialog_Real_HardwareAcceleration:
		if (checked)
		{
			if (!mApp->Is3DAccelerationRecommended())

			{
				mApp->DoDialog(
					Dialogs::DIALOG_INFO,
					true,
					_S("[NOT_RECOMMENDED_ACCELERATION_HEADER]"),
					_S("[NOT_RECOMMENDED_ACCELERATION]"),
					_S("[DIALOG_BUTTON_OK]"),
					Dialog::BUTTONS_FOOTER
				);
			}
		}
		break;
	}
}
void NewOptionsDialog::KeyDown(Sexy::KeyCode theKey)
{
	if (mApp->mBoard)
	{
		mApp->mBoard->DoTypingCheck(theKey);
	}
	if (theKey == KeyCode::KEYCODE_SPACE || theKey ==
		KeyCode::KEYCODE_RETURN)
	{
		if (mAdvancedMode)
			ButtonDepress(NewOptionsDialog::NewOptionsDialog_Back);
		else
		{
			Dialog::ButtonDepress(Dialog::ID_OK);
			mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		}
	}
	else if (theKey == KeyCode::KEYCODE_ESCAPE)
	{
		Dialog::ButtonDepress(Dialog::ID_CANCEL);
	}
}
void NewOptionsDialog::UpdateAdvancedPage()
{
	if (mAdvancedPage == 1)
		mLeftPageButton->SetVisible(false);
	else
		mLeftPageButton->SetVisible(true);

	// تأكد من أن قيمة ADVANCEDOPTIONS_MAX_PAGES في ملف الثوابت الخاص بك أصبحت 3 بدلاً من 4
	if (mAdvancedPage == ADVANCEDOPTIONS_MAX_PAGES)
		mRightPageButton->SetVisible(false);
	else
		mRightPageButton->SetVisible(true);

	mDebugModeCheckbox->SetVisible(false);
	mDiscordCheckbox->SetVisible(false);
	mBankKeybindsCheckbox->SetVisible(false);
	m09FormatCheckbox->SetVisible(false);
	mSpeedEditWidget->SetVisible(false);
	mAutoCollectSunsCheckbox->SetVisible(false);
	mAutoCollectCoinsCheckbox->SetVisible(false);
	mZombieHealthbarsCheckbox->SetVisible(false);
	mPlantHealthbarsCheckbox->SetVisible(false);
	mReloadLanguagesButton->SetVisible(false); // ستبقى مخفية دائماً
	mLanguageButton->SetVisible(false); // ستبقى مخفية دائماً
	mReloadResourcePacksButton->SetVisible(false); // ستبقى مخفية دائماً
	mResourcePackButton->SetVisible(false); // ستبقى مخفية دائماً
	mRealHardwareAccelerationCheckbox->SetVisible(false);
	mCustomCursorCheckbox->SetVisible(false);
	mBitsLeftButton->SetVisible(false);
	mBitsRightButton->SetVisible(false);
	mDifficultySlider->SetVisible(false);
	mDifficultyLeftButton->SetVisible(false);
	mDifficultyRightButton->SetVisible(false);

	switch (mAdvancedPage)
	{
	case 1:
		mDebugModeCheckbox->SetVisible(true);
		mDiscordCheckbox->SetVisible(true);
		mBankKeybindsCheckbox->SetVisible(true);
		m09FormatCheckbox->SetVisible(true);
		break;
	case 2:
		mSpeedEditWidget->SetVisible(true);
		mAutoCollectSunsCheckbox->SetVisible(true);
		mAutoCollectCoinsCheckbox->SetVisible(true);
		mZombieHealthbarsCheckbox->SetVisible(true);
		mPlantHealthbarsCheckbox->SetVisible(true);
		break;
	case 3: // تم دمج عناصر الصفحة 4 هنا لتصبح الصفحة 3 الأخيرة
		mRealHardwareAccelerationCheckbox->SetVisible(true);
		mCustomCursorCheckbox->SetVisible(true);
		if (mApp->mDifficulty == 0) // سهل
		{
			mDifficultyLeftButton->SetVisible(false);
			mDifficultyRightButton->SetVisible(true);
		}
		else if (mApp->mDifficulty == 1) // متوسط
		{
			mDifficultyLeftButton->SetVisible(true);
			mDifficultyRightButton->SetVisible(true);
		}
		else // صعب
		{
			mDifficultyLeftButton->SetVisible(true);
			mDifficultyRightButton->SetVisible(false); // مخفي لأنه لا يوجد مستوى بعد الصعب
		}
		break;
	}
}
void NewOptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);
	mMusicVolumeSlider->Resize(199, 115, 135, 40);
	mSfxVolumeSlider->Resize(199, 147, 135, 40);
	mHardwareAccelerationCheckbox->Resize(284, 195, 46, 39);
	mFullscreenCheckbox->Resize(284, 220, 46, 39);
	mAlmanacButton->Resize(107, 255, 209, 46);
	mRestartButton->Resize(mAlmanacButton->mX, mAlmanacButton->mY + 43, 209, 46);
	mBackToMainButton->Resize(mRestartButton->mX, mRestartButton->mY + 43, 209, 46);
	mAdvancedButton->Resize(mRestartButton->mX, mRestartButton->mY + 43, 209, 46);
	mBackToGameButton->Resize(30, 381, mBackToGameButton->mWidth, mBackToGameButton->mHeight);
	mLeftPageButton->Resize(100, ADVANCEDOPTIONS_PAGE_Y - 25, IMAGE_QUICKPLAY_LEFT_BUTTON->mWidth, IMAGE_QUICKPLAY_LEFT_BUTTON->mHeight);
	mRightPageButton->Resize(280, ADVANCEDOPTIONS_PAGE_Y - 25, IMAGE_QUICKPLAY_RIGHT_BUTTON->mWidth, IMAGE_QUICKPLAY_RIGHT_BUTTON->mHeight);
	mGameAdvancedButton->Resize(mWidth - Sexy::IMAGE_BUTTON_SMALL->mWidth - 9, mRestartButton->mY, Sexy::IMAGE_BUTTON_SMALL->mWidth, Sexy::IMAGE_BUTTON_SMALL->mHeight);

	mDebugModeCheckbox->Resize(284, 148, 46, 39);
	mDiscordCheckbox->Resize(mDebugModeCheckbox->mX, mDebugModeCheckbox->mY + 40, 46, 39);
	mBankKeybindsCheckbox->Resize(mDiscordCheckbox->mX, mDiscordCheckbox->mY + 40, 46, 39);
	m09FormatCheckbox->Resize(mBankKeybindsCheckbox->mX, mBankKeybindsCheckbox->mY + 40, 46, 39);

	mSpeedEditWidget->Resize(ADVANCEDOPTIONS_SPEED_X + 6, ADVANCEDOPTIONS_SPEED_Y + 5, 28, 24);
	mAutoCollectSunsCheckbox->Resize(mDiscordCheckbox->mX, mDiscordCheckbox->mY - 20, 46, 39);
	mAutoCollectCoinsCheckbox->Resize(mAutoCollectSunsCheckbox->mX, mAutoCollectSunsCheckbox->mY + 40, 46, 39);
	mZombieHealthbarsCheckbox->Resize(mAutoCollectCoinsCheckbox->mX, mAutoCollectCoinsCheckbox->mY + 40, 46, 39);
	mPlantHealthbarsCheckbox->Resize(mZombieHealthbarsCheckbox->mX, mZombieHealthbarsCheckbox->mY + 40, 46, 39);

	// أزرار الصفحة 3 القديمة - تركناها مخفية ونحتفظ بأحجامها لتجنب الأخطاء
	int aReloadResourcePacksWidth = 260;
	mReloadResourcePacksButton->Resize(mWidth / 2 - aReloadResourcePacksWidth / 2, ADVANCEDOPTIONS_SPEED_Y - 10, aReloadResourcePacksWidth, 46);
	mResourcePackButton->Resize(0, mReloadResourcePacksButton->mY + 52, 0, FONT_DWARVENTODCRAFT18->GetHeight());
	ResizeResourcePackButton();
	int aReloadLanguagesWidth = 225;
	mReloadLanguagesButton->Resize(mWidth / 2 - aReloadLanguagesWidth / 2, mResourcePackButton->mY + 35, aReloadLanguagesWidth, 46);
	mLanguageButton->Resize(0, mReloadLanguagesButton->mY + 52, 0, FONT_DWARVENTODCRAFT18->GetHeight());
	ResizeLanguageButton();

	mRealHardwareAccelerationCheckbox->Resize(ADVANCEDOPTIONS_SPEED_X, ADVANCEDOPTIONS_SPEED_Y, 46, 39);
	mCustomCursorCheckbox->Resize(mRealHardwareAccelerationCheckbox->mX, mRealHardwareAccelerationCheckbox->mY + 40, 46, 39);

	mDifficultySlider->Resize(138, mCustomCursorCheckbox->mY + 65, 135, 40);
	// تم إضافة 25 بكسل لتنزيل العناصر عن Custom Cursor (أصبحت +70 بدلاً من +45)
	int aDiffY = mCustomCursorCheckbox->mY + 70;
	int aBtnWidth = IMAGE_QUICKPLAY_LEFT_BUTTON->mWidth;
	int aBtnHeight = IMAGE_QUICKPLAY_LEFT_BUTTON->mHeight;

	mDifficultyLeftButton->Resize(100, aDiffY, aBtnWidth, aBtnHeight);
	mDifficultyRightButton->Resize(280, aDiffY, aBtnWidth, aBtnHeight);
	mDifficultySlider->Resize(100, aDiffY, 212, aBtnHeight);

	mBitsLeftButton->Resize(-1000, -1000, IMAGE_QUICKPLAY_LEFT_BUTTON->mWidth, IMAGE_QUICKPLAY_LEFT_BUTTON->mHeight);
	mBitsRightButton->Resize(-1000, -1000, IMAGE_QUICKPLAY_RIGHT_BUTTON->mWidth, IMAGE_QUICKPLAY_RIGHT_BUTTON->mHeight);

	if ((!mRestartButton->mVisible || !mAlmanacButton->mVisible) && !mFromGameSelector && !mAdvancedMode)
	{
		LawnStoneButton* button;
		if (!mRestartButton->mVisible)
			button = mRestartButton;
		else if (!mAlmanacButton->mVisible)
			button = mAlmanacButton;
		mAdvancedButton->Resize(button->mX, button->mY, button->mWidth, button->mHeight);
	}
	if (mFromGameSelector)
	{
		mMusicVolumeSlider->mY += 5;
		mSfxVolumeSlider->mY += 10;
		mHardwareAccelerationCheckbox->mY += 20;
		mFullscreenCheckbox->mY += 25;
	}
	if (mAdvancedMode)
	{
		mMusicVolumeSlider->SetVisible(false);
		mSfxVolumeSlider->SetVisible(false);
		mHardwareAccelerationCheckbox->SetVisible(false);
		mFullscreenCheckbox->SetVisible(false);
		mLeftPageButton->SetVisible(true);
		mRightPageButton->SetVisible(true);
		UpdateAdvancedPage();
	}
	if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN || mApp->mGameMode == GameMode::GAMEMODE_TREE_OF_WISDOM)
	{
		mAlmanacButton->mY += 43;

	}
}
void NewOptionsDialog::Draw(Sexy::Graphics* g)
{
	g->DrawImage(IMAGE_OPTIONS_MENUBACK, 0, 0);

	int aMusicOffset = 0;
	int aSfxOffset = 0;
	int a3DAccelOffset = 0;
	int aFullScreenOffset = 0;
	if (mFromGameSelector)
	{
		aMusicOffset = 5;
		aSfxOffset = 10;
		a3DAccelOffset = 20;
		aFullScreenOffset = 25;
	}
	if (!mAdvancedMode)
	{
		TodDrawString(g, TodStringTranslate(_S("[OPTIONS_MUSIC]")), 186, 139 + aMusicOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
		TodDrawString(g, to_string((int)(mMusicVolumeSlider->mVal * 100)), 203 + mMusicVolumeSlider->mWidth, 142 + aMusicOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, TodStringTranslate(_S("[OPTIONS_SOUND]")), 186, 171 + aSfxOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
		TodDrawString(g, to_string((int)(mSfxVolumeSlider->mVal * 100)), 203 + mSfxVolumeSlider->mWidth, 174 + aSfxOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_LEFT);
		TodDrawString(g, TodStringTranslate(_S("[OPTIONS_ACCELERATION]")), 274, 217 + a3DAccelOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
		TodDrawString(g, TodStringTranslate(_S("[OPTIONS_FULLSCREEN]")), 274, 242 + aFullScreenOffset, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
	}
	else
	{
		switch (mAdvancedPage)
		{
		case 1:
			TodDrawString(g, mApp->mReconVersion, mWidth / 2, 137, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_CENTER);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_DEBUG_MODE]")), mDebugModeCheckbox->mX - 6, mDebugModeCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_DISCORD_PRESENCE]")), mDiscordCheckbox->mX - 6, mDiscordCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_SEED_BANK_KEYBINDS]")), mBankKeybindsCheckbox->mX - 6, mBankKeybindsCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodReplaceString(_S("[OPTIONS_SEED_BANK_KEYBIND]"), _S("{KEYBIND}"), m09FormatCheckbox->mChecked ? "1-0" : "0-9"), m09FormatCheckbox->mX - 6, m09FormatCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_SHOVEL_KEYBIND]")), mWidth / 2, m09FormatCheckbox->mY + 55, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_CENTER);
			break;
		case 2:
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_SPEED_MODIFIER]")), ADVANCEDOPTIONS_SPEED_X - 6, ADVANCEDOPTIONS_SPEED_Y + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_AUTO_COLLECT_SUNS]")), mAutoCollectSunsCheckbox->mX - 6, mAutoCollectSunsCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_AUTO_COLLECT_COINS]")), mAutoCollectCoinsCheckbox->mX - 6, mAutoCollectCoinsCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_ZOMBIE_HEALTHBARS]")), mZombieHealthbarsCheckbox->mX - 6, mZombieHealthbarsCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_PLANT_HEALTHBARS]")), mPlantHealthbarsCheckbox->mX - 6, mPlantHealthbarsCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			g->DrawImage(Sexy::IMAGE_OPTIONS_CHECKBOX0, ADVANCEDOPTIONS_SPEED_X, ADVANCEDOPTIONS_SPEED_Y, 40, 36);
			break;
		case 3: // تم حذف نصوص اللغات والحزم بالكامل ونقل نصوص الصفحة 4 إلى هنا
		{
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_ACTUAL_ACCELERATION]")), mRealHardwareAccelerationCheckbox->mX - 6, mRealHardwareAccelerationCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);
			TodDrawString(g, TodStringTranslate(_S("[OPTIONS_CUSTOM_CURSOR]")), mCustomCursorCheckbox->mX - 6, mCustomCursorCheckbox->mY + 22, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_RIGHT);

			// منتصف النافذة تماماً
			int aCenterX = mWidth / 2;

			// عنوان الخيار فوق الأزرار في المنتصف
			TodDrawString(g, _S("Difficulty Level:"), aCenterX, mDifficultyLeftButton->mY - 12, FONT_DWARVENTODCRAFT18, cTextColor, DrawStringJustification::DS_ALIGN_CENTER);

			// تحديد مستوى الصعوبة ولونه
			SexyString diffText;
			Color diffColor;
			if (mApp->mDifficulty == 0) {
				diffText = _S("EASY");
				diffColor = Color(0, 255, 0);
			}
			else if (mApp->mDifficulty == 1) {
				diffText = _S("MEDIUM");
				diffColor = Color(255, 255, 0);
			}
			else {
				diffText = _S("HARD");
				diffColor = Color(255, 0, 0);
			}

			// رسم نص الصعوبة في المنتصف تماماً بين الزرين
			int aTextY = mDifficultyLeftButton->mY + (mDifficultyLeftButton->mHeight / 2) + 5;
			TodDrawString(g, diffText, aCenterX, aTextY, FONT_BRIANNETOD16, diffColor, DrawStringJustification::DS_ALIGN_CENTER);
		}
		}
		TodDrawString(g, TodReplaceNumberString(_S("[OPTIONS_PAGE]"), _S("{PAGE}"), mAdvancedPage), mWidth / 2, ADVANCEDOPTIONS_PAGE_Y, FONT_DWARVENTODCRAFT18GREENINSET, Color::White, DrawStringJustification::DS_ALIGN_CENTER);
	}
}
void NewOptionsDialog::ButtonDepress(int theId)
{
	Dialog::ButtonDepress(theId);
	switch (theId)
	{
	case NewOptionsDialog::NewOptionsDialog_BitsLeft:
	case NewOptionsDialog::NewOptionsDialog_BitsRight:
		break;
	case NewOptionsDialog::NewOptionsDialog_DifficultyLeft:
		if (mApp->mDifficulty == 1)
			mApp->mDifficulty = 0;
		else if (mApp->mDifficulty == 2)
			mApp->mDifficulty = 1;
		mApp->WriteCurrentUserConfig();
		mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		UpdateAdvancedPage();
		break;
	case NewOptionsDialog::NewOptionsDialog_DifficultyRight:
		if (mApp->mDifficulty == 0)
			mApp->mDifficulty = 1;
		else if (mApp->mDifficulty == 1)
			mApp->mDifficulty = 2;
		else if (mApp->mDifficulty == 2)
			mApp->mDifficulty = 0;
		mApp->WriteCurrentUserConfig();
		mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		UpdateAdvancedPage();
		break;
	case NewOptionsDialog::NewOptionsDialog_Almanac:
	{
		AlmanacDialog* aDialog = mApp->DoAlmanacDialog(SeedType::SEED_NONE, ZombieType::ZOMBIE_INVALID);
		aDialog->WaitForResult(true);
		break;
	}
	case NewOptionsDialog::NewOptionsDialog_Advanced:
	{
		mApp->KillNewOptionsDialog();
		mApp->DoAdvancedOptions(mFromGameSelector, mX, mY);
		mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		break;
	}
	case NewOptionsDialog::NewOptionsDialog_MainMenu:
	{
		if (mApp->mBoard && mApp->mBoard->NeedSaveGame())
		{
			mApp->DoConfirmBackToMain();
		}
		else if (mApp->mBoard && mApp->mBoard->mCutScene && mApp->mBoard->mCutScene->IsSurvivalRepick())
		{
			mApp->DoConfirmBackToMain();
		}
		else
		{
			mApp->mBoardResult = BoardResult::BOARDRESULT_QUIT;
			mApp->DoBackToMain(true);
		}
		break;
	}
	case NewOptionsDialog::NewOptionsDialog_Restart:
	{
		if (mApp->mBoard)
		{
			SexyString aDialogTitle;
			SexyString aDialogMessage;
			if (mApp->IsPuzzleMode())
			{
				aDialogTitle = _S("[RESTART_PUZZLE_HEADER]");
				aDialogMessage = _S("[RESTART_PUZZLE_BODY]");
			}
			else if (mApp->IsChallengeMode())
			{
				aDialogTitle = _S("[RESTART_CHALLENGE_HEADER]");
				aDialogMessage = _S("[RESTART_CHALLENGE_BODY]");
			}
			else if (mApp->IsSurvivalMode())
			{
				aDialogTitle = _S("[RESTART_SURVIVAL_HEADER]");
				aDialogMessage = _S("[RESTART_SURVIVAL_BODY]");
			}
			else
			{
				aDialogTitle = _S("[RESTART_LEVEL_HEADER]");
				aDialogMessage = _S("[RESTART_LEVEL_BODY]");
			}
			LawnDialog* aDialog = (LawnDialog*)mApp->DoDialog(Dialogs::DIALOG_CONFIRM_RESTART, true, aDialogTitle, aDialogMessage, _S(""), Dialog::BUTTONS_YES_NO);
			aDialog->mLawnYesButton->mLabel = _S("[RESTART_BUTTON]");
			aDialog->mLawnNoButton->mLabel = _S("[DIALOG_BUTTON_CANCEL]");
			if (aDialog->WaitForResult(true) == Dialog::ID_YES)
			{
				mApp->mMusic->StopAllMusic();
				mApp->mSoundSystem->CancelPausedFoley();
				mApp->KillNewOptionsDialog();
				mApp->mBoardResult = BoardResult::BOARDRESULT_RESTART;
				mApp->mSawYeti = mApp->mBoard->mKilledYeti;
				if (mApp->mPlayingQuickplay)
					mApp->StartQuickPlay();
				else
					mApp->PreNewGame(mApp->mGameMode, false);

				mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
			}
		}
		break;
	}
	case NewOptionsDialog::NewOptionsDialog_ReloadLanguages:
	{
		mApp->ReloadLanguages();
		bool aResetLanguage = true;
		for (std::map<std::string, StringWStringMap>::iterator aIt = mApp->mLanguages.begin(); aIt != mApp->mLanguages.end(); ++aIt)
		{
			if (mApp->mLanguage == aIt->first)
			{
				aResetLanguage = false;
				break;
			}
		}
		if (aResetLanguage)
		{
			mApp->mLanguage = mApp->mLanguages.begin()->first;
			mApp->mLanguageIndex = 0;
		}
		mApp->LoadCurrentLanguage();
		ResizeLanguageButton();
		break;
	}
	case NewOptionsDialog::NewOptionsDialog_Language:
		mApp->SwitchLanguage();
		ResizeLanguageButton();
		break;
	case NewOptionsDialog::NewOptionsDialog_ReloadResourcePacks:
		mApp->ReloadResourcePacks();
		mResourcePackButton->mLabel = mApp->GetResourcePackString();
		ResizeResourcePackButton();
		break;
	case NewOptionsDialog::NewOptionsDialog_ResourcePack:
		mApp->SwitchResourcePack();
		mResourcePackButton->mLabel = mApp->GetResourcePackString();
		ResizeResourcePackButton();
		break;
	case NewOptionsDialog::NewOptionsDialog_LeftPage:
		mAdvancedPage--;
		UpdateAdvancedPage();
		break;
	case NewOptionsDialog::NewOptionsDialog_RightPage:
		mAdvancedPage++;
		UpdateAdvancedPage();
		break;
	case NewOptionsDialog::NewOptionsDialog_Back:
		mApp->KillNewOptionsDialog();
		mApp->DoNewOptions(mFromGameSelector, mX, mY);
		mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		break;
	case Dialog::ID_OK:
		mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
		break;
	}
}
void NewOptionsDialog::Update()
{
	bool isGameAdvancedDown = mGameAdvancedButton->mIsDown;
	mGameAdvancedButton->mTextDownOffsetX = isGameAdvancedDown;
	mGameAdvancedButton->mTextDownOffsetY = isGameAdvancedDown;
	if (mAdvancedMode)
	{
		if (mSpeedEditWidget->mHasFocus && mSpeedEditWidget->mFont !=
			FONT_DWARVENTODCRAFT18BRIGHTGREENINSET)
			mSpeedEditWidget->SetFont(FONT_DWARVENTODCRAFT18BRIGHTGREENINSET);
		if (mSpeedEditPrevText != mSpeedEditWidget->mString)
		{
			if ((mSpeedEditWidget->mString == "" || mSpeedEditWidget->mString == " ") &&
				(mSpeedEditPrevText != "" || mSpeedEditPrevText != " "))
				mSpeedEditWidget->mString = mSpeedEditPrevText;
			int num;
			try
			{
				num = stoi(mSpeedEditWidget->mString);
			}
			catch (exception)
			{
				mSpeedEditWidget->mString = mSpeedEditPrevText;
				return;
			}
			if (num < SPEED_MODIFIER_MIN)
				mSpeedEditWidget->mString = to_string(SPEED_MODIFIER_MIN);
			else if (num > SPEED_MODIFIER_MAX)
				mSpeedEditWidget->mString = to_string(SPEED_MODIFIER_MAX);
			mSpeedEditPrevText = mSpeedEditWidget->mString;

		}
	}
}
void NewOptionsDialog::ResizeLanguageButton()
{
	int aWidth = mLanguageButton->mFont->StringWidth(TodStringTranslate(mLanguageButton->mLabel));
	mLanguageButton->Resize(mWidth / 2 - aWidth / 2, mLanguageButton->mY,
		aWidth, mLanguageButton->mHeight);
}
void NewOptionsDialog::ResizeResourcePackButton()
{
	int aWidth = mResourcePackButton->mFont->StringWidth(TodStringTranslate(mResourcePackButton->mLabel));
	mResourcePackButton->Resize(mWidth / 2 - aWidth / 2,
		mResourcePackButton->mY,
		aWidth, mResourcePackButton->mHeight);
}
void NewOptionsDialog::ButtonPress(int theId)
{
	mApp->PlaySample(SOUND_GRAVEBUTTON);
}
