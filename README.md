## Explanation of C++ Code

# Classes:

Train: Each train has an ID, platform, route, current location, and delay status.

Track: Each track has an ID and an operational status (whether it’s broken or operational).

SignalSystem: Manages trains, tracks, and signaling logic. It includes methods for assigning routes, updating train status, applying brakes, and rerouting if necessary.
Simulation:

# Loop: The code runs in a loop where:

- Tracks randomly break or repair.

- Train locations are updated.

- Signal system checks each train's status, applying brakes or holding trains if necessary.

 The loop simulates the real-time decision-making needed in a signaling system.

# Memory Management:

Dynamically allocated Train and Track objects are deleted at the end to prevent memory leaks.
