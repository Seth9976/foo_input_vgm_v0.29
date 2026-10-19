#pragma warning(disable : 4996)

#include "resource.h"
#include "stdafx.h"
#include "../helpers/helpers.h"

// {DEF9B447-931D-42D1-BD4C-1D3D2163886B}
const GUID guid_preferences_page_foo_input_vgm =
{ 0xdef9b447, 0x931d, 0x42d1, { 0xbd, 0x4c, 0x1d, 0x3d, 0x21, 0x63, 0x88, 0x6b } };
// {CF4C538B-28F9-4221-84F5-3B6D4FA3C4E7}
static const GUID guid_cfg_chip_rate =
{ 0xcf4c538b, 0x28f9, 0x4221, { 0x84, 0xf5, 0x3b, 0x6d, 0x4f, 0xa3, 0xc4, 0xe7 } };
// {84D6CF01-069A-49D2-A0D2-71669B7792C6}
static const GUID guid_cfg_chip_sample_mode =
{ 0x84d6cf01, 0x69a, 0x49d2, { 0xa0, 0xd2, 0x71, 0x66, 0x9b, 0x77, 0x92, 0xc6 } };
// {045CF322-A60B-4B82-9AC0-D52926393F2E}
static const GUID guid_cfg_loop_count =
{ 0x45cf322, 0xa60b, 0x4b82, { 0x9a, 0xc0, 0xd5, 0x29, 0x26, 0x39, 0x3f, 0x2e } };
// {1BBE5F9B-6081-4001-B236-8F1C92C6CAF4}
static const GUID guid_cfg_playback_rate =
{ 0x1bbe5f9b, 0x6081, 0x4001, { 0xb2, 0x36, 0x8f, 0x1c, 0x92, 0xc6, 0xca, 0xf4 } };
// {8B81C25F-314C-4B0B-98D2-D4296124BA76}
static const GUID guid_cfg_combo_playback_rate =
{ 0x8b81c25f, 0x314c, 0x4b0b, { 0x98, 0xd2, 0xd4, 0x29, 0x61, 0x24, 0xba, 0x76 } };
// {06D91B0B-1DF0-44B9-A47A-199FF016B899}
static const GUID guid_cfg_edit_playback_rate =
{ 0x6d91b0b, 0x1df0, 0x44b9, { 0xa4, 0x7a, 0x19, 0x9f, 0xf0, 0x16, 0xb8, 0x99 } };
// {C15330FB-3AB7-4D9F-B090-F6643D27D646}
static const GUID  guid_cfg_fade_length =
{ 0xc15330fb, 0x3ab7, 0x4d9f, { 0xb0, 0x90, 0xf6, 0x64, 0x3d, 0x27, 0xd6, 0x46 } };
// {87B12281-F57F-4F55-8E09-88D340818611}
static const GUID guid_cfg_pause_looping =
{ 0x87b12281, 0xf57f, 0x4f55, { 0x8e, 0x9, 0x88, 0xd3, 0x40, 0x81, 0x86, 0x11 } };
// {CAB0CEB8-E0C6-465D-8408-889205A10E5D}
static const GUID guid_cfg_pause_non_looping =
{ 0xcab0ceb8, 0xe0c6, 0x465d, { 0x84, 0x8, 0x88, 0x92, 0x5, 0xa1, 0xe, 0x5d } };
// {2CF6D337-98EC-454C-AD6D-BAE013CA563C}
static const GUID guid_cfg_prefer_jap_tag =
{ 0x2cf6d337, 0x98ec, 0x454c, { 0xad, 0x6d, 0xba, 0xe0, 0x13, 0xca, 0x56, 0x3c } };
// {0B5A9809-FF1B-4563-8BC1-76BDD39092FA}
static const GUID guid_cfg_display_both_tag =
{ 0xb5a9809, 0xff1b, 0x4563, { 0x8b, 0xc1, 0x76, 0xbd, 0xd3, 0x90, 0x92, 0xfa } };
// {87F9FBC2-D2EC-4BE6-A6D1-1926C899E9BB}
static const GUID guid_cfg_resampling_mode =
{ 0x87f9fbc2, 0xd2ec, 0x4be6, { 0xa6, 0xd1, 0x19, 0x26, 0xc8, 0x99, 0xe9, 0xbb } };
// {1C687E96-81D0-4DE8-B8F1-C7C3DFC234DB}
static const GUID guid_cfg_sample_rate =
{ 0x1c687e96, 0x81d0, 0x4de8, { 0xb8, 0xf1, 0xc7, 0xc3, 0xdf, 0xc2, 0x34, 0xdb } };
// {01C60CDC-5490-43DA-BE15-12D33A60F872}
static const GUID guid_cfg_surround_sound =
{ 0x1c60cdc, 0x5490, 0x43da, { 0xbe, 0x15, 0x12, 0xd3, 0x3a, 0x60, 0xf8, 0x72 } };
// {A130022F-DC64-4577-8A86-72A17E99080E}
static const GUID guid_cfg_vgm =
{ 0xa130022f, 0xdc64, 0x4577, { 0x8a, 0x86, 0x72, 0xa1, 0x7e, 0x99, 0x8, 0xe } };
// {E6FCA799-C5B7-46A2-9C0F-88358DBB40FD}
static const GUID guid_cfg_volume =
{ 0xe6fca799, 0xc5b7, 0x46a2, { 0x9c, 0xf, 0x88, 0x35, 0x8d, 0xbb, 0x40, 0xfd } };
// {97811C08-A662-4F1D-9A73-289A0C3EF4B0}
static const GUID guid_cfg_double_ssg_volume =
{ 0x97811c08, 0xa662, 0x4f1d, { 0x9a, 0x73, 0x28, 0x9a, 0xc, 0x3e, 0xf4, 0xb0 } };
// {844C969D-4C82-4D21-BA2A-5071322CD97D}
static const GUID guid_cfg_recomp_level =
{ 0x844c969d, 0x4c82, 0x4d21, { 0xba, 0x2a, 0x50, 0x71, 0x32, 0x2c, 0xd9, 0x7d } };
// {AA55EE32-58B0-4EC6-B76F-2BDE9E174191}
static const GUID guid_cfg_guess_track_number =
{ 0xaa55ee32, 0x58b0, 0x4ec6, { 0xb7, 0x6f, 0x2b, 0xde, 0x9e, 0x17, 0x41, 0x91 } };
// {98611FA9-91AD-4847-AC0D-991235EE0994}
static const GUID guid_cfg_hard_stop_old_vgms =
{ 0x98611fa9, 0x91ad, 0x4847, { 0xac, 0xd, 0x99, 0x12, 0x35, 0xee, 0x9, 0x94 } };


