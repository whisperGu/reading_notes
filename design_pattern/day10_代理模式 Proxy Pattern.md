#### 代理模式 Proxy Pattern

* 定义：给某一个对象提供代理，该代理对象可以控制对原对象的引用。

* 动机：客户可能不想或不能直接引用一个对象，此时可以通过代理进行简介引用。通过引入一个新的对象（如小图片和远程代理 对象）来实现对真实对象的操作或者将新的对 象作为真实对象的一个替身，这种实现机制即 为代理模式，通过引入代理对象来间接访问一 个对象，这就是代理模式的模式动机。

* 结构：Subject: 抽象主题角色  Proxy: 代理主题角色  RealSubject: 真实主题角色

* 实例：

  ```C++
  #include <iostream>
  #include "RealSubject.h"
  #include "Proxy.h"
  
  using namespace std;
  
  int main(int argc, char *argv[])
  {
  	Proxy proxy;
  	proxy.request();
  	
  	return 0;
  }
  #include "RealSubject.h"
  #include "Subject.h"
  
  class Proxy : public Subject
  {
  
  public:
  	Proxy();
  	virtual ~Proxy();
  
  	void request();
  
  private:
  	void afterRequest();
  	void preRequest();	
  	RealSubject *m_pRealSubject;
  
  };
  #endif 
  
  #include "Proxy.h"
  #include <iostream>
  using namespace std;
  
  
  Proxy::Proxy(){ 
  	m_pRealSubject = new RealSubject();
  }
  
  Proxy::~Proxy(){
  	delete m_pRealSubject;
  }
  
  void Proxy::afterRequest(){
  	cout << "Proxy::afterRequest" << endl;
  }
  
  void Proxy::preRequest(){
  	cout << "Proxy::preRequest" << endl;
  }
  
  void Proxy::request(){
  	preRequest();
  	m_pRealSubject->request();
  	afterRequest();
  ```

* 适用环境：

  1. 