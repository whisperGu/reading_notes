#### 建造者模式
* 动机：建造者模式可以将部件和其组装过程分开，一步一步创建一个复杂的对象。用户只需要指定复杂对象的类型就可以得到该对象，而无需知道该对象的构造细节。
  复杂对象相当于一辆有待建造的汽车，而对象的属性相当于汽车的部件，建造产品的过程就相当于组合部件的过程。由于组合部件的过程很复杂，
  因此，这些部件的组合过程往往被“外部化”到一个称作建造者的对象里，建造者返还给客户端的是一个已经建造完毕的完整产品对象，
  而用户无须关心该对象所包含的属性以及它们的组装方式，这就是建造者模式的模式动机。
  软件开发中，也会有类似的痛点，比如一个复杂对象的成员变量的初始化的顺序，还能存在一些限制。

* 模式结构：Builder：抽象建造者 ConcreteBuilder：具体建造者  Director：指挥者   Product：产品角色

* 优点：客户无需知道对象的具体组装过程，只需确定具体的建造者的类型就可以了，建造者模式将复杂对象的构建和表现分离开来，这样使用相同的构建方法，可以创建不同的表现形式。

* 实例：KFC

  ```c++
  /*
  建造者模式可以用于描述KFC如何创建套餐：套餐是一个复杂对象，它一般包含主食（如汉堡、鸡肉卷等）和饮料（如果汁、 可乐等）等组成部分，不同的套餐有不同的组成部分，而KFC的服务员可以根据顾客的要求，一步一步装配这些组成部分，构造一份完整的套餐，然后返回给顾客。
  */
  class Up
  {
      string drink;
      string hamburger;
  }
  class KFCMealMachine
  {
      Up up;
      virtual void getHamburger() = 0;
      virtual void getDrink() = 0;
      virtual void getUserprofile() = 0;
  }
  class SpringFestivalUpMachine
  {
      Up up;
      virtual void getHamburger()
      {
          
      }
      virtual void getDrink()
      {
          
      }
      virtual void getUserprofile()
      {
          
      }
  }
  class KFCAssistant
  {
      void setMakeUpMachine(KFCMealMachine *m);
      void getUp()
      {
          m->getHamburger();
          m->getDrink();
          return m->getUserprofile();
      }
  }
  ```

  