cfg_int cfg_resampling_mode(guid_cfg_resampling_mode, 0);
cfg_int cfg_chip_sample_mode(guid_cfg_chip_sample_mode, 0);
cfg_int cfg_combo_playback_rate(guid_cfg_combo_playback_rate, 0);
cfg_int cfg_playback_rate(guid_cfg_playback_rate, 0);

cfg_int cfg_sample_rate(guid_cfg_sample_rate, 44100);
cfg_int cfg_chip_rate(guid_cfg_chip_rate, 0);
cfg_int cfg_edit_playback_rate(guid_cfg_edit_playback_rate, 0);

cfg_int cfg_fade_length(guid_cfg_fade_length, 5000);
cfg_int cfg_loop_count(guid_cfg_loop_count, 2);
cfg_int cfg_pause_looping(guid_cfg_pause_looping, 0);
cfg_int cfg_pause_non_looping(guid_cfg_pause_non_looping, 0);
cfg_int cfg_hard_stop_old_vgms(guid_cfg_hard_stop_old_vgms, 0);

cfg_int cfg_surround_sound(guid_cfg_surround_sound, 0);
cfg_int cfg_double_ssg_volume(guid_cfg_double_ssg_volume, 1);
cfg_int cfg_volume(guid_cfg_volume, 100);
cfg_int cfg_prefer_jap_tag(guid_cfg_prefer_jap_tag, 0);
cfg_int cfg_display_both_tag(guid_cfg_display_both_tag, 0);
cfg_int cfg_guess_track_number(guid_cfg_guess_track_number, 0);
cfg_int cfg_recomp_level(guid_cfg_recomp_level, 6);

