/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "brave/browser/payments/payments_service_impl.h"

#include "base/guid.h"
#include "bat/ledger/ledger.h"

namespace payments {

PaymentsServiceImpl::PaymentsServiceImpl() :
    ledger_(ledger::Ledger::CreateInstance(this)) {
}

PaymentsServiceImpl::~PaymentsServiceImpl() {
}

void PaymentsServiceImpl::CreateWallet() {
  ledger_->CreateWallet();
}

std::string PaymentsServiceImpl::GenerateGUID() const {
  return base::GenerateGUID();
}

void PaymentsServiceImpl::Shutdown() {
  ledger_.reset();
  PaymentsService::Shutdown();
}

void PaymentsServiceImpl::OnWalletCreated(ledger::Result result) {
  LOG(ERROR) << "on wallet created " << (result == ledger::Result::OK);
  if (result == ledger::Result::OK)
    TriggerOnWalletCreated();
  else
    TriggerOnWalletCreationFailed(result);
}

void PaymentsServiceImpl::OnReconcileComplete(ledger::Result result,
                                              const std::string& viewing_id) {
  LOG(ERROR) << "reconcile complete " << viewing_id;
}

void PaymentsServiceImpl::LoadLedgerState(
    ledger::LedgerCallbackHandler* handler) {
  LOG(ERROR) << "load ledger state";
  handler->OnLedgerStateLoaded(ledger::Result::ERROR, "");
}

void PaymentsServiceImpl::LoadPublisherState(
    ledger::LedgerCallbackHandler* handler) {
  LOG(ERROR) << "load publisher state";
  handler->OnPublisherStateLoaded(ledger::Result::ERROR, "");
}

void PaymentsServiceImpl::SaveLedgerState(const std::string& ledger_state,
                                      ledger::LedgerCallbackHandler* handler) {
  LOG(ERROR) << "save ledger state";
  handler->OnLedgerStateSaved(ledger::Result::ERROR);
}

void PaymentsServiceImpl::SavePublisherState(const std::string& publisher_state,
                                      ledger::LedgerCallbackHandler* handler) {
  LOG(ERROR) << "save publisher state";
  handler->OnPublisherStateSaved(ledger::Result::ERROR);
}

uint64_t PaymentsServiceImpl::LoadURL(const std::string& url,
                 const std::vector<std::string>& headers,
                 const std::string& content,
                 const std::string& contentType,
                 const ledger::URL_METHOD& method,
                 ledger::LedgerCallbackHandler* handler) {
  LOG(ERROR) << "load url " << url;
  return -1;
  // handler->OnURLRequestResponse(-1, 400, "");
}

void PaymentsServiceImpl::RunIOTask(std::unique_ptr<ledger::LedgerTaskRunner> task) {
  // call task->Run() on IO thread
}

void PaymentsServiceImpl::RunTask(std::unique_ptr<ledger::LedgerTaskRunner> task) {
  // explicitely call task->Run() on UI thread
}

void PaymentsServiceImpl::AddObserver(PaymentsServiceObserver* observer) {
  observers_.AddObserver(observer);
}

void PaymentsServiceImpl::RemoveObserver(PaymentsServiceObserver* observer) {
  observers_.RemoveObserver(observer);
}

void PaymentsServiceImpl::TriggerOnWalletCreated() {
  for (auto& observer : observers_)
    observer.OnWalletCreated(this);
}

void PaymentsServiceImpl::TriggerOnWalletCreationFailed(ledger::Result result) {
  for (auto& observer : observers_)
    observer.OnWalletCreationFailed(this, result);
}

}  // namespace payments
