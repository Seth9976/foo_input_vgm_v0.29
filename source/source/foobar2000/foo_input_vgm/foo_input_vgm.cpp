// foobar2000 component for playing vgm and vgz file


//#include "../../vgm_player/VGMPlay/chips/mamedef.h"	// for (U)INTxx types
//#include "../../vgm_player/VGMPlay/VGMPlay.h"
//#include "../../vgm_player/VGMPlay/VGMPlay_Intf.h"
#include "stdafx.h"
#include "MemoryModule/MemoryModule.h"
#include "foo_input_vgm.h"
#include <math.h>

static_assert(sizeof(t_size) >= sizeof(UINT32), "t_size is too short");
static_assert(sizeof(t_filesize) >= sizeof(UINT32), "t_filesize is too short");

#define DLL_NAME "VGM_PLAYER.DLL"


const unsigned int input_vgm::m_bps = 16;
const unsigned int input_vgm::m_channels = 2;
//static const unsigned int m_render_ms = 10;
const unsigned int input_vgm::m_render_sample = 588;
const GUID input_vgm::m_decoder_priority_table_guid = { 0x2b993715, 0x81e0, 0x46a4,{ 0xb0, 0x2d, 0x34, 0x33, 0x7f, 0xae, 0xa4, 0x6e } };

static const UINT32 max_file_len = 2147483648;
static const UINT32 max_gd3_len = 1073741824;


#define GETVGMPLAYAPIADDRESS(api, dll_hnd, func_name) \
{	\
(api).VGMPlay_Init					= (p_VGMPlay)func_name((dll_hnd), "VGMPlay_Init");	\
(api).VGMPlay_Init2					= (p_VGMPlay)func_name((dll_hnd), "VGMPlay_Init2");	\
(api).VGMPlay_Deinit				= (p_VGMPlay)func_name((dll_hnd), "VGMPlay_Deinit");	\
(api).CloseVGMFile					= (p_VGMPlay)func_name((dll_hnd), "CloseVGMFile");	\
(api).PlayVGM						= (p_VGMPlay)func_name((dll_hnd), "PlayVGM");	\
(api).StopVGM						= (p_VGMPlay)func_name((dll_hnd), "StopVGM");	\
/*(api).RestartVGM					= (p_VGMPlay)func_name((dll_hnd), "RestartVGM");*/	\
(api).RefreshMuting					= (p_VGMPlay)func_name((dll_hnd), "RefreshMuting");	\
(api).RefreshPanning				= (p_VGMPlay)func_name((dll_hnd), "RefreshPanning");	\
/*(api).RefreshPlaybackOptions		= (p_VGMPlay)func_name((dll_hnd), "RefreshPlaybackOptions");*/	\
	\
/*(api).FindFile					= (p_FindFile)func_name((dll_hnd),					"FindFile");*/	\
/*(api).GetGZFileLength				= (p_GetGZFileLength)func_name((dll_hnd),			"GetGZFileLength");*/	\
(api).GetGZFileLength2				= (p_GetGZFileLength2)func_name((dll_hnd),			"GetGZFileLength2");	\
/*(api).OpenVGMFile					= (p_OpenVGMFile)func_name((dll_hnd),				"OpenVGMFile");*/	\
(api).OpenVGMFile2					= (p_OpenVGMFile2)func_name((dll_hnd),				"OpenVGMFile2");	\
(api).FreeGD3Tag					= (p_FreeGD3Tag)func_name((dll_hnd),				"FreeGD3Tag");	\
/*(api).GetVGMFileInfo				= (p_GetVGMFileInfo)func_name((dll_hnd),			"GetVGMFileInfo");*/	\
(api).GetVGMFileInfo2				= (p_GetVGMFileInfo2)func_name((dll_hnd),			"GetVGMFileInfo2");	\
/*(api).CalcSampleMSec				= (p_CalcSampleMSec)func_name((dll_hnd),			"CalcSampleMSec");*/	\
/*(api).CalcSampleMSecExt			= (p_CalcSampleMSecExt)func_name((dll_hnd),			"CalcSampleMSecExt");*/	\
/*(api).GetChipName					= (p_GetChipName)func_name((dll_hnd),				"GetChipName");*/	\
(api).GetAccurateChipName			= (p_GetAccurateChipName)func_name((dll_hnd),		"GetAccurateChipName");	\
(api).GetChipClock					= (p_GetChipClock)func_name((dll_hnd),				"GetChipClock");	\
/*(api).SampleVGM2Playback			= (p_SampleVGM2Playback)func_name((dll_hnd),		"SampleVGM2Playback");*/	\
/*(api).SampleVGM2Playback			= (p_SampleVGM2Playback)func_name((dll_hnd),		"SamplePlayback2VGM");*/	\
(api).PauseVGM						= (p_PauseVGM)func_name((dll_hnd),					"PauseVGM");	\
(api).SeekVGM						= (p_SeekVGM)func_name((dll_hnd),					"SeekVGM");	\
(api).FillBuffer					= (p_FillBuffer)func_name((dll_hnd),				"FillBuffer");	\
(api).SetVGMPlaybackOptions			= (p_SetVGMPlaybackOptions)func_name((dll_hnd),		"SetVGMPlaybackOptions");	\
(api).SetVGMChipsOption				= (p_SetVGMChipsOption)func_name((dll_hnd),			"SetVGMChipsOption");	\
/*(api).GetPointerAppPaths			= (p_GetPointerAppPaths)func_name((dll_hnd),		"GetPointerAppPaths");*/	\
(api).GetPointerChipsOption			= (p_GetPointerChipsOption)func_name((dll_hnd),		"GetPointerChipsOption");	\
(api).GetPointerEndPlayFlag			= (p_GetPointerEndPlayFlag)func_name((dll_hnd),		"GetPointerEndPlayFlag");	\
	\
(api).UncompressGZFile				= (p_UncompressGZFile)func_name((dll_hnd),			"UncompressGZFile");	\
(api).CompressFile					= (p_CompressFile)func_name((dll_hnd),				"CompressFile");	\
}


// Load vgm_player.dll
bool input_vgm::load_dll(bool unique, abort_callback & p_abort)
{
	if (m_dll_hnd)
		return true;

	if (!unique)
	{
		m_dll_hnd = (HMEMORYMODULE)g_dll_hnd;
		m_api = g_api;
	}
	else
	{
		pfc::string8 path = core_api::get_my_full_path();
		path.truncate(path.scan_filename());
		path += DLL_NAME;

		service_ptr_t<file> p_module;
		pfc::array_t<t_uint8> module_buf;

		filesystem::g_open(p_module, path, filesystem::open_mode_read, p_abort);
		size_t module_len = static_cast<size_t>(p_module->get_size(p_abort));
		module_buf.set_size(module_len + 16);
		p_module->read(module_buf.get_ptr(), module_len, p_abort);

		m_dll_hnd = MemoryLoadLibrary(module_buf.get_ptr(), module_len);

		if (m_dll_hnd)
		{
			GETVGMPLAYAPIADDRESS(m_api, m_dll_hnd, MemoryGetProcAddress)
		}
	}

	if (!m_dll_hnd) {
		console::info("Could not locate VGM_PLAYER.DLL.");
		return false;
	}

	if (!m_api.VGMPlay_Init || !m_api.VGMPlay_Deinit || !m_api.PlayVGM || !m_api.PauseVGM || 
		!m_api.OpenVGMFile2 || !m_api.SeekVGM || !m_api.FillBuffer || !m_api.GetPointerChipsOption ||
		!m_api.VGMPlay_Init2 || !m_api.StopVGM || !m_api.CloseVGMFile || !m_api.GetAccurateChipName || 
		!m_api.GetVGMFileInfo2 || !m_api.FreeGD3Tag || !m_api.GetPointerEndPlayFlag || !m_api.GetChipClock ||
		!m_api.RefreshMuting || !m_api.UncompressGZFile || !m_api.CompressFile || !m_api.GetGZFileLength2 ||
		!m_api.RefreshPanning
		)
	{
		console::info("VGM_PLAYER.DLL function lookups failed.");
		return false;
	}

	return true;
}

