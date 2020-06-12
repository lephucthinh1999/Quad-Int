#pragma once
#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>



class QInt
{
public:
	QInt(const int = 0);
	QInt(const QInt&);

	//get data
	std::bitset<128> getData();

	//Input
	void decimalStringToQInt(std::string&);
	void binaryStringToQInt(std::string&);
	void hexadecimalStringToQInt(std::string&);

	//Output
	std::string toDecimalString();
	std::string toBinaryString();
	std::string toHexadecimalString();

	//Operator
	QInt operator+(const QInt&) const;
	QInt operator-(const QInt&) const;
	QInt operator*(const QInt&) const;
	QInt operator/(const QInt&) const;
	QInt& operator=(const QInt&);
	QInt& operator=(const int);

	
	QInt operator>>(const int&);
	QInt operator<<(const int&);
	QInt operator&(const QInt&) const;
	QInt operator|(const QInt&) const;
	QInt operator^(const QInt&) const;
	QInt& operator~();
	QInt& rol();
	QInt& ror();

	void fromPosToNeg();
	void fromNegToPos();
	void setToOne();

protected:
	void decimalStringDiv2(std::string&);
	std::string twoPowerN(const int);
	std::string addTwoPositiveDecimalString(const std::string&, const std::string&);
	bool operator>(const QInt&) const;
	
private:
	std::bitset<128> data;
};

//Constructor
QInt::QInt(const int value)
{
	this->data = value;
}

//Copy constructor
QInt::QInt(const QInt& object)
{
	data = object.data;
}

//ultilities

//Get data
std::bitset<128> QInt::getData()
{
	return this->data;
}

//Set QInt from zero to One
void QInt::setToOne()
{
	data.flip(data.size() - 1);
}

//Change from positive to negative
void QInt::fromPosToNeg()
{
	//Use bitwise NOT operator to convert from positive to negative counterpart of QInt
		//One's complement format
	*this = ~*this;
	//Add 1 to QInt so it becomes two's complement format
	QInt temp;
	temp.setToOne();
	*this = *this + temp;
}

//Change from negative to positive
void QInt::fromNegToPos()
{
	//Subtract 1 to QInt so it becomes One's complement format
	QInt temp;
	temp.setToOne();
	*this = *this - temp;
	//Use bitwise NOT operator to convert from negative to positive counterpart of QInt
	//Two's complement format
	*this = ~*this;
}

//for addition 
//Divide decimal string by 2
void QInt::decimalStringDiv2(std::string &decimalString)
{
	for(int digits = 0; digits < decimalString.length(); digits++)
	{
		if((decimalString[digits] - '0') % 2 == 1)
			decimalString[digits + 1] = 10 + decimalString[digits + 1];

		decimalString[digits] =(decimalString[digits] - '0') / 2 + '0';

		if(decimalString[0] == '0')
		{
			//decimalString.begin();
			decimalString.erase(decimalString.begin());
			digits--;
		}
	}
}

//for division
//Compare 2 QInt number, return true if QInt1 > QInt2
bool QInt::operator>(const QInt& value) const
{
	if (data == value.data)
		return false;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i]!=value.data[i])
		{
			if (data.test(i))
				return true;
			else
				return false;
		}
	}
}

//Compute 2 raised to the n-th power
std::string QInt::twoPowerN(const int n)// need optimization
{
	std::string result(41, ' ');
	result[40] = '1';
	if (n == 0)
		return result;
	result[40] = '2';
	
	for (int i = 1; i < n; i++)
	{
		char carry = '0';

		for (int j = result.length() - 1; j >= 0; j--)
		{
			result[j] = (result[j] - '0') * 2 + (carry - '0');
			carry = (result[j] / 10) + '0';
			result[j] = result[j] % 10 + '0';
		}

		if (carry != '0')
			result = carry + result;
	}

	return result;
}

//Add 2 positive decimal string and return the sum
std::string QInt::addTwoPositiveDecimalString(const std::string& value1, const std::string& value2)// need optimization
{
	std::string result(41, ' ');
	std::string op1 = value1, op2 = value2;
	//while (op1.length() < op2.length())// err
	//	op1 = "0" + op1;
	//while (op2.length() < op1.length())// err
	//	op2 = "0" + op2;

	int i = op1.length() - 1;
	int carry = 0;

	for (; i >= 0; i--)
	{
		int temp = (op1[i] - '0') + (op2[i] - '0') + carry;
		/*result = (char)(temp % 10 + '0') + result;*/
		result[i] = (char)(temp % 10 + '0');
		carry = temp / 10;
	}

	if (carry == 1)
		result = "1" + result;

	return result;
}

