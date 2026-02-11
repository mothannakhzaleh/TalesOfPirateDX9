
//---------------------------
//
//  音频播放
//  by Jampe
//  2007/03/28
//
//---------------------------


#if !defined __AUDIO_SDL_H__
#define __AUDIO_SDL_H__


#include "datatype.h"



class AudioSDL : public _singleton<AudioSDL>
{
    SINGLETON_OBJECT(AudioSDL);
public:
    ~AudioSDL();

    //  初始化声音系统
    bool init();

    //  释放声音系统
    void release();

    //  声音系统是否有效
    bool is_valid();

    //  获取音乐资源索引
    ulong get_resID(const char* resource, int type);

    //  播放
    bool play(ulong id, bool loop = false);

    //  渐入
    bool fadeIn(ulong id, int ms, bool loop = false);

    //  渐退
    bool fadeOut(ulong id, int ms);

    //  停止播放
    bool stop(ulong id);

    //  暂停
    bool pause(ulong id);

    //  恢复
    bool resume(ulong id);

    //  重置(仅音乐有效.如MP3,OGG等)
    bool rewind(ulong id);

    //  是否正在播放
    bool is_playing(ulong id);

    //  是否暂停
    bool is_paused(ulong id);

    //  是否停止
    bool is_stopped(ulong id);

    //  调整音量
    bool volume(ulong id, int vol);

    //  检测资源有效性(清楚超时未引用资源)(秒)
    void checkRes(ulong timeout = 300);

protected:
    AudioSDL();
};


#define GetAudioDevice()            AudioSDL::get_instance()


#ifdef _DEBUG
    #pragma comment(lib, "AudioSDL_D.lib")
#else
    #pragma comment(lib, "AudioSDL.lib")
#endif


#endif
