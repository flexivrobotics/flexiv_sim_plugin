/**
 * @file user_node.hpp
 * @copyright Copyright (C) 2016-2025 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIV_SIM_PLUGIN_USER_NODE_HPP_
#define FLEXIV_SIM_PLUGIN_USER_NODE_HPP_

#include "data.hpp"
#include <memory>

namespace flexiv {
namespace sim_plugin {

/**
 * @class UserNode
 * @brief A communication node for an external simulator to publish robot states to and receive
 * robot commands from Flexiv Elements Studio.
 */
class UserNode final
{
public:
    /**
     * @brief Create the node.
     * @param[in] robot_sn Serial number of the simulated robot created in Flexiv Elements Studio.
     * The accepted formats are: "Rizon 4-123456" and "Rizon4-123456".
     * @param[in] debug_print Print debug info.
     */
    UserNode(const std::string& robot_sn, bool debug_print = false);
    virtual ~UserNode();

    /**
     * @brief Whether this node is connected with Flexiv Elements Studio.
     */
    bool connected() const;

    /**
     * @brief Send robot states data to Flexiv Elements Studio.
     * @param[in] robot_states States data of the simulated robot in the external simulator.
     * @return True: data sent successfully; false: failed to send data.
     */
    bool SendRobotStates(const SimRobotStates& robot_states);

    /**
     * @brief Blocks until the next data packet of robot commands from Flexiv Elements Studio is
     * received or timeout.
     * @param[in] timeout_ms Maximum wait time before this function returns. Must set a value to
     * avoid deadlock. Unit: [ms].
     * @return True: packet received before timeout; false: packet not received before timeout.
     */
    bool WaitForRobotCommands(unsigned int timeout_ms);

    /**
     * @brief Current commands data to apply to the simulated robot in the external simulator.
     * @return Value copy of SimRobotCommands struct.
     * @warning Call this function after WaitForRobotCommands() to get the latest data.
     */
    SimRobotCommands robot_commands() const;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} /* namespace sim_plugin */
} /* namespace flexiv */

#endif /* FLEXIV_SIM_PLUGIN_USER_NODE_HPP_ */
