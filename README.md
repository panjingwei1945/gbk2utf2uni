# gbk2utf2uni

Bidirectional converter string from simplified chinese GBK string to UTF-8 / unicode for a embed system (stm32, x86, or any other little endian systerm). It is also suit for bidirctional converting string from BIG5, Japanese Shift-JIS or Korean EUC-KR to UTF-8/unicode.

* This library didn't depend on any other library.
* Only suit for **little endian** system. For example x86, stm32.
* This library only suit for characters who's unicode index smaller than 0xFFFF. Whicn means it is suit for almost every character.
* I only test this library for GBK converting on stm32f4 system.
* The encode table is modified from fatfs library. And converter part is enlightened from [不依赖任何系统API,用c语言实现gbk/utf8/unicode编码转换](https://blog.csdn.net/bladeandmaster88/article/details/54837338 "不依赖任何系统API,用c语言实现gbk/utf8/unicode编码转换") and [GB2312转unicode程序](https://www.cnblogs.com/flying_bat/archive/2008/04/11/1148042.html "GB2312转unicode程序")

## How to use this library.
* Add gbk2utf2uni.h, gbk2utf2uni.c and GBK.c into your project.
* If you using fatfs library and you already have cc936.c file in your project, you needn't to do anything.
> Traditional chinese correspond to cc950.c. Japanese correspond to cc932.c.Korean correspond to cc949.c. 

* If you don't use fatfs, then add GBK.c to you project. Which can provide encode translate table. 
> And this table cost about 180KB rom (Not suit for low cost MCU).

> Traditional chinese correspond to Big5.c. Japanese correspond to Shift-JIS.c.Korean correspond to EUC-KR.c. 
* Then convert string like this:
```c
char tmpGBK[128];
memset(tmpGBK,0, sizeof(tmpGBK));
utf82gbk(tmpGBK, "测试一下UTF-8转成GBK.\n");
printf(tmpGBK);
```
> In my case, all text is store in UTF-8 mode.
> If you store code is GBK mode. Then you should test code like this:
> ```C
> char tmpUTF8[128];
> memset(tmpUTF8,0, sizeof(tmpUTF8));
> gb2utf8(tmpUTF8, "测试一下GBK转成UTF-8.\n");
> printf(tmpUTF8);
> ```

-------------------------------
> 这本来就是一个简单的功能。不过查了半天资料，居然没人写这种库。所以就查了资料自己写了个库。自从用了stm32，一个项目还没构建完，居然就造了好几个轮子了。累觉不爱。
> 本来想写个中文的说明，不过今天太晚了。睡了。


