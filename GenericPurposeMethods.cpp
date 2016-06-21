// GenericPurposeMethods.cpp: implementation of the GenericPurposeMethods class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NativeDumper.h"
#include "GenericPurposeMethods.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GenericPurposeMethods::GenericPurposeMethods()
{

}

GenericPurposeMethods::~GenericPurposeMethods()
{

}

int GenericPurposeMethods::StringToNumber(char *buffer)
{
   int result = 0;
   int startIndex = 0;
   bool negativeNumber = false;

   if (buffer[0] == '-')
   {
   negativeNumber = true;
   startIndex = 1;
   }

   for (int i = startIndex; i < (int)strlen(buffer); i++)
   {

      if(buffer[i] >= '0' && buffer[i] <= '9')
      {
         int digit = buffer[i] - '0';
         result = result * 10 + digit;
      }
      else
      return 0;

   }

   if (negativeNumber == true)
      result *= -1;

   return result;
}

unsigned int GenericPurposeMethods::ConvertHexStringToInt(CString sHexNum)
{
   unsigned int iSum = 0;
   if (sHexNum.GetLength() == 0) return 0;

   for(int i=sHexNum.GetLength()-1; i >= 0; i--)
   {
	  if (!GenericPurposeMethods::IsHexNumber(sHexNum[i])) return 0;

      if(sHexNum[i])
      {
	  unsigned int current_char = sHexNum[i]-'0';
	  if (current_char>=0x011&&current_char<=0x016)  // is it 'A'- 'F'
	  current_char = current_char - 07;

	  if (current_char>=0x031&&current_char<=0x036)  // is it 'a'- 'f'
	  current_char = current_char - 0x27;

      iSum += current_char*Pow(16,sHexNum.GetLength()-1-i);
      }
   }
   return iSum;

}

bool GenericPurposeMethods::IsHexNumber(char tobetested)
{
if (tobetested >= '0' && tobetested <= '9')
return true;

if (tobetested >= 'A' && tobetested <= 'F')
return true;

if (tobetested >= 'a' && tobetested <= 'f')
return true;

return false;
}

unsigned int GenericPurposeMethods::Pow(int value,int exponent)
{
	unsigned int result = 1;

	for (int i = 0; i < exponent; ++i)
	result *= value;

	return (result);
}

void GenericPurposeMethods::ToUpperCase(char *buffer)
{
   for (int i = 0; i < (int)strlen(buffer); i++)
   {

      if(buffer[i] >= 'a' && buffer[i] <= 'z')
      {
	   buffer[i] -= 'a'-'A';
	  }
   }
}

unsigned int GenericPurposeMethods::UnsignedArrayToUInt(unsigned char *buffer,int position)
{
return ((buffer[position+3]<<24)|(buffer[position+2]<<16)|(buffer[position+1]<<8)|(buffer[position+0]));
}

unsigned int GenericPurposeMethods::UnsignedArrayToShort(unsigned char *buffer,int position)
{
return ((buffer[position+1]<<8)|(buffer[position+0]));
}

unsigned char* GenericPurposeMethods::UIntToUnsignedArray(unsigned int value)
{
unsigned char* array = new unsigned char[4];
array[0] = value & 0x000000ff;
array[1] = (value & 0x0000ff00) >> 8;
array[2] = (value & 0x00ff0000) >> 16;
array[3] = (value & 0xff000000) >> 24;

return array;

}

TCHAR* GenericPurposeMethods::GetShortModuleName(TCHAR fullname[])
{
TCHAR* shortname = new TCHAR[MAX_PATH];
int i=0;
int ostringlen = _tcslen(fullname);
int slash_position = 0;

for (i = ostringlen - 1; i >= 0; --i)
{
if (fullname[i]==92)  // if char == "\"
{
slash_position=i+1;
break;
}

}

for (i=slash_position;i<=ostringlen;i++)  // including null (00) terminating char!
shortname[i-slash_position]=fullname[i];

return shortname;

}

TCHAR* GenericPurposeMethods::GetDirectory(TCHAR fullname[])
{
TCHAR* directory = new TCHAR[MAX_PATH];
int i=0;
int ostringlen = _tcslen(fullname);
int slash_position = 0;

for (i = ostringlen - 1; i >= 0; --i)
{
if (fullname[i]==92)  // if char == "\"
{
slash_position=i+1;
break;
}

}

for (i=0;i<slash_position;i++)  // copy original chars include "\" char
directory[i]=fullname[i];

directory[slash_position] = 00; // null ending char

return directory;

}

TCHAR* GenericPurposeMethods::GetDumpFileName(TCHAR shortname[])
{
TCHAR* dumpname = new TCHAR[MAX_PATH];
int i=0;
int ostringlen = _tcslen(shortname);
int point_position = 0;

for (i = ostringlen - 1; i >= 0; --i)
{
if (shortname[i]==46)  // if char == "."
{
point_position=i;
break;
}

}

for (i=0;i<point_position;i++)  // copy before point (.)
dumpname[i]=shortname[i];

TCHAR* dump_prefix ="_dump";
int prefix_len = _tcslen(dump_prefix);

for (i=0;i<prefix_len;i++)  // copy prefix
dumpname[point_position+i]=dump_prefix[i];

//for (i=point_position;i<ostringlen;i++)  // copy extension
//dumpname[prefix_len+i]=shortname[i];

dumpname[prefix_len+point_position] = 00;  // mark the end of string!

return dumpname;

}

TCHAR* GenericPurposeMethods::GetExtension(TCHAR shortname[])
{
TCHAR* extension = new TCHAR[MAX_PATH];
int i=0;
int ostringlen = _tcslen(shortname);
int point_position = 0;

for (i = ostringlen - 1; i >= 0; --i)
{
if (shortname[i]==46)  // if char == "."
{
point_position=i;
break;
}

}

for (i=point_position;i<ostringlen;i++)  // copy after point (.)
extension[i-point_position]=shortname[i];

extension[ostringlen-point_position] = 00;  // mark the end of string!

return extension;

}

TCHAR* GenericPurposeMethods::JoinChars(TCHAR first[],TCHAR second[])
{
TCHAR* joined = new TCHAR[MAX_PATH];

int firstlen = _tcslen(first);
int secondlen = _tcslen(second);

for (int i=0;i<firstlen;i++)
joined[i]=first[i];

for (int j=0;j<secondlen;j++)
joined[j+firstlen]=second[j];

joined[firstlen+secondlen] = 0; // end of string char

return joined;
}


bool GenericPurposeMethods::ContainsExtension(TCHAR filename[])
{

int i=0;
int stringlen = _tcslen(filename);
if (stringlen==0) return false;
int point_position = -1;

for (i = stringlen - 1; i >= 0; --i)
{
if (filename[i]==46)  // if char == "."
{
point_position=i;
break;
}

}

if (point_position == -1) return false;

int extpos = stringlen-point_position;
if (extpos==4)  // if 3 chars as extension
return true;

if (extpos==3)  // if 2 chars as extension
return true;

if (extpos==2)  // if 1 chars as extension
return true;

return false;
}