// VGMPlay_Intf.h: VGMPlay Interface Header File
//

//#define NO_WCHAR_FILENAMES
#ifndef WIN32
// Linux uses UTF-8 Unicode and has no special wide-character file routines.
#define NO_WCHAR_FILENAMES
#endif

typedef struct waveform_16bit_stereo
{
	INT16 Left;
	INT16 Right;
} WAVE_16BS;

typedef struct waveform_32bit_stereo
{
	INT32 Left;
	INT32 Right;
} WAVE_32BS;


#ifdef VGM_PLAYER_REMOVED
void VGMPlay_Init(void);
void VGMPlay_Init2(void);
void VGMPlay_Deinit(void);
char* FindFile(const char* FileName);
char* FindFile_List(const char** FileNameList);

UINT32 GetGZFileLength(const char* FileName);
bool OpenVGMFile(const char* FileName);
void CloseVGMFile(void);

void FreeGD3Tag(GD3_TAG* TagData);
UINT32 GetVGMFileInfo(const char* FileName, VGM_HEADER* RetVGMHead, GD3_TAG* RetGD3Tag);
UINT32 CalcSampleMSec(UINT64 Value, UINT8 Mode);
UINT32 CalcSampleMSecExt(UINT64 Value, UINT8 Mode, VGM_HEADER* FileHead);
const char* GetChipName(UINT8 ChipID);
const char* GetAccurateChipName(UINT8 ChipID, UINT8 SubType);
UINT32 GetChipClock(VGM_HEADER* FileHead, UINT8 ChipID, UINT8* RetSubType);

#ifndef NO_WCHAR_FILENAMES
UINT32 GetGZFileLengthW(const wchar_t* FileName);
bool OpenVGMFileW(const wchar_t* FileName);
UINT32 GetVGMFileInfoW(const wchar_t* FileName, VGM_HEADER* RetVGMHead, GD3_TAG* RetGD3Tag);
#endif

INT32 SampleVGM2Playback(INT32 SampleVal);
INT32 SamplePlayback2VGM(INT32 SampleVal);

void PlayVGM(void);
void StopVGM(void);
void RestartVGM(void);
void PauseVGM(bool Pause);
void SeekVGM(bool Relative, INT32 PlayBkSamples);
void RefreshMuting(void);
void RefreshPanning(void);
void RefreshPlaybackOptions(void);

UINT32 FillBuffer(WAVE_16BS* Buffer, UINT32 BufferSize);

#else

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VGMPLAYAPI
#define VGMPLAYAPI __declspec(dllimport)
#endif
	
VGMPLAYAPI void VGMPlay_Init(void);
VGMPLAYAPI void VGMPlay_Init2(void);
VGMPLAYAPI void VGMPlay_Deinit(void);
VGMPLAYAPI char* FindFile(const char* FileName);
VGMPLAYAPI char* FindFile_List(const char** FileNameList);

VGMPLAYAPI UINT32 GetGZFileLength(const char* FileName);
VGMPLAYAPI bool OpenVGMFile(const char* FileName);
VGMPLAYAPI void CloseVGMFile(void);

VGMPLAYAPI void FreeGD3Tag(GD3_TAG* TagData);
VGMPLAYAPI UINT32 GetVGMFileInfo(const char* FileName, VGM_HEADER* RetVGMHead, GD3_TAG* RetGD3Tag);
VGMPLAYAPI UINT32 CalcSampleMSec(UINT64 Value, UINT8 Mode);
VGMPLAYAPI UINT32 CalcSampleMSecExt(UINT64 Value, UINT8 Mode, VGM_HEADER* FileHead);
VGMPLAYAPI const char* GetChipName(UINT8 ChipID);
VGMPLAYAPI const char* GetAccurateChipName(UINT8 ChipID, UINT8 SubType);
VGMPLAYAPI UINT32 GetChipClock(VGM_HEADER* FileHead, UINT8 ChipID, UINT8* RetSubType);

#ifndef NO_WCHAR_FILENAMES
VGMPLAYAPI UINT32 GetGZFileLengthW(const wchar_t* FileName);
VGMPLAYAPI bool OpenVGMFileW(const wchar_t* FileName);
VGMPLAYAPI UINT32 GetVGMFileInfoW(const wchar_t* FileName, VGM_HEADER* RetVGMHead, GD3_TAG* RetGD3Tag);
#endif

VGMPLAYAPI INT32 SampleVGM2Playback(INT32 SampleVal);
VGMPLAYAPI INT32 SamplePlayback2VGM(INT32 SampleVal);

VGMPLAYAPI void PlayVGM(void);
VGMPLAYAPI void StopVGM(void);
VGMPLAYAPI void RestartVGM(void);
VGMPLAYAPI void PauseVGM(bool Pause);
VGMPLAYAPI void SeekVGM(bool Relative, INT32 PlayBkSamples);
VGMPLAYAPI void RefreshMuting(void);
VGMPLAYAPI void RefreshPanning(void);
VGMPLAYAPI void RefreshPlaybackOptions(void);

VGMPLAYAPI UINT32 FillBuffer(WAVE_16BS* Buffer, UINT32 BufferSize);
	
	
/****************************************************************************************************************/
/*VGM_PLAYER_DLL CODE 2016-05-05*/
/****************************************************************************************************************/

VGMPLAYAPI UINT32 GetGZFileLength2(const UINT8* pData, UINT32 uSize, bool* GZ);
VGMPLAYAPI bool OpenVGMFile2(const void* pData, UINT32 uSize);
VGMPLAYAPI UINT32 GetVGMFileInfo2(const void* pData, UINT32 uSize, VGM_HEADER* RetVGMHead, GD3_TAG* RetGD3Tag);

typedef struct playback_options
{
	bool SurroundSound_;
	UINT32 SampleRate_;
	bool DoubleSSGVol_;
	UINT32 VGMMaxLoop_;
	UINT32 VGMPbRate_;
	UINT32 FadeTime_;
	UINT32 PauseTime_;
	UINT32 CHIP_SAMPLE_RATE_;
	float VolumeLevel_;
	UINT8 ResampleMode_;
	UINT8 CHIP_SAMPLING_MODE_;
	UINT8 HardStopOldVGMs_;
} PLBK_OPTS;


VGMPLAYAPI void SetVGMPlaybackOptions(PLBK_OPTS* PlbkOpts);	//Please call before VGMPlay_Init2
VGMPLAYAPI void SetVGMChipsOption(CHIPS_OPTION* ChipsOpt, UINT8 ChipSet);
//VGMPLAYAPI char** GetPointerAppPaths(void);
VGMPLAYAPI CHIPS_OPTION* GetPointerChipsOption(void);
VGMPLAYAPI const bool* GetPointerEndPlayFlag(void);
VGMPLAYAPI UINT32 UncompressGZFile(const UINT8* pSource, UINT32 uSize, UINT8* pDest, UINT32 uDestSize);
VGMPLAYAPI UINT32 CompressFile(const UINT8* pSource, UINT32 uSize, UINT8* pDest, UINT32 uDestSize, INT32 CompLevel);

/****************************************************************************************************************/
/*END OF VGM_PLAYER_DLL CODE*/
/****************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif