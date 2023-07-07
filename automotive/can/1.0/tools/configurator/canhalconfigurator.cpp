/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "canbus_config.pb.h"
#include "canprototools.h"

#include <android-base/logging.h>
#include <android/hardware/automotive/can/1.0/ICanController.h>
#include <libcanhaltools/libcanhaltools.h>

#include <chrono>
#include <thread>

namespace android::hardware::automotive::can {

using ICanController = V1_0::ICanController;

/**
 * Takes output from parsed protobuf config and uses it to configure the CAN HAL.
 *
 * \param pb_cfg is an instance of the autogenerated protobuf object for our configuration.
 * \return boolean status, true on success, false on failure.
 */
static bool processPbCfg(const config::CanBusConfig& pb_cfg) {
    for (auto const& bus : pb_cfg.buses()) {
        if (bus.name().empty()) {
            LOG(ERROR) << "Invalid config: Bus config must have a valid name field";
            return false;
        }

        LOG(INFO) << "Configure " << bus.name();
        auto bus_cfg = config::fromPbBus(bus);
        if (!bus_cfg.has_value()) {
            return false;
        }

        // TODO(149405589): remove this sleep and associated includes.
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (libcanhaltools::configureIface(*bus_cfg) != ICanController::Result::OK) {
            LOG(ERROR) << "No controller supports " << bus.name() << std::endl;
            // TODO(149405589): add retry logic in case a bus fails to come up.
            continue;
        }
        LOG(INFO) << bus.name() << " has been successfully configured!";
    }
    return true;
}

/**
 * This kicks off the CAN HAL configuration process. This starts the following:
 *     1. Reading the config file
 *     2. Setting up CAN buses
 *     3. Handling services
 * \param filepath is a string specifying the absolute path of the config file
 * \return boolean status, true on success, false on failure
 */
static bool configuratorStart(const std::string& filepath) {
    base::SetDefaultTag("CanConfigurator");

    LOG(WARNING) << "The HIDL version of CAN HAL has been deprecated, if this tool fails with "
                 << "SIGABRT, you may need canhalconfigurator-aidl instead.";

    auto pb_cfg = config::parseConfigFile(filepath);
    if (!pb_cfg.has_value()) {
        return false;
    }

    // process the rest of the config file data and configure the CAN buses.
    if (!processPbCfg(*pb_cfg)) {
        return false;
    }
    LOG(INFO) << "CAN HAL has been configured!";
    return true;
}

}  // namespace android::hardware::automotive::can

int main(int argc, char* argv[]) {
    std::string config_filepath = "/etc/canbus_config.pb";

    // allow for CLI specification of a config file.
    if (argc == 2) {
        config_filepath = argv[1];
    } else if (argc > 2) {
        std::cerr << "usage: " << argv[0] << " [optional config filepath]";
        return 1;
    }

    if (!::android::hardware::automotive::can::configuratorStart(config_filepath)) {
        return 1;
    }
    return 0;
}
