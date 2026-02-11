// 天气本质上是指系统产生的地面状态, 比如风,  雷,  雾
// 处于该地面状态下的客户端表现为播放范围特效


// 目前天气系统有2个作用
// 一. 为航海士的贝壳充值
// 二. 影响玩家的移动速度, 生命值等, 丰富游戏表现

#pragma once

class SubMap;

class CWeather
{
public:

	CWeather(BYTE btType) // 构造时必须传入天气类型
	:_dwLastLocationTick(0), _dwFrequence(2)
	{
		_btType = btType;
		_dwStateLastTime = 20; // 默认为20秒
	}
	
	void	SetRange(int sx, int sy, int w, int h);	// 指定一个大的坐标范围
	void	RandLocation(SubMap *pMap);				// 在指定的范围内, 随机选择地点出现
	void	SetFrequence(DWORD dwFre);				// 设定出现频率, 单位是秒
	void	SetStateLastTime(DWORD dwTime);			// 设置状态持续时间, 单位是秒

protected:

	BYTE	_btType;  // 天气类型 0风  1雷  2雾
	int		_sx;	  // 记录坐标范围
	int		_sy;
	int		_w;
	int		_h;
	DWORD	_dwLastLocationTick;
	DWORD	_dwFrequence;
	DWORD	_dwStateLastTime;
};

inline void CWeather::SetRange(int sx, int sy, int w, int h)
{
	_sx = sx;
	_sy = sy;
	_w  = w;
	_h  = h;
}

inline void CWeather::SetFrequence(DWORD dwFre)
{
	_dwFrequence = dwFre;
}

inline void CWeather::SetStateLastTime(DWORD dwTime)
{
	_dwStateLastTime = dwTime;
}


// 天气管理类, 可以添加, 删除, 遍历所有天气，需为每个地图对象实例化一个
class CWeatherMgr 
{
public:
	
	~CWeatherMgr();
	void	AddWeatherRange(CWeather *pWeather);
	void	RemoveWeather(CWeather *pWeather);
	void	Run(SubMap *pMap);	// 被定时器调用, 内部会调用每一个Weather的RandLocation
	void	ClearAll();

protected:

	std::list<CWeather*>  _WeatherList;
};

inline void CWeatherMgr::AddWeatherRange(CWeather *pWeather)
{
	_WeatherList.push_back(pWeather);
}

inline void CWeatherMgr::RemoveWeather(CWeather *pWeather)
{
	_WeatherList.remove(pWeather);
	delete pWeather;
}

inline void CWeatherMgr::ClearAll()
{
	for(std::list<CWeather*>::iterator it = _WeatherList.begin(); it!=_WeatherList.end(); it++)
	{
		CWeather *pCur = (*it);
		delete pCur;
	}

	_WeatherList.clear();
}


