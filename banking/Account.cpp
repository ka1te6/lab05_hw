#include "Account.h"
#include <iostream>
#include <stdexcept>

Account::Account(int id, int balance)
    : id_(id), balance_(balance), is_locked_(false) {}

Account::~Account() {}

int Account::GetBalance() const { return balance_; }

void Account::ChangeBalance(int diff) {
  std::cout << "ChangeBalance called with diff=" << diff << std::endl;
    if (!is_locked_) {
        throw std::runtime_error("account is not locked");
    }
    balance_ += diff;
}

void Account::Lock() {
  if (is_locked_) throw std::runtime_error("already locked");
  is_locked_ = true;
}

void Account::Unlock() { is_locked_ = false; }
