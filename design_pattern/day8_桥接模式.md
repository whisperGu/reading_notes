```
桥接模式(Bridge Pattern)
什么是桥接模式？
桥接模式的适用场景有哪些？
桥接模式的优缺点有哪些？
```

什么是桥接模式？

* 桥接模式(Bridge Pattern)：将抽象部分与它的实现部分分离，使它们都可以独立地变化。它是一种对象结构型模式，又称为柄体(Handle and Body)模式或接口(Interface)模式。

  cm:抽象化就是忽略一些信息，把不同的实体当作同样的实体对待；实现化就是对抽象化事物的进一步具体化的产物。脱耦就是抽象化和实现化之间使用关联关系（组合或者聚合关系）而不是继承关系。

* 桥接模式结构：Abstraction：抽象类；RefinedAbstraction：扩充抽象类；Implementor：实现类接口；ConcreteImplementor：具体实现类

  cm：实例开发一个跨平台视频播放器

  ```C++
  class VideoPlayer
  {
  public:
      Platform* os;
  	virtual VideoPlayer(Platform* os) = 0;
      virtual play() = 0;
  }
  class Mp4VideoPlayer:public VideoPlayer
  {
  public:
  	virtual VideoPlayer(Platform* os)
      {
          cout<<"正在播放mp4视频"<<endl;
          os->playInCurrentOsPlatform();
      }
  }
  class AviVideoPlayer:public VideoPlayer
  {
  public:
  	virtual VideoPlayer(Platform* os)
      {
          cout<<"正在播放avi视频"<<endl;
          os->playInCurrentOsPlatform();
      }
  }
  class Platform
  {
      virtual playInCurrentOsPlatform() = 0;
  }
  class WindosPlatform : public Platform
  {
       virtual playInCurrentOsPlatform()
       {
           cout<<"play in windos!"<<end;
       }
  }
  class UnixPlatform : public Platform
  {
       virtual playInCurrentOsPlatform()
       {
           cout<<"play in unix!"<<end;
       }
  }
  int main()
  {
      VideoPlayer *p1 = new Mp4VideoPlayer(new WindosPlatform());
      p1->VideoPlayer();
      
      VideoPlayer *p2 = new AviVideoPlayer(new UnixPlatform());
      p2->VideoPlayer();
  }
  ```

桥接模式的适用场景有哪些？

* 一个类存在两个独立变化的维度，且这两个维度都需要进行扩展。

  cm：比如说是玻璃贴纸厂商，既可以生产萌宠系的长方形贴纸，也可以生产大自然系得圆形贴纸。

* 抽象化角色和实现化角色可以以继承的方式独立扩展而互不影响，在程序运行时可以动态将一个抽象化子类的对象和一个实现化子类的对象进行组合，即系统需要对抽象化角色和实现化角色进行动态耦合。

  cm: 比如上述例子中的视频播放器，操作系统平台和视频种类分别扩展，互不影响，随时组合，太灵活了。

桥接模式的优缺点？

* 优点：这种随机组合的方式类似于多重继承，这其实是违反SRP的，桥接模式则是更好的方法；提高各个维度的扩展。
* 缺点：设计难度大，后续维护理解难度上升。

> 来源：https://design-patterns.readthedocs.io/zh_CN/latest/structural_patterns/bridge.html





