//Decimal
//Change from decimal to QInt
void QInt::decimalStringToQInt(std::string &decimalString)
{
	bool bit, neg = false;
	int bitPos = data.size()-1;

	if(decimalString == "0")
		return;

	if(decimalString[0] == '-')
	{
		decimalString.erase(decimalString.begin());
		neg = true;
	}

	while(decimalString != "")
	{
		bit =(decimalString[decimalString.length() - 1] - '0') % 2;
		data.set(bitPos, bit);
		decimalStringDiv2(decimalString);
		bitPos--;	
	}


	if(neg == true)
	{
		fromPosToNeg();
	}
}

//Change from QInt to decimal
std::string QInt::toDecimalString()// need optimization
{
	std::string decimalString(41, ' ');
	bool neg = false;

	if (data[0] == 1)
	{
		neg = true;
		fromNegToPos();
	}
	
	for (int i = data.size(); i > 0; i--)
		if ( data.test(i-1))
			decimalString = addTwoPositiveDecimalString(twoPowerN(data.size() - i), decimalString);

	decimalString.erase(std::remove(decimalString.begin(), decimalString.end(), ' '), decimalString.end());
	std::cout << decimalString;
	system("pause");
	if (neg == true)
		decimalString = "-" + decimalString;

	return decimalString;
}

//Binary
//Change from binary to QInt
void QInt::binaryStringToQInt(std::string& binaryString) //need optimization
{
	reverse(binaryString.begin(), binaryString.end());
	char carry[128];
	binaryString.copy(carry, binaryString.length() - 1);

	for (int i = binaryString.length()-1; i < 128; ++i)
		carry[i] = '0';

	/*while (true)
	{
		if (binaryString.size()-1 < data.size())
			binaryString = binaryString + "0";
		else
			break;
	}*/
	
	std::string tempstr(carry);
	std::bitset<128> temp(tempstr);
	data = temp;
}

//Change from QInt to binary
std::string QInt::toBinaryString() //need optimization
{
	std::string binaryString = data.to_string();
	while (binaryString[binaryString.size() - 1] == '0')
	{
		binaryString.pop_back();
		if (binaryString.size() - 1 == 0)
			break;
	}
	
	reverse(binaryString.begin(), binaryString.end());
	return binaryString;
}

//Hexadecimal
//Change from hexadecimal to QInt
void QInt::hexadecimalStringToQInt(std::string& hexString)
{
	std::string  binaryString = "";
	std::map<char, std::string> hexToBin
	{
	{ '0', "0000" },{ '1', "0001" },{ '2', "0010" },{ '3', "0011" },
	{ '4', "0100" },{ '5', "0101" },{ '6', "0110" },{ '7', "0111" },
	{ '8', "1000" },{ '9', "1001" },{ 'A', "1010" },{ 'B', "1011" },
	{ 'C', "1100" },{ 'D', "1101" },{ 'E', "1110" },{ 'F', "1111" } 
	};
	for (int i = 0; i < hexString.length(); i++)
		 binaryString =  binaryString + hexToBin[hexString[i]];

	binaryStringToQInt(binaryString);
	
}

//Change from QInt to hexadecimal
std::string QInt::toHexadecimalString()
{
	std::string binaryString = toBinaryString();
	std::string hexString = "";
	while (binaryString.size() % 4 != 0)
	{
		binaryString = "0" + binaryString;
	}
	std::map<std::string, std::string> binToHex
	{
	{ "0000", "0" },{ "0001", "1" },{ "0010", "2" },{ "0011", "3" },
	{ "0100", "4" },{ "0101", "5" },{ "0110", "6" },{ "0111", "7" },
	{ "1000", "8" },{ "1001", "9" },{ "1010", "A" },{ "1011", "B" },
	{ "1100", "C" },{ "1101", "D" },{ "1110", "E" },{ "1111", "F" } 
	};

	int pos = 0;
	for (int i = 0; i < binaryString.length() / 4; i++)
	{
		std::string str1 = binaryString.substr(pos, 4);
		hexString += binToHex[str1];
		pos += 4;
	}

	return hexString;
}

