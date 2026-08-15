#ifndef __NEWOPTIONSDIALOG_H__
#define __NEWOPTIONSDIALOG_H__
#include "../../SexyAppFramework/Dialog.h"
#include "../../SexyAppFramework/SliderListener.h"
#include "../../SexyAppFramework/CheckboxListener.h"
#include "../../SexyAppFramework/EditListener.h"
class LawnApp;
class LawnStoneButton;
class NewLawnButton;
namespace Sexy
{
	class Slider;
	class Checkbox;
};
class NewOptionsDialog : public Sexy::Dialog, public Sexy::SliderListener, public
	Sexy::CheckboxListener, public EditListener
{
protected:
	enum
	{
		NewOptionsDialog_Almanac,
		NewOptionsDialog_MainMenu,
		NewOptionsDialog_Restart,
		NewOptionsDialog_MusicVolume,
		NewOptionsDialog_SoundVolume,
		NewOptionsDialog_Difficulty,
		NewOptionsDialog_Fullscreen,
		NewOptionsDialog_HardwareAcceleration,
		NewOptionsDialog_Advanced,
		NewOptionsDialog_ReloadLanguages,
		NewOptionsDialog_Language,
		NewOptionsDialog_ReloadResourcePacks,
		NewOptionsDialog_ResourcePack,
		NewOptionsDialog_Real_HardwareAcceleration,
		NewOptionsDialog_CustomCursor,
		NewOptionsDialog_LeftPage,
		NewOptionsDialog_RightPage,
		NewOptionsDialog_BitsLeft,
		NewOptionsDialog_BitsRight,
		NewOptionsDialog_Back,
		NewOptionsDialog_DebugMode,
		NewOptionsDialog_Discord,
		NewOptionsDialog_BankKeybinds,
		NewOptionsDialog_09Format,
		NewOptionsDialog_AutoCollectSuns,
		NewOptionsDialog_AutoCollectCoins,
		NewOptionsDialog_ZombieHealthbars,
		NewOptionsDialog_PlantHealthbars,
		NewOptionsDialog_DifficultyLeft,
		NewOptionsDialog_DifficultyRight
	};
public:
	LawnApp* mApp;
	Sexy::Slider* mMusicVolumeSlider;
	Sexy::Slider* mSfxVolumeSlider;
	Sexy::Slider* mDifficultySlider;
	Sexy::Checkbox* mFullscreenCheckbox;
	Sexy::Checkbox* mHardwareAccelerationCheckbox;
	Sexy::Checkbox* mDebugModeCheckbox;
	Sexy::Checkbox* mDiscordCheckbox;
	Sexy::Checkbox* mBankKeybindsCheckbox;
	Sexy::Checkbox* m09FormatCheckbox;
	Sexy::Checkbox* mAutoCollectSunsCheckbox;
	Sexy::Checkbox* mAutoCollectCoinsCheckbox;
	Sexy::Checkbox* mZombieHealthbarsCheckbox;
	Sexy::Checkbox* mPlantHealthbarsCheckbox;
	LawnStoneButton* mAlmanacButton;
	LawnStoneButton* mBackToMainButton;
	LawnStoneButton* mRestartButton;
	NewLawnButton* mBackToGameButton;
	LawnStoneButton* mAdvancedButton;
	NewLawnButton* mGameAdvancedButton;
	NewLawnButton* mRightPageButton;
	NewLawnButton* mLeftPageButton;
	NewLawnButton* mBitsLeftButton;
	NewLawnButton* mBitsRightButton;
	NewLawnButton* mDifficultyLeftButton;
	NewLawnButton* mDifficultyRightButton;
	bool mFromGameSelector;
	bool mAdvancedMode;
	int mAdvancedPage;
	int mBitsValue;
	EditWidget* mSpeedEditWidget;
	SexyString mSpeedEditPrevText;
	LawnStoneButton* mReloadLanguagesButton;
	NewLawnButton* mLanguageButton;
	LawnStoneButton* mReloadResourcePacksButton;
	NewLawnButton* mResourcePackButton;
	Sexy::Checkbox* mRealHardwareAccelerationCheckbox;
	Sexy::Checkbox* mCustomCursorCheckbox;
public:
	NewOptionsDialog(LawnApp* theApp, bool theFromGameSelector, bool theAdvanced);
	~NewOptionsDialog();
	int GetPreferredHeight(int theWidth);
	void AddedToManager(Sexy::WidgetManager* theWidgetManager);
	void RemovedFromManager(Sexy::WidgetManager* theWidgetManager);
	void Resize(int theX, int theY, int theWidth, int theHeight);
	void Draw(Sexy::Graphics* g);
	void SliderVal(int theId, double theVal);
	void CheckboxChecked(int theId, bool checked);
	void ButtonPress(int theId);
	void ButtonDepress(int theId);
	void KeyDown(Sexy::KeyCode theKey);
	void UpdateAdvancedPage();
	void Update();
	void ResizeLanguageButton();
	void ResizeResourcePackButton();
};
#endif