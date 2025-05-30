#include "crow.h"
#include <iostream>

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() { return "Hello world"; });

  CROW_ROUTE(app, "/health")([]() { return "Healthy"; });

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
