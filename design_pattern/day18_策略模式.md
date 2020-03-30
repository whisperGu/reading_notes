#### 策略模式

* 定义：定义一系列算法，将每一个算法封装起来，并让它们可以相互替换。策略模式让算法独立于使用它的客户而变化。

* 动机：完成一项任务，往往可以有多种不同的方式，每一种方式称为一个策略，我们可以根据环境或者条件的不同选择不同的策略来完成该项任务。解决硬编码和ifelse过度的情况。

* 角色：环境类，具体策略类

* 实例：排序的最优

* 总结：策略模式主要优点在于对“开闭原则”的完美支持，在不修改原有系统的基础上可以更换算法或者增加新的算法，它很好地管理算法族，提高了代码的复用性，是一种替换继承，避免多重条件转移语句的实现方式；其缺点在于客户端必须知道所有的策略类，并理解其区别，同时在一定程度上增加了系统中类的个数，可能会存在很多策略类。

  ```c++C++
  class BaseSort
  {
  	virtual void sort();
  }
  class QuickSort(BaseSort);
  class SelectedSort(BaseSort);
  class Context
  {
  	BaseSort *p;
  	setSort(BaseSort *p;);
  	sort
  	{
  		p->sort()
  	}
  }
  void click()
{
  	Context *p = new Context();
  	p->setSort(new QuickSort());
  	p->sort();
  	p->setSort(new SelectedSort());
  	p->sort();
  }
  ```
  
  