// Free vgm_player.dll
void input_vgm::free_dll()
{
	if (!m_dll_hnd)
		return;

	if (m_open_reason == input_open_decode)
		MemoryFreeLibrary(m_dll_hnd);
	
	m_dll_hnd = 0;
}

void input_vgm::set_current_vgm_chip_info()
{
	UINT8 chip_id(0xFF);
	UINT32 chip_clock(0);
	UINT8 chip_type(0);
	UINT8 pos1(0);
	//UINT8 pos2(0);

	for (unsigned cur_chip = 0x00; cur_chip < CHIP_COUNT; cur_chip++)
	{
		chip_clock = m_api.GetChipClock(&m_vgm_header, cur_chip, &chip_type);
		if (chip_clock && m_api.GetChipClock(&m_vgm_header, 0x80 | cur_chip, 0))
			chip_clock |= 0x40000000;

		if (!chip_clock) continue;

		chip_id = cur_chip;

		if (chip_id == 0x00 && (chip_clock & 0x80000000)) //T6W28
			chip_clock &= ~0x40000000;
		if (chip_clock & 0x80000000)
		{
			chip_clock &= ~0x80000000;
			chip_id |= 0x80;
		}
	
			m_chip_info[pos1].chip_id = chip_id;
			m_chip_info[pos1].chip_sub_type = chip_type;
		
			if ((chip_clock & 0x40000000))
			{
				//Dual Chip
				m_chip_info[pos1].chip_set = 2;
			}
			else
				m_chip_info[pos1].chip_set = 1;

			pos1++;
	}
}

// Constructor
input_vgm::input_vgm() : 
m_dll_hnd(0),			m_file(0),				m_initialized(false),			m_set_len(false), 
m_sample_rate(44100),	m_fade_len(5000),		m_loop_count(2),				m_play_infinitely(false), 
m_render_done(0),		m_played_sample(0),		m_dynamic_info(false),			m_song_sample(44100),
m_loop_flag(false),		m_opened(false),		m_got_vgm_file_info(false),		m_played(false), 
m_decode_flags(0),		p_VGMPlayEndPlay(0),	p_VGMPlayChipOpts(0),			m_gz(false), 
m_recomp_level(6)
//, m_render_sample(0)
{

	m_gd3_tag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		for (UINT8 i = 0; i < CHIP_COUNT; i++)
		{
			m_chip_info[i].chip_id = 0xFF;
			m_chip_info[i].chip_sub_type = 0x00;
			m_chip_info[i].chip_set = 1;
		}
}

// Destructor
input_vgm::~input_vgm()
{
	// free memory if need
	if (m_got_vgm_file_info && m_api.FreeGD3Tag)	m_api.FreeGD3Tag(&m_gd3_tag);
	if (m_played && m_api.StopVGM)					m_api.StopVGM();
	if (m_opened && m_api.CloseVGMFile)				m_api.CloseVGMFile();
	if (m_initialized && m_api.VGMPlay_Deinit)		m_api.VGMPlay_Deinit();
	m_got_vgm_file_info		= false;
	m_played				= false;
	m_opened				= false;
	m_initialized			= false;

	free_dll();
}


// Supported Format
bool input_vgm::g_is_our_path(const char *p_path, const char *p_extension)
{
	if (cfg_vgm > 0 && (::stricmp_utf8(p_extension, "vgm") == 0 || ::stricmp_utf8(p_extension, "vgz") == 0))
		return true;

	return false;
}

// Open file
void input_vgm::open(service_ptr_t<file> p_filehint, const char *p_path, t_input_open_reason p_reason, abort_callback &p_abort)
{
	m_file = p_filehint;
	m_file_path = p_path;
	m_open_reason = p_reason;
	//pfc::string_extension ext(p_path);
	//pfc::stringToUpperAppend(m_file_ext, ext, ~0);

	// open input file
	input_open_file_helper(m_file, p_path, p_reason, p_abort);

	// read input file
	t_filesize temp_file_len = m_file->get_size(p_abort);
	if (temp_file_len >= 0xFFFFFFFF)
		throw exception_io_unsupported_format();

	m_file_len = static_cast<UINT32>(temp_file_len);
	t_size file_buf_len = m_file_len;
	if (m_file_len <= 0xFFFFFFEF)
		file_buf_len += 16;
	m_file_buf.resize(file_buf_len);
	m_file->read(&m_file_buf[0], m_file_len, p_abort);
	
	
	// load vgm_player.dll
	bool load_unique(false);
	if (p_reason == input_open_decode)
		load_unique = true;
	else
		load_unique = false;

	if (!load_dll(load_unique, p_abort))
		throw exception_io_data("Unable to load VGM_PLAYER.DLL");
	
	UINT32 ret_get_info(0);
	if (p_reason == input_open_info_read)
	{
		// get vgm file information
		ret_get_info = m_api.GetVGMFileInfo2(&m_file_buf[0], m_file_len, &m_vgm_header, &m_gd3_tag);
	}
	else
	{		
		//p_reason == input_open_decode or input_open_info_write
		UINT32 ret_temp = m_api.GetGZFileLength2(&m_file_buf[0], m_file_len, &m_gz);
		if (ret_temp == 0xFFFFFFFF) throw exception_io_data("Could not uncompress VGZs because file length was unknown");

		if (m_gz)
		{
			//Uncompress GZip-Compressed File
			ret_temp = pfc::add_unsigned_clipped<UINT32>(ret_temp, 0xFF);
			std::vector<t_uint8> uncomp_file_buf(ret_temp);

			ret_temp = m_api.UncompressGZFile(&m_file_buf[0], m_file_len, &uncomp_file_buf[0], ret_temp);
			if (!ret_temp) throw exception_io_data("Could not uncompress VGZs");
#ifdef _DEBUG
			else if (ret_temp == 0xFFFFFFFF) { m_gz = false;}

			if (m_gz)
#endif
			{
				m_file_buf = std::move(uncomp_file_buf);
				m_file_len = ret_temp;
			}
		}
		
		ret_get_info = m_api.GetVGMFileInfo2(&m_file_buf[0], m_file_len, &m_vgm_header, &m_gd3_tag);		
	}

	if (!ret_get_info)
		throw exception_io_unsupported_format();
	m_got_vgm_file_info = true;
	

	// load settings
	m_loop_flag = m_vgm_header.lngLoopSamples ? true : false;
	m_resampling_mode = cfg_resampling_mode;
	m_chip_sample_mode = cfg_chip_sample_mode;
	m_sample_rate = cfg_sample_rate > 0 ? cfg_sample_rate : 44100;
	m_chip_rate = cfg_chip_rate;
	m_playback_rate = cfg_playback_rate;
	m_fade_len = cfg_fade_length;
	m_loop_count = cfg_loop_count;
	m_pause_looping = cfg_pause_looping;
	m_pause_non_looping = cfg_pause_non_looping;
	m_hard_stop_old_vgms = cfg_hard_stop_old_vgms;
	m_play_infinitely = cfg_loop_count == 0 && m_loop_flag ? true : false;
	m_surround_sound = cfg_surround_sound > 0 ? true : false;
	m_double_ssg_volume = cfg_double_ssg_volume > 0 ? true : false;
	m_prefer_jap_tag = cfg_prefer_jap_tag > 0 ? true : false;
	m_display_both_tag = cfg_display_both_tag > 0 ? true : false;
	m_guess_track_number = cfg_guess_track_number > 0 ? true : false;
	m_recomp_level = cfg_recomp_level;
	m_volume = static_cast<double>(cfg_volume) * .01;

	//set current vgm chip info
	set_current_vgm_chip_info();
	
}

