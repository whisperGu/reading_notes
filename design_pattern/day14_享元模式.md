享元模式
定义：用共享技术有效地支持大量细粒度对象的复用。系统只使用少量的对象，而这些对象都很相似，状态变化很小，
可以实现对象的多次复用。由于享元模式要求能够共享的对象必须是细粒度对象，因此它又称为轻量级模式
细粒度：在实际使用中，能够共享的内部状态是有限的，因此享元对象一般都设计为较小的对象，它所包含的内部状态较少，即变化较小才能更多的被复用。
模式结构：FlyweightFactory: 享元工厂类，Flyweight: 抽象享元类，ConcreteFlyweight: 具体享元类，UnsharedConcreteFlyweight: 非共享具体享元类

实例：图像显示文档，同一个照片只需要创建一个图像对象，变的状态是位置就可以。

class Photo
{
	string file_path;
	void showPicture(Point p)
	{
		
	}

}

class flyWeightPhotoFactoy
{
	unordered_map<string,Photo*> m;
	Photo *getPhoto(string file_path)
	{
		if exist file_path:
		return old
		else:
		return new 	Photo(file_path);
	}
}
Point p1 = new Photo('1.jpg');
p1->showPicture(Point(1,2));
Point p2 = new Photo('1.jpg');
p2->showPicture(Point(1,2));//其实用的还是p1的对象