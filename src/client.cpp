#include "client.hpp"
#include "node.hpp"

namespace open62541 {

class ClientPrivate {
  src::severity_channel_logger<severity_level, std::string> m_lg;
  std::string m_channel = "ua_client";
  std::shared_ptr<UA_Client> m_client;
  std::shared_ptr<Client> q_ptr;


public:

  ClientPrivate(std::shared_ptr<Client> client) : q_ptr{client} {}
//  ClientPrivate(Client *client) : q_ptr(std::make_shared<Client>(client)) {}
  ClientPrivate() {}

  UA_BrowseResponse browse(const UA_BrowseRequest &request) {
    UA_BrowseResponse browse_response =
        UA_Client_Service_browse(m_client.get(), request);
    return browse_response;
  }

  std::vector<EndpointDescription> get_endpoints(std::string const &url) {
    std::vector<EndpointDescription> result;
    UA_EndpointDescription *arr = nullptr;
    size_t len = 0;
    auto status =
        UA_Client_getEndpoints(m_client.get(), url.c_str(), &len, &arr);
    if (status == UA_STATUSCODE_GOOD) {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, info)
          << "Got endpoints from " << url;
    } else {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, error)
          << "Getting endpoints failed. Status code = " << status;
    }
    for (size_t i = 0; i < len; i++) {
      result.push_back(EndpointDescription(arr[i]));
    }
    UA_Array_delete(arr, len, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
    return result;
  }

  void connect(std::string const &url) {
    auto status = UA_Client_connect(m_client.get(), url.c_str());
    if (status == UA_STATUSCODE_GOOD) {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, info) << "Connected to " << url;
    } else {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, error)
          << "Connect failed. Status code = " << status;
    }
  }

  void connect(EndpointDescription const &endpoint) {
    auto url = endpoint.to_json()["EndpointUrl"].get<std::string>();
    auto status = UA_Client_connect(m_client.get(), url.c_str());
    if (status == UA_STATUSCODE_GOOD) {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, info) << "Connected to " << url;
    } else {
      BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, error)
          << "Connect failed. Status code = " << status;
    }
  }

  std::shared_ptr<Node> node(NodeId const &node_id) {
    return std::make_shared<Node>(node_id, q_ptr);
  }

  LocalizedText read_display_name_attribute(const NodeId &node_id) {
    UA_LocalizedText out;
    auto status = UA_Client_readDisplayNameAttribute(
        m_client.get(), node_id.ua_node_id(), &out);
    if (status == UA_STATUSCODE_GOOD) {
      return LocalizedText(out);
    }
    BOOST_LOG_CHANNEL_SEV(m_lg, m_channel, error)
        << "Read display name failed. Error code = " << status;
    return LocalizedText();
  }

  std::shared_ptr<Client> client() { return q_ptr; }

  std::shared_ptr<Client> create() {
    return std::shared_ptr<Client>(new Client());
  }
};

Client::Client()
//    : d_ptr{new ClientPrivate(shared_from_this())} {
    : d_ptr{new ClientPrivate} {
}

std::vector<EndpointDescription> Client::get_endpoints(std::string const &url) {
    return d_ptr->get_endpoints(url);
}

void Client::connect(std::string const &url) {
    d_ptr->connect(url);
}

void Client::connect(EndpointDescription const &endpoint) {
    d_ptr->connect(endpoint);
}

std::shared_ptr<Node> Client::node(NodeId const &node_id) {
    return d_ptr->node(node_id);
}

LocalizedText Client::read_display_name_attribute(const NodeId &node_id) {
    return d_ptr->read_display_name_attribute(node_id);
}

std::shared_ptr<Client> Client::client() {
    return d_ptr->client();
}

std::shared_ptr<Client> Client::create() {
    return std::shared_ptr<Client>(new Client());
}

UA_BrowseResponse Client::browse(UA_BrowseRequest const &request) {
    return d_ptr->browse(request);

}

Client::~Client() = default;
//{

//}

}  // namespace open62541
