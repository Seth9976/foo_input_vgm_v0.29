#include "stdafx.h"

void cfg_chips_options::get_data_raw(stream_writer * p_stream, abort_callback & p_abort)
{
	stream_writer_formatter<> out(*p_stream, p_abort);
	out << CHIP_COUNT;

	for (unsigned i = 0; i < CHIP_COUNT; i++)
	{
		CHIP_OPTS *cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(&m_chips_opt) + i;
		t_uint32 temp = cur_chip_opts->Disabled ? 1 : 0;
		out << temp;
		out << cur_chip_opts->EmuCore;
		out << cur_chip_opts->SpecialFlags;
		out << cur_chip_opts->ChnMute1;
		out << cur_chip_opts->ChnMute2;
		out << cur_chip_opts->ChnMute3;
		
		out << cur_chip_opts->ChnCnt;

		if (cur_chip_opts->ChnCnt && cur_chip_opts->Panning)
			p_stream->write_object(cur_chip_opts->Panning, cur_chip_opts->ChnCnt * sizeof(cur_chip_opts->Panning[0]), p_abort);
	}
}

void cfg_chips_options::set_data_raw(stream_reader * p_stream, t_size p_sizehint, abort_callback & p_abort)
{
	stream_reader_formatter<> in(*p_stream, p_abort);
	t_uint32 chip_count = 0;
	in >> chip_count;

	for (unsigned i = 0; i < chip_count; i++)
	{
		CHIP_OPTS *cur_chip_opts = reinterpret_cast<CHIP_OPTS*>(&m_chips_opt) + i;
		t_uint8 temp8 = 0;
		t_uint32 temp32 = 0;

		in >> temp32;
		cur_chip_opts->Disabled = temp32 > 0 ? true : false;
		in >> cur_chip_opts->EmuCore;
		in >> cur_chip_opts->SpecialFlags;
		in >> cur_chip_opts->ChnMute1;
		in >> cur_chip_opts->ChnMute2;
		in >> cur_chip_opts->ChnMute3;

		in >> temp8;	//ChnCnt
		if (temp8)
		{
			if (cur_chip_opts->ChnCnt >= temp8 && cur_chip_opts->Panning)
				p_stream->read_object(cur_chip_opts->Panning, temp8 * sizeof(INT16), p_abort);
			else
				p_stream->skip(temp8 * sizeof(INT16), p_abort);
		}
	}
}