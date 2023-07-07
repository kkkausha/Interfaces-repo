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

package android.hardware.wifi;

/**
 * RTT Measurement Preamble.
 */
@VintfStability
@Backing(type="int")
enum RttPreamble {
    LEGACY = 0x1,
    HT = 0x2,
    VHT = 0x4,
    /**
     * Preamble type for 11ax
     */
    HE = 0x8,
    /**
     * Preamble type for 11be
     */
    EHT = 0x10,
}
