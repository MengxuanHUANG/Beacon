// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"

Unit& Unit::operator = (const Unit& otherUnit)
{
	this->m_Value = otherUnit.GetValue();
	return *this;
}

Unit& Unit::operator = (float value)
{
	this->m_Value = value;
	return *this;
}

Unit& Unit::operator + (float value)
{
	this->m_Value += value;
	return *this;
}

Unit& Unit::operator += (float value)
{
	return (*this) + value;
}

Unit& Unit::operator - (float value)
{
	this->m_Value -= value;
	return *this;
}

Unit& Unit::operator -= (float value)
{
	return (*this) - value;
}

Unit& Unit::operator * (float value)
{
	this->m_Value *= value;
	return *this;
}

Unit& Unit::operator *= (float value)
{
	return (*this) * value;
}

Unit& Unit::operator / (float value)
{
	this->m_Value /= value;
	return *this;
}

Unit& Unit::operator /= (float value)
{
	return (*this) / value;
}