cfg_int cfg_vgm(guid_cfg_vgm, 1);


class preferences_page_instance_foo_input_vgm : public preferences_page_instance
{
protected:
	HWND m_parent;
	HWND m_hwnd;
	preferences_page_callback::ptr m_callback;
	bool m_initialized;

public:
	static BOOL CALLBACK ConfigProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

	// Constructor
	preferences_page_instance_foo_input_vgm(HWND parent, preferences_page_callback::ptr callback) :
		m_parent(parent),
		m_callback(callback),
		m_initialized(false)
	{
		m_hwnd = create(m_parent);
	}

	HWND get_wnd()
	{
		return m_hwnd;
	}

	HWND create(HWND p_parent)
	{
		return uCreateDialog(IDD_DIALOG1, p_parent, ConfigProc, reinterpret_cast<LPARAM>(this));
	}

	t_uint32 get_state()
	{
		if (cfg_resampling_mode != uSendDlgItemMessage(m_hwnd, IDC_RESAMPLING_MODE, CB_GETCURSEL, 0, 0) ||
			cfg_chip_sample_mode != uSendDlgItemMessage(m_hwnd, IDC_CHIP_SAMPLE_MODE, CB_GETCURSEL, 0, 0) ||
			cfg_combo_playback_rate != uSendDlgItemMessage(m_hwnd, IDC_PLAYBACK_RATE, CB_GETCURSEL, 0, 0) ||
			cfg_sample_rate != uGetDlgItemInt(m_hwnd, IDC_SAMPLE_RATE, NULL, FALSE) ||
			cfg_chip_rate != uGetDlgItemInt(m_hwnd, IDC_CHIP_RATE, NULL, FALSE) ||
			cfg_edit_playback_rate != uGetDlgItemInt(m_hwnd, IDC_EDIT_PLAYBACK_RATE, NULL, FALSE) ||
			cfg_fade_length != uGetDlgItemInt(m_hwnd, IDC_FADE_LENGTH, NULL, FALSE) ||
			cfg_loop_count != uSendDlgItemMessage(m_hwnd, IDC_SPIN_LOOP_COUNT, UDM_GETPOS, 0, 0) ||
			cfg_pause_looping != uGetDlgItemInt(m_hwnd, IDC_PAUSE_LOOPING, NULL, FALSE) ||
			cfg_pause_non_looping != uGetDlgItemInt(m_hwnd, IDC_PAUSE_NON_LOOPING, NULL, FALSE) ||
			cfg_surround_sound != IsDlgButtonChecked(m_hwnd, IDC_SURROUND_SOUND) ||
			cfg_double_ssg_volume != IsDlgButtonChecked(m_hwnd, IDC_DOUBLE_SSG_VOLUME) ||
			cfg_prefer_jap_tag != IsDlgButtonChecked(m_hwnd, IDC_PREFER_JAP_TAG) ||
			cfg_display_both_tag != IsDlgButtonChecked(m_hwnd, IDC_DISPLAY_BOTH_TAG) ||
			cfg_guess_track_number != IsDlgButtonChecked(m_hwnd, IDC_GUESS_TRACK_NUMBER) ||
			cfg_recomp_level != uSendDlgItemMessage(m_hwnd, IDC_SPIN_RECOMP_LEVEL, UDM_GETPOS, 0, 0) ||
			cfg_volume != uGetDlgItemInt(m_hwnd, IDC_VOLUME, NULL, FALSE) ||
			cfg_vgm != IsDlgButtonChecked(m_hwnd, IDC_VGM) ||
			cfg_hard_stop_old_vgms != IsDlgButtonChecked(m_hwnd, IDC_HARD_STOP_OLD_VGMS)
			)
			return preferences_state::changed | preferences_state::resettable;
		return preferences_state::resettable;
	}

