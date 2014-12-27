// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_PROTOCOL_THIRD_PARTY_AUTHENTICATOR_BASE_H_
#define REMOTING_PROTOCOL_THIRD_PARTY_AUTHENTICATOR_BASE_H_

#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/protocol/authenticator.h"
#include "third_party/libjingle/source/talk/xmllite/qname.h"

namespace buzz {

class XmlElement;

}  // namespace buzz

namespace remoting {
namespace protocol {

// Implements an authentication method that relies on a third party server for
// authentication of both client and host.
// When third party authentication is being used, the client must request both a
// token and a shared secret from a third-party server (which may require the
// user to authenticate themselves). The client then sends only the token to the
// host. The host signs the token, then contacts the third-party server to
// exchange the token for the shared secret. Once both client and host have the
// shared secret, they use an underlying |V2Authenticator| (SPAKE2) to negotiate
// an authentication key, which is used to establish the connection.
class ThirdPartyAuthenticatorBase : public Authenticator {
 public:
  virtual ~ThirdPartyAuthenticatorBase();

  // Authenticator interface.
  virtual State state() const OVERRIDE;
  virtual bool started() const OVERRIDE;
  virtual RejectionReason rejection_reason() const OVERRIDE;
  virtual void ProcessMessage(const buzz::XmlElement* message,
                              const base::Closure& resume_callback) OVERRIDE;
  virtual scoped_ptr<buzz::XmlElement> GetNextMessage() OVERRIDE;
  virtual scoped_ptr<ChannelAuthenticator>
      CreateChannelAuthenticator() const OVERRIDE;

 protected:
  // XML tag names for third party authentication fields.
  static const buzz::StaticQName kTokenUrlTag;
  static const buzz::StaticQName kTokenScopeTag;
  static const buzz::StaticQName kTokenTag;

  explicit ThirdPartyAuthenticatorBase(State initial_state);

  // Gives the message to the underlying authenticator for processing.
  void ProcessUnderlyingMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback);

  // Processes the token-related elements of the message.
  virtual void ProcessTokenMessage(
      const buzz::XmlElement* message,
      const base::Closure& resume_callback) = 0;

  // Adds the token related XML elements to the message.
  virtual void AddTokenElements(buzz::XmlElement* message) = 0;

  scoped_ptr<Authenticator> underlying_;
  State token_state_;
  bool started_;
  RejectionReason rejection_reason_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThirdPartyAuthenticatorBase);
};

}  // namespace protocol
}  // namespace remoting

#endif  // REMOTING_PROTOCOL_THIRD_PARTY_AUTHENTICATOR_BASE_H_