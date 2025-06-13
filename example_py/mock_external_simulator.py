#!/usr/bin/env python

"""mock_external_simulator.py

A simple example demonstrating how to use Flexiv Sim Plugin in an external simulator. Please
refer to README on how to run this example.
"""

__copyright__ = "Copyright (C) 2016-2025 Flexiv Ltd. All Rights Reserved."
__author__ = "Flexiv"

import time
import math
import argparse
import spdlog  # pip install
import flexivsimplugin  # pip install

# Frequency of the external simulator's physics loop [Hz]
PHYSICS_FREQ = 2000
# Period of the external simulator's physics loop [sec]
PHYSICS_PERIOD = 1.0 / PHYSICS_FREQ

# Sine-sweep trajectory amplitude and frequency
SINE_AMP = 0.035
SINE_FREQ = 0.3

# Initial joint positions when the robot is at home posture [rad]
INIT_Q = [0.0, -0.698132, 0.0, 1.5708, 0.0, 0.698132, 0.0]

# Joint degrees of freedom
JOINT_DOF = 7

# Servo cycle of the physics loop
g_servo_cycle = 0


def step_physics(user_node, logger):
    """
    Step once the external simulator's physics loop.

    """

    # Step 1: obtain robot states from the external simulator
    # =============================================================================================
    # Mocked robot states
    robot_states = flexivsimplugin.SimRobotStates()

    # Set and increment servo cycle per physics step
    global g_servo_cycle
    g_servo_cycle += 1
    robot_states.servo_cycle = g_servo_cycle

    # robot_states.q and dq are fixed-size tuples and don't support per-element assignment
    temp_list = [0] * JOINT_DOF

    # Set joint positions to sine waves
    for i in range(JOINT_DOF):
        temp_list[i] = INIT_Q[i] + SINE_AMP * math.sin(
            2 * math.pi * SINE_FREQ * g_servo_cycle * PHYSICS_PERIOD
        )
    robot_states.q = temp_list

    # Set joint velocities to corresponding cosine waves
    for i in range(JOINT_DOF):
        temp_list[i] = (
            SINE_AMP
            * 2
            * math.pi
            * SINE_FREQ
            * math.cos(2 * math.pi * SINE_FREQ * g_servo_cycle * PHYSICS_PERIOD)
        )
    robot_states.dq = temp_list

    # Step 2: send robot states to Flexiv Elements Studio
    # =============================================================================================
    if not user_node.SendRobotStates(robot_states):
        logger.error("Failed to send robot states data")

    # Step 3: wait for Elements Studio to calculate and deliver new robot commands
    # =============================================================================================
    # Must set a timeout value to avoid deadlock
    WAIT_TIMEOUT_MS = 100
    if not user_node.WaitForRobotCommands(WAIT_TIMEOUT_MS):
        logger.error("WaitForRobotCommands() timeout")

    # Step 4: apply joint torques command to the simulated robot in the external simulator
    # =============================================================================================
    target_joint_torques = user_node.robot_commands().tau_d
    # Call the external simulator's API to apply the target joint torques

    # End the current physics step with kPhysicsPeriod as roughly the total loop period
    time.sleep(PHYSICS_PERIOD)


def main():
    # Program Setup
    # ==============================================================================================
    # Parse arguments
    argparser = argparse.ArgumentParser()
    argparser.add_argument(
        "robot_sn",
        help="Serial number of the robot in Flexiv Elements Studio to connect. Remove any space, e.g. Rizon4s-123456",
    )
    args = argparser.parse_args()

    # Create logger
    logger = spdlog.ConsoleLogger("Example")

    # Print description
    logger.info(
        ">>> Tutorial description <<<\nA simple example demonstrating how to use Flexiv Sim Plugin "
        "in an external simulator.\n"
    )

    # Create user node
    user_node = flexivsimplugin.UserNode(args.robot_sn)

    # Wait for connected
    while not user_node.connected():
        logger.warn("Waiting for connection with Flexiv Elements Studio")
        time.sleep(1)
    logger.info("Connected with Flexiv Elements Studio, starting mocked physics loop")

    # Run mocked physics loop
    while user_node.connected():
        step_physics(user_node, logger)


if __name__ == "__main__":
    main()
