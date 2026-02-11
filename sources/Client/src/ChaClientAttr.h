#pragma  once
struct SClientAttr
{
	short sTeamAngle; // 组队时的头像角度
	float fTeamDis;   // 组队时的头像距离
	float fTeamHei;   // 组队时的头像距离

	SClientAttr()
		:sTeamAngle(0),
		fTeamDis(0),
		fTeamHei(0)
	{
	}
};

extern SClientAttr g_ClientAttr[2000];

// 参数为CharacterInfo里面的ID
inline SClientAttr* GetClientAttr(int nScriptID)
{
	return &g_ClientAttr[nScriptID];
}


struct SCameraMode
{
	float	m_fminxy ;
	float   m_fmaxxy ;
	float	m_fminHei;
	float   m_fmaxHei;
	float   m_fminfov;
	float   m_fmaxfov;
	int		m_nShowWidth;
	int		m_nShowHeight;

	BOOL	m_bRotate;
};

extern SCameraMode CameraMode[4];
