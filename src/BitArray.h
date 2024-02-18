#pragma once

#include <vector>
#include <cstdlib>
#include <limits>
#include <cassert>
#include <cstring>
#include <iostream>
#include <bitset>
#include "CoreTypeDef.h"

//--------------------------------------------------------------------
// Class: BitArray
// Desc	: 2 ���� BitArray Class
//--------------------------------------------------------------------
template <typename _Ty>
class BitArray : public std::vector<_Ty>
{
public:
	div_t	div(Int32 pos)					{ return ::div(pos, __NBITMASK); }
	_Ty		divmaskbits(const div_t& dv)	{ return (_Ty)1 << dv.rem; }
	Int32	bitsposition(Int32 x, Int32 y)	{ return (Int32)(y * m_bitswidths + x); }

	Int32	m_bitswidths = 0;
	Int32	m_wordwidths = 0;

	Int32	m_width = 0;
	Int32	m_depth = 0;

public:
	static const Int32	__NBITMASK = static_cast<Int32>(8 * sizeof(_Ty));	// 64bit
	static const _Ty	__FULLBITS = std::numeric_limits<_Ty>::max();
	static const _Ty	__CLEARBITS = 0;

	auto	GetBitsWidths() const-> decltype(m_bitswidths)	{ return m_bitswidths; }
	auto	GetWordWidths() const-> decltype(m_wordwidths)	{ return m_wordwidths; }
	auto	GetWidth()		const->decltype(m_width)		{ return m_width;	}
	auto	GetHeight()		const->decltype(m_depth)		{ return m_depth;	}


	//--------------------------------------------------------------------
	// Class: Create()
	// Desc : width, height ũ��� �����մϴ�.
	//--------------------------------------------------------------------
	void Create(Int32 width, Int32 depth, _Ty val = 0)
	{
		auto	dv = div(width);
		if (dv.rem) ++dv.quot;

		this->m_wordwidths = dv.quot;							// 64bit�� ������ ���� ����
		this->m_bitswidths = this->m_wordwidths * __NBITMASK;	// 64bit�� ������ ���� ������ ǥ�������� bit ����
		this->resize(this->m_wordwidths * depth, val);			// wordwidths_ * depth = �� 64bit ����

		this->m_width = width;
		this->m_depth = depth;
	}

	//--------------------------------------------------------------------
	// Class: Set()
	// Desc : Data�� �����մϴ�.
	//--------------------------------------------------------------------
	bool Set(_Ty* pData, Int32 Count)
	{
		Int32 Size = sizeof(_Ty)*Count;
		assert((Int32)this->size() == Size);
		if ((Int32)this->size() < Size) return false;

		std::memcpy((void*)this->data(), (void*)pData, Size);
		return true;
	}

	//--------------------------------------------------------------------
	// Class: GetValue()
	// Desc : x, y ��ġ�� _Ty ������ �����ɴϴ�.
	//--------------------------------------------------------------------
	_Ty GetValue(Int32 x, Int32 y)
	{
		auto pos = bitsposition(x, y);
		if (pos < 0 || pos >= (static_cast<Int32>(this->size()) * __NBITMASK))
			return ~(0);

		return (*this)[static_cast<Int32>(pos / __NBITMASK)];
	}

	//--------------------------------------------------------------------
	// Class: Clear()
	// Desc : Data�� ��� 0 ���� �ʱ�ȭ�մϴ�.
	//--------------------------------------------------------------------
	void Clear(_Ty val = 0)
	{
		std::fill(this->begin(), this->end(), val);
	}

	//--------------------------------------------------------------------
	// Class: SetAt()
	// Desc : x, y ��ġ Bit�� 1 ���� �����մϴ�.
	//--------------------------------------------------------------------
	bool SetAt(Int32 x, Int32 y, bool flags)
	{
		auto pos = bitsposition(x, y);
		if (pos < 0 || pos >= (static_cast<Int32>(this->size()) * __NBITMASK))
			return false;

		auto dv = div(pos);
		if (flags)
			(*this)[dv.quot] |= divmaskbits(dv);
		else
			(*this)[dv.quot] &= ~divmaskbits(dv);

		return true;
	}

	//--------------------------------------------------------------------
	// Class: IsSet()
	// Desc : x, y ��ġ Bit�� On���� Ȯ���մϴ�.
	//--------------------------------------------------------------------
	bool IsSet(Int32 x, Int32 y)
	{
		auto pos = bitsposition(x, y);
		if (pos < 0 || pos >= (static_cast<Int32>(this->size()) * __NBITMASK))
			return true;

		auto dv = div(pos);
		return ((*this)[dv.quot] & divmaskbits(dv)) ? true : false;
	}

};











