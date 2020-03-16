#### 适配器模式
* 模式动机：通常情况下，客户端可以通过目标类的接口访问他所提供的服务，有时现在的类可以满足客户类的需求，
但是它提供的接口不是客户类所期望的，在这种情况下需要将现在接口适配到客户端期望的接口。在适配模式中，需要
定义一个包装类，包装那些不兼容的接口对象，这个包装就是适配器，它所包装的就是适配者。
* 模式结构：Target 目标抽象类，Adapter 适配器类， Adaptee 适配者类，Client 客户端类
* 实现可以两种方法，一是对象适配器，二是类适配器，前者是类中含有适配者类实例，后者需要继承。
* 优点：目标类和适配者类解耦，增加了类的复用性和透明性，可方便更换适配器和增加适配器类，灵活扩展性都很好。
* 应用：Sun公司在1996年公开了Java语言的数据库连接工具JDBC，JDBC使得Java语言程序能够与数据库连接，并使用SQL语言来查询和操作数据。
JDBC给出一个客户端通用的抽象接口，每一个具体数据库引擎（如SQL Server、Oracle、MySQL等）的JDBC驱动软件都是一个介于JDBC接口和数据库引擎接口之间的适配器软件

class Target
{
	void insert();
	void delete();
	void select();
}

class SQLServer
{
	void SQLServer_insert();
	void SQLServer_delete();
	void SQLServer_select();
}

class Oracle
{
	void Oracle_insert();
	void Oracle_delete();
	void Oracle_select();
}

class JDBC
{
	SQLServer *s;
	Oracle *o;
    void insert()
	{
		s->SQLServer_insert();
	}
	void delete();
	void select();
}