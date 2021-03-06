#pragma once

#include <cstdint>

namespace MORL {
  namespace Network {
    struct PlayerMovementPacket {
      enum Direction : uint8_t {
        Left,
        Right,
        Up,
        Down
      } direction;
    };
  }
}