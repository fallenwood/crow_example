#define CROW_STATIC_DIRECTORY "ClientApp/dist"
#define CROW_STATIC_ENDPOINT "/<path>"

#include "crow.h"

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/health")([]() { return "Healthy"; });

  CROW_ROUTE(app, "/greeting").methods("GET"_method)([]() {
    static std::array<std::string, 3> data{"hello", "world", "!!!"};
    auto n = std::rand() % 3;
    return data[n];
  });

  CROW_ROUTE(app, "/add_json")
      .methods("POST"_method)([](const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x)
          return crow::response(
              crow::status::BAD_REQUEST); // same as crow::response(400)
        int sum = x["a"].i() + x["b"].i();
        std::ostringstream os;
        os << sum;
        return crow::response{os.str()};
      });

  app.port(18080).multithreaded().run();
}
