// SPDX-FileCopyrightText: 2022 Contributors to the Power Grid Model project <dynamic.grid.calculation@alliander.com>
//
// SPDX-License-Identifier: MPL-2.0

#include <power_grid_model/auxiliary/input.hpp>
#include <power_grid_model/auxiliary/serialization/deserializer.hpp>
#include <power_grid_model/auxiliary/update.hpp>

#include <doctest/doctest.h>

constexpr char const* json_single = R"(
{
  "version": "1.0",
  "type": "input",
  "is_batch": false,
  "attributes": {
    "node": [
      "id",
      "u_rated"
    ],
    "sym_load": [
      "id",
      "node",
      "status",
      "type",
      "p_specified",
      "q_specified"
    ],
    "source": [
      "id",
      "node",
      "status",
      "u_ref",
      "sk"
    ]
  },
  "data": {
    "node": [
      [
        1,
        10.5e3
      ],
      [
        2,
        10.5e3
      ],
      [
        3,
        10.5e3
      ]
    ],
    "line": [
      {
        "id": 4,
        "from_node": 1,
        "to_node": 2,
        "from_status": 1,
        "to_status": 1,
        "r1": 0.11,
        "x1": 0.12,
        "c1": 4e-05,
        "tan1": 0.1,
        "i_n": 500.0
      },
      {
        "id": 5,
        "from_node": 2,
        "to_node": 3,
        "from_status": 1,
        "to_status": 1,
        "r1": 0.15,
        "x1": 0.16,
        "c1": 5e-05,
        "tan1": 0.12,
        "i_n": 550.0
      }
    ],
    "source": [
      [
        15,
        1,
        1,
        1.03,
        1e20
      ],
      [
        16,
        1,
        1,
        1.04,
        null
      ],
      {
        "id": 17,
        "node": 1,
        "status": 1,
        "u_ref": 1.03,
        "sk": 1e10,
        "rx_ratio": 0.2
      }
    ],
    "sym_load": [
      [
        7,
        2,
        1,
        0,
        1.01e6,
        0.21e6
      ],
      [
        8,
        3,
        1,
        0,
        1.02e6,
        0.22e6
      ]
    ]
  }
}
)";

namespace power_grid_model::meta_data {

namespace {

inline std::map<std::string, Deserializer::Buffer> get_buffer_map(Deserializer const& deserializer) {
    std::map<std::string, Deserializer::Buffer> map;
    for (Idx i = 0; i != deserializer.n_components(); ++i) {
        auto const& buffer = deserializer.get_buffer_info(i);
        map[buffer.component->name] = buffer;
    }
    return map;
}
} // namespace

TEST_CASE("Deserializer") {

    Deserializer deserializer{};

    SUBCASE("Single dataset") {
        deserializer.deserialize_from_json(json_single);

        SUBCASE("Check meta data") {
            CHECK(deserializer.dataset_name() == "input");
            CHECK(deserializer.batch_size() == 1);
            CHECK(deserializer.n_components() == 4);
        }

        SUBCASE("Check buffer") {
            auto const map = get_buffer_map(deserializer);
            CHECK(map.at("node").elements_per_scenario == 3);
            CHECK(map.at("node").total_elements == 3);
            CHECK(map.at("line").elements_per_scenario == 2);
            CHECK(map.at("line").total_elements == 2);
            CHECK(map.at("source").elements_per_scenario == 3);
            CHECK(map.at("source").total_elements == 3);
            CHECK(map.at("sym_load").elements_per_scenario == 2);
            CHECK(map.at("sym_load").total_elements == 2);
        }

        SUBCASE("Check parse") {
            std::vector<NodeInput> node(3);
            std::vector<LineInput> line(2);
            std::vector<SourceInput> source(3);
            std::vector<SymLoadGenInput> sym_load(2);
            std::array<void*, 4> all_data{node.data(), line.data(), source.data(), sym_load.data()};
            std::array all_components{"node", "line", "source", "sym_load"};
            deserializer.set_buffer(all_components.data(), all_data.data(), nullptr);
            deserializer.parse();
        }
    }
}

} // namespace power_grid_model::meta_data
