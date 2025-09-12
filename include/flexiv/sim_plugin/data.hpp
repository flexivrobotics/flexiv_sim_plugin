/**
 * @file data.hpp
 * @brief Header file containing various constant expressions and data structures.
 * @copyright Copyright (C) 2016-2025 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIV_SIM_PLUGIN_DATA_HPP_
#define FLEXIV_SIM_PLUGIN_DATA_HPP_

#include <array>
#include <string>

namespace flexiv {
namespace sim_plugin {

/** Number of simulated digital IO ports */
constexpr size_t kIOPorts = 16;

/** States data of a simulated robot in the external simulator */
struct SimRobotStates
{
    /** @brief Customized constructor */
    SimRobotStates(
        uint64_t _servo_cycle, const std::vector<float>& _q, const std::vector<float>& _dq)
    : servo_cycle(_servo_cycle)
    , q(_q)
    , dq(_dq)
    {
    }

    /** @brief Default constructor */
    SimRobotStates() = default;

    /** Servo cycle incremented once per physics step of the external simulator */
    uint64_t servo_cycle = 0;

    /** Current joint positions of the simulated robot: \f$ q \in \mathbb{R}^{n \times 1} \f$. Unit:
     * \f$ [rad] or [m] \f$.
     * @note This contains values for both the external axes (if any) and the robot manipulator. */
    std::vector<float> q = {};

    /** Current joint velocities of the simulated robot: \f$ \dot{q} \in \mathbb{R}^{n \times 1}
     * \f$. Unit: \f$ [rad/s] or [m/s] \f$.
     * @note This contains values for both the external axes (if any) and the robot manipulator. */
    std::vector<float> dq = {};
};

/** Commands data for a simulated robot in the external simulator */
struct SimRobotCommands
{
    /** @brief Customized constructor */
    SimRobotCommands(const std::array<float, kJointDoF>& _tau_d,
        const std::array<bool, kIOPorts>& _digital_outputs)
    : tau_d(_tau_d)
    , digital_outputs(_digital_outputs)
    {
    }
    /** @brief Default constructor */
    SimRobotCommands() = default;

    /** Target joint torques for the simulated robot [Nm]. */
    std::array<float, kJointDoF> tau_d = {};

    /** Desired digital outputs for the simulated robot. The index of this boolean array corresponds
     * to that of the digital output ports. True: port high, false: port low. */
    std::array<bool, kIOPorts> digital_outputs = {};
};

} /* namespace sim_plugin */
} /* namespace flexiv */

#endif /* FLEXIV_SIM_PLUGIN_DATA_HPP_ */
