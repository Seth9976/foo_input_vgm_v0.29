#pragma warning(disable : 4996)

#include "resource.h"
#include "stdafx.h"
#include "../helpers/helpers.h"

// {72EE8F23-AEF1-4627-ACD9-A580DD63EA3A}
static const GUID guid_preferences_page_other_options =
{ 0x72ee8f23, 0xaef1, 0x4627, { 0xac, 0xd9, 0xa5, 0x80, 0xdd, 0x63, 0xea, 0x3a } };


//Other Options preferences instance class. Any changes will be immediately saved regardless of OK/Cancel/Apply.
class preferences_page_instance_other_options : public preferences_page_instance
{
private:
	HWND m_parent;
	HWND m_hwnd;
	preferences_page_callback::ptr m_callback;
	bool m_initialized;

	static void show_other_options(HWND hwnd);
	static void set_panning(t_uint32 slider, t_uint16 value);

public:
	static BOOL CALLBACK config_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

	// Constructor
	preferences_page_instance_other_options(HWND parent, preferences_page_callback::ptr callback) :
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
		return uCreateDialog(IDD_DIALOG3, p_parent, config_proc, reinterpret_cast<LPARAM>(this));
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
			cfg_chips_opts[i]->reset<chips_options_ex::emulation_core | chips_options_ex::panning, false>();
		}
		show_other_options(m_hwnd);
	}
};


