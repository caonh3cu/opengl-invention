# opengl-invention
basic Engine of openGL and openVR, model&amp;material reader and basic shading
chinese , no english╮(╯﹏╰）╭


基于opengl编写的引擎

目的是编写一个易于扩展重用的引擎
为了实现一些图形算法时
在基本opengl、openvr等接口方面有基本的易用的接口
大概酱紫(～￣▽￣)～ 

IDE用的vs2015-c++ 32位debug/relese， 调用的库都在/VREngine/VREngineine/libAndInclude.zip压缩包里面,在原目录解压即可，但是好像需要机器有opengl,
项目名字随便起的不要在意这些细节 (=´ω｀=)


一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一

AssetManager类：单例类，基于assimp的模型导入类，可以导入材质、纹理，也可以导入shader，并对材质和相应shader进行绑定以供渲染
	其中材质支持：
	specularColor, diffuseColor, ambientColor，shininess;
	specularTexture, diffuseTexture, ambientTexture, shininessTexture，
	normalTexture, heightTexture, lightmapTexture, emissiveTexture
	贴图除了漫反射和环境光都没有测试

	贴图有缓存保证不重复读取图片，材质也有材质名的唯一性（同样的名字只读取第一个）

	和材质类直接相配的两个着色器如shader/shader1

	另外实现了简易的根据配置文件读取模型和着色器（实际上传入的是stringstream）
	挺想写支持XML或者json格式的然而不太了解╮(╯﹏╰）╭

一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一

object类：比较简单了就，只实现了基本功能，物体的各种变换，绑定网格
	另外顺便写了物体管理器，更简单了，只是够现在用

	配套的写了light类（与示例shader匹配的）和camera类，都是刚好能用的。

一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一

massage类：单例类，基于glfw写的窗口类，创建窗口，获取键盘鼠标消息
	其中myWindow类中有一个消息队列messagePump，接收各类消息，放入队列，要用的时候依次读取就好
		键盘事件借用的glfw的宏，鼠标是自己定义的枚举类型在代码里有写
		
	这里面有一些bug，键盘esc按键事件接收不到，
	然后读取鼠标位置本身好用，但是和设置鼠标位置配合用就会有bug(╯︵╰)

一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一

然后VRDriver类：就是驱动vr的，官方给的示例代码改的，简化了一下，手柄和定位器显示的部分我给删了
	这里我用的设备是vive，版本不记得了
	主要过程就是先init() //常规套路
	渲染的时候先调用 VRBegin(); 然后渲染;  然后调用VREnd(); 最后submit()
	实际上是绑定一个帧缓存然后渲染最后提交的过程，也可以直接获取帧缓存，但是这部分我还没写o(*￣3￣)o 

	消息部分是调用HandleInput()来更新头盔和手柄位置，处理各种事件，这里我没写事件队列，
	需要亲自去ProcessVREvent()里改写事件处理过程

	另外HandleInput()和submit()两个函数的调用顺序steam有过建议，
	说要在提交后马上更新位置，还是在更新位置后马上提交，我忘了，也忘了在哪里看的了_(:3|∠)_

	再另外，这个代码我还没测试。。。过两天再测吧。

一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一一

最后scene类是我针对特定应用写的，所以比较乱，里面有使用前面各个类的基本过程

运行图挂目录里面了，好像显示不了。。。程序如果能运行的话就是wasd控制摄像头旋转，ijkl移动，空格退出

具体注释以后再写吧，就酱(～￣▽￣)～ 



