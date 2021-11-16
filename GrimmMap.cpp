//
// Created by Spencer Comin on 2021-11-15.
//

#include "GrimmMap.h"
#include <boost/json.hpp>
#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>
#include <thread>

static boost::json::object parse_map(string &file_path) {
    boost::json::stream_parser parser;
    boost::json::error_code ec;
    string line;
    std::fstream file(file_path);

    while (std::getline(file, line)) {
	parser.write(line, ec);
	// std::cerr << line << '\n';
    }

    if (ec)
	std::cerr << "Parsing the JSON map failed: " << ec.message() << std::endl;

    assert(parser.done());

    return parser.release().as_object();
}

static segment_id string_to_segment_id(const string &segment) {
    size_t pos = segment.find('.');
    int story_number = std::stoi(segment.substr(0, pos));
    int segment_number = std::stoi(segment.substr(pos+1, segment.length()));
    return {story_number, segment_number};
}

static inline string segment_id_to_string(segment_id id) {
    return std::to_string(id.first) + "." + std::to_string(id.second);
}

GrimmMap::GrimmMap(string json_file_path) {
    boost::json::object map_json = parse_map(json_file_path);
    for (auto &story: map_json["stories"].as_array()) {
	auto story_obj = story.as_object();
	GrimmMap::story story_info{
	    .title = story_obj["name"].as_string().data(),
	    .segments = static_cast<int>(story_obj["segments"].as_int64()),
	    .progress = 0
	};
	stories[story_obj["number"].as_int64()] = story_info;
    }
    for (auto &location: map_json["start locations"].as_array()) {
	start_locations.emplace_back(location.as_string().data());
    }
    for (auto &location_info: map_json["map"].as_object()) {
	string here = location_info.key().data();
	locations.emplace_back(here);
	for (auto &destination_info: location_info.value().as_object()) {
	    string there = destination_info.key().data();
	    for (auto &story: destination_info.value().as_array()) {
		string story_title = story.as_string().data();
		segment_id id = string_to_segment_id(story_title);
		location_map[here][there].emplace_back(id);
		story_segments[story_title] = "";
	    }
	}
    }

    for (auto &segment: story_segments) {
	// (segment label, segment text)
	if (segment.first == "0.0")
	    continue;
	std::fstream file("fairytales/" + segment.first + ".txt");
	segment.second = std::string(std::istreambuf_iterator<char>(file), {});
    }
}

static void slow_print(const string &text) {
    for (const char &c: text) {
	std::cout << c;
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	std::cout.flush();
    }
}


vector<string> GrimmMap::go_to(const string& next_location) {
    vector<segment_id> segments = location_map[current_location][next_location];
    current_location = next_location;
    // choose segment id from segments
    std::optional<segment_id> id = choose_segment_id(segments);
    // if one chosen, print out story segment
    if (id.has_value()) {
	slow_print(story_segments[segment_id_to_string(id.value())]);
	std::cout << "\n\n" << stories[id->first].title
		  << " part " << std::to_string(id->second)
		  << " of " << std::to_string(stories[id->first].segments)
		  << std::endl;
    }
    // return locations reachable from new current location
    vector<string> reachable_locations{};
    for (auto &location: location_map[current_location]) {
	reachable_locations.emplace_back(location.first);
    }
    return reachable_locations;
}

std::optional<segment_id>
GrimmMap::choose_segment_id(const vector<segment_id>& segment_options) {
    for (auto &id: segment_options) {
	int story_id = id.first;
	int segment = id.second;
	auto story_info = &stories[story_id];
	if (segment == story_info->progress + 1) {
	    story_info->progress++;
	    return id;
	}
    }
    return {};
}

vector<string> GrimmMap::first_move() {
    return start_locations;
}

bool GrimmMap::is_done() {
    for (auto &story: stories) {
	if (story.second.progress != story.second.segments)
	    return false;
    }
    return true;
}
