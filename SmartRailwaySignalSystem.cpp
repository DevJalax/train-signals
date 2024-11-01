#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>

class Train {
public:
    int id;
    int platform;
    std::vector<int> route;
    int location;
    bool delayed;

    Train(int id, int platform, std::vector<int> route)
        : id(id), platform(platform), route(route), location(platform), delayed(false) {}

    void updateLocation(int new_location) {
        location = new_location;
        std::cout << "Train " << id << " moved to location: " << location << std::endl;
    }

    void setDelayed(bool status) {
        delayed = status;
        std::cout << "Train " << id << " delay status set to: " << delayed << std::endl;
    }
};

class Track {
public:
    int id;
    bool is_operational;

    Track(int id) : id(id), is_operational(true) {}

    void breakTrack() {
        is_operational = false;
        std::cout << "Track " << id << " is broken and not operational" << std::endl;
    }

    void repairTrack() {
        is_operational = true;
        std::cout << "Track " << id << " is repaired and operational" << std::endl;
    }
};

class SignalSystem {
private:
    std::vector<Train*> trains;
    std::unordered_map<int, Track*> tracks;

public:
    void addTrain(Train* train) {
        trains.push_back(train);
    }

    void addTrack(Track* track) {
        tracks[track->id] = track;
    }

    void assignRoute(int train_id, std::vector<int> route) {
        for (auto& train : trains) {
            if (train->id == train_id) {
                train->route = route;
                std::cout << "Assigned route to train " << train_id << std::endl;
                return;
            }
        }
    }

    void updateTrainStatus(Train* train) {
        if (std::find(train->route.begin(), train->route.end(), train->location) == train->route.end()) {
            std::cout << "Collision prevention: Train " << train->id << " is on the wrong route!" << std::endl;
            applyBrakes(train);
            return;
        }

        int next_track = getNextTrack(train);
        if (next_track != -1 && !tracks[next_track]->is_operational) {
            std::cout << "Track " << next_track << " ahead of Train " << train->id << " is broken. Signal is not given." << std::endl;
            holdTrain(train);
            return;
        }

        if (train->delayed) {
            updateSignalsForDelay(train);
        } else {
            std::cout << "Train " << train->id << " is on track " << train->location << " and moving normally." << std::endl;
        }
    }

    void applyBrakes(Train* train) {
        std::cout << "Applying emergency brakes to Train " << train->id << " due to route error." << std::endl;
    }

    void holdTrain(Train* train) {
        std::cout << "Holding Train " << train->id << " at location " << train->location << " until track is clear." << std::endl;
    }

    void updateSignalsForDelay(Train* delayed_train) {
        for (auto& train : trains) {
            if (train->location == delayed_train->location && train->id != delayed_train->id) {
                std::cout << "Train " << train->id << " rerouted to avoid collision with delayed Train " << delayed_train->id << std::endl;
            }
        }
    }

    int getNextTrack(Train* train) {
        auto it = std::find(train->route.begin(), train->route.end(), train->location);
        if (it != train->route.end() && (it + 1) != train->route.end()) {
            return *(it + 1);
        }
        return -1;
    }
};

int main() {
    SignalSystem signalSystem;

    // Create Tracks
    for (int i = 1; i <= 5; ++i) {
        Track* track = new Track(i);
        signalSystem.addTrack(track);
    }

    // Create Trains with routes and platforms
    Train* train1 = new Train(1, 1, {1, 2, 3});
    Train* train2 = new Train(2, 2, {2, 3, 4});

    signalSystem.addTrain(train1);
    signalSystem.addTrain(train2);

    // Assign routes to trains
    signalSystem.assignRoute(train1->id, {1, 2, 3});
    signalSystem.assignRoute(train2->id, {2, 3, 4});

    // Simulate Train Movements
    for (int i = 0; i < 5; ++i) {
        // Randomly break or repair a track
        if (rand() % 5 == 0) {
            int track_id = rand() % 5 + 1;
            signalSystem.tracks[track_id]->breakTrack();
        } else if (rand() % 5 == 1) {
            int track_id = rand() % 5 + 1;
            signalSystem.tracks[track_id]->repairTrack();
        }

        // Update Train Locations
        train1->updateLocation(rand() % 3 + 1);
        train2->updateLocation(rand() % 3 + 2);

        // Check each train's status
        signalSystem.updateTrainStatus(train1);
        signalSystem.updateTrainStatus(train2);

        // Simulate time delay
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Clean up dynamically allocated memory
    delete train1;
    delete train2;
    for (auto& track : signalSystem.tracks) {
        delete track.second;
    }

    return 0;
}
