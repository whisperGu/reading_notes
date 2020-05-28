观察者模式：
定义：定义一种一对多依赖关系，使得每当一个对象状态发生改变时，其相关联的对象得到通知并自动更新。又叫发布订阅模式。
动机：一个对象发生改变时将自动通知其他对象，其他对象将相应做出反应。在此，发生改变的对象称为观察目标，而被通知的对象称为观察者
角色：观察目标，观察者
模式应用：网站推送打折信息，mvc架构
实例：京东打折信息推送

class Client
{
pubilc:
	string name;
	vector<string> main_info;
	void subscribe(JD *jd)
	{
		jd->registe(this);
	}
	void unsubscribe(JD *jd);
	{
		jd->unregiste(this);
	}
	void sendEmail(string)
	{
		main_info.push_back(string)
	}
	void LookEmail()
	{
		for info in main_info:
		cout<<info<<endl;
	}
}

class JD
{
pubic:
	vector<Client*> my_vector;
	void sendAccoutInfo(string)
	{
		for Client* p in my_vector:
		p->sendEmail(string);
	}
	
}

main
{
	Client *pXiaoHong = new Clinet("小红");
	Client *pXiaoGang = new Clinet("小刚");
	JD *pGouDong = new JD();
	
	pXiaoHong->subscribe(pGouDong);
	pXiaoGang->subscribe(pGouDong);
	
	pGouDong->sendAccoutInfo("PS4降价了！");
	pXiaoHong->LookEmail();
	pXiaoGang->LookEmail();
	
	pXiaoGang->unsubscribe(pGouDong);
	
	pGouDong->sendAccoutInfo("猪肉涨价了！");
	pXiaoHong->LookEmail();
	pXiaoGang->LookEmail();


​	
}

cm: 状态模式可以封装状态，去掉大量的if,else。比如用map就可以。
实例：电梯类

class LiftState()
{
	Content *pContent;
	void setContent();
	void open();
	void run();
	void close();
	void stop();
}

public class StoppingState extends LiftState {
    
    @Override
    public void close() {
        // do nothing;
    }
    
    @Override
    public void open() {
        super.context.setLiftState(Context.openningState);
        super.context.getLiftState().open();
    }
    
    @Override
    public void run() {
        super.context.setLiftState(Context.runningState);
        super.context.getLiftState().run();
    }
    
    @Override
    //他的职责
    public void stop() {
        System.out.println("电梯停止了...");
    }
}
# 客户端调用的真实电梯
class Context()
{
	openningState = new OpenningState();
	runningState = new RunningState();
	
	m_state;
	getLiftState();
	setLiftState()
	{
		m_state= m_state;
		m_state->setContext(this)
	}
	void open()
	{
	m_state->open();
	}
	void run();
	void close();
	void stop();
}