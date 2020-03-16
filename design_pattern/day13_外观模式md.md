class Pancake
{
    virtual ingredients()=0;
    virtual cost()=0;
}


class EggPancake
{
    ingredients()
    {

    }
    virtual cost()
    {
    
    }
}

class Decorate:public Pancake
{
    virtual Decorate(Pancake* ep);
    virtual ingredients()=0;
    virtual cost()=0;
}

class Bacon:public Decorate
{
    virtual Bacon(Pancake* ep);
    void addBacon()
    {

    }
    virtual ingredients()
    {
        addBacon()
        Decorate::ingredients();
    }
    virtual cost()
    {
        cout<<6<<endl;
    }
}
int main()
{
    Pancake *p = new EggPancake();
    Pancake *p2 = new Bacon(p);
    Pancake *p2 = new Vegetable(p);
    return;
}

外观模式：
在外观模式中，外部与一个子系统的通信必须通过一个统一的外观对象进行，
为子系统中的一组接口提供一个一致的界面，外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。外观模式又称为门面模式
模式分析：
在软件中将一个系统划分为若干个子系统有利于降低整个系统的复杂性，一个常见的设计目标是使子系统间的通信和相互依赖关系达到最小，
而达到该目标的途径之一就是引入一个外观对象，它为子系统的访问提供了一个简单而单一的入口。

举例：买书无需先去排版，印刷室，直接去出版社即可。

class Typeseter;
class Printier;

class Publisher
{
	protect:
	Typeseter *typeseter;
	Printier *printier;

	public:
	Publisher()
	{
		 typeseter = new();
		 printier = new();
	}
	vgetBook()
	{
		typeseter->dosomething();
		printier->dosomething();
	}
}

main
{
	Publisher *p = new Publisher();
	p->getBook();
	
}