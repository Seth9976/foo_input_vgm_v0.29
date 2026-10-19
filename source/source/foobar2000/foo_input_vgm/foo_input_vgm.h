#ifndef INC_FOO_INPUT_VGM_H
#define INC_FOO_INPUT_VGM_H

#include "../../vgm_player/VGMPlay/VGMPlay_Intf.h"
#include <vector>


typedef void(*p_VGMPlay)(void);
typedef char*(*p_FindFile)(const char*);
typedef UINT32(*p_GetGZFileLength)(const char*);
typedef UINT32(*p_GetGZFileLength2)(const UINT8*, UINT32, bool*);
typedef bool(*p_OpenVGMFile)(const char*);
typedef bool(*p_OpenVGMFile2)(const void*, UINT32);
typedef void(*p_FreeGD3Tag)(GD3_TAG*);
typedef UINT32(*p_GetVGMFileInfo)(const char*, VGM_HEADER*, GD3_TAG*);
typedef UINT32(*p_GetVGMFileInfo2)(const void*, UINT32, VGM_HEADER*, GD3_TAG*);
typedef UINT32(*p_CalcSampleMSec)(UINT64, UINT8);
typedef UINT32(*p_CalcSampleMSecExt)(UINT64, UINT8, VGM_HEADER*);
typedef const char*(*p_GetChipName)(UINT8);
typedef const char*(*p_GetAccurateChipName)(UINT8, UINT8);
typedef UINT32(*p_GetChipClock)(VGM_HEADER*, UINT8, UINT8*);
typedef INT32(*p_SampleVGM2Playback)(INT32);
typedef void(*p_PauseVGM)(bool);
typedef void(*p_SeekVGM)(bool, INT32);
typedef UINT32(*p_FillBuffer)(WAVE_16BS*, UINT32);
typedef void(*p_SetVGMPlaybackOptions)(PLBK_OPTS*);
typedef void(*p_SetVGMChipsOption)(CHIPS_OPTION*, UINT8);
typedef char**(*p_GetPointerAppPaths)(void);
typedef CHIPS_OPTION*(*p_GetPointerChipsOption)(void);
typedef const bool*(*p_GetPointerEndPlayFlag)(void);
typedef UINT32(*p_UncompressGZFile)(const UINT8*, UINT32, UINT8*, UINT32);
typedef UINT32(*p_CompressFile)(const UINT8*, UINT32, UINT8*, UINT32, INT32);

//vgmplay api
struct vgm_play_api
{
	p_VGMPlay				VGMPlay_Init;
	p_VGMPlay				VGMPlay_Init2;
	p_VGMPlay				VGMPlay_Deinit;
	p_VGMPlay				CloseVGMFile;
	p_VGMPlay				PlayVGM;
	p_VGMPlay				StopVGM;
	p_VGMPlay				RestartVGM;
	p_VGMPlay				RefreshMuting;
	p_VGMPlay				RefreshPanning;
	p_VGMPlay				RefreshPlaybackOptions;

	p_FindFile					FindFile;
	p_GetGZFileLength			GetGZFileLength;
	p_GetGZFileLength2			GetGZFileLength2;
	p_OpenVGMFile				OpenVGMFile;
	p_OpenVGMFile2				OpenVGMFile2;
	p_FreeGD3Tag				FreeGD3Tag;
	p_GetVGMFileInfo			GetVGMFileInfo;
	p_GetVGMFileInfo2			GetVGMFileInfo2;
	p_CalcSampleMSec			CalcSampleMSec;
	p_CalcSampleMSecExt			CalcSampleMSecExt;
	p_GetChipName				GetChipName;
	p_GetAccurateChipName		GetAccurateChipName;
	p_GetChipClock				GetChipClock;
	p_SampleVGM2Playback		SampleVGM2Playback;
	p_SampleVGM2Playback		SamplePlayback2VGM;
	p_PauseVGM					PauseVGM;
	p_SeekVGM					SeekVGM;
	p_FillBuffer				FillBuffer;
	p_SetVGMPlaybackOptions		SetVGMPlaybackOptions;
	p_SetVGMChipsOption			SetVGMChipsOption;
	p_GetPointerAppPaths		GetPointerAppPaths;
	p_GetPointerChipsOption		GetPointerChipsOption;
	p_GetPointerEndPlayFlag		GetPointerEndPlayFlag;

	p_UncompressGZFile			UncompressGZFile;
	p_CompressFile				CompressFile;
};

