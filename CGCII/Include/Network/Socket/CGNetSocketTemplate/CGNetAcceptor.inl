//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 8.0 / Release 2015.01.05                        *
//*                                                                           *
//*                      Network Socket Template Classes                      *
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
template <class TSOCKET, class TCONNECTABLEMANAGER>
CGNet::CAcceptor<TSOCKET, TCONNECTABLEMANAGER>::CAcceptor(_In_opt_z_ const char* _strName, _In_ int _nMaxAllocate)
	: CGNet::IO::Connective::Acceptor::NTCP(_strName, _nMaxAllocate)
	, NCGDispatchable()
	, TCONNECTABLEMANAGER()	
{
	// 1) Name이 없을 경우 Acceptor의 Name을 설정한다.
	if(_strName==nullptr)
	{
		// Declare) 
		char	temp[MAX_NAME_LENGTH];

		// - 기본 Acceptor의 Name을 설정한다.
		sprintf_s(temp, MAX_NAME_LENGTH, "acceptor Base<%s>", typeid(TSOCKET).name());

		// - 기본 이름을 설정한다.
		SetName(temp);
	}
}

template <class TSOCKET, class TCONNECTABLEMANAGER>
CGNet::CAcceptor<TSOCKET, TCONNECTABLEMANAGER>::CAcceptor(_In_opt_z_ const wchar_t* _strName, _In_ int _nMaxAllocate)
	: CGNet::IO::Connective::Acceptor::NTCP(_strName, _nMaxAllocate)
	, NCGDispatchable()
	, TCONNECTABLEMANAGER()
{
	// 1) Name이 없을 경우 Acceptor의 Name을 설정한다.
	if(_strName==nullptr)
	{
		// Declare) 
		wchar_t	temp[MAX_NAME_LENGTH];

		// - 기본 Acceptor의 Name을 설정한다.
		swprintf_s(temp, MAX_NAME_LENGTH, L"acceptor Base<%hs>", typeid(TSOCKET).name());

		// - 기본 이름을 설정한다.
		SetName(temp);
	}
}

template <class TSOCKET, class TCONNECTABLEMANAGER>
CGNet::CAcceptor<TSOCKET, TCONNECTABLEMANAGER>::~CAcceptor()
{
	// 1) Acceptor를 소멸하기 전 반드시 CloseSocket()을 해주어야 한다!!! (반드시...)
	Stop();
}

template <class TSOCKET, class TCONNECTABLEMANAGER>
CGPTR<CGNet::IO::IConnectable> CGNet::CAcceptor<TSOCKET, TCONNECTABLEMANAGER>::ProcessAllocConnectable()
{
	// Return) Socket을 일단 할당하여 반환한다.
	return NEW<TSOCKET>();
}

template <class TSOCKET, class TCONNECTABLEMANAGER>
void CGNet::CAcceptor<TSOCKET, TCONNECTABLEMANAGER>::_ProcessDestroy()
{
	// 1) Call Parent's _ProcessDestroy function
	CGNet::IO::Connective::Acceptor::NTCP::_ProcessDestroy();

	// 2) Unregister all messageable objects
	ResetDispatchable();
}
