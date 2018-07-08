/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_PAYMENTS_PAYMENTS_SERVICE_IMPL_
#define BRAVE_BROWSER_PAYMENTS_PAYMENTS_SERVICE_IMPL_

#include <memory>

#include "base/observer_list.h"
#include "bat/ledger/ledger_client.h"
#include "brave/browser/payments/payments_service.h"
#include "brave/browser/payments/payments_service_observer.h"

namespace ledger {
class Ledger;
class LedgerCallbackHandler;
}

namespace payments {

class PaymentsServiceImpl : public PaymentsService,
                            public ledger::LedgerClient {
 public:
  PaymentsServiceImpl();
  ~PaymentsServiceImpl() override;

  // KeyedService:
  void Shutdown() override;

  void CreateWallet() override;

  void AddObserver(PaymentsServiceObserver* observer);
  void RemoveObserver(PaymentsServiceObserver* observer);

 private:
  // ledger::LedgerClient
  std::string GenerateGUID() const override;
  void OnWalletCreated(ledger::Result result) override;
  void OnReconcileComplete(ledger::Result result,
                           const std::string& viewing_id) override;
  void LoadLedgerState(ledger::LedgerCallbackHandler* handler) override;
  void LoadPublisherState(ledger::LedgerCallbackHandler* handler) override;
  void SaveLedgerState(const std::string& ledger_state,
                       ledger::LedgerCallbackHandler* handler) override;
  void SavePublisherState(const std::string& publisher_state,
                          ledger::LedgerCallbackHandler* handler) override;
  uint64_t LoadURL(const std::string& url,
                   const std::vector<std::string>& headers,
                   const std::string& content,
                   const std::string& contentType,
                   const ledger::URL_METHOD& method,
                   ledger::LedgerCallbackHandler* handler) override;
  void RunIOTask(std::unique_ptr<ledger::LedgerTaskRunner> task) override;
  void RunTask(std::unique_ptr<ledger::LedgerTaskRunner> task) override;

  void TriggerOnWalletCreated();
  void TriggerOnWalletCreationFailed(ledger::Result result);

  std::unique_ptr<ledger::Ledger> ledger_;
  base::ObserverList<PaymentsServiceObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(PaymentsServiceImpl);
};

}  // namespace history

#endif  // BRAVE_BROWSER_PAYMENTS_PAYMENTS_SERVICE_IMPL_
