#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <memory>

class Observer;

class Subject {
  std::vector<std::shared_ptr<Observer>> observers;
 protected:
  int numObservers() const;
 public:
  Subject();
  virtual ~Subject() = default;
  void attach(std::shared_ptr<Observer> observer);
  void detach(std::shared_ptr<Observer> observer);
  void detachAll();  
  void notifyObservers();
  void invertAll_h();
  void invertAll_v();
};


#endif
