#pragma warning(disable : 4996)

#include "stdafx.h"
#include "resource.h"
#include "../helpers/helpers.h"


// {E89FBB75-E00E-413E-8B86-A819342C8C3A}
static const GUID guid_preferences_page_chips_options =
{ 0xe89fbb75, 0xe00e, 0x413e, { 0x8b, 0x86, 0xa8, 0x19, 0x34, 0x2c, 0x8c, 0x3a } };
// {B4B6216E-40A0-4aaa-82E9-945D230479C3}
static const GUID guid_cfg_chips_opts1 =
{ 0xb4b6216e, 0x40a0, 0x4aaa, { 0x82, 0xe9, 0x94, 0x5d, 0x23, 0x4, 0x79, 0xc3 } };
// {A970CFC0-9BDF-4cd4-B5AF-9606EC647DC9}
static const GUID guid_cfg_chips_opts2 =
{ 0xa970cfc0, 0x9bdf, 0x4cd4, { 0xb5, 0xaf, 0x96, 0x6, 0xec, 0x64, 0x7d, 0xc9 } };
// {053294F2-90DC-4BF3-A5A8-3A292534B868}
static const GUID guid_cfg_chip_id_index =
{ 0x53294f2, 0x90dc, 0x4bf3, { 0xa5, 0xa8, 0x3a, 0x29, 0x25, 0x34, 0xb8, 0x68 } };
// {BC98104F-BAD1-43C2-9257-FEB05738AA4D}
static const GUID guid_cfg_chip_set_index =
{ 0xbc98104f, 0xbad1, 0x43c2, { 0x92, 0x57, 0xfe, 0xb0, 0x57, 0x38, 0xaa, 0x4d } };

static cfg_chips_options cfg_chips_options_1(guid_cfg_chips_opts1);
static cfg_chips_options cfg_chips_options_2(guid_cfg_chips_opts2);


cfg_chips_options *cfg_chips_opts[2] = { &cfg_chips_options_1, &cfg_chips_options_2 };

//コンボボックスの位置保存用
cfg_int cfg_chip_id_index(guid_cfg_chip_id_index, 0);
cfg_int cfg_chip_set_index(guid_cfg_chip_set_index, 0);


//Chips Options preferences instance class. Any changes will be immediately updated regardless of OK/Cancel/Apply.
class preferences_page_instance_chips_options : public preferences_page_instance
{
private:
	static void show_muting(HWND hwnd);
	static void show_special_flags(HWND hwnd);
	static void set_muting(t_uint32 check_box, bool checked);
	static void set_special_flags(t_uint32 check_box, bool checked);
	HWND m_parent;
	HWND m_hwnd;
	preferences_page_callback::ptr m_callback;
	bool m_initialized;

public:
	static BOOL CALLBACK config_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

	// Constructor
	preferences_page_instance_chips_options(HWND parent, preferences_page_callback::ptr callback) :
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
		return uCreateDialog(IDD_DIALOG2, p_parent, config_proc, reinterpret_cast<LPARAM>(this));
	}

	t_uint32 get_state()
	{
		return preferences_state::resettable;
	}

	void apply()
	{}

	void reset()
	{
		//uSendDlgItemMessage(m_hwnd, IDC_CHIP_ID, CB_SETCURSEL, 0, 0);	
		//uSendDlgItemMessage(m_hwnd, IDC_CHIP_SET, CB_SETCURSEL, 0, 0);

		//cfg_chip_id_index = 0;
		//cfg_chip_set_index = 0;

		for (t_uint8 i = 0; i < 2; i++)
		{
			cfg_chips_opts[i]->reset<chips_options_ex::muting | chips_options_ex::special_flags, false>();
		}
		show_muting(m_hwnd);
		show_special_flags(m_hwnd);
	}
};

