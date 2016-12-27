// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	if (BitLen > 0)
	{
		MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
		pMem = new TELEM[MemLen];
		if (pMem != NULL)
			for (int i = 0; i < MemLen; ++i)
				pMem[i] = 0;

	}
	else
		throw std::logic_error("");
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; ++i)
			pMem[i] = bf.pMem[i];
	
}

TBitField::~TBitField()
{
	delete [] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw std::logic_error("");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= 0) && (n < BitLen))
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else
		throw std::logic_error("");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= 0) && (n < BitLen))
		return pMem[GetMemIndex(n)] & GetMemMask(n);
	else
		throw std::logic_error("");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (pMem != NULL)
		delete[] pMem;
	pMem = new TELEM[MemLen];
	if (pMem != NULL)
		for (int i = 0; i < MemLen; ++i)
			pMem[i] = bf.pMem[i];
	return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < MemLen; ++i)
			if (pMem[i] != bf.pMem[i])
				return 0;
		return 1;
	}
  return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
	{
		int f = 0;
		for (int i = 0; i < MemLen; ++i)
			if (pMem[i] != bf.pMem[i])
			{
				f = 1;
				break;
			}
		return f;
	}
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = max(BitLen, bf.BitLen);
	TBitField tmp(len);
	for (int i = 0; i < tmp.MemLen; ++i)
		tmp.pMem[i] = bf.pMem[i];
	int mlen = min (MemLen, tmp.MemLen);
	for (int i = 0; i < mlen; ++i)
		tmp.pMem[i] |= pMem[i]; 
	return tmp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = max(BitLen, bf.BitLen);
	TBitField tmp(len);
	int mlen = min(MemLen, tmp.MemLen);
	for (int i = 0; i < mlen; ++i)
		tmp.pMem[i] = bf.pMem[i];
	for (int i = 0; i < mlen; ++i)
		tmp.pMem[i] &= pMem[i];
	for (int i = mlen; i < tmp.MemLen; ++i)
		tmp.pMem[i] = 0;
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	/*int len = BitLen;
	TBitField tmp (len);
	for (int i = 0; i < MemLen; ++i)
		tmp.pMem[i] = ~pMem[i];
	return tmp;*/
	TBitField tmp = (*this);
	for (int i = 0; i < BitLen; ++i)
	{
		if (tmp.GetBit(i))
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char ch;
    while (true)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else if (ch == '1')
			bf.SetBit(i++);
		else
			break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i < len; ++i)
	{
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	}
	return ostr;
}
