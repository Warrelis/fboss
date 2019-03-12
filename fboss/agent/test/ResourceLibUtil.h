// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <folly/IPAddressV4.h>
#include <folly/Singleton.h>
#include <cstdint>
#include <type_traits>

#include "fboss/agent/state/RouteTypes.h"
#include "fboss/agent/state/Route.h"

namespace facebook {
namespace fboss {
namespace utility {

template <typename IdT>
class ResourceCursor {
 public:
  ResourceCursor();
  IdT getNext();
  void resetCursor(IdT count);
  IdT getCursorPosition() const;

 private:
  IdT current_;
};

template <typename AddrT>
struct IdT {
  using type = std::
      enable_if_t<std::is_same<AddrT, folly::IPAddressV4>::value, uint32_t>;
};

template <typename AddrT>
class IPAddressGenerator : private ResourceCursor<typename IdT<AddrT>::type> {
 public:
  using ResourceT = AddrT;
  using BaseT = ResourceCursor<typename IdT<AddrT>::type>;
  using IdT = typename IdT<AddrT>::type;

  /* simply generate an ip at cursor position id, doesn't update cursor */
  ResourceT get(IdT id) const {
    return getIP(id);
  }

  /* generate an ip and update cursor */
  ResourceT getNext() {
    return get(BaseT::getNext());
  }

  /* generate next n ips, updates cursor */
  std::vector<ResourceT> getNextN(uint32_t n);

  /* generate next n ips, starting from startId, doesn't update cursor */
  std::vector<ResourceT> getN(IdT startId,  uint32_t n) const;

  /* reset cursor */
  void startOver(IdT id) {
    BaseT::resetCursor(id);
  }

  /* return current cursor position */
  IdT getCursorPosition() {
    return BaseT::getCursorPosition();
  }

 private:
  ResourceT getIP(uint32_t id) const;
};


} // namespace utility
} // namespace fboss
} // namespace facebook