#pragma once
#include "Def_Enum.h"

#ifdef LT_EQP	// 프로젝트 설정에서 /D 커맨드라인 사용 할 경우
#define		SET_INSPECTOR		LT_EQP
#else			// 수동 설정
#define		SET_INSPECTOR		SYS_GHOSTFLARE
#endif

#define		SW_VERSION			_T("[S/W Ver] 22.07.19.1")

#define		DevMode_Simulator