BOOL CALLBACK preferences_page_instance_chips_options::config_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		uSetWindowLong(wnd, DWL_USER, lp);
		preferences_page_instance_chips_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_chips_options*>(lp);
		if (p_this) p_this->m_initialized = false;

		//combo box
		HWND h = uGetDlgItem(wnd, IDC_CHIP_ID);
		for (UINT8 i = 0; i < CHIP_COUNT; i++)
		{
			uSendMessageText(h, CB_ADDSTRING, 0, my_chip_name[i]);
		}
		::SendMessage(h, CB_SETCURSEL, cfg_chip_id_index, 0);

		h = uGetDlgItem(wnd, IDC_CHIP_SET);
		uSendMessageText(h, CB_ADDSTRING, 0, "Chip #1");
		uSendMessageText(h, CB_ADDSTRING, 0, "Chip #2");
		::SendMessage(h, CB_SETCURSEL, cfg_chip_set_index, 0);

		show_muting(wnd);
		show_special_flags(wnd);

		//if (p_this) p_this->m_initialized = true;
		break;
	}
	case WM_COMMAND:
	{
		preferences_page_instance_chips_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_chips_options*> (uGetWindowLong(wnd, DWL_USER));
		//CHIP_OPTS_AT* curopts = reinterpret_cast<CHIP_OPTS_AT*>(cfg_chip_opts[cfg_chip_set_index].get_ptr()) + cfg_chip_id_index;

		switch (LOWORD(wp))
		{
		case IDC_CHIP_ID:
		case IDC_CHIP_SET:
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				cfg_chip_id_index = uSendDlgItemMessage(wnd, IDC_CHIP_ID, CB_GETCURSEL, 0, 0);
				cfg_chip_set_index = uSendDlgItemMessage(wnd, IDC_CHIP_SET, CB_GETCURSEL, 0, 0);

				//show muting check box
				show_muting(wnd);

				//show special chip option check box
				if (LOWORD(wp) == IDC_CHIP_ID)
					show_special_flags(wnd);

			}
			break;
		}
		case IDC_DISABLE_CHIP:
		{
			bool checked = IsDlgButtonChecked(wnd, IDC_DISABLE_CHIP) == BST_CHECKED ? true : false;
			CHIP_OPTS* cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[cfg_chip_set_index]->get_ptr()) + cfg_chip_id_index;
			cur_chip_opts->Disabled = checked;
			break;
		}
		default:
			break;
		}

		if (LOWORD(wp) >= IDC_MUTE_CHANNEL1 && LOWORD(wp) <= IDC_MUTE_CHANNEL32)
		{
			if (HIWORD(wp) == BN_CLICKED)
			{
				bool checked = IsDlgButtonChecked(wnd, LOWORD(wp)) == BST_CHECKED ? true : false;
				set_muting(LOWORD(wp) - IDC_MUTE_CHANNEL1, checked);
			}
		}
		else if (LOWORD(wp) >= IDC_SPECIAL_FLAG1 && LOWORD(wp) <= IDC_SPECIAL_FLAG16)
		{
			if (HIWORD(wp) == BN_CLICKED)
			{
				bool checked = IsDlgButtonChecked(wnd, LOWORD(wp)) == BST_CHECKED ? true : false;
				set_special_flags(LOWORD(wp) - IDC_SPECIAL_FLAG1, checked);
			}
		}

		break;
	}
	case WM_WINDOWPOSCHANGED:
	{
		preferences_page_instance_chips_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_chips_options*> (uGetWindowLong(wnd, DWL_USER));
		WINDOWPOS* p_wpos = reinterpret_cast<WINDOWPOS*>(lp);
		//BOOL enabled = IsWindowEnabled(wnd);
		if (p_this && p_this->m_initialized && p_wpos && p_wpos->flags & SWP_SHOWWINDOW)
		{
			uSendDlgItemMessage(wnd, IDC_CHIP_ID, CB_SETCURSEL, cfg_chip_id_index, 0);
			uSendDlgItemMessage(wnd, IDC_CHIP_SET, CB_SETCURSEL, cfg_chip_set_index, 0);
			show_muting(wnd);
			show_special_flags(wnd);
		}
		if (p_this && !(p_this->m_initialized))
			p_this->m_initialized = true;
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

//These functions is from in_vgm's dlg_cfg.c (modified a little, thanks ValleyBell)
void preferences_page_instance_chips_options::show_muting(HWND hwnd)
{
	UINT8 channel_count;
	UINT8 channel_count_s[0x04];
	const char* special_channel_name[0x40];	// Special Channel Names
	UINT8 cur_channel;
	UINT8 special_modes;
	bool enable_check(true);
	bool checked;
	UINT8 cur_mode;
	UINT8 channel_base;
	UINT8 final_channel;
	char temp_name[0x18];

	for (cur_channel = 0x00; cur_channel < 0x40; cur_channel++)
		special_channel_name[cur_channel] = NULL;

	special_modes = 0;
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	switch (chip_id)
	{
	case 0x00:	// SN76496
		channel_count = 4;
		special_channel_name[3] = "&Noise";
		break;
	case 0x01:	// YM2413
	case 0x09:	// YM3812
	case 0x0A:	// YM3526
	case 0x0B:	// Y8950
		channel_count = 14;	// 9 + 5
		special_channel_name[9] = "&Bass Drum";
		special_channel_name[10] = "S&nare Drum";
		special_channel_name[11] = "&Tom Tom";
		special_channel_name[12] = "C&ymbal";
		special_channel_name[13] = "&Hi-Hat";
		if (chip_id == 0x0B)
		{
			channel_count = 15;
			special_channel_name[14] = "&Delta-T";
		}
		break;
	case 0x02:	// YM2612
		channel_count = 7;	// 6 + DAC
		special_channel_name[6] = "&DAC";
		break;
	case 0x03:	// YM2151
		channel_count = 8;
		break;
	case 0x04:	// Sega PCM
		channel_count = 16;
		enable_check &= !chip_set;
		break;
	case 0x05:	// RF5C68
	case 0x10:	// RF5C164
		channel_count = 8;
		enable_check &= !chip_set;
		break;
	case 0x06:	// YM2203
		channel_count = 6;	// 3 FM + 3 AY8910
		special_modes = 3;
		channel_count_s[0] = 3;
		special_channel_name[0] = "FM Channel";
		channel_count_s[1] = 0;
		channel_count_s[2] = 3;
		special_channel_name[2] = "PSG Channel";
		break;
	case 0x07:	// YM2608
	case 0x08:	// YM2610
		channel_count = 16;	// 6 FM + 6 ADPCM + 1 DeltaT + 3 AY8910
		special_modes = 3;
		channel_count_s[0] = 6;
		special_channel_name[0] = "FM Channel";
		channel_count_s[1] = 7;
		special_channel_name[1] = "PCM Channel";
		special_channel_name[14] = "&Delta-T";
		channel_count_s[2] = 3;
		special_channel_name[2] = "PSG Channel";
		break;
	case 0x0C:	// YMF262
		channel_count = 23;	// 18 + 5
		special_channel_name[18] = "Bass Drum (&J)";
		special_channel_name[19] = "S&nare Drum";
		special_channel_name[20] = "&Tom Tom";
		special_channel_name[21] = "C&ymbal";
		special_channel_name[22] = "Hi-Hat (&K)";
		break;
	case 0x0D:	// YMF278B
		channel_count = 24;
		break;
	case 0x0E:	// YMF271
		channel_count = 12;
		break;
	case 0x0F:	// YMZ280B
		channel_count = 8;
		break;
	case 0x11:	// PWM
		channel_count = 0;
		enable_check &= !chip_set;
		break;
	case 0x12:	// AY8910
		channel_count = 3;
		break;
	case 0x13:	// GB DMG
		special_channel_name[0] = "Square &1";
		special_channel_name[1] = "Square &2";
		special_channel_name[2] = "Progr. &Wave";
		special_channel_name[3] = "&Noise";
		channel_count = 4;
		break;
	case 0x14:	// NES APU
		special_channel_name[0] = "Square &1";
		special_channel_name[1] = "Square &2";
		special_channel_name[2] = "&Triangle";
		special_channel_name[3] = "&Noise";
		special_channel_name[4] = "&DPCM";
		special_channel_name[5] = "&FDS";
		channel_count = 6;
		break;
	case 0x15:	// Multi PCM
		channel_count = 28;
		break;
	case 0x16:	// UPD7759
		channel_count = 0;
		break;
	case 0x17:	// OKIM6258
		channel_count = 0;
		break;
	case 0x18:	// OKIM6295
		channel_count = 4;
		break;
	case 0x19:	// K051649
		channel_count = 5;
		break;
	case 0x1A:	// K054539
		channel_count = 8;
		break;
	case 0x1B:	// HuC6280
		channel_count = 6;
		break;
	case 0x1C:	// C140
		channel_count = 24;
		break;
	case 0x1D:	// K053260
		channel_count = 4;
		break;
	case 0x1E:	// Pokey
		channel_count = 4;
		break;
	case 0x1F:	// Q-Sound
		channel_count = 16;
		enable_check &= !chip_set;
		break;
	case 0x20:	// SCSP
		channel_count = 32;
		break;
	case 0x21:	// WonderSwan
		channel_count = 4;
		break;
	case 0x22:	// VSU
		channel_count = 6;
		break;
	case 0x23:	// SAA1099
		channel_count = 6;
		break;
	case 0x24:	// ES5503
		channel_count = 32;
		break;
	case 0x25:	// ES5506
		channel_count = 32;
		break;
	case 0x26:	// X1-010
		channel_count = 16;
		break;
	case 0x27:	// C352
		channel_count = 32;
		break;
	case 0x28:	// GA20
		channel_count = 4;
		break;
	default:
		channel_count = 0;
		enable_check &= !chip_set;
		break;
	}
	CHIP_OPTS * cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[chip_set]->get_ptr()) + chip_id;
	if (channel_count > 32)
		channel_count = 32;	// currently only 32 check_boxes are supported

	CheckDlgButton(hwnd, IDC_DISABLE_CHIP, (UINT)cur_chip_opts->Disabled);
	EnableWindow(GetDlgItem(hwnd, IDC_DISABLE_CHIP), enable_check);

	if (!special_modes)
	{
		for (cur_channel = 0; cur_channel < channel_count; cur_channel++)
		{
			if (special_channel_name[cur_channel] == NULL)
			{
				if (1 + cur_channel < 10)
					sprintf(temp_name, "Channel &%u", 1 + cur_channel);
				else
					sprintf(temp_name, "Channel %u (&%c)", 1 + cur_channel, 'A' + (cur_channel - 9));
				uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + cur_channel, temp_name);
			}
			else
			{
				uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + cur_channel, special_channel_name[cur_channel]);
			}

			if (chip_id == 0x0D)
				checked = (cur_chip_opts->ChnMute2 >> cur_channel) & 0x01;
			else
				checked = (cur_chip_opts->ChnMute1 >> cur_channel) & 0x01;
			CheckDlgButton(hwnd, IDC_MUTE_CHANNEL1 + cur_channel, checked);
			EnableWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + cur_channel), enable_check);
			ShowWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + cur_channel), SW_SHOW);
		}
	}
	else
	{
		for (cur_mode = 0; cur_mode < special_modes; cur_mode++)
		{
			channel_base = cur_mode * 8;
			if (special_channel_name[cur_mode] == NULL)
				special_channel_name[cur_mode] = "Channel";

			for (cur_channel = 0, final_channel = channel_base; cur_channel < channel_count_s[cur_mode]; cur_channel++, final_channel++)
			{
				if (final_channel < special_modes || special_channel_name[final_channel] == NULL)
				{
					if (1 + cur_channel < 10)
						sprintf(temp_name, "%s &%u", special_channel_name[cur_mode], 1 + cur_channel);
					else
						sprintf(temp_name, "%s %u (&%c)", special_channel_name[cur_mode], 1 + cur_channel,
						'A' + (cur_channel - 9));
					uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + final_channel, temp_name);
				}
				else
				{
					uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + final_channel, special_channel_name[final_channel]);
				}

				switch (cur_mode)
				{
				case 0:
					checked = (cur_chip_opts->ChnMute1 >> cur_channel) & 0x01;
					break;
				case 1:
					checked = (cur_chip_opts->ChnMute2 >> cur_channel) & 0x01;
					break;
				case 2:
					checked = (cur_chip_opts->ChnMute3 >> cur_channel) & 0x01;
					break;
				}
				CheckDlgButton(hwnd, IDC_MUTE_CHANNEL1 + final_channel, checked);
				EnableWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + final_channel), enable_check);
				ShowWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + final_channel), SW_SHOW);
			}
			for (; cur_channel < 8; cur_channel++, final_channel++)
			{
				ShowWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + final_channel), SW_HIDE);
				EnableWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + final_channel), enable_check);
				uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + final_channel, "");
			}
		}
		cur_channel = final_channel;
	}
	for (; cur_channel < 32; cur_channel++)
	{
		// I thought that disabling a window should prevent it from catching other
		// windows' keyboard shortcuts.
		// But NO, it seems that you have to make its text EMPTY to make it work!
		ShowWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + cur_channel), SW_HIDE);
		EnableWindow(GetDlgItem(hwnd, IDC_MUTE_CHANNEL1 + cur_channel), FALSE);
		uSetDlgItemText(hwnd, IDC_MUTE_CHANNEL1 + cur_channel, "");
	}
}

