/*
 * Copyright (C) 2008 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LinkHash_h
#define LinkHash_h

#include "platform/PlatformExport.h"
#include "platform/wtf/Allocator.h"
#include "platform/wtf/Forward.h"
#include "platform/wtf/text/StringHash.h"

namespace blink {

class KURL;

typedef uint64_t LinkHash;

// Use the low 32-bits of the 64-bit LinkHash as the key for HashSets.
struct LinkHashHash {
  STATIC_ONLY(LinkHashHash);
  static unsigned GetHash(LinkHash key) { return static_cast<unsigned>(key); }
  static bool Equal(LinkHash a, LinkHash b) { return a == b; }
  static const bool safe_to_compare_to_empty_or_deleted = true;

  // See AlreadyHashed::avoidDeletedValue.
  static unsigned AvoidDeletedValue(LinkHash hash64) {
    DCHECK(hash64);
    unsigned hash = static_cast<unsigned>(hash64);
    unsigned new_hash = hash | (!(hash + 1) << 31);
    DCHECK(new_hash);
    ASSERT(new_hash != 0xFFFFFFFF);
    return new_hash;
  }
};

// Resolves the potentially relative URL "attributeURL" relative to the given
// base URL, and returns the hash of the string that will be used for visited
// link coloring. It will return the special value of 0 if attributeURL does not
// look like a relative URL.
PLATFORM_EXPORT LinkHash VisitedLinkHash(const KURL& base,
                                         const AtomicString& attribute_url);

}  // namespace blink

#endif  // LinkHash_h
