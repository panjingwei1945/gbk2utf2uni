# gbk2utf2uni

Threeway converter string between simplified Chinese GBK, UTF-8, and Unicode for an embed system (stm32, x86, or any other little-endian system).

 It also suits for threeway string converting for BIG5, Japanese Shift-JIS or Korean EUC-KR to UTF-8/unicode.

## Important facts:
* This library didn't need any other library.
* Only suit for **little-endian system**. For example x86, stm32.
* This library cost about **180KB ROM space**.(Lookup table is supper large.)
* This library only suit for characters whose Unicode index smaller than 0xFFFF. Which means it suits for almost every character.
* This library didn't need malloc or any other dynamic memory. (Suit for embed system.)
*  This library is super fast. Searching lookup table only needs 16 comparisons at most. (Normally this kind of search need about 10000 comparisons on average.)
* I only test this library for GBK converting on stm32f4 system.
* The Lookup table is modified from fatfs library. And converter part is enlightened from:
 [不依赖任何系统API,用c语言实现gbk/utf8/unicode编码转换](https://blog.csdn.net/bladeandmaster88/article/details/54837338 "不依赖任何系统API,用c语言实现gbk/utf8/unicode编码转换") 
and
 [GB2312转unicode程序](https://www.cnblogs.com/flying_bat/archive/2008/04/11/1148042.html "GB2312转unicode程序")


## How to use this library.
* Add **gbk2utf2uni.h**, **gbk2utf2uni.c** and  into your project.

* **GBK.c (optional)**: This file provides a Lookup table. If you use fatfs library and there are already **cc936.c** file in your project, you don't need this file at all.

> Either GBK.c or cc932.c may cost about **180KB ROM** space.(Not suit for low-cost MCU.)

* File names of the lookup table for other language, and corresponding file in FatFs library.

Lookup table        | gbk2utf2uni | fatfs
--------------------|-------------|-------
simple chinese      | GBK.c       | cc936.c
traditional chinese | Big5.c      | cc950.c
japanese            | Shift-JIS.c | cc932.c
korean              | EUC-KR.c    | cc949.c

* Then convert string like this:

```c
char tmpGBK[128];
memset(tmpGBK,0, sizeof(tmpGBK));
utf82gbk(tmpGBK, "测试一下UTF-8转成GBK.\n");
printf(tmpGBK);
```
> Convert function would detect string end when converting. So GBK string and UTF-8 string for converted must end with **'\0'**. And Unicode string must end with **"\0\0"**.

* **Notice**: Immediate string type is according to your code file store mode.

> In my case, all code is stored in UTF-8 mode.(Upper code demo.)
> If your code file is in GBK mode. Then you should test code like this:
> ```C
> char tmpUTF8[128];
> memset(tmpUTF8,0, sizeof(tmpUTF8));
> gb2utf8(tmpUTF8, "测试一下GBK转成UTF-8.\n");
> printf(tmpUTF8);
> ```


-------------------------------
> 这本来就是一个简单的功能。不过查了半天资料，居然没有一个库是可以拿来就用的。崩溃。所以就查了资料自己写了个库。自从用了stm32，一个项目还没构建完，居然就造了好几个轮子了。
> 本来想写个中文的说明，不过今天太晚了。睡了。


