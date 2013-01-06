//============================================================================
//============================================================================
//
// Financial Applications of Excel Add-in Development in C/C++ (2nd edition)
// 
// Author: Steve Dalton
// 
// Published by John Wiley & Sons Ltd, The Atrium, Southern Gate, Chichester,
// West Sussex, PO19 8SQ, UK.
// 
// All intellectual property rights and copyright in the code listed in this
// module are reserved.  You may reproduce and use this code only as permitted
// in the Rules of Use that you agreed to on installation.  To use the material
// in this source module, or material derived from it, in any other way you
// must first obtain written permission.  Requests for permission can be sent
// by email to permissions@eigensys.com.
// 
// No warranty, explicit or implied, is made by either the author or publisher
// as to the quality, fitness for a particular purpose, accuracy or
// appropriateness of material in this module.  The code and methods contained
// are intended soley for example and clarification.  You should not rely on
// any part of this code without having completely satisfied yourself that it
// is correct and appropriate for your needs.
//
//============================================================================
//============================================================================
// This source file contains examples functions that manipulate text strings.
//============================================================================
//============================================================================

#include <windows.h>

#include "xlw/mgw/cpp_xloper.h"
#pragma warning (disable : 4996)
#pragma warning (disable : 4267)

bool to_lower(char *text);

// Simple example string class to simplify handling of strings used
// with Excel C API (Excel4 and Excel12).  Represents strings
// internally as length counted (prepended) byte ASCII strings
// and/or wide character unsicode strings and converts where
// necessary depending on the use being made of the class instance.

// Where the class is being used to create strings for inclusion
// in an xloper or xloper12, the xloper's destructor will free
// the string, so the xlString's destructor, which might get called
// first, has to be prevented from doing this.  The methods
// ExtractStr() and ExtractWcStr() return pointers to the memory and
// 
#define USE_xlString_CLASS	0 // Not used in this project

#if USE_xlString_CLASS

class xlString
{
public:
	xlString() {m_zStr = NULL; m_zwcStr = NULL;}

	xlString(xloper *p_op)
	{
		if(!coerce_to_string(p_op, m_zStr))
			m_zStr = NULL;
		m_zwcStr = NULL;
	}
	~xlString()
	{
		if(m_zStr)
		{
			free(m_zStr);
			m_zStr = NULL;
		}
		if(m_zwcStr)
		{
			free(m_zwcStr);
			m_zwcStr = NULL;
		}
	}

	xlString(char *p_zStr) // Initialise to ASCII byte string
	{
		int len = strlen(p_zStr);
		if(len)
		{

		}
	}

	void operator=(xloper *p_op)
	{
		if(m_zStr)
			free(m_zStr);

		if(!coerce_to_string(p_op, m_zStr))
			m_zStr = NULL;
	}

	char *zStr(void) const {return m_zStr;}

	char *zStr(const xloper *p_op)
	{
		free(m_zStr);

		if(!coerce_to_string(p_op, m_zStr))
			m_zStr = NULL;

		return m_zStr;
	}
	bool StartsWith(char ch) {return *m_zStr == ch;}
	bool Valid(void) {return m_zStr != NULL;}
	void ToLower(void) {to_lower(m_zStr);}
	bool Is(const char *s) {return strcmp(m_zStr, s) == 0;}

private:
	char *m_zStr;
	wchar_t *m_zwcStr;
};
#endif // USE_xlString_CLASS