void preferences_page_instance_chips_options::set_muting(t_uint32 check_box, bool checked)
{
	CHIP_OPTS* cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[cfg_chip_set_index]->get_ptr()) + cfg_chip_id_index;
	UINT8 cur_channel;
	UINT8 special_modes;
	UINT8 cur_mode;

	special_modes = 0;
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	switch (chip_id)
	{
	case 0x06:	// YM2203
		special_modes = 2;
		break;
	case 0x07:	// YM2608
	case 0x08:	// YM2610
		special_modes = 3;
		break;
	}

	if (!special_modes)
	{
		cur_mode = 0;
		if (chip_id == 0x0D)
			cur_mode = 1;
		cur_channel = check_box;
	}
	else
	{
		cur_mode = check_box / 8;
		cur_channel = check_box % 8;
	}
	switch (cur_mode)
	{
	case 0:
		cur_chip_opts->ChnMute1 &= (~(1 << cur_channel));
		cur_chip_opts->ChnMute1 |= (checked << cur_channel);
		break;
	case 1:
		cur_chip_opts->ChnMute2 &= (~(1 << cur_channel));
		cur_chip_opts->ChnMute2 |= (checked << cur_channel);
		break;
	case 2:
		cur_chip_opts->ChnMute3 &= (~(1 << cur_channel));
		cur_chip_opts->ChnMute3 |= (checked << cur_channel);
		break;
	}
}

