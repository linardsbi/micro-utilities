/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <avr/pgmspace.h>

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

class __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
// The String class
template<typename CharType>
class StringBase
{
public:
	// constructors
	// creates a copy of the initial value.
	// if the initial value is null or invalid, or if memory allocation
	// fails, the String will be marked as invalid (i.e. "if (s)" will
	// be false).

	 constexpr explicit StringBase(const CharType *cstr = "");
	 constexpr explicit StringBase(const StringBase<CharType> &str);
	 constexpr explicit StringBase(const __FlashStringHelper *str);
       #if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
	 constexpr StringBase(StringBase<CharType> &&rval);
	#endif
    constexpr explicit StringBase(unsigned count, CharType c);
	constexpr explicit StringBase(CharType c);
	constexpr explicit StringBase(unsigned char, unsigned char base=10);
	constexpr explicit StringBase(int, unsigned char base=10);
	constexpr explicit StringBase(unsigned int, unsigned char base=10);
	constexpr explicit StringBase(long, unsigned char base=10);
	constexpr explicit StringBase(unsigned long, unsigned char base=10);
	constexpr StringBase(float, unsigned char decimalPlaces=2);
	constexpr StringBase(double, unsigned char decimalPlaces=2);
	constexpr ~StringBase();

	// memory management
	// return true on success, false on failure (in which case, the string
	// is left unchanged).  reserve(0), if successful, will validate an
	// invalid String (i.e., "if (s)" will be true afterwards)
	constexpr unsigned char reserve(unsigned int size);
	constexpr inline unsigned int length() const {return len;}

	// creates a copy of the assigned value.  if the value is null or
	// invalid, or if the memory allocation fails, the String will be
	// marked as invalid ("if (s)" will be false).
	 constexpr StringBase<CharType> & operator = (const StringBase<CharType> &rhs);
	 constexpr StringBase<CharType> & operator = (const CharType *cstr);
	 constexpr StringBase<CharType> & operator = (const __FlashStringHelper *str);
       #if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
	 constexpr StringBase<CharType> & operator = (StringBase<CharType> &&rval);
	#endif

	// concatenate (works w/ built-in types)

	// returns true on success, false on failure (in which case, the string
	// is left unchanged).  if the argument is null or invalid, the
	// concatenation is considered unsuccessful.
	constexpr unsigned char concat(const StringBase<CharType> &str);
	constexpr unsigned char concat(const CharType *cstr);
	constexpr unsigned char concat(CharType c);
	constexpr unsigned char concat(unsigned char c);
	constexpr unsigned char concat(int num);
	constexpr unsigned char concat(unsigned int num);
	constexpr unsigned char concat(long num);
	constexpr unsigned char concat(unsigned long num);
	constexpr unsigned char concat(float num);
	constexpr unsigned char concat(double num);
	constexpr unsigned char concat(const __FlashStringHelper * str);