void input_vgm::display_chip_name(file_info & p_info)
{
	pfc::string8_fast temp_chip_name;
	pfc::string8_fast temp_accurate_chip_name;

	
		for (UINT8 i = 0; i < CHIP_COUNT; i++)
		{
			UINT8 chip_id = m_chip_info[i].chip_id;
			if (chip_id == 0xFF) break;
			UINT8 chip_sub_type = m_chip_info[i].chip_sub_type;
			if (temp_chip_name.is_empty())
			{
				if ((chip_id & 0x7F) < CHIP_COUNT)
				{
					if (m_chip_info[i].chip_set == 2)
					{
						//if ( 0x80 != chip_id)	//NOT T6W28
							temp_accurate_chip_name = "2x";
						temp_chip_name = "2x";
					}
					temp_accurate_chip_name += m_api.GetAccurateChipName(chip_id, chip_sub_type);
					temp_chip_name += ::my_chip_name[chip_id & 0x7F];
				}
			}
			else
			{
				if ((chip_id & 0x7F) < CHIP_COUNT)
				{
					temp_accurate_chip_name += ", ";
					temp_chip_name += ", ";
					if (m_chip_info[i].chip_set == 2)
					{
						//if ( 0x80 != chip_id)	//NOT T6W28
							temp_accurate_chip_name += "2x";
						temp_chip_name += "2x";
					}
					temp_accurate_chip_name += m_api.GetAccurateChipName(chip_id, chip_sub_type);					
					temp_chip_name += ::my_chip_name[chip_id & 0x7F];
				}
			}
		}

	if (!temp_accurate_chip_name.is_empty()) p_info.info_set("VGM_ACCURATE_CHIP_NAME", temp_accurate_chip_name);
	if (!temp_chip_name.is_empty()) p_info.info_set("VGM_CHIP_NAME", temp_chip_name);
}

void meta_add_multi_field(file_info & p_info, const char* name, const char* value)
{
	if (!value || '\0' == value[0] || !name)
		return;

	const char* begin = value;
	t_size len = 0;

	while (1)
	{
		if (value[0] == '\0' || value[0] == ',' && value[1] == ' ' /*|| value[0] == -0x1D && value[1] == -0x80 && value[2] == -0x7F*/ )//Japanese Comma
		{
			if (len > 0)
				p_info.meta_add_ex(name, ~0, begin, len);

			if (*value == '\0') break;
			len = 0;
			//if (*value == ',')
				value += 2;
			//else
				//value += 3;

			begin = value;
			continue;
		}
		len++;
		value++;
	}
}

void input_vgm::display_gd3_tag(file_info & p_info)
{
	pfc::stringcvt::string_utf8_from_wide game_name_E;
	pfc::stringcvt::string_utf8_from_wide game_name_J;
	pfc::stringcvt::string_utf8_from_wide track_name_E;
	pfc::stringcvt::string_utf8_from_wide track_name_J;
	pfc::stringcvt::string_utf8_from_wide author_name_E;
	pfc::stringcvt::string_utf8_from_wide author_name_J;
	pfc::stringcvt::string_utf8_from_wide system_name_E;
	pfc::stringcvt::string_utf8_from_wide system_name_J;
	pfc::stringcvt::string_utf8_from_wide release_date;
	pfc::stringcvt::string_utf8_from_wide creator;
	pfc::stringcvt::string_utf8_from_wide notes;

	if (m_gd3_tag.strGameNameE && m_gd3_tag.strGameNameE[0])
	{
		game_name_E.convert(m_gd3_tag.strGameNameE);
	}
	if (m_gd3_tag.strGameNameJ && m_gd3_tag.strGameNameJ[0])
	{
		game_name_J.convert(m_gd3_tag.strGameNameJ);		
	}

	if (m_gd3_tag.strTrackNameE && m_gd3_tag.strTrackNameE[0])
	{
		track_name_E.convert(m_gd3_tag.strTrackNameE);		
	}
	if (m_gd3_tag.strTrackNameJ && m_gd3_tag.strTrackNameJ[0])
	{
		track_name_J.convert(m_gd3_tag.strTrackNameJ);	
	}

	if (m_gd3_tag.strAuthorNameE && m_gd3_tag.strAuthorNameE[0])
	{
		author_name_E.convert(m_gd3_tag.strAuthorNameE);		
	}
	if (m_gd3_tag.strAuthorNameJ && m_gd3_tag.strAuthorNameJ[0])
	{
		author_name_J.convert(m_gd3_tag.strAuthorNameJ);		
	}

	if (m_gd3_tag.strSystemNameE && m_gd3_tag.strSystemNameE[0])
	{
		system_name_E.convert(m_gd3_tag.strSystemNameE);		
	}
	if (m_gd3_tag.strSystemNameJ && m_gd3_tag.strSystemNameJ[0])
	{
		system_name_J.convert(m_gd3_tag.strSystemNameJ);		
	}

	if (m_gd3_tag.strCreator && m_gd3_tag.strCreator[0]) creator.convert(m_gd3_tag.strCreator);
	if (m_gd3_tag.strReleaseDate && m_gd3_tag.strReleaseDate[0]) release_date.convert(m_gd3_tag.strReleaseDate);
	if (m_gd3_tag.strNotes && m_gd3_tag.strNotes[0]) notes.convert(m_gd3_tag.strNotes);

	if (m_prefer_jap_tag)
	{
		if (!game_name_J.is_empty())	p_info.meta_add("album", game_name_J);
		else if (!game_name_E.is_empty())	p_info.meta_add("album", game_name_E);

		if (!author_name_J.is_empty())	meta_add_multi_field(p_info, "artist", author_name_J);
		else if (!author_name_E.is_empty())	meta_add_multi_field(p_info, "artist", author_name_E);

		if (!track_name_J.is_empty())	p_info.meta_add("title", track_name_J);
		else if (!track_name_E.is_empty())	p_info.meta_add("title", track_name_E);

		if (!system_name_J.is_empty())	p_info.meta_add("system", system_name_J);
		else if (!system_name_E.is_empty())	p_info.meta_add("system", system_name_E);

		if (m_display_both_tag)
		{
			if (!game_name_E.is_empty()) p_info.meta_add("ALBUM_E", game_name_E);
			if (!track_name_E.is_empty()) p_info.meta_add("TITLE_E", track_name_E);
			if (!author_name_E.is_empty()) p_info.meta_add("ARTIST_E", author_name_E);
			if (!system_name_E.is_empty()) p_info.meta_add("SYSTEM_E", system_name_E);
		}
	}
	else
	{
		if (!game_name_E.is_empty())	p_info.meta_add("album", game_name_E);
		if (!author_name_E.is_empty())	meta_add_multi_field(p_info, "artist", author_name_E);
		if (!track_name_E.is_empty())	p_info.meta_add("title", track_name_E);
		if (!system_name_E.is_empty())	p_info.meta_add("system", system_name_E);

		if (m_display_both_tag)
		{
			if (!game_name_J.is_empty()) p_info.meta_add("ALBUM_J", game_name_J);
			if (!track_name_J.is_empty()) p_info.meta_add("TITLE_J", track_name_J);
			if (!author_name_J.is_empty()) p_info.meta_add("ARTIST_J", author_name_J);
			if (!system_name_J.is_empty()) p_info.meta_add("SYSTEM_J", system_name_J);
		}
	}


	if (!release_date.is_empty()) p_info.meta_add("date", release_date);
	if (!creator.is_empty()) p_info.meta_add("ripper", creator);
	//if (!notes.is_empty()) p_info.meta_add("comment", notes);

	//We have to convert 'LF' (or 'CR') to 'CRLF' for multi-line notes.
	if (!notes.is_empty())
	{
		pfc::array_t<t_int8> temp_notes;
		t_size notes_len = notes.length();
		temp_notes.set_size(notes_len + 16);
		const int CR = 13;
		const int LF = 10;
		t_size i = 0;
		t_size p = 0;
		for (i = 0; i < notes_len; i++)
		{
			if (temp_notes.get_size() <= i + p + 1) temp_notes.increase_size(16);
			if (notes[i] == '\0')
				break;
			if (notes[i] == CR && notes[i + 1] != LF)
			{
				temp_notes[i + p] = CR;
				p++;
				temp_notes[i + p] = LF;
				continue;
			}
			else if (notes[i] == CR && notes[i + 1] == LF)
			{
				temp_notes[i + p] = CR;
				temp_notes[i + p] = LF;
				continue;
			}
			else if (notes[i] == LF)
			{
				temp_notes[i + p] = CR;
				p++;
				temp_notes[i + p] = LF;
				continue;
			}
			temp_notes[i + p] = notes[i];
		}
		if (temp_notes.get_size() <= i + p) temp_notes.increase_size(1);
		temp_notes[i + p] = '\0';
		if (temp_notes[0]) p_info.meta_add("comment", reinterpret_cast<const char*>(temp_notes.get_ptr()));
	}

}

