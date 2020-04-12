/*------------------------------------------------------------------------*
 * A GBK string to UTF-8 or to Unicode bidirection coverter.
 * cc936.c file is needed.(which can finded in fatfs library)
 *------------------------------------------------------------------------*/

#include "gbk2utf2uni.h"

extern unsigned short ff_convert (unsigned short chr, unsigned int dir);	/* OEM-Unicode bidirectional conversion */
/**
  * @brief  Convert a GBK character to unicode
  * @param  gb: GBK character
  * @retval unicode character
  */
unsigned short getun(unsigned short gb)
{
  gb = (gb<<8) + (gb>>8); //switch to little endian
  unsigned short res = ff_convert(gb, 1);
  return res;
}

/**
  * @brief  Convert a unicode character to GBK
  * @param  unicode: unicode character
  * @retval GBK character
  */
unsigned short getgb(unsigned short unicode)
{
  unsigned short res = ff_convert(unicode, 0);
  res = (res<<8) + (res>>8); //switch to little endian
  return res;
}

/**
  * @brief  Convert a GBK string to unicode string , unicode string is must big enough to contain the result.
  * @param  * unicode: unicode string pointer for store the result.
  * @param  * gb: GBK string pointer for converting.
  * @retval Number of character is converted.
  */
int gb2unicode(char *unicode,char*gb)
  {
  int j=0;
  while (*gb)
  {
    if (*gb<0x80)
    {
     *(unsigned short *)unicode=*gb;
    gb++;
    }else
    {
     *(unsigned short *)unicode=getun(*(unsigned short *)(gb));
    gb+=2;
    }
    unicode+=2;
    j++;
  }
  return j;
}

/**
  * @brief  Convert a unicode string to unicode string , GBK string is must big enough to contain the result.
  * @param  * gb: GBK string pointer for store the result.
  * @param  * unicode: unicode string pointer for converting.
  * @retval Number of character is converted.
  */
int unicode2gb(char * gb,char * unicode)
{
  int j;
  unsigned short iTmp;
  j=0;
  while (*unicode)
  {
    if (*(unsigned short *)unicode<0x80)
    {
     gb[j]=*(unsigned short *)unicode;
     j++;
    }
    else
    {
     iTmp=getgb(*(unsigned short *)unicode);
     *(unsigned short int*)(gb+j)=iTmp;
     j+=2;
    }
    unicode+=2;
  }
  gb[j]=0;
  return j;
}

/**
  * @brief  Convert a unicode string to UTF-8 string , UTF-8 string is must big enough to contain the result.
  * @param  * unicode: unicode string pointer for converting.
  * @param  * utf: UTF-8 string pointer for store the result.
  * @retval Number of character is converted.
  */
int unicode2utf8( char * utf, char * unicode)
{
  int len = 0; //记录转换后的Utf8字符串的字节数
  while (*unicode)
  {
    //处理一个unicode字符
    char low = *unicode; //取出unicode字符的低8位
    unicode++;
    char high = *unicode; //取出unicode字符的高8位
    unsigned short wchar = (high << 8) + low; //高8位和低8位组成一个unicode字符,加法运算级别高

    if(wchar <= 0x7F) //英文字符
    {
      utf[len] = (char) wchar; //取wchar的低8位
      len++;
    }
    else if(wchar >= 0x80 && wchar <= 0x7FF) //可以转换成双字节pOutput字符
    {
      utf[len] = 0xc0 | ((wchar >> 6) & 0x1f); //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
      len++;
      utf[len] = 0x80 | (wchar & 0x3f); //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
      len++;
    }
    else if(wchar >= 0x800 && wchar < 0xFFFF) //可以转换成3个字节的pOutput字符
    {
      utf[len] = 0xe0 | ((wchar >> 12) & 0x0f); //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
      len++;
      utf[len] = 0x80 | ((wchar >> 6) & 0x3f); //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
      len++;
      utf[len] = 0x80 | (wchar & 0x3f); //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
      len++;
    }

    else //对于其他字节数的unicode字符不进行处理
    {
      return -1;
    }
    unicode++; //处理下一个unicode字符
  }
  //utf8字符串后面，有个\0
  utf[len] = 0;
  return len;
}

/**
  * @brief  Convert a UTF-8 string to unicode string , unicode string is must big enough to contain the result.
  * @param  * utf: UTF-8 string pointer for converting.
  * @param  * unicode: unicode string pointer for store the result.
  * @retval Number of character is converted.
  */
