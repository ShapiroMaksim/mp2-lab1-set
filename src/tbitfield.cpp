// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)
		throw(len);
	BitLen = len;
	MemLen = BitLen / 32 + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n<0 || n>BitLen)
		throw n;
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n<0 || n>BitLen)
		throw n;
	int m = n % 32;
	TELEM res = 1;
	res = res << m;
	return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0 || n>BitLen)
		throw n;
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>BitLen)
		throw n;
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>BitLen)
		throw n;
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	if ((pMem[index] & mask) == 0)
		return 0;
	else
		return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	else
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] != bf.pMem[i])
				return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int lenght;
	if (BitLen > bf.BitLen)
		lenght = BitLen;
	else
		lenght = bf.BitLen;
	TBitField tmp(lenght);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int lenght;
	if (BitLen > bf.BitLen)
		lenght = BitLen;
	else
		lenght = bf.BitLen;
	TBitField tmp(lenght);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (this->GetBit(i) == 0)
			tmp.SetBit(i);
		else
			tmp.ClrBit(i);
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char symbol;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> symbol;
		if (symbol == '1')
			bf.GetBit(i);
		else if (symbol == '0')
			bf.ClrBit(i);
		else
		{
			for (i; i < bf.GetLength(); i++)
				bf.GetBit(i);
			break;
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) == 0)
			ostr << 0;
		else
			ostr << 1;
	}
	return ostr;
}