void input_vgm::guess_track_number(file_info & p_info)
{
	pfc::string_filename file_name(m_file_path);
	t_size file_name_len = file_name.get_length();
	
	t_size n = 0;
	for (; (n < file_name_len && '0' <= file_name[n] && file_name[n] <= '9'); n++) {}
			
	//track number is usually between 1 - 999
	if (n > 0 && n < 4 && (file_name[n] == ' ' || file_name[n] == '_' || file_name[n] == '.' && file_name[n + 1] == ' '))
		p_info.meta_add_ex("tracknumber", ~0, file_name, n);
	else if (m_gd3_tag.strGameNameE)
	{		
		pfc::stringcvt::string_utf8_from_wide game_name_E(m_gd3_tag.strGameNameE);
		//pfc::stringcvt::string_utf8_from_wide track_name_E;
		//if (m_gd3_tag.strTrackNameE)
			//track_name_E.convert(m_gd3_tag.strTrackNameE);

		n = 0;

		while ( n < file_name_len )
		{
			if (file_name[n] == '\0')
				break;
			if (file_name[n] == ' ' && file_name[n + 1] == '-' && file_name[n + 2] == ' ')
			{		
				if (stricmp_utf8_max(file_name, game_name_E, n))
					break;
				n += 3;
				t_size begin = n;
				t_size len = 0;
				for (; (n < file_name_len && '0' <= file_name[n] && file_name[n] <= '9'); n++, len++) {}
				if (len > 0 && len < 4 && (file_name[n] == '\0' || file_name[n] == ' ' && file_name[n + 1] == '-' && file_name[n + 2] == ' '))
					p_info.meta_add_ex("tracknumber", ~0, file_name.get_ptr() + begin, len);
				break;
			}
			n++;
		}
	}
}

// Get file information 
void input_vgm::get_info(file_info &p_info, abort_callback &p_abort)
{
	if (!m_set_len)
	{
		if (m_decode_flags & input_flag_no_looping)
		{
			m_play_infinitely = false;
			if (m_loop_count == 0)
				m_loop_count = 1;
		}

		if (m_loop_count == 0)
		{
			m_loop_count_mod = 0x01;
		}
		else if (m_vgm_header.bytLoopModifier != 0x10 || m_vgm_header.bytLoopBase != 0x00)
		{
			signed int temp_loop_count = (m_loop_count * m_vgm_header.bytLoopModifier + 0x08) / 0x10 - m_vgm_header.bytLoopBase;
			m_loop_count_mod = (temp_loop_count >= 0x01) ? temp_loop_count : 0x01;
		}
		else
		{
			m_loop_count_mod = m_loop_count;
		}

		//set vgm songlength
		if (m_loop_flag)
			m_song_sample = m_vgm_header.lngTotalSamples + m_vgm_header.lngLoopSamples * (m_loop_count_mod - 1);
		else
			m_song_sample = m_vgm_header.lngTotalSamples;

		//vgm header's sample count is ALWAYS 44.1 KHz, vgm's native sample rate
		m_song_len = audio_math::samples_to_time(m_song_sample, 44100);
		if (m_vgm_header.lngRate > 0 && m_playback_rate > 0 && m_vgm_header.lngRate != m_playback_rate)
			m_song_len = m_song_len * (double)m_vgm_header.lngRate / (double)m_playback_rate;

		m_song_sample = audio_math::time_to_samples(m_song_len, m_sample_rate);
		m_fade_sample = audio_math::time_to_samples(double(m_fade_len) * .001, m_sample_rate);

		if (m_loop_flag && !m_play_infinitely)
		{
			m_song_sample += m_fade_sample;
			m_song_len += (double)m_fade_len * .001;
		}

		m_pause_sample = audio_math::time_to_samples(double(m_loop_flag ? m_pause_looping : m_pause_non_looping) * .001, m_sample_rate);
		
	}


	p_info.set_length(m_song_len);
	p_info.info_set_int("channels", m_channels);
	p_info.info_set_int("bitspersample", m_bps);
	p_info.info_set_int("VGM_HEADER_TOTAL_SAMPLES", m_vgm_header.lngTotalSamples);
	p_info.info_set_int("VGM_HEADER_LOOP_SAMPLES", m_vgm_header.lngLoopSamples);
	//p_info.info_set_int("VGM_HEADER_LOOP_OFFSET", m_vgm_header.lngLoopOffset);
	p_info.info_set_int("VGM_HEADER_FRAME_RATE", m_vgm_header.lngRate);
	p_info.info_set("codec", "VGM");
	p_info.info_set("encoding", "synthesized");

	int version = m_vgm_header.lngVersion;
	p_info.info_set("VGM_VERSION", pfc::string_formatter() << pfc::format_int(version >> 8, 0, 16) << "." << pfc::format_int(version & 0xFF, 2, 16));
	
	//display gain. these code is from in_vgm.c. Thanks Valley Bell !
	int temp_slng;
	if (m_vgm_header.bytVolumeModifier <= VOLUME_MODIF_WRAP)
		temp_slng = m_vgm_header.bytVolumeModifier;
	else if (m_vgm_header.bytVolumeModifier == (VOLUME_MODIF_WRAP + 0x01))
		temp_slng = VOLUME_MODIF_WRAP - 0x100;
	else
		temp_slng = m_vgm_header.bytVolumeModifier - 0x100;
	p_info.info_set_float("VGM_GAIN", pow(2.0, temp_slng / (double)0x20), 2);


	//display current chip name
	display_chip_name(p_info);

	//display gd3 tag
	display_gd3_tag(p_info);

	if (m_guess_track_number)
		guess_track_number(p_info);
	
}

// Get Dynamic Information
bool input_vgm::decode_get_dynamic_info(file_info &p_out, double &p_timestamp_delta)
{
	if (m_dynamic_info)
	{
		m_dynamic_info = false;
		p_out.info_set_int("samplerate", m_sample_rate);
		p_timestamp_delta = -(audio_math::samples_to_time(m_render_done, m_sample_rate));
		//p_timestamp_delta = 0;
		return true;
	}
	return false;
}


