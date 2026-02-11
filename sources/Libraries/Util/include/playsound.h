/**
*  @file playsound.h
*
*  playsound.h, v 2.0 2004/8/22 13:14:05 shanghai China
*
*  @author claude Fan <fanyf2002@hotmail.com>
*
*/


#ifndef _PLAYSOUND_H_
#define _PLAYSOUND_H_

// 音乐系统初始化，在应用程序入口点调用
bool mus_mgr_init(bool enable, DWORD_PTR dwValue = 0);
// 音乐系统Reset，在切换场景时调用
void mus_mgr_reset(int op = 0); // op为-1清除所有音乐, 0清除除背景音乐外的所有音乐
// 音乐系统退出，在应用程序退出点调用
void mus_mgr_exit();


// 播放环境音效
bool env_snd_is_full(); // 环境音效通道数是否已经全被占用
int env_snd_add(char const* fname); // 返回-1表示环境音效通道已满
bool env_snd_vol(int id, int vol); // vol >= 0，负数返回false，超过128按128计算
bool env_snd_play(int id);
inline bool env_snd_del(int id) {return env_snd_vol(id, 0);}

// 播放普通音效
void cmn_snd_set_cache(int n = 20); // n是缓冲WAVE文件的最大个数
bool cmn_snd_play(char const* fname, int vol = 128); // vol >= 0，负数返回false，超过128按128计算

// 使能音效
void snd_enable(bool enable = true);

// 播放背景音乐
void bkg_snd_set_notify(HWND hwnd, UINT msg_id);
bool bkg_snd_play(char const* fname, bool loop = false);
void bkg_snd_vol(int vol);
void bkg_snd_stop();


//////////////////////////////////////////////////////////////////////////
//
// 以下的老接口全部用新接口实现，这些接口会逐步废弃 by claude Fan at 2004-10-20
//
//////////////////////////////////////////////////////////////////////////

// 操作背景音乐
inline void SetBkMusicNotify(HWND hWnd, UINT uMsgId)
    {bkg_snd_set_notify(hWnd, uMsgId);}
inline int PlayBkMusic(char const* filename, bool loop = false)
    {return int(bkg_snd_play(filename, loop));}
inline int StopBkMusic() {bkg_snd_stop(); return 0;}

// 播放音效
inline void SetWaveQueueLen(int n = 20) {cmn_snd_set_cache(n);}
inline int PlayMusic(char const* filename, int reserved = 0)
    {return int(cmn_snd_play(filename));}

#endif // _PLAYSOUND_H_