int utf82unicode( char * unicode, char * utf)
{
  int outputSize = 0; //记录转换后的Unicode字符串的字节数

  while (*utf)
  {
    if(*utf > 0x00 && *utf <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
    {
      *unicode = *utf;
      unicode++;
      *unicode = 0; //小端法表示，在高地址填补0
    }
    else if(((*utf) & 0xE0) == 0xC0) //处理双字节UTF8字符
    {
      char high = *utf;
      utf++;
      char low = *utf;
      if((low & 0xC0) != 0x80) //检查是否为合法的UTF8字符表示
      {
        return -1; //如果不是则报错
      }

      *unicode = (high << 6) + (low & 0x3F);
      unicode++;
      *unicode = (high >> 2) & 0x07;
    }
    else if(((*utf) & 0xF0) == 0xE0) //处理三字节UTF8字符
    {
      char high = *utf;
      utf++;
      char middle = *utf;
      utf++;
      char low = *utf;
      if(((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
      {
        return -1;
      }
      *unicode = (middle << 6) + (low & 0x3F); //取出middle的低两位与low的低6位，组合成unicode字符的低8位
      unicode++;
      *unicode = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
    }
    else //对于其他字节数的UTF8字符不进行处理
    {
      return -1;
    }
    utf++; //处理下一个utf8字符
    unicode++;
    outputSize += 2;
  }
  //unicode字符串后面，有两个\0
  *unicode = 0;
  unicode++;
  *unicode = 0;
  return outputSize;
}


/**
  * @brief  Convert a UTF-8 string to GBK string , GBK string is must big enough to contain the result.
  * @param  * utf: UTF-8 string pointer for converting.
  * @param  * GBK: GBK string pointer for store the result.
  * @retval Number of character is converted.
  */
int utf82gbk( char * gb, char * utf)
{
  int outputSize = 0; //记录转换后的Unicode字符串的字节数
  unsigned short unicode;

  while (*utf)
  {
    if(*utf > 0x00 && *utf < 0x80) //处理单字节UTF8字符（英文字母、数字）
    {
      *gb = *utf;
      gb++;
    }
    else if(((*utf) & 0xE0) == 0xC0) //处理双字节UTF8字符
    {
      unsigned short high = *utf;
      utf++;
      unsigned short low = *utf;
      if((low & 0xC0) != 0x80) //检查是否为合法的UTF8字符表示
      {
        return -1; //如果不是则报错
      }
      unicode = ((high& 0x1F) << 6) + (low & 0x3F); //取出high的低5位与low的低6位，组合成unicode字符的低8位
      *(unsigned short *)gb = getgb(unicode); //get gbk from unicode
      gb +=2;
//      *gb = (high << 6) + (low & 0x3F);
//      gb++;
//      *gb = (high >> 2) & 0x07;
    }
    else if(((*utf) & 0xF0) == 0xE0) //处理三字节UTF8字符
    {
      unsigned short high = *utf;
      utf++;
      unsigned short middle = *utf;
      utf++;
      unsigned short low = *utf;
      if(((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
      {
        return -1;
      }
      unicode = ((high & 0x0F) << 12) + ((middle & 0x3F) << 6) + (low & 0x3F); //取出high的低4位，middle的低6位和low的低6位，组合成unicode
      *(unsigned short *)gb = getgb(unicode); //get gbk from unicode
      gb +=2;
    }
    else //对于其他字节数的UTF8字符不进行处理
    {
      return -1;
    }
    utf++; //处理下一个utf8字符
//    gb++;
    outputSize += 2;
  }
  //gbk字符串后面，有1个\0
  *gb = 0;
  gb++;
  return outputSize;
}

/**
  * @brief  Convert a GBK string to UTF-8 string , UTF-8 string is must big enough to contain the result.
  * @param  * GBK: GBK string pointer for converting.
  * @param  * utf: UTF-8 string pointer for store the result.
  * @retval Number of character is converted.
  */
int gb2utf8( char * utf, char * gb)
{
  int len = 0; //记录转换后的Utf8字符串的字节数
  while (*gb)
  {
    //处理一个gbk字符
    if(*gb < 0x80) //英文字符
    {
      utf[len] = *gb; //取wchar的低8位
      gb++;
      len++;
    }
    else
    {
    //处理两个gbk字符
      unsigned short wchar =getun(*(unsigned short *)(gb));
      gb+=2;
        if(wchar >= 0x80 && wchar <= 0x7FF) //可以转换成双字节pOutput字符
      {
        utf[len] = 0xc0 | ((wchar >> 6) & 0x1f); //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
        len++;
        utf[len] = 0x80 | (wchar & 0x3f); //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
        len++;
      }
      else if(wchar >= 0x800 && wchar < 0xFFFF) //可以转换成3个字节的pOutput字符
      {
        utf[len] = 0xe0 | ((wchar >> 12) & 0x0f); //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
        len++;
        utf[len] = 0x80 | ((wchar >> 6) & 0x3f); //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
        len++;
        utf[len] = 0x80 | (wchar & 0x3f); //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
        len++;
      }

      else //对于其他字节数的unicode字符不进行处理
      {
        return -1;
      }
    }
  }
  //utf8字符串后面，有个\0
  utf[len] = 0;
  return len;
}