// Decode Initialize
void input_vgm::decode_initialize(unsigned int p_flags, abort_callback &p_abort)
{
	m_set_len = false;
	m_decode_flags = p_flags;

	//load vgm_player.dll
	if (!load_dll(true, p_abort))
	{
		throw exception_io_data("Unable to load VGM_PLAYER.DLL");
	}

	p_VGMPlayEndPlay = m_api.GetPointerEndPlayFlag();
	//p_VGMPlayAppPaths = m_api.GetPointerAppPaths();
	p_VGMPlayChipOpts = m_api.GetPointerChipsOption();
	if (!p_VGMPlayEndPlay || !p_VGMPlayChipOpts)
		throw exception_io_data("VGM_PLAYER.DLL function lookups failed.");

	if (m_decode_flags & input_flag_no_looping)
	{
		m_play_infinitely = false;
		if (m_loop_count == 0)
			m_loop_count = 1;
	}

	if (m_loop_count == 0)
	{
		m_loop_count_mod = 0x01;
	}
	else if (m_vgm_header.bytLoopModifier != 0x10 || m_vgm_header.bytLoopBase != 0x00)
	{
		signed int temp_loop_count = (m_loop_count * m_vgm_header.bytLoopModifier + 0x08) / 0x10 - m_vgm_header.bytLoopBase;
		m_loop_count_mod = (temp_loop_count >= 0x01) ? temp_loop_count : 0x01;
	}
	else
	{
		m_loop_count_mod = m_loop_count;
	}
	

	//set vgm songlength
	if (m_loop_flag)
		m_song_sample = m_vgm_header.lngTotalSamples + m_vgm_header.lngLoopSamples * (m_loop_count_mod - 1);
	else
		m_song_sample = m_vgm_header.lngTotalSamples;

	//vgm header's sample count is ALWAYS 44.1 KHz, vgm's native sample rate
	m_song_len = audio_math::samples_to_time(m_song_sample, 44100);
	if (m_vgm_header.lngRate > 0 && m_playback_rate > 0 && m_vgm_header.lngRate != m_playback_rate)
		m_song_len = m_song_len * (double)m_vgm_header.lngRate / (double)m_playback_rate;

	m_song_sample = audio_math::time_to_samples(m_song_len, m_sample_rate);
	m_fade_sample = audio_math::time_to_samples(double(m_fade_len) * .001, m_sample_rate);

	if (m_loop_flag && !m_play_infinitely)
	{
		m_song_sample += m_fade_sample;
		m_song_len += (double)m_fade_len * .001;
	}

	m_pause_sample = audio_math::time_to_samples(double(m_loop_flag ? m_pause_looping : m_pause_non_looping) * .001, m_sample_rate);

	m_set_len = true;

	m_api.VGMPlay_Init();

	//setting playback options
	m_plbk_opts.SampleRate_				= m_sample_rate;
	m_plbk_opts.FadeTime_				= m_fade_len;
	m_plbk_opts.PauseTime_				= m_loop_flag ? m_pause_looping : m_pause_non_looping;
	m_plbk_opts.VGMMaxLoop_				= m_loop_count;
	m_plbk_opts.VGMPbRate_				= m_playback_rate;
	m_plbk_opts.VolumeLevel_			= static_cast<float>(m_volume);
	m_plbk_opts.SurroundSound_			= m_surround_sound;
	m_plbk_opts.ResampleMode_			= m_resampling_mode;
	m_plbk_opts.CHIP_SAMPLING_MODE_		= m_chip_sample_mode;
	m_plbk_opts.CHIP_SAMPLE_RATE_		= m_chip_rate > 0 ? m_chip_rate : m_sample_rate;
	m_plbk_opts.DoubleSSGVol_			= m_double_ssg_volume;
	m_plbk_opts.HardStopOldVGMs_		= m_hard_stop_old_vgms;
	m_api.SetVGMPlaybackOptions(&m_plbk_opts);
	
	//setting chips options
	for (unsigned i = 0; i < CHIP_COUNT; i++)
	{
		UINT8 chip_id = m_chip_info[i].chip_id;
		if (chip_id == 0xFF) break;

		for (UINT8 j = 0; j < m_chip_info[i].chip_set; j++)
			cfg_chips_opts[j]->copy<~0>(&p_VGMPlayChipOpts[j], chip_id);
	}
	

	//setting path of foo_input_vgm
	/*	
    m_my_path = core_api::get_my_full_path();
	m_my_path.truncate(m_my_path.scan_filename());
	p_VGMPlayAppPaths[0] = const_cast<char*>(m_my_path.get_ptr());
	
	//setting path of foobar2000.exe
	uGetModuleFileName(NULL, m_fb2k_path);
	m_fb2k_path.truncate(m_fb2k_path.scan_filename());
	p_VGMPlayAppPaths[1] = const_cast<char*>(m_fb2k_path.get_ptr());
	*/

	//we must call after setting all options
	m_api.VGMPlay_Init2();
	m_initialized = true;

	//load vgm file
	if (!m_api.OpenVGMFile2(&m_file_buf[0], m_file_len))
		throw exception_io_unsupported_format();
	m_opened = true;
	
	//free input file buffer
	std::vector<t_uint8>().swap(m_file_buf);

	// allocate decode buffer.
	//m_render_sample = audio_math::time_to_samples(double(m_render_ms) * .001, m_sample_rate);	// buffer duration [sample]
	m_decode_buf_len = m_render_sample * m_channels * (m_bps / 8) * 2;		// [byte]
	m_decode_buf.set_size(m_decode_buf_len + 16);

	m_api.PlayVGM();
	m_played = true;

	m_dynamic_info = true;
	m_render_done = 0;

}

// Decode
bool input_vgm::decode_run(audio_chunk &p_chunk, abort_callback &p_abort)
{
	if (*p_VGMPlayEndPlay)
	{
		return false;
	}

	bool update_muting(false);
	bool update_panning(false);

	for (unsigned i = 0; i < CHIP_COUNT; i++)
	{
		UINT8 chip_id = m_chip_info[i].chip_id;
		if (chip_id == 0xFF) break;

		for (UINT8 j = 0; j < m_chip_info[i].chip_set; j++)
		{
			bool ret = cfg_chips_opts[j]->compare<chips_options_ex::muting>(&p_VGMPlayChipOpts[j], chip_id);
			if (!ret)
			{
				update_muting = true;
				cfg_chips_opts[j]->copy<chips_options_ex::muting>(&p_VGMPlayChipOpts[j], chip_id);
			}
			ret = cfg_chips_opts[j]->compare<chips_options_ex::panning>(&p_VGMPlayChipOpts[j], chip_id);
			if (!ret)
			{
				update_panning = true;
				cfg_chips_opts[j]->copy<chips_options_ex::panning>(&p_VGMPlayChipOpts[j], chip_id);
			}
		}
	}

	if (update_muting)
		m_api.RefreshMuting();
	if (update_panning)
		m_api.RefreshPanning();
	

	t_uint64 render_sample = 0;
	
	// Render samples.
	render_sample = m_api.FillBuffer((WAVE_16BS*)m_decode_buf.get_ptr(), m_render_sample);
	
	if (render_sample == 0)	//if we set 0 sample p_chunk.set_date_fixedpoint, decode error "Decoder produced garbage" will occur.
	{
		render_sample = m_api.FillBuffer((WAVE_16BS*)m_decode_buf.get_ptr(), m_render_sample);
		if (*p_VGMPlayEndPlay || render_sample == 0)
		{
			m_render_done = 0;
			return false;
		}				
	}
	
	p_chunk.set_data_fixedpoint(
		m_decode_buf.get_ptr(),
		render_sample * m_channels * (m_bps / 8),
		m_sample_rate, m_channels, m_bps,
		audio_chunk::g_guess_channel_config(m_channels));

	if (!m_play_infinitely)
		m_played_sample += render_sample;
	m_render_done = render_sample;
	return true;	
}


// Whether can seek or not.
bool input_vgm::decode_can_seek()
{
	if (m_play_infinitely)
		return false;
	return true;
}


// Seek
void input_vgm::decode_seek(double p_seconds, abort_callback &p_abort)
{
	// compare desired seek pos with current pos
	t_uint64 seek_sample = ::audio_math::time_to_samples(p_seconds, m_sample_rate);

	if (seek_sample < 0 || seek_sample > m_song_sample + 1) throw exception_io_seek_out_of_range();

	m_dynamic_info = true;
	m_render_done = 0;

	m_api.SeekVGM(false, seek_sample);

	m_played_sample = seek_sample;
}

