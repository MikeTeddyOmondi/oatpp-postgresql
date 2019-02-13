//
//  main.cpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "./controller/UserController.hpp"
#include "./AppComponent.hpp"
#include "./ServiceComponent.hpp"
#include "./SwaggerComponent.hpp"
#include "./Logger.hpp"

#include "oatpp/network/server/Server.hpp"

#include "oatpp-swagger/Controller.hpp"

#include <iostream>


/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run(const oatpp::base::CommandLineArguments& args) {

  AppComponent appComponent(args);
  ServiceComponent serviceComponent;
  SwaggerComponent swaggerComponent;

  /* create ApiControllers and add endpoints to router */

  auto router = serviceComponent.httpRouter.getObject();
  auto docEndpoints = oatpp::swagger::Controller::Endpoints::createShared();

  auto userController = UserController::createShared();
  userController->addEndpointsToRouter(router);

  docEndpoints->pushBackAll(userController->getEndpoints()); // Add userController to swagger

  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  swaggerController->addEndpointsToRouter(router);

  /* create server */

  oatpp::network::server::Server server(serviceComponent.serverConnectionProvider.getObject(),
                                        serviceComponent.serverConnectionHandler.getObject());

  OATPP_LOGD("Server", "Running on port %s...", serviceComponent.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());

  server.run();

}

/**
 *  main
 */
int main(int argc, const char * argv[]) {
  
  oatpp::base::Environment::setLogger(new Logger());
  oatpp::base::Environment::init();

  run(oatpp::base::CommandLineArguments(argc, argv));

  oatpp::base::Environment::setLogger(nullptr); ///< free Logger

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  oatpp::base::Environment::destroy();

  return 0;
}
