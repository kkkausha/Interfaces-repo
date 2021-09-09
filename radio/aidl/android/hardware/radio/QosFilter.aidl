/*
 * Copyright (C) 2021 The Android Open Source Project
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

package android.hardware.radio;

import android.hardware.radio.MaybePort;
import android.hardware.radio.QosFilterDirection;
import android.hardware.radio.QosFilterIpsecSpi;
import android.hardware.radio.QosFilterIpv6FlowLabel;
import android.hardware.radio.QosFilterTypeOfService;
import android.hardware.radio.QosProtocol;

/**
 * See 3gpp 24.008 10.5.6.12 and 3gpp 24.501 9.11.4.13
 */
@VintfStability
parcelable QosFilter {
    /**
     * Local and remote IP addresses, typically one IPv4 or one IPv6 or one of each. Addresses could
     * be with optional "/" prefix length, e.g.,"192.0.1.3" or "192.0.1.11/16 2001:db8::1/64".
     * If the prefix length is absent the addresses are assumed to be point to point with IPv4
     * having a prefix length of 32 and IPv6 128.
     */
    String[] localAddresses;
    String[] remoteAddresses;
    /**
     * Local and remote port/ranges
     */
    MaybePort localPort;
    MaybePort remotePort;
    /**
     * QoS protocol
     */
    QosProtocol protocol;
    /**
     * Type of service value or mask as defined in RFC 1349
     */
    QosFilterTypeOfService tos;
    /**
     * IPv6 flow label as defined in RFC 6437
     */
    QosFilterIpv6FlowLabel flowLabel;
    /**
     * IPSec security parameter index
     */
    QosFilterIpsecSpi spi;
    /**
     * Filter direction
     */
    QosFilterDirection direction;
    /**
     * Specifies the order in which the filter needs to be matched. A lower numerical (positive)
     * value has a higher precedence. Set -1 when unspecified.
     */
    int precedence;
}
