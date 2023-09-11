#pragma once

#ifndef BUFFER_INC_
#define BUFFER_INC_

template <typename type>
class CStack
{
public:
	CStack(const ULONG ulSize)
	{
		m_lpBuffer	= NULL;

		AllocStack(ulSize);
	}

	CStack(void)
	{
		m_lpBuffer	= NULL;
		m_ulHead	= 0;
		m_ulSize	= 0;
	}

	~CStack(void)
	{
		FreeStack();
	}

private:
	type	* m_lpBuffer;

	ULONG	m_ulHead;
	ULONG	m_ulSize;

public:
	void	AllocStack(const ULONG ulSize)
	{
		FreeStack();

		m_lpBuffer = new type [ulSize];
		m_ulSize = ulSize;

		Clear();
	}

	void	FreeStack()
	{
		if(m_lpBuffer)
			delete [] m_lpBuffer;

		m_lpBuffer	= NULL;
		m_ulHead	= 0;
		m_ulSize	= 0;
	}

	void	Clear()
	{
		for(ULONG i = 0; i < m_ulSize; i++)
			m_lpBuffer[i] = type();

		m_ulHead = 0;
	}

	ULONG	GetCount() const
	{
		return m_ulHead;
	}

	BOOL	Push(const type Data)
	{
		if(GetCount() >= m_ulSize)
			return FALSE;

		m_lpBuffer[m_ulHead++] = Data;

		return TRUE;
	}

	BOOL	Pop(type &Data)
	{
		if(GetCount() <= 0)
			return FALSE;

		Data = m_lpBuffer[--m_ulHead];

		return TRUE;
	}
};


template <typename type>
class CQueue
{
public:
	CQueue(const ULONG ulSize)
	{
		m_lpBuffer	= NULL;

		AllocQueue(ulSize);
	}

	CQueue(void)
	{
		m_lpBuffer	= NULL;
		m_ulHead	= 0;
		m_ulTail	= 0;
		m_ulSize	= 0;
	}

	~CQueue(void)
	{
		FreeQueue();
	}

private:
	type	* m_lpBuffer;

	ULONG	m_ulHead;
	ULONG	m_ulTail;
	ULONG	m_ulSize;

public:
	void	AllocQueue(const ULONG ulSize)
	{
		FreeQueue();

		m_lpBuffer	= new type [ulSize];
		m_ulSize	= ulSize;

		Clear();
	}

	void	FreeQueue()
	{
		if(m_lpBuffer)
			delete [] m_lpBuffer;

		m_lpBuffer	= NULL;
		m_ulHead	= 0;
		m_ulTail	= 0;
		m_ulSize	= 0;
	}

	void	Clear()
	{
		for(ULONG i = 0; i < m_ulSize; i++)
			m_lpBuffer[i] = type();

		m_ulHead	= 0;
		m_ulTail	= 0;
	}

	ULONG	GetCount() const
	{
		return (m_ulHead - m_ulTail);
	}

	BOOL	Push(const type Data)
	{
		if(GetCount() >= m_ulSize)
			return FALSE;

		m_lpBuffer[m_ulHead % m_ulSize] = Data;
		m_ulHead++;

		return TRUE;
	}

	BOOL	Pop(type &Data)
	{
		if(GetCount() <= 0)
			return FALSE;

		Data = m_lpBuffer[m_ulTail % m_ulSize];
		m_ulTail++;

		return TRUE;
	}
};

#endif // BUFFER_INC_
