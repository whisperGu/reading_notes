#### 中介者模式

* 定义：用一个中介对象来封装一些列的对象交互，使对象不需要显示的相互调用，从而使其松耦合。
* 动机：比如用户与用户经常交互的系统中，用户对象之间有很强的关联性，那么若一个对象发生变化，则需要跟踪和该对象关联的所有对象。
* 角色：中介者，同事类
* 总结：

1. MVC架构中控制器就是中介模式（v与M之间）

2. 通过引入中介者对象，可以将系统的网状结构变成以中介者为中心的星形结构，中介者承担了中转作用和协调作用

3. 中介者模式的主要优点在于简化了对象之间的交互，将各同事解耦，还可以减少子类生成，对于复杂的对象之间的交互，
   通过引入中介者，可以简化各同事类的设计和实现；
   中介者模式主要缺点在于具体中介者类中包含了同事之间的交互细节，可能会导致具体中介者类非常复杂，使得系统难以维护。

##### 实例：虚拟聊天室
```
   某论坛系统欲增加一个虚拟聊天室，允许论坛会员通过该聊天室进行信息交流，
   普通会员(CommonMember)可以给其他会员发送文本信息，钻石会员(DiamondMember)既可以给其他会员发送文本信息，
   还可以发送图片信息。该聊天室可以对不雅字符进行过滤，如“日”等字符；还可以对发送的图片大小进行控制。用中介者模式设计该虚拟聊天室。
```

```C++
class Mediator()
{
    virtual void send_messeage(int nWho, string messeage)
    {
		# 可以对messeage进行过滤
        map<int,Colleague*>::const_iterator itr = m_mpColleague.find(nWho);
        if(itr == m_mpColleague.end())
        {
            cout << "not found this colleague!" << endl;
            return;
        }

        Colleague* pc = itr->second;
        pc->receivemsg(str);
    }
    virtual void registered(int nWho, Member * member)
    {
        map<int,Colleague*>::const_iterator itr = m_mpColleague.find(nWho);
        if(itr == m_mpColleague.end())
        {
            m_mpColleague.insert(make_pair(nWho,aColleague));
            //同时将中介类暴露给colleague 
            aColleague->setMediator(this);
        }
    }
    void send_photo(int nWho, string path)
    {

        map<int,Colleague*>::const_iterator itr = m_mpColleague.find(nWho);
        if(itr == m_mpColleague.end())
        {
            cout << "not found this colleague!" << endl;
            return;
        }

        Colleague* pc = itr->second;
        if (pc->getPermissions() != "VIP")
        {
            cout << "No permission to send photo!" << endl;
            return;
        }
        # 可以对phto大小进行过滤
        pc->receivephoto(str);
    }
}

class NormalMember()
{
protected:
    Mediator *m_media;
public:
	virtual setMember(Mediator *media);
	virtual void sendmsg(int toWho,string str);
	virtual void receivemsg(string str);
    virtual void receivephoto(string str);
    virtual string getPermissions()
    {
        return "NOrmal";
    }
    virtual void send_photo(int nWho, string path)
    {
        m_media->send_photo();
    }
};
class VIPMember(NormalMember)
{
public:
 virtual string getPermissions()
 {
     return "VIP"
 }
};
main
{
    NormalMember * pNormalMember = new NormalMember();
    NormalMember * pVipMember = new VIPMember();
    Mediator * pm = new NormalMediator();

	pm->registered(1,pNormalMember);
	pm->registered(2,pVipMember);
	
	// sendmsg from a to b
	pa->sendmsg(2,"hello,i am a");
	// sendmsg from b to a
	pb->sendmsg(1,"hello,i am b");
    
    // sendphto from a to b
	pa->send_photo(2,"1.jpg");
	// sendmsg from b to a
	pb->send_photo(1,"1.jpg");
}
```

