// GenericPurposeMethods.h: interface for the GenericPurposeMethods class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICPURPOSEMETHODS_H__DB5CA191_5583_4171_9A54_3B45355468DF__INCLUDED_)
#define AFX_GENERICPURPOSEMETHODS_H__DB5CA191_5583_4171_9A54_3B45355468DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GenericPurposeMethods  
{
public:
	GenericPurposeMethods();
	virtual ~GenericPurposeMethods();
	static unsigned int ConvertHexStringToInt(CString sHexNum);
	static bool IsHexNumber(char tobetested);
	static unsigned int Pow(int value,int exponent);
	static int StringToNumber(char *buffer);
	static void ToUpperCase(char *buffer);
	static unsigned int UnsignedArrayToUInt(unsigned char *buffer,int position);
	static unsigned char* UIntToUnsignedArray(unsigned int value);
	static unsigned int UnsignedArrayToShort(unsigned char *buffer,int position);
	static TCHAR* GetShortModuleName(TCHAR fullname[]);
	static TCHAR* GetDirectory(TCHAR fullname[]);
	static TCHAR* GetDumpFileName(TCHAR shortname[]);
	static TCHAR* GetExtension(TCHAR shortname[]);
	static TCHAR* JoinChars(TCHAR first[],TCHAR second[]);
	static bool ContainsExtension(TCHAR filename[]);


};

#endif // !defined(AFX_GENERICPURPOSEMETHODS_H__DB5CA191_5583_4171_9A54_3B45355468DF__INCLUDED_)
