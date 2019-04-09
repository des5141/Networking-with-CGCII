//*****************************************************************************
//*                                                                           *
//*                      Cho SangHyun's Game Classes II                       *
//*                       Ver 8.0 / Release 2015.01.05                        *
//*                                                                           *
//*                             CG Object Classes                             *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangduck@cgcii.co.kr         *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                 ⓒ 2016 Cho Sanghyun All right reserved.                  *
//*                          http://www.CGCII.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once


//----------------------------------------------------------------------------
// CGCII afxwin Prefix
// 
// 이 파일은 MFC를 사용할 때 stdafx.h 파일의 "afxwin.h"를 include 하기전에
// Include 해줘야 한다.
// 이 파일의 include는 Microsof C++ Core Guide lines의 포함으로 필요한 설정이다.
// 
// afxwin.h의 매크로인 min()과 max()의 중첩으로 인한 문제를 해결하기 위한 것이다. 
// 
//    1) NOMINMAX를 afxwin.h가 Include 되기 전에 선언해 afxwin.h의 min()과 
//       max()를 무력화한다.
//    2) 기존 매크로 min()/max()의 대체로 함수로 된 min()과 max()를 선언한다.
// 
//----------------------------------------------------------------------------
// Debug)
#if defined(_MSC_VER)
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#define _INC_MALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
	#endif
#endif

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#if !defined(min) & !defined(_FUNCTION_MIN)
	#define	_FUNCTION_MIN

	template<class VALUE>
	const VALUE& min(const VALUE& _a, const VALUE& _b)
	{
		return	(_a>_b) ? _b : _a;
	}
#endif

#if !defined(max) & !defined(_FUNCTION_MAX)
	#define	_FUNCTION_MAX

	template<class VALUE>
	const VALUE& max(const VALUE& _a, const VALUE& _b)
	{
		return	(_a>_b) ? _a : _b;
	}
#endif
