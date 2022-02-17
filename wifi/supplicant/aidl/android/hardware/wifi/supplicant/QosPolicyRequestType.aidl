/*
 * Copyright (C) 2022 The Android Open Source Project
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

package android.hardware.wifi.supplicant;

/**
 * Enum values for QoS Policy request type.
 */
@VintfStability
@Backing(type="byte")
enum QosPolicyRequestType {
    /**
     * If an Add request includes an existing policy,
     * it should be considered an update request by the handler.
     */
    QOS_POLICY_ADD,
    QOS_POLICY_REMOVE,
}
