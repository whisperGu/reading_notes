#### 3.抽象工厂模式
* 模式动机：在工厂模式中，具体工厂生产具体的产品，这是一对一的关系，但是有时候我们需要一个工厂可以提供多个产品对象。
* 产品等级结构 ：产品等级结构即产品的继承结构，如一个抽象类是电视机，其子类有海尔电视机、海信电视机、TCL电视机，
则抽象电视机与具体品牌的电视机之间构成了一个产品等级结构，抽象电视机是父类，而具体品牌的电视机是其子类。
产品族 ：在抽象工厂模式中，产品族是指由同一个工厂生产的，位于不同产品等级结构中的一组产品，
如海尔电器工厂生产的海尔电视机、海尔电冰箱，海尔电视机位于电视机产品等级结构中，海尔电冰箱位于电冰箱产品等级结构中。
* 模式结构：AbstractFactory：抽象工厂ConcreteFactory：具体工厂AbstractProduct：抽象产品Product：具体产品
抽象工厂-》具体生产产品族的工厂-》生产产品A，生产产品B派生为抽象a和抽象B

class HomeAppFactory
{
public:
	virtual Washer* buildWasher()=0;
	virtual Fridge* buildFridge()=0;
}

class HaierHomeAppFactory
{
	virtual Washer* buildWasher()
	{
		return new HaierWasherFactory();
	}
	virtual Fridge* buildFridge()
	{
		return new HaierFridgeFactory();
	}
}

class Washer
{
public:
	virtual void* washClothes()=0;	
}

class Fridge
{
public:
	virtual void* freshFood()=0;	
}

class HaierWasher
{
	public:
	virtual void* washClothes()
	{
	}
}

class HaierWasherFactory
{
	public:
	virtual Washer* buildHaierWasher()
	{
		return new HaierWasher();
	}
}

int main()
{
	HomeAppFactory * p1 = new HaierHomeAppFactory();
	Washer *p2 = p1->buildHaierWasher();
	p2->washClothes();
	
}