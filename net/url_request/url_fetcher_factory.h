// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_FETCHER_FACTORY_H_
#define NET_URL_REQUEST_URL_FETCHER_FACTORY_H_
#pragma once

#include "net/base/net_export.h"
#include "net/url_request/url_fetcher.h"

namespace net {
class URLFetcherDelegate;

// URLFetcher::Create uses the currently registered Factory to create the
// URLFetcher. Factory is intended for testing.
class NET_EXPORT URLFetcherFactory {
 public:
  virtual URLFetcher* CreateURLFetcher(
      int id,
      const GURL& url,
      URLFetcher::RequestType request_type,
      URLFetcherDelegate* d) = 0;

 protected:
  virtual ~URLFetcherFactory();
};

}  // namespace net

#endif  // NET_URL_REQUEST_URL_FETCHER_FACTORY_H_
