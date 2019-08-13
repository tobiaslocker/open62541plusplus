#ifndef COMMON_H
#define COMMON_H

#include "applicationdescription.hpp"
#include "browsedescription.hpp"
#include "browserequest.hpp"
#include "datetime.hpp"
#include "endpointdescription.hpp"
#include "nodeid.hpp"
#include "requestheader.hpp"
#include "viewdescription.hpp"

namespace common {

using namespace open62541;

ApplicationDescription make_application_description_1() {
  return ApplicationDescription(
      "app:uri:test:123",
      "prod:uri:test:456",
      LocalizedText(Locale("en"), Text("app-test-name")),
      ApplicationType::Client,
      "gateway:server:uri",
      "discovery:profile:uri",
      {"url-1", "url-2"});
}

ApplicationDescription make_application_description_2() {
  return ApplicationDescription(
      "app:uri:test:123:different",
      "prod:uri:test:456:different",
      LocalizedText(Locale("de"), Text("app-test-name:different")),
      ApplicationType::Client,
      "gateway:server:uri:different",
      "discovery:profile:uri:different",
      {"url-1:different", "url-2:different"});
}

UserTokenPolicy make_user_token_policy_1() {
  return UserTokenPolicy("1",
                         UserTokenType::Anonymous,
                         "issued-token-type-1",
                         "opc.tcp://issuer-endpoint.com:51210/url",
                         "http://opcfoundation.org/UA/SecurityPolicy#None");
}

UserTokenPolicy make_user_token_policy_2() {
  return UserTokenPolicy("2",
                         UserTokenType::Certificate,
                         "issued-token-type-2",
                         "opc.tcp://issuer-endpoint.com:51210/url-2",
                         "http://opcfoundation.org/UA/SecurityPolicy#Basic256");
}

EndpointDescription make_endpoint_description_1() {
  auto server = make_application_description_1();
  return EndpointDescription(
      "opc.tcp://opcua.demo-this.com:51210/UA/SampleServer",
      server,
      "cert",
      MessageSecurityMode::SignAndEncrypt,
      "http://opcfoundation.org/UA/SecurityPolicy#Basic256",
      std::vector<UserTokenPolicy>(
          {make_user_token_policy_1(), make_user_token_policy_2()}),
      "http://opcfoundation.org/UA-Profile/Transport/https-uabinary",
      0);
}

EndpointDescription make_endpoint_description_2() {
  auto server = make_application_description_2();
  return EndpointDescription(
      "opc.tcp://opcua.demo-this.com:51210/UA/SampleServerDifferent",
      server,
      "cert-different",
      MessageSecurityMode::SignAndEncrypt,
      "http://opcfoundation.org/UA/SecurityPolicy#Basic128",
      std::vector<UserTokenPolicy>({make_user_token_policy_2()}),
      "http://opcfoundation.org/UA-Profile/Transport/https-uabinary-different",
      2);
}

NodeId make_node_id_1() {
  return NodeId(0, Identifier(1234), IdentifierType::Numeric);
}

NodeId make_node_id_2() {
  return NodeId(4, Identifier("my-node"), IdentifierType::String);
}

BrowseDescription make_browse_description_1() {
  return BrowseDescription(
      make_node_id_2(), BrowseDirection::Forward, make_node_id_1(), true, 0, 0);
}

BrowseDescription make_browse_description_2() {
  return BrowseDescription(
      make_node_id_1(), BrowseDirection::Inverse, make_node_id_2(), true, 3, 4);
}

DateTime make_date_time_1() {
  // Date and time (GMT): Thursday, August 8, 2019 7:26:35 PM
  int64_t t = 132097659950000000;
  return DateTime(t);
}

ViewDescription make_view_description_1() {
  return ViewDescription(make_node_id_1(), make_date_time_1(), 42);
}

ByteString make_byte_string_1() {
  return ByteString("this-is-byte-string-content");
}

DataTypeMember make_data_type_member_1() {
  return DataTypeMember(
      "data-type-member-name-1", 3, std::byte(2), false, true);
}

DataTypeMember make_data_type_member_2() {
  return DataTypeMember(
      "data-type-member-name-2", 4, std::byte(64), true, false);
}

DataType make_data_type_1() {
  return DataType("data-type-name",
                  make_node_id_2(),
                  255,
                  4,
                  true,
                  false,
                  true,
                  13,
                  {make_data_type_member_1(), make_data_type_member_2()});
}

ExtensionObject make_extension_object_1() {
  return ExtensionObject(
      make_node_id_1(), make_byte_string_1(), ExtensionObjectEncoding::Nobody);
}

ExtensionObject make_extension_object_1_1() {
  return ExtensionObject(
      std::pair<NodeId, ByteString>(make_node_id_1(), make_byte_string_1()),
      ExtensionObjectEncoding::Nobody);
}

ExtensionObject make_extension_object_2() {
  return ExtensionObject(make_data_type_1(),
                         std::make_shared<std::string>("Hello, World!"),
                         ExtensionObjectEncoding::Decoded);
}

ExtensionObject make_extension_object_2_1() {
  return ExtensionObject(
      std::pair<DataType, std::shared_ptr<void>>(
          make_data_type_1(), std::make_shared<std::string>("Hello, World!")),
      ExtensionObjectEncoding::Decoded);
}

RequestHeader make_request_header_1() {
  return RequestHeader(make_node_id_1(),
                       make_date_time_1(),
                       7,
                       13,
                       "this-audit-entry",
                       1888,
                       make_extension_object_1());
}

BrowseRequest make_browse_request_1() {
  return BrowseRequest(
      make_request_header_1(),
      make_view_description_1(),
      34,
      {make_browse_description_1(), make_browse_description_2()});
}

}  // namespace common

#endif  // COMMON_H
