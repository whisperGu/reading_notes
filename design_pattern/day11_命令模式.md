#### 命令模式 Command Pattern

* 定义：将一个请求封装为一个对象，从而使我们可用不同的请求对客户进行参数化；对请求排队或者记录请求日志，以及支持可撤销的操作。
* 结构：Command: 抽象命令类 ConcreteCommand: 具体命令类  Invoker: 调用者  Receiver: 接收者 Client:客户类
* 