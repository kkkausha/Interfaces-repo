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

package android.hardware.media.c2;

import android.hardware.media.c2.Buffer;

/**
 * An extension of @ref Buffer that also contains a C2Param structure index.
 *
 * This is a part of @ref FrameData.
 */
parcelable InfoBuffer {
    /**
     * A C2Param structure index.
     */
    int index;
    /**
     * Associated @ref Buffer object.
     */
    Buffer buffer;
}
