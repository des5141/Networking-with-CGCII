//*****************************************************************************
//*                                                                           *
//*                      Cho sanghyun's Game Classes II                       *
//*                       Ver 8.0 / Release 2015.01.05                        *
//*                                                                           *
//*                          Group Template Classes                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*                                                                           *
//*  This Program is programmed by Cho SangHyun. sangduck@cgcii.co.kr         *
//*  Best for Game Developement and Optimized for Game Developement.          *
//*                                                                           *
//*                 �� 2016 Cho Sanghyun All right reserved.                  *
//*                          http://www.CGCII.co.kr                           *
//*                                                                           *
//*****************************************************************************

#pragma once

namespace CGServerGroup
{

//-----------------------------------------------------------------------------
/**

 @class		CArraySender
 @brief		�迭������ ����� �����ϴ� �׷� Ŭ������ �ڸ��� �ڵ� �������ش�.
 @todo		
 @ref		CGServerGroup::CArray
 @details	\n
 �迭�� �׷쿡 ��ü ��� ���� ����� �߰��� Ŭ�����̴�.
       
*///-----------------------------------------------------------------------------
template <class _TMEMBER, class _TMEMBER_DATA=int, class _TMEMBERBASE=_TMEMBER>
class CArraySender : 
// ****************************************************************************
// Inherited classes)
// ----------------------------------------------------------------------------
	public						CArray<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>,// (@) Group
	virtual public				CGNet::IO::ISender						//     Sender Base
{
// ****************************************************************************
// Definitnion)
// ----------------------------------------------------------------------------
public:
	typedef	CArray<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>			BASEGROUP;
	typedef	_TMEMBER												TMEMBER;
	typedef	_TMEMBERBASE											TMEMBERBASE;
	typedef	_TMEMBER_DATA											TMEMBER_DATA;
	typedef typename BASEGROUP::ITERATOR							ITERATOR;
	typedef typename BASEGROUP::CONST_ITERATOR						CONST_ITERATOR;


// ****************************************************************************
// Constructor/Destructor)
// ----------------------------------------------------------------------------
public:
			CArraySender(int _nMaxMember = 256);
	virtual	~CArraySender();


// ****************************************************************************
// Public)
// ----------------------------------------------------------------------------
public:
	//! @brief ��ü ������� �޸� ������ ������ �����Ѵ�. @param _Buffer ������ �޸� ���� @return true ���� @return false ����
	virtual	bool				Send(_In_ const CCGBuffer& _Buffer) override;
	//! @brief Ư�� ������� �޸� ���۸� �����Ѵ�. @param _iMember ������ ��� �ε��� @param _Buffer ������ �޸� ���� @return true ���� @return false ����
			bool				SendTo(_In_ int _iIndex, _In_ const CCGBuffer& _Buffer);
	//! @brief Ư�� ����� ������ ��ü ������� �޸� ������ �����Ѵ�. @param _pExcept ������ ��� @param _Buffer ������ �޸� ���� @return true ���� @return false ����
			bool				SendExcept(_In_ TMEMBER* _pExcept, _In_ const CCGBuffer& _Buffer);
	//! @brief ������ ������ ������Ը� �޸� ������ �����Ѵ�. @param _Buffer ������ �޸� ���� @param _fPred ���� �Լ� @return true ���� @return false ����
			bool				SendConditional(_In_ const CCGBuffer& _Buffer, _In_ const std::function<bool (const TMEMBER*)>& _fPred);
};


template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::CArraySender(int _nMaxMember) : 
	CArray<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>(_nMaxMember)
{
}

template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::~CArraySender()
{
}

template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
bool CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::Send(_In_ const CCGBuffer& _Buffer)
{
	// Check) _Buffer->buf�� nullptr�� �ƴѰ�?
	ERROR_RETURN_IF(_Buffer.buf==nullptr, false, )

	// Check) _Buffer->len�� 0�� �ƴѰ�?
	ERROR_RETURN_IF(_Buffer.len==0, false, )
	
	LOCK(ICGServerGroup<TMEMBERBASE>::m_csGroup)
	{
		int		iCount	 = BASEGROUP::GetMemberCount();

		for(auto iter= BASEGROUP::m_containerMember.begin(); iCount>0; ++iter)
		{
			CONTINUE_IF((*iter).IsEmpty());

			(*iter)->Send(_Buffer);

			--iCount;
		}
	}

	// Return)
	return	true;
}

template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
bool CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::SendTo(_In_ int _iIndex, _In_ const CCGBuffer& _Buffer)
{
	// Check) _Buffer->buf�� nullptr�� �ƴѰ�?
	ERROR_RETURN_IF(_Buffer.buf==nullptr, false, )

	// Check) _Buffer->len�� 0�� �ƴѰ�?
	ERROR_RETURN_IF(_Buffer.len==0, false, )

	LOCK(ICGServerGroup<TMEMBERBASE>::m_csGroup)
	{
		// Check) _Index�� ������ ���� ���� �ִ°�?
		ERROR_RETURN_IF(_iIndex<0 || _iIndex>= BASEGROUP::GetMaxMemberCount(), false, )

		// 1) �ش� Member�� ��´�.
		CGPTR<TMEMBER>	pMember	 = BASEGROUP::GetMember(_iIndex);

		// Check)
		RETURN_IF(pMember.empty(), false);

		// 2) �ش� Member���� �����Ѵ�.
		pMember->Send(_Buffer);
	}

	// Return)
	return	true;
}

template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
bool CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::SendExcept(_In_ TMEMBER* _pExcept, _In_ const CCGBuffer& _Buffer)
{
	LOCK(ICGServerGroup<TMEMBERBASE>::m_csGroup)
	{
		// Declare) 
		int		iCount	 = BASEGROUP::GetMemberCount();

		auto iter		 = BASEGROUP::m_containerMember.begin();
		auto iterEnd	 = BASEGROUP::m_containerMember.end();

		// 1) ���ܽ�ų ��ü�� ã����...(��ü�� ����.)
		for(; iCount>0; ++iter)
		{
			// Check)
			CGASSERT_ERROR(iter!=iterEnd);

			// Check) 
			RETURN_IF(iter==iterEnd, false);

			// Check) nullptr�̸� ����...
			CONTINUE_IF((*iter).IsEmpty());

			// Check) _pExcept�� ��� ����...
			if((*iter).GetMember()==_pExcept)
			{
				++iter;
				break;
			}

			(*iter)->Send(_Buffer);

			--iCount;
		}

		// 2) ���ܽ�ų ��ü�� ã�� ��...(��ü �񱳾��� �ٷ� ����)
		for(; iCount>0; ++iter)
		{
			// Check)
			CGASSERT_ERROR(iter!=iterEnd);

			// Check) 
			RETURN_IF(iter==iterEnd, false);

			// Check) nullptr�̸� ����...
			CONTINUE_IF((*iter).IsEmpty());

			(*iter)->Send(_Buffer);

			--iCount;
		}
	}

	// Return)
	return	true;
}

template <class _TMEMBER, class _TMEMBER_DATA, class _TMEMBERBASE>
bool CArraySender<_TMEMBER, _TMEMBER_DATA, _TMEMBERBASE>::SendConditional(_In_ const CCGBuffer& _Buffer, _In_ const std::function<bool (const TMEMBER*)>& _fPred)
{
	LOCK(ICGServerGroup<TMEMBERBASE>::m_csGroup)
	{
		int		iCount	 = BASEGROUP::GetMemberCount();

		for(auto iter= BASEGROUP::m_containerMember.begin(); iCount>0; ++iter)
		{
			// Check) Member�� ��� ������ �Ѿ��.
			CONTINUE_IF((*iter).IsEmpty());

			// Check) ���ǿ� ���� ������ ��! fPred�Լ��� ����� false�̸� �׳� �Ѿ��.
			CONTINUE_IF(_fPred((*iter).GetMember())==false);

			(*iter)->Send(_Buffer);

			--iCount;
		}
	}

	// Return) ����...
	return	true;
}


}