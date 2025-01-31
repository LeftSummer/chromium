/*
 * Copyright (C) 2011, 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2014 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SelectorQuery_h
#define SelectorQuery_h

#include <memory>
#include "core/css/CSSSelectorList.h"
#include "platform/heap/Handle.h"
#include "platform/wtf/HashMap.h"
#include "platform/wtf/Vector.h"
#include "platform/wtf/text/AtomicStringHash.h"

namespace blink {

class CSSSelector;
class ContainerNode;
class Document;
class Element;
class ExceptionState;
template <typename NodeType>
class StaticNodeTypeList;
using StaticElementList = StaticNodeTypeList<Element>;

class CORE_EXPORT SelectorQuery {
  WTF_MAKE_NONCOPYABLE(SelectorQuery);
  USING_FAST_MALLOC(SelectorQuery);

 public:
  static std::unique_ptr<SelectorQuery> Adopt(CSSSelectorList);

  // https://dom.spec.whatwg.org/#dom-element-matches
  bool Matches(Element&) const;

  // https://dom.spec.whatwg.org/#dom-element-closest
  Element* Closest(Element&) const;

  // https://dom.spec.whatwg.org/#dom-parentnode-queryselectorall
  StaticElementList* QueryAll(ContainerNode& root_node) const;

  // https://dom.spec.whatwg.org/#dom-parentnode-queryselector
  Element* QueryFirst(ContainerNode& root_node) const;

  struct QueryStats {
    unsigned total_count;
    unsigned fast_id;
    unsigned fast_class;
    unsigned fast_tag_name;
    unsigned fast_scan;
    unsigned slow_scan;
    unsigned slow_traversing_shadow_tree_scan;
  };
  // Used by unit tests to get information about what paths were taken during
  // the last query. Always reset between queries. This system is disabled in
  // non DCHECK builds to avoid the overhead on the query process.
  static QueryStats LastQueryStats();

 private:
  explicit SelectorQuery(CSSSelectorList);

  bool CanUseFastQuery(const ContainerNode& root_node) const;

  template <typename SelectorQueryTrait>
  void ExecuteWithId(ContainerNode& root_node,
                     typename SelectorQueryTrait::OutputType&) const;
  template <typename SelectorQueryTrait>
  void FindTraverseRootsAndExecute(
      ContainerNode& root_node,
      typename SelectorQueryTrait::OutputType&) const;
  template <typename SelectorQueryTrait>
  void ExecuteForTraverseRoot(ContainerNode& traverse_root,
                              ContainerNode& root_node,
                              typename SelectorQueryTrait::OutputType&) const;
  template <typename SelectorQueryTrait>
  void ExecuteSlow(ContainerNode& root_node,
                   typename SelectorQueryTrait::OutputType&) const;
  template <typename SelectorQueryTrait>
  void ExecuteSlowTraversingShadowTree(
      ContainerNode& root_node,
      typename SelectorQueryTrait::OutputType&) const;
  template <typename SelectorQueryTrait>
  void Execute(ContainerNode& root_node,
               typename SelectorQueryTrait::OutputType&) const;

  bool SelectorListMatches(ContainerNode& root_node, Element&) const;

  CSSSelectorList selector_list_;
  // Contains the list of CSSSelector's to match, but without ones that could
  // never match like pseudo elements, div::before. This can be empty, while
  // m_selectorList will never be empty as SelectorQueryCache::add would have
  // thrown an exception.
  Vector<const CSSSelector*> selectors_;
  AtomicString selector_id_;
  bool selector_id_is_rightmost_ : 1;
  bool selector_id_affected_by_sibling_combinator_ : 1;
  bool uses_deep_combinator_or_shadow_pseudo_ : 1;
  bool needs_updated_distribution_ : 1;
};

class SelectorQueryCache {
  USING_FAST_MALLOC(SelectorQueryCache);

 public:
  SelectorQuery* Add(const AtomicString&, const Document&, ExceptionState&);
  void Invalidate();

 private:
  HashMap<AtomicString, std::unique_ptr<SelectorQuery>> entries_;
};

}  // namespace blink

#endif