//Operator + (Add).
QInt QInt::operator+(const QInt& value) const
{
	QInt bin1 = *this;
	QInt bin2 = value;
	QInt result;
	QInt carry;
	result = bin1 ^ bin2;

	carry = bin1 & bin2;
	carry = carry << 1;
	
	do
	{
		bin1 = result;
		bin2 = carry;

		result = bin1 ^ bin2;

		carry = bin1 & bin2;
		carry = carry << 1;
	}
	while (!(carry.data.none()));

	return result;
}

//Operator - (Subtract).
QInt QInt::operator-(const QInt& value) const
{
	QInt bin = value;
	bin.fromPosToNeg();

	QInt result = *this + bin;
	return result;

}

//Operator * (Multiply).
QInt QInt::operator*(const QInt& value) const
{
	QInt bin1 = *this;
	QInt bin2;

	QInt result;
	if (value.data.none())
		return result;
	for (int i = value.data.size()-1; i >0; i--)
	{
		if (value.data[i] == 1)
		{
			bin2 = bin1;
			result = result + bin2;
		}
		
		bin1 << 1;
	}
	return result;
}

//Operator / (Divide).
QInt QInt::operator/(const QInt& value) const
{
	QInt divident = *this;
	QInt divisor = value;
	QInt remainder;
	QInt quotient;
	bool neg = false;
	if (divisor.data.none())
		throw "Division by zero condition!";
	if (divident.data.test(0) == divisor.data.test(0))
	{
		if (divident.data.test(0))
		{
			divident.fromNegToPos();
			divisor.fromNegToPos();
		}
	}
	else
	{
		neg = true;
		if (divident.data.test(0))
			divident.fromNegToPos();
		else
			divisor.fromNegToPos();
	}
	if (divisor > divident)
		return quotient;
	for(int pos = 0; pos < divident.data.size() ; pos++)
	{
		remainder << 1;
		remainder.data[divident.data.size() - 1] = divident.data[pos];
		if(remainder>divisor)
		{
			quotient << 1;
			quotient.data.flip(divident.data.size() - 1);
			remainder = remainder - divisor;
		}
		else
			quotient << 1;
	}
	if (neg==true)
		quotient.fromPosToNeg();
	return quotient;
}

//Operator >>(Shift Right).
QInt QInt::operator >>(const int &ShiftOffset)
{
	this->data <<= ShiftOffset;

	return *this;
}

//Operator <<(Shift Left).
QInt QInt::operator <<(const int &ShiftOffset)
{
	this->data >>= ShiftOffset;

	return *this;
}

//operator &(AND)
QInt QInt::operator &(const QInt &value) const
{
	QInt result;

	result.data = value.data & this->data;

	return result;
}

//Operator |(OR).
QInt QInt::operator |(const QInt &value) const
{
	QInt result;

	result.data = value.data | this->data;

	return result;
}

//Operator ^(XOR).
QInt QInt::operator ^(const QInt &value) const
{
	QInt result;

	result.data = value.data ^ this->data;

	return result;
}

//Operator ~(NOT).
QInt& QInt::operator ~()
{
	this->data = ~(this->data);

	return *this;
}

//Assignment operator =
QInt& QInt::operator=(const QInt &value)
{
	this->data = value.data;
	return *this;
}
QInt& QInt::operator=(const int value)
{
	this->data = value;
	return *this;
}

//Operator rotate left(ROL).
QInt& QInt::rol()
{
	if (this->data[0] == 1)
	{
		*this = *this << 1;
		this->data[data.size() - 1] = 1;
	}
	else
	*this = *this << 1;

	return *this;
}

//Operator rotate right(ROR).
QInt& QInt::ror()
{
	if (this->data[data.size() - 1] == 1)
	{
		*this = *this >> 1;
		this->data[0] =1;
	}
	else
		*this = *this >> 1;

	return *this;
}
	
//Main ultities
void Scan(QInt &, std::string, int);
void Print(std::ofstream&, QInt, int);

void Scan(QInt &q, std::string str, int base)
{
	switch (base)
	{
	case 2:
		q.binaryStringToQInt(str);
		break;
	case 10:
		q.decimalStringToQInt(str);
		break;
	case 16:
		q.hexadecimalStringToQInt(str);
		break;
	}
}

void Print(std::ofstream & outfile, QInt q, int base)
{
	switch (base)
	{
	case 2:
		outfile << q.toBinaryString();
		break;
	case 10:
		outfile << q.toDecimalString();
		break;
	case 16:
		outfile << q.toHexadecimalString();
		break;
	}
}
