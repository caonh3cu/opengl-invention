# opengl-invention
basic Engine of openGL and openVR, model&amp;material reader and basic shading

chinese , no english╮(╯﹏╰）╭


基于opengl编写的引擎

目的是编写一个易于扩展重用的引擎<br/>
为了实现一些图形算法时<br/>
在基本opengl、openvr等接口方面有基本的易用的接口<br/>
大概酱紫(～￣▽￣)～ <br/>

IDE用的vs2015-c++ 32位debug/relese（一定要切到x86才能运行起来，不然就要换64位的各种库）<br/>
<font size="20" color="red"><b> 调用的库都在/VREngine/VREngineine/libAndInclude.zip压缩包里面,在原目录解压即可</b></font><br/>
但是好像需要机器有opengl,
项目名字随便起的不要在意这些细节 (=´ω｀=)
<br/>
<br/>
<br/>
运行图挂目录里面了，好像显示不了。。。程序如果能运行的话就是wasd控制摄像头旋转，ijkl移动，esc退出<br/>

具体注释一点一点写吧，就酱(～￣▽￣)～ <br/>


//最近修改了几个bug<br/>

在Scene目录里面添加了一个sceneDelo.h和相应的cpp，是我的课程作业，简单多边形（没有自相交的多边形）划分成单调多边形（与直线y=k最多只有两个交点的多边形），调用了二维绘图的函数<br/>


