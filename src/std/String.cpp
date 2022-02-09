/*
  WString.cpp - StringBase<CharType> library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
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
#include "String.hpp"
#include <utility>
#include <pgmspace.h>
/*********************************************/
/*  Constructors                             */
/*********************************************/

template<typename CharType> constexpr StringBase<CharType>::StringBase(const CharType *cstr)
{
	if (!cstr) {
        return;
    }

    copy(cstr, strlen(cstr));
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(const StringBase<CharType> &value)
{
	*this = value;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(const __FlashStringHelper *pstr)
{
	*this = pstr;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
template<typename CharType> constexpr StringBase<CharType>::StringBase(StringBase<CharType> &&rval)
{
    // fixme: moving should avoid calloc-ing the m_buffer member
	move(std::move(rval));
}

#endif

template<typename CharType> constexpr void StringBase<CharType>::fill(CharType c, unsigned count)
{
	if (reserve(count)) {
        len = c;
        memset(m_buffer, c, count); 
        return;
    }
    invalidate();
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(CharType c)
{
	CharType buf[2];
	buf[0] = c;
	buf[1] = 0;
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(unsigned char value, unsigned char base)
{
	CharType buf[1 + 8 * sizeof(unsigned char)];
	utoa(value, buf, base);
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(int value, unsigned char base)
{
	CharType buf[2 + 8 * sizeof(int)];
	itoa(value, buf, base);
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(unsigned int value, unsigned char base)
{
	CharType buf[1 + 8 * sizeof(unsigned int)];
	utoa(value, buf, base);
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(long value, unsigned char base)
{
	CharType buf[2 + 8 * sizeof(long)];
	ltoa(value, buf, base);
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(unsigned long value, unsigned char base)
{
	CharType buf[1 + 8 * sizeof(unsigned long)];
	ultoa(value, buf, base);
	*this = buf;
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(float value, unsigned char decimalPlaces)
{
	CharType buf[33];
	*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

template<typename CharType> constexpr StringBase<CharType>::StringBase(double value, unsigned char decimalPlaces)
{
	CharType buf[33];
	*this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

template<typename CharType> inline constexpr StringBase<CharType>::~StringBase()
{
	if (m_buffer) free(m_buffer);
}

/*********************************************/
/*  Memory Management                        */
/*********************************************/

template<typename CharType> constexpr void StringBase<CharType>::invalidate()
{
	if (m_buffer) free(m_buffer);
	m_buffer = nullptr;
	m_capacity = len = 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::reserve(unsigned int size)
{
	if (m_buffer && m_capacity >= size) return 1;
	if (changeBuffer(size)) {
		if (len == 0) m_buffer[0] = 0;
		return 1;
	}
	return 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::changeBuffer(unsigned int maxStrLen)
{
	if (auto *newbuffer = static_cast<CharType *>(realloc(m_buffer, maxStrLen + 1))) {
		m_buffer = newbuffer;
		m_capacity = maxStrLen;
		return 1;
	}
	return 0;
}

/*********************************************/
/*  Copy and Move                            */
/*********************************************/


template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::copy(const CharType *cstr, unsigned int length)
{
    if (length > m_capacity && !reserve(length)) {
		invalidate();
		return *this;
	}
	len = length;
	strncpy(m_buffer, cstr, length + 1);
	return *this;
}

template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::copy(const __FlashStringHelper *pstr, unsigned int length)
{
	if (length > m_capacity && !reserve(length)) {
		invalidate();
		return *this;
	}
	len = length;
	strcpy_P(m_buffer, pstr);
	return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
template<typename CharType> constexpr void StringBase<CharType>::move(StringBase<CharType> &&rhs)
{
    if (m_buffer) {
        free(m_buffer);
    }

    m_buffer = rhs.m_buffer;
    len = rhs.len;
    m_capacity = rhs.m_capacity;
    
	rhs.m_buffer = nullptr;
	rhs.m_capacity = 0;
	rhs.len = 0;
}
#endif

template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::operator = (const StringBase<CharType> &rhs)
{
	if (this == &rhs) return *this;
	
	if (rhs.m_buffer) copy(rhs.m_buffer, rhs.len);
	else invalidate();
	
	return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::operator = (StringBase<CharType> &&rval)
{
	if (this != &rval) move(rval);
	return *this;
}
#endif

template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::operator = (const CharType *cstr)
{
	if (cstr != nullptr) copy(cstr, strlen(cstr));
	else invalidate();
	
	return *this;
}

template<typename CharType> constexpr StringBase<CharType> & StringBase<CharType>::operator = (const __FlashStringHelper *pstr)
{
	if (pstr) copy(pstr, strlen_P((PGM_P)pstr));
	else invalidate();

	return *this;
}

/*********************************************/
/*  concat                                   */
/*********************************************/

template<typename CharType> inline constexpr unsigned char StringBase<CharType>::concat(const StringBase<CharType> &s)
{
	return concat(s.m_buffer, s.len);
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(const CharType *cstr, unsigned int length)
{
	unsigned int newlen = len + length;
	if (!cstr) return 0;
	if (length == 0) return 1;
	if (newlen > m_capacity && !reserve(newlen)) return 0;
	strncpy(m_buffer + len, cstr, length + 1);
	len = newlen;
	return 1;
}

template<typename CharType> inline constexpr unsigned char StringBase<CharType>::concat(const CharType *cstr)
{
    return cstr ? concat(cstr, strlen(cstr)) : 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(CharType c)
{
	CharType buf[2];
	buf[0] = c;
	buf[1] = 0;
	return concat(buf, 1);
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(unsigned char num)
{
	CharType buf[1 + 3 * sizeof(unsigned char)];
	utoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(int num)
{
	CharType buf[2 + 3 * sizeof(int)];
	itoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(unsigned int num)
{
	CharType buf[1 + 3 * sizeof(unsigned int)];
	utoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(long num)
{
	CharType buf[2 + 3 * sizeof(long)];
	ltoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(unsigned long num)
{
	CharType buf[1 + 3 * sizeof(unsigned long)];
	ultoa(num, buf, 10);
	return concat(buf, strlen(buf));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(float num)
{
	CharType buf[20];
	CharType* string = dtostrf(num, 4, 2, buf);
	return concat(string, strlen(string));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(double num)
{
	CharType buf[20];
	CharType* string = dtostrf(num, 4, 2, buf);
	return concat(string, strlen(string));
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::concat(const __FlashStringHelper * str)
{
	if (!str) return 0;
	int length = strlen_P((const CharType *) str);
	if (length == 0) return 1;
	unsigned int newlen = len + length;
	if (newlen > len && !reserve(newlen)) return 0;
	strcpy_P(m_buffer + len, (const CharType *) str);
	len = newlen;
	return 1;
}

/*********************************************/
/*  Comparison                               */
/*********************************************/

template<typename CharType> constexpr int StringBase<CharType>::compareTo(const StringBase<CharType> &s) const
{
	if (!m_buffer || !s.m_buffer) {
		if (s.m_buffer && s.len > 0) return 0 - *(unsigned char *)s.m_buffer;
		if (m_buffer && len > 0) return *(unsigned char *)m_buffer;
		return 0;
	}
	return strcmp(m_buffer, s.m_buffer);
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::equals(const StringBase<CharType> &s2) const
{
	return (len == s2.len && compareTo(s2) == 0);
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::equals(const CharType *cstr) const
{
	if (len == 0) return (cstr == nullptr || *cstr == 0);
	if (cstr == nullptr) return m_buffer[0] == 0;
	return strcmp(m_buffer, cstr) == 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::operator<(const StringBase<CharType> &rhs) const
{
	return compareTo(rhs) < 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::operator>(const StringBase<CharType> &rhs) const
{
	return compareTo(rhs) > 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::operator<=(const StringBase<CharType> &rhs) const
{
	return compareTo(rhs) <= 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::operator>=(const StringBase<CharType> &rhs) const
{
	return compareTo(rhs) >= 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::equalsIgnoreCase( const StringBase<CharType> &s2 ) const
{
	if (this == &s2) return 1;
	if (len != s2.len) return 0;
	if (len == 0) return 1;
	const CharType *p1 = m_buffer;
	const CharType *p2 = s2.m_buffer;
	while (*p1) {
		if (tolower(*p1++) != tolower(*p2++)) return 0;
	} 
	return 1;
}

template<typename CharType> inline constexpr unsigned char StringBase<CharType>::startsWith( const StringBase<CharType> &s2 ) const
{
	return len < s2.len ? 0 : startsWith(s2, 0);
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::startsWith( const StringBase<CharType> &s2, unsigned int offset ) const
{
	if (offset > len - s2.len || !m_buffer || !s2.m_buffer) return 0;
	return strncmp( &m_buffer[offset], s2.m_buffer, s2.len ) == 0;
}

template<typename CharType> constexpr unsigned char StringBase<CharType>::endsWith( const StringBase<CharType> &s2 ) const
{
	if ( len < s2.len || !m_buffer || !s2.m_buffer) return 0;
	return strcmp(&m_buffer[len - s2.len], s2.m_buffer) == 0;
}

/*********************************************/
/*  Character Access                         */
/*********************************************/

template<typename CharType> inline constexpr CharType StringBase<CharType>::charAt(unsigned int loc) const
{
	return operator[](loc);
}

template<typename CharType> constexpr void StringBase<CharType>::setCharAt(unsigned int loc, CharType c) 
{
	if (loc < len) m_buffer[loc] = c;
}

template<typename CharType> constexpr CharType & StringBase<CharType>::operator[](unsigned int index)
{
	if (index >= len || !m_buffer) {
        dummy_char = 0;
		return dummy_char;
	}
	return m_buffer[index];
}

template<typename CharType> constexpr CharType StringBase<CharType>::operator[]( unsigned int index ) const
{
	return index >= len || !m_buffer ? 0 : m_buffer[index];
}

template<typename CharType> constexpr void StringBase<CharType>::toCharArray(char *buf, unsigned int bufsize, unsigned int index) const {
    if (!bufsize || !buf) return;
	if (index >= len) {
		buf[0] = 0;
		return;
	}
	unsigned int n = bufsize - 1;
	if (n > len - index) n = len - index;
	strncpy(buf, m_buffer + index, n);
	buf[n] = 0;
}

template<typename CharType> inline constexpr void StringBase<CharType>::getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index) const
{
	return toCharArray(static_cast<char*>(static_cast<void*>(buf)), bufsize, index);
}

/*********************************************/
/*  Search                                   */
/*********************************************/
template<typename CharType> inline constexpr bool StringBase<CharType>::contains(CharType c) const
{
	return indexOf(c, 0) != -1;
}

template<typename CharType> inline constexpr bool StringBase<CharType>::contains(const CharType* c) const
{
	return indexOf(c, 0) != -1;
}

template<typename CharType> inline constexpr bool StringBase<CharType>::contains(const StringBase<CharType> &str) const
{
	return indexOf(str, 0) != -1;
}

template<typename CharType> constexpr int StringBase<CharType>::indexOf(CharType ch, unsigned int fromIndex) const
{
	if (fromIndex >= len) return -1;
	const CharType* temp = strchr(m_buffer + fromIndex, ch);
	if (temp == nullptr) return -1;
	return temp - m_buffer;
}

template<typename CharType> constexpr int StringBase<CharType>::indexOf(const CharType* c, unsigned int fromIndex) const
{
	if (fromIndex >= len) return -1;
	const CharType *found = strstr(m_buffer + fromIndex, m_buffer);
	if (found == nullptr) return -1;
	return found - m_buffer;
}

template<typename CharType> inline constexpr int StringBase<CharType>::indexOf(const StringBase<CharType> &s2, unsigned int fromIndex) const
{
	return indexOf(s2.m_buffer, fromIndex);
}

template<typename CharType> inline constexpr int StringBase<CharType>::lastIndexOf( CharType theChar ) const
{
	return lastIndexOf(theChar, len - 1);
}

template<typename CharType> constexpr int StringBase<CharType>::lastIndexOf(CharType ch, unsigned int fromIndex) const
{
	if (fromIndex >= len) return -1;
	CharType tempchar = m_buffer[fromIndex + 1];
	m_buffer[fromIndex + 1] = '\0';
	CharType* temp = strrchr( m_buffer, ch );
	m_buffer[fromIndex + 1] = tempchar;
	if (temp == nullptr) return -1;
	return temp - m_buffer;
}

template<typename CharType> inline constexpr int StringBase<CharType>::lastIndexOf(const StringBase<CharType> &s2) const
{
	return lastIndexOf(s2, len - s2.len);
}

template<typename CharType> constexpr int StringBase<CharType>::lastIndexOf(const StringBase<CharType> &s2, unsigned int fromIndex) const
{
  	if (s2.len == 0 || len == 0 || s2.len > len) return -1;
	if (fromIndex >= len) fromIndex = len - 1;
	int found = -1;
	for (CharType *p = m_buffer; p <= m_buffer + fromIndex; p++) {
		p = strstr(p, s2.m_buffer);
		if (!p) break;
		if ((unsigned int)(p - m_buffer) <= fromIndex) found = p - m_buffer;
	}
	return found;
}

template<typename CharType> constexpr StringBase<CharType> StringBase<CharType>::subString(unsigned int left, unsigned int right) const
{
	if (left > right) {
		unsigned int temp = right;
		right = left;
		left = temp;
	}
	StringBase<CharType> out;
	if (left >= len) return out;
	if (right > len) right = len;
	CharType temp = m_buffer[right];  // save the replaced character
	m_buffer[right] = '\0';	
	out = m_buffer + left;  // pointer arithmetic
	m_buffer[right] = temp;  //restore character
	return out;
}

/*********************************************/
/*  Modification                             */
/*********************************************/

template<typename CharType> constexpr void StringBase<CharType>::replace(CharType find, CharType replace)
{
	if (!m_buffer) return;
    
    for (auto& p : *this) {
        if (p == find) p = replace;
    }
}

template<typename CharType> constexpr void StringBase<CharType>::replace(const StringBase<CharType>& find, const StringBase<CharType>& replace)
{
	if (len == 0 || find.len == 0) return;
	int diff = replace.len - find.len;
	CharType *readFrom = m_buffer;
	CharType *foundAt;
	if (diff == 0) {
		while ((foundAt = strstr(readFrom, find.m_buffer)) != nullptr) {
			memcpy(foundAt, replace.m_buffer, replace.len);
			readFrom = foundAt + replace.len;
		}
	} else if (diff < 0) {
		CharType *writeTo = m_buffer;
		while ((foundAt = strstr(readFrom, find.m_buffer)) != nullptr) {
			unsigned int n = foundAt - readFrom;
			memcpy(writeTo, readFrom, n);
			writeTo += n;
			memcpy(writeTo, replace.m_buffer, replace.len);
			writeTo += replace.len;
			readFrom = foundAt + find.len;
			len += diff;
		}
		strcpy(writeTo, readFrom);
	} else {
		unsigned int size = len; // compute size needed for result
		while ((foundAt = strstr(readFrom, find.m_buffer)) != nullptr) {
			readFrom = foundAt + find.len;
			size += diff;
		}
		if (size == len) return;
		if (size > m_capacity && !changeBuffer(size)) return; // XXX: tell user!
		int index = len - 1;
		while (index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
			readFrom = m_buffer + index + find.len;
			memmove(readFrom + diff, readFrom, len - (readFrom - m_buffer));
			len += diff;
			m_buffer[len] = 0;
			memcpy(m_buffer + index, replace.m_buffer, replace.len);
			index--;
		}
	}
}

template<typename CharType> constexpr void StringBase<CharType>::remove(unsigned int index){
	// Pass the biggest integer as the count. The remove method
	// below will take care of truncating it at the end of the
	// string.
	remove(index, (unsigned int)-1);
}

template<typename CharType> constexpr void StringBase<CharType>::remove(unsigned int index, unsigned int count){
	if (index >= len) { return; }
	if (count <= 0) { return; }
	if (count > len - index) { count = len - index; }
	CharType *writeTo = m_buffer + index;
	len = len - count;
	strncpy(writeTo, m_buffer + index + count,len - index);
	m_buffer[len] = 0;
}

template<typename CharType> constexpr void StringBase<CharType>::toLowerCase()
{
	if (!m_buffer) return;
	for (auto& p : *this) {
        p = tolower(p);
    }
}

template<typename CharType> constexpr void StringBase<CharType>::toUpperCase()
{
	if (!m_buffer) return;
    for (auto& p : *this) {
        p = toupper(p);
    }
}

template<typename CharType> constexpr void StringBase<CharType>::trim()
{
	if (!m_buffer || len == 0) return;
	CharType *begin = m_buffer;
	while (isspace(*begin)) begin++;
	CharType *end = m_buffer + len - 1;
	while (isspace(*end) && end >= begin) end--;
	len = end + 1 - begin;
	if (begin > m_buffer) memcpy(m_buffer, begin, len);
	m_buffer[len] = 0;
}

template<typename CharType> constexpr void StringBase<CharType>::erase() {
    if (!m_buffer || len == 0) return;
    m_buffer[0] = '\0';
    len = 0;
}

template<typename CharType> constexpr void StringBase<CharType>::reset() {
    erase();
    changeBuffer(30); // 30 since it will add 1 for the terminator
}

/*********************************************/
/*  Parsing / Conversion                     */
/*********************************************/

template<typename CharType> inline constexpr long StringBase<CharType>::toInt() const
{
	if (m_buffer) return atol(m_buffer);
	return 0;
}

template<typename CharType> inline constexpr float StringBase<CharType>::toFloat() const
{
	return float(toDouble());
}

template<typename CharType> inline constexpr double StringBase<CharType>::toDouble() const
{
	if (m_buffer) return atof(m_buffer);
	return 0;
}