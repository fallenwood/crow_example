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

int main() {
  crow::SimpleApp app;

  // app.loglevel(crow::LogLevel::Warning);

  // TODO: do not use ifstream
  CROW_ROUTE(app, "/")([]() {
    std::ifstream file("ClientApp/dist/index.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  });

  CROW_ROUTE(app, "/health")([]() { return "Healthy"; });

  CROW_ROUTE(app, "/greeting").methods("GET"_method)([]() {
    static std::array<std::string, 3> data{"hello", "world", "!!!"};
    auto n = std::rand() % 3;
    return data[n];
  });

  CROW_ROUTE(app, "/api/todo/list").methods("GET"_method)([]() {
    std::lock_guard<std::mutex> lock(mutex);
    crow::json::wvalue response;

    response["status"] = 0;

    std::vector<crow::json::wvalue> todo_list{};
    todo_list.reserve(todos.size());
    for (const auto &pair : todos) {
      const Todo &todo = pair.second;
      crow::json::wvalue todo_json;
      todo_json["id"] = todo.id;
      todo_json["task"] = todo.task;
      todo_json["done"] = todo.done;
      todo_list.push_back(todo_json);
    }

    response["data"] = std::move(todo_list);

    return crow::response{response};
  });

  CROW_ROUTE(app, "/api/todo")
      .methods("POST"_method)([](const crow::request &req) {
        std::cout << "Received POST request to /api/todo: " << req.body << std::endl;
        auto x = crow::json::load(req.body);
        if (!x) {
          return crow::response(crow::status::BAD_REQUEST);
        }

        auto task = x["task"].s();
        auto done = x["done"].b();

        auto id = std::atomic_fetch_add(&incrementer, 1);

        auto todo = Todo(id, task, done);

        crow::json::wvalue todo_json;
        todo_json["id"] = todo.id;
        todo_json["task"] = todo.task;
        todo_json["done"] = todo.done;

        {
          std::lock_guard<std::mutex> lock(mutex);
          // todos[id] = std::move(todo);
          todos.insert({id, std::move(todo)});
        }

        crow::json::wvalue response;
        response["status"] = 0;
        response["data"] = std::move(todo_json);

        return crow::response{response};
      });

  CROW_ROUTE(app, "/api/todo/<int>")
      .methods("PUT"_method)([](const crow::request &req, int id) {
        auto x = crow::json::load(req.body);
        if (!x) {
          return crow::response(crow::status::BAD_REQUEST);
        }

        bool done = x["done"].b();

        crow::json::wvalue response;

        {
          std::lock_guard<std::mutex> lock(mutex);
          auto it = todos.find(id);
          if (it == todos.end()) {
            return crow::response(crow::status::NOT_FOUND);
          }
          it->second.done = done;

          response["status"] = 0;

          crow::json::wvalue todo_json;
          todo_json["id"] = it->second.id;
          todo_json["task"] = it->second.task;
          todo_json["done"] = it->second.done;
          response["data"] = std::move(todo_json);

          return crow::response{response};
        }
      });

  CROW_ROUTE(app, "/api/todo/<int>")
      .methods("DELETE"_method)([](const crow::request &req, int id) {
        crow::json::wvalue response;
        {
          std::lock_guard<std::mutex> lock(mutex);
          auto it = todos.find(id);
          if (it == todos.end()) {
            return crow::response(crow::status::NOT_FOUND);
          }

          todos.erase(it);

          response["status"] = 0;
          return crow::response{response};
        }
      });

  app.port(18080).multithreaded().run();
}
