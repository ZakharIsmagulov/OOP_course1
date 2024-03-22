#include "Object.h"

int Object::obj_count = 0;

Object::Object() {
	obj_count++;
}

Object::~Object() {
	obj_count--;
}

int Object::get_obj_count() {
	return obj_count;
}

IncorrectTaskException::IncorrectTaskException(std::string nm) {
	class_name = nm;
}

TaskResultException::TaskResultException(std::string nm) {
	class_name = nm;
}

Task::Task() {
	is_done = false;
	type = types::WITHOUT_OUTPUT;
}

std::string Task::toString() const {
	return is_done ? "true" : "false";
}

void Task::do_task() {
	if (is_done) {
		throw "Task is already done\n";
	}
	else {
		is_done = true;
	}
}

bool const Task::get_state() const {
	return is_done;
}

Task::types const Task::get_type() const {
	return type;
}

Named::Named() {
	name = "";
}

Named::Named(std::string nm) {
	name = nm;
}

std::string Named::toString() const {
	return name;
}

BinaryOperationTask::BinaryOperationTask() {
	op = operators::add;
	v1 = 0;
	v2 = 0;
	res = NULL;
	type = types::WITH_OUTPUT;
}

BinaryOperationTask::BinaryOperationTask(operators oper, double val1, double val2, std::string nm) : Named(nm) {
	op = oper;
	v1 = val1;
	v2 = val2;
	res = NULL;
	type = types::WITH_OUTPUT;
}

void BinaryOperationTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
	}
	Task::do_task();
	switch (op)
	{
	case operators::add:
		res = v1 + v2;
		break;
	case operators::sub:
		res = v1 - v2;
		break;
	case operators::mult:
		res = v1 * v2;
		break;
	case operators::div:
		if (v2 != 0.0) {
			res = v1 / v2;
		}
		else {
			res = NULL;
			throw std::overflow_error("Division by zero");
		}
		break;
	default:
		break;
	}
}

double BinaryOperationTask::get_result() const {
	if (res == NULL && Task::get_state()) {
		throw IncorrectTaskException("BinaryOperationTask");
	}
	else if (res != NULL) {
		return res;
	}
	throw TaskResultException("BinaryOperationTask");
}

std::string BinaryOperationTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: ";
	switch (op)
	{
	case operators::add:
		out += "add " + std::to_string(v2) + " to " + std::to_string(v1) + "\n";
		break;
	case operators::sub:
		out += "subtract " + std::to_string(v2) + " from " + std::to_string(v1) + "\n";
		break;
	case operators::mult:
		out += "multiply " + std::to_string(v1) + " by " + std::to_string(v2) + "\n";
		break;
	case operators::div:
		out += "divide " + std::to_string(v1) + " by " + std::to_string(v2) + "\n";
		break;
	default:
		break;
	}
	if (res != NULL) {
		out += "Task state: done\nResult: " + std::to_string(res) + "\n";
	}
	else if (Task::get_state()) {
		out += "Task state: error\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}

ContainerAppendTask::ContainerAppendTask(Container<Task>* cn, Task *ts, std::string nm) : Named(nm) {
	cont = cn;
	task = ts;
	type = types::WITHOUT_OUTPUT;
}

void ContainerAppendTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
	}
	Task::do_task();
	cont->add_tail(task);
}

std::string ContainerAppendTask::toString() const {
	std::string out = "Task without output \"" + Named::toString() + "\":\n";
	out += "Aim: append task\n{\n" + (*task).toString() + "\n}\nto container\n";
	if (Task::get_state()) {
		out += "Task state: done\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}

ContainerObjectCountTask::ContainerObjectCountTask(Container<Task> *cn, std::string nm) : Named(nm) {
	cont = cn;
	result = NULL;
	type = types::WITH_OUTPUT;
}

void ContainerObjectCountTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
	}
	Task::do_task();
	result = cont->get_count();
}

int ContainerObjectCountTask::get_result() const {
	if (Task::get_state()) {
		return result;
	}
	throw TaskResultException("ContainerObjectCountTask");
}

std::string ContainerObjectCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count objects in the container\n";
	if (Task::get_state()) {
		out += "Task state: done\nResult: " + std::to_string(result) + "\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}

WithOutputCountTask::WithOutputCountTask(Container<Task> *cn, std::string nm) : Named(nm) {
	cont = cn;
	result = NULL;
	type = types::WITH_OUTPUT;
}

void WithOutputCountTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
		return;
	}
	Task::do_task();
	result = 0;
	for (Container<Task>::iterator iter = (*cont).begin(); iter != (*cont).end(); iter++) {
		if ((*iter).get_type() == types::WITH_OUTPUT) {
			result++;
		}
	}
	if ((*(*cont).end()).get_type() == types::WITH_OUTPUT) {
		result++;
	}
}

int WithOutputCountTask::get_result() const {
	if (Task::get_state()) {
		return result;
	}
	throw TaskResultException("WithOutputCountTask");
}

std::string WithOutputCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count tasks with output in the container\n";
	if (Task::get_state()) {
		out += "Task state: done\nResult: " + std::to_string(result) + "\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}

ContainerClearTask::ContainerClearTask(Container<Task> *cn) {
	cont = cn;
	type = types::WITHOUT_OUTPUT;
}

void ContainerClearTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
	}
	Task::do_task();
	if (cont->is_empty()) {
		return;
	}
	for (Container<Task>::iterator iter = (*cont).begin(); iter != (*cont).end(); iter++) {
		if ((&iter) == this) {
			throw "Error: called ContainerClearTask object is in the container\n";
		}
	}
	if ((&(*cont).end() == this)) {
		throw "Error: called ContainerClearTask object is in the container\n";
	}
	cont->clear();
}

std::string ContainerClearTask::toString() const {
	std::string out = "Task without output:\n";
	out += "Aim: clear the container\n";
	if (Task::get_state()) {
		out += "Task state: done\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}

ProgramObjectCountTask::ProgramObjectCountTask() {
	result = NULL;
	type = types::WITH_OUTPUT;
}

ProgramObjectCountTask::ProgramObjectCountTask(std::string nm) : Named(nm) {
	result = NULL;
	type = types::WITH_OUTPUT;
}

void ProgramObjectCountTask::do_task() {
	if (Task::get_state()) {
		throw "Task is already done";
	}
	Task::do_task();
	result = get_obj_count();
}

int ProgramObjectCountTask::get_result() const {
	if (Task::get_state()) {
		return result;
	}
	throw TaskResultException("ProgramObjectCountTask");
}

std::string ProgramObjectCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count all existing Object in the program\n";
	if (Task::get_state()) {
		out += "Task state: done\nResult: " + std::to_string(result) + "\n";
	}
	else {
		out += "Task state: not done\n";
	}
	return out;
}
