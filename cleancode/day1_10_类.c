10.2.1 The Single Responsibility Principle

单一权责原则（SRP）认为，类或模块应有且只有一条加以修改的理由。
该原则既给出了权责的定义，又是关于类的长度的指导方针。类只应有一个权责——只有一条修改的理由。
一个类就算只有3个方法，也可能需要拆分。

我们为什么会经常会遇到做太多事的类？
原因就是欲速，太多人在程序能工作时就以为万事大吉了。我们没能把思维转向有关代码组织和整洁的部分。
我们直接转向下一个问题，而不是回头将臃肿的类切分为只有单一权责的去耦式单元。

10.2.2 内聚 Cohesion

单一权责类的设计准则：内聚 Cohesion
内聚越高，代表类方法使用类变量次数越多，整个类是一个互相依赖，逻辑紧密的整体。

当一个超级大都函数出现，我们想要拆分一部分代码为函数，但此时需要传递给这个拆分函数4个参数时，你需要讲这4个
参数提升为类的实体变量即可，但如果拆分出的函数很多都共享这些实体变量，那么我们就可以讲他们内聚成一个类。

重构方法 第一程序->第二程序：
我们通过编写验证第一个程序的精确行为的用例来实现修改。然后，我们做了许多小改动，每次改动一处。
每改动一次，就执行一次，确保程序的行为没有变化。一小步接着一小步，第一个程序被逐渐清理和转换为第二个程序。

10.2.3 为了修改而组织 ORGANIZING FOR CHANGE

我们经常面临修改类的情况，比如对类新增某个方法，我们就得“打开”这个类进行修改。
打开类带来的问题是风险随之而来。对类的任何修改都有可能破坏类中的其他代码。必须全面重新测试。
所以，一旦打开了类，就应当修正设计方案，重新设计类！

开闭原则（OCP）Open-Closed Principle：
类应当对扩展开放，对修改封闭。通过子类化手段，重新架构的 Sql 类对添加新功能是开放的，而且可以同时不触及其他类。

外部依赖采用打桩方法，即DIP 认为类应当依赖于抽象而不是依赖于具体细节，依赖倒置原则（Dependency Inversion Principle，DIP）
依赖倒置原则的本质就是通过抽象(抽象类或接口)使各个类或模块实现彼此独立，不互相影响，实现模块间的松耦合。

付一段DPI的例子代码：

/将司机模块抽象为一个接口
public interface IDriver {
     //是司机就应该会驾驶汽车
     public void drive(ICar car);
}

public class Driver implements IDriver{
    //司机的主要职责就是驾驶汽车
    public void drive(ICar car){
        car.run();
    }
}

//将汽车模块抽象为一个接口：可以是奔驰汽车，也可以是宝马汽车
public interface ICar {
      //是汽车就应该能跑
      public void run();
}

public class Benz implements ICar{
    //汽车肯定会跑
    public void run(){
        System.out.println("奔驰汽车开始运行...");
  }
}

public class BMW implements ICar{
   //宝马车当然也可以开动了
   public void run(){
      System.out.println("宝马汽车开始运行...");
   }
}

//高层模块
public class Client {
     public static void main(String[] args) {
       IDriver xiaoLi = new Driver();
       ICar benz = new Benz();
      //小李开奔驰车
      xiaoLi.drive(benz);
   }
}