#ifdef FB2K_OLD_SDK
class input_vgm
#else
class input_vgm : public input_stubs
#endif
{
private:
	//current playing VGM ChipID and SubType
	typedef struct vgm_chip_info
	{
		UINT8 chip_id;
		UINT8 chip_sub_type;
		UINT8 chip_set;
	} vgm_chip_info;

	vgm_play_api				m_api;
	static vgm_play_api			g_api;

	//instance
	HMEMORYMODULE				m_dll_hnd;
	static HINSTANCE			g_dll_hnd;
	service_ptr_t<file>			m_file;			//handle of input file
	
	//buffer
	std::vector<t_uint8>		m_file_buf;		// in-memory copy of input file
	pfc::array_t<t_uint8>		m_decode_buf;	// decode buffer
	t_size						m_decode_buf_len;	// buffer size of decode buffer

	// about input file
	pfc::string8				m_file_path;		// input file path
	//pfc::string8				m_file_ext;		//input file extension
	UINT32						m_file_len;		// UNCOMPRESSED input file size
	bool						m_gz;
	VGM_HEADER					m_vgm_header;
	GD3_TAG						m_gd3_tag;
	vgm_chip_info				m_chip_info[CHIP_COUNT];
	bool						m_prefer_jap_tag;
	bool						m_display_both_tag;
	bool						m_guess_track_number;


	//song time
	double						m_song_len;	//total vgm song length [s]
	unsigned int				m_fade_len;	//configure fade length [ms]
	unsigned int				m_loop_count;
	unsigned int				m_loop_count_mod;
	bool						m_loop_flag;
	t_uint64					m_song_sample;
	t_uint64					m_fade_sample;	
	bool						m_set_len;

	//playback options
	PLBK_OPTS					m_plbk_opts;
	int							m_resampling_mode;
	int							m_chip_sample_mode;
	unsigned int				m_sample_rate;
	unsigned int				m_chip_rate;
	unsigned int				m_playback_rate;
	unsigned int				m_pause_looping;
	unsigned int				m_pause_non_looping;
	t_uint64					m_pause_sample;
	bool						m_surround_sound;
	bool						m_double_ssg_volume;
	double						m_volume;
	bool						m_play_infinitely;
	unsigned int				m_hard_stop_old_vgms;

	//playback
	static const unsigned int	m_bps;
	static const unsigned int	m_channels;
	static const unsigned int	m_render_sample;	// buffer duration [sample]
	unsigned int				m_render_done;
	t_uint64					m_played_sample;		//played time [sample]
	bool						m_dynamic_info;			//reporting configuration dynamically
	volatile bool				m_got_vgm_file_info;	//become true after GetVGMFileInfo func was called
	volatile bool				m_opened;			//become true after OpenVGMfile func was called 
	volatile bool				m_initialized;		//become true after VGM_Init2 func was called 
	volatile bool				m_played;			//become true after Play func was called 

	//misc.
	const bool*					p_VGMPlayEndPlay;
	CHIPS_OPTION*				p_VGMPlayChipOpts;	//pointer of VGMPlay's ChipOpts variable
	//char**					p_VGMPlayAppPaths;	//pointer of VGMPlay's AppPaths variable
	//pfc::string8				m_fb2k_path; //path of foobar2000.exe
	//pfc::string8				m_my_path;			//path of foo_input_vgm.dll
	t_input_open_reason			m_open_reason;
	unsigned int				m_decode_flags;
	int							m_recomp_level;
	static const GUID			m_decoder_priority_table_guid;


	bool load_dll(bool unique, abort_callback & p_abort);
	void free_dll();
	void set_current_vgm_chip_info();
	void display_gd3_tag(file_info & p_info);
	void display_chip_name(file_info & p_info);
	void guess_track_number(file_info & p_info);

public:
	input_vgm();
	~input_vgm();

	static void g_load_dll();
	static void g_free_dll();

	static bool g_is_our_path(const char *p_path, const char *p_extension);
	
	static bool g_is_our_content_type(const char *p_content_type) { return false; }

	void open(service_ptr_t<file> p_filehint, const char * p_path, t_input_open_reason p_reason, abort_callback &p_abort);
	t_filestats get_file_stats(abort_callback & p_abort) { return m_file->get_stats(p_abort); }
	void get_info(file_info &p_info, abort_callback &p_abort);

	bool decode_get_dynamic_info(file_info &p_out, double &p_timestamp_delta);
	bool decode_get_dynamic_info_track(file_info &p_out, double &p_timestamp_delta) { return false; }

	void decode_initialize(unsigned int p_flags, abort_callback &p_abort);
	bool decode_run(audio_chunk &p_chunk, abort_callback &p_abort);
	bool decode_can_seek();
	void decode_seek(double p_seconds, abort_callback &p_abort);
	void decode_on_idle(abort_callback & p_abort) { m_file->on_idle(p_abort); }

	bool decode_run_raw(audio_chunk & p_chunk, mem_block_container & p_raw, abort_callback & p_abort) {	return false; }
	void set_logger(event_logger::ptr ptr) {  };

	void set_pause(bool paused) { m_api.PauseVGM(paused); }
	bool flush_on_pause() {	return false; }


	void retag(const file_info &p_info, abort_callback &p_abort);

#ifdef FB2K_OLD_SDK
#else
	static GUID g_get_guid()
	{
		return m_decoder_priority_table_guid;
	}

	static const char * g_get_name()
	{
		return my_component_name;
	}

	static GUID g_get_preferences_guid()
	{
		return guid_preferences_page_foo_input_vgm;
	}
#endif
};

#endif