void preferences_page_instance_chips_options::show_special_flags(HWND hwnd)
{
	const char* special_flags_name[16];
	UINT8 special_flags_count = 0;
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	//Currently EmuCore is shared between Chip#1 and Chip#2
	CHIP_OPTS *temp_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[0]->get_ptr()) + chip_id;
	UINT8 cur_emu_core = temp_chip_opts->EmuCore;

	for (UINT8 i = 0; i < 16; i++)
	{
		special_flags_name[i] = 0;
	}

	switch (chip_id)
	{
		//YM2612
	case 0x02:
	{
		if (cur_emu_core == 0x00)
		{
			//MAME
			special_flags_name[0] = 0;
			special_flags_name[1] = 0;
			special_flags_name[2] = "PseudoStereo";	//MAME only
			special_flags_count = 3;
		}
		else if (cur_emu_core == 0x01)
		{
			//Nuked OPN2
			special_flags_name[0] = 0;
			special_flags_name[1] = 0;
			special_flags_name[2] = 0;
			special_flags_name[3] = "NukedType bit0(LSB)";
			special_flags_name[4] = "NukedType bit1";
			special_flags_count = 5;
		}
		else if (cur_emu_core == 0x02)
		{
			//Gens
			special_flags_name[0] = "DACHighpass";
			special_flags_name[1] = "SSG-EG (buggy)";
			special_flags_count = 2;
		}
		break;
	}
	//YM2203
	case 0x06:
		special_flags_name[0] = "Disable AY";
		special_flags_count = 1;
		break;
		//YM2608
	case 0x07:
		special_flags_name[0] = "Disable AY";
		special_flags_count = 1;
		break;
		//YM2610
	case 0x08:
		special_flags_name[0] = "Disable AY";
		special_flags_count = 1;
		break;
		//GameBoy
	case 0x13:
		special_flags_name[0] = "Boost WaveCh";
		//v0.40.8で消去された
		//special_flags_name[1] = "Reduce Noise";
		//special_flags_name[2] = "Inaccurate";
		//special_flags_count = 3;
		special_flags_count = 1;
		break;
		//NES APU
	case 0x14:
	{
		if (cur_emu_core == 0x00)
		{
			special_flags_name[0] = "Unmute On Reset";
			special_flags_name[1] = "Non Linear Mixer";
			special_flags_name[2] = "Phase Refresh";
			special_flags_name[3] = "Duty Swap";
			special_flags_name[4] = "Enable $4011";
			special_flags_name[5] = "Enable PNoise";
			special_flags_name[6] = "DPCM Anti Click";
			special_flags_name[7] = "Randomize Noise";
			special_flags_name[8] = "Tri Mute";
			special_flags_name[9] = "Tri Null";
			special_flags_name[10] = 0;
			special_flags_name[11] = 0;
			special_flags_name[12] = "$4085 Reset";
			special_flags_count = 13;
		}
		else //if (cur_emu_core == 0x01 || cur_emu_core == 0x02)
		{
			special_flags_count = 0;
		}
		break;
	}
	//OKIM6258
	case 0x17:
		special_flags_name[0] = "Internal 10bit";
		//v0.40.8で消去された
		//special_flags_name[1] = "Remove DC Offset";
		//special_flags_count = 2;
		special_flags_count = 1;
		break;
		//SCSP
	case 0x20:
		special_flags_name[0] = "Bypass DSP";
		special_flags_count = 1;
		break;
		//C352
	case 0x27:
		special_flags_name[0] = "Disable Rear";
		special_flags_count = 1;
		break;
	default:
		special_flags_count = 0;
	}


	if (special_flags_count > 16) special_flags_count = 16;

	//Currently ChipsOptions are shared between Chip#1 and Chip#2
	CHIP_OPTS *cur_special_flags = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[0]->get_ptr()) + chip_id;

	for (UINT8 i = 0; i < special_flags_count; i++)
	{
		if (special_flags_name[i])
		{
			uSetDlgItemText(hwnd, IDC_SPECIAL_FLAG1 + i, special_flags_name[i]);
			bool checked = (cur_special_flags->SpecialFlags >> i) & 0x01;
			CheckDlgButton(hwnd, IDC_SPECIAL_FLAG1 + i, checked);
			EnableWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + i), TRUE);
			ShowWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + i), SW_SHOW);
		}
		else
		{
			uSetDlgItemText(hwnd, IDC_SPECIAL_FLAG1 + i, "");
			EnableWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + i), FALSE);
			ShowWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + i), SW_HIDE);
		}

	}

	for (UINT8 j = special_flags_count; j < 16; j++)
	{
		uSetDlgItemText(hwnd, IDC_SPECIAL_FLAG1 + j, "");
		EnableWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + j), FALSE);
		ShowWindow(GetDlgItem(hwnd, IDC_SPECIAL_FLAG1 + j), SW_HIDE);
	}
}

void preferences_page_instance_chips_options::set_special_flags(UINT check_box, bool checked)
{
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	//Currently ChipOptions are shared between Chip#1 and Chip#2
	CHIP_OPTS* cur_special_flags = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[0]->get_ptr()) + chip_id;

	cur_special_flags->SpecialFlags &= (~(1 << check_box));
	cur_special_flags->SpecialFlags |= (checked << check_box);
}


class preferences_page_chips_options : public preferences_page_v3
{
public:

	virtual const char * get_name()
	{
		return "Chips Options";
	}

	virtual GUID get_guid()
	{
		return guid_preferences_page_chips_options;
	}

	virtual GUID get_parent_guid()
	{
		return guid_preferences_page_foo_input_vgm;
	}

	virtual preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback)
	{
		return new service_impl_t<preferences_page_instance_chips_options>(parent, callback);
	}

};

static preferences_page_factory_t<preferences_page_chips_options> g_preferences_page_chips_options;