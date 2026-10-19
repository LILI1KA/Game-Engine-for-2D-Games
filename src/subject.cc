#include <vector>
#include "subject.h"
#include "observer.h"


Subject::Subject(): observers{std::vector<std::shared_ptr<Observer>>()} {}

int Subject::numObservers() const { return observers.size(); }

void Subject::attach(std::shared_ptr<Observer> observer) {
  observers.emplace_back(observer);
}

void Subject::detach(std::shared_ptr<Observer> observer) {
  for (auto it = observers.begin(); it != observers.end(); ++it) {
    if (*it == observer) {
      observers.erase(it);
      break;
    }
  }
}

void Subject::detachAll() { observers.clear(); }

void Subject::notifyObservers() {
  for (auto &ob : observers) ob->notify(*this);
}

void Subject::invertAll_h()  {
  for (auto &ob : observers) ob->invert_h();
}

void Subject::invertAll_v()  {
  for (auto &ob : observers) ob->invert_v();
}
