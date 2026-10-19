#pragma once
#include "../../vgm_player/VGMPlay/VGMPlay.h"

class chips_options_ex
{
protected:
	CHIPS_OPTION m_chips_opt;
public:
	enum chips_options_mode
	{
		muting = 1,
		special_flags = 2, 
		emulation_core = 4,
		panning = 8
	};
	
	chips_options_ex()
	{
		reset<~0, true>();
	};

	~chips_options_ex()
	{
		if (m_chips_opt.SN76496.ChnCnt && m_chips_opt.SN76496.Panning)
			delete[] m_chips_opt.SN76496.Panning;
		if (m_chips_opt.YM2413.ChnCnt && m_chips_opt.YM2413.Panning)
			delete[] m_chips_opt.YM2413.Panning;

		m_chips_opt.SN76496.ChnCnt = 0;
		m_chips_opt.YM2413.ChnCnt = 0;
		m_chips_opt.SN76496.Panning = 0;
		m_chips_opt.YM2413.Panning = 0;
	}

	template<t_size chips_options_modes, bool allocate_panning_buf = false>
	void reset();

	template<t_size chips_options_modes>
	void inline copy(CHIPS_OPTION *p_dest, UINT8 chip_id);

	template<t_size chips_options_modes>
	bool inline compare(CHIPS_OPTION *p_compare, UINT8 chip_id);

	inline CHIPS_OPTION *get_ptr() { return &m_chips_opt; }
	inline const CHIPS_OPTION *get_ptr() const { return &m_chips_opt; }
	inline operator CHIPS_OPTION * () { return get_ptr(); }
	inline operator const CHIPS_OPTION * ()  const { return get_ptr(); }

	chips_options_ex(const chips_options_ex &) = delete;
	chips_options_ex& operator=(const chips_options_ex &) = delete;
};
#include "my_cfg_var_template_detail.h"

class cfg_chips_options : public cfg_var, public chips_options_ex
{
public:
	explicit cfg_chips_options(const GUID& guid) : cfg_var(guid), chips_options_ex() {};
	void get_data_raw(stream_writer * p_stream, abort_callback & p_abort);
	void set_data_raw(stream_reader * p_stream, t_size p_sizehint, abort_callback & p_abort);

	cfg_chips_options(const chips_options &) = delete;
	cfg_chips_options& operator=(const cfg_chips_options &) = delete;
};