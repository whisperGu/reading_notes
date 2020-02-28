#### 工厂方法
* 模式动机：不在设计一个工厂类创建所有类，而是采用先定义一个抽象工厂类，在定义具体的工厂类生产各自的产品类。如果出现新的产品，只需要新增具体工厂类即可，
无需修改其他类，符合开封闭原则。
* 模式结构：抽象工厂-》具体工厂-》生产具体产品-》抽象产品
* 代码结构：
#include "ConcreteFactory.h"
#include "ConcreteProduct.h"

Product* ConcreteFactory::factoryMethod(){

	return  new ConcreteProduct();
}
#include "Factory.h"
#include "ConcreteFactory.h"
#include "Product.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	Factory * fc = new ConcreteFactory();
	Product * prod = fc->factoryMethod();
	prod->use();
	
	delete fc;
	delete prod;
	
	return 0;
}
* 优点：在工厂方法模式中，核心的工厂类不再负责所有产品的创建，而是将具体创建工作交给子类去做。
这个核心类仅仅负责给出具体工厂必须实现的接口，而不负责哪一个产品类被实例化这种细节，这使得工厂方法模式可以允许系统在不修改工厂角色的情况下引进新产品。
* 实例：日志记录器

某系统日志记录器要求支持多种日志记录方式，如文件记录、数据库记录等，且用户可以根据要求动态选择日志记录方式， 现使用工厂方法模式设计该系统。

class LogFactory
{
public:
	virtual Log* write()=0;
}

class FileLogFactory:public LogFactory
{
public:
	virtual Log* create();
};
Log* FileLogFactory::create()
{
	return new FileLog();
	cout<<"FileLog::create file log()"<<endl;
}
class DBLogFactory:public LogFactory
{
public:
	virtual Log* create();
};
Log* DBLogFactory::create()
{
	return new DBLog();
	cout<<"DBLog::create file log()"<<endl;
}

class Log
{
	virtual void write()=0;
}