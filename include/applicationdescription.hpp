#ifndef OPEN62541_CPP_WRAPPER_APPLICATION_DESCRIPTION_H
#define OPEN62541_CPP_WRAPPER_APPLICATION_DESCRIPTION_H

#include "open62541.h"

// STL
#include <string>
#include <vector>

// Dependencies
#include <nlohmann/json.hpp>

#include "localizedtext.hpp"

namespace open62541 {

using namespace nlohmann;

enum class ApplicationType : u_int32_t {
  Server = 0,
  Client = 1,
  ClientAndServer = 2,
  DiscoveryServer = 3
};

class ApplicationDescription {
  std::string m_application_uri;
  std::string m_product_uri;
  LocalizedText m_application_name;
  ApplicationType m_application_type;
  std::string m_gateway_server_uri;
  std::string m_discovery_profile_uri;
  std::vector<std::string> m_discovery_urls;

 public:
  ApplicationDescription() {}
  ApplicationDescription(
      UA_ApplicationDescription const &application_description);

  std::string application_uri() const;
  std::string product_uri() const;
  LocalizedText application_name() const;
  ApplicationType application_type() const;
  std::string gateway_server_uri() const;
  std::string discovery_profile_uri() const;
  std::vector<std::string> discovery_urls() const;
  json to_json() const;
};

}  // namespace open62541

#endif  // OPEN62541_CPP_WRAPPER_APPLICATION_DESCRIPTION_H