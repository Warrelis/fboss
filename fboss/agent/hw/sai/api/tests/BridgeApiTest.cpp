/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/agent/hw/sai/api/BridgeApi.h"
#include "fboss/agent/hw/sai/fake/FakeSai.h"

#include <folly/logging/xlog.h>

#include <gtest/gtest.h>

using namespace facebook::fboss;

class BridgeApiTest : public ::testing::Test {
 public:
  void SetUp() override {
    fs = FakeSai::getInstance();
    sai_api_initialize(0, nullptr);
    bridgeApi = std::make_unique<BridgeApi>();
  }
  std::shared_ptr<FakeSai> fs;
  std::unique_ptr<BridgeApi> bridgeApi;
  void checkBridge(const sai_object_id_t& bridgeId) {
    EXPECT_NE(0, bridgeId);
    auto fb = fs->brm.getBridge(bridgeId);
    EXPECT_EQ(fb.id, bridgeId);
  }
  void checkBridgePort(
      const sai_object_id_t& bridgeId,
      const sai_object_id_t& bridgePortId) {
    EXPECT_NE(0, bridgePortId);
    auto fbp = fs->brm.getBridgePort(bridgePortId);
    EXPECT_EQ(fbp.bridgeId, bridgeId);
    EXPECT_EQ(fbp.id, bridgePortId);
  }
};

TEST_F(BridgeApiTest, createBridge) {
  auto bridgeId = bridgeApi->create({}, 0);
  checkBridge(bridgeId);
}

TEST_F(BridgeApiTest, removeBridge) {
  auto bridgeId = bridgeApi->create({}, 0);
  checkBridge(bridgeId);
  bridgeApi->remove(bridgeId);
}

TEST_F(BridgeApiTest, createBridgePort) {
  auto bridgeId = bridgeApi->create({}, 0);
  checkBridge(bridgeId);
  BridgeTypes::MemberAttributeType bridgeIdAttribute =
      BridgeTypes::MemberAttributes::BridgeId(bridgeId);
  auto bridgePortId = bridgeApi->createMember({bridgeIdAttribute}, 0);
  checkBridgePort(bridgeId, bridgePortId);
}

TEST_F(BridgeApiTest, removeBridgePort) {
  auto bridgeId = bridgeApi->create({}, 0);
  checkBridge(bridgeId);
  BridgeTypes::MemberAttributeType bridgeIdAttribute =
      BridgeTypes::MemberAttributes::BridgeId(bridgeId);
  auto bridgePortId = bridgeApi->createMember({bridgeIdAttribute}, 0);
  checkBridgePort(bridgeId, bridgePortId);
  bridgeApi->remove(bridgePortId);
}
