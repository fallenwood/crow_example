#pragma once

#include <string>

struct Todo {
  int id;
  std::string task;
  bool done;
  Todo(int id, const std::string &task, bool done)
      : id(id), task(task), done(done) {}
  Todo(const Todo &other) = default;
  Todo(Todo &&other) = default;
  Todo &operator=(const Todo &other) = default;
  Todo &operator=(Todo &&other) = default;
};
