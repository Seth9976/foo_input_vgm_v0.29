#pragma once

template<t_size chips_options_modes, bool allocate_panning_buf>
void chips_options_ex::reset()
{
	for (unsigned i = 0; i < CHIP_COUNT; i++)
	{
		CHIP_OPTS* cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(&m_chips_opt) + i;

		if (chips_options_modes & emulation_core)
			cur_chip_opts->EmuCore = 0;

		if (chips_options_modes & special_flags)
			cur_chip_opts->SpecialFlags = 0;

		if (chips_options_modes & muting)
		{
			cur_chip_opts->Disabled = false;
			cur_chip_opts->ChnMute1 = 0;
			cur_chip_opts->ChnMute2 = 0;
			cur_chip_opts->ChnMute3 = 0;
		}

		if ((chips_options_modes & panning) && allocate_panning_buf)
		{
			cur_chip_opts->ChnCnt = 0;
			cur_chip_opts->Panning = nullptr;
		}
	}

	if (chips_options_modes & special_flags)
	{
		m_chips_opt.GameBoy.SpecialFlags = 0x1;
		m_chips_opt.NES.SpecialFlags = ((0x00 << 12) | (0x3B << 4) | (0x01 << 2) | (0x03 << 0));
		m_chips_opt.SCSP.SpecialFlags = 0x1;
	}

	if (chips_options_modes & panning)
	{
		if (allocate_panning_buf)
		{
			m_chips_opt.SN76496.ChnCnt = 0x4;
			m_chips_opt.YM2413.ChnCnt = 0xE;
			
			const char *err_msg = "Cannot allocate memory for panning.";
			try
			{
				m_chips_opt.SN76496.Panning = new INT16[m_chips_opt.SN76496.ChnCnt];
			}
			catch (...)
			{
				m_chips_opt.SN76496.ChnCnt = 0x0;
				m_chips_opt.SN76496.Panning = nullptr;
				console::complain("VGM input Error", err_msg);
			}
			try
			{
				m_chips_opt.YM2413.Panning = new INT16[m_chips_opt.YM2413.ChnCnt];
			}
			catch (...)
			{
				m_chips_opt.YM2413.ChnCnt = 0x0;
				m_chips_opt.YM2413.Panning = nullptr;
				console::complain("VGM input Error", err_msg);
			}
		}
		
		if (m_chips_opt.SN76496.ChnCnt && m_chips_opt.SN76496.Panning)
			memset(m_chips_opt.SN76496.Panning, 0, m_chips_opt.SN76496.ChnCnt * sizeof(m_chips_opt.SN76496.Panning[0]));
		if (m_chips_opt.YM2413.ChnCnt && m_chips_opt.YM2413.Panning)
			memset(m_chips_opt.YM2413.Panning, 0, m_chips_opt.YM2413.ChnCnt * sizeof(m_chips_opt.YM2413.Panning[0]));
	}
}

template<t_size chips_options_modes>
void chips_options_ex::copy(CHIPS_OPTION *p_dest, UINT8 chip_id)
{
	if (chip_id == 0xFF) return;
	chip_id &= 0x7F;
	if (chip_id >= CHIP_COUNT) return;

	CHIP_OPTS *src = reinterpret_cast<CHIP_OPTS*>(&m_chips_opt) + chip_id;
	CHIP_OPTS *dest = reinterpret_cast<CHIP_OPTS*>(p_dest)+chip_id;

	if (chips_options_modes & emulation_core)
		dest->EmuCore = src->EmuCore;

	if (chips_options_modes & special_flags)
		dest->SpecialFlags = src->SpecialFlags;

	if (chips_options_modes & panning)
	{
		if (src->ChnCnt && src->Panning && dest->ChnCnt && dest->Panning)
		{
			memcpy(dest->Panning, src->Panning, pfc::min_t(src->ChnCnt, dest->ChnCnt) * sizeof(src->Panning[0]));
		}
	}

	if (chips_options_modes & muting)
	{
		dest->Disabled = src->Disabled;
		dest->ChnMute1 = src->ChnMute1;
		dest->ChnMute2 = src->ChnMute2;
		dest->ChnMute3 = src->ChnMute3;
	}
}

template<t_size chips_options_modes>
bool chips_options_ex::compare(CHIPS_OPTION *p_compare, UINT8 chip_id)
{
	if (chip_id == 0xFF) return false;
	chip_id &= 0x7F;
	if (chip_id >= CHIP_COUNT) return false;

	CHIP_OPTS *src = reinterpret_cast<CHIP_OPTS*>(&m_chips_opt) + chip_id;
	CHIP_OPTS *compare = reinterpret_cast<CHIP_OPTS*>(p_compare)+chip_id;

	if (chips_options_modes & emulation_core)
	{
		if (src->EmuCore != compare->EmuCore)
			return false;
	}

	if (chips_options_modes & special_flags)
	{
		if (src->SpecialFlags != compare->SpecialFlags)
			return false;
	}

	if (chips_options_modes & panning)
	{
		if (src->ChnCnt && src->Panning && compare->ChnCnt && compare->Panning)
		{
			if (memcmp(compare->Panning, src->Panning, pfc::min_t(src->ChnCnt, compare->ChnCnt) * sizeof(src->Panning[0])))
				return false;
		}
	}

	if (chips_options_modes & muting)
	{
		if (src->Disabled != compare->Disabled)
			return false;
		if (src->ChnMute1 != compare->ChnMute1)
			return false;
		if (src->ChnMute2 != compare->ChnMute2)
			return false;
		if (src->ChnMute3 != compare->ChnMute3)
			return false;
	}

	return true;
}