	void apply()
	{
		cfg_resampling_mode = uSendDlgItemMessage(m_hwnd, IDC_RESAMPLING_MODE, CB_GETCURSEL, 0, 0);
		cfg_chip_sample_mode = uSendDlgItemMessage(m_hwnd, IDC_CHIP_SAMPLE_MODE, CB_GETCURSEL, 0, 0);
		cfg_combo_playback_rate = uSendDlgItemMessage(m_hwnd, IDC_PLAYBACK_RATE, CB_GETCURSEL, 0, 0);

		cfg_sample_rate = uGetDlgItemInt(m_hwnd, IDC_SAMPLE_RATE, NULL, FALSE);
		if (cfg_sample_rate > 1411200) cfg_sample_rate = 1411200;
		if (0 < cfg_sample_rate && cfg_sample_rate < 4000) cfg_sample_rate = 4000;
		else if (cfg_sample_rate <= 0) cfg_sample_rate = 44100;

		cfg_chip_rate = uGetDlgItemInt(m_hwnd, IDC_CHIP_RATE, NULL, FALSE);
		if (cfg_chip_rate > 192000) cfg_chip_rate = 192000;
		if (0 < cfg_chip_rate && cfg_chip_rate < 4000) cfg_chip_rate = 4000;
		else if (cfg_chip_rate < 0) cfg_chip_rate = 0;

		cfg_edit_playback_rate = uGetDlgItemInt(m_hwnd, IDC_EDIT_PLAYBACK_RATE, NULL, FALSE);
		if (cfg_edit_playback_rate > 100000) cfg_edit_playback_rate = 100000;
		if (cfg_edit_playback_rate < 0) cfg_edit_playback_rate = 0;

		switch (cfg_combo_playback_rate)
		{
		case 0:
			cfg_playback_rate = 0;
			break;
		case 1:
			cfg_playback_rate = 60;
			break;
		case 2:
			cfg_playback_rate = 50;
			break;
		default:
			cfg_playback_rate = cfg_edit_playback_rate;
			break;
		}

		cfg_fade_length = uGetDlgItemInt(m_hwnd, IDC_FADE_LENGTH, NULL, FALSE);
		if (cfg_fade_length > 36000000) cfg_fade_length = 36000000;
		if (cfg_fade_length < 0) cfg_fade_length = 0;

		cfg_loop_count = uSendDlgItemMessage(m_hwnd, IDC_SPIN_LOOP_COUNT, UDM_GETPOS, 0, 0);
		cfg_pause_looping = uGetDlgItemInt(m_hwnd, IDC_PAUSE_LOOPING, NULL, FALSE);
		if (cfg_pause_looping > 36000000) cfg_pause_looping = 36000000;
		if (cfg_pause_looping < 0) cfg_pause_looping = 0;

		cfg_pause_non_looping = uGetDlgItemInt(m_hwnd, IDC_PAUSE_NON_LOOPING, NULL, FALSE);
		if (cfg_pause_non_looping > 36000000) cfg_pause_non_looping = 36000000;
		if (cfg_pause_non_looping < 0) cfg_pause_non_looping = 0;

		cfg_hard_stop_old_vgms = IsDlgButtonChecked(m_hwnd, IDC_HARD_STOP_OLD_VGMS);
		cfg_surround_sound = IsDlgButtonChecked(m_hwnd, IDC_SURROUND_SOUND);
		cfg_double_ssg_volume = IsDlgButtonChecked(m_hwnd, IDC_DOUBLE_SSG_VOLUME);
		cfg_prefer_jap_tag = IsDlgButtonChecked(m_hwnd, IDC_PREFER_JAP_TAG);
		cfg_display_both_tag = IsDlgButtonChecked(m_hwnd, IDC_DISPLAY_BOTH_TAG);
		cfg_guess_track_number = IsDlgButtonChecked(m_hwnd, IDC_GUESS_TRACK_NUMBER);
		cfg_recomp_level = uSendDlgItemMessage(m_hwnd, IDC_SPIN_RECOMP_LEVEL, UDM_GETPOS, 0, 0);

		cfg_volume = uGetDlgItemInt(m_hwnd, IDC_VOLUME, NULL, FALSE);
		if (cfg_volume > 400) cfg_volume = 400;
		if (cfg_volume <= 0) cfg_volume = 100;

		cfg_vgm = IsDlgButtonChecked(m_hwnd, IDC_VGM);

		m_callback->on_state_changed();
	}

