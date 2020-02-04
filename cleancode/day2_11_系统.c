11.1 HOW WOULD YOU BUILD A CITY? 如何建造一个城市

这一章节过于复杂，仅理解了下面的内容，2020年下半年二刷时，看看能否看出心得。

精髓：构造与使用分开。
1# 将全部构造过程搬迁到 main 或被称之为 main 的模块中，设计系统的其余部分时，假设所有对象都已正确构造和设置
2# 依赖注入 Dependency Injection 
为什么要用依赖注入？
付代码一段：
class Car {
  Engine engine;
  Tires tires;
  var description = 'No DI';

  Car() {
    engine = new Engine();
    tires = new Tires();
  }

  // Method using the engine and tires
  String drive() => '$description car with '
    '${engine.cylinders} cylinders and '
    '${tires.make} tires.';
}

如果Engine类升级了，它的构造函数要求一个参数，这该怎么办？这个Car类就被破坏了，
在把创建引擎的代码重写为engine = new Engine(theNewParameter)之前，它都是坏的，
当Engine类的定义发生变化时，就不得不在乎了，Car类也不得不跟着改变。这就会让Car类过于脆弱。
测试时也一头雾水，因为依赖过多，你无法想清楚所以testcase.我们很难控制这背后的依赖。

采用DI可轻松解决：
final Engine engine;
final Tires tires;
String description = 'DI';
Car(this.engine, this.tires);
Car类不再创建引擎或者轮胎。它仅仅“消费”它们，即可以让类从外部源中获得它的依赖，而不必亲自创建它们。

接着依赖注入框架，用这个注入器注册一些类，它会解决如何创建它们。
当需要一个Car时，就简单的请求注入器获取它就可以了。
var car = injector.get(Car);


设值注入是指通过setter方法传入被调用者的实例。这种注入方式简单、直观，因而在Spring的依赖注入里大量使用。

注入的配置文件：
＜!-- 下面是标准的XML文件头 --＞
＜?xml version="1.0" encoding="gb2312"?＞
＜!-- 下面一行定义Spring的XML配置文件的dtd --＞
"http://www.springframework.org/dtd/spring-beans.dtd"＞
＜!-- 以上三行对所有的Spring配置文件都是相同的 --＞
＜!-- Spring配置文件的根元素 --＞
＜BEANS＞
＜!—定义第一bean，该bean的id是chinese, class指定该bean实例的实现类 --＞
＜BEAN class=lee.Chinese id=chinese＞
＜!-- property元素用来指定需要容器注入的属性，axe属性需要容器注入此处是设值注入，因此Chinese类必须拥有setAxe方法 --＞
＜property name="axe"＞
＜!-- 此处将另一个bean的引用注入给chinese bean --＞
＜REF local="”stoneAxe”/"＞
＜/property＞
＜/BEAN＞
＜!-- 定义stoneAxe bean --＞
＜BEAN class=lee.StoneAxe id=stoneAxe /＞
＜/BEANS＞

ApplicationContext ctx = new FileSystemXmlApplicationContext("bean.xml");
//通过Person bean的id来获取bean实例，面向接口编程，因此
//此处强制类型转换为接口类型
Person p = (Person)ctx.getBean("chinese");
//直接执行Person的userAxe()方法。
p.useAxe();

主程序调用Person的useAxe()方法时，该方法的方法体内需要使用Axe的实例，但程序里没有任何地方将特定的Person实例和Axe实 例耦合在一起。
或者说，程序里没有为Person实例传入Axe的实例，Axe实例由Spring在运行期间动态注入

3# 工厂模式

目的：解耦

简单工厂： 	工厂对象根据消息决定要创建什么的类实例。

public class ShapeFactory 
{ 
    public Shape getShape(String type)
    { 
        if ("circle".equals(type)) 
        { 
            return new Circle(); 
        } 
        else if ("rectangle".equals(type)) 
        { 
            return new Rectangle(); 
        } else if ("square".equals(type)) 
        { 
            return new Square(); 
        } 
        else { return null; 
        }
    } 
}

工厂方法：定义一个创建对象的工厂接口，子类去创建对象。
比如以下场景：
 1、日志记录器：记录可能记录到本地硬盘、系统事件、远程服务器等，用户可以选择记录日志到什么地方。
 2、数据库访问，当用户不知道最后系统采用哪一类数据库，以及数据库可能有变化时。
 3、设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口。

public class SquareFactory implements ShapeFactory {

	@Override
	public Shape getShape() {

		return new Square();

	}
}

抽象工厂：围绕一个超级工厂创建其他工厂。使用场景是产品族。

public interface AbstractFactory {

	public Button createButton();

	public Text createText();

}
其他实体工厂派生出来。


4# 无论是设计系统或单独的模块，别忘了使用大概可工作的最简单方案。