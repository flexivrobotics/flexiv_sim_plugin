/**
 * @example mock_external_simulator.cpp
 * A simple example demonstrating how to use Flexiv Sim Plugin in an external simulator. Please
 * refer to README on how to run this example.
 * @copyright Copyright (C) 2016-2025 Flexiv Ltd. All Rights Reserved.
 * @author Flexiv
 */

#include <flexiv/sim_plugin/user_node.hpp>
#include <spdlog/spdlog.h>

#include <math.h>
#include <iostream>
#include <iomanip>
#include <thread>

using namespace flexiv;

namespace {
// Physics loop frequency of the external simulator
constexpr unsigned int kPhysicsLoopFreq = 2000;
// Physics loop period of the external simulator [sec]
constexpr double kPhysicsLoopPeriod = 1.0 / kPhysicsLoopFreq;

// Sine-sweep trajectory amplitude and frequency
constexpr double kSineAmp = 0.035;
constexpr double kSineFreq = 0.3;

// Initial joint positions when the robot is at home posture [rad]
constexpr std::array<float, sim_plugin::kJointDoF> kInitQ
    = {0.0, -0.698132, 0.0, 1.5708, 0.0, 0.698132, 0.0};

// Servo cycle of the physics loop
unsigned int g_servo_cycle = 0;
}

/** @brief Print program usage help */
void PrintHelp()
{
    // clang-format off
    std::cout << "Required arguments: [robot_sn]" << std::endl;
    std::cout << "    robot_sn: Serial number of the robot in Flexiv Elements Studio to connect. Remove any space, e.g. Rizon4s-123456" << std::endl;
    std::cout << "Optional arguments: None" << std::endl;
    std::cout << std::endl;
    // clang-format on
}

/** @brief Print robot states data @ 1Hz */
void StepPhysics(sim_plugin::UserNode& user_node)
{
    // Record time point at the beginning of this cycle
    auto current_time = std::chrono::steady_clock::now();

    // Step 1: obtain robot states from the external simulator
    // =============================================================================================
    // Mocked robot states
    sim_plugin::SimRobotStates robot_states;

    // Set and increment servo cycle per physics step
    robot_states.servo_cycle = g_servo_cycle++;

    // Set joint positions to sine waves
    for (size_t i = 0; i < sim_plugin::kJointDoF; i++) {
        robot_states.q[i]
            = kInitQ[i] + kSineAmp * sin(2 * M_PI * kSineFreq * g_servo_cycle * kPhysicsLoopPeriod);
    }

    // Set joint velocities to corresponding cosine waves
    for (size_t i = 0; i < sim_plugin::kJointDoF; i++) {
        robot_states.dq[i] = kSineAmp * 2 * M_PI * kSineFreq
                             * cos(2 * M_PI * kSineFreq * g_servo_cycle * kPhysicsLoopPeriod);
    }

    // Step 2: send robot states to Flexiv Elements Studio
    // =============================================================================================
    user_node.SendRobotStates(robot_states);

    // Step 3: wait for Elements Studio to calculate and deliver new robot commands
    // =============================================================================================
    // Must set a timeout value to avoid deadlock
    constexpr unsigned int kWaitTimeoutMs = 100;
    user_node.WaitForRobotCommands(kWaitTimeoutMs);

    // Step 4: apply joint torques command to the simulated robot in the external simulator
    // =============================================================================================
    auto target_joint_torques = user_node.robot_commands().tau_d;
    // Call the external simulator's API to apply the target joint torques, here we just print the
    // values
    std::cout << std::fixed << std::setprecision(1) << "tau_d = ";
    for (const auto& t : target_joint_torques) {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    // End the current physics step with kPhysicsLoopPeriod as the total loop period
    auto loop_period_us = static_cast<unsigned int>(kPhysicsLoopPeriod * 1'000'000);
    std::this_thread::sleep_until(current_time + std::chrono::microseconds(loop_period_us));
}

int main(int argc, char* argv[])
{
    // Program Setup
    // =============================================================================================
    // Parse parameters
    if (argc < 2) {
        PrintHelp();
        return 1;
    }
    // Serial number of the robot to connect to. Remove any space, for example: Rizon4s-123456
    std::string robot_sn = argv[1];

    // Print description
    spdlog::info(
        ">>> Tutorial description <<<\nA simple example demonstrating how to use Flexiv Sim Plugin "
        "in an external simulator.\n");

    // Create user node
    sim_plugin::UserNode user_node(robot_sn);

    // Wait for connected
    while (!user_node.connected()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Run mocked physics loop
    while (user_node.connected()) {
        StepPhysics(user_node);
    }

    return 0;
}