	void reset()
	{
		uSendDlgItemMessage(m_hwnd, IDC_RESAMPLING_MODE, CB_SETCURSEL, 0, 0);
		uSendDlgItemMessage(m_hwnd, IDC_CHIP_SAMPLE_MODE, CB_SETCURSEL, 0, 0);
		uSendDlgItemMessage(m_hwnd, IDC_PLAYBACK_RATE, CB_SETCURSEL, 0, 0);

		uSetDlgItemInt(m_hwnd, IDC_SAMPLE_RATE, 44100, FALSE);

		uSetDlgItemInt(m_hwnd, IDC_CHIP_RATE, 0, FALSE);
		EnableWindow(GetDlgItem(m_hwnd, IDC_CHIP_RATE), FALSE);

		uSetDlgItemInt(m_hwnd, IDC_EDIT_PLAYBACK_RATE, 0, FALSE);
		EnableWindow(GetDlgItem(m_hwnd, IDC_EDIT_PLAYBACK_RATE), FALSE);

		uSetDlgItemInt(m_hwnd, IDC_FADE_LENGTH, 5000, FALSE);
		uSendDlgItemMessage(m_hwnd, IDC_SPIN_LOOP_COUNT, UDM_SETPOS, 0, (LPARAM)MAKELONG(2, 0));

		uSetDlgItemInt(m_hwnd, IDC_PAUSE_LOOPING, 0, FALSE);
		uSetDlgItemInt(m_hwnd, IDC_PAUSE_NON_LOOPING, 0, FALSE);

		CheckDlgButton(m_hwnd, IDC_HARD_STOP_OLD_VGMS, BST_UNCHECKED);
		CheckDlgButton(m_hwnd, IDC_SURROUND_SOUND, BST_UNCHECKED);
		CheckDlgButton(m_hwnd, IDC_DOUBLE_SSG_VOLUME, BST_CHECKED);
		CheckDlgButton(m_hwnd, IDC_PREFER_JAP_TAG, BST_UNCHECKED);
		CheckDlgButton(m_hwnd, IDC_DISPLAY_BOTH_TAG, BST_UNCHECKED);
		CheckDlgButton(m_hwnd, IDC_GUESS_TRACK_NUMBER, BST_UNCHECKED);
		uSendDlgItemMessage(m_hwnd, IDC_SPIN_RECOMP_LEVEL, UDM_SETPOS, 0, (LPARAM)MAKELONG(6, 0));
		uSetDlgItemInt(m_hwnd, IDC_VOLUME, (UINT)100, FALSE);

		CheckDlgButton(m_hwnd, IDC_VGM, BST_CHECKED);

		m_callback->on_state_changed();
	}
};

