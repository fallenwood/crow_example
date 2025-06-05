

//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 ".\\main.cpp2"


//=== Cpp2 type definitions and function declarations ===========================

#line 1 ".\\main.cpp2"
#define CROW_STATIC_DIRECTORY "ClientApp/dist"
#define CROW_STATIC_ENDPOINT "/<path>"

#include <crow.h>

#include "todo.h"
#include <atomic>
#include <map>
#include <mutex>

std::atomic<int> incrementer = 0;
std::map<int, Todo> todos;
std::mutex mutex;

#line 15 ".\\main.cpp2"
auto main() -> int;

//=== Cpp2 function definitions =================================================

#line 1 ".\\main.cpp2"

#line 15 ".\\main.cpp2"
auto main() -> int{
  auto app {crow::SimpleApp()}; 

  // app.loglevel(crow::LogLevel::Warning);

  // TODO: do not use ifstream
  CROW_ROUTE(app, "/")([]() -> auto{
    // std::ifstream file("ClientApp/dist/index.html");
    auto file {std::ifstream("ClientApp/dist/index.html")}; 
    auto buffer {std::stringstream()}; 
    buffer << CPP2_UFCS(rdbuf)(cpp2::move(file));
    return CPP2_UFCS(str)(cpp2::move(buffer)); 
  });

  CROW_ROUTE(app, "/health")([]() -> decltype(auto) { return "Healthy";  });

  CPP2_UFCS(methods)(CROW_ROUTE(app, "/greeting"), "GET"_method)([]() -> auto{
    std::array<std::string,3> data {"hello", "world", "!!!"}; 
    // auto n = std::rand() % 3;
    auto n {std::rand() % CPP2_UFCS(size)(data)}; 
    return CPP2_ASSERT_IN_BOUNDS(cpp2::move(data), cpp2::move(n)); 
  });

  CPP2_UFCS(methods)(CROW_ROUTE(app, "/api/todo/list"), "GET"_method)([]() -> auto{
    std::lock_guard<std::mutex> lock {mutex}; 
    crow::json::wvalue response {}; 

    CPP2_ASSERT_IN_BOUNDS(response, "status") = 0;

    auto todo_list {std::vector<crow::json::wvalue>()}; 
    CPP2_UFCS(reserve)(todo_list, CPP2_UFCS(size)(todos));
    for ( auto const& pair : todos ) {
      auto todo {pair.second}; 
      crow::json::wvalue todo_json {}; 
      CPP2_ASSERT_IN_BOUNDS(todo_json, "id") = todo.id;
      CPP2_ASSERT_IN_BOUNDS(todo_json, "task") = todo.task;
      CPP2_ASSERT_IN_BOUNDS(todo_json, "done") = cpp2::move(todo).done;
      CPP2_UFCS(push_back)(todo_list, cpp2::move(todo_json));
    }

    CPP2_ASSERT_IN_BOUNDS(response, "data") = move(cpp2::move(todo_list));

    return crow::response(cpp2::move(response)); 
  });

  CPP2_UFCS(methods)(CROW_ROUTE(app, "/api/todo")
      , "POST"_method)([](auto const& req) -> auto{
        // std::cout << "Received POST request to /api/todo: " << req.body << std::endl;
        auto x {crow::json::load(req.body)}; 
        if ((!(x))) {
          return crow::response(crow::status::BAD_REQUEST); 
        }

        auto task {CPP2_UFCS(s)(CPP2_ASSERT_IN_BOUNDS(x, "task"))}; 
        auto done {CPP2_UFCS(b)(CPP2_ASSERT_IN_BOUNDS(cpp2::move(x), "done"))}; 

        auto id {std::atomic_fetch_add(&incrementer, 1)}; 

        Todo todo {id, cpp2::move(task), cpp2::move(done)}; 

        auto todo_json {crow::json::wvalue()}; 
        CPP2_ASSERT_IN_BOUNDS(todo_json, "id") = todo.id;
        CPP2_ASSERT_IN_BOUNDS(todo_json, "task") = todo.task;
        CPP2_ASSERT_IN_BOUNDS(todo_json, "done") = todo.done;

        {
                std::lock_guard<std::mutex> lock {mutex}; 
          CPP2_UFCS(insert)(todos, (cpp2::move(id), move(cpp2::move(todo))));
        }

        crow::json::wvalue response {}; 
        CPP2_ASSERT_IN_BOUNDS(response, "status") = 0;
        CPP2_ASSERT_IN_BOUNDS(response, "data") = move(cpp2::move(todo_json));

        return crow::response(cpp2::move(response)); 
      });

  CPP2_UFCS(methods)(CROW_ROUTE(app, "/api/todo/<int>")
      , "PUT"_method)([](auto const& req, auto const& id) -> auto{
        auto x {crow::json::load(req.body)}; 
        if ((!(x))) {
          return crow::response(crow::status::BAD_REQUEST); 
        }
        crow::json::wvalue response {}; 
        bool done {CPP2_UFCS(b)(CPP2_ASSERT_IN_BOUNDS(cpp2::move(x), "done"))}; 

        {
          std::lock_guard<std::mutex> lock {mutex}; 
          auto it {CPP2_UFCS(find)(todos, id)}; 
          if ((it == CPP2_UFCS(end)(todos))) {
            return crow::response(crow::status::NOT_FOUND); 
          }else {
            (*cpp2::impl::assert_not_null(it)).second.done = cpp2::move(done);

            CPP2_ASSERT_IN_BOUNDS(response, "status") = 0;

            crow::json::wvalue todo_json {}; 
            CPP2_ASSERT_IN_BOUNDS(todo_json, "id") = (*cpp2::impl::assert_not_null(it)).second.id;
            CPP2_ASSERT_IN_BOUNDS(todo_json, "task") = (*cpp2::impl::assert_not_null(it)).second.task;
            CPP2_ASSERT_IN_BOUNDS(todo_json, "done") = (*cpp2::impl::assert_not_null(cpp2::move(it))).second.done;

            CPP2_ASSERT_IN_BOUNDS(response, "data") = move(cpp2::move(todo_json));
          }
        }

        return crow::response(cpp2::move(response)); 
      });

  CPP2_UFCS(methods)(CROW_ROUTE(app, "/api/todo/<int>")
      , "DELETE"_method)([](auto const& req, auto const& id) -> auto{
        crow::json::wvalue response {}; 
        {
          std::lock_guard<std::mutex> lock {mutex}; 
          auto it {CPP2_UFCS(find)(todos, id)}; 
          if ((it == CPP2_UFCS(end)(todos))) {
            return crow::response(crow::status::NOT_FOUND); 
          }
          CPP2_UFCS(erase)(todos, cpp2::move(it));

          CPP2_ASSERT_IN_BOUNDS(response, "status") = 0;
          return crow::response(cpp2::move(response)); 
        }
      });

  CPP2_UFCS(run)(CPP2_UFCS(multithreaded)(CPP2_UFCS(port)(cpp2::move(app), 18080)));
}

