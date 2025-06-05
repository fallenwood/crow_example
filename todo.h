
#ifndef TODO_H_CPP2
#define TODO_H_CPP2

#define CPP2_IMPORT_STD          Yes

//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 "todo.h2"

#line 17 "todo.h2"
class Todo;
  

//=== Cpp2 type definitions and function declarations ===========================

#line 1 "todo.h2"
// #include <string>

import string;

// struct Todo {
//   int id;
//   std::string task;
//   bool done;
//   Todo(int id, const std::string &task, bool done)
//       : id(id), task(task), done(done) {}
//   Todo(const Todo &other) = default;
//   Todo(Todo &&other) = default;
//   Todo &operator=(const Todo &other) = default;
//   Todo &operator=(Todo &&other) = default;
// };

#line 17 "todo.h2"
class Todo {
  private: int id; 
  private: std::string task; 
  private: bool done; 
  public: Todo() = default;
  public: Todo(Todo const&) = delete; /* No 'that' constructor, suppress copy */
  public: auto operator=(Todo const&) -> void = delete;

#line 21 "todo.h2"
};

#endif
