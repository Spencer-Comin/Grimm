//
// Created by Spencer Comin on 2021-11-15.
//

#ifndef GRIMM_GRIMM_MAP_H
#define GRIMM_GRIMM_MAP_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

using std::string;
using std::vector;
using std::unordered_map;

using segment_id = std::pair<int, int>;


class GrimmMap {
    struct story {
	string title;
	int segments;
	int progress;
    };
    unordered_map<int, story> stories;
    vector<string> locations;
    vector<string> start_locations;
    unordered_map<string, unordered_map<string, vector<segment_id>>> location_map {};  // here : there : [segment ids]
    unordered_map<string, string> story_segments {}; // encoded segment id : segment text
    string current_location = "start";

    std::optional<segment_id> choose_segment_id(const vector<segment_id>& segment_options);

public:
    explicit GrimmMap(string json_file_path);
    ~GrimmMap() = default;

    vector<string> go_to(const string& location);
    vector<string> first_move();
    bool is_done();

};


#endif //GRIMM_GRIMM_MAP_H
