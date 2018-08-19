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
运行图挂目录里面了，好像显示不了。。。<br/>
程序如果能运行的话就是wasd控制摄像头旋转，ijkl移动，esc退出<br/>
然后加载的模型、shader的设置都在config.txt里面<br/>

具体注释一点一点写吧，就酱(～￣▽￣)～ <br/><br/><br/>

———————————————————————————————————————<br/>
几号不记得了：最近修改了几个bug<br/>
然后在Scene目录里面添加了一个sceneDelo.h和相应的cpp，是我的课程作业，简单多边形（没有自相交的多边形）划分成单调多边形（与直线y=k最多只有两个交点的多边形），调用了二维绘图的函数<br/>


———————————————————————————————————————<br/>

8-14：添加了64位的外部库，这样就可以用64位编译运行了<br/>
首先要解压includeAndLib64.zip到所在目录,然后vs里需要设置一下路径和lib的引用<br/>
另外<b>一定要把includeAndLib64里面的dll拿出来替换掉\VREngine\VREngine下的dll，因为那里面的dll还是32位的！！！</b>
ps:代码不用改，但是32位和64位用的各个库版本不一定相同，不知道会不会出什么问题ヽ(￣▽￣)ﾉ

8-19：glm和opencv冲突(╯‵□′)╯︵┻━┻，无法using namespace glm;所以我把项目里面所有using glm都删了，相应地要把所有用到的glm里的函数前面加上glm::累死我了(╯‵□′)╯︵┻━┻，还好文件不多，除了scene都改完上传了scene里面自带using不会出错，而如果以后的scene<b>想加opencv就不要using glm！！</b>
