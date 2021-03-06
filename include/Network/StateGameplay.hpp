#pragma once

#include "Network/SessionState.hpp"
#include "IPEndpoint.hpp"
#include "Game.hpp"
#include "Gameplay/Player.hpp"
#include "Network/Session.hpp"

#include <unordered_map>
#include <functional>

class UdpSocket;

namespace MORL {
  namespace Gameplay {
    class Game;
  }

  namespace Network {
    class StateGameplay : public SessionState {
    public:
      using NumClientsChangeCallback = std::function<void(std::unordered_map<IPEndpoint, Gameplay::Player> const&)>;

      StateGameplay(Session &session);
      StateGameplay(StateGameplay const &) = delete;
      StateGameplay(StateGameplay && other);
      ~StateGameplay() = default;

      inline void SetNumClientsChangeCallback(NumClientsChangeCallback const &callback) {
        #ifdef MORL_SERVER_SIDE
        mNumClientsChangeCallback = callback;
        #else
        (void)callback;
        #endif
      }

      void Update() override;
    private:
      UdpSocket &Socket() const {
        return mSession.Socket();
      }

      #ifdef MORL_SERVER_SIDE
      void ServerUpdate();

      size_t PlayerIndex(IPEndpoint const &player);

      void SendPlayerUpdate(IPEndpoint const &forPlayer, IPEndpoint const &to);
      inline void SendPlayerUpdate(IPEndpoint const &to) {
        SendPlayerUpdate(to, to);
      }

      void SendPlayerListing(IPEndpoint const &to);

      void SendPlayerDataToAll(IPEndpoint const &newPlayer);

      void SendPlayerDisconnectToAll(IPEndpoint const &disconnectedPlayer);
      #else
      void ClientUpdate();
      #endif
      inline Gameplay::Game &GameplayGame() {
        return mSession.Game().GameplayGame();
      }
    private:
      bool mWasMoved = false;
      #ifdef MORL_SERVER_SIDE
      NumClientsChangeCallback mNumClientsChangeCallback = [](auto&){};
      #else
      #endif
    };
  }
}