	// if there's not enough memory for the concatenated value, the string
	// will be left unchanged (but this isn't signalled in any way)
	 constexpr StringBase<CharType> & operator += (const StringBase<CharType> &rhs)	{concat(rhs); return (*this);}
	 constexpr StringBase<CharType> & operator += (const CharType *cstr)		{concat(cstr); return (*this);}
	 constexpr StringBase<CharType> & operator += (char c)			{concat(c); return (*this);}
	 constexpr StringBase<CharType> & operator += (unsigned char num)		{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (int num)			{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (unsigned int num)		{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (long num)			{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (unsigned long num)	{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (float num)		{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (double num)		{concat(num); return (*this);}
	 constexpr StringBase<CharType> & operator += (const __FlashStringHelper *str){concat(str); return (*this);}

    /*********************************************/
    /*  Concatenate                              */
    /*********************************************/

    constexpr StringBase<CharType> & operator + (const StringBase<CharType> &rhs) const
    {
        if (!concat(rhs.m_buffer, rhs.len)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (const CharType *cstr) const
    {
        if (!cstr || !concat(cstr, strlen(cstr))) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (CharType c) const
    {
        if (!concat(c)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (unsigned char num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (int num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (unsigned int num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (long num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (unsigned long num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (float num) const 
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (double num) const
    {
        if (!concat(num)) invalidate();
        return this;
    }

    constexpr StringBase<CharType> & operator + (const __FlashStringHelper *rhs) const
    {
        if (!concat(rhs))	invalidate();
        return this;
    }

	// comparison (only works w/ Strings and "strings")
	constexpr operator bool() const { return len > 0; }
	constexpr int compareTo(const StringBase<CharType> &s) const;
	constexpr unsigned char equals(const StringBase<CharType> &s) const;
	constexpr unsigned char equals(const CharType *cstr) const;
	constexpr unsigned char operator == (const StringBase<CharType> &rhs) const {return equals(rhs);}
	constexpr unsigned char operator == (const CharType *cstr) const {return equals(cstr);}
	constexpr unsigned char operator != (const StringBase<CharType> &rhs) const {return !equals(rhs);}
	constexpr unsigned char operator != (const CharType *cstr) const {return !equals(cstr);}
	constexpr unsigned char operator <  (const StringBase<CharType> &rhs) const;
	constexpr unsigned char operator >  (const StringBase<CharType> &rhs) const;
	constexpr unsigned char operator <= (const StringBase<CharType> &rhs) const;
	constexpr unsigned char operator >= (const StringBase<CharType> &rhs) const;
	constexpr unsigned char equalsIgnoreCase(const StringBase<CharType> &s) const;
	constexpr unsigned char startsWith( const StringBase<CharType> &prefix) const;
	constexpr unsigned char startsWith(const StringBase<CharType> &prefix, unsigned int offset) const;
	constexpr unsigned char endsWith(const StringBase<CharType> &suffix) const;

	// character access
	constexpr CharType charAt(unsigned int index) const;
	constexpr void setCharAt(unsigned int index, CharType c);
	constexpr CharType operator [] (unsigned int index) const;
	constexpr CharType& operator [] (unsigned int index);
	constexpr void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index=0) const;
	constexpr void toCharArray(char *buf, unsigned int bufsize, unsigned int index=0) const;
	constexpr const CharType* c_str() const { return m_buffer; }
	constexpr CharType* begin() { return m_buffer; }
    constexpr const CharType* begin() const { return c_str(); }
	constexpr CharType* end() { return m_buffer + len; }
	constexpr const CharType* end() const { return c_str() + length(); }

	// search
    constexpr bool contains(CharType) const;
    constexpr bool contains(const CharType*) const;
    constexpr bool contains(const StringBase<CharType>&) const;
	constexpr int indexOf( CharType ch, unsigned int fromIndex = 0) const;
    constexpr int indexOf(const CharType* c, unsigned int fromIndex = 0) const;
	constexpr int indexOf(const StringBase<CharType> &str, unsigned int fromIndex = 0) const;
	constexpr int lastIndexOf( CharType ch ) const;
	constexpr int lastIndexOf( CharType ch, unsigned int fromIndex ) const;
	constexpr int lastIndexOf( const StringBase<CharType> &str ) const;
	constexpr int lastIndexOf( const StringBase<CharType> &str, unsigned int fromIndex ) const;
	constexpr StringBase<CharType> subString( unsigned int beginIndex ) const { return subString(beginIndex, len); };
	constexpr StringBase<CharType> subString( unsigned int beginIndex, unsigned int endIndex ) const;

	// modification
	constexpr void replace(CharType find, CharType replace);
	constexpr void replace(const StringBase<CharType>& find, const StringBase<CharType>& replace);
	constexpr void remove(unsigned int index);
	constexpr void remove(unsigned int index, unsigned int count);
	constexpr void toLowerCase();
	constexpr void toUpperCase();
	constexpr void trim();
    constexpr void fill(CharType c, unsigned count = 0);

    // This method will only invalidate the contents of the string
    constexpr void erase();
    // This method will reset the string (i.e. shrink the string and invalidate contents) 
    constexpr void reset();
    
	// parsing/conversion
	constexpr long toInt() const;
	constexpr float toFloat() const;
	constexpr double toDouble() const;

protected:
	CharType* m_buffer = static_cast<CharType*>(calloc(31, sizeof(CharType))); // the actual char array
	unsigned int m_capacity{31};  // the array length
	unsigned int len{0};       // the String length
    char dummy_char{0};

	constexpr void invalidate();
	constexpr unsigned char changeBuffer(unsigned int maxStrLen);
	constexpr unsigned char concat(const CharType *cstr, unsigned int length);

	// copy and move
	 constexpr StringBase<CharType> & copy(const CharType *, unsigned int length);
	 constexpr StringBase<CharType> & copy(const __FlashStringHelper *pstr, unsigned int length);
       #if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
	constexpr void move(StringBase<CharType> &&rhs);
	#endif
};

#endif  // __cplusplus

using String = StringBase<char>;

#endif  // String_class_h

