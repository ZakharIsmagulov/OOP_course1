#include <iostream>
#include <string>
#include "Container.cpp"
#include "Object.h"

int int_rand_seed(int from, int to) {
	return (std::rand() % (to - from + 1)) + from;
}

double double_rand_seed(double from, double to) {
	return (double(std::rand()) / RAND_MAX) * (to - from) + from;
}

void rand_cont(Container<Task> * const cont, int num) {
	int class_num = int_rand_seed(0, 4);
	switch (class_num)
	{
	case 0: {
		int op = int_rand_seed(0, 3);
		switch (op)
		{
		case 0:
			cont->add_tail(new BinaryOperationTask(BinaryOperationTask::operators::add, double_rand_seed(0.0, 9999.0),
				double_rand_seed(0.0, 9999.0), "add" + std::to_string(num)));
			break;
		case 1:
			cont->add_tail(new BinaryOperationTask(BinaryOperationTask::operators::sub, double_rand_seed(0.0, 9999.0),
				double_rand_seed(0.0, 9999.0), "subtract" + std::to_string(num)));
			break;
		case 2:
			cont->add_tail(new BinaryOperationTask(BinaryOperationTask::operators::mult, double_rand_seed(0.0, 9999.0),
				double_rand_seed(0.0, 9999.0), "multiply" + std::to_string(num)));
			break;
		case 3:
			cont->add_tail(new BinaryOperationTask(BinaryOperationTask::operators::div, double_rand_seed(0.0, 9999.0),
				double_rand_seed(0.0, 9999.0), "divide" + std::to_string(num)));
			break;
		default:
			break;
		}
		break;
	}
	case 1:
		cont->add_tail(new ContainerAppendTask(cont, new ProgramObjectCountTask(), "container append" + std::to_string(num)));
		break;
	case 2:
		cont->add_tail(new ContainerObjectCountTask(cont, "container count" + std::to_string(num)));
		break;
	case 3:
		cont->add_tail(new WithOutputCountTask(cont, "container with output count" + std::to_string(num)));
		break;
	case 4:
		cont->add_tail(new ProgramObjectCountTask("program count" + std::to_string(num)));
		break;
	default:
		break;
	}
}

std::ostream &operator<<(std::ostream &os, Task const &task) {
	return os << task.toString();
}

int main() {


	int seed = 1;
	std::srand(seed);
	Container<Task> cont;
	for (int i = 1; i <= int_rand_seed(10, 20); i++) {
		rand_cont(&cont, i);
	}
	
	Container<std::string> cont_info;

	ProgramObjectCountTask *pr_count = new ProgramObjectCountTask("object count");
	pr_count->do_task();
	std::cout << "There is " + std::to_string(pr_count->get_result()) + " objects in the program\n";

	while (!cont.is_empty()) {
		try {
			cont.begin()->do_task();
			cont_info.add_tail(new std::string(cont.begin()->toString()));
			cont.del_head();
		}
		catch (IncorrectTaskException const &ex) {
			std::cout << "Error in " + ex.class_name + ":\nDivide by zero\n";
			cont_info.add_tail(new std::string("Missing task (divide by zero)\n"));
			cont.del_head();
		}
		catch (...) {
			cont_info.add_tail(new std::string("Missing task\n"));
			cont.del_head();
		}
	}

	for (Container<std::string>::iterator iter = cont_info.begin(); iter != cont_info.end(); iter++) {
		std::cout << *iter << "\n";
	}
	std::cout << *cont_info.end() << "\n";

	ContainerClearTask *clear1 = new ContainerClearTask(&cont);
	clear1->do_task();
	delete clear1;
	cont_info.clear();
	delete pr_count;

	std::cout << "There is " + std::to_string(Object::get_obj_count()) << " objects in the program\n";
}