//===================================================================
//===================================================================
//===================================================================
char *deep_copy_xlstring(const char *xlstring)
{
	if(!xlstring)
		return NULL;

	size_t len = (BYTE)xlstring[0] + 2;
	char *p = (char *)malloc(len);
	memcpy(p, xlstring, len); // copies the char count, string and null termination
	return p;
}
//-------------------------------------------------------------------
// Deep copy and cast down from Unicode to ASCII byte string
char *deep_copy_xlstring(const wchar_t *xl12string)
{
	if(!xl12string)
		return NULL;

	size_t len = xl12string[0];
	char *p = (char *)malloc(len + 2);
	if(!p || (len = wcstombs(p + 1, xl12string + 1, xl12string[0])) < 0)
	{
		free(p);
		return NULL;
	}
	p[0] = len; // string p[1] is NOT necessarily null terminated
	p[len + 1] = 0; // now it is
	return p;
}
//-------------------------------------------------------------------
// Deep copy and cast up from ASCII to Unicode string
wchar_t *deep_copy_xl12string(const char *xlstring)
{
	if(!xlstring)
		return NULL;

	size_t len, mem_size = ((BYTE)xlstring[0] + 2) * sizeof(wchar_t);
	wchar_t *p = (wchar_t *)malloc(mem_size);
	if((len = mbstowcs(p + 1, xlstring + 1, (BYTE)xlstring[0])) == -1)
	{
		free(p);
		return NULL;
	}
	p[0] = len; // string p[1] is NOT necessarily null terminated
	p[len + 1] = 0; // now it is
	return p;
}
//-------------------------------------------------------------------
wchar_t *deep_copy_xl12string(const wchar_t *xl12string)
{
	if(!xl12string)
		return NULL;

	size_t mem_size = (xl12string[0] + 2) * sizeof(wchar_t);
	wchar_t *p = (wchar_t *)malloc(mem_size);
	memcpy(p, xl12string, mem_size); // copies the char count, string and null termination
	return p;
}
//-------------------------------------------------------------------
// Create counted ASCII byte string from null-terminated ASCII input
char *new_xlstring(const char *text)
{
	size_t len;

	if(!text || !(len = strlen(text)))
		return NULL;

	if(len > MAX_XL4_STR_LEN)
		len = MAX_XL4_STR_LEN; // truncate

	char *p = (char *)malloc(len + 2);
	if(!p) return NULL;
	memcpy(p + 1, text, len + 1);
	p[0] = (BYTE)len;
	return p;
}
//-------------------------------------------------------------------
// Create counted ASCII byte string from null-terminated Unicode input
char *new_xlstring(const wchar_t *text)
{
	size_t len;

	if(!text || !(len = wcslen(text)))
		return NULL;

	if(len > MAX_XL4_STR_LEN)
		len = MAX_XL4_STR_LEN; // truncate

	char *p = (char *)malloc(len + 2);
	if(!p || wcstombs(p + 1, text, len) < 0)
	{
		free(p);
		return NULL;
	}
	p[0] = (BYTE)len;
	p[len + 1] = 0;
	return p;
}
//-------------------------------------------------------------------
// Create counted Unicode wchar string from null-terminated ASCII input
wchar_t *new_xl12string(const char *text)
{
	size_t len;

	if(!text || !(len = strlen(text)))
		return NULL;

	if(len > MAX_XL12_STR_LEN)
		len = MAX_XL12_STR_LEN; // truncate

	wchar_t *p = (wchar_t *)malloc((len + 2) * sizeof(wchar_t));
	if(!p) return NULL;
	mbstowcs(p + 1, text, len);
	p[0] = len; // string p[1] is NOT null terminated
	p[len + 1] = 0; // now it is
	return p;
}
//-------------------------------------------------------------------
// Create counted Unicode wchar string from null-terminated Unicode input
wchar_t *new_xl12string(const wchar_t *text)
{
	size_t len;

	if(!text || !(len = wcslen(text)))
		return NULL;

	if(len > MAX_XL12_STR_LEN)
		len = MAX_XL12_STR_LEN; // truncate

	wchar_t *p = (wchar_t *)malloc((len + 2) * sizeof(wchar_t));
	if(!p) return NULL;
	memcpy(p + 1, text, (len + 1) * sizeof(wchar_t));
	p[0] = len;
	return p;
}
//-------------------------------------------------------------------
// Returns a new xlstring which the caller must free.  Does not
// free the original string.
//-------------------------------------------------------------------
char *append_xlstring(const char *xlstring, const char *new_text)
{
	size_t new_len = strlen(new_text), old_len = (BYTE)xlstring[0], len = old_len + new_len;

	if(!new_text || !new_len || old_len == MAX_XL4_STR_LEN)
		return NULL;

	if(len > MAX_XL4_STR_LEN)
	{
		len = MAX_XL4_STR_LEN;
		new_len = old_len - len; // prevents buffer overrun
	}

	char *new_buffer = (char *)malloc(len + 2);

	if(!new_buffer)
		return NULL; // No effect

	strncpy(new_buffer + 1, xlstring + 1, old_len);
	strncpy(new_buffer + old_len + 1, new_text, new_len);
	new_buffer[0] = (BYTE)len;
	new_buffer[len + 1] = 0;
	return new_buffer;
}
//-------------------------------------------------------------------
char *append_xlstring(const char *xlstring, const wchar_t *new_text)
{
	size_t new_len = wcslen(new_text), old_len = (BYTE)xlstring[0], len = old_len + new_len;

	if(!new_text || !new_len || old_len == MAX_XL4_STR_LEN)
		return NULL;

	if(len > MAX_XL4_STR_LEN)
	{
		len = MAX_XL4_STR_LEN;
		new_len = old_len - len; // prevents buffer overrun
	}

	char *new_buffer = (char *)malloc(len + 2);
	char *temp_buffer = (char *)malloc(new_len);

	if(!new_buffer || !temp_buffer)
	{
		free(new_buffer);
		free(temp_buffer);
		return NULL; // No effect
	}
// Convert from wchar_t string to multi-byte string
	wcstombs(temp_buffer, new_text, new_len);

	strncpy(new_buffer + 1, xlstring + 1, old_len);
	strncpy(new_buffer + old_len + 1, temp_buffer, new_len);
	new_buffer[0] = len;
	new_buffer[len + 1] = 0;
	free(temp_buffer);
	return new_buffer;
}
//-------------------------------------------------------------------
wchar_t *append_xl12string(const wchar_t *xl12string, const wchar_t *new_text)
{
	size_t new_len = wcslen(new_text), old_len = xl12string[0], len = old_len + new_len;

	if(!new_text || !new_len || old_len == MAX_XL12_STR_LEN)
		return NULL;

	if(len > MAX_XL12_STR_LEN)
	{
		len = MAX_XL12_STR_LEN;
		new_len = old_len - len; // prevents buffer overrun
	}

	wchar_t *new_buffer = (wchar_t *)malloc((len + 2) * sizeof(wchar_t));

	if(!new_buffer)
		return NULL; // No effect

	wcsncpy(new_buffer + 1, xl12string + 1, old_len);
	wcsncpy(new_buffer + old_len + 1, new_text, new_len);
	new_buffer[0] = len;
	new_buffer[len + 1] = 0;
	return new_buffer;
}
//-------------------------------------------------------------------
wchar_t *append_xl12string(const wchar_t *xl12string, const char *new_text)
{
	size_t new_len = strlen(new_text), old_len = xl12string[0], len = old_len + new_len;

	if(!new_text || !new_len || old_len == MAX_XL12_STR_LEN)
		return NULL;

	if(len > MAX_XL12_STR_LEN)
	{
		len = MAX_XL12_STR_LEN;
		new_len = old_len - len; // prevents buffer overrun
	}

	wchar_t *new_buffer = (wchar_t *)malloc((len + 2) * sizeof(wchar_t));
	wchar_t *temp_buffer = (wchar_t *)malloc(new_len * sizeof(wchar_t));

	if(!new_buffer || !temp_buffer)
	{
		free(new_buffer);
		free(temp_buffer);
		return NULL; // No effect
	}
// Convert to wchar_t string from multi-byte string
	mbstowcs(temp_buffer, new_text, new_len);

	wcsncpy(new_buffer + 1, xl12string + 1, old_len);
	wcsncpy(new_buffer + old_len + 1, temp_buffer, new_len);
	new_buffer[0] = len;
	new_buffer[len + 1] = 0;
	free(temp_buffer);
	return new_buffer;
}
//-------------------------------------------------------------------
wchar_t *concat_xlstrings(const wchar_t *xlstring1, const wchar_t *xlstring2)
{
	if(!xlstring1 || !xlstring2)
		return NULL;

	size_t len1 = xlstring1[0], len2 = xlstring2[0];

	if(len1 > MAX_XL12_STR_LEN)
	{
		len1 = MAX_XL12_STR_LEN;
		len2 = 0;
	}

	int new_len = len1 + len2;

	if(new_len > MAX_XL12_STR_LEN)
	{
		len2 = MAX_XL12_STR_LEN - len1;
		new_len = MAX_XL12_STR_LEN;
	}
	wchar_t *new_buffer = (wchar_t *)malloc((new_len + 2) * sizeof(wchar_t));

	if(!new_buffer)
		return NULL; // No effect

	wcsncpy(new_buffer + 1, xlstring1 + 1, len1);
	if(len2) wcsncpy(new_buffer + 1 + len1, xlstring2 + 1, len2);
	new_buffer[0] = new_len;
	new_buffer[new_len + 1] = 0;
	return new_buffer;
}
//-------------------------------------------------------------------
char *concat_xlstrings(const char *xlstring1, const char *xlstring2)
{
	if(!xlstring1 || !xlstring2)
		return NULL;

	size_t len1 = (BYTE)xlstring1[0], len2 = (BYTE)xlstring2[0];

	if(len1 > MAX_XL4_STR_LEN)
	{
		len1 = MAX_XL4_STR_LEN;
		len2 = 0;
	}

	int new_len = len1 + len2;

	if(new_len > MAX_XL4_STR_LEN)
	{
		len2 = MAX_XL4_STR_LEN - len1;
		new_len = MAX_XL4_STR_LEN;
	}
	char *new_buffer = (char *)malloc(new_len + 2);

	if(!new_buffer)
		return NULL; // No effect

	strncpy(new_buffer + 1, xlstring1 + 1, len1);
	if(len2) strncpy(new_buffer + 1 + len1, xlstring2 + 1, len2);
	new_buffer[0] = new_len;
	new_buffer[new_len + 1] = 0;
	return new_buffer;
}
//===================================================================
size_t count_char(char *text, char ch)
{
	if(!text || !ch)
		return 0;
	size_t count;
	for(count = 0; *text; )
		if(*text++ == ch)
			count++;
	return count;
}
//===================================================================
size_t count_char(wchar_t *text, wchar_t ch)
{
	if(!text || !ch)
		return 0;
	size_t count;
	for(count = 0; *text; )
		if(*text++ == ch)
			count++;
	return count;
}
//========================================================
size_t count_chars(char *text, char *chars)
{
	if(!text || !chars)
		return 0;

	short count = 0;
	char *p;

	do
	{
		for(p = chars; *p;)
		{
			if(*text == *p++)
			{
				count++;
				break;
			}
		}
	}
	while(*(++text));

	return count;
}
//========================================================
void replace_mask(char *text, char *old_chars, char *new_chars)
{
	if(!text || !old_chars)
		return;

	char *p_old, *p, *pt;

	if(!new_chars)
	{
// Remove all occurances of all characters in old_chars
		for(p_old = old_chars; *p_old; p_old++)
		{
			for(pt = text; *pt;)
			{
				if(*pt == *p_old)
				{
					p = pt;
					do {*p = p[1];} while (*(++p));
				}
				else
					pt++;
			}
		}
		return;
	}

// Substitute all occurances of old chars with corresponding new
	if(strlen(old_chars) != strlen(new_chars))
		return;

	char *p_new;

	for(p = text; *p; p++)
	{
		p_old = old_chars;
		p_new = new_chars;

		for(; *p_old; p_old++, p_new++)
		{
			if(*p == *p_old)
			{
				*p = *p_new;
				break;
			}
		}
	}
}
//========================================================
void replace_mask(wchar_t *text, wchar_t *old_chars, wchar_t *new_chars)
{
	if(!text || !old_chars)
		return;

	wchar_t *p_old, *p, *pt;

	if(!new_chars)
	{
// Remove all occurances of all characters in old_chars
		for(p_old = old_chars; *p_old; p_old++)
		{
			for(pt = text; *pt;)
			{
				if(*pt == *p_old)
				{
					p = pt;
					do {*p = p[1];} while (*(++p));
				}
				else
					pt++;
			}
		}
		return;
	}

// Substitute all occurances of old chars with corresponding new
	if(wcslen(old_chars) != wcslen(new_chars))
		return;

	wchar_t *p_new;

	for(p = text; *p; p++)
	{
		p_old = old_chars;
		p_new = new_chars;

		for(; *p_old; p_old++, p_new++)
		{
			if(*p == *p_old)
			{
				*p = *p_new;
				break;
			}
		}
	}
}
//=================================================================
// Find the first char in text that is also in mask
char *strfirst(char *text, char *mask)
{
	for(char *p; *text; text++)
		for(p = mask; *p; p++)
			if(*p == *text)
				return text;

	return NULL;
}
//========================================================
size_t find_first(char *text, char *search_text)
{
	if(!text || !search_text) return 0;
	char *p = strpbrk(text, search_text);
	return p ? 1 + p - text : 0;
}
//========================================================
size_t find_first(wchar_t *text, wchar_t *search_text)
{
	if(!text || !search_text) return 0;
	wchar_t *p = wcspbrk(text, search_text);
	return p ? 1 + p - text : 0;
}
//========================================================
// Returns the position of the first character (counting from 1) in
// text that does not belong to the set of characters in search_text.
// Returns 0 if text consists entirely of characters from string2 or if
// there was an error.
size_t find_first_excluded(char *text, char *search_text)
{
	if(!text || !search_text)
		return 0;

	for(char *t = text; *t; t++)
		if(!strchr(search_text, *t)) // *t not in search_text: return posn
			return 1 + t - text;

	return 0; // all of text chars are in search_text (but not vice versa)
}
//========================================================
size_t find_first_excluded(wchar_t *text, wchar_t *search_text)
{
	if(!text || !search_text)
		return 0;

	for(wchar_t *t = text; *t; t++)
		if(!wcschr(search_text, *t)) // *t not in search_text: return posn
			return 1 + t - text;

	return 0; // all of text chars are in search_text (but not vice versa)
}
//========================================================
// Returns pointer to the first character in
// text that does not belong to the set of characters in search_text.
// Returns NULL if text consists entirely of characters from string2 or if
// there was an error.
char * strspnp(char *text, char *search_text)
{
	if(!text || !search_text)
		return NULL;

	for(char *t = text; *t; t++)
		if(!strchr(search_text, *t)) // then *t was not in search_text, so return its position
			return t;

	return NULL; // all of text chars are in search_text (but not necessarily vice versa)
}
//========================================================
size_t find_last(char *text, char ch)
{
	if(!text || !ch) return 0;
	char *p = strrchr(text, ch);
	return p ? 1 + p - text : 0;
}
//========================================================
size_t find_last(wchar_t *text, wchar_t ch)
{
	if(!text || !ch) return 0;
	wchar_t *p = wcsrchr(text, ch);
	return p ? 1 + p - text : 0;
}
//========================================================
int compare_text(char *a, char *b, bool case_sensitive)
{
	if(!a || !b)
		return -2; // str*cmp functions return <0, 0, >0

	return case_sensitive ? strcmp(a, b) : stricmp(a, b);
}
//========================================================
int compare_text(wchar_t *a, wchar_t *b, bool case_sensitive)
{
	if(!a || !b)
		return -2; // str*cmp functions return <0, 0, >0

	return case_sensitive ? wcscmp(a, b) : wcsicmp(a, b);
}
//========================================================
int compare_nchars(char *a, char *b, size_t n_chars, bool case_sensitive)
{
	if(!a || !b || !n_chars || n_chars > MAX_XL4_STR_LEN)
		return -2;

	return case_sensitive ? strncmp(a, b, n_chars) : strnicmp(a, b, n_chars);
}
//========================================================
int compare_nchars(wchar_t *a, wchar_t *b, size_t n_chars, bool case_sensitive)
{
	if(!a || !b || !n_chars || n_chars > MAX_XL12_STR_LEN)
		return -2;

	return case_sensitive ? wcsncmp(a, b, n_chars) : wcsnicmp(a, b, n_chars);
}
//====================================================================
// Core code function written in terms of cpp_xlopers to make it
// version-independent.  cpp_xloper class is version-aware and
// uses either xlopers or xloper12s depending on the running
// version.
//------------------------------------------------------------
bool concat_xl(cpp_xloper &RetVal, const cpp_xloper &Inputs, const cpp_xloper &Delim,
	const cpp_xloper &MaxLen, const cpp_xloper &NumDecs, const cpp_xloper &NumScaling)
{
	if(Inputs.IsType(xltypeMissing | xltypeNil))
	{
		RetVal.SetToError(xlerrValue);
		return false;
	}

	char delim_str[2] = {Delim.IsStr() ? (char)Delim.First() : ',', 0};
	int num_decs = NumDecs.IsNum() ? (int)NumDecs : -1;
	size_t max_len = MAX_XL12_STR_LEN;
	
	if(MaxLen.IsNum())
		max_len = (size_t)(int)MaxLen;

	if(max_len > (gExcelVersion12plus ? MAX_XL12_STR_LEN : MAX_XL4_STR_LEN))
		max_len = (gExcelVersion12plus ? MAX_XL12_STR_LEN : MAX_XL4_STR_LEN);

	DWORD size;
	Inputs.GetArraySize(size);
	bool scaling = NumScaling.IsNum();
	double scale = scaling ? (double)NumScaling : 0.0;
	cpp_xloper Op;

	for(DWORD i = 0; i < size; i++)
	{
		if(i)
			RetVal += delim_str;

		Inputs.GetArrayElt(i, Op);

		if(num_decs >= 0 && Op.IsNum())
		{
			Op.Excel(xlfRound, 2, &Op, &NumDecs);
			if(scaling)
				Op *= scale;
		}

		if(i == 0)
		{
			RetVal = Op;
			RetVal.ConvertToString();
		}	
		else
			RetVal += Op; // RetVal is a string, so += concatenates

		if(RetVal.Len() >= max_len)
			break;
	}
	return true;
}
//====================================================================
bool parse_xl(cpp_xloper &RetVal, const cpp_xloper &Input, const cpp_xloper &Delim,
	const cpp_xloper &Numeric, const cpp_xloper &Empty, const cpp_xloper &NumScaling)
{
	if(!Input.IsStr())
	{
		RetVal.SetToError(xlerrValue);
		return false;
	}
	cpp_xloper Caller;
	Caller.Excel(xlfCaller);

// Get the caller's size and shape
	RW c_rows;
	COL c_cols;
	if(!Caller.GetRangeSize(c_rows, c_cols)) // Checks that type is Sref, Ref
		return NULL; // return NULL in case was not called by Excel

	DWORD num_calling_cells = c_rows * c_cols;
	wchar_t delimiter = Delim.IsStr() ? Delim.First() : L',';
	wchar_t *input_copy = (wchar_t *)Input; // Work with Unicode strings
	wchar_t *p_last = input_copy, *p;
	DWORD count = 1;

	for(p = input_copy; *p;)
		if(*p++ == delimiter)
			++count;

	RetVal.SetTypeMulti(c_rows, c_cols); // Same shape as caller
// CLIB strtok ignores empty fields, so must do our own tokenizing
	DWORD i = 0;
	bool numeric = Numeric.IsTrue();
	bool have_empty_val = // single value types only
		Empty.IsType(xltypeNum | xltypeStr | xltypeErr | xltypeBool);
	bool scaling = NumScaling.IsNum();
	double scale = scaling ? (double)NumScaling : 0.0;

// Fill the target range in row-by-row
	if(count > num_calling_cells) // Need to avoid overwriting array bounds
		count = num_calling_cells;

	while(i < count)
	{
		if((p = wcschr(p_last, (int)delimiter)))
			*p = 0;

		if((!p && *p_last) || p > p_last)
		{
			if(numeric)
			{
// Need to convert p_last to a byte-string to convert to a double
// as there is no wchar equivalent of atof
				char mbstr[100];
				wcstombs(mbstr, p_last, 100);
				mbstr[99] = 0;
				RetVal.SetArrayElt(i, atof(mbstr) * (scaling ? scale : 1.0));
			}
			else
				RetVal.SetArrayElt(i, p_last);
		}
		else if(have_empty_val)
		{
			RetVal.SetArrayElt(i, Empty);
		}
		i++;
		if(!p) 	break;
		p_last = p + 1;
	}

// If there's space at the end of the calling range, fill with empty value
	if(have_empty_val)
		for(; i < num_calling_cells; i++)
			RetVal.SetArrayElt(i, Empty);

	free(input_copy);
	return true;
}
//====================================================================