BOOL CALLBACK preferences_page_instance_foo_input_vgm::ConfigProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		uSetWindowLong(wnd, DWL_USER, lp);
		preferences_page_instance_foo_input_vgm* p_this;
		p_this = reinterpret_cast<preferences_page_instance_foo_input_vgm*>(lp);
		if (p_this) p_this->m_initialized = false;

		//combo
		HWND h = uGetDlgItem(wnd, IDC_RESAMPLING_MODE);
		uSendMessageText(h, CB_ADDSTRING, 0, "HQ resampling");
		uSendMessageText(h, CB_ADDSTRING, 0, "HQ up, LQ down");
		uSendMessageText(h, CB_ADDSTRING, 0, "LQ resampling");
		::SendMessage(h, CB_SETCURSEL, cfg_resampling_mode, 0);

		h = uGetDlgItem(wnd, IDC_CHIP_SAMPLE_MODE);
		uSendMessageText(h, CB_ADDSTRING, 0, "native");
		uSendMessageText(h, CB_ADDSTRING, 0, "highest(nat./cust.)");
		uSendMessageText(h, CB_ADDSTRING, 0, "custom");
		uSendMessageText(h, CB_ADDSTRING, 0, "highest, FM native");
		::SendMessage(h, CB_SETCURSEL, cfg_chip_sample_mode, 0);

		h = uGetDlgItem(wnd, IDC_PLAYBACK_RATE);
		uSendMessageText(h, CB_ADDSTRING, 0, "Recorded rate");
		uSendMessageText(h, CB_ADDSTRING, 0, "60 Hz");
		uSendMessageText(h, CB_ADDSTRING, 0, "50 Hz");
		uSendMessageText(h, CB_ADDSTRING, 0, "Other:");
		::SendMessage(h, CB_SETCURSEL, cfg_combo_playback_rate, 0);


		uSetDlgItemInt(wnd, IDC_SAMPLE_RATE, cfg_sample_rate, FALSE);

		uSetDlgItemInt(wnd, IDC_CHIP_RATE, cfg_chip_rate, FALSE);
		BOOL ctrl_enable = (cfg_chip_sample_mode > 0) ? TRUE : FALSE;
		EnableWindow(GetDlgItem(wnd, IDC_CHIP_RATE), ctrl_enable);

		uSetDlgItemInt(wnd, IDC_EDIT_PLAYBACK_RATE, cfg_edit_playback_rate, FALSE);
		ctrl_enable = (cfg_combo_playback_rate == 3) ? TRUE : FALSE;
		EnableWindow(GetDlgItem(wnd, IDC_EDIT_PLAYBACK_RATE), ctrl_enable);

		uSetDlgItemInt(wnd, IDC_FADE_LENGTH, cfg_fade_length, FALSE);
		uSendDlgItemMessage(wnd, IDC_SPIN_LOOP_COUNT, UDM_SETRANGE32, (WPARAM)0, (LPARAM)50);
		uSendDlgItemMessage(wnd, IDC_SPIN_LOOP_COUNT, UDM_SETPOS, 0, (LPARAM)MAKELONG(cfg_loop_count, 0));

		uSetDlgItemInt(wnd, IDC_PAUSE_LOOPING, cfg_pause_looping, FALSE);
		uSetDlgItemInt(wnd, IDC_PAUSE_NON_LOOPING, cfg_pause_non_looping, FALSE);

		CheckDlgButton(wnd, IDC_HARD_STOP_OLD_VGMS, (UINT)cfg_hard_stop_old_vgms);
		CheckDlgButton(wnd, IDC_SURROUND_SOUND, (UINT)cfg_surround_sound);
		CheckDlgButton(wnd, IDC_DOUBLE_SSG_VOLUME, (UINT)cfg_double_ssg_volume);
		CheckDlgButton(wnd, IDC_PREFER_JAP_TAG, (UINT)cfg_prefer_jap_tag);
		CheckDlgButton(wnd, IDC_DISPLAY_BOTH_TAG, (UINT)cfg_display_both_tag);
		CheckDlgButton(wnd, IDC_GUESS_TRACK_NUMBER, (UINT)cfg_guess_track_number);
		uSendDlgItemMessage(wnd, IDC_SPIN_RECOMP_LEVEL, UDM_SETRANGE32, (WPARAM)3, (LPARAM)9);
		uSendDlgItemMessage(wnd, IDC_SPIN_RECOMP_LEVEL, UDM_SETPOS, 0, (LPARAM)MAKELONG(cfg_recomp_level, 0));
		uSetDlgItemInt(wnd, IDC_VOLUME, (UINT)cfg_volume, FALSE);

		CheckDlgButton(wnd, IDC_VGM, (UINT)cfg_vgm);

		if (p_this) p_this->m_initialized = true;
		break;
	}
	case WM_COMMAND:
	{
		preferences_page_instance_foo_input_vgm* p_this;
		p_this = reinterpret_cast<preferences_page_instance_foo_input_vgm*> (uGetWindowLong(wnd, DWL_USER));
		switch (LOWORD(wp))
		{
		case IDC_CHIP_RATE: case IDC_EDIT_LOOP_COUNT: case IDC_EDIT_PLAYBACK_RATE:
		case IDC_FADE_LENGTH: case IDC_PAUSE_LOOPING: case IDC_PAUSE_NON_LOOPING:
		case IDC_PREFER_JAP_TAG: case IDC_RESAMPLING_MODE: case IDC_SAMPLE_RATE: case IDC_SPIN_LOOP_COUNT:
		case IDC_SURROUND_SOUND: case IDC_VGM: case IDC_VOLUME: case IDC_DOUBLE_SSG_VOLUME:
		case IDC_DISPLAY_BOTH_TAG: case IDC_GUESS_TRACK_NUMBER: case IDC_EDIT_RECOMP_LEVEL: case IDC_SPIN_RECOMP_LEVEL:
		case IDC_HARD_STOP_OLD_VGMS:
			if (p_this && p_this->m_initialized) p_this->m_callback->on_state_changed();
			break;
		case IDC_CHIP_SAMPLE_MODE:
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				int combo_pos = uSendDlgItemMessage(wnd, IDC_CHIP_SAMPLE_MODE, CB_GETCURSEL, 0, 0);
				BOOL ctrl_enable = (combo_pos > 0) ? TRUE : FALSE;
				EnableWindow(GetDlgItem(wnd, IDC_CHIP_RATE), ctrl_enable);
				if (p_this && p_this->m_initialized) p_this->m_callback->on_state_changed();
			}
			break;
		}
		case IDC_PLAYBACK_RATE:
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				int combo_pos = uSendDlgItemMessage(wnd, IDC_PLAYBACK_RATE, CB_GETCURSEL, 0, 0);
				BOOL ctrl_enable = (combo_pos == 3) ? TRUE : FALSE;
				EnableWindow(GetDlgItem(wnd, IDC_EDIT_PLAYBACK_RATE), ctrl_enable);
				if (p_this && p_this->m_initialized) p_this->m_callback->on_state_changed();
			}
			break;
		}
		default:
			break;

		}
		break;
	}
	case WM_DESTROY:
	{
		uSetWindowLong(wnd, DWL_USER, 0);
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}


class preferences_page_foo_input_vgm : public preferences_page_v3
{
public:

	virtual const char * get_name()
	{
		return my_component_name;
	}

	virtual GUID get_guid()
	{
		return guid_preferences_page_foo_input_vgm;
	}

	virtual GUID get_parent_guid()
	{
		return guid_input;
	}

	virtual preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback)
	{
		return new service_impl_t<preferences_page_instance_foo_input_vgm>(parent, callback);
	}

};

static preferences_page_factory_t<preferences_page_foo_input_vgm> g_preferences_page_foo_input_vgm;
