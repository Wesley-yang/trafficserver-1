/** @file
 *
 *  A brief file description
 *
 *  @section license License
 *
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "catch.hpp"

#include <cstring>

#ifdef OPENSSL_IS_BORINGSSL
#include <openssl/base.h>
#endif

#include <openssl/ssl.h>

#include "Mock.h"
#include "QUICKeyGenerator.h"

void
print_hex(const uint8_t *v, size_t len)
{
  for (size_t i = 0; i < len; i++) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<uint32_t>(v[i]) << " ";

    if (i != 0 && (i + 1) % 32 == 0 && i != len - 1) {
      std::cout << std::endl;
    }
  }

  std::cout << std::endl;

  return;
}

TEST_CASE("QUICKeyGenerator", "[quic]")
{
  SECTION("CLIENT Cleartext")
  {
    QUICKeyGenerator keygen(QUICKeyGenerator::Context::CLIENT);

    QUICConnectionId cid          = 0x8394c8f03e515708;
    uint8_t expected_client_key[] = {0x9e, 0xdc, 0x91, 0xd5, 0x51, 0x8c, 0x1e, 0x6b,
                                     0x2f, 0x80, 0x2b, 0xd1, 0xc8, 0xad, 0x59, 0x23};
    uint8_t expected_client_iv[] = {0x78, 0xc4, 0x90, 0xe2, 0xe4, 0x22, 0x62, 0x0b, 0x4e, 0xc1, 0xce, 0xc3};

    std::unique_ptr<KeyMaterial> actual_km = keygen.generate(cid);

    CHECK(actual_km->key_len == sizeof(expected_client_key));
    CHECK(memcmp(actual_km->key, expected_client_key, sizeof(expected_client_key)) == 0);
    CHECK(actual_km->iv_len == sizeof(expected_client_iv));
    CHECK(memcmp(actual_km->iv, expected_client_iv, sizeof(expected_client_iv)) == 0);
  }

  SECTION("SERVER Cleartext")
  {
    QUICKeyGenerator keygen(QUICKeyGenerator::Context::SERVER);

    QUICConnectionId cid          = 0x8394c8f03e515708;
    uint8_t expected_server_key[] = {0xa2, 0xaa, 0x67, 0xd4, 0x32, 0x13, 0xba, 0x8d,
                                     0x55, 0xf5, 0x76, 0x84, 0xb7, 0x1c, 0x0f, 0xc0};
    uint8_t expected_server_iv[] = {0xa2, 0x6a, 0xa2, 0x24, 0x5c, 0x4f, 0x76, 0x24, 0xa9, 0x5b, 0x0a, 0xbd};

    std::unique_ptr<KeyMaterial> actual_km = keygen.generate(cid);

    CHECK(actual_km->key_len == sizeof(expected_server_key));
    CHECK(memcmp(actual_km->key, expected_server_key, sizeof(expected_server_key)) == 0);
    CHECK(actual_km->iv_len == sizeof(expected_server_iv));
    CHECK(memcmp(actual_km->iv, expected_server_iv, sizeof(expected_server_iv)) == 0);
  }
}
