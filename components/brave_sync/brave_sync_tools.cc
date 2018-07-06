/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <string>
#include <sstream>
#include "crypto/random.h"
#include "base/strings/string_util.h"

namespace bravesync {

namespace tools {

std::string GenerateObjectId() {
/*
private String GenerateObjectId(String localId) {
    String res = GetObjectId(localId);
    if (0 != res.length()) {
        return res;
    }
    // Generates 16 random 8 bits numbers
    Random random = new Random();
    for (int i = 0; i < 16; i++) {
        if (i != 0) {
            res += ", ";
        }
        try {
            res += String.valueOf(random.nextInt(256));
        } catch (IllegalArgumentException exc) {
            res = "";
            Log.e(TAG, "ObjectId generation exception " + exc);
        }
    }

    return res;
}
*/

  //16 random 8-bit unsigned numbers
  const size_t length = 16;
  uint8_t bytes[length];
  crypto::RandBytes(bytes, sizeof(bytes));
  std::stringstream ss;
  for (size_t i = 0; i < length; ++i) {
    const uint8_t &byte = bytes[i];
    ss << std::dec << (int)byte;
    if (i != length - 1) {
      ss << ", ";
    }
  }
LOG(ERROR) << "TAGAB GenerateObjectId ss.str()="<<ss.str();
  return ss.str();
}

std::string replaceUnsupportedCharacters(const std::string &in) {
/*
private String replaceUnsupportedCharacters(String in) {
  return in.replace("\\", "\\\\").replace("\"", "\\\"");
}
*/
  std::string result;
  base::ReplaceChars(in, "\\", "\\\\", &result);
  base::ReplaceChars(result, "\"", "\\\"", &result);

  return result;
}

} // namespace tools

} // namespace bravesync