static inline t_uint32 read_le32(const t_uint8* data)
{
	return (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
}

static inline void write_le32(t_uint8* data, t_uint32 value)
{
	data[0] = (t_uint8)value;
	data[1] = (t_uint8)(value >> 8);
	data[2] = (t_uint8)(value >> 16);
	data[3] = (t_uint8)(value >> 24);
}

static inline void write_le16(t_uint8* data, t_uint16 value)
{
	data[0] = (t_uint8)value;
	data[1] = (t_uint8)(value >> 8);
}

static t_uint32 write_wstr_utf16le(t_uint8* p_data, t_uint32* file_pos, t_uint32 eof_pos, const wchar_t* p_wstr, t_uint32 wstr_len)
{
	//Works with ONLY Windows (wchar_t == UTF16LE include Surrogate Pair)
	t_uint32 cur_pos;
	t_uint16 ret_str_len = 0x00;

	cur_pos = *file_pos;
	if (cur_pos >= eof_pos)
		return 0x00;

	while (ret_str_len < wstr_len)
	{
		if (cur_pos >= eof_pos || p_wstr == NULL || *p_wstr == L'\0')
		{
			break;
		}
		write_le16(&p_data[cur_pos], *p_wstr);
		p_wstr++;
		ret_str_len++;
		cur_pos += 0x02;
	}

	if (cur_pos >= eof_pos)
	{
		t_uint32 temp_delta = cur_pos - eof_pos;
		write_le16(&p_data[cur_pos - 0x02 - temp_delta], L'\0');
		cur_pos -= temp_delta;
	}
	else if (cur_pos == eof_pos - 0x01)
	{
		write_le16(&p_data[cur_pos - 0x01], L'\0');
		cur_pos++;
	}
	else
	{
		write_le16(&p_data[cur_pos], L'\0');
		cur_pos += 0x02;
	}

	*file_pos = cur_pos;

	return ret_str_len;
}

void input_vgm::retag(const file_info &p_info, abort_callback &p_abort)
{
	if (!load_dll(false, p_abort))
		throw exception_io_data("Unable to load VGM_PLAYER.DLL");
	if (m_file_len > max_file_len)
		throw exception_io_data("File Size is too large to edit tag");


	if (m_file_len <= 0x24 || read_le32(&m_file_buf[0]) != FCC_VGM)	//VGM's Header Size is larger than 0x24 byte
		throw exception_io_unsupported_format();

	// I myself get these value because GetVGMFileInfo2 func often corrects them.
	// Correction may be required when PLAYING vgm, but NOT when WRITING tag.
	m_vgm_header.lngEOFOffset = read_le32(&m_file_buf[0x04]);
	m_vgm_header.lngGD3Offset = read_le32(&m_file_buf[0x14]);

	//I reject large value in order to prevent integer overflow...
	if (m_vgm_header.lngEOFOffset > max_file_len)
		throw exception_io_data("EOFOffset is too large to edit tag");
	if (m_vgm_header.lngGD3Offset > max_file_len)
		throw exception_io_data("GD3Offset is too large to edit tag");
	if (m_gd3_tag.lngTagLength > max_gd3_len)
		throw exception_io_data("GD3 Length is too large to edit tag");

	//relative -> absolute offset
	if (m_vgm_header.lngEOFOffset)
		m_vgm_header.lngEOFOffset += 0x04;
	if (m_vgm_header.lngGD3Offset)
		m_vgm_header.lngGD3Offset += 0x14;

	if (!m_vgm_header.lngEOFOffset || m_vgm_header.lngEOFOffset != m_file_len)
		throw exception_io_data("Invalid EOFOffset");

	if (m_vgm_header.lngGD3Offset)
	{
		if (m_vgm_header.lngGD3Offset + 0x04 > m_file_len)
			throw exception_io_data("Invalid GD3Offset");

		t_uint32 fcc_gd3 = read_le32(&m_file_buf[m_vgm_header.lngGD3Offset]);
		if (fcc_gd3 != FCC_GD3)
			throw exception_io_data("Invalid GD3Offset");

		if (m_gd3_tag.lngVersion != 0x100)
			throw exception_io_data("Unsupported GD3 Version");

		if (m_vgm_header.lngGD3Offset + 0x0C + m_gd3_tag.lngTagLength > m_file_len)
			throw exception_io_data("Invalid GD3 Length");
	}

	pfc::stringcvt::string_wide_from_utf8 game_name_E;
	pfc::stringcvt::string_wide_from_utf8 game_name_J;
	pfc::stringcvt::string_wide_from_utf8 track_name_E;
	pfc::stringcvt::string_wide_from_utf8 track_name_J;
	pfc::stringcvt::string_wide_from_utf8 author_name_E;
	pfc::stringcvt::string_wide_from_utf8 author_name_J;
	pfc::stringcvt::string_wide_from_utf8 system_name_E;
	pfc::stringcvt::string_wide_from_utf8 system_name_J;
	pfc::stringcvt::string_wide_from_utf8 release_date;
	pfc::stringcvt::string_wide_from_utf8 creator;
	pfc::stringcvt::string_wide_from_utf8 notes;
	GD3_TAG new_gd3_tag = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


	bool del_gd3(false);
	bool del_game_name_E(false);
	bool del_game_name_J(false);
	bool del_track_name_E(false);
	bool del_track_name_J(false);
	bool del_author_name_E(false);
	bool del_author_name_J(false);
	bool del_system_name_E(false);
	bool del_system_name_J(false);

	const char* value = 0;
	pfc::string8 formatted_value;

	value = p_info.meta_get("del_gd3", 0);

	if (value)
	{
		const char* walk = value;
		const char* begin = value;
		t_size len = 0;

		while (1)
		{
			if (walk[0] == '\0' || (walk[0] == ',' && walk[1] == ' '))
			{
				if (len > 0)
				{
					if (!stricmp_utf8_max(begin, "album_e", len))
						del_game_name_E = true;
					if (!stricmp_utf8_max(begin, "album_j", len))
						del_game_name_J = true;
					if (!stricmp_utf8_max(begin, "title_e", len))
						del_track_name_E = true;
					if (!stricmp_utf8_max(begin, "title_j", len))
						del_track_name_J = true;
					if (!stricmp_utf8_max(begin, "artist_e", len))
						del_author_name_E = true;
					if (!stricmp_utf8_max(begin, "artist_j", len))
						del_author_name_J = true;
					if (!stricmp_utf8_max(begin, "system_e", len))
						del_system_name_E = true;
					if (!stricmp_utf8_max(begin, "system_j", len))
						del_system_name_J = true;
					if (!stricmp_utf8_max(begin, "english", len))	{
						del_game_name_E = true;
						del_track_name_E = true;
						del_author_name_E = true;
						del_system_name_E = true;
					}
					if (!stricmp_utf8_max(begin, "japanese", len))	{
						del_game_name_J = true;
						del_track_name_J = true;
						del_author_name_J = true;
						del_system_name_J = true;
					}
					if (!stricmp_utf8_max(begin, "all", len))
						del_gd3 = true;
				}


				if (walk[0] == '\0') break;
				len = 0;
				walk += 2;
				begin = walk;
				continue;

			}
			len++;
			walk++;
		}
	}

	
	if (!m_prefer_jap_tag)
	{
		if (del_game_name_J)
		{
			//new_gd3_tag.strGameNameJ = 0;
		}
		else
		{
			value = p_info.meta_get("album_j", 0);
			if (value)
			{
				game_name_J.convert(value);
				new_gd3_tag.strGameNameJ = const_cast<wchar_t*>(game_name_J.get_ptr());
			}
			else if (m_gd3_tag.strGameNameJ)
			{
				new_gd3_tag.strGameNameJ = m_gd3_tag.strGameNameJ;
			}
		}
		
		if (del_track_name_J)
		{
			//new_gd3_tag.strTrackNameJ = 0;
		}
		else
		{
			value = p_info.meta_get("title_j", 0);
			if (value)
			{
				track_name_J.convert(value);
				new_gd3_tag.strTrackNameJ = const_cast<wchar_t*>(track_name_J.get_ptr());
			}
			else if (m_gd3_tag.strTrackNameJ)
			{
				new_gd3_tag.strTrackNameJ = m_gd3_tag.strTrackNameJ;
			}
		}
		
		if (del_author_name_J)
		{
			//new_gd3_tag.strAuthorNameJ = 0;
		}
		else
		{
			value = p_info.meta_get("artist_j", 0);
			if (value)
			{
				author_name_J.convert(value);
				new_gd3_tag.strAuthorNameJ = const_cast<wchar_t*>(author_name_J.get_ptr());
			}
			else if (m_gd3_tag.strAuthorNameJ)
			{
				new_gd3_tag.strAuthorNameJ = m_gd3_tag.strAuthorNameJ;
			}
		}
		
		if (del_system_name_J)
		{
			//new_gd3_tag.strSystemNameJ = 0;
		}
		else
		{
			value = p_info.meta_get("system_j", 0);
			if (value)
			{
				system_name_J.convert(value);
				new_gd3_tag.strSystemNameJ = const_cast<wchar_t*>(system_name_J.get_ptr());
			}
			else if (m_gd3_tag.strSystemNameJ)
			{
				new_gd3_tag.strSystemNameJ = m_gd3_tag.strSystemNameJ;
			}
		}
		
	}
	else
	{
		if (del_game_name_E)
		{
			//new_gd3_tag.strGameNameE = 0;
		}
		else
		{
			value = p_info.meta_get("album_e", 0);
			if (value)
			{
				game_name_E.convert(value);
				new_gd3_tag.strGameNameE = const_cast<wchar_t*>(game_name_E.get_ptr());
			}
			else if (m_gd3_tag.strGameNameE)
			{
				new_gd3_tag.strGameNameE = m_gd3_tag.strGameNameE;
			}
		}
		if (del_track_name_E)
		{
			//new_gd3_tag.strTrackNameE = 0;
		}
		else
		{
			value = p_info.meta_get("title_e", 0);
			if (value)
			{
				track_name_E.convert(value);
				new_gd3_tag.strTrackNameE = const_cast<wchar_t*>(track_name_E.get_ptr());
			}
			else if (m_gd3_tag.strTrackNameE)
			{
				new_gd3_tag.strTrackNameE = m_gd3_tag.strTrackNameE;
			}
		}
		if (del_author_name_E)
		{
			//new_gd3_tag.strAuthorNameE = 0;
		}
		else
		{
			value = p_info.meta_get("artist_e", 0);
			if (value)
			{
				author_name_E.convert(value);
				new_gd3_tag.strAuthorNameE = const_cast<wchar_t*>(author_name_E.get_ptr());
			}
			else if (m_gd3_tag.strAuthorNameE)
			{
				new_gd3_tag.strAuthorNameE = m_gd3_tag.strAuthorNameE;
			}
		}
		if (del_system_name_E)
		{
			//new_gd3_tag.strSystemNameE = 0;
		}
		else
		{
			value = p_info.meta_get("system_e", 0);
			if (value)
			{
				system_name_E.convert(value);
				new_gd3_tag.strSystemNameE = const_cast<wchar_t*>(system_name_E.get_ptr());
			}
			else if (m_gd3_tag.strSystemNameE)
			{
				new_gd3_tag.strSystemNameE = m_gd3_tag.strSystemNameE;
			}
		}	
	}
	

	value = p_info.meta_get("album", 0);
	if (value)
	{
		if (m_prefer_jap_tag)
		{
			if (!del_game_name_J)	{
				game_name_J.convert(value);
				new_gd3_tag.strGameNameJ = const_cast<wchar_t*>(game_name_J.get_ptr());
			}
		}
		else if (!del_game_name_E)
		{
			game_name_E.convert(value);
			new_gd3_tag.strGameNameE = const_cast<wchar_t*>(game_name_E.get_ptr());
		}
	}

	value = p_info.meta_get("title", 0);
	if (value)
	{
		if (m_prefer_jap_tag)
		{
			if (!del_track_name_J)	{
				track_name_J.convert(value);
				new_gd3_tag.strTrackNameJ = const_cast<wchar_t*>(track_name_J.get_ptr());
			}
		}
		else if (!del_track_name_E)
		{
			track_name_E.convert(value);
			new_gd3_tag.strTrackNameE = const_cast<wchar_t*>(track_name_E.get_ptr());
		}
	}

	p_info.meta_format("artist", formatted_value);
	if (!formatted_value.is_empty())
	{
		if (m_prefer_jap_tag)
		{
			if (!del_author_name_J)	{
				author_name_J.convert(formatted_value);
				new_gd3_tag.strAuthorNameJ = const_cast<wchar_t*>(author_name_J.get_ptr());
			}
		}
		else if (!del_author_name_E)
		{
			author_name_E.convert(formatted_value);
			new_gd3_tag.strAuthorNameE = const_cast<wchar_t*>(author_name_E.get_ptr());
		}
	}

	value = p_info.meta_get("system", 0);
	if (value)
	{
		if (m_prefer_jap_tag)
		{
			if (!del_system_name_J)	{
				system_name_J.convert(value);
				new_gd3_tag.strSystemNameJ = const_cast<wchar_t*>(system_name_J.get_ptr());
			}
		}
		else if (!del_system_name_E)
		{
			system_name_E.convert(value);
			new_gd3_tag.strSystemNameE = const_cast<wchar_t*>(system_name_E.get_ptr());
		}
	}

	value = p_info.meta_get("date", 0);
	if (value)
	{
		release_date.convert(value);
		new_gd3_tag.strReleaseDate = const_cast<wchar_t*>(release_date.get_ptr());
	}
	value = p_info.meta_get("ripper", 0);
	if (value)
	{
		creator.convert(value);
		new_gd3_tag.strCreator = const_cast<wchar_t*>(creator.get_ptr());
	}
	value = p_info.meta_get("comment", 0);
	if (value)
	{
		//We have to convert 'CRLF' to 'LF' for multi-line notes.
		pfc::array_t<char> temp_notes;
		t_size notes_len = strlen(value);
		temp_notes.set_size(notes_len + 1);
		if (temp_notes.get_size() < notes_len) throw pfc::exception_overflow();

		const int CR = 13;
		const int LF = 10;
		t_size i = 0;
		t_size p = 0;
		for (i = 0; i < notes_len; i++)
		{
			if (value[i + p] == '\0')
				break;
			if (value[i + p] == CR && value[i + p + 1] == LF)
			{
				temp_notes[i] = LF;
				p++;
				continue;
			}

			temp_notes[i] = value[i + p];
		}
		temp_notes[i] = '\0';
		notes.convert(temp_notes.get_ptr());
		new_gd3_tag.strNotes = const_cast<wchar_t*>(notes.get_ptr());
	}
	

	const unsigned int gd3_elements = 11;
	UINT32 new_gd3_total_len = 0x00000000;	
	UINT32 new_gd3_ind_len[gd3_elements];
	memset(new_gd3_ind_len, 0x00000000, sizeof(new_gd3_ind_len));

	if (new_gd3_tag.strTrackNameE)  new_gd3_ind_len[0] = wcslen(new_gd3_tag.strTrackNameE);
	if (new_gd3_tag.strTrackNameJ)  new_gd3_ind_len[1] = wcslen(new_gd3_tag.strTrackNameJ);
	if (new_gd3_tag.strGameNameE)   new_gd3_ind_len[2] = wcslen(new_gd3_tag.strGameNameE);
	if (new_gd3_tag.strGameNameJ)   new_gd3_ind_len[3] = wcslen(new_gd3_tag.strGameNameJ);
	if (new_gd3_tag.strSystemNameE) new_gd3_ind_len[4] = wcslen(new_gd3_tag.strSystemNameE);
	if (new_gd3_tag.strSystemNameJ) new_gd3_ind_len[5] = wcslen(new_gd3_tag.strSystemNameJ);
	if (new_gd3_tag.strAuthorNameE) new_gd3_ind_len[6] = wcslen(new_gd3_tag.strAuthorNameE);
	if (new_gd3_tag.strAuthorNameJ) new_gd3_ind_len[7] = wcslen(new_gd3_tag.strAuthorNameJ);
	if (new_gd3_tag.strReleaseDate) new_gd3_ind_len[8] = wcslen(new_gd3_tag.strReleaseDate);
	if (new_gd3_tag.strCreator)     new_gd3_ind_len[9] = wcslen(new_gd3_tag.strCreator);
	if (new_gd3_tag.strNotes)       new_gd3_ind_len[10] = wcslen(new_gd3_tag.strNotes);

	for (unsigned i = 0; i < gd3_elements; i++)
	{
		new_gd3_total_len = pfc::add_unsigned_clipped<UINT32>(new_gd3_total_len, new_gd3_ind_len[i]);
	}

	if (new_gd3_total_len > (max_gd3_len / sizeof(wchar_t)))
		throw exception_io_data("GD3 Length is too large to edit tag");

	if (new_gd3_total_len == 0x00000000) del_gd3 = true;
	if (!m_vgm_header.lngGD3Offset && del_gd3)
		throw exception_io_data("You don't need to update tag");
	
	new_gd3_total_len *= sizeof(wchar_t);	//[byte]
	new_gd3_total_len += gd3_elements * sizeof(wchar_t);	//size of null-string [byte]

	if (new_gd3_total_len > max_gd3_len)
		throw exception_io_data("GD3 Length is too large to edit tag");

	UINT32 cur_pos = 0x00000000;
	UINT32 new_gd3_offset = 0x00000000;
	UINT32 new_file_len = 0x00000000;
	

	bool update_only_gd3(false);
	
		if (!m_vgm_header.lngGD3Offset || m_vgm_header.lngGD3Offset + 0x0C + m_gd3_tag.lngTagLength == m_vgm_header.lngEOFOffset)
		{
			if (!m_gz)
				update_only_gd3 = true;

			if (del_gd3)
			{
				new_file_len = m_vgm_header.lngGD3Offset;
				write_le32(&m_file_buf[0x14], 0x00000000);
			}
			else
			{
				if (new_gd3_total_len > m_gd3_tag.lngTagLength)
				{
					UINT32 temp = new_gd3_total_len - m_gd3_tag.lngTagLength;
					if (!m_vgm_header.lngGD3Offset)
						temp += 0x0C;	//'Gd3 ' 'Version' 'TagLength'

					m_file_buf.resize(m_file_buf.size() + temp);
				}

				if (!m_vgm_header.lngGD3Offset)
				{
					//Create New GD3Tag
					cur_pos = m_vgm_header.lngEOFOffset;
					new_gd3_offset = cur_pos;
				
					write_le32(&m_file_buf[0x14], cur_pos - 0x14);	//Update Header GD3Offset
					write_le32(&m_file_buf[cur_pos], FCC_GD3);
					cur_pos += 0x04;

					write_le32(&m_file_buf[cur_pos], 0x100);	//Now Only Support GD3 v1.00
					cur_pos += 0x04;
				}
				else
				{
					//Header -> (XX) -> VGMData -> (XX) -> GD3Tag -> EOF   XX may be Extra Header(from v1.70)
					new_gd3_offset = m_vgm_header.lngGD3Offset;
					cur_pos = new_gd3_offset + 0x08;
				}

			
				write_le32(&m_file_buf[cur_pos], new_gd3_total_len);
				cur_pos += 0x04;
				new_file_len = cur_pos + new_gd3_total_len;

				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strTrackNameE,	new_gd3_ind_len[0]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strTrackNameJ, new_gd3_ind_len[1]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strGameNameE, new_gd3_ind_len[2]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strGameNameJ, new_gd3_ind_len[3]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strSystemNameE, new_gd3_ind_len[4]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strSystemNameJ, new_gd3_ind_len[5]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strAuthorNameE, new_gd3_ind_len[6]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strAuthorNameJ, new_gd3_ind_len[7]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strReleaseDate, new_gd3_ind_len[8]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strCreator, new_gd3_ind_len[9]);
				write_wstr_utf16le(&m_file_buf[0], &cur_pos, new_file_len, new_gd3_tag.strNotes, new_gd3_ind_len[10]);
			}
		}
		else
		{
			//Not implemented yet...
			throw pfc::exception_not_implemented();
		}

		write_le32(&m_file_buf[0x04], new_file_len - 0x04);	//Update Header EOFOffset

		std::vector<t_uint8> gz_file_buf;
		if (m_gz)
		{
			//Recompress
			UINT32 extra_buf_len = new_file_len / 100;
			extra_buf_len += 20;
			UINT32 gz_file_buf_len = pfc::add_unsigned_clipped<UINT32>(new_file_len, extra_buf_len);
			gz_file_buf.resize(gz_file_buf_len);
			UINT32 gz_new_file_len = m_api.CompressFile(&m_file_buf[0], new_file_len, &gz_file_buf[0], gz_file_buf_len, m_recomp_level);

			if (!gz_new_file_len || gz_new_file_len == 0xFFFFFFFF)
				throw exception_io_data("Could not recompress VGZs");

			m_file_len = new_file_len;
			new_file_len = gz_new_file_len;
		}
		else
		{
			m_file_len = new_file_len;
		}

		if (!new_file_len)
			throw exception_io_unsupported_format();

		if (m_file->get_size(p_abort) > new_file_len)
			m_file->truncate(new_file_len, p_abort);

		if (update_only_gd3)
		{
			m_file->seek(0x04, p_abort);
			m_file->write(&m_file_buf[0x04], 0x04, p_abort);	//Write Header EOFOffset
			m_file->seek(0x14, p_abort);
			m_file->write(&m_file_buf[0x14], 0x04, p_abort);	//Write Header GD3Offset
			if (!del_gd3)
			{
				m_file->seek(new_gd3_offset, p_abort);
				m_file->write(&m_file_buf[new_gd3_offset], 0x0C + new_gd3_total_len, p_abort);	//Write GD3 Tag
			}
		}
		else
		{
			m_file->seek(0x00, p_abort);
			if (m_gz)
				m_file->write(&gz_file_buf[0], new_file_len, p_abort);
			else
				m_file->write(&m_file_buf[0], new_file_len, p_abort);
		}
	
	//Reload Info
	if (m_got_vgm_file_info && m_api.FreeGD3Tag)	m_api.FreeGD3Tag(&m_gd3_tag);
	m_got_vgm_file_info = false;

	if (!m_api.GetVGMFileInfo2(&m_file_buf[0], m_file_len, &m_vgm_header, &m_gd3_tag))
		throw exception_io_data("Could not get info after tag editing");

	m_got_vgm_file_info = true;
	m_set_len = false;
}

