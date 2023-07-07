///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS IMMUTABLE. DO NOT EDIT IN ANY CASE.                          //
///////////////////////////////////////////////////////////////////////////////

// This file is a snapshot of an AIDL file. Do not edit it manually. There are
// two cases:
// 1). this is a frozen version file - do not edit this in any case.
// 2). this is a 'current' file. If you make a backwards compatible change to
//     the interface (from the latest frozen version), the build system will
//     prompt you to update this file with `m <name>-update-api`.
//
// You must not make a backward incompatible change to any AIDL file built
// with the aidl_interface module type with versions property set. The module
// type is used to build AIDL files in a way that they can be used across
// independently updatable components of the system. If a device is shipped
// with such a backward incompatible change, it has a high risk of breaking
// later when a module using the interface is updated, e.g., Mainline modules.

package android.hardware.wifi.supplicant;
@Backing(type="int") @VintfStability
enum QosPolicyScsResponseStatusCode {
  SUCCESS = 0,
  TCLAS_REQUEST_DECLINED = 1,
  TCLAS_NOT_SUPPORTED_BY_AP = 2,
  TCLAS_INSUFFICIENT_RESOURCES = 3,
  TCLAS_RESOURCES_EXHAUSTED = 4,
  TCLAS_PROCESSING_TERMINATED_INSUFFICIENT_QOS = 5,
  TCLAS_PROCESSING_TERMINATED_POLICY_CONFLICT = 6,
  TCLAS_PROCESSING_TERMINATED = 7,
  TIMEOUT = 8,
}