BOOL CALLBACK preferences_page_instance_other_options::config_proc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{
		uSetWindowLong(wnd, DWL_USER, lp);
		preferences_page_instance_other_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_other_options*>(lp);
		if (p_this) p_this->m_initialized = false;

		//combo box
		HWND h = uGetDlgItem(wnd, IDC_CHIP_ID);
		for (unsigned i = 0; i < CHIP_COUNT; i++)
		{
			uSendMessageText(h, CB_ADDSTRING, 0, my_chip_name[i]);
		}
		::SendMessage(h, CB_SETCURSEL, cfg_chip_id_index, 0);

		h = uGetDlgItem(wnd, IDC_CHIP_SET);
		uSendMessageText(h, CB_ADDSTRING, 0, "Chip #1");
		uSendMessageText(h, CB_ADDSTRING, 0, "Chip #2");
		::SendMessage(h, CB_SETCURSEL, cfg_chip_set_index, 0);

		//slider control
		for (unsigned i = 0; i < 15; i++)
		{ 
			h = uGetDlgItem(wnd, IDC_PAN_SLIDER1 + i);
			uSendMessage(h, TBM_SETRANGE, 0, MAKELONG(0, 0x40));
			uSendMessage(h, TBM_SETTICFREQ, 0x8, 0);
			uSendMessage(h, TBM_SETLINESIZE, 0, 1);
			uSendMessage(h, TBM_SETPAGESIZE, 0, 0x8);
		}
		
		show_other_options(wnd);

		//if (p_this) p_this->m_initialized = true;
		break;
	}
	case WM_COMMAND:
	{
		preferences_page_instance_other_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_other_options*> (uGetWindowLong(wnd, DWL_USER));
		//CHIP_OPTS_AT* cur_chip_opts = reinterpret_cast<CHIP_OPTS_AT*>(cfg_chip_opts[cfg_chip_set_index].get_ptr()) + cfg_chip_id_index;

		switch (LOWORD(wp))
		{
		case IDC_CHIP_ID:
		case IDC_CHIP_SET:
		{
			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				cfg_chip_id_index = uSendDlgItemMessage(wnd, IDC_CHIP_ID, CB_GETCURSEL, 0, 0);
				cfg_chip_set_index = uSendDlgItemMessage(wnd, IDC_CHIP_SET, CB_GETCURSEL, 0, 0);
				//show emulation core radio bottun
				show_other_options(wnd);
			}
			break;
		}
		default:
			if (LOWORD(wp) >= IDC_EMU_CORE1 && LOWORD(wp) < IDC_EMU_CORE1 + EMU_CORE_COUNT)
			{
				CHIP_OPTS *temp = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[0]->get_ptr()) + cfg_chip_id_index;
				temp->EmuCore = LOWORD(wp) - IDC_EMU_CORE1;
				show_other_options(wnd);
				break;
			}
			break;
		}
		break;
	}
	case WM_HSCROLL:
	{
		
		int slider_id = GetDlgCtrlID((HWND)lp);

		if (slider_id >= IDC_PAN_SLIDER1 && slider_id <= IDC_PAN_SLIDER15)
		{
			t_uint16 pos;

			if (LOWORD(wp) == TB_THUMBTRACK || LOWORD(wp) == TB_THUMBPOSITION)
				pos = HIWORD(wp);
			else
				pos = static_cast<t_uint16>(uSendDlgItemMessage(wnd, slider_id, TBM_GETPOS, 0, 0));

			set_panning(slider_id - IDC_PAN_SLIDER1, pos);
		}
		break;
	}
	case WM_WINDOWPOSCHANGED:
	{
		preferences_page_instance_other_options* p_this;
		p_this = reinterpret_cast<preferences_page_instance_other_options*> (uGetWindowLong(wnd, DWL_USER));

		WINDOWPOS* p_wpos = reinterpret_cast<WINDOWPOS*>(lp);
		//BOOL enabled = IsWindowEnabled(wnd);
		if (p_this && p_this->m_initialized && p_wpos && p_wpos->flags & SWP_SHOWWINDOW)
		{
			uSendDlgItemMessage(wnd, IDC_CHIP_ID, CB_SETCURSEL, cfg_chip_id_index, 0);
			uSendDlgItemMessage(wnd, IDC_CHIP_SET, CB_SETCURSEL, cfg_chip_set_index, 0);
			show_other_options(wnd);
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
void preferences_page_instance_other_options::show_other_options(HWND hwnd)
{
	UINT8 channel_count;
	UINT8 cur_channel;
	const char* special_channel_name[0x20];	// Special Channel Names
	const char* core_name[EMU_CORE_COUNT];	// Names for the Emulation Cores
	bool multi_core[3];
	bool enable_panning;
	INT16 panning_pos;
	char temp_name[0x20];
	const char* temp_str;
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	for (cur_channel = 0x00; cur_channel < 0x20; cur_channel++)
		special_channel_name[cur_channel] = NULL;

	for (unsigned int i = 0; i < EMU_CORE_COUNT; i++)
		core_name[i] = 0;
	core_name[0x00] = "MAME";

	//Currently EmuCore is shared between Chip#1 and Chip#2
	CHIP_OPTS *temp_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[0]->get_ptr()) + chip_id;
	for (unsigned int i = 0; i < EMU_CORE_COUNT; i++)
		multi_core[i] = false;
	enable_panning = false;
	channel_count = 0;
	switch (chip_id)
	{
	case 0x00:	// SN76496
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x01] = "Maxim";
		enable_panning = (temp_chip_opts->EmuCore == 0x01);

		channel_count = 4;
		special_channel_name[3] = "&Noise";
		break;
	case 0x01:	// YM2413
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "EMU2413";
		core_name[0x01] = "MAME";
		enable_panning = (temp_chip_opts->EmuCore == 0x00);

		channel_count = 14;	// 9 + 5
		special_channel_name[9] = "C&ymbal";
		special_channel_name[10] = "&Tom Tom";
		special_channel_name[11] = "Snare &Drum";	
		special_channel_name[12] = "&Hi-Hat";
		special_channel_name[13] = "&Bass Drum";
		break;
	case 0x02:	// YM2612
		multi_core[0] = true;
		multi_core[1] = true;
		multi_core[2] = true;
		core_name[0x01] = "Nuked OPN2";
		core_name[0x02] = "Gens";
		break;
	case 0x06:	// YM2203
	case 0x07:	// YM2608
	case 0x08:	// YM2610
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "EMU2149";
		core_name[0x01] = "MAME";
		break;
		/*case 0x06:	// YM2203
		channel_count = 6;	// 3 FM + 3 AY8910
		SpcModes = 2;
		ChnCntS[0] = 3;
		special_channel_name[0] = "FM Chn";
		ChnCntS[1] = 3;
		special_channel_name[1] = "PSG Chn";
		break;*/
	case 0x09:	// YM3812
	case 0x0C:	// YMF262
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "AdLibEmu";
		core_name[0x01] = "MAME";
		break;
	case 0x0D:	// YMF272B
		core_name[0x00] = "openMSX";
		break;
	case 0x10:	// RF5C164
	case 0x11:	// PWM
		core_name[0x00] = "Gens";
		break;
	case 0x12:	// AY8910
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "EMU2149";
		core_name[0x01] = "MAME";
		break;
	case 0x14:	// NES APU
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "NSFPlay";
		core_name[0x01] = "MAME";
		break;
	case 0x1B:	// HuC6280
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "Ootake";
		core_name[0x01] = "MAME";
		break;
	case 0x1F:	// QSound
		multi_core[0] = true;
		multi_core[1] = true;
		core_name[0x00] = "superctr";	
		core_name[0x01] = "MAME";
		break;	
	case 0x21: //WonderSwan
		core_name[0x00] = "in_wsr";
		break;
	case 0x22:	//VSU-VUE
		core_name[0x00] = "vbjin/mednafen";
		break;
	case 0x27:	// C352
		core_name[0x00] = "superctr";
		break;
	default:
		//channel_count = 0;
		break;
	}

	CHIP_OPTS *panning_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[chip_set]->get_ptr()) + chip_id;
	if (!panning_chip_opts->ChnCnt || !panning_chip_opts->Panning)
		enable_panning = false;

	if (channel_count > 15)
		channel_count = 15;	// there are only 15 sliders

	for (cur_channel = 0x00; cur_channel < EMU_CORE_COUNT; cur_channel++)
	{
		EnableWindow(GetDlgItem(hwnd, IDC_EMU_CORE1 + cur_channel), multi_core[cur_channel]);
		if (cur_channel == 0)
			temp_str = "Default";
		else if (cur_channel == 1)
			temp_str = "Alternative";
		else
			temp_str = "Alternative 2";
		if (!core_name[cur_channel])
			sprintf(temp_name, "%s Core", temp_str);
		else
			sprintf(temp_name, "%s (%s)", temp_str, core_name[cur_channel]);
		panning_pos = pfc::strlen_utf8(temp_name);
		uSetDlgItemText(hwnd, IDC_EMU_CORE1 + cur_channel, temp_name);
	}

	CheckRadioButton(hwnd, IDC_EMU_CORE1, (IDC_EMU_CORE1+EMU_CORE_COUNT-1),
		IDC_EMU_CORE1 + temp_chip_opts->EmuCore);

	for (cur_channel = 0; cur_channel < channel_count; cur_channel++)
	{
		if (special_channel_name[cur_channel] == NULL)
		{
			if (1 + cur_channel < 10)
				sprintf(temp_name, "Channel &%u", 1 + cur_channel);
			else
				sprintf(temp_name, "Channel &%c", 'A' + (cur_channel - 9));
			
			uSetDlgItemText(hwnd, IDC_PAN_LABEL1 + cur_channel, temp_name);
		}
		else
		{
			uSetDlgItemText(hwnd, IDC_PAN_LABEL1 + cur_channel, special_channel_name[cur_channel]);
		}
		EnableWindow(GetDlgItem(hwnd, IDC_PAN_LABEL1 + cur_channel), enable_panning);

		if (panning_chip_opts->Panning)
		{
			if (enable_panning)
				panning_pos = panning_chip_opts->Panning[cur_channel] / 0x08 + 0x20;
			else
				panning_pos = 0x20;
			SendDlgItemMessage(hwnd, IDC_PAN_SLIDER1 + cur_channel, TBM_SETPOS, TRUE, panning_pos);
		}
		EnableWindow(GetDlgItem(hwnd, IDC_PAN_SLIDER1 + cur_channel), enable_panning);
	}
	panning_pos = 0x20;
	for (; cur_channel < 15; cur_channel++)
	{
		if (1 + cur_channel < 10)
			sprintf(temp_name, "Channel &%u", 1 + cur_channel);
		else
			sprintf(temp_name, "Channel &%c", 'A' + (cur_channel - 9));
		uSetDlgItemText(hwnd, IDC_PAN_LABEL1 + cur_channel, temp_name);

		EnableWindow(GetDlgItem(hwnd, IDC_PAN_LABEL1 + cur_channel), FALSE);
		EnableWindow(GetDlgItem(hwnd, IDC_PAN_SLIDER1 + cur_channel), FALSE);
		SendDlgItemMessage(hwnd, IDC_PAN_SLIDER1 + cur_channel, TBM_SETPOS, TRUE, panning_pos);
	}
}

void preferences_page_instance_other_options::set_panning(t_uint32 slider, t_uint16 value)
{
	t_uint32 chip_id = cfg_chip_id_index;
	t_uint32 chip_set = cfg_chip_set_index;

	CHIP_OPTS *temp_chip_opts = reinterpret_cast<CHIP_OPTS*>(cfg_chips_opts[chip_set]->get_ptr()) + chip_id;
	if (slider >= temp_chip_opts->ChnCnt || !temp_chip_opts->Panning)
		return;

	temp_chip_opts->Panning[slider] = (value - 0x20) * 0x08;
}

class preferences_other_options : public preferences_page_v3
{
public:

	virtual const char * get_name()
	{
		return "Other Options";
	}

	virtual GUID get_guid()
	{
		return guid_preferences_page_other_options;
	}

	virtual GUID get_parent_guid()
	{
		return guid_preferences_page_foo_input_vgm;
	}

	virtual preferences_page_instance::ptr instantiate(HWND parent, preferences_page_callback::ptr callback)
	{
		return new service_impl_t<preferences_page_instance_other_options>(parent, callback);
	}

};

static preferences_page_factory_t<preferences_other_options> g_preferences_other_options;
