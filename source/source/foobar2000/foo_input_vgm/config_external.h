#pragma once
#include "../../vgm_player/VGMPlay/VGMPlay.h"

//コンポーネント名
extern const char* const my_component_name;

//Preferencesでのチップ名表示用
extern const char* const my_chip_name[CHIP_COUNT];

extern const GUID guid_preferences_page_foo_input_vgm;

extern cfg_int cfg_resampling_mode;
extern cfg_int cfg_chip_sample_mode;
extern cfg_int cfg_playback_rate;
extern cfg_int cfg_sample_rate;
extern cfg_int cfg_chip_rate;

extern cfg_int cfg_fade_length;
extern cfg_int cfg_loop_count;
extern cfg_int cfg_pause_looping;
extern cfg_int cfg_pause_non_looping;
extern cfg_int cfg_hard_stop_old_vgms;

extern cfg_int cfg_surround_sound;
extern cfg_int cfg_double_ssg_volume;
extern cfg_int cfg_volume;
extern cfg_int cfg_prefer_jap_tag;
extern cfg_int cfg_display_both_tag;
extern cfg_int cfg_guess_track_number;
extern cfg_int cfg_recomp_level;

extern cfg_int cfg_vgm;

extern cfg_chips_options *cfg_chips_opts[2];

extern cfg_int cfg_chip_id_index;
extern cfg_int cfg_chip_set_index;