void input_vgm::g_load_dll()
{
	pfc::string8 path = core_api::get_my_full_path();
	path.truncate(path.scan_filename());
	path += DLL_NAME;

	input_vgm::g_dll_hnd = uLoadLibrary(path);

	if (input_vgm::g_dll_hnd)
	{
		GETVGMPLAYAPIADDRESS(g_api, g_dll_hnd, GetProcAddress)
	}
}

void input_vgm::g_free_dll()
{
	if (input_vgm::g_dll_hnd)
	{
		FreeLibrary(input_vgm::g_dll_hnd);
		input_vgm::g_dll_hnd = 0;
	}
}
vgm_play_api input_vgm::g_api = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
HINSTANCE input_vgm::g_dll_hnd = 0;

//static input_singletrack_factory_t<input_vgm> g_input_vgm_factory;
static input_singletrack_factory_ex_t<input_vgm, 0, input_decoder_v3> g_input_vgm_factory;


class initquit_input_vgm : public initquit
{
public:
	virtual void on_init()
	{
		input_vgm::g_load_dll();
	}

	virtual void on_quit()
	{
		input_vgm::g_free_dll();
	}
};
static initquit_factory_t<initquit_input_vgm> g_initquit_input_vgm_factory;


// version info
DECLARE_COMPONENT_VERSION(my_component_name,
"0.29",
"in_vgm/VGMPlay 0.40.9\n"
"by Valley Bell 2011-2018\n"
"http://vgmrips.net/\n"
"\n"
"ZLib 1.2.11\n"
"Copyright (c) 1995-2017 Jean-loup Gailly and Mark Adler\n"
"http://www.zlib.org/\n"
"\n"
"Memory DLL loading code\n"
"Copyright (c) 2004-2017 by Joachim Bauch\n"
"http://www.joachim-bauch.de\n"
)

#define DECLARE_FILE_TYPE_EX_A(space, extensions, name, namePlural) \
namespace space { static service_factory_single_t<input_file_type_v2_impl> g_myfiletype(extensions, name, namePlural); }


DECLARE_FILE_TYPE_EX_A(a, "vgm", "VGM file", "VGM files")
DECLARE_FILE_TYPE_EX_A(b, "vgz", "VGZ file", "VGM files")

// This will prevent users from renaming your component around (important for proper troubleshooter behaviors) or loading multiple instances of it.
VALIDATE_COMPONENT_FILENAME("foo_input_vgm.dll");

