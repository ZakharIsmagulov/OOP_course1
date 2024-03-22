#pragma once
#include <iostream>
#include <string>
#include "Container.cpp"

class Object {
public:
	Object();
	~Object();

	virtual std::string toString() const = 0;
	static int get_obj_count();
private:
	static int obj_count;
};

struct IncorrectTaskException {
	IncorrectTaskException(std::string nm);
	std::string class_name;
};

struct TaskResultException {
	TaskResultException(std::string nm);
	std::string class_name;
};

class Task : virtual public Object {
public:
	enum class types { WITH_OUTPUT, WITHOUT_OUTPUT };
	Task();

	virtual void do_task();
	bool const get_state() const;
	types const get_type() const;
	std::string toString() const override;
private:
	bool is_done;
protected:
	types type;
};

class Named : virtual public Object {
public:
	Named();
	Named(std::string nm);

	std::string toString() const override;
private:
	std::string name;
};

class BinaryOperationTask : public Task, public Named {
public:
	enum class operators { add, sub, mult, div };
	BinaryOperationTask();
	BinaryOperationTask(operators oper, double val1, double val2, std::string nm);

	void do_task() override;
	double get_result() const;
	std::string toString() const override;
private:
	operators op;
	double v1;
	double v2;
	double res;
};

class ContainerAppendTask : public Task, public Named {
public:
	ContainerAppendTask(Container<Task> *cn, Task *ts, std::string nm);

	void do_task() override;
	std::string toString() const override;
private:
	Container<Task> *cont;
	Task *task;
};

class ContainerObjectCountTask : public Task, public Named {
public:
	ContainerObjectCountTask(Container<Task> *cn, std::string nm);

	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	Container<Task> *cont;
	int result;
};

class WithOutputCountTask : public Task, public Named {
public:
	WithOutputCountTask(Container<Task> *cn, std::string nm);

	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	Container<Task> *cont;
	int result;
};

class ContainerClearTask : public Task {
public:
	ContainerClearTask(Container<Task> *cn);

	void do_task() override;
	std::string toString() const override;
private:
	Container<Task> *cont;
};

class ProgramObjectCountTask : public Task, public Named {
public:
	ProgramObjectCountTask();
	ProgramObjectCountTask(std::string nm);
	
	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	